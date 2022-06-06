#ifndef YUKI_HEADER_GAME_DEFS
#define YUKI_HEADER_GAME_DEFS

#include "yuki/broadcast.h"


typedef	struct s_yuki_game_config {

	s32		x, y;
	s32		width, height;

	const_str	name;

	s32		framerate_limit;

}
yuki_game_config;

typedef	struct s_yuki_game_state {

	f32	delta_time;

}
yuki_game_state;

typedef	struct s_yuki_game {

	yuki_game_config	config;
	void			*state;
	void			*application_state;

	bool(*pfn_init)(struct s_yuki_game *game);
	bool(*pfn_update)(struct s_yuki_game *game, const f32 delta_time);
	bool(*pfn_render)(struct s_yuki_game *game, const f32 delta_time);

}
yuki_game;


#endif
