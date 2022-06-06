#ifndef YUKI_HEADER_STR
#define YUKI_HEADER_STR

#include "yuki/broadcast.h"


void
str_startup
(void);

void
str_shutdown
(void);


const u32
str_get_string_length
(const_str string);

str
str_get_duplicate
(const_str string);

const bool
str_strings_equal_sensitive
(const_str left, const_str right);

const bool
str_strings_equal_insensitive
(const_str left, const_str right);

const bool
str_strings_equal_ranged_sensitive
(const_str left, const_str right, const s32 range);

const bool
str_strings_equal_ranged_insensitive
(const_str left, const_str right, const s32 range);

const s32
str_format_string
(str dest, const_str format, ...);

const s32
str_format_string_variadic
(str dest, const_str format, void *va_list);

const str
str_empty_string
(str string);

const str
str_copy_string
(str dest, const_str string);

const str
str_copy_string_ranged
(str dest, const_str string, const s32 range);

const str
str_remove_whitespace
(const_str string);

const str
str_get_substring
(const_str string, const s32 position, const s32 range);

const s32
str_get_character_index
(const_str string, const s8 character);

const u32
str_hash_string
(const_str string);


#endif

