workspace "Guardian"
    configurations { "Debug", "Release" }
    platforms "x64"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}/%{prj.name}"

    filter "configurations:Debug"
        defines "_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NDEBUG"
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

    links "opengl32"
    submodules { "glfw", "imgui" }
