#include "pch.h"

#include "Websocket.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXUserAgent.h>

Websocket::Websocket(const std::string& url)
{
	if (!ix::initNetSystem())
		FatalError();

	m_Websocket.setUrl(url);

	m_Websocket.setOnMessageCallback(std::bind(&Websocket::OnMessage, this, std::placeholders::_1));
	m_Websocket.disableAutomaticReconnection();
}

Websocket::~Websocket()
{
	ix::uninitNetSystem();
}

void Websocket::Connect()
{
	m_Websocket.start();
}

bool Websocket::SendPacket(PacketStream&& packet)
{
	PacketStream finalPacket;
	auto packetData = packet.GetData();
	auto size = (int)packetData.size();
	finalPacket.Write(size);
	finalPacket.WriteBytes(packetData);

	auto data = finalPacket.GetData();
	auto sendInfo = m_Websocket.sendBinary(data);
	return sendInfo.success;
}

void Websocket::OnMessage(const ix::WebSocketMessagePtr& message)
{
	switch (message->type)
	{
	case ix::WebSocketMessageType::Open: {
		OnConnected();
		break;
	}
	case ix::WebSocketMessageType::Close:
		OnDisconnected();
		break;
	case ix::WebSocketMessageType::Error:
		OnError(message->errorInfo.reason);
		break;
	case ix::WebSocketMessageType::Message:
		printf("%I64d\n", message->str.size());
		break;
	default:
		break;
	}
}
