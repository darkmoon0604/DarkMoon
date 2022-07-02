
workspace "DarkMoon"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "Release"
    }


outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "DarkMoon/3rd/GLFW/include"

include "DarkMoon/3rd/GLFW"

project "DarkMoon"
    location "DarkMoon"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-tmp/" .. outputDir .. "/%{prj.name}")

    pchheader "dmpch.h"
    pchsource "DarkMoon/dmpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs    
    {
        "%{prj.name}/src",
        "%{prj.name}/3rd/spdlog/include",
        "%{IncludeDir.GLFW}",
    }

    links
    {
        "GLFW",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest" --"10.0"

        defines
        {
            "DM_PLATFORM_WINDOWS",
            "DM_BUILD_DLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/SandBox/")
        }

    filter "configurations:Debug"
        defines "DM_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DM_RELESE"
        optimize "On"


project "SandBox"
    location "SandBox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-tmp/" .. outputDir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs
    {
        "DarkMoon/3rd/spdlog/include",
        "DarkMoon/src"
    }

    links
    {
        "DarkMoon"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest" --"10.0"

        defines
        {
            "DM_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "DM_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "DM_RELESE"
        optimize "On"