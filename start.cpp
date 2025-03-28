#include "std.h"

#define SAVE	ID_40001
#define LOAD	ID_40002

INT_PTR CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:		return OnInitDialLog(hDlg, msg, wParam, lParam);
	case WM_COMMAND:		return OnCommand(hDlg, msg, wParam, lParam);
	}

	return FALSE;
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPTSTR cmdLine, int nShowCmd)
{

	INT_PTR ret = DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc);

	return 0;
}