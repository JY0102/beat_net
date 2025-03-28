#include "std.h"


void con_Init(HWND hDlg)
{
	ui_InitControl(hDlg);	
	net_CallBackFunction(hDlg, con_RecvData, con_LogData);		// 콜백함수 전달?
	net_InitNetwork(hDlg , ui_GetPort(hDlg));
}
void con_Exit(HWND hDlg)
{
	net_Exit();
}

void con_SendData(HWND hDlg)
{
	Packet packet;
	ui_GetSendData(hDlg, &packet);
	net_SendData(hDlg, packet);
}

void con_Multicast_Join(HWND hDlg)
{
	int idx = ui_GetComboBox_Idx(hDlg);

	TCHAR msg[30];
	bool b = net_Multicast_Join(idx);
	wsprintf(msg, _T("%d조 %s"), idx, b ? _T("가입성공") : _T("가입 실패"));
	con_LogData(msg);
}
void con_Multicast_Drop(HWND hDlg)
{
	int idx = ui_GetComboBox_Idx(hDlg);

	TCHAR msg[30];
	bool b = net_Multicast_Drop(idx);
	wsprintf(msg, _T("%d조 %s"), idx, b ? _T("탈퇴 성공") : _T("탈퇴 실패"));
	con_LogData(msg);
}


// CALLBACK : 미리 함수를 등록시킴 - > 특정상황에서 비동기적으로 부를 수 있음.
// recvThread 에서 callback 형식으로 메세지를 파싱.
void con_RecvData(const TCHAR* msg)
{
	Packet* packet = (Packet*)msg;

	ui_RecvMessagePrint(packet->name , packet->msg);
}
void con_LogData(const TCHAR* msg)
{	
	ui_LogMessagePrint(msg);
}