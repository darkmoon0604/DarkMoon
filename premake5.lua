
workspace "DarkMoon"
    architecture "x64"
    startproject "SandBox"

    configurations
    {
        "Debug",
        "Release"
    }


outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "DarkMoon/3rd/GLFW/include"
IncludeDir["Glad"] = "DarkMoon/3rd/Glad/include"
IncludeDir["imgui"] = "DarkMoon/3rd/imgui"
IncludeDir["glm"] = "DarkMoon/3rd/glm"

include "DarkMoon/3rd/GLFW"
include "DarkMoon/3rd/Glad"
include "DarkMoon/3rd/imgui"

project "DarkMoon"
    location "DarkMoon"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin-tmp/" .. outputDir .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "DarkMoon/src/pch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/3rd/glm/glm/**.hpp",
        "%{prj.name}/3rd/glm/glm/**.inl",
    }

    includedirs    
    {
        "%{prj.name}/src",
        "%{prj.name}/3rd/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.glm}",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "DM_PLATFORM_WINDOWS",
            "DM_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            --"DYNAMIC_LINK",
            "ENABLE_IMGUI",
        }

        -- postbuildcommands
        -- {
        --     ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/SandBox/")
        -- }

    filter "configurations:Debug"
        defines "DM_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "DM_RELEASE"
        runtime "Release"
        optimize "on"


project "SandBox"
    location "SandBox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

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
        "DarkMoon/src",
        "DarkMoon/3rd",
        "%{IncludeDir.glm}",
    }

    links
    {
        "DarkMoon"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "DM_PLATFORM_WINDOWS",
        }

    filter "configurations:Debug"
        defines "DM_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "DM_RELEASE"
        runtime "Release"
        optimize "on"