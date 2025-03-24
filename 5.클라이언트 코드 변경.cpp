#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

//#define SERVER_IP_MAIN	"220.90.180.111"		//������
#define SERVER_IP_MAIN	"192.168.0.39"					//��
//#define SERVER_IP_MAIN	"192.168.0.139"			//�̻�
#define SERVER_PROT		9000


/*
1. �����û
	1_1 . ������Է�
		* ���ڿ� �Է½� break;
	1_2 . �Էµ� ���ڿ� �۽�
	1_3 . ����

	1_1 ~ 1_3 �ݺ�.
*/


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


	
	/*		���� ����		*/
	printf("------ ���� ��� ------\n\n\n");
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));					// �޸� �� 
	addr.sin_family = AF_INET;								// IPv4   // AF_INET6: IPv6 
	addr.sin_addr.s_addr = inet_addr(SERVER_IP_MAIN);		// ���� IP �ּ�
	addr.sin_port = htons(SERVER_PROT);						// ������ ���� ��Ʈ ��ȣ.
	// inet_addr( SERVER_IP_MAIN ) -> ���ڿ� -> ���� -> ntohl �� ��ȯ���� 

	while (1)
	{
		int ret = connect(sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
		if (ret == SOCKET_ERROR)
		{
			// �Է´��ǥ�� ���µ� ������. ( return�� �ȵ� )
			printf(" connect  ����\n");
			return -1;
		}
		printf("------ ���� ���� : (%s : %d )------\n\n\n",
			inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));

		while (1)
		{
			/*		�޼��� �۽�		*/
			char buf[1024];
			printf(">>");		gets_s(buf, sizeof(buf));
			int send_byte = send(sock, buf, (int)strlen(buf) + 1, 0);
			printf(">>[�۽�] %dbyte\n\n", send_byte);
			if (strlen(buf) <= 0)
			{
				printf(" ���� \n\n");
				break;
			}

			/*		�޼��� ����		*/
			int recv_byte = recv(sock, buf, sizeof(buf), 0);
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
			else
			{
				printf(">>[����] : %dbyte , %s \n\n", recv_byte, buf);
			}
		}
	}
	

	closesocket(sock);	
	/*		����		*/
	WSACleanup;
	return 0;
}