return {
    include = function()
        includedirs { "include" }
    end,
    project = function()
        kind "StaticLib"
        language "C"

        files { "src/**.h", "src/**.c" }
    end
}
