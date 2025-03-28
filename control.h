#pragma once


void con_Init(HWND hDlg);
void con_Exit(HWND hDlg);

void con_SendData(HWND hDlg);

void con_Multicast_Join(HWND hDlg);
void con_Multicast_Drop(HWND hDlg);


void con_RecvData(const TCHAR* msg);
void con_LogData(const TCHAR* msg);
