#ifndef GAME_H
#define GAME_H

#include <yuki/core/application/game_defs.h>


bool
game_init
(yuki_game *game);

bool
game_update
(yuki_game *game, const f32 delta_time);

bool
game_render
(yuki_game *game, const f32 delta_time);



#endif