#include "zt_udp.h"

#include <winsock2.h>

int create_udp(unsigned short port)
{
    SOCKET s;
	SOCKADDR_IN addr;
	int buf_size = 1024 * 512;
	s = socket(AF_INET,SOCK_DGRAM,0);

	setsockopt(s, SOL_SOCKET, SO_RCVBUF, (void *)&buf_size, sizeof(int));
	setsockopt(s, SOL_SOCKET, SO_SNDBUF, (void *)&buf_size, sizeof(int));

	memset(&addr,0,sizeof(struct sockaddr_in));
	addr.sin_family 	= AF_INET;
	addr.sin_port       = htons(port);
	addr.sin_addr.s_addr= htonl(INADDR_ANY);
	if (bind(s, (struct sockaddr *)&addr, sizeof(struct sockaddr)) < 0) {
		return -1;
	}

	*fd = s;
	return 0;
}