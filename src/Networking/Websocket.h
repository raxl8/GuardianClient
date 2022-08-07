#pragma once

#include "Core/Callback.h"
#include "Networking/PacketStream.h"

#include <ixwebsocket/IXWebSocket.h>

class Websocket
{
public:
	Websocket(const std::string& url);
	~Websocket();

	void Connect();
	bool SendPacket(PacketStream&& packet);

	Callback<> OnConnected;
	Callback<> OnDisconnected;
	Callback<const std::string&> OnError;

private:
	void OnMessage(const ix::WebSocketMessagePtr& message);

private:
	ix::WebSocket m_Websocket;
};
