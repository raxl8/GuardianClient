return {
	include = function()
		includedirs { "nghttp2/lib/includes", "../includes/nghttp2" }

		defines { '_SSIZE_T_DEFINED=1', '_U_=', 'NGHTTP2_STATICLIB' }
		
		if os.istarget('windows') then
			defines 'ssize_t=__int64'
		end
	end,
	project = function()
		language "C"
		kind "StaticLib"

		flags "NoRuntimeChecks"

		files "nghttp2/lib/*.c"
	end
}