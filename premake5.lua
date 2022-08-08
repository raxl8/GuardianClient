workspace "Guardian"
    configurations { "Debug", "Release", "Production" }
    platforms "x64"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}/%{prj.name}"

    filter "configurations:Debug"
        defines { "_DEBUG", "GDN_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG", "GDN_RELEASE" }
        optimize "On"

    filter "configurations:Production"
        defines { "NDEBUG", "GDN_PRODUCTION" }
        optimize "On"

include "vendor"

project "Guardian"
    kind "WindowedApp"
    language "C++"
    cppdialect "C++20"

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    includedirs {
        "assets",
        "src" -- fix intellisense not finding pch.h
    }

    files { "src/**.h", "src/**.cpp" }
    submodules { "glfw", "imgui", "utfcpp", "fmt", "ixwebsocket" }

    if os.istarget('windows') then
        defines "_CRT_SECURE_NO_WARNINGS"
        links "opengl32"
        submodules "minhook"
    else
        removefiles "src/**.Win32.cpp"
    end
