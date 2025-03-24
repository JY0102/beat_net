#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#define SERVER_IP		"220.90.180.111"	//
#define SERVER_IP_MAIN	"10.101.41.203"		//IPv4 
#define SERVER_PROT		9000


/*
1. ���Ἲ��
	1_1. ������ ����
	1_2. ���ŵ� ������ �۽�
		ret: 0 or -1 �̸� �ݺ� ����
		*  1_1 ~ 1_2 �� ���ѹݺ�

* 1�� ������ ���� ����( �ٽ� ������ ��ٸ��ڴٴ� �� ) 
		



*/


int main()
{
	WSADATA wsa;


	/*		���� �ʱ�ȭ	  */
	// MAKEWORD(2,2) -> ver 2.2
	if (WSAStartup((MAKEWORD(2, 2)), &wsa) != 0)
	{
		printf(" ������ ���� �ʱ�ȭ ����");
		return -1;
	}

	/*		���� ����	  */
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// AF_INET : TCP / UDP //  SOCK_STREAM : TCP //  IPPROTO_TCP ,IPPROTO_UDP 
	if (listen_sock == INVALID_SOCKET)
	{
		printf(" ���� ���� ����\n");
		return -1;
	}
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));	// �޸� �� 
	/*
	sin_family			= IP version
	USHORT sin_port;	= ��Ʈ ��ȣ
	IN_ADDR sin_addr;
		s_addr
		s_host
		s_net
		s_imp
		s_impn
		s_lh
	CHAR sin_zero[8];
	*/
	
	addr.sin_family = AF_INET;					// IPv4   // AF_INET6: IPv6 
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	// ��� ������ IP �Ҵ�. ( ���� ) 
	addr.sin_port = htons(SERVER_PROT);			// ���� ��� �� ��Ʈ ��ȣ.

	//bind	( ���� ���������� ���� -> ���Ῡ�� os���� ���� ) 
	int ret = bind(listen_sock, (SOCKADDR*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		printf(" bind  ����\n");
		return -1;
	}

	// listen 
	ret = listen(listen_sock, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		printf(" listen  ����\n");
		return -1;
	}

	// ���� ��� �� �޼��� �ۼ���
	while (1)
	{
		/*		������		*/
		// IN �� ���� ������ ���

		printf("------ ���� ��� ------\n\n\n");
		SOCKADDR_IN client_addr;
		memset(&client_addr, 0, sizeof(SOCKADDR_IN));
		int client_length = sizeof(SOCKADDR_IN);

		static SOCKET client_sock = accept(listen_sock, (SOCKADDR*)&client_addr, &client_length);
		if (client_sock == INVALID_SOCKET)		
		{
			//listen_socket ��ü�� ������ �ƴ϶� return -1 �� ������ ����
			printf(" accept  ����\n");			
			continue;					// ���� �ٽ� ����
		}

		printf("------ ���� ���� : (%s : %d )------\n\n\n",
			inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		/*		�޼��� ����		*/
		char buf[1024];
		int recv_byte = recv(client_sock, buf, sizeof(buf), 0);
		printf(">>[����] : %dbyte , %s \n\n", recv_byte, buf);

		if (recv_byte == 0)
		{
			printf("----- ���� ������ ������ϴ� -----\n\n");
			break;
		}
		else if (recv_byte == -1)
		{
			printf("\n\n  ERROR  \ n\n");
			break;
		}
		/*		�޼��� �۽�		*/
		else
		{
			printf(">>");	gets_s(buf, sizeof(buf));							// ���� ��
			if (strlen(buf) <= 0)
			{
				printf(" ���� \n\n");
				break;
			}
			int send_byte = send(client_sock, buf, (int)strlen(buf) + 1, 0);	// send_byte == strlen(buf) + 1
			printf(">>[�۽�] %dbyte , %s \n\n", send_byte, buf);
		}
		
	}
	/*		����		*/	
	WSACleanup;
	return 0;
}

