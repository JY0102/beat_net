#include "std.h"


int send_data(SOCKET s, char* buf, int len, int flag)
{
	int send_byte = send(s, (char*)&len, sizeof(int), flag);
	send_byte = send(s, buf, len, flag);

	return send_byte;
}

int recv_data(SOCKET s, char* buf, int len, int flag)
{
	// ��� recv
	int size;
	int ret = recvn(s, (char*)&size, sizeof(int), flag);
	if (ret == SOCKET_ERROR)
	{
		return SOCKET_ERROR;
	}
	else if (ret == 0)
	{
		return 0;
	}

	// ������ ũ�� ��ŭ  �����͸� ����.
	ret = recvn(s, buf, size, 0);
	return ret;
}
int recvn(SOCKET s, char* buf, int len, int flag)
{
	int recv_byte;
	char* ptr = buf;
	int left = len;

	while (left > 0)
	{
		recv_byte = recv(s, ptr, left, flag);
		if (recv_byte == SOCKET_ERROR)
		{
			return SOCKET_ERROR;
		}
		else if (recv_byte == 0)
		{
			break;
		}

		left -= recv_byte;
		ptr += recv_byte;
	}

	return (len - left);
}

