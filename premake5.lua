workspace "BlackHole"
    architecture "x64"

    configurations
    {
        "Debug",
        "Release"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to the root folder
IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"

include "vendor/GLFW"

project "BlackHole"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "bhpch.h"
    pchsource "src/bhpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp"
    }

    includedirs
    {
        "src/",
        "vendor/spdlog/include",
        "%{IncludeDir.GLFW}"
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines
        {
            "BH_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "BH_DEBUG", "BH_ENABLE_ASSERTS" }
        symbols "On"
        
    filter "configurations:Release"
        defines { "BH_RELEASE" }
        optimize "On"