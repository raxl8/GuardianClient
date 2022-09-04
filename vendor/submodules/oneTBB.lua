return {
    include = function()
        defines "__TBB_NO_IMPLICIT_LINKAGE=1"
        includedirs "oneTBB/include"
    end,
    project = function()
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        defines { "__TBB_BUILD", "__TBB_DYNAMIC_LOAD_ENABLED=0" }

        includedirs "oneTBB/src"
        files "oneTBB/src/tbb/*.cpp"

        if os.istarget('windows') then
		    defines { 'USE_WINTHREAD', '_WIN32_WINNT=0x0601' }
        end
    end
}
