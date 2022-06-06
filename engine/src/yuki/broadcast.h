#ifndef YUKI_HEADER_BROADCAST
#define YUKI_HEADER_BROADCAST


///////////////////////////// -- TYPEDEFS -- //////////////////////////////

// -- signed types -- //

typedef		signed char		s8;
typedef		signed short		s16;
typedef		signed int		s32;
typedef		signed long long	s64;


// -- unsigned types -- //

typedef		unsigned char		u8;
typedef		unsigned short		u16;
typedef		unsigned int		u32;
typedef		unsigned long long	u64;


// -- floating point types -- //

typedef		float			f32;
typedef		double			f64;


// -- string types -- //

typedef		char *			str;
typedef		const char *		const_str;



////////////////////////////// -- DEFINES -- ////////////////////////////// 

// -- platform detection -- //

#if	defined(_WIN32) || defined(_WIN64)		// Windows 
#	define	YUKI_PLATFORM_WINDOWS		1

#elif	defined(__linux__)				// Linux
#	define	YUKI_PLATFORM_LINUX		1

#elif	defined(__APPLE__)				// Apple
#	define	YUKI_PLATFORM_APPLE		1	

#elif	defined(_POSIX_VERSION)				// POSIX
#	define	YUKI_PLATFORM_POSIX		1

#else							// Unknown platform
#	error	"Unknown platform detected!"

#endif


// -- import/export DLL -- //

#if	defined(YUKI_EXPORT_DLL)	// DLL export

#	if	defined(_MSC_VER)						// MSVC compiler
#		define	YUKI_API	__declspec(dllexport)

#	else									// non-MSVC compiler
#		define	YUKI_API	__attribute__((visibility("default")))

#	endif

#else					// DLL import

#	if	defined(_MSC_VER)						// MSVC compiler
#		define	YUKI_API	__declspec(dllimport)

#	else									// non-MSVC compiler
#		define	YUKI_API

#	endif

#endif


// -- disable MSVC-specific secure and deprecated function warnings -- //

#if	defined(_MSC_VER)
#	define	_CRT_SECURE_NO_WARNINGS
#	pragma	warning	(disable : 4996)
#endif


// -- debug break -- //

#if	defined(_MSC_VER)					// MSVC compiler
#	define	YUKI_DEBUG_BREAK()	__debugbreak()

#else								// non-MSVC compiler
#	define	YUKI_DEBUG_BREAK()	__builtin_trap()

#endif


// -- C-style casting wrapper -- //

#define	YUKI_CAST(type, value)	((type)(value))


// -- define boolean type (if not defined) -- //

#if	!defined(bool)
#	define	bool	u8
#	define	true	YUKI_CAST(bool, 1)
#	define	false	YUKI_CAST(bool, 0)
#endif


// -- variadic arguments list -- //

#if	defined(_MSC_VER)					// MSVC compiler
#	define	YUKI_VA_LIST		va_list

#else								// non-MSVC compiler
#	define	YUKI_VA_LIST		__builtin_va_list

#endif



#endif
