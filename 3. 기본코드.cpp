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


// 1. IP �ּ� ��ȯ
/*
inet_addr() : ���α׷� -> ���� �Լ����� ���ڿ� ���� ��
inet_ntoa() : �����Լ� -> ���α׷����� IP ȹ�� �� ��.
*/
void exam1()
{
	char ip[512] = "230.200.12.5";
	printf("%s\n", ip);

	// ���ڿ�-> ���� : inet_addr();
	int number_ip = inet_addr(ip);
	printf("0x%08X\n", number_ip);

	//���� -> ���ڿ�	: inet_ntoa(INT_ADDR)	
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

//2. byte ����
/*
1) ��Ʋ�����	: ���� �ּ� ���� ����
2) �򿣵��		: ū �ּ� ���� ����

���� ��ݵȴٸ� �޸� �ּҰ� ������.

���� : �򿣵�Ȼ��(���)				-> ��Ʈ��ũ ����Ʈ ����
�Ϲ�PC : ��Ʋ or �� ����� �� �ϳ�	-> ȣ��Ʈ ����Ʈ ����

���α׷� -> �����Լ� ȣ�� : ��Ʈ��ũ ����Ʈ ���ķ� ��ȯ �� ȣ��
�����Լ� -> ���α׷� ȣ�� : ȣ��Ʈ ����Ʈ ���ķ� ��ȯ �� ȣ��
*/
void exam2()
{
	unsigned short us = 0x1234;			// 2byte
	unsigned long  ul = 0x12345678;		// 4byte

	// ȣ��Ʈ -> ��Ʈ��ũ					host  to network short
	printf("0x%08X = > 0x%08X\n", us, htons(us));
	printf("0x%08X = > 0x%08X\n", ul, htonl(ul));
	//									host  to network long

	unsigned short n_us = htons(us);		// 2byte
	unsigned long  n_ul = htonl(ul);		// 4byte

	// ��Ʈ��ũ -> ȣ��Ʈ					network  to host short
	printf("0x%08X = > 0x%08X\n", n_us, ntohs(n_us));
	printf("0x%08X = > 0x%08X\n", n_ul, ntohl(n_ul));
	//									network  to host long
}