workspace "yuki"
	architecture    "x86"
	configurations  { "Debug", "Release" }
	startproject    "testbed"


outdir = "%{cfg.system}_%{cfg.architecture}"


project "engine"
	location        "engine"
	kind            "SharedLib"
	language        "C"
        floatingpoint   "Fast"
        compileas       "C"

	targetdir       ("bin/" .. outdir .. "/%{prj.name}")
	objdir          ("obj/" .. outdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c"
	}

	includedirs {
		"%{prj.name}/src",
		"%{prj.name}/external/libsdl/include"
	}

	libdirs {
		"%{prj.name}/external/libsdl/lib/x86"
	}

	links {
		"SDL2.lib",
		"SDL2main.lib",
		"SDL2_image.lib",
                "SDL2_ttf.lib"
	}

        defines {
                "YUKI_EXPORT_DLL",
                "YUKI_ENABLE_ASSERTIONS"
        }

	filter "system:windows"
		systemversion           "latest"
                postbuildcommands       ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outdir .. "/testbed")

        filter "not system:windows"
                postbuildcommands       ("cp %{cfg.buildtarget.relpath} ../bin/" .. outdir .. "/testbed")

	filter "configurations:Debug"
		defines         "YUKI_CONFIG_DEBUG"
                runtime         "Debug"
		symbols         "On"

	filter "configurations:Release"
                defines         "YUKI_CONFIG_RELEASE"
		runtime         "Release"
		optimize        "On"
                



project "testbed"
        location        "testbed"
        kind            "ConsoleApp"
        language        "C"
        floatingpoint   "Fast"
        compileas       "C"

        targetdir       ("bin/" .. outdir .. "/%{prj.name}")
	objdir          ("obj/" .. outdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c"
	}

	includedirs {
		"engine/src",
                "engine/external/libsdl/include"
	}

	links {
                "engine"
	}

        defines {
                "YUKI_EXPORT_DLL",
                "YUKI_ENABLE_ASSERTIONS"
        }

	filter "configurations:Debug"
		defines         "YUKI_CONFIG_DEBUG"
                runtime         "Debug"
		symbols         "On"

	filter "configurations:Release"
                defines         "YUKI_CONFIG_RELEASE"
		runtime         "Release"
		optimize        "On"
