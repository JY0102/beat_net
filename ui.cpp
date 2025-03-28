#include "std.h"

HWND hEditView;
HWND hCombo;


void ui_InitControl(HWND hDlg)
{
	ui_SetInit(hDlg);
}
void ui_SetInit(HWND hDlg)
{
	hEditView = GetDlgItem(hDlg, IDC_EDIT1);
	hCombo    = GetDlgItem(hDlg, IDC_COMBO1);

	SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)_T("선택하세요"));
	SendMessage(hCombo, CB_ADDSTRING, 1, (LPARAM)_T("1조"));
	SendMessage(hCombo, CB_ADDSTRING, 2, (LPARAM)_T("2조"));
	SendMessage(hCombo, CB_ADDSTRING, 3, (LPARAM)_T("3조"));
	SendMessage(hCombo, CB_ADDSTRING, 4, (LPARAM)_T("4조"));
	SendMessage(hCombo, CB_ADDSTRING, 5, (LPARAM)_T("5조"));
	SendMessage(hCombo, CB_ADDSTRING, 6, (LPARAM)_T("6조"));

	SendMessage(hCombo, CB_SETCURSEL, 0, 0);
	SetDlgItemInt(hDlg, IDC_EDITPORT, SERVER_PORT , 0);
	SetDlgItemText(hDlg, IDC_EDITNAME, _T("이준영"));
}
int  ui_GetPort(HWND hDlg)
{	
	return (int)GetDlgItemInt(hDlg, IDC_EDITPORT, 0, 0);	
}

void ui_GetSendData(HWND hDlg, Packet* packet)
{	
	packet->ip_idx = (int)SendMessage(hCombo, CB_GETCURSEL, 0, 0);
	packet->port = (int)GetDlgItemInt(hDlg, IDC_EDITPORT, 0, 0);
	GetDlgItemText(hDlg, IDC_EDITNAME, packet->name, _countof(packet->name));
	GetDlgItemText(hDlg, IDC_EDITMSG, packet->msg, _countof(packet->msg));
}

int ui_GetComboBox_Idx(HWND hDlg)
{
	return (int)SendMessage(hCombo, CB_GETCURSEL, 0, 0);
}

// edit 여러줄 작업
void ui_RecvMessagePrint(const TCHAR* name , const TCHAR* msg)
{
	TCHAR buf[BUFSIZE/2];
	wsprintf(buf, _T("[이름 :%s] : %s"), name, msg);
	DWORD r;
	SendMessage(hEditView, EM_GETSEL, 0, (LPARAM)&r);
	SendMessage(hEditView, EM_REPLACESEL, r, (LPARAM)msg);
	SendMessage(hEditView, EM_REPLACESEL, r + _tcslen(msg), (LPARAM)_T("[Recv]\r\n"));	
}
void ui_LogMessagePrint(const TCHAR* msg)
{	
	DWORD r;
	SendMessage(hEditView, EM_GETSEL, 0,					(LPARAM)&r);
	SendMessage(hEditView, EM_REPLACESEL, r,				(LPARAM)msg);
	SendMessage(hEditView, EM_REPLACESEL, r +_tcslen(msg),	(LPARAM)_T("[Log]\r\n"));
}