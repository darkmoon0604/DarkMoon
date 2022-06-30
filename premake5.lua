
-- 工程名字
workspace "DarkMoon"
    architecture "x64"
    
    configurations
    {
        "Debug",
        "Release"
    }

-- 输入目录
outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- 动态链接库名字
project "DarkMoon"
    location "DarkMoon"
    kind "SharedLib"
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
        "%{prj.name}/3rd/spdlog/include"
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