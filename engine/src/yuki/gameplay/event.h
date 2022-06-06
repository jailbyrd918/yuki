#ifndef YUKI_HEADER_EVENT
#define YUKI_HEADER_EVENT

#include "yuki/broadcast.h"


typedef enum e_yuki_event_code {

	YUKI_EVENT_CODE_APP_QUIT = 0x01,
	YUKI_EVENT_CODE_KEY_PRESSED = 0x02,
	YUKI_EVENT_CODE_KEY_RELEASED = 0x03,
	YUKI_EVENT_CODE_MOUSE_BTN_PRESSED = 0x04,
	YUKI_EVENT_CODE_MOUSE_BTN_RELEASED = 0x05,
	YUKI_EVENT_CODE_MOUSE_MOVED = 0x06,
	YUKI_EVENT_CODE_MOUSE_SCROLLED = 0x07,
	YUKI_EVENT_CODE_RESIZED = 0x08,

	YUKI_EVENT_CODES_NUM = 0xff
}
yuki_event_code;

#	define	YUKI_EVENT_CODES_MAX		32767

typedef struct s_yuki_event_context {

	union {
		s64	s64[2];
		u64	u64[2];
		f64	f64[2];

		s32	s32[4];
		u32	u32[4];
		f32	f32[4];

		s16	s16[8];
		u16	u16[8];

		s8	s8[16];
		u8	u8[16];

		char	ch[16];
	} data;

}
yuki_event_context;

typedef bool(*pfn_on_event)(const u32 code, void *sender, void *listener, const yuki_event_context context);


bool
event_module_startup
(u64 *required_memory_size, void *state);

void
event_module_shutdown
(void *state);


YUKI_API bool
event_module_register_event
(void *listener, const u32 code, pfn_on_event on_event_callback);

YUKI_API bool
event_module_unregister_event
(void *listener, const u32 code, pfn_on_event on_event_callback);

YUKI_API bool
event_module_fire_event
(void *sender, const u32 code, yuki_event_context context);



#endif

