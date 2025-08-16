// a few notes:
//
// - Remember to avoid using 3D features of NFlib.
//
// - Don't use the dual 3D mode in Nitro Engine.
//
// - You are free to use any features of NFlib in the secondary screen, but you
//   need to assign VRAM C and D to NFlib to get space for sprites and
//   backgrounds.
//
// - You could load 2D background and sprites in the same screen as the 3D
//   rendering, but you will have to give up VRAM banks A and B. If you also use
//   banks C and D for the secondary screen, it will leave you without any VRAM
//   for textures.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include <filesystem.h>
#include <nds.h>
#include <nf_lib.h>
#include <NEMain.h>
#include <maxmod9.h>

#include "nitrofs/soundbank_info.h"

// audio
#include "audioManager.hpp"

// game
#include "player.hpp"

// physics
#include "shadowPEngine.hpp"
#include "staticPhysics.hpp"
#include "gameFifo.hpp"

// gui
#include "gui.hpp"
#include "crosshair.hpp"

// map
#include "map_flatgrass.hpp"
#include "cmap_flatgrass.hpp"

// tools
#include "tl_hand.hpp"

// props
#include "props.hpp"

// utility
#include "LUTs.hpp"
#include "math.hpp"

ShadowPEngine* StaticPhysics::enginePointer = nullptr;
bool AudioManager::init = false;

int32 sqrtv(int32 x)
{
	const u16 a = f32toint(x);
	const u16 b = x&4095;
	return (a<SQRTRANGE)?((a)?mulf32(sqrtLUT1[a],sqrtLUT2[b/(2*a)]):(sqrtLUT3[b])):(0);
}

typedef struct
{
    Player* player;
    NE_Camera* camera;
    
    // font
    // NE_Material* testFont;

    Crosshair* crosshair;

    Tool* handTool;
    GameProp* props[MAXRIGIDBODIES] = { nullptr };

    // map
    GameMap* map;
    
} SceneData;

void draw3DScene(SceneData* arg)
{
    SceneData* scene = arg;

    NE_CameraUse(scene->camera);

    // map
    scene->map->drawMap();

    // props
    for (int i = 0; i < MAXRIGIDBODIES; i++)
    {
        if (scene->props[i] != nullptr)
            scene->props[i]->drawProp();
    }

    NE_TouchTestStart();
    for (int i = 0; i < MAXRIGIDBODIES; i++)
    {
        if (scene->props[i] != nullptr)
        {
            NE_TouchTestObject();
            scene->props[i]->rawDraw();
            scene->props[i]->distance = NE_TouchTestResult();
        }
    }
    NE_TouchTestEnd();

    NE_2DViewInit();
    
    // crosshair
    scene->crosshair->drawCrosshair();

    // held tool
    if (scene->player != nullptr && scene->player->toolHeld != nullptr)
        scene->handTool->drawTool();
}

void waitLoop(void)
{
    while (1)
        swiWaitForVBlank();
}

void setupCamera(SceneData* scene)
{
    /* CAMERA SETUP */
    scene->camera = NE_CameraCreate();
    NE_CameraSet(scene->camera,
                 6, 3, -4,
                 0, 3, 0,
                 0, 1, 0);
    NE_ClippingPlanesSet(0.1, 1000);

    NE_SetFov(65);
}

void loadAndSetupGraphics3D(SceneData* scene)
{
    // When using nflib for the 2D sub screen engine, banks C and H are used for
    // backgrounds and banks D and I are used for sprites. Nitro Engine only
    // uses bank E for palettes, so the only thing we need to do is to tell
    // Nitro Engine to only use banks A and B and leave C and D unused.

    NE_TextureSystemReset(0, 0, NE_VRAM_AB);

    /* GAME MAP */
    scene->map->initAndLoadMapResources();

    /* TOOLS */
    scene->handTool->initAndLoadToolResources();

    /* PROPS */
    for (size_t i = 0; i < MAXRIGIDBODIES; i++)
    {
        if (scene->props[i] != nullptr)
            scene->props[i]->initAndLoadPropResources();
    }

    /* CROSSHAIR */
    scene->crosshair->initAndLoadCrosshairResources();

    /*
    // test font
    scene->testFont = NE_MaterialCreate();
    QuickAssert(NE_MaterialTexLoadGRF(scene->testFont, NULL, NE_TEXGEN_TEXCOORD, "grit/testfnt_png.grf"));
    NE_TextInit(0, scene->testFont, 8, 8);    
    */

    /* LIGHTS */
    NE_LightSet(0, NE_White, 0.2, -0.3, -1);

    /* BG COLOR */
    NE_ClearColorSet(NE_DarkBlue, 31, 63);

    /* FOG */
    //NE_FogEnable(7, NE_DarkBlue, 31, 3, 0x7ff00);

    /* TOON SHADING */
    NE_SetupToonShadingTables(true);
    /* ANTIALIASING */
    NE_AntialiasEnable(true);
    /* OUTLINING */
    NE_OutliningEnable(true);
    NE_OutliningSetColor(1, NE_Red);
}

void loadAndSetupGraphics2D(GameUI* gui)
{
    // initialize sub 2D engine
    NF_Set2D(1, 0);

    gui->initAndLoadUIResources();
}

touchPosition touchCurrent, touchOld;
void fetchPlayerInput(PlayerInput& input)
{
    uint32_t keys = keysHeld();

    input.wishDir = vector3_zero;
    if (keys & KEY_RIGHT)
        input.wishDir.x = 4096;
    if (keys & KEY_LEFT)
        input.wishDir.x = -4096;
    if (keys & KEY_UP)
        input.wishDir.z = 4096;
    if (keys & KEY_DOWN)
        input.wishDir.z = -4096;
    
    int32 itmp[3] = {input.wishDir.x, 0, input.wishDir.z};
    normalizef32((int32*) &itmp);;
    
    input.wishDir.x = itmp[0];
    input.wishDir.z = itmp[2];

    input.wishJump = keysDown() & KEY_B;
    input.wishPrimary = keys & KEY_L;

    touchRead(&touchCurrent);

    if(keysDown() & KEY_TOUCH)
        touchOld = touchCurrent;

    input.look = keys & KEY_TOUCH;
    if (input.look)
        input.lookDelta = vector2(touchCurrent.px - touchOld.px, touchCurrent.py - touchOld.py);
    else
        input.lookDelta = vector2_zero;
    
    touchOld = touchCurrent;
}

int main(int argc, char *argv[])
{   
    SceneData scene = { 0 };

    NF_Set2D(0, 0);
    NF_Set2D(1, 0);

    consoleDemoInit();
    consoleDebugInit(DebugDevice_NOCASH);
    printf("starting nitroFS...\n");

    if (!nitroFSInit(NULL))
    {
        printf("failed to start nitroFS.\n");
        printf("press START to exit. fuck.\n");

        while (1)
        {
            swiWaitForVBlank();
            scanKeys();
            if (keysHeld() & KEY_START)
                return -1;
        }
    }

    swiWaitForVBlank();

    // set root folder for the nitroFS filesystem
    NF_SetRootFolder("NITROFS");

    mmInitDefault("maxmod/soundbank.bin");

    // setup interrupt handlers
    irqEnable(IRQ_HBLANK);
    irqSet(IRQ_VBLANK, NE_VBLFunc);
    irqSet(IRQ_HBLANK, NE_HBLFunc);

    // init static physics class
    ShadowPEngine shadowPEngine = ShadowPEngine();
    StaticPhysics::enginePointer = &shadowPEngine;

    // instantiate necessary classes
    GameUI* gui = new GameUI();
    scene.crosshair = new Crosshair();
    scene.map = new map_Flatgrass();
    scene.handTool = new tl_Hand();

    NE_Init3D();
    setupCamera(&scene);

    // init props objects
    SRigidbody* testPropRb = shadowPEngine.initSphereRigidbody(vector3(0, floattof32(2), 0), floattof32(0.8));
    SRigidbody* testPropRb2 = shadowPEngine.initSphereRigidbody(vector3(floattof32(-2), floattof32(2), 0), floattof32(0.8));
    SRigidbody* testPropRb3 = shadowPEngine.initSphereRigidbody(vector3(0, floattof32(2), 0), floattof32(0.8));
    SRigidbody* testPropRb4 = shadowPEngine.initSphereRigidbody(vector3(floattof32(-2), floattof32(2), 0), floattof32(0.8));
    SRigidbody* testPropRb5 = shadowPEngine.initSphereRigidbody(vector3(0, floattof32(2), 0), floattof32(0.8));
    SRigidbody* testPropRb6 = shadowPEngine.initSphereRigidbody(vector3(floattof32(-2), floattof32(2), 0), floattof32(0.8));
    scene.props[0] = new pr_BouncyBall(*testPropRb, scene.camera);
    scene.props[1] = new pr_BouncyBall(*testPropRb2, scene.camera);
    scene.props[2] = new pr_BouncyBall(*testPropRb3, scene.camera);
    scene.props[3] = new pr_BouncyBall(*testPropRb4, scene.camera);
    scene.props[4] = new pr_BouncyBall(*testPropRb5, scene.camera);
    scene.props[5] = new pr_BouncyBall(*testPropRb6, scene.camera);

    // load and setup graphics/audio
    loadAndSetupGraphics3D(&scene);
    loadAndSetupGraphics2D(gui);
    AudioManager::loadAndSetupAudioResources();

    // create collision map
    cmap_Flatgrass* cmap = new cmap_Flatgrass();

    // init player
    PlayerInput input;
    Player player(*scene.camera, *cmap);
    scene.player = &player; // set this fucking pointer

    player.setPosition(6, 3, -4);
    player.toolHeld = scene.handTool;

    while (1)
    {
        NE_WaitForVBL(NE_UPDATE_ANIMATIONS);

        // update 2d elements
        // print text
        
        gui->scrollGUIBackground();
        gui->writeMapName(scene.map);

        // debug
        /*
        char debug[64];
        snprintf(debug, sizeof(debug), "x%d y%d z%d d%d", testPropRb->position.x, testPropRb->position.y, testPropRb->position.z, scene.props[0]->distance);
        NF_WriteText(1, 0, 1, 2, debug);
        */

        NF_UpdateTextLayers();
        // ...

        // copy the shadow OAM to the OAM of the 2D sub engine
        oamUpdate(&oamSub);

        consoleClear();

        // process new frame after the 2D elements have been updated
        scanKeys();
        uint32_t keys = keysHeld();

        if (keys & KEY_START)
            break;
        
        // update player
        fetchPlayerInput(input);
        player.updateWithInput(input);
        player.getTargetPropLineTrace(scene.props);

        // refresh shadow OAM copy
        NF_SpriteOamSet(1);

        // draw 3D scene
        NE_ProcessArg(reinterpret_cast<NE_VoidArgfunc>(draw3DScene), &scene);

        // sync physics engine
        shadowPEngine.syncShadowPhysicsEngine();
    }

    return 0;
}
