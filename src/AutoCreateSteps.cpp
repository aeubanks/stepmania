#include "global.h"
#include "AutoCreateSteps.h"
#include "RageLog.h"
#include "RageMath.hpp"
#include "RageUtil.h"
#include "PanelCoord.h"

#include <array>
#include <utility>
#include <vector>

namespace AutoCreateSteps {
namespace {

template<typename T, typename S>
bool Contains(const T& t, const S& s)
{
    return std::find(t.begin(), t.end(), s) != t.end();
}

std::pair<PanelCoord, PanelCoord> startCoordsForStepsType(StepsType stepstype)
{
    switch (stepstype)
    {
        case StepsType_dance_single:
        case StepsType_horizon_single:
            return {{0.0f, 1.0f}, {2.0f, 1.0f}};
        case StepsType_dance_threepanel:
            return {{0.0f, 2.0f}, {2.0f, 2.0f}};
        case StepsType_dance_double:
        case StepsType_horizon_double:
            return {{2.0f, 1.0f}, {3.0f, 1.0f}};
        case StepsType_pump_single:
            return {{0.0f, 0.0f}, {2.0f, 0.0f}};
        case StepsType_pump_double:
            return {{2.0f, 0.0f}, {3.0f, 0.0f}};
        DEFAULT_FAIL(stepstype);
    }
}

class AutoCreate
{
private:
    PanelCoord left, right;
    bool curLeft;
    float curAngle;
    float originalAngle;
    int leftRepeatCount, rightRepeatCount;

    PanelCoord& getCurPanel()
    {
        return curLeft ? left : right;
    }

    const PanelCoord& getCurPanel() const
    {
        return curLeft ? left : right;
    }

    PanelCoord& getOtherPanel()
    {
        return curLeft ? right : left;
    }

    const PanelCoord& getOtherPanel() const
    {
        return curLeft ? right : left;
    }

    int& getCurRepeatCount()
    {
        return curLeft ? leftRepeatCount : rightRepeatCount;
    }

    const int& getCurRepeatCount() const
    {
        return curLeft ? leftRepeatCount : rightRepeatCount;
    }

public:
    AutoCreate(StepsType stepstype)
    {
        auto start = startCoordsForStepsType(stepstype);
        left = start.first;
        right = start.second;
        curLeft = RandomBool();
        curAngle = angle(left, right);
        originalAngle = curAngle;
        leftRepeatCount = 0;
        rightRepeatCount = 0;
    }

    PanelCoord getLeft() {
        return left;
    }

    PanelCoord getRight() {
        return right;
    }

    float getCurAngle() {
        return curAngle;
    }

    float getOriginalAngle() {
        return originalAngle;
    }

    void toggleSide()
    {
        curLeft = !curLeft;
    }

    int getSide()
    {
        return curLeft ? 1 : 0;
    }

    int getOtherSide()
    {
        return curLeft ? 0 : 1;
    }

    std::pair<PanelCoord, PanelCoord> testCoords(PanelCoord next) const
    {
        if (curLeft)
        {
            return {next, right};
        }
        else
        {
            return {left, next};
        }
    }

    static float dist(PanelCoord a, PanelCoord b)
    {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    float testDist(PanelCoord next) const
    {
        auto coords = testCoords(next);
        return dist(coords.first, coords.second);
    }

    static float angle(PanelCoord a, PanelCoord b)
    {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return std::atan2(dy, dx);
    }

    float testAngle(PanelCoord next) const
    {
        auto coords = testCoords(next);
        float testAngle = angle(coords.first, coords.second); // -135
        testAngle -= Quantize(testAngle - curAngle, M_PI * 2); // Quantize(-135 - 180, 360)
        return testAngle;
    }

    void step(PanelCoord next)
    {
        // need to randomize when 180 deg
        float newAngle = testAngle(next);
        if (getCurPanel() == next)
        {
            ++getCurRepeatCount();
        }
        else
        {
            getCurRepeatCount() = 1;
        }
        getCurPanel() = next;
        curAngle = newAngle;
    }

    float scaleRepetitive(PanelCoord next, int repetitiveMax, float decay) const
    {
        if (next == getCurPanel())
        {
            int i = getCurRepeatCount() - repetitiveMax;
            if (i > 0) {
                return std::pow(decay, i);
            }
        }
        return 1.0;
    }

    bool testWithinDistanceTwoFeet(PanelCoord next, float maxDist) const
    {
        return testDist(next) <= maxDist + 0.001f;
    }

    float scaleDistanceTwoFeet(PanelCoord next, float maxDist, float decay) const
    {
        float dist = testDist(next);
        if (dist > maxDist)
        {
            return std::pow(decay, dist - maxDist);
        }
        return 1.0;
    }

    bool testWithinDistanceSameFoot(PanelCoord next, float maxDist) const
    {
        return dist(next, getCurPanel()) <= maxDist + 0.001f;
    }

    float scaleDistanceSameFoot(PanelCoord next, float maxDist, float decay) const
    {
        float d = dist(next, getCurPanel());
        if (d > maxDist)
        {
            return std::pow(decay, d - maxDist);
        }
        return 1.0;
    }

    bool testWithinAngle(PanelCoord next, float maxTurnAngle) const
    {
        if (next == getOtherPanel())
        {
            return false;
        }

        float angle = testAngle(next);
        float delta = std::abs(angle - originalAngle);
        return delta <= maxTurnAngle + 0.001f;
    }

    bool testWithinDeltaAngle(PanelCoord next, float maxDeltaTurnAngle) const
    {
        if (next == getOtherPanel())
        {
            return false;
        }

        float angle = testAngle(next);
        float delta = std::abs(angle - curAngle);
        return delta <= maxDeltaTurnAngle + 0.001f;
    }

    float scaleAngle(PanelCoord next, float maxTurnAngle, float decay) const
    {
        float angle = testAngle(next);
        float delta = std::abs(angle - originalAngle);
        if (delta > maxTurnAngle)
        {
            return std::pow(decay, delta - maxTurnAngle);
        }
        return 1.0;
    }
};

std::vector<int> GetPossibleNextSteps(const std::vector<PanelCoord>& panelCoords, const AutoCreate& creator, const AutoCreateParameters& params )
{
    std::vector<int> possibleSteps;
    for (int track = 0; track < panelCoords.size(); ++track)
    {
        PanelCoord curCoord = panelCoords[track];
        bool maxAngleTest = creator.testWithinAngle(curCoord, Rage::DegreesToRadians(params.maxTurnDegree));
        bool maxDeltaAngleTest = creator.testWithinDeltaAngle(curCoord, Rage::DegreesToRadians(params.maxDeltaTurnDegree));
        bool distTwoFeetTest = creator.testWithinDistanceTwoFeet(curCoord, params.maxDistBetweenTwoFeet);
        bool distSameFootTest = creator.testWithinDistanceSameFoot(curCoord, params.maxDistBetweenSameFoot);
        if (maxAngleTest && maxDeltaAngleTest && distTwoFeetTest && distSameFootTest)
        {
            float possibleAngle = creator.testAngle(curCoord);
            LOG->Trace("Auto creating considering track %d with angle %f", track, possibleAngle);
            possibleSteps.push_back(track);
        }
        else
        {
            float possibleAngle = creator.testAngle(curCoord);
            LOG->Trace("Auto creating skipping track %d with angle %f (%d, %d, %d, %d)", track, possibleAngle, maxAngleTest, maxDeltaAngleTest, distTwoFeetTest, distSameFootTest);
        }
    }
    return possibleSteps;
}

std::vector<std::pair<int, float>> GetNextStepsProbabilities( const std::vector<PanelCoord> panelCoords, const AutoCreate& creator, const AutoCreateParameters& params, const std::vector<int>& possibleNextSteps )
{
    std::vector<std::pair<int, float>> ret;
    ret.reserve(possibleNextSteps.size());

    for (auto possibleNextStep : possibleNextSteps)
    {
        float prob = 1.0;
        prob *= creator.scaleRepetitive(panelCoords[possibleNextStep], params.uncomfortableRepetitions, params.uncomfortableRepetitionsDecay);
        prob *= creator.scaleDistanceSameFoot(panelCoords[possibleNextStep], params.uncomfortableDistBetweenSameFoot, params.uncomfortableDistBetweenSameFootDecay);
        prob *= creator.scaleDistanceTwoFeet(panelCoords[possibleNextStep], params.uncomfortableDistBetweenTwoFeet, params.uncomfortableDistBetweenTwoFeetDecay);
        prob *= creator.scaleAngle(panelCoords[possibleNextStep], Rage::DegreesToRadians(params.uncomfortableTurnDegree), params.uncomfortableTurnDegreeDecay);
        ret.emplace_back(possibleNextStep, prob);
    }

    return ret;
}

int ChooseRandomNextStep(const std::vector<std::pair<int, float>>& possibleNextStepsAndProbs)
{
    int size = possibleNextStepsAndProbs.size();
    float sum = 0.0;
    for (const auto& pair : possibleNextStepsAndProbs)
    {
        sum += pair.second;
    }

    float rand = RandomFloat(0.0, sum);

    int idx = 0;
    for (; idx < size; ++idx)
    {
        float curProb = possibleNextStepsAndProbs[idx].second;
        if (rand < curProb)
        {
            break;
        }
        rand -= curProb;
    }
    idx = std::min(idx, size - 1);
    return possibleNextStepsAndProbs[idx].first;
}

int GetNextStep(const std::vector<PanelCoord>& panelCoords, const AutoCreate& creator, const AutoCreateParameters& params )
{
    auto possibleSteps = GetPossibleNextSteps(panelCoords, creator, params);
    if (possibleSteps.empty())
    {
        LOG->Trace("Auto creating couldn't find suitable steps");
        return -1;
    }
    LOG->Trace("Auto creating choosing from %d possible steps", possibleSteps.size());
    auto possibleStepsAndProbs = GetNextStepsProbabilities(panelCoords, creator, params, possibleSteps);
    int ret = ChooseRandomNextStep(possibleStepsAndProbs);
    return ret;
}

}

void AutoCreateSteps( NoteData &inout, StepsType stepstype, const AutoCreateParameters& params, int iStartIndex, int iEndIndex )
{
    const std::vector<PanelCoord> panelCoords = panelCoordsForStepsType(stepstype);

    inout.ClearRange(iStartIndex, iEndIndex);

    int space = params.noteSpace;

    AutoCreate creator(stepstype);

        LOG->Trace("Auto creating steps with space %d, original angle %f", space, creator.getOriginalAngle());

    for (int row = iStartIndex; row < iEndIndex; row += space)
    {
        LOG->Trace("cur state: l (%f, %f), r (%f, %f), angle %f", creator.getLeft().x, creator.getLeft().y, creator.getRight().x, creator.getRight().y, creator.getCurAngle());
        int nextStep = GetNextStep(panelCoords, creator, params);
        if (nextStep < 0)
        {
            LOG->Trace("Auto creating couldn't create steps");
            // couldn't generate steps, break
            break;
        }
        inout.SetTapNote(nextStep, row, TAP_ORIGINAL_TAP);
        creator.step(panelCoords[nextStep]);
        creator.toggleSide();
        LOG->Trace("Auto creating added step to track %d", nextStep);
    }
}

void AutoCreateSteps( NoteData &inout, StepsType stepstype, const AutoCreateParameters& params, const NoteData& copyRhythmFrom )
{
    const std::vector<PanelCoord> panelCoords = panelCoordsForStepsType(stepstype);

    LOG->Trace("Auto creating steps using existing rhythm");

    bool done = false;

    AutoCreate creator(stepstype);
    FOREACH_NONEMPTY_ROW_ALL_TRACKS(copyRhythmFrom, row)
	{
        if (done)
        {
            break;
        }
        int numNotesToGenerate = 0;
        for (int track = 0; track < copyRhythmFrom.GetNumTracks(); ++track)
        {
            const TapNote& note = copyRhythmFrom.GetTapNote(track, row);
            switch (note.type)
            {
                case TapNoteType_Tap:
                case TapNoteType_Lift:
                case TapNoteType_HoldHead:
                {
                    ++numNotesToGenerate;
                }
            }
        }
        if (numNotesToGenerate > 1) {
            if (RandomBool()) {
                creator.toggleSide();
            }
        }
        for (int i = 0 ; i < numNotesToGenerate; ++i)
        {
            int nextStep = GetNextStep(panelCoords, creator, params);
            if (nextStep < 0)
            {
                LOG->Trace("Auto creating couldn't create steps");
                done = true;
                break;
            }
            inout.SetTapNote(nextStep, row, TAP_ORIGINAL_TAP);
            creator.step(panelCoords[nextStep]);
            creator.toggleSide();
        }
	}
}

}