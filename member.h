#pragma once

// 멤버 추가
#define PACKET_ADD_MEMBER		1
#define PACKET_ADD_MEMBER_S		11
// 멤버 검색
#define PACKET_SEL_MEMBER		2
#define PACKET_SEL_MEMBER_S		21
#define PACKET_SEL_MEMBER_F		22
// 멤버 수정
#define PACKET_MOD_MEMBER		3
#define PACKET_MOD_MEMBER_S		31
#define PACKET_MOD_MEMBER_F		32
// 멤버 삭제
#define PACKET_DEL_MEMBER		4
#define PACKET_DEL_MEMBER_S		41
#define PACKET_DEL_MEMBER_F		42

// 추가 ( 멤버 추가에 사용할 구조체 ) 
struct AddMemberPacket
{
	int flag;
	char name[20];
	char phone[20];
	int age;
};
typedef AddMemberPacket AddMemberAckPacket;
// 검색 ( 멤버 검색에 사용할 구조체 ) 
struct SelMemberPacket
{
	int flag;
	char name[20];	
};
typedef AddMemberPacket SelMemberAckPacket;
// 수정 ( 멤버 수정에 사용할 구조체 ) 
typedef AddMemberPacket	ModMemberPacket;
typedef ModMemberPacket ModMemberAckPacket;
// 삭제 ( 멤버 삭제에 사용할 구조체 ) 
typedef SelMemberPacket	DelMemberPacket;
typedef DelMemberPacket	DelMemberAckPacket;