#include <yuki/core/debug_log.h>
#include <yuki/core/memory/memory_module.h>
#include <yuki/gameplay/input.h>

#include "game.h"

bool
game_init
(yuki_game *game)
{
	YUKI_LOG_DEBUG("game_init() called from testbed");

	return true;
}

bool
game_update
(yuki_game *game, const f32 delta_time)
{
	return true;
}

bool
game_render
(yuki_game *game, const f32 delta_time)
{
	return true;
}
