#pragma once
#include"SocketBase.h"

namespace GuruLib
{
	class SocketBaseTCP :
		public SocketBase
	{
	private:
		bool ForceShutdownConnection();
	protected:
	public:
		SocketBaseTCP();
	};
}

