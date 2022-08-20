#include "pch.h"

#include "Hooking/HookFunction.h"
#include "Networking/PacketID.h"
#include "Networking/Websocket.h"

static HookFunction hookFunction([]()
{
	Websocket::OnInitializeInstance.Subscribe([](Websocket* instance)
	{
		instance->OnHandshaked.Subscribe([instance]()
		{
			static std::mutex conditionMutex;
			static std::condition_variable conditionVariable;

			std::thread([instance]()
			{
				while (instance->IsConnected())
				{
					PacketStream packet;
					packet.Write<PacketID>(PacketID::Heartbeat);
					if (!instance->SendPacket(packet))
						continue;

					std::unique_lock lock(conditionMutex);
					conditionVariable.wait_for(lock, std::chrono::seconds(5));
				}
			}).detach();

			instance->OnDisconnect.Subscribe([]()
			{
				conditionVariable.notify_one();
			});
		});
	});
});
