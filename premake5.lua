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
IncludeDir["spdlog"]   = "vendor/spdlog/include"
IncludeDir["GLFW"]   = "vendor/GLFW/include"
IncludeDir["Glad"]   = "vendor/Glad/include"
IncludeDir["ImGui"]  = "vendor/ImGui"
IncludeDir["glm"]    = "vendor/glm"

include "vendor/GLFW"
include "vendor/Glad"
include "vendor/ImGui"

project "BlackHole"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "bhpch.h"
    pchsource "src/bhpch.cpp"

    files
    {
        "src/**.h",
        "src/**.cpp",
        "%{IncludeDir.glm}/**.hpp",
        "%{IncludeDir.glm}/**.inl",
    }

    includedirs
    {
        "src/",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}"
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "BH_PLATFORM_WINDOWS",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines { "BH_DEBUG", "BH_ENABLE_ASSERTS" }
        runtime "Debug"
        symbols "on"
        
    filter "configurations:Release"
        defines { "BH_RELEASE" }
        runtime "Release"
        optimize "on"