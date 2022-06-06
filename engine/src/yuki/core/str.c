#include "yuki/containers/dynamic_array.h"
#include "yuki/core/memory/memory_module.h"

#include "yuki/core/str.h"

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>


#if	!defined(_MSC_VER)
#	include <strings.h>
#endif

#define	STRING_BUFFER_MAX	32767

static str *str_list = NULL;


void
str_startup
(void)
{
	str_list = dynamic_array_alloc(str);
}

void
str_shutdown
(void)
{
	for (u32 i = 0; i < dynamic_array_get_length(str_list); ++i)
		memory_module_deallocate_block(str_list[i], strlen(str_list[i]), YUKI_MEMORY_TAG_STRING);

	dynamic_array_free(str_list);
}


const u32
str_get_string_length
(const_str string)
{
	return strlen(string);
}

str
str_get_duplicate
(const_str string)
{
	u32 len = strlen(string);
	str dup = memory_module_allocate_block(len, YUKI_MEMORY_TAG_STRING);
	dynamic_array_push_back(str_list, dup);
	memory_module_copy_block(dup, YUKI_CAST(str, string), len);
	dup[len] = '\0';
	return dup;
}

const bool
str_strings_equal_sensitive
(const_str left, const_str right)
{
	return (strcmp(left, right) == 0);
}

const bool
str_strings_equal_insensitive
(const_str left, const_str right)
{
#if	defined(_MSC_VER)
	return (strcmpi(left, right) == 0);

#else
	return (strcasecmp(left, right) == 0);

#endif
}

const bool
str_strings_equal_ranged_sensitive
(const_str left, const_str right, const s32 range)
{
	return (strncmp(left, right, range) == 0);
}

const bool
str_strings_equal_ranged_insensitive
(const_str left, const_str right, const s32 range)
{
#if	defined(_MSC_VER)
	return (strnicmp(left, right, range) == 0);

#else
	return (strncasecmp(left, right, range) == 0);

#endif
}

const s32
str_format_string
(str dest, const_str format, ...)
{
	if (!dest)
		return -1;

	YUKI_VA_LIST ls;
	va_start(ls, format);
	s32 res = str_format_string_variadic(dest, format, ls);
	va_end(ls);
	return res;
}

const s32
str_format_string_variadic
(str dest, const_str format, void *va_list)
{
	if (!dest)
		return -1;

	char buf[STRING_BUFFER_MAX];
	s32 res = vsnprintf(buf, STRING_BUFFER_MAX, format, va_list);
	buf[res] = '\0';

	memory_module_copy_block(dest, buf, res + 1);
	return res;
}

const str
str_empty_string
(str string)
{
	if (string)
		string[0] = '\0';

	return string;
}

const str
str_copy_string
(str dest, const_str string)
{
	return strcpy(dest, string);
}

const str
str_copy_string_ranged
(str dest, const_str string, const s32 range)
{
	return strncpy(dest, string, range);
}

const str
str_remove_whitespace
(const_str string)
{
	str out = str_get_duplicate(string);
	str w = out, r = out;

	do {
		if (!isspace(*r))
			*w++ = *r;
	} while (*r++);

	return out;
}

const str
str_get_substring
(const_str string, const s32 position, const s32 range)
{
	if (range == 0)
		return NULL;

	s32 srclen = strlen(string);
	if (position >= srclen)
		return NULL;

	str substr = memory_module_allocate_block(range + 1, YUKI_MEMORY_TAG_STRING);
	dynamic_array_push_back(str_list, substr);
	s32 j = 0;
	for (s32 i = position; j < range && string[i]; ++i, ++j)
		substr[j] = string[i];

	substr[j] = '\0';
	return substr;
}

const s32
str_get_character_index
(const_str string, const s8 character)
{
	if (!string)
		return -1;

	s32 len = strlen(string);
	for (s32 i = 0; i < len; ++i)
		if (string[i] == character)
			return i;

	return -1;
}

const u32
str_hash_string
(const_str string)
{
	u32 hash = 0;

	s32 len = strlen(string);
	for (s32 i = 0; i < len; ++i) {
		hash += string[i] + (hash << 10);
		hash ^= (hash >> 6);
	}

	hash += (hash << 3);
	hash ^= (hash >> 1);
	hash += (hash << 15);

	return hash;
}

