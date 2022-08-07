return {
    include = function()
        includedirs "include"
    end,
    project = function()
        kind "StaticLib"
        language "C"

        includedirs "library"
        files { "library/*.h", "library/*.c" }
    end
}
