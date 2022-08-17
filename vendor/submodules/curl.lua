return {
    include = function()
        defines "CURL_STATICLIB"
        includedirs "curl/include"
    end,
    project = function()
        kind "StaticLib"
        language "C"

        defines { "BUILDING_LIBCURL", "USE_MBEDTLS", "USE_NGHTTP2" }
        submodules { "mbedtls", "nghttp2" }

        includedirs "curl/lib"
        files_prefix "curl/lib" {
            "*.c",
            "vauth/*.c",
            "vtls/vtls.c",
            "vtls/mbedtls*.c",
        }

        if os.istarget('windows') then
            links "Wldap32"
        end
    end
}
