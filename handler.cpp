#include "std.h"



INT_PTR OnInitDialLog(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	con_Init(hDlg);
	return TRUE;
}
INT_PTR OnCommand(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{	
	switch(LOWORD(wParam))
	{
	case IDCANCEL:
	{
		con_Exit(hDlg);
		EndDialog(hDlg, IDCANCEL);
		return TRUE;
	}
	case IDC_BUTTONSEND:
	{
		con_SendData(hDlg);
		return TRUE;
	}
	case IDC_BUTTON2:
	{
		con_Multicast_Join(hDlg);
		return TRUE;
	}	
	case IDC_BUTTON3:
	{
		con_Multicast_Drop(hDlg);
		return TRUE;
	}
	}

	return FALSE;	
}