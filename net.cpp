#include "std.h"

// 입력 받을 값 전역번수로 지정
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
// 통신 소켓생성
SOCKET net_create_socket(const char* ip, int port)
{
	SOCKET temp_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (temp_sock == INVALID_SOCKET)
	{
		printf("socket() 생성 오류\n");
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
		printf("연결 요청 에러\n");
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
// 통신 소켓 삭제
void net_delete_socket(SOCKET a)
{
	closesocket(a);
}

// 회원 정보를 전송
void send_addmember(SOCKET socket)
{	
	printf(">> 이름:");
	gets_s(name, sizeof(name));
	printf(">> 전화번호:");
	gets_s(phone, sizeof(phone));
	printf(">> 나이:");
	scanf_s("%d", &age);
	getchar();
	AddMemberPacket packet;
	packet.flag = PACKET_ADD_MEMBER;
	strcpy_s(packet.name, sizeof(packet.name), name);
	strcpy_s(packet.phone, sizeof(packet.phone), phone);
	packet.age = age;

	send_data(socket, (char*)&packet, sizeof(packet), 0);
}
// 검색하고 싶은 회원 이름 전송
void send_selmember(SOCKET socket)
{
	printf(">> 이름:");
	gets_s(name, sizeof(name));

	SelMemberPacket packet;
	packet.flag = PACKET_SEL_MEMBER;
	strcpy_s(packet.name, sizeof(packet.name), name);


	send_data(socket, (char*)&packet, sizeof(packet), 0);
}
// 수정하고 싶은 회원 정보 전송
void send_modmember(SOCKET socket)
{
	printf(">> 이름:");
	gets_s(name, sizeof(name));
	printf(">> 수정할 전화번호:");
	gets_s(phone, sizeof(phone));
	printf(">> 수정할 나이:");
	scanf_s("%d", &age);
	getchar();
	AddMemberPacket packet;
	packet.flag = PACKET_MOD_MEMBER;
	strcpy_s(packet.name, sizeof(packet.name), name);
	strcpy_s(packet.phone, sizeof(packet.phone), phone);
	packet.age = age;
	
	send_data(socket, (char*)&packet, sizeof(packet), 0);
}
// 삭제하고 싶은 회원 이름 전송
void send_delmember(SOCKET socket)
{
	printf(">> 이름:");
	gets_s(name, sizeof(name));

	SelMemberPacket packet;
	packet.flag = PACKET_DEL_MEMBER;
	strcpy_s(packet.name, sizeof(packet.name), name);

	send_data(socket, (char*)&packet, sizeof(packet), 0);
}