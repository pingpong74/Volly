workspace "Volly"
    configurations { "Release", "Debug" }

project "Volly"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++23"

    targetdir "bin/%{cfg.buildcfg}"
    objdir "bin/temp"

    files { "src/**.hpp", "src/**.cpp", "thirdparty/**.c", "thirdparty/**.h", "includes/**.hpp" }

    includedirs {
        "/usr/include/glm",        -- glm
        "/usr/include/GLFW",       -- GLFW headers
        "/usr/include/vulkan",      -- Vulkan headers
        "includes/",
        "thirdparty/"
    }

    libdirs {
        "/usr/lib"
    }

    links {
        "glfw",
        "vulkan"
    }

    filter { "configurations:Debug" }
        defines { "DEBUG" }
        symbols "On"

    filter { "configurations:Release" }
        defines { "RELEASE" }
        optimize "On"

    filter {} -- Reset filter after configs
