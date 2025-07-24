#ifndef MAP_CONSTRUCT_HPP
#define MAP_CONSTRUCT_HPP

#include "map.hpp"

class map_Construct : public GameMap
{
private:
    NE_Model* skybox;
    NE_Material* skyboxMaterial;

    NE_Model* biggarage;
    NE_Model* fence;
    NE_Model* floor;
    NE_Model* grass;
    NE_Model* roof;
    NE_Model* smallgarageramp;
    NE_Material* biggarageMaterial;
    NE_Material* fenceMaterial;
    NE_Material* floorMaterial;
    NE_Material* grassMaterial;
    NE_Material* roofMaterial;
    NE_Material* smallgaragerampMaterial;
public:
    map_Construct() {};

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
        biggarage = NE_ModelCreate(NE_Static);
        biggarageMaterial = NE_MaterialCreate();
        NE_ModelScale(biggarage, 4, 4, 4);
        NE_ModelLoadStaticMeshFAT(biggarage, "models/big-garage.dl");
        NE_MaterialTexLoadGRF(biggarageMaterial, NULL, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_WRAP_S | NE_TEXTURE_WRAP_T), "grit/bricks_png.grf");
        NE_ModelSetMaterial(biggarage, biggarageMaterial);

        fence = NE_ModelCreate(NE_Static);
        fenceMaterial = NE_MaterialCreate();
        NE_ModelScale(fence, 4, 4, 4);
        NE_ModelLoadStaticMeshFAT(fence, "models/fence.dl");
        NE_MaterialTexLoadGRF(fenceMaterial, NULL, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_WRAP_S | NE_TEXTURE_WRAP_T), "grit/fence_png.grf");
        NE_ModelSetMaterial(fence, fenceMaterial);

        floor = NE_ModelCreate(NE_Static);
        floorMaterial = NE_MaterialCreate();
        NE_ModelScale(floor, 4, 4, 4);
        NE_ModelLoadStaticMeshFAT(floor, "models/floor.dl");
        NE_MaterialTexLoadGRF(floorMaterial, NULL, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_WRAP_S | NE_TEXTURE_WRAP_T), "grit/concrete_png.grf");
        NE_ModelSetMaterial(floor, floorMaterial);

        grass = NE_ModelCreate(NE_Static);
        grassMaterial = NE_MaterialCreate();
        NE_ModelScale(grass, 4, 4, 4);
        NE_ModelLoadStaticMeshFAT(grass, "models/grass.dl");
        NE_MaterialTexLoadGRF(grassMaterial, NULL, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_WRAP_S | NE_TEXTURE_WRAP_T), "grit/grass_png.grf");
        NE_ModelSetMaterial(grass, grassMaterial);

        roof = NE_ModelCreate(NE_Static);
        roofMaterial = NE_MaterialCreate();
        NE_ModelScale(roof, 4, 4, 4);
        NE_ModelLoadStaticMeshFAT(roof, "models/roof.dl");
        NE_MaterialTexLoadGRF(roofMaterial, NULL, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_WRAP_S | NE_TEXTURE_WRAP_T), "grit/rooftop_png.grf");
        NE_ModelSetMaterial(roof, roofMaterial);

        smallgarageramp = NE_ModelCreate(NE_Static);
        smallgaragerampMaterial = NE_MaterialCreate();
        NE_ModelScale(smallgarageramp, 4, 4, 4);
        NE_ModelLoadStaticMeshFAT(smallgarageramp, "models/small-garage-ramp.dl");
        NE_MaterialTexLoadGRF(smallgaragerampMaterial, NULL, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD | NE_TEXTURE_WRAP_S | NE_TEXTURE_WRAP_T), "grit/wallbi_png.grf");
        NE_ModelSetMaterial(smallgarageramp, smallgaragerampMaterial);
    };

    void drawMap() override
    {
        // skybox
        NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, NE_RENDER_FAR_CLIPPED);
        NE_ModelDraw(skybox);

        // map and other crap
        NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, (NE_OtherFormatEnum)(NE_MODULATION));
        NE_ModelDraw(biggarage);
        NE_ModelDraw(fence);
        NE_ModelDraw(floor);
        NE_ModelDraw(grass);
        NE_ModelDraw(roof);
        NE_ModelDraw(smallgarageramp);
    }
};

#endif