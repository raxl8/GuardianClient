#pragma once

enum class PacketID : uint16_t
{
	Error = 0,
	Handshake,
	Heartbeat,
};
