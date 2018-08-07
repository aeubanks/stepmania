#include "PanelCoord.h"

bool operator==(PanelCoord a, PanelCoord b)
{
    return a.x == b.x && a.y == b.y;
}

std::vector<PanelCoord> panelCoordsForStepsType(StepsType stepstype)
{
    switch (stepstype)
    {
        case StepsType_dance_single:
            return {
                {0.0f, 1.0f},
                {1.0f, 0.0f},
                {1.0f, 2.0f},
                {2.0f, 1.0f},
            };
        case StepsType_dance_double:
            return {
                {0.0f, 1.0f},
                {1.0f, 0.0f},
                {1.0f, 2.0f},
                {2.0f, 1.0f},
                {3.0f, 1.0f},
                {4.0f, 0.0f},
                {4.0f, 2.0f},
                {5.0f, 1.0f},
            };
        case StepsType_dance_threepanel:
            return {
                {0.0f, 2.0f},
                {1.0f, 0.0f},
                {2.0f, 2.0f},
            };
        case StepsType_pump_single:
            return {
                {0.0f, 0.0f},
                {0.0f, 2.0f},
                {1.0f, 1.0f},
                {2.0f, 2.0f},
                {2.0f, 0.0f},
            };
        case StepsType_pump_halfdouble:
            return {
                {1.0f, 1.0f},
                {2.0f, 2.0f},
                {2.0f, 0.0f},
                {3.0f, 0.0f},
                {3.0f, 2.0f},
                {4.0f, 1.0f},
            };
        case StepsType_pump_double:
            return {
                {0.0f, 0.0f},
                {0.0f, 2.0f},
                {1.0f, 1.0f},
                {2.0f, 2.0f},
                {2.0f, 0.0f},
                {3.0f, 0.0f},
                {3.0f, 2.0f},
                {4.0f, 1.0f},
                {5.0f, 2.0f},
                {5.0f, 0.0f},
            };
        case StepsType_horizon_single:
            return {
                {0.0f, 0.0f},
                {0.0f, 1.0f},
                {0.0f, 2.0f},
                {1.0f, 0.0f},
                {1.0f, 1.0f},
                {1.0f, 2.0f},
                {2.0f, 2.0f},
                {2.0f, 1.0f},
                {2.0f, 0.0f},
            };
        case StepsType_horizon_double:
            return {
                {0.0f, 0.0f},
                {0.0f, 1.0f},
                {0.0f, 2.0f},
                {1.0f, 0.0f},
                {1.0f, 1.0f},
                {1.0f, 2.0f},
                {2.0f, 2.0f},
                {2.0f, 1.0f},
                {2.0f, 0.0f},
                {3.0f, 0.0f},
                {3.0f, 1.0f},
                {3.0f, 2.0f},
                {4.0f, 0.0f},
                {4.0f, 1.0f},
                {4.0f, 2.0f},
                {5.0f, 2.0f},
                {5.0f, 1.0f},
                {5.0f, 0.0f},
            };
        default:
            return {
                {0.0f, 0.0f},
            };
    }
}
