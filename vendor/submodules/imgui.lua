return {
    include = function()
        includedirs { ".", "backends" }
    end,
    project = function()
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        files { "*.h", "*.cpp" }
        removefiles "imgui_demo.cpp"
    end
}
