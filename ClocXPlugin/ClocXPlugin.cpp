#include "ClocXPlugin.h"
#include <tchar.h>
#include <stdio.h>
#include <shellapi.h>
#include "debugtrace.h"

UINT_PTR g_timerID;
HHOOK g_hook;
HWND g_hwMainWindow;
HINSTANCE g_hPluginInstance;


BOOL APIENTRY DllMain( HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	g_timerID = 0;
	g_hook = NULL;
	g_hwMainWindow = NULL;
	g_hPluginInstance = hinstDLL;

	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

inline BOOL IsRectInsideRect(const LPRECT r1, const LPRECT r2)
{ //r1 inside r2
return (r1->top >= r2->top) && 
	   (r1->bottom <= r2->bottom) && 
	   (r1->right <= r2->right) &&
	   (r2->left <= r1->left);
}

void WINAPI TimerProc(HWND hWnd, UINT msg, UINT_PTR timerID, DWORD sysMsec)
{
	DBGTRACE("TimerProc called\n");
	QUERY_USER_NOTIFICATION_STATE quns;
	if SUCCEEDED(SHQueryUserNotificationState(&quns))
	{
		DBGTRACE("UserNotificationState=%s\n", DbgTraceUserNotificationStateA(quns));
		if (quns == QUNS_ACCEPTS_NOTIFICATIONS) //don't interrupt full-screen apps
		{
			SetWindowPos(g_hwMainWindow, HWND_TOPMOST, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		}
		else 
		{
			HMONITOR hMonitor = MonitorFromWindow(g_hwMainWindow, MONITOR_DEFAULTTOPRIMARY);
			if (hMonitor == NULL) return;
			MONITORINFO mi;
			mi.cbSize = sizeof(mi);
			if (!GetMonitorInfo(hMonitor, &mi)) return;

			HWND hwfg = GetForegroundWindow();
			RECT fgwrc;
			if (!GetWindowRect(hwfg, &fgwrc)) return;

			if (IsRectInsideRect(&mi.rcMonitor, &fgwrc)) //fullscreen window may extend outside screen
			{
				DBGTRACE("RectInsideRect\n");
				SetWindowPos(g_hwMainWindow, hwfg, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			}
			else
			{
				SetWindowPos(g_hwMainWindow, HWND_TOPMOST, 0,0,0,0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
			}

		}
	}
}

/* OnInit is called when module is loaded (but not when it is loaded only for configuration */
EXPORT int OnInit(InitData *idat)
{
	g_hwMainWindow = idat->MainWindow;  //handle to ClocX main window
	DBGTRACE("hwnd=0x%x\n",g_hwMainWindow);

	g_timerID = SetTimer(NULL, 0, 2000, (TIMERPROC)TimerProc);
	DBGTRACE("g_timerID=%d\n",g_timerID);

	return 0;  /* 0 = OK,  1 = Error,  -1 = OK but unload this plugin */
}

/* OnDestroy is called on ClocX exit */
EXPORT void OnDestroy ()
{
	if (g_timerID) KillTimer(NULL, g_timerID);
}

/* OnConfig is called when user clicks Configure in Plugin Options */
EXPORT void OnConfig (HWND ParentDlg)
{
	MessageBox(ParentDlg, _T("Fixes window on top behaviour"), _T("ClocX Window Helper Plugin"), MB_OK | MB_ICONINFORMATION);
}
