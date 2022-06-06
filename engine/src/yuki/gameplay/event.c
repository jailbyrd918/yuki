#include "yuki/core/debug_log.h"
#include "yuki/core/memory/memory_module.h"
#include "yuki/containers/dynamic_array.h"

#include "yuki/gameplay/event.h"

#include <stdlib.h>


typedef struct s_yuki_registered_event {

	void		*listener;
	pfn_on_event	event_callback;

}
yuki_registered_event;

typedef struct s_yuki_event_code_entry {

	yuki_registered_event	*events;

}
yuki_event_code_entry;

typedef struct s_yuki_event_module_state {

	// event codes lookup
	yuki_event_code_entry	entries[YUKI_EVENT_CODES_MAX];

}
yuki_event_module_state;

static yuki_event_module_state *state_ref;


bool
event_module_startup
(u64 *required_memory_size, void *state)
{
	*required_memory_size = sizeof(yuki_event_module_state);

	if (!state)
		return false;

	state_ref = YUKI_CAST(yuki_event_module_state *, state);
	memory_module_set_block_zero(state_ref, sizeof(yuki_event_module_state));

	return true;
}

void
event_module_shutdown
(void *state)
{
	if (!state)
		return;

	state_ref = YUKI_CAST(yuki_event_module_state *, state);
	for (u32 i = 0; i < YUKI_EVENT_CODES_MAX; ++i)
		if (state_ref->entries[i].events != NULL)
			dynamic_array_free(state_ref->entries[i].events);
}


YUKI_API bool
event_module_register_event
(void *listener, const u32 code, pfn_on_event on_event_callback)
{
	if (!state_ref) {
		YUKI_LOG_ERROR("event subsystem state is null!");
		return false;
	}

	if (!state_ref->entries[code].events)
		state_ref->entries[code].events = dynamic_array_alloc(yuki_registered_event);

	// bail if there is event with duplicated listener 
	for (u32 i = 0; i < dynamic_array_get_length(state_ref->entries[code].events); ++i)
		if (state_ref->entries[code].events[i].listener == listener)
			return false;

	// register event
	yuki_registered_event event;
	event.listener = listener;
	event.event_callback = on_event_callback;
	dynamic_array_push_back(state_ref->entries[code].events, event);

	return true;
}

YUKI_API bool
event_module_unregister_event
(void *listener, const u32 code, pfn_on_event on_event_callback)
{
	if (!state_ref) {
		YUKI_LOG_ERROR("event subsystem state is null!");
		return false;
	}

	// bail if there are no registered event for the code
	if (!state_ref->entries[code].events)
		return false;

	// scour through the list for the specified event
	for (u32 i = 0; i < dynamic_array_get_length(state_ref->entries[code].events); ++i) {
		yuki_registered_event event = state_ref->entries[code].events[i];

		if ((event.listener == listener) && event.event_callback == on_event_callback) {
			// found the registered event, unregister and remove it
			dynamic_array_erase(state_ref->entries[code].events, i);
			return true;
		}
	}

	return false;
}

YUKI_API bool
event_module_fire_event
(void *sender, const u32 code, yuki_event_context context)
{
	if (!state_ref) {
		YUKI_LOG_ERROR("event subsystem state is null!");
		return false;
	}

	// bail if there are no registered event for the code
	if (!state_ref->entries[code].events)
		return false;

	for (u32 i = 0; i < dynamic_array_get_length(state_ref->entries[code].events); ++i) {
		yuki_registered_event event = state_ref->entries[code].events[i];

		if (event.event_callback(code, sender, event.listener, context)) {
			// handle message
			return true;
		}
	}

	return false;
}


