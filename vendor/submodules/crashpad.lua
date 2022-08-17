return {
    include = function()
        includedirs { "crashpad", "../includes/crashpad" }
        submodules "mini_chromium"
    end,
    project = function()
        kind "StaticLib"
        language "C++"
        cppdialect "C++17"

        defines { "CLIENT_STACKTRACES_ENABLED", "CRASHPAD_ZLIB_SOURCE_EXTERNAL" }
        submodules "zlib"

        files_prefix "crashpad/client" {
            "annotation.cc",
            "annotation_list.cc",
            "crash_report_database.cc",
            "crashpad_info.cc",
            "prune_crash_reports.cc",
            "settings.cc",
        }

        files_prefix "crashpad/handler" {
            "crash_report_upload_thread.cc",
            "handler_main.cc",
            "minidump_to_upload_parameters.cc",
            "prune_crash_reports_thread.cc",
            "user_stream_data_source.cc",
        }

        files_prefix "crashpad/minidump" {
            "*.cc",
        }

        files_prefix "crashpad/snapshot" {
            "crashpad_types/crashpad_info_reader.cc",
            "minidump/*.cc",
            "*.cc",
        }

        files_prefix "crashpad/tools" {
            "tool_support.cc",
        }

        files_prefix "crashpad/util" {
            "file/file_helper.cc",
            "file/file_io.cc",
            "file/file_reader.cc",
            "file/file_seeker.cc",
            "file/file_writer.cc",
            "file/scoped_remove_file.cc",
            "misc/time.cc",
            "misc/metrics.cc",
            "misc/initialization_state_dcheck.cc",
            "misc/random_string.cc",
            "misc/uuid.cc",
            "misc/zlib.cc",
            "net/http_body_gzip.cc",
            "net/http_body.cc",
            "net/http_multipart_builder.cc",
            "net/http_transport.cc",
            "net/url.cc",
            "numeric/checked_address_range.cc",
            "process/process_memory.cc",
            "process/process_memory_range.cc",
            "stdlib/*.cc",
            "thread/thread.cc",
            "thread/worker_thread.cc",
        }

        files "crashpad/third_party/getopt/getopt.cc"

        if os.istarget('windows') then
            defines { "NOMINMAX", "WIN32_LEAN_AND_MEAN" }
            links { "version", "dbghelp", "powrprof", "winhttp" }
            includedirs "crashpad/compat/win"

            files_prefix "crashpad/client" {
                "crash_report_database_win.cc",
                "crashpad_client_win.cc"
            }

            files {
                "crashpad/handler/win/crash_report_exception_handler.cc",
            }

            files_prefix "crashpad/snapshot" {
                "win/*.cc",
            }

            files_prefix "crashpad/util" {
                "file/directory_reader_win.cc",
                "file/file_io_win.cc",
                "file/filesystem_win.cc",
                "misc/capture_context_win.asm",
                "misc/paths_win.cc",
                "misc/time_win.cc",
                "net/http_transport_win.cc",
                "process/process_memory_win.cc",
                "string/split_string.cc",
                "synchronization/semaphore_win.cc",
                "thread/thread_win.cc",
                "win/*.cc",
            }
        end

        removefiles "crashpad/**/*test*"
    end
}
