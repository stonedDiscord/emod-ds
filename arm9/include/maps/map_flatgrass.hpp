#ifndef MAP_CONSTRUCT_HPP
#define MAP_CONSTRUCT_HPP

#include "map.hpp"

class map_Flatgrass : public GameMap
{
private:
    NE_Model* skybox;
    NE_Material* skyboxMaterial;

    NE_Model* grass;
    NE_Material* grassMaterial;
    NE_Model* building;
    NE_Material* buildingMaterial;
public:
    map_Flatgrass()
    {
        mapName = "ds_flatgrass";
    };

    void initAndLoadMapResources() override
    {
        /* SKYBOX */
        skybox = NE_ModelCreate(NE_Static);
        NE_ModelScale(skybox, 200, 200, 200);
        NE_ModelLoadStaticMeshFAT(skybox, "models/skybox.dl");

        // material
        skyboxMaterial = NE_MaterialCreate();
        NE_MaterialSetProperties(skyboxMaterial,
                    RGB15(0, 0, 0), // Diffuse
                    RGB15(0, 0, 0),    // Ambient
                    RGB15(0, 0, 0),    // Specular
                    RGB15(24, 24, 24),    // Emission
                    false, false);     // Vertex color, use shininess table
        NE_MaterialTexLoadGRF(skyboxMaterial, NULL, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_WRAP_S | NE_TEXTURE_WRAP_T), "grit/skybox_png.grf");

        // assign material
        NE_ModelSetMaterial(skybox, skyboxMaterial);

        /* MAP */
        grass = NE_ModelCreate(NE_Static);
        grassMaterial = NE_MaterialCreate();
        NE_ModelScale(grass, 10, 10, 10);
        NE_ModelLoadStaticMeshFAT(grass, "models/grass.dl");
        NE_MaterialTexLoadGRF(grassMaterial, NULL, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_WRAP_S | NE_TEXTURE_WRAP_T), "grit/grass_png.grf");
        NE_ModelSetMaterial(grass, grassMaterial);

        building = NE_ModelCreate(NE_Static);
        buildingMaterial = NE_MaterialCreate();
        NE_ModelScale(building, 3, 3, 3);
        NE_ModelLoadStaticMeshFAT(building, "models/building.dl");
        NE_MaterialTexLoadGRF(buildingMaterial, NULL, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_WRAP_S | NE_TEXTURE_WRAP_T), "grit/bricks_png.grf");
        NE_ModelSetMaterial(building, buildingMaterial);
    };

    void drawMap() override
    {
        // skybox
        NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_RENDER_FAR_CLIPPED);
        NE_ModelDraw(skybox);

        // map and other crap
        NE_LightSet(0, NE_White, -0.5, -0.5, -0.5);
        //NE_PolyFormat(31, 0, NE_LightBlue, NE_CULL_BACK, NE_MODULATION);
        NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, (NE_OtherFormatEnum)(NE_MODULATION));
        NE_ModelDraw(grass);
        NE_ModelDraw(building);
    }
};

#endif