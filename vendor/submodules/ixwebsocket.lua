return {
    include = function()
        includedirs "ixwebsocket"
    end,
    project = function()
        kind "StaticLib"
        language "C++"
        cppdialect "C++11"

        files_prefix "ixwebsocket/ixwebsocket" {
            "*.h",
            "*.cpp"
        }

        defines {
            "IXWEBSOCKET_USE_TLS",
            "IXWEBSOCKET_USE_MBED_TLS",
            "IXWEBSOCKET_USE_MBED_TLS_MIN_VERSION_3"
        }
        submodules "mbedtls"

        if os.istarget("windows") then
            links { "ws2_32", "crypt32" }
        end
    end
}
