#include "NetDefine.h"

namespace Net
{
	bool socket_set_nonblocking(socket_t s, bool on)
	{
#if defined(WIN)
		return ioctlsocket(s, FIONBIO, (u_long *)&on) == 0;
#else
		int32_t opts = fcntl(s, F_GETFL, 0);
		if (opts < 0)
		{
			return false;
		}

		opts = on ? (opts | O_NONBLOCK) : (opts & ~O_NONBLOCK);
		fcntl(s, F_SETFL, opts);
		return opts >= 0;
#endif
	}
}
