#pragma once
#include"../Common/Config.h"

//
//Global
//
extern HINSTANCE g_hinstDLL;

//PatchedProcessTable
extern DWORD g_dwPatchedProcessId[CONF_HookPort_MaxProcessCount];

//Current module
extern DWORD g_dwCurrentModule_ImageBase;
extern DWORD g_dwCurrentModule_ImageHigh;

//kernel32.dll
extern DWORD g_dwCloneAPIModule_KERNEL32_ImageBase;
extern DWORD g_dwCloneAPIModule_KERNEL32_ImageHigh;

//fltlib.dll
extern DWORD g_dwCloneAPIModule_FLTLIB_ImageBase;
extern DWORD g_dwCloneAPIModule_FLTLIB_ImageHigh;

//shell32.dll
extern DWORD g_dwCloneAPIModule_SHELL32_ImageBase;
extern DWORD g_dwCloneAPIModule_SHELL32_ImageHigh;

//advapi32.dll
extern DWORD g_dwCloneAPIModule_ADVAPI32_ImageBase;
extern DWORD g_dwCloneAPIModule_ADVAPI32_ImageHigh;

//kernel32
extern HINSTANCE g_hCloneKERNEL32;
extern WCHAR g_szCloneKERNEL32[MAX_PATH];

//fltlib
extern HINSTANCE g_hCloneFLTLIB;
extern WCHAR g_szCloneFLTLIB[MAX_PATH];

//shell32
extern HINSTANCE g_hCloneSHELL32;
extern WCHAR g_szCloneSHELL32[MAX_PATH];

//advapi32
extern HINSTANCE g_hCloneADVAPI32;
extern WCHAR g_szCloneADVAPI32[MAX_PATH];

int HookPortInitalization(IN HMODULE hModule);
int BeginDispatchBySandBoxName();
BOOL HookPort_Start_DllMain(void);
int GetCurrentModuleInfo(IN HMODULE hModule);
int GetCloenAPIModuleInfo(void);
int GetCloneAPIPath(void);
BOOL InitPatchedProcessTable(void);
int SelfCheck(void);
BOOL IsBypassCaller(IN DWORD lpdwReturnAddress);
BOOL IsTargetHookModule(IN LPCSTR);

DWORD __stdcall searchexport(IN DWORD modbase, IN DWORD szexportname);
void* __stdcall getkernel32modhandle();
DWORD GetProcAddressEx(IN HMODULE hModule, IN LPCSTR pProcName);
HINSTANCE GetKERNEL32ModHandle(void);

BOOL IsProcessPatched(IN DWORD dwProcessId, IN BOOL bQueryInCached);
int ScanCurrentProcModule(void);
int CheckTargetModule(IN const WCHAR * lpModuleName);

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int ResumeCurrentProcess(void);
int ConvertToGUIThread(void);


//
//Dll Export [HookPort_Start]
//
extern "C" __declspec(dllexport) BOOL HookPort_Start(LPVOID lpParameter);