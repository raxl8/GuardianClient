return {
    include = function()
        includedirs { "imgui", "imgui/backends" }
    end,
    project = function()
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        files_prefix "imgui" {
            "*.h",
            "*.cpp"
        }
        
        removefiles "imgui_demo.cpp"
    end
}
