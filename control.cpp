#include "std.h"


void con_Init(HWND hDlg)
{
	ui_InitControl(hDlg);	
	net_CallBackFunction(hDlg, con_RecvData, con_LogData);		// �ݹ��Լ� ����?
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
	wsprintf(msg, _T("%d�� %s"), idx, b ? _T("���Լ���") : _T("���� ����"));
	con_LogData(msg);
}
void con_Multicast_Drop(HWND hDlg)
{
	int idx = ui_GetComboBox_Idx(hDlg);

	TCHAR msg[30];
	bool b = net_Multicast_Drop(idx);
	wsprintf(msg, _T("%d�� %s"), idx, b ? _T("Ż�� ����") : _T("Ż�� ����"));
	con_LogData(msg);
}


// CALLBACK : �̸� �Լ��� ��Ͻ�Ŵ - > Ư����Ȳ���� �񵿱������� �θ� �� ����.
// recvThread ���� callback �������� �޼����� �Ľ�.
void con_RecvData(const TCHAR* msg)
{
	Packet* packet = (Packet*)msg;

	ui_RecvMessagePrint(packet->name , packet->msg);
}
void con_LogData(const TCHAR* msg)
{	
	ui_LogMessagePrint(msg);
}