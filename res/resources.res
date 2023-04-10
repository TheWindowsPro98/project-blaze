# Images
IMAGE sega_logo "gfx/sega.png" BEST ALL
IMAGE custom_font "gfx/font_menu.png" BEST NONE 
IMAGE title_logo "gfx/title.png" BEST ALL
IMAGE bsod_frown "gfx/frown.png" BEST ALL

# Tilesets
TILESET opts_tiles "tiles/options_bg.png" BEST ALL
TILESET test_tiles "tiles/test_lvl.png" BEST ALL

# Tilemaps
TILEMAP options_map "maps/options_bg.png" opts_tiles BEST ALL
#------------------------------------------------------------


# Sprites
SPRITE stephanie "gfx/stephanie_rev0.png" 8 8 BEST 12
SPRITE lucy "gfx/lucy_rev0.png" 6 8 BEST 12
SPRITE cursor "gfx/cursor.png" 1 1 BEST 0
SPRITE bars "gfx/healthbars.png" 1 1 BEST 0
SPRITE plr_icns "gfx/player-icons.png" 2 2 BEST 0
SPRITE signpost "gfx/signpost.png" 6 6 BEST 2
SPRITE coin "gfx/coin.png" 2 2 BEST 6

# PCM
WAV testxgm "pcm/testpcm.wav" XGM 
WAV back_sfx "pcm/back_menu.wav" PCM
WAV back_xgm "pcm/back_menu.wav" XGM  
WAV barrel_brk "pcm/barrel_kd.wav" XGM
WAV angy_yell "pcm/big_mad.wav" XGM
WAV max_punch "pcm/cronch.wav" XGM 
WAV punch1 "pcm/punch1.wav" XGM
WAV punch2 "pcm/punch2.wav" XGM
WAV punch3 "pcm/punch3.wav" XGM
WAV punch4 "pcm/punch4.wav" XGM
WAV punch5 "pcm/punch5.wav" XGM
WAV consume "pcm/eat.wav" XGM 
WAV boom_sfx "pcm/explode.wav" XGM
WAV fall "pcm/fall.wav" XGM 
WAV break_gen "pcm/generic_break.wav" XGM
WAV menu_hvr "pcm/hover.wav" PCM 
WAV hvr_xgm "pcm/hover.wav" XGM 
WAV itm_cons "pcm/item_collect.wav" XGM 
WAV jump_sfx "pcm/jump.wav" XGM 
WAV land "pcm/land.wav" XGM 
WAV segaxgm "pcm/Sega.wav" XGM 
WAV menu_sel_sfx "pcm/select.wav" PCM
WAV sel_xgm "pcm/select.wav" XGM   
WAV stab_sfx "pcm/stab.wav" XGM 
#-----------------------------------------
WAV fem_die "pcm/girl_ded.wav" XGM
WAV kikoku "pcm/girl_egrgball.wav" XGM 
WAV fem_kick "pcm/girl_jumpkick.wav" XGM 
WAV fem_throw "pcm/girl_throw.wav" XGM 
#-----------------------------------------
WAV mans_ded1 "pcm/mans_ded_a.wav" XGM 
WAV mans_ded2 "pcm/mans_ded_b.wav" XGM 
WAV fasthit "pcm/mans_power.wav" XGM 
WAV mans_throw_enemy "pcm/mans_throw.wav" XGM 
WAV mans_throw_item "pcm/mans_throw_item.wav" XGM 

# Music

XGM lvlclr "vgm/victory.vgm"
XGM gameover "vgm/gameover.vgm"
XGM rboss "vgm/boss.vgm"
XGM fboss "vgm/boss2.vgm"
XGM ending "vgm/end.vgm"
XGM testtrck "vgm/testvgm.vgm"
XGM testtrck2 "vgm/test2.vgm"
XGM titlevgm "vgm/title.vgm"
#-Stage-Themes--------------------
XGM elevator "vgm/elevator.vgm"
XGM stg1 "vgm/city.vgm"
XGM stg2 "vgm/city2.vgm"
XGM stg3 "vgm/bar.vgm"
XGM stg4 "vgm/alley.vgm"
XGM stg5 "vgm/bridge.vgm"
XGM stg6 "vgm/station.vgm"
XGM stg7 "vgm/train.vgm"
XGM stg8 "vgm/final.vgm"

# Palettes
BIN options_pal "pals/main_menu.bin" 2 2 0 APLIB FALSE
BIN test_palette "pals/test_lvl.bin" 2 2 0 APLIB FALSE
BIN bsod_palette "pals/bsod.bin" 2 2 0 APLIB FALSE

# Raw binary data
BIN playerNames "bin/playerNames.bin" 2 2 0 APLIB FALSE