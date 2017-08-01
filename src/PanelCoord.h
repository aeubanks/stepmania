#ifndef PANEL_COORD_H
#define PANEL_COORD_H

#include <vector>
#include "NoteData.h"

struct PanelCoord {
    float x, y;
};

bool operator==(PanelCoord a, PanelCoord b);

std::vector<PanelCoord> panelCoordsForStepsType(StepsType stepstype);

#endif