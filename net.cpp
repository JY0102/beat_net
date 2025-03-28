//net.cpp
#include "std.h"

char multicast_ip[][30] = {
	"-" ,
	"234.5.5.11",
	"234.5.5.12",
	"234.5.5.13",
	"234.5.5.14",
	"234.5.5.15",
	"234.5.5.16",
};

SOCKET recv_sock;
SOCKET send_sock;
// 함수 보관?
Func call_RecvFunc,call_LogFunc;
void net_CallBackFunction(HWND hDlg, Func RecvFunc, Func LogFunc)
{	// 콜백함수 등록?
	call_RecvFunc = RecvFunc;
	call_LogFunc = LogFunc;
}

// lib -> socket -> bind
bool net_InitNetwork(HWND hDlg , int port)
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;		
	//recv socket
	recv_sock = socket(AF_INET, SOCK_DGRAM ,  IPPROTO_UDP);
	if (recv_sock == INVALID_SOCKET)
	{
		printf("recv_socket() 생성 오류\n");
		return false;
	}

	BOOL bEnable1 = TRUE;
	int retval = setsockopt(recv_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&bEnable1, sizeof(bEnable1));
	if (retval == SOCKET_ERROR)
		return false;

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	retval = bind(recv_sock, (SOCKADDR*)&addr, sizeof(addr));
	if (retval == SOCKET_ERROR)
	{
		call_LogFunc(_T("bind에러"));
		return false;
	}
	CloseHandle(CreateThread(0, 0, RecvThread, 0, 0, 0));

	//send socket
	send_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (send_sock == INVALID_SOCKET)
	{
		call_LogFunc(_T("send_socket() 생성 오류\n"));
		return false;
	}

	BOOL bEnable = TRUE;
	retval = setsockopt(recv_sock, SOL_SOCKET, SO_BROADCAST, (char*)&bEnable, sizeof(bEnable));
	if (retval == SOCKET_ERROR)
		return false;

	call_LogFunc(TEXT("수신스레드실행"));
	return true;
}
void net_Exit()
{
	closesocket(recv_sock);
	closesocket(send_sock);
	WSACleanup();
}

unsigned long __stdcall RecvThread(void* p)
{
	char buf[BUFSIZE];

	while (true)
	{
		ZeroMemory(buf, sizeof(buf));	

		SOCKADDR_IN	clientaddr;
		int	addrlen = sizeof(clientaddr);
		int retval = recvfrom(recv_sock, buf, sizeof(buf), 0,(SOCKADDR*)&clientaddr, &addrlen);
		//return 0 이 존재 할 수 없음.
		if (retval == SOCKET_ERROR)
			continue;				
				
		call_RecvFunc((TCHAR*)buf);		
		Sleep(5);
	}
	return 0;
}

bool net_Multicast_Join(int idx)
{
	ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(multicast_ip[idx]);
	mreq.imr_interface.s_addr = INADDR_ANY;
	int ret = setsockopt(recv_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));
	if (ret == SOCKET_ERROR)
		return false;
		
	return true;
}
bool net_Multicast_Drop(int idx)
{
	ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(multicast_ip[idx]);
	mreq.imr_interface.s_addr = INADDR_ANY;
	int ret = setsockopt(recv_sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
		(char*)&mreq, sizeof(mreq));
	if (ret == SOCKET_ERROR)
		return false;

	return true;
}

void net_SendData(HWND hDlg, Packet packet)
{	
	// 소켓 주소 구조체 초기화
	SOCKADDR_IN sendaddr;
	ZeroMemory(&sendaddr, sizeof(sendaddr));
	sendaddr.sin_family = AF_INET;
	sendaddr.sin_port = htons(packet.port);
	sendaddr.sin_addr.s_addr = inet_addr(multicast_ip[packet.ip_idx]);

	int ret = sendto(send_sock, (char*)&packet, sizeof(packet), 0,(SOCKADDR*)&sendaddr, sizeof(sendaddr));
	if (ret == SOCKET_ERROR)
	{
		call_LogFunc(_T("메세지 전송 오류"));
		return;
	}
	Sleep(5);
}



