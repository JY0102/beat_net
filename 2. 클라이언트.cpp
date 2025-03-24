#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
// inet_addr �� IPv4 �� ����

#define SERVER_IP		"220.90.180.111"	//
#define SERVER_IP_MAIN	"10.101.41.203"		//IPv4  , ���� ��� IP
#define SERVER_PROT		9000

int main()
{
	WSADATA wsa;

		
	/*		���� �ʱ�ȭ		*/
	// MAKEWORD(2,2) -> ver 2.2
	if (WSAStartup((MAKEWORD(2, 2)), &wsa) != 0)
	{
		printf(" ������ ���� �ʱ�ȭ ����");
		return -1;
	}

	/*		���� ����	  */
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// AF_INET : TCP / UDP //  SOCK_STREAM : TCP //  IPPROTO_TCP ,IPPROTO_UDP 
	if (sock == INVALID_SOCKET)
	{
		printf(" ���� ���� ����\n");
		return -1;
	}
	
	
	while (1)
	{
	/*		���� ����		*/
	printf("------ ���� ��� ------\n\n\n");
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));					// �޸� �� 
	addr.sin_family			= AF_INET;						// IPv4   // AF_INET6: IPv6 
	addr.sin_addr.s_addr	= inet_addr(SERVER_IP_MAIN);	// ��� ������ IP �Ҵ�.	htonl (int �� IP)  // inet_addr (���ڿ� �� IP )
	addr.sin_port			= htons(SERVER_PROT);			// ���� ��� �� ��Ʈ ��ȣ
	// inet_addr( SERVER_IP_MAIN ) -> ���ڿ� -> ���� -> ntohl �� ��ȯ���� 

	int ret = connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (ret == SOCKET_ERROR)
	{
		// �Է´��ǥ�� ���µ� ������. ( return�� �ȵ� )
		printf(" connect  ����\n");
		return -1;
	}
	printf("------ ���� ���� : (%s : %d )------\n\n\n",
		inet_ntoa(addr.sin_addr), htons(addr.sin_port));


		/*		�޼��� �۽�		*/
		char buf[1024];
		printf(">>");		gets_s(buf, sizeof(buf));
		int send_byte = send(sock, buf, (int)strlen(buf) + 1, 0);
		printf(">>[�۽�] %dbyte\n\n", send_byte);

		/*		�޼��� ����		*/
		int recv_byte = recv(sock, buf, sizeof(buf), 0);
		printf(">>[����] : %dbyte , %s \n\n", recv_byte, buf);



		/*		����		*/
		WSACleanup();
	}
	return 0;
}