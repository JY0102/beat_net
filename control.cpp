#include "std.h"

SOCKET sock;
vector <AddMemberPacket> members;

HANDLE hThread;

void con_init()
{
	// ���� ����.
	if (net_initlibrary() == false)
	{
		printf(" ���� �ʱ�ȭ����\n\n");
		return;
	}
	// ��� ���� ���� �� ��� ���� �ڵ� �� return
	sock = net_create_socket(SERVER_IP, SERVER_PORT);
	if (sock == 0 )
	{
		printf(" ���� ���� ���� \n\n");
		return ;
	}
}
void con_run()
{	
	while (sock == INVALID_SOCKET);
	
	printf("------ ���� ------\n\n\n");		
	while (1) 
	{	// ����ϰ� ���� �Լ� ����
		printf("[1] ȸ������ [2] �˻� [3] ���� [4] ���� [5] ����\n");

		char idx = _getch();
		// 1 �� ���� 5������ ����.
		if (idx == '1') send_addmember(sock);
		else if (idx == '2') send_selmember(sock);
		else if (idx == '3') send_modmember(sock);
		else if (idx == '4') send_delmember(sock);
		else if (idx == '5') break;
		
		Sleep(1000);	//****
		printf("\n");
	}
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);	
}

void con_exit()
{
	net_delete_socket(sock);
	net_exitlibrary();
}

unsigned long __stdcall recv_thread(void* p)
{
	sock = (SOCKET)p;

	char recv_buf[1024];
	while (true)
	{
		memset(recv_buf, 0, sizeof(recv_buf));
		int recv_byte = recv_data(sock, recv_buf, sizeof(recv_buf), 0);
		if (recv_byte == -1)
		{
			printf("����-���� ���α׷��� ���� ����\n");
			break;
		}
		else if (recv_byte == 0)
		{
			printf("������ closesocket() ���� ����\n");
			break;
		}
		else
		{			
			msg_parsing(recv_buf);
		}
	}
	return 0;
}

void msg_parsing(char* msg)
{	
	// flag �� Ȯ��.
	int* flag = (int*)msg;

	switch (*flag)
	{
	// ����߰� ����
	case PACKET_ADD_MEMBER_S:
	{
		AddMemberAckPacket* member = (AddMemberAckPacket*)msg;
		printf(" ��� �߰� ���� : %s\n" , member->name);
		break;
	}
	// ��� �˻� ����
	case PACKET_SEL_MEMBER_S:
	{	
		SelMemberAckPacket* member = (SelMemberAckPacket*)msg;
		printf("�˻� ���� \n");
		printf("�̸�		:%s\n", member->name);
		printf("��ȭ��ȣ	:%s\n", member->phone);
		printf("����		:%d\n", member->age);
		break;		
	}
	// ��� �˻� ����
	case PACKET_SEL_MEMBER_F:
	{
		printf("�˻� ���� \n");
		break;
	}
	// ��� ���� ����
	case PACKET_MOD_MEMBER_S:
	{
		printf("���� ���� ���� \n");
		break;
	}
	// ��� ���� ����
	case PACKET_MOD_MEMBER_F:
	{
		printf("���� ���� ���� \n");
		break;
	}
	// ��� ���� ����
	case PACKET_DEL_MEMBER_S:
	{
		printf("���� ���� \n");
		break;
	}
	// ��� ���� ����
	case PACKET_DEL_MEMBER_F:
	{
		printf("���� ���� \n");
		break;
	}
	}

	return ;
}