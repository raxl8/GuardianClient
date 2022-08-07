return {
    include = function()
        includedirs "glfw/include"
    end,
    project = function()
        kind "StaticLib"
        language "C"

        files_prefix "glfw/src" {
            "context.c",
            "egl_context.h",
            "egl_context.c",
            "init.c",
            "input.c",
            "internal.h",
            "mappings.h",
            "monitor.c",
            "osmesa_context.h",
            "osmesa_context.c",
            "vulkan.c",
            "window.c",
        }

        if os.istarget('windows') then
            defines "_GLFW_WIN32"
            files_prefix "glfw/src" {
                "wgl_context.h",
                "wgl_context.c",
                "win32_*.h",
                "win32_*.c"
            }
        end
    end
}
