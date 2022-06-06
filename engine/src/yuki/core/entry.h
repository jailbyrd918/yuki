#ifndef YUKI_HEADER_ENTRY
#define YUKI_HEADER_ENTRY

#include "yuki/broadcast.h"

#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/core/application/application.h"

#include <stdlib.h>
#include <string.h>


extern void
yuki_construct_game
(yuki_game *game);


int
main
(int argc, char **argv)
{
	yuki_game game_inst;

	{
		// construct game instance
		yuki_construct_game(&game_inst);

		// ensure all game callback functions exist
		if (!game_inst.pfn_init || !game_inst.pfn_update || !game_inst.pfn_render) {
			YUKI_LOG_CRITICAL("all game callback functions must be assigned!");
			return 1;
		}

		// initialize game application
		if (!application_construct(&game_inst)) {
			YUKI_LOG_CRITICAL("failed to initialize game application!");
			return 2;
		}

		// run game application loop
		if (!application_run(&game_inst)) {
			YUKI_LOG_CRITICAL("game application did not shutdown properly!");
			return 3;
		}

	}

	return 0;
}


#endif
