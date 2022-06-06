#include "game.h"

#include <yuki.h>


#define	RANDOM_WINDOW_POS	(0x1fff0000u | (0))


extern void
yuki_construct_game
(yuki_game *game)
{
	// configure game properties 
	yuki_game_config gamecfg;
	gamecfg.x = RANDOM_WINDOW_POS;
	gamecfg.y = RANDOM_WINDOW_POS;
	gamecfg.width = 1024;
	gamecfg.height = 768;
	gamecfg.name = "Yuki Game Engine Testbed";
	gamecfg.framerate_limit = 120;

	// assign game config
	game->config = gamecfg;

	game->pfn_init = game_init;
	game->pfn_update = game_update;
	game->pfn_render = game_render;

	game->application_state = NULL;
}
