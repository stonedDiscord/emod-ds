#ifndef CROSSHAIR_HPP
#define CROSSHAIR_HPP

#include <nds.h>

#include "math.hpp"

#include <NEMain.h>

class Crosshair
{
protected:
    NE_Sprite* crosshairSprite;
    NE_Material* crosshairMaterial;
    NE_Palette* crosshairPalette;
public:
    void initAndLoadCrosshairResources()
    {
        crosshairSprite = NE_SpriteCreate();
        crosshairMaterial = NE_MaterialCreate();
        crosshairPalette = NE_PaletteCreate();

        NE_MaterialTexLoadGRF(crosshairMaterial, crosshairPalette, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_COLOR0_TRANSPARENT), "grit/crosshair_png.grf");
        NE_MaterialSetPalette(crosshairMaterial, crosshairPalette);
        NE_SpriteSetMaterial(crosshairSprite, crosshairMaterial);
        NE_SpriteSetParams(crosshairSprite, 16, 0, NE_White);
    };
    
    void drawCrosshair()
    {
        NE_PolyFormat(31, 0, (NE_LightEnum)0, NE_CULL_NONE, NE_MODULATION);
        NE_SpriteSetPos(crosshairSprite, 256 / 2 - 4, 192 / 2 - 4);
        NE_SpriteSetSize(crosshairSprite, 8, 8);
        NE_SpriteDraw(crosshairSprite);
    };
};

#endif