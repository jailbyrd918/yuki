#include "yuki/core/debug_log.h"
#include "yuki/core/assertions.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/core/memory/linear_allocator.h"
#include "yuki/core/str.h"
#include "yuki/platform/filesystem.h"
#include "yuki/platform/window.h"
#include "yuki/gameplay/event.h"
#include "yuki/gameplay/input.h"
#include "yuki/resources/resource_module.h"
#include "yuki/renderer/frontend/texture.h"
#include "yuki/renderer/frontend/mesh.h"
#include "yuki/renderer/frontend/render_module.h"

#include "yuki/core/application/application.h"

#include <stdlib.h>
#include <ctype.h>
#include <SDL2/SDL.h>


typedef struct s_yuki_application_state {

	yuki_game		*game;
	yuki_window_module	window_module;
	yuki_linear_allocator	modules_allocator;

	bool	active;

	s32	last_time,
		frames_per_sec;

	f32	delta_time;
	f64	absolute_time;


	u64	memory_module_required_memory_size;
	void	*memory_module_state;

	u64	log_module_required_memory_size;
	void	*log_module_state;

	u64	event_module_required_memory_size;
	void	*event_module_state;

	u64	input_module_required_memory_size;
	void	*input_module_state;

	u64	resource_module_required_memory_size;
	void	*resource_module_state;

	u64	texture_module_required_memory_size;
	void	*texture_module_state;

	u64	mesh_module_required_memory_size;
	void	*mesh_module_state;

	u64	render_module_required_memory_size;
	void	*render_module_state;

}
yuki_application_state;

static yuki_application_state *app_state;
static yuki_render_data render_data;

static yuki_mesh *g_mesh;


static bool
_ykstatic_app_on_event
(const u32 code, void *sender, void *listener, const yuki_event_context context)
{
	switch (code) {
		case YUKI_EVENT_CODE_APP_QUIT:
		{
			YUKI_LOG_INFO("EVENT_CODE_APP_QUIT invoked - shutting down...");
			app_state->active = false;
			return true;
		}
		break;

		default:
			break;
	}

	return false;
}

static bool
_ykstatic_app_on_key
(const u32 code, void *sender, void *listener, const yuki_event_context context)
{
	switch (code) {
		case YUKI_EVENT_CODE_KEY_PRESSED:
		{
			const u16 key = context.data.u16[0];

			if (key == YUKI_KEYCODE_ESCAPE) {
				yuki_event_context ctx = { 0 };
				event_module_fire_event(NULL, YUKI_EVENT_CODE_APP_QUIT, ctx);
				return true;
			}

			YUKI_LOG_INFO("key '%c' pressed!", toupper(YUKI_GET_KEYCODE_CHARACTER(key)));

		}
		break;

		case YUKI_EVENT_CODE_KEY_RELEASED:
		{
			const u16 key = context.data.u16[0];
			YUKI_LOG_INFO("key '%c' released!", toupper(YUKI_GET_KEYCODE_CHARACTER(key)));
		}
		break;

		default:
			break;
	}

	return false;
}

static void
_ykstatic_app_update_state
(void)
{
	const s32 frameduration = YUKI_CAST(s32, 1000 / app_state->frames_per_sec);

	// delay until frame duration is reached
	s32 delaytime = frameduration - (SDL_GetTicks() - app_state->last_time);
	if (delaytime >= 0 && delaytime <= frameduration)
		SDL_Delay(delaytime);

	// update delta_time and last_time
	{
		app_state->delta_time = (SDL_GetTicks() - app_state->last_time) / 1000.f;
		app_state->last_time = SDL_GetTicks();
		app_state->absolute_time += app_state->delta_time;
	}
}


YUKI_API bool
application_construct
(yuki_game *game)
{
	if (game->application_state) {
		YUKI_LOG_CRITICAL("application is already initialized!");
		return false;
	}

	// initialize game applicaton state
	game->application_state = memory_module_allocate_block(sizeof(struct s_yuki_application_state), YUKI_MEMORY_TAG_APPLICATION);

	// initialize application internal state properties
	{
		app_state = game->application_state;
		app_state->game = game;
		app_state->frames_per_sec = game->config.framerate_limit;
		app_state->last_time = 0;
		app_state->active = true;
		app_state->delta_time = 0.f;
	}

	// allocate stack allocator with capacity of 32 MB
	linear_allocator_construct(&app_state->modules_allocator, 64 * (1024 * 1024), NULL);

	// modules startup
	{
		// memory module
		memory_module_startup(&app_state->memory_module_required_memory_size, NULL);
		app_state->memory_module_state = linear_allocator_allocate(&app_state->modules_allocator, app_state->memory_module_required_memory_size);
		if (!memory_module_startup(&app_state->memory_module_required_memory_size, app_state->memory_module_state)) {
			YUKI_LOG_CRITICAL("failed to initialize memory module!");
			return false;
		}

		// log module
		log_module_startup("out/log.txt", &app_state->log_module_required_memory_size, NULL);
		app_state->log_module_state = linear_allocator_allocate(&app_state->modules_allocator, app_state->log_module_required_memory_size);
		if (!log_module_startup("out/log.txt", &app_state->log_module_required_memory_size, app_state->log_module_state)) {
			YUKI_LOG_CRITICAL("failed to initialize debug logging module!");
			return false;
		}

		// window module
		if (!window_module_startup(
			&app_state->window_module,
			game->config.name,
			game->config.x, game->config.y,
			game->config.width, game->config.height
		)) {
			YUKI_LOG_CRITICAL("failed to construct application window!");
			return false;
		}

		// event module
		event_module_startup(&app_state->event_module_required_memory_size, NULL);
		app_state->event_module_state = linear_allocator_allocate(&app_state->modules_allocator, app_state->event_module_required_memory_size);
		if (!event_module_startup(&app_state->event_module_required_memory_size, app_state->event_module_state)) {
			YUKI_LOG_CRITICAL("failed to initialize event module!");
			return false;
		}

		// input module
		input_module_startup(&app_state->input_module_required_memory_size, NULL);
		app_state->input_module_state = linear_allocator_allocate(&app_state->modules_allocator, app_state->input_module_required_memory_size);
		if (!input_module_startup(&app_state->input_module_required_memory_size, app_state->input_module_state)) {
			YUKI_LOG_CRITICAL("failed to initialize input module!");
			return false;
		}

		// resource module
		yuki_resource_module_config resrcconfig;
		resrcconfig.asset_base_path = "assets";
		resrcconfig.register_loaders_count = 2;
		resource_module_startup(&app_state->resource_module_required_memory_size, NULL, resrcconfig);
		app_state->resource_module_state = linear_allocator_allocate(&app_state->modules_allocator, app_state->resource_module_required_memory_size);
		if (!resource_module_startup(&app_state->resource_module_required_memory_size, app_state->resource_module_state, resrcconfig)) {
			YUKI_LOG_CRITICAL("failed to initialize resource module!");
			return false;
		}

		// texture module
		yuki_texture_module_config texconfig;
		texconfig.max_textures_count = 1;
		texture_module_startup(&app_state->texture_module_required_memory_size, NULL, texconfig);
		app_state->texture_module_state = linear_allocator_allocate(&app_state->modules_allocator, app_state->texture_module_required_memory_size);
		if (!texture_module_startup(&app_state->texture_module_required_memory_size, app_state->texture_module_state, texconfig)) {
			YUKI_LOG_CRITICAL("failed to initialize texture module!");
			return false;
		}

		// mesh module 
		yuki_mesh_module_config meshconfig;
		meshconfig.max_meshes_count = 1;
		mesh_module_startup(&app_state->mesh_module_required_memory_size, NULL, meshconfig);
		app_state->mesh_module_state = linear_allocator_allocate(&app_state->modules_allocator, app_state->mesh_module_required_memory_size);
		if (!mesh_module_startup(&app_state->mesh_module_required_memory_size, app_state->mesh_module_state, meshconfig)) {
			YUKI_LOG_CRITICAL("failed to initialize mesh module!");
			return false;
		}

		// render module
		render_module_startup(&app_state->render_module_required_memory_size, NULL, &app_state->window_module);
		app_state->render_module_state = linear_allocator_allocate(&app_state->modules_allocator, app_state->render_module_required_memory_size);
		if (!render_module_startup(&app_state->render_module_required_memory_size, app_state->render_module_state, &app_state->window_module)) {
			YUKI_LOG_CRITICAL("failed to initialize renderer module!");
			return false;
		}
	}

	// initialize strings management list
	str_startup();

	// initialize SDL library
	YUKI_ASSERT_MSG(SDL_Init(SDL_INIT_EVERYTHING) == 0, "failed to initialize SDL library!");

	// initialize game state
	game->state = memory_module_allocate_block(sizeof(yuki_game_state), YUKI_MEMORY_TAG_GAME);

	// register events
	{
		event_module_register_event(NULL, YUKI_EVENT_CODE_APP_QUIT, _ykstatic_app_on_event);
		event_module_register_event(NULL, YUKI_EVENT_CODE_KEY_PRESSED, _ykstatic_app_on_key);
		event_module_register_event(NULL, YUKI_EVENT_CODE_KEY_RELEASED, _ykstatic_app_on_key);
	}

	// allocate/initialize render data properties 
	{
		render_data.delta_time = 0.f;
		render_data.display_width = window_module_get_width(&app_state->window_module);
		render_data.display_height = window_module_get_height(&app_state->window_module);

		u32 numpx = window_module_get_width(&app_state->window_module) * window_module_get_height(&app_state->window_module);
		render_data.z_buffer = memory_module_allocate_block(sizeof(f32) * numpx, YUKI_MEMORY_TAG_RENDERER);
		render_data.display_buffer = memory_module_allocate_block(sizeof(color32) * numpx, YUKI_MEMORY_TAG_RENDERER);
	}

	// initialize game instance
	if (!app_state->game->pfn_init(app_state->game)) {
		YUKI_LOG_CRITICAL("failed to initialize game instance!");
		return false;
	}

	{
		g_mesh = mesh_module_acquire_mesh("cube");

		yuki_mesh_resource_data *data = YUKI_CAST(yuki_mesh_resource_data *, g_mesh->data);
		YUKI_LOG_INFO("faces count = %u", data->faces_count);
		YUKI_LOG_INFO("uv count = %u", data->uv_coordinates_count);
		YUKI_LOG_INFO("vertices count = %u", data->vertices_count);
	}

	YUKI_LOG_INFO(memory_module_get_usage_info());

	return true;
}

YUKI_API bool
application_run
(yuki_game *game)
{
	_ykstatic_app_update_state();

	do {
		window_module_process_events();
		_ykstatic_app_update_state();

		// update game instance
		if (!app_state->game->pfn_update(app_state->game, app_state->delta_time)) {
			YUKI_LOG_CRITICAL("failed to update game instance!");
			return false;
		}

		// render game instance outputs
		if (!app_state->game->pfn_render(app_state->game, app_state->delta_time)) {
			YUKI_LOG_CRITICAL("failed to render game instance outputs!");
			return false;
		}

		render_data.delta_time = app_state->delta_time;
		render_module_draw_frame(&render_data, &app_state->window_module);

		input_module_update(app_state->delta_time);

	} while (app_state->active);

	// unregister events
	{
		event_module_unregister_event(NULL, YUKI_EVENT_CODE_APP_QUIT, _ykstatic_app_on_event);
		event_module_unregister_event(NULL, YUKI_EVENT_CODE_KEY_PRESSED, _ykstatic_app_on_key);
		event_module_unregister_event(NULL, YUKI_EVENT_CODE_KEY_RELEASED, _ykstatic_app_on_key);
	}

	// shutdown strings management list
	str_shutdown();

	// shutdown render module
	render_module_shutdown(app_state->render_module_state);

	{
		mesh_module_release_mesh("cube");
	}

	// modules shutdown
	{
		// mesh module
		mesh_module_shutdown(app_state->mesh_module_state);

		// texture module
		texture_module_shutdown(app_state->texture_module_state);

		// resource module
		resource_module_shutdown(app_state->resource_module_state);

		// input module 
		input_module_shutdown(app_state->input_module_state);

		// event module 
		event_module_shutdown(app_state->event_module_state);

		// window module
		window_module_shutdown(&app_state->window_module);
	}

	// deallocate render data properties
	{
		memory_module_deallocate_block(render_data.z_buffer, sizeof(f32) * (render_data.display_width * render_data.display_height), YUKI_MEMORY_TAG_RENDERER);
		memory_module_deallocate_block(render_data.display_buffer, sizeof(color32) * (render_data.display_width * render_data.display_height), YUKI_MEMORY_TAG_RENDERER);
	}

	// deallocate game state
	memory_module_deallocate_block(game->state, sizeof(yuki_game_state), YUKI_MEMORY_TAG_GAME);

	YUKI_LOG_INFO(memory_module_get_usage_info());

	// modules shutdown
	{
		// memory modules
		memory_module_shutdown(app_state->memory_module_state);

		// debug logging modules
		log_module_shutdown(app_state->log_module_state);
	}

	// deallocate game state
	memory_module_deallocate_block(game->application_state, sizeof(yuki_application_state), YUKI_MEMORY_TAG_APPLICATION);

	// shutdown SDL library
	SDL_Quit();

	return true;
}

