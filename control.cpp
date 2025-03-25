#include "std.h"

SOCKET sock;
vector <AddMemberPacket> members;

HANDLE hThread;

void con_init()
{
	// 소켓 생성.
	if (net_initlibrary() == false)
	{
		printf(" 소켓 초기화오류\n\n");
		return;
	}
	// 통신 소켓 생성 및 통신 소켓 핸들 값 return
	sock = net_create_socket(SERVER_IP, SERVER_PORT);
	if (sock == 0 )
	{
		printf(" 소켓 생성 오류 \n\n");
		return ;
	}
}
void con_run()
{	
	while (sock == INVALID_SOCKET);
	
	printf("------ 성공 ------\n\n\n");		
	while (1) 
	{	// 사용하고 싶은 함수 고르기
		printf("[1] 회원가입 [2] 검색 [3] 수정 [4] 제거 [5] 종료\n");

		char idx = _getch();
		// 1 번 부터 5번까지 선택.
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
			printf("오류-상대방 프로그램이 강제 종료\n");
			break;
		}
		else if (recv_byte == 0)
		{
			printf("상대방이 closesocket() 연결 종료\n");
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
	// flag 값 확인.
	int* flag = (int*)msg;

	switch (*flag)
	{
	// 멤버추가 성공
	case PACKET_ADD_MEMBER_S:
	{
		AddMemberAckPacket* member = (AddMemberAckPacket*)msg;
		printf(" 멤버 추가 성공 : %s\n" , member->name);
		break;
	}
	// 멤버 검색 성공
	case PACKET_SEL_MEMBER_S:
	{	
		SelMemberAckPacket* member = (SelMemberAckPacket*)msg;
		printf("검색 성공 \n");
		printf("이름		:%s\n", member->name);
		printf("전화번호	:%s\n", member->phone);
		printf("나이		:%d\n", member->age);
		break;		
	}
	// 멤버 검색 실패
	case PACKET_SEL_MEMBER_F:
	{
		printf("검색 실패 \n");
		break;
	}
	// 멤버 수정 성공
	case PACKET_MOD_MEMBER_S:
	{
		printf("정보 수정 성공 \n");
		break;
	}
	// 멤버 수정 실패
	case PACKET_MOD_MEMBER_F:
	{
		printf("정보 수정 실패 \n");
		break;
	}
	// 멤버 삭제 성공
	case PACKET_DEL_MEMBER_S:
	{
		printf("삭제 성공 \n");
		break;
	}
	// 멤버 삭제 실패
	case PACKET_DEL_MEMBER_F:
	{
		printf("삭제 실패 \n");
		break;
	}
	}

	return ;
}