return {
    include = function()
        includedirs "mini_chromium"
    end,
    project = function()
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        files_prefix "mini_chromium/base" {
            "files/file_path.cc",
            "process/memory.cc",
            "strings/string_number_conversions.cc",
            "strings/string_util.cc",
            "strings/stringprintf.cc",
            "strings/utf_string_conversion_utils.cc",
            "strings/utf_string_conversions.cc",
            "synchronization/lock.cc",
            "third_party/icu/icu_utf.cc",
            "logging.cc",
            "rand_util.cc",
        }

        if os.istarget('windows') then
            defines "NOMINMAX"

            files_prefix "mini_chromium/base" {
                "memory/page_size_win.cc",
                "strings/string_util_win.cc",
                "synchronization/lock_impl_win.cc",
                "scoped_clear_last_error_win.cc",
            }
        end
    end
}
