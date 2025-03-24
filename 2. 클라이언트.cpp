#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
// inet_addr 은 IPv4 만 지원

#define SERVER_IP		"220.90.180.111"	//
#define SERVER_IP_MAIN	"10.101.41.203"		//IPv4  , 받을 사람 IP
#define SERVER_PROT		9000

int main()
{
	WSADATA wsa;

		
	/*		소켓 초기화		*/
	// MAKEWORD(2,2) -> ver 2.2
	if (WSAStartup((MAKEWORD(2, 2)), &wsa) != 0)
	{
		printf(" 윈도우 소켓 초기화 실패");
		return -1;
	}

	/*		소켓 생성	  */
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// AF_INET : TCP / UDP //  SOCK_STREAM : TCP //  IPPROTO_TCP ,IPPROTO_UDP 
	if (sock == INVALID_SOCKET)
	{
		printf(" 소켓 생성 실패\n");
		return -1;
	}
	
	
	while (1)
	{
	/*		소켓 연결		*/
	printf("------ 연결 대기 ------\n\n\n");
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));					// 메모리 셋 
	addr.sin_family			= AF_INET;						// IPv4   // AF_INET6: IPv6 
	addr.sin_addr.s_addr	= inet_addr(SERVER_IP_MAIN);	// 사용 가능한 IP 할당.	htonl (int 값 IP)  // inet_addr (문자열 값 IP )
	addr.sin_port			= htons(SERVER_PROT);			// 내가 사용 할 포트 번호
	// inet_addr( SERVER_IP_MAIN ) -> 문자열 -> 정수 -> ntohl 로 변환까지 

	int ret = connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (ret == SOCKET_ERROR)
	{
		// 입력대기표에 들어갔는데 못받음. ( return이 안됨 )
		printf(" connect  실패\n");
		return -1;
	}
	printf("------ 연결 성공 : (%s : %d )------\n\n\n",
		inet_ntoa(addr.sin_addr), htons(addr.sin_port));


		/*		메세지 송신		*/
		char buf[1024];
		printf(">>");		gets_s(buf, sizeof(buf));
		int send_byte = send(sock, buf, (int)strlen(buf) + 1, 0);
		printf(">>[송신] %dbyte\n\n", send_byte);

		/*		메세지 수신		*/
		int recv_byte = recv(sock, buf, sizeof(buf), 0);
		printf(">>[수신] : %dbyte , %s \n\n", recv_byte, buf);



		/*		종료		*/
		WSACleanup();
	}
	return 0;
}