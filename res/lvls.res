# Tilesets
TILESET opts_tiles "tiles/options_bg.png" BEST ALL
TILESET test_tiles "tiles/test_lvl.png" BEST ALL
TILESET window_tiles "gfx/stars.png" BEST ALL 

# Tilemaps
TILEMAP options_map "maps/options_bg.png" opts_tiles BEST ALL
TILEMAP window_map "maps/hud_bg.png" window_tiles BEST ALL 
#------------------------------------------------------------
MAP test_fg "maps/test_lvl/foreground.png" test_tiles BEST 0
MAP test_bg "maps/test_lvl/background.png" test_tiles BEST 0