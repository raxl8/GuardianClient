return {
    include = function()
        includedirs { "include" }
    end,
    project = function()
        kind "StaticLib"
        language "C"

        files {
            "src/context.c",
            "src/egl_context.h",
            "src/egl_context.c",
            "src/init.c",
            "src/input.c",
            "src/internal.h",
            "src/mappings.h",
            "src/monitor.c",
            "src/osmesa_context.h",
            "src/osmesa_context.c",
            "src/vulkan.c",
            "src/window.c",
        }

        if os.istarget('windows') then
            defines "_GLFW_WIN32"
            files {
                "src/wgl_context.h",
                "src/wgl_context.c",
                "src/win32_*.h",
                "src/win32_*.c"
            }
        end
    end
}
