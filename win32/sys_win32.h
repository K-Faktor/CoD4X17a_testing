#include "../q_shared.h"
#include "../objfile_parser.h"
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

void CON_Show( int visLevel, qboolean quitOnClose );
char** PE32_GetStrTable(void *buff, int len, sharedlib_data_t *text);

extern WinVars_t g_wv;
extern byte cod4_plt[8192];
#define IDI_ICON1                       1