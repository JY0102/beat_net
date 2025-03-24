#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
//#pragma comment (lib," ws2_32.lib")
// 라이브러리 등록.

#define SERVER_IP		"1.2.3.4"
#define SERVER_IP_MAIN	"10.101.41.203"
#define SERVER_PROT		9000




int main()
{
	WSADATA wsa;


	/*		소켓 초기화	  */
	// MAKEWORD(2,2) -> ver 2.2
	if (WSAStartup((MAKEWORD(2, 2)), &wsa) != 0)
	{
		printf(" 윈도우 소켓 초기화 실패");
		return -1;
	}

	/*		소켓 생성	  */
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	// AF_INET : TCP / UDP //  SOCK_STREAM : TCP //  IPPROTO_TCP ,IPPROTO_UDP 
	if (listen_sock == INVALID_SOCKET)
	{
		printf(" 소켓 생성 실패\n");
		return -1;
	}
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));	// 메모리 셋 
	/*
	sin_family			= IP version
	USHORT sin_port;	= 포트 번호
	IN_ADDR sin_addr;
		s_addr
		s_host
		s_net
		s_imp
		s_impn
		s_lh
	CHAR sin_zero[8];
	*/

	addr.sin_family			= AF_INET;				// IPv4   // AF_INET6: IPv6 
	addr.sin_addr.s_addr	= htonl(INADDR_ANY);	// 사용 가능한 IP 할당. ( 랜덤 ) 
	addr.sin_port			= htons(SERVER_PROT);	// 내가 사용 할 포트 ㅓㄴ호.

	//bind	( 망에 직접적으로 연결 -> 연결여부 os에서 질문 ) 
	int ret = bind(listen_sock,(SOCKADDR*)&addr ,sizeof(addr ));
	if (ret == SOCKET_ERROR)
	{
		printf(" bind  실패\n");
		return -1;
	}

	// listen 
	ret = listen(listen_sock ,SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		printf(" listen  실패\n");
		return -1;
	}


	while (1)
	{
	/*		연결대기		*/
	// IN 을 쓰는 이유을 상기

	printf("------ 연결 대기 ------\n\n\n");
	SOCKADDR_IN client_addr;
	memset(&client_addr, 0, sizeof(SOCKADDR_IN));
	int client_length = sizeof(SOCKADDR_IN);

	SOCKET client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &client_length);
	if (client_sock == INVALID_SOCKET)
	{
		printf(" accept  실패\n");
		return -1;
	}

	printf("------ 연결 성공 : (%s : %d )------\n\n\n",
		inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));




	/*		메세지 수신		*/
	char buf[1024];
	int recv_byte = recv(client_sock, buf, sizeof(buf), 0);
	printf(">>[수신] : %dbyte , %s \n\n", recv_byte, buf);

	if (recv_byte == 0)
	{
		printf("----- 상대방 연결이 끊겼습니다 -----\n\n");
	}
	else if (recv_byte == -1)
	{
		printf("\n\n  ERROR  \ n\n");
	}	

	/*		메세지 송신		*/
	printf(">>");	gets_s(buf, sizeof(buf));							// 보낼 값
	int send_byte = send(client_sock, buf, (int)strlen(buf) + 1, 0);	// send_byte == strlen(buf) + 1
	printf(">>[송신] %dbyte , %s \n\n", send_byte, buf);


	/*		종료		*/
	WSACleanup();
	}
	return 0;
}