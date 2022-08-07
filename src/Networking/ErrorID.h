#pragma once

enum class ErrorID : uint16_t
{
	Unknown = 0,
	TimedOut,
	OutdatedClient,
	OutdatedServer,
};
