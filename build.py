#!/usr/bin/env python3

from architectds import *

nitrofs = NitroFS()

# map
nitrofs.add_grit(['assets/ds_flatgrass'])
nitrofs.add_nitro_engine_obj(['assets/ds_flatgrass'])
# skybox
nitrofs.add_grit(['assets/skybox'])
nitrofs.add_nitro_engine_obj(['assets/skybox'])
# props
nitrofs.add_grit(['assets/props'])
nitrofs.add_nitro_engine_obj(['assets/props'])

# textures
nitrofs.add_grit(['assets/textures'])
# fonts
nitrofs.add_grit(['assets/nefnt'])

# gravity gun
nitrofs.add_grit(['assets/gravitygun'])
nitrofs.add_nitro_engine_obj(['assets/gravitygun'])

# bg and font
nitrofs.add_nflib_bg_tiled_tileset(['assets/backgrounds'], 'bg')
nitrofs.add_nflib_font(['assets/fnt'], 'fnt')

# audio support
nitrofs_soundbank_header = nitrofs.add_mmutil(['audio'])
nitrofs.generate_image()

arm9 = Arm9Binary(
    sourcedirs=['arm9/source', 'common/source'],
    includedirs=[
        'arm9/include',
        'arm9/include/fifo',
        'arm9/include/game',
        'arm9/include/physics',
        'arm9/include/utility',
        'arm9/include/maps',
        'arm9/include/tools',
        'arm9/include/props',
        'arm9/include/ui',
        'common/include'
        ],
    libs=['NE', 'nflib', 'nds9', 'mm9'],
    libdirs=[
        '${BLOCKSDS}/libs/libnds',
        '${BLOCKSDSEXT}/nitro-engine',
        '${BLOCKSDSEXT}/nflib',
        '${BLOCKSDS}/libs/maxmod'
    ]
)
arm9.add_header_dependencies([nitrofs_soundbank_header])
arm9.generate_elf()

arm7 = Arm7Binary(
    sourcedirs=['arm7/source', 'common/source'],
    includedirs=[
        'arm7/include',
        'arm7/include/fifo',
        'arm7/include/physics',
        'common/include'
        ],
    libs=['nds7', 'mm7'],
    libdirs=['${BLOCKSDS}/libs/libnds', '${BLOCKSDS}/libs/maxmod']
)
arm7.generate_elf()

nds = NdsRom(
    binaries=[arm9, arm7, nitrofs],
    game_title='Python build system',
    game_subtitle='Built with BlocksDS',
    game_author='github.com/blocksds/sdk',
    game_icon='icon.bmp'
)
nds.generate_nds()

nds.run_command_line_arguments()
