#ifndef TOOL_HPP
#define TOOL_HPP

#include <nds.h>

#include "math.hpp"

#include <NEMain.h>
#include "gameObject.hpp"
#include "props.hpp"

struct ToolContext
{
    vector3 cameraPosition;
    vector3 cameraForward;
    GameProp* targetProp;
};

class Tool : public GameObject
{
protected:
    NE_Sprite* toolSprite;
    NE_Material* toolMaterial;
    NE_Palette* toolPalette;

    vector3 fixedPosition = vector3_zero;
    int width, height;
public:
    virtual void initAndLoadToolResources();
    virtual void drawTool();

    virtual void primaryUse(ToolContext& context);

    vector3 localPosition = vector3_zero;
};

#endif