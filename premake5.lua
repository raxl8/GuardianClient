workspace "Guardian"
    configurations { "Debug", "Optimized", "Release" }
    platforms "x64"
    targetdir "bin/%{cfg.buildcfg}"
    objdir "obj/%{cfg.buildcfg}/%{prj.name}"

    filter "Debug"
        defines { "_DEBUG", "GDN_DEBUG" }
        symbols "On"

    filter "Optimized or Release"
        defines "NDEBUG"
        optimize "On"

    filter "Release"
        defines "GDN_RELEASE"

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
    submodules { "glfw", "imgui", "utfcpp", "fmt", "ixwebsocket", "json", "cpr", "crashpad", "stb", "oneTBB" }

    if os.istarget('windows') then
        defines "_CRT_SECURE_NO_WARNINGS"
        links { "opengl32", "comctl32", "dwmapi", "uxtheme" }
        linkoptions "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\""
        submodules "minhook"
    else
        removefiles "src/**.Win32.cpp"
    end
