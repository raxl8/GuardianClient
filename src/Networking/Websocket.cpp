#include "pch.h"

#include "Websocket.h"

#include "Core/Application.h"
#include "Networking/PacketID.h"
#include "Networking/ErrorID.h"

#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXUserAgent.h>

Callback<Websocket*> Websocket::OnInitializeInstance;

Websocket::Websocket(const std::string& url)
	: m_State(WebsocketState::Idle)
{
	if (!ix::initNetSystem())
		FatalError();

	m_Websocket.setUrl(url);

	m_Websocket.setOnMessageCallback(std::bind(&Websocket::OnMessage, this, std::placeholders::_1));
	m_Websocket.disableAutomaticReconnection();

	OnConnect.Subscribe([this]() {
		m_ConnectCondition.notify_one();

		PacketStream packet;
		packet.Write<PacketID>(PacketID::Handshake);
		packet.Write<uint16_t>(WEBSOCKET_PROTOCOL_VERSION);

		SendPacket(packet);
	});

	OnInitializeInstance(this);
}

Websocket::~Websocket()
{
	Stop();

	ix::uninitNetSystem();
}

void Websocket::Start()
{
	m_Websocket.start();

	{
		std::unique_lock lock(m_ConnectMutex);
		m_ConnectCondition.wait_for(lock, std::chrono::seconds(20));
	}

	if (!IsConnected())
		Instance<Application>::Get()->GetUserInterface()->DisplayError("Websocket Error", "Could not reach server, maybe down? Try again at a later time.");
}

void Websocket::Stop()
{
	m_Websocket.stop();
}

bool Websocket::IsConnected()
{
	return m_Websocket.getReadyState() == ix::ReadyState::Open;
}

bool Websocket::SendPacket(const PacketStream& packet)
{
	PacketStream finalPacket;
	const auto& packetData = packet.GetData();
	auto size = (int)packetData.size();
	finalPacket.Write(size);
	finalPacket.WriteBytes(packetData);

	ix::IXWebSocketSendData sendData(finalPacket.GetData());
	auto sendInfo = m_Websocket.sendBinary(sendData);
	return sendInfo.success;
}

void Websocket::OnMessage(const ix::WebSocketMessagePtr& message)
{
	switch (message->type)
	{
	case ix::WebSocketMessageType::Open:
		OnConnect();
		break;
	case ix::WebSocketMessageType::Close:
		OnDisconnect();
		break;
	case ix::WebSocketMessageType::Error:
		m_ConnectCondition.notify_one();
		OnError(message->errorInfo.reason);
		break;
	case ix::WebSocketMessageType::Message:
	{
		std::vector<uint8_t> data(message->str.size());
		std::memcpy(&data[0], &message->str[0], data.size());

		// We move the buffer as we don't need it outside of this function
		HandlePacket(PacketStream(std::move(data)));
		break;
	}
	default:
		break;
	}
}

void Websocket::HandlePacket(PacketStream&& packet)
{
	auto size = packet.Read<uint16_t>();
	if (packet.GetData().size() - sizeof(uint16_t) != size)
		return;

	auto packetID = packet.Read<PacketID>();
	switch (packetID)
	{
	case PacketID::Error:
	{
		m_Websocket.close();

		std::string errorTitle, errorDescription;
		auto errorID = packet.Read<ErrorID>();
		switch (errorID)
		{
		case ErrorID::TimedOut:
			errorTitle = "Websocket Error";
			errorDescription = "Websocket server estimated connection timed out.";
			break;
		case ErrorID::OutdatedClient:
			errorTitle = "Client Error";
			errorDescription = "The version of the client you are using is outdated.\nPlease download the latest from our website.";
			break;
		case ErrorID::OutdatedServer:
			errorTitle = "Server Error";
			errorDescription = "The version of the server is outdated.\nThis is very unlikely, try to wait a bit and try again.";
			break;
		case ErrorID::Unknown:
		default:
			errorTitle = "Unknown Error";
			errorDescription = "An unknown error occured, server returned an unknown error code.";
			break;
		}

		Instance<Application>::Get()->GetUserInterface()->DisplayError(errorTitle, errorDescription);
		break;
	}
	case PacketID::Handshake:
		m_State = WebsocketState::Handshaked;
		OnHandshaked();
		break;
	default:
		break;
	}
}
