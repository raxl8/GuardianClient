return {
    include = function()
        includedirs "mbedtls/include"
    end,
    project = function()
        kind "StaticLib"
        language "C"

        includedirs "mbedtls/library"
        files_prefix "mbedtls/library" {
            "*.h",
            "*.c"
        }
    end
}
