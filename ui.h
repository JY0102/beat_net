#pragma once


void ui_InitControl(HWND hDlg);
void ui_SetInit(HWND hDlg);
int  ui_GetPort(HWND hDlg);

void ui_GetSendData(HWND hDlg, Packet* packet);

int ui_GetComboBox_Idx(HWND hDlg);

void ui_RecvMessagePrint(const TCHAR* name, const TCHAR* msg);
void ui_LogMessagePrint(const TCHAR* msg);