#ifndef AUTO_CREATE_STEPS_H
#define AUTO_CREATE_STEPS_H

#include "NoteData.h"
#include "NoteTypes.h"

namespace AutoCreateSteps {

struct AutoCreateParameters {
    // can't block a step due to repetition because you can get into a corner with no way out.
    // should only decrease probability.
    int uncomfortableRepetitions;
    float uncomfortableRepetitionsDecay;

    float maxTurnDegree;
    float uncomfortableTurnDegree;
    float uncomfortableTurnDegreeDecay;

    float maxDeltaTurnDegree;
    float uncomfortableDeltaTurnDegree;
    float uncomfortableDeltaTurnDegreeDecay;

    float maxDistBetweenSameFoot;
    float uncomfortableDistBetweenSameFoot;
    float uncomfortableDistBetweenSameFootDecay;

    float maxDistBetweenTwoFeet;
    float uncomfortableDistBetweenTwoFeet;
    float uncomfortableDistBetweenTwoFeetDecay;
};

enum AutoCreatePattern {
    autocreate_pattern_1,
    autocreate_pattern_11222,
    autocreate_pattern_112,
    autocreate_pattern_11114,
    autocreate_pattern_111122,
    autocreate_pattern_1111112,
};

void AutoCreateSteps( NoteData &inout, StepsType stepstype, const AutoCreateParameters& params, int iStartIndex, int iEndIndex, int noteSpacing, AutoCreatePattern pattern );
void AutoCreateSteps( NoteData &inout, StepsType stepstype, const AutoCreateParameters& params, const NoteData& copyRhythmFrom );

}

#endif