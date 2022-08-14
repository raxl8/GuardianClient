#include "pch.h"

#include "Hooking/HookFunction.h"
#include "Networking/PacketID.h"
#include "Networking/Websocket.h"

static std::mutex g_HeartbeatMutex;
static std::condition_variable g_HeartbeatConditionVariable;
static std::thread g_HeartbeatThread;

static HookFunction hookFunction([]()
{
	Websocket::OnInitializeInstance.Subscribe([](Websocket* instance)
	{
		instance->OnHandshaked.Subscribe([instance]()
		{
			g_HeartbeatThread = std::thread([instance]()
			{
				while (instance->IsConnected())
				{
					PacketStream packet;
					packet.Write<PacketID>(PacketID::Heartbeat);
					instance->SendPacket(packet);

					std::unique_lock lock(g_HeartbeatMutex);
					g_HeartbeatConditionVariable.wait_for(lock, std::chrono::seconds(30));
				}
			});

			instance->OnDisconnect.Subscribe([]()
			{
				if (g_HeartbeatThread.joinable())
				{
					g_HeartbeatConditionVariable.notify_one();
					// Don't try to join on current thread, would cause deadlock
					if (g_HeartbeatThread.get_id() != std::this_thread::get_id())
						g_HeartbeatThread.join();
				}
			});
		});
	});
});
