#ifndef AUDIO_MANAGER_HPP
#define AUDIO_MANAGER_HPP

#include <nds.h>
#include <vector>

#include "math.hpp"

#include <maxmod9.h>
#include "nitrofs/soundbank_info.h"
#include <cstdlib>

class AudioManager
{
private:
    static bool init;
public:
    static void loadAndSetupAudioResources()
    {
        // grass footsteps
        mmLoadEffect(SFX_GRASS1);
        mmLoadEffect(SFX_GRASS2);
        mmLoadEffect(SFX_GRASS3);

        init = true;
    };

    static void playFootstep()
    {
        if(!init)
            return;
        
        int footstep = 0;
        switch (rand() % 3)
        {
            default:
                footstep = SFX_GRASS1;
                break;
            case 1:
                footstep = SFX_GRASS2;
                break;
            case 2:
                footstep = SFX_GRASS3;
                break;
        }

        mmEffect(footstep);
    };
};

#endif