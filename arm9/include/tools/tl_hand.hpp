#ifndef TL_HAND_HPP
#define TL_HAND_HPP

#include "tool.hpp"
#include "staticPhysics.hpp"

class tl_Hand : public Tool
{
    bool primaryUsing = false;
    bool holdingProp = false;

    void initAndLoadToolResources() override
    {
        toolSprite = NE_SpriteCreate();
        toolMaterial = NE_MaterialCreate();
        toolPalette = NE_PaletteCreate();

        NE_MaterialTexLoadGRF(toolMaterial, toolPalette, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_COLOR0_TRANSPARENT), "grit/hand_png.grf");
        NE_MaterialSetPalette(toolMaterial, toolPalette);
        NE_SpriteSetMaterial(toolSprite, toolMaterial);
        NE_SpriteSetMaterialCanvas(toolSprite, 0, 0, 128, 128);

        width = 96;
        height = 96;
        fixedPosition = vector3(160, 125, 0);
    };

    void drawTool() override
    {
        NE_PolyFormat(31, 5, (NE_LightEnum)0, NE_CULL_NONE, NE_MODULATION);
        NE_SpriteSetSize(toolSprite, width, height);

        if (primaryUsing)
            NE_SpriteSetMaterialCanvas(toolSprite, 0, 128, 128, 256);
        else
            NE_SpriteSetMaterialCanvas(toolSprite, 0, 0, 128, 128);
        
        if (holdingProp)
            // shake em up shake em up shake em up shake em
            NE_SpriteSetPos(toolSprite, fixedPosition.x + localPosition.x + (rand() % 10), fixedPosition.y + localPosition.y + (rand() % 10));
        else
            NE_SpriteSetPos(toolSprite, fixedPosition.x + localPosition.x, fixedPosition.y + localPosition.y);


        NE_SpriteDraw(toolSprite);
        
        primaryUsing = false;
        holdingProp = false;
    };

    void primaryUse(ToolContext& context) override
    {
        primaryUsing = true;

        // dragging objects
        if (context.targetProp != nullptr)
        {
            holdingProp = true;
            vector3 targetPoint = addVector(context.cameraPosition, multVectorInt(context.cameraForward, 6));
            vector3 diff = vectorDifference(targetPoint, context.targetProp->getPosition());
            
            if (diff.x == 0 && diff.y == 0 && diff.z == 0)
                return;
            
            vector3 force = multVectorInt(diff, 15);

            if (StaticPhysics::engineActive())
            {
                StaticPhysics::enginePointer->applyForce(context.targetProp->getRigidbodyPointer(), force);
            }
        }
    };
};

#endif