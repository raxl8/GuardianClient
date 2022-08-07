return {
    include = function()
        includedirs "fmt/include"
    end,
    project = function()
        kind "StaticLib"
        language "C++"
        cppdialect "C++20"

        files "fmt/src/*.cc"
        removefiles "fmt/src/fmt.cc"
    end
}
