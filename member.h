#pragma once

// ��� �߰�
#define PACKET_ADD_MEMBER		1
#define PACKET_ADD_MEMBER_S		11
// ��� �˻�
#define PACKET_SEL_MEMBER		2
#define PACKET_SEL_MEMBER_S		21
#define PACKET_SEL_MEMBER_F		22
// ��� ����
#define PACKET_MOD_MEMBER		3
#define PACKET_MOD_MEMBER_S		31
#define PACKET_MOD_MEMBER_F		32
// ��� ����
#define PACKET_DEL_MEMBER		4
#define PACKET_DEL_MEMBER_S		41
#define PACKET_DEL_MEMBER_F		42

// �߰� ( ��� �߰��� ����� ����ü ) 
struct AddMemberPacket
{
	int flag;
	char name[20];
	char phone[20];
	int age;
};
typedef AddMemberPacket AddMemberAckPacket;
// �˻� ( ��� �˻��� ����� ����ü ) 
struct SelMemberPacket
{
	int flag;
	char name[20];	
};
typedef AddMemberPacket SelMemberAckPacket;
// ���� ( ��� ������ ����� ����ü ) 
typedef AddMemberPacket	ModMemberPacket;
typedef ModMemberPacket ModMemberAckPacket;
// ���� ( ��� ������ ����� ����ü ) 
typedef SelMemberPacket	DelMemberPacket;
typedef DelMemberPacket	DelMemberAckPacket;