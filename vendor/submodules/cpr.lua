return {
    include = function()
        includedirs "cpr/include"
        submodules "curl"
    end,
    project = function()
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        files "cpr/cpr/*.cpp"
    end
}
