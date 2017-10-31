#pragma once
#include"SocketBase.h"

namespace GuruLib
{
	class SocketBaseUDP :
		public SocketBase
	{
	private:
	protected:
		int udp_send_max_msg_size;
	public:
		SocketBaseUDP();
	};
}

