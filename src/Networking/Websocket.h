#pragma once

#include "Core/Callback.h"
#include "Networking/PacketStream.h"

#include <ixwebsocket/IXWebSocket.h>

enum class WebsocketState
{
	Idle = 0,
	Handshaked,
};

class Websocket
{
public:
	Websocket(const std::string& url);
	~Websocket();

	void Start();
	void Stop();
	bool IsConnected();
	bool SendPacket(const PacketStream& packet);

	static Callback<Websocket*> OnInitializeInstance;
	Callback<> OnConnect;
	Callback<> OnHandshaked;
	Callback<> OnDisconnect;
	Callback<const std::string&> OnError;

private:
	void OnMessage(const ix::WebSocketMessagePtr& message);
	void HandlePacket(PacketStream&& packet);

private:
	ix::WebSocket m_Websocket;
	WebsocketState m_State;
	std::mutex m_ConnectMutex;
	std::condition_variable m_ConnectCondition;
};
