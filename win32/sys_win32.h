#include "../q_shared.h"
#include <windows.h>

typedef struct
{

HINSTANCE reflib_library; // Handle to refresh DLL
qboolean reflib_active;

HWND hWnd;
HINSTANCE hInstance;
qboolean activeApp;
qboolean isMinimized;
OSVERSIONINFO osversion;

// when we get a windows message, we store the time off so keyboard processing
// can know the exact time of an event
unsigned sysMsgTime;
} WinVars_t;

extern WinVars_t g_wv;

#define IDI_ICON1                       1