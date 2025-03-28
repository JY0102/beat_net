#pragma once

typedef void (*Func)(const TCHAR*);

bool net_InitNetwork(HWND hDlg, int port);
void net_Exit();

unsigned long __stdcall RecvThread(void* p);

bool net_Multicast_Join(int idx);
bool net_Multicast_Drop(int idx);

void net_SendData(HWND hDlg, Packet packet);

void net_CallBackFunction(HWND hDlg, Func RecvFunc, Func LogFunc);
