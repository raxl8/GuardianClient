return {
    include = function()
        includedirs "minhook/include"
    end,
    project = function()
        kind "StaticLib"
        language "C"

        files_prefix "minhook/src" {
            "**.h",
            "**.c"
        }
    end
}
