#pragma once

#include "Core/Callback.h"
#include "Networking/PacketStream.h"

#include <ixwebsocket/IXWebSocket.h>

enum class WebsocketState
{
	Handshaking = 0,
	Etablished,
};

class Websocket
{
public:
	Websocket(const std::string& url);
	~Websocket();

	void Start();
	void Stop();
	bool IsConnected();
	bool SendPacket(PacketStream&& packet);

	static Callback<Websocket*> OnInitializeInstance;
	Callback<> OnConnect;
	Callback<> OnDisconnect;
	Callback<const std::string&> OnError;

private:
	void OnMessage(const ix::WebSocketMessagePtr& message);
	void HandlePacket(PacketStream&& packet);

private:
	ix::WebSocket m_Websocket;
	WebsocketState m_State;
};
