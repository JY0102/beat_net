#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>

#define size 1024

void exam1();
void exam2();

void tmep();

int main()
{
	exam2();
	
	return 0;
}


// 1. IP 주소 변환
/*
inet_addr() : 프로그램 -> 소켓 함수에게 문자열 전달 시
inet_ntoa() : 소켓함수 -> 프로그램으로 IP 획득 할 때.
*/
void exam1()
{
	char ip[512] = "230.200.12.5";
	printf("%s\n", ip);

	// 문자열-> 숫자 : inet_addr();
	int number_ip = inet_addr(ip);
	printf("0x%08X\n", number_ip);

	//숫자 -> 문자열	: inet_ntoa(INT_ADDR)	
	IN_ADDR in_addr;
	in_addr.s_addr = number_ip;

	printf("%s\n", inet_ntoa(in_addr));
}


void tmep()
{
	IN_ADDR test;
	test.S_un.S_un_b.s_b1 = 1;
	test.S_un.S_un_b.s_b1 = 2;
	test.S_un.S_un_b.s_b1 = 3;
	test.S_un.S_un_b.s_b1 = 4;

	int value = test.S_un.S_addr;
	printf("0x%08X\n", value);
}

//2. byte 정렬
/*
1) 리틀엔디안	: 작은 주소 부터 저장
2) 빅엔디안		: 큰 주소 부터 저장

서로 상반된다면 메모리 주소가 뒤집힘.

소켓 : 빅엔디안사용(약속)				-> 네트워크 바이트 정렬
일반PC : 리틀 or 빅 엔디안 중 하나	-> 호스트 바이트 정렬

프로그램 -> 소켓함수 호출 : 네트워크 바이트 정렬로 변환 후 호출
소켓함수 -> 프로그램 호출 : 호스트 바이트 정렬로 변환 후 호출
*/
void exam2()
{
	unsigned short us = 0x1234;			// 2byte
	unsigned long  ul = 0x12345678;		// 4byte

	// 호스트 -> 네트워크					host  to network short
	printf("0x%08X = > 0x%08X\n", us, htons(us));
	printf("0x%08X = > 0x%08X\n", ul, htonl(ul));
	//									host  to network long

	unsigned short n_us = htons(us);		// 2byte
	unsigned long  n_ul = htonl(ul);		// 4byte

	// 네트워크 -> 호스트					network  to host short
	printf("0x%08X = > 0x%08X\n", n_us, ntohs(n_us));
	printf("0x%08X = > 0x%08X\n", n_ul, ntohl(n_ul));
	//									network  to host long
}