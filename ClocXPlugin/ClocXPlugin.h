#ifndef CLOCXPLUGIN_H
#define CLOCXPLUGIN_H

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT _WIN32_WINNT_VISTA //_WIN32_WINNT_WIN2K	// Change this to the appropriate value to target Windows 2000 or later.
#endif	

#define WIN32_LEAN_AND_MEAN		/* Exclude rarely-used stuff from Windows headers */

#include <windows.h>
#include "resource.h"

/* define macro for exporting functions and variables */
#ifdef __cplusplus
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/* structure received from OnInit */
struct InitData {
	HWND MainWindow;  //handle to ClocX main window
	const char *LangFileName;  //name of used language file (can be used for language selection)
};

/* structure received from OnSkinLoaded function */
struct SkinLoadedData {
	int Width;  //width of zoomed bitmap
	int Height;  //height of zoomed bitmap
	int AMPMColor;  //color of AM/PM indicator
	int AMPMFontSize;  //zoomed font size of AM/PM indicator 
	int DateColor;  //color of date indicator
	int DateFontSize;  //zoomed size of date indicator
	int ZoomFactor;  //percentual factor of zoom (doubled if using antialiasing)
	POINT ZoomedCenter;  //zoomed coordinates of bitmap center
	POINT AMPMCenter;
	POINT DateCenter;
	HBITMAP LoadedBitmap;  //handle to skin bitmap
	const char *SkinPath;  //path of skin bitmap
};

#endif /* CLOCXPLUGIN_H */

