// ���� ���� ����
// 1��1 ���  ,echo ���

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <Windows.h>	
#include <winsock2.h>
// winsock �̶� windows �浹 �� ���� ����. ( �� ���� )  
// #define WIN32_LEAN_AND_MEAN << Windows.h �� winsock2 �浹 ����


#define SERVER_IP		"220.90.180.111"	//������
#define SERVER_IP_MAIN	"192.168.0.39"		//��
#define SERVER_PORT		9000

SOCKET listen_socket;

// ���̺귯�� �ʱ�ȭ �� ����ó��
bool net_initlibrary();
void net_exitlibrary();

// ���� ���� �� �ʱ�ȭ + ����ó��
bool net_create_socket(int port);
void net_delete_socket();

// Ŭ���̾�Ʈ ������ ����ϴ� ������
// accept -> ��� ������ ����.
unsigned long CALLBACK listen_thread(void* temp);
// ���ӵ� Ŭ���̾�Ʈ�� ��� recv -> send
unsigned long CALLBACK work_thread(void* p);


int main()
{
	/*		���� �ʱ�ȭ	  */
	if (net_initlibrary() == false)
	{
		printf(" ���� �ʱ�ȭ����\n\n");
		return -1;
	}

	/*		���� ����	  */
	if (net_create_socket(SERVER_PORT) == false)
	{
		printf(" ���� ���� ���� \n\n");
		return -1;
	}
	
	/*		������		*/
	printf("------ ���� ��� ------\n\n\n");
	HANDLE hThread = CreateThread(0, 0, listen_thread, 0, 0, 0);
	if (hThread == 0)
	{
		printf(" �������������� \n");
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
		printf(" ���� ���� ����\n");
		return false;
	}
	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));	// �޸� �� 

	addr.sin_family = AF_INET;					// IPv4   // AF_INET6: IPv6 
	addr.sin_addr.s_addr = htonl(INADDR_ANY);	// ��� ������ IP �Ҵ�. ( ���� ) 
	addr.sin_port = htons(port);			// ���� ��� �� ��Ʈ ��ȣ.

	//bind	( ���� ���������� ���� -> ���Ῡ�� os���� ���� ) 
	int ret = bind(listen_socket, (SOCKADDR*)&addr, sizeof(addr));
	if (ret == SOCKET_ERROR)
	{
		printf(" bind  ����\n");
		return false;
	}

	// listen 
	ret = listen(listen_socket, SOMAXCONN);
	if (ret == SOCKET_ERROR)
	{
		printf(" listen  ����\n");
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
		// IN �� ���� ������ ���
		SOCKADDR_IN client_addr;
		memset(&client_addr, 0, sizeof(SOCKADDR_IN));

		SOCKET client_socket = accept(listen_socket, (SOCKADDR*)&client_addr, &client_length);
		if (client_socket == INVALID_SOCKET)
		{
			//listen_socket ��ü�� ������ �ƴ϶� return -1 �� ������ ����
			printf(" accept  ����\n");
			continue;					// ���� �ٽ� ����
		}

		printf("------ ���� ���� : (%s : %d )------\n\n\n",
			inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		DWORD tid;
		HANDLE hThread = CreateThread(0, 0, work_thread, (void*)client_socket, 0, &tid);
		if (hThread == 0)
		{
			printf(" workthread ����\n");
			return -1;
		}

		CloseHandle(hThread);
	}
	return 0
;
}
unsigned long CALLBACK work_thread(void* p)
{
	/*		�޼��� ����		*/

	SOCKET client_socket = (SOCKET)p;
	char buf[1024];

	while (true)
	{		
		memset(buf, 0, sizeof(buf));
		int recv_byte = recv(client_socket, buf, sizeof(buf), 0);
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
			int send_byte = send(client_socket, buf, (int)strlen(buf) + 1, 0);	// send_byte == strlen(buf) + 1
			printf(">>[�۽�] %dbyte , %s \n\n", send_byte, buf);
		}
	}

	SOCKADDR_IN client_addr;
	int client_length = sizeof(SOCKADDR);
	// �� ���� ����
	//getsockname(client_socket, (SOCKADDR*)&client_addr, &client_length);
	// ���� ���� ����
	getpeername(client_socket, (SOCKADDR*)&client_addr, &client_length);

	printf("------ Ŭ���̾�Ʈ ���� ���� : (%s : %d )------\n\n\n",
		inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

	closesocket(client_socket);
	return 0;
}