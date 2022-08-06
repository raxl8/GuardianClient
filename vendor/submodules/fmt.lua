return {
    include = function()
        includedirs "include"
    end,
    project = function()
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"

        files "src/*.cc"
        removefiles "src/fmt.cc"
    end
}
