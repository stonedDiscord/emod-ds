#ifndef GAME_PROP_HPP
#define GAME_PROP_HPP

#include <nds.h>
#include <NEMain.h>

#include "shadowPEngine.hpp"

#define MAP_4096_TO_511(x)  ( ((x + 4096) * 511) / 8192 )

// TO DO: add deconstructor to clean up the pointer in main, or method to destroy prop
class GameProp
{
protected:
    SRigidbody& propRb;
public:
    GameProp(SRigidbody& rb) : propRb(rb) {};

    virtual void initAndLoadPropResources();
    virtual void drawProp();
    virtual void rawDraw();

    vector3 getPosition() const { return propRb.position; }
    SRigidbody* getRigidbodyPointer() const { return &propRb; }

    bool outlined = false;
    int32 distance = -1;
};

// all game props. -implemented in respective cpp source files- nvm
class pr_BouncyBall : public GameProp 
{
private:
    NE_Model* bouncyball;
    NE_Material* bouncyballMaterial;

    NE_Model* testsphere;

    NE_Camera* camera = nullptr;
public:
    pr_BouncyBall(SRigidbody& rb, NE_Camera* cam) : GameProp(rb)
    {
        if(cam != nullptr)
            camera = cam;
        else
            camera = nullptr;
    };

    void initAndLoadPropResources() override
    {
        bouncyball = NE_ModelCreate(NE_Static);
        NE_ModelLoadStaticMeshFAT(bouncyball, "models/bouncyball.dl");
        testsphere = NE_ModelCreate(NE_Static);
        NE_ModelLoadStaticMeshFAT(testsphere, "models/testsphere.dl");

        bouncyballMaterial = NE_MaterialCreate();
        NE_MaterialTexLoadGRF(bouncyballMaterial, NULL, (NE_TextureFlags)(NE_TEXGEN_TEXCOORD), "grit/bouncyball_png.grf");

        NE_MaterialSetProperties(bouncyballMaterial,
                  RGB15(0, 0, 0), // Diffuse
                  RGB15(rand() % 25, rand() % 25, rand() % 25),    // Ambient
                  RGB15(0, 0, 0),    // Specular
                  RGB15(0, 0, 0),    // Emission
                  false, false);     // Vertex color, use shininess table
        NE_ModelSetMaterial(bouncyball, bouncyballMaterial);
    };

    // TO DO: check when rigidbody is not used anymore and destroy prop
    void drawProp() override
    {
        if (camera == nullptr)
            NE_ModelSetCoordI(bouncyball, propRb.position.x, propRb.position.y, propRb.position.z);
        else
        {
            vector3 cameraPosition = vector3(camera->from[0], camera->from[1], camera->from[2]);

            vector3 forward = vectorDifference(cameraPosition, propRb.position); normalizef32(&forward.x);
            vector3 right = vector3_zero;
            vector3 up = vector3_zero;

            crossf32((int32_t*) &camera->up, &forward.x, &right.x); normalizef32(&right.x);
            crossf32(&forward.x, &right.x, &up.x);

            m4x3 m[12] = {
                right.x, right.y, right.z,
                up.x, up.y, up.z,
                forward.x, forward.y, forward.z,
                propRb.position.x, propRb.position.y, propRb.position.z
            };

            NE_ModelSetMatrix(bouncyball, m);
            NE_ModelSetMatrix(testsphere, m);
        }
        
        if (outlined)
        {
            NE_PolyFormat(21, 8, NE_LIGHT_0, NE_CULL_BACK, (NE_OtherFormatEnum)(NE_MODULATION));
            NE_ModelDraw(bouncyball);    
        }
        else
        {
            NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, (NE_OtherFormatEnum)(NE_MODULATION));
            NE_ModelDraw(bouncyball);
        }
    };

    void rawDraw() override
    {
        NE_PolyFormat(31, 0, NE_LIGHT_0, NE_CULL_BACK, (NE_OtherFormatEnum)(NE_MODULATION));
        NE_ModelDraw(testsphere);
    };
};

#endif