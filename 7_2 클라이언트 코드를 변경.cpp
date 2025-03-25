// 7_2 클라리언트 코드 변경

/*
연결 성공 -> 수신 스레드 생성
*/
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <Windows.h>

#define SERVER_IP_MAIN	"220.90.180.111"		//교수님
//#define SERVER_IP_MAIN	"192.168.0.39"			//나
//#define SERVER_IP_MAIN	"192.168.0.139"			//이상영
#define SERVER_PROT		9000

SOCKET sock;

bool net_initlibrary();
void net_exitlibrary();

// 소켓 생성 및 초기화 + 종료처리
bool net_create_socket(const char* ip , int port);
void net_delete_socket();

// 클라이언트 접속을 대기하는 스레드
// accept -> 통신 스레드 생성.
unsigned long CALLBACK recv_thread(void* temp);
// 접속된 클라이언트와 통신 recv -> send
unsigned long CALLBACK work_thread(void* p);

int main()
{
	/*		소켓 초기화		*/
	if (net_initlibrary() == 0)	return -1;

	/*		소켓 생성 + 연결 + 스레드 생성	*/
	if (net_create_socket(SERVER_IP_MAIN, SERVER_PROT) == 0)	return -1;	

	while (1)
	{
		/*		메세지 송신		*/
		char buf[1024];
		printf(">>");	 gets_s(buf, sizeof(buf));
		if (strlen(buf) <= 0)
		{
			printf(" 종료 \n\n");
			break;
		}
		int send_byte = send(sock, buf, (int)strlen(buf) + 1, 0);
		printf(">>[송신] %dbyte\n\n", send_byte);				
	}

	/*		종료		*/
	net_delete_socket();
	net_exitlibrary();
	return 0;
}


bool net_initlibrary()
{
	// MAKEWORD(2,2) -> ver 2.2
	WSADATA wsa;

	if (WSAStartup((MAKEWORD(2, 2)), &wsa) != 0)
	{
		return false;
	}
	return true;
}
void net_exitlibrary()
{
	WSACleanup();
}

bool net_create_socket(const char* ip, int port) 
{	
	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// AF_INET : TCP / UDP //  SOCK_STREAM : TCP //  IPPROTO_TCP ,IPPROTO_UDP 
	if (sock == INVALID_SOCKET)
	{
		printf(" 소켓 생성 실패\n");
		return false;
	}

	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));				// 메모리 셋 
	addr.sin_family = AF_INET;							// IPv4   // AF_INET6: IPv6 
	addr.sin_addr.s_addr = inet_addr(ip);				// 상대방 IP 주소		// 범용타입 안됨. -> 엤날꺼라
	addr.sin_port = htons(port);						// 서버의 수신 포트 번호.

	/*		소켓 연결		*/
	printf("------ 연결 대기 ------\n\n\n");

	int ret = connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (ret == SOCKET_ERROR)
	{
		// 입력대기표에 들어갔는데 못받음. ( return이 안됨 )
		printf(" connect  실패\n");
		return false;
	}
	printf("------ 연결 성공 : (%s : %d )------\n\n\n",
		inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

	// 스레드생성
	HANDLE hThread;
	hThread = CreateThread(0, 0, recv_thread, (void*)sock, 0, 0);
	if (hThread == 0)
	{
		return false;
	}

	return true;
}
void net_delete_socket()
{
	closesocket(sock);
}

unsigned long CALLBACK recv_thread(void* temp)
{
	SOCKET a = (SOCKET)temp;
	/*		메세지 수신		*/
	char buf[1024];
	while (true)
	{
		memset(buf, 0, sizeof(buf));
		int recv_byte = recv(sock, buf, sizeof(buf), 0);
		if (recv_byte == 0)
		{
			printf("----- 상대방 연결이 끊겼습니다 -----\n\n");
			break;
		}
		else if (recv_byte == -1)
		{
			printf("\n\n  ERROR  \n\n");
			break;
		}
		else
		{
			printf("[수신] : %dbyte , %s \n\n", recv_byte, buf);
			printf(">>");
		}
	}

	return 0;
}

