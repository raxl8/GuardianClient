return {
    include = function()
        includedirs "zlib"
    end,
    project = function()
        kind "StaticLib"
        language "C"

        files "zlib/*.c"
    end
}
