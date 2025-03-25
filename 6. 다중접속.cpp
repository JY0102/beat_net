// 다중 접속 가능
// 1ㄷ1 통신  ,echo 통신

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>	
#include <winsock2.h>
// winsock 이랑 windows 충돌 될 수도 있음. ( 후 선언 )  
// #define WIN32_LEAN_AND_MEAN << Windows.h 와 winsock2 충돌 방지


#define SERVER_IP		"220.90.180.111"	//교수님
#define SERVER_IP_MAIN	"192.168.0.39"		//나
#define SERVER_PORT		9000

SOCKET listen_socket;

// 라이브러리 초기화 및 종료처리
bool net_initlibrary();
void net_exitlibrary();

// 소켓 생성 및 초기화 + 종료처리
bool net_create_socket(int port);
void net_delete_socket();

// 클라이언트 접속을 대기하는 스레드
// accept -> 통신 스레드 생성.
unsigned long CALLBACK listen_thread(void* temp);
// 접속된 클라이언트와 통신 recv -> send
unsigned long CALLBACK work_thread(void* p);


int main()
{
	/*		소켓 초기화	  */
	if (net_initlibrary() == false)
	{
		printf(" 소켓 초기화오류\n\n");
		return -1;
	}

	/*		소켓 생성	  */
	if (net_create_socket(SERVER_PORT) == false)
	{
		printf(" 소켓 생성 오류 \n\n");
		return -1;
	}
	
	/*		연결대기		*/
	printf("------ 연결 대기 ------\n\n\n");
	HANDLE hThread = CreateThread(0, 0, listen_thread, 0, 0, 0);
	if (hThread == 0)
	{
		printf(" 스레스생성오류 \n");
		return -1;
	}
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);

	//exit
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

bool net_create_socket(int port)
{
	listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// AF_INET : TCP / UDP //  SOCK_STREAM : TCP //  IPPROTO_TCP ,IPPROTO_UDP 
	if (listen_socket == INVALID_SOCKET)
	{
		printf(" 소켓 생성 실패\n");
		return false;
	}
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));	// 메모리 셋 

	addr.sin_family = AF_INET;					// IPv4   // AF_INET6: IPv6 
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	// 사용 가능한 IP 할당. ( 랜덤 ) 
	addr.sin_port = htons(port);			// 내가 사용 할 포트 번호.

	//bind	( 망에 직접적으로 연결 -> 연결여부 os에서 질문 ) 
	int ret = bind(listen_socket, (SOCKADDR*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		printf(" bind  실패\n");
		return false;
	}

	// listen 
	ret = listen(listen_socket, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		printf(" listen  실패\n");
		return false;
	}

	return true;
}
void net_delete_socket()
{
	closesocket(listen_socket);
}

unsigned long CALLBACK listen_thread(void* temp)
{
	
	SOCKET client_sock;
	int client_length = sizeof(SOCKADDR_IN);

	while (true)
	{
		// IN 을 쓰는 이유을 상기
		SOCKADDR_IN client_addr;
		memset(&client_addr, 0, sizeof(SOCKADDR_IN));

		SOCKET client_socket = accept(listen_socket, (SOCKADDR*)&client_addr, &client_length);
		if (client_socket == INVALID_SOCKET)
		{
			//listen_socket 자체의 오류가 아니라 return -1 할 이유가 없음
			printf(" accept  실패\n");
			continue;					// 따라서 다시 시작
		}

		printf("------ 연결 성공 : (%s : %d )------\n\n\n",
			inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		DWORD tid;
		HANDLE hThread = CreateThread(0, 0, work_thread, (void*)client_socket, 0, &tid);
		if (hThread == 0)
		{
			printf(" workthread 오류\n");
			return -1;
		}

		CloseHandle(hThread);
	}
	return 0
;
}
unsigned long CALLBACK work_thread(void* p)
{
	/*		메세지 수신		*/

	SOCKET client_socket = (SOCKET)p;
	char buf[1024];

	while (true)
	{		
		memset(buf, 0, sizeof(buf));
		int recv_byte = recv(client_socket, buf, sizeof(buf), 0);
		printf(">>[수신] : %dbyte , %s \n\n", recv_byte, buf);

		if (recv_byte == 0)
		{
			printf("----- 상대방 연결이 끊겼습니다 -----\n\n");
			break;
		}
		else if (recv_byte == -1)
		{
			printf("\n\n  ERROR  \ n\n");
			break;
		}
		/*		메세지 송신		*/
		else
		{
			printf(">>");	gets_s(buf, sizeof(buf));							// 보낼 값
			if (strlen(buf) <= 0)
			{
				printf(" 종료 \n\n");
				break;
			}
			int send_byte = send(client_socket, buf, (int)strlen(buf) + 1, 0);	// send_byte == strlen(buf) + 1
			printf(">>[송신] %dbyte , %s \n\n", send_byte, buf);
		}
	}

	SOCKADDR_IN client_addr;
	int client_length = sizeof(SOCKADDR);
	// 내 소켓 정보
	//getsockname(client_socket, (SOCKADDR*)&client_addr, &client_length);
	// 상대방 소켓 정보
	getpeername(client_socket, (SOCKADDR*)&client_addr, &client_length);

	printf("------ 클라이언트 연결 종료 : (%s : %d )------\n\n\n",
		inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	closesocket(client_socket);
	return 0;
}