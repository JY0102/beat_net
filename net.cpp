#include "std.h"

// �Է� ���� �� ���������� ����
char name[20];
char phone[20];
int  age;

bool net_initlibrary()
{
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
// ��� ���ϻ���
SOCKET net_create_socket(const char* ip, int port)
{
	SOCKET temp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (temp_sock == INVALID_SOCKET)
	{
		printf("socket() ���� ����\n");
		return 0;
	}

	SOCKADDR_IN addr;
	memset(&addr, 0, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_port = htons(port);

	int ret = connect(temp_sock, (SOCKADDR*)&addr, sizeof(SOCKADDR_IN));
	if (ret == SOCKET_ERROR)
	{
		printf("���� ��û ����\n");
		return 0;
	}

	HANDLE hthread = CreateThread(0, 0, recv_thread, (void*)temp_sock, 0, 0);
	if(hthread == 0)
	{
		return 0;
	}

	CloseHandle(hthread);
	return temp_sock;
}
// ��� ���� ����
void net_delete_socket(SOCKET a)
{
	closesocket(a);
}

// ȸ�� ������ ����
void send_addmember(SOCKET socket)
{	
	printf(">> �̸�:");
	gets_s(name, sizeof(name));
	printf(">> ��ȭ��ȣ:");
	gets_s(phone, sizeof(phone));
	printf(">> ����:");
	scanf_s("%d", &age);
	getchar();
	AddMemberPacket packet;
	packet.flag = PACKET_ADD_MEMBER;
	strcpy_s(packet.name, sizeof(packet.name), name);
	strcpy_s(packet.phone, sizeof(packet.phone), phone);
	packet.age = age;

	send_data(socket, (char*)&packet, sizeof(packet), 0);
}
// �˻��ϰ� ���� ȸ�� �̸� ����
void send_selmember(SOCKET socket)
{
	printf(">> �̸�:");
	gets_s(name, sizeof(name));

	SelMemberPacket packet;
	packet.flag = PACKET_SEL_MEMBER;
	strcpy_s(packet.name, sizeof(packet.name), name);


	send_data(socket, (char*)&packet, sizeof(packet), 0);
}
// �����ϰ� ���� ȸ�� ���� ����
void send_modmember(SOCKET socket)
{
	printf(">> �̸�:");
	gets_s(name, sizeof(name));
	printf(">> ������ ��ȭ��ȣ:");
	gets_s(phone, sizeof(phone));
	printf(">> ������ ����:");
	scanf_s("%d", &age);
	getchar();
	AddMemberPacket packet;
	packet.flag = PACKET_MOD_MEMBER;
	strcpy_s(packet.name, sizeof(packet.name), name);
	strcpy_s(packet.phone, sizeof(packet.phone), phone);
	packet.age = age;
	
	send_data(socket, (char*)&packet, sizeof(packet), 0);
}
// �����ϰ� ���� ȸ�� �̸� ����
void send_delmember(SOCKET socket)
{
	printf(">> �̸�:");
	gets_s(name, sizeof(name));

	SelMemberPacket packet;
	packet.flag = PACKET_DEL_MEMBER;
	strcpy_s(packet.name, sizeof(packet.name), name);

	send_data(socket, (char*)&packet, sizeof(packet), 0);
}