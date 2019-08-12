#include "stdafx.h"
#include "HookPort.h"
#include<Psapi.h>
#include<TlHelp32.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include"Ntdll\Dispatch_NTDLL.h"
#include"ADVAPI32\Dispatch_ADVAPI32.h"
#include"KERNEL32\Dispatch_KERNEL32.h"
#include"USER32\Dispatch_USER32.h"
#include"Inject\RemoteInjection.h"
#include"CloneAPI\CloneAPI_KERNEL32.h"
#include"SELF\Dispatch_SELF.h"

//
//Global
//
HINSTANCE g_hinstDLL = NULL;

//PatchedProcessTable
DWORD g_dwPatchedProcessId[CONF_HookPort_MaxProcessCount] = {0};

//Current module
DWORD g_dwCurrentModule_ImageBase = 0;
DWORD g_dwCurrentModule_ImageHigh = 0;

//kernel32.dll
DWORD g_dwCloneAPIModule_KERNEL32_ImageBase = 0;
DWORD g_dwCloneAPIModule_KERNEL32_ImageHigh = 0;

//fltlib.dll
DWORD g_dwCloneAPIModule_FLTLIB_ImageBase = 0;
DWORD g_dwCloneAPIModule_FLTLIB_ImageHigh = 0;

//shell32.dll
DWORD g_dwCloneAPIModule_SHELL32_ImageBase = 0;
DWORD g_dwCloneAPIModule_SHELL32_ImageHigh = 0;

//advapi32.dll
DWORD g_dwCloneAPIModule_ADVAPI32_ImageBase = 0;
DWORD g_dwCloneAPIModule_ADVAPI32_ImageHigh = 0;

//kernel32
HINSTANCE g_hCloneKERNEL32 = NULL;
WCHAR g_szCloneKERNEL32[MAX_PATH] = { NULL };

//fltlib
HINSTANCE g_hCloneFLTLIB = NULL;
WCHAR g_szCloneFLTLIB[MAX_PATH] = { NULL };

//shell32
HINSTANCE g_hCloneSHELL32 = NULL;
WCHAR g_szCloneSHELL32[MAX_PATH] = { NULL };

//advapi32
HINSTANCE g_hCloneADVAPI32 = NULL;
WCHAR g_szCloneADVAPI32[MAX_PATH] = { NULL };


int GetCurrentModuleInfo(IN HMODULE hModule)
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = -1;

	//
	//Calculate Base Address & Size of Image
	//
	if (hModule)
	{
		MODULEINFO ModInfo = { 0 };

		if (GetModuleInformation(GetCurrentProcess(), hModule, &ModInfo, sizeof(ModInfo)) == TRUE)
		{
			//Base Address
			g_dwCurrentModule_ImageBase = (DWORD)ModInfo.lpBaseOfDll;

			//Size of image
			g_dwCurrentModule_ImageHigh = (DWORD)ModInfo.lpBaseOfDll + ModInfo.SizeOfImage;

			if (g_dwCurrentModule_ImageBase > 0 &&
				g_dwCurrentModule_ImageHigh > 0
				)
			{
				iRet = 0;

#ifdef Dbg
				WCHAR szDebugString[256] = { 0 };
				wsprintf(szDebugString, L"CurrentModuleImageBase = [0x%08X] CurrentModuleImageHigh = [0x%08X]", g_dwCurrentModule_ImageBase, g_dwCurrentModule_ImageHigh);
				DebugLog(DbgInfo, szDebugString);
#endif
			}
		}
	}

	return iRet;
}

int GetCloenAPIModuleInfo(void)
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = 0;

	//
	//Calculate Base Address & Size of Image
	//

	//kernel32.dll
	g_hCloneKERNEL32 = LoadLibrary(g_szCloneKERNEL32);
	if (g_hCloneKERNEL32)
	{
		MODULEINFO ModInfo = { 0 };

		if (GetModuleInformation(GetCurrentProcess(), g_hCloneKERNEL32, &ModInfo, sizeof(ModInfo)) == TRUE)
		{
			//Base Address
			g_dwCloneAPIModule_KERNEL32_ImageBase = (DWORD)ModInfo.lpBaseOfDll;

			//Size of image
			g_dwCloneAPIModule_KERNEL32_ImageHigh = (DWORD)ModInfo.lpBaseOfDll + ModInfo.SizeOfImage;

			if (g_dwCloneAPIModule_KERNEL32_ImageBase > 0 &&
				g_dwCloneAPIModule_KERNEL32_ImageHigh > 0
				)
			{
				iRet = 0;

#ifdef Dbg
				WCHAR szDebugString[256] = { 0 };
				wsprintf(
					szDebugString,
					L"CloneAPIModuleImageBase = [0x%08X] CloneAPIModuleImageHigh = [0x%08X]",
					g_dwCloneAPIModule_KERNEL32_ImageBase,
					g_dwCloneAPIModule_KERNEL32_ImageHigh
					);
				DebugLog(DbgInfo, szDebugString);
#endif
			}
		}
	}

	//fltlib.dll
	g_hCloneFLTLIB = LoadLibrary(g_szCloneFLTLIB);
	if (g_hCloneFLTLIB)
	{
		MODULEINFO ModInfo = { 0 };

		if (GetModuleInformation(GetCurrentProcess(), g_hCloneFLTLIB, &ModInfo, sizeof(ModInfo)) == TRUE)
		{
			//Base Address
			g_dwCloneAPIModule_FLTLIB_ImageBase = (DWORD)ModInfo.lpBaseOfDll;

			//Size of image
			g_dwCloneAPIModule_FLTLIB_ImageHigh = (DWORD)ModInfo.lpBaseOfDll + ModInfo.SizeOfImage;

			if (g_dwCloneAPIModule_FLTLIB_ImageBase > 0 &&
				g_dwCloneAPIModule_FLTLIB_ImageHigh > 0
				)
			{
				iRet = 0;

#ifdef Dbg
				WCHAR szDebugString[256] = { 0 };
				wsprintf(
					szDebugString,
					L"CloneAPIModuleImageBase = [0x%08X] CloneAPIModuleImageHigh = [0x%08X]",
					g_dwCloneAPIModule_FLTLIB_ImageBase,
					g_dwCloneAPIModule_FLTLIB_ImageHigh
					);
				DebugLog(DbgInfo, szDebugString);
#endif
			}
		}
	}

	//shell32.dll
	g_hCloneSHELL32 = LoadLibrary(g_szCloneSHELL32);
	if (g_hCloneSHELL32)
	{
		MODULEINFO ModInfo = { 0 };

		if (GetModuleInformation(GetCurrentProcess(), g_hCloneSHELL32, &ModInfo, sizeof(ModInfo)) == TRUE)
		{
			//Base Address
			g_dwCloneAPIModule_SHELL32_ImageBase = (DWORD)ModInfo.lpBaseOfDll;

			//Size of image
			g_dwCloneAPIModule_SHELL32_ImageHigh = (DWORD)ModInfo.lpBaseOfDll + ModInfo.SizeOfImage;

			if (g_dwCloneAPIModule_SHELL32_ImageBase > 0 &&
				g_dwCloneAPIModule_SHELL32_ImageHigh > 0
				)
			{
				iRet = 0;

#ifdef Dbg
				WCHAR szDebugString[256] = { 0 };
				wsprintf(
					szDebugString,
					L"CloneAPIModuleImageBase = [0x%08X] CloneAPIModuleImageHigh = [0x%08X]",
					g_dwCloneAPIModule_SHELL32_ImageBase,
					g_dwCloneAPIModule_SHELL32_ImageHigh
					);
				DebugLog(DbgInfo, szDebugString);
#endif
			}
		}
	}

	//advapi32.dll
	g_hCloneADVAPI32 = LoadLibrary(g_szCloneADVAPI32);
	if (g_hCloneADVAPI32)
	{
		MODULEINFO ModInfo = { 0 };

		if (GetModuleInformation(GetCurrentProcess(), g_hCloneADVAPI32, &ModInfo, sizeof(ModInfo)) == TRUE)
		{
			//Base Address
			g_dwCloneAPIModule_ADVAPI32_ImageBase = (DWORD)ModInfo.lpBaseOfDll;

			//Size of image
			g_dwCloneAPIModule_ADVAPI32_ImageHigh = (DWORD)ModInfo.lpBaseOfDll + ModInfo.SizeOfImage;

			if (g_dwCloneAPIModule_ADVAPI32_ImageBase > 0 &&
				g_dwCloneAPIModule_ADVAPI32_ImageHigh > 0
				)
			{
				iRet = 0;

#ifdef Dbg
				WCHAR szDebugString[256] = { 0 };
				wsprintf(
					szDebugString,
					L"CloneAPIModuleImageBase = [0x%08X] CloneAPIModuleImageHigh = [0x%08X]",
					g_dwCloneAPIModule_ADVAPI32_ImageBase,
					g_dwCloneAPIModule_ADVAPI32_ImageHigh
					);
				DebugLog(DbgInfo, szDebugString);
#endif
			}
		}
	}

	return iRet;
}

int GetCloneAPIPath(void)
{
	//Return Value:
	//-1 = error
	//0 = succeed
	int iRet = 0;

	//
	//Get [CloneAPI] directory
	//
	WCHAR szCloneAPIDirectory[MAX_PATH] = { NULL };
	GetModuleFileName(g_hinstDLL, szCloneAPIDirectory, MAX_PATH);
	PathRemoveFileSpec(szCloneAPIDirectory);
	lstrcat(szCloneAPIDirectory, L"\\");
	lstrcat(szCloneAPIDirectory, CONF_CloneAPI_DirectoryName);

	//kernel32.dll
	wsprintf(g_szCloneKERNEL32, L"%s\\%s", szCloneAPIDirectory, CONF_CloneAPI_CloneKERNEL32);

#ifdef Dbg
	WCHAR szDebugString[512] = { 0 };
	wsprintf(szDebugString, L"g_szCloneKERNEL32=[%s]", g_szCloneKERNEL32);
	DebugLog(DbgInfo, szDebugString);
#endif

	//fltlib.dll
	wsprintf(g_szCloneFLTLIB, L"%s\\%s", szCloneAPIDirectory, CONF_CloneAPI_CloneFLTLIB);

#ifdef Dbg
	wsprintf(szDebugString, L"g_szCloneFLTLIB=[%s]", g_szCloneFLTLIB);
	DebugLog(DbgInfo, szDebugString);
#endif

	//shell32.dll
	wsprintf(g_szCloneSHELL32, L"%s\\%s", szCloneAPIDirectory, CONF_CloneAPI_CloneSHELL32);

#ifdef Dbg
	wsprintf(szDebugString, L"g_szCloneSHELL32=[%s]", g_szCloneSHELL32);
	DebugLog(DbgInfo, szDebugString);
#endif

	//advapi32.dll
	wsprintf(g_szCloneADVAPI32, L"%s\\%s", szCloneAPIDirectory, CONF_CloneAPI_CloneADVAPI32);

#ifdef Dbg
	wsprintf(szDebugString, L"g_szCloneADVAPI32=[%s]", g_szCloneADVAPI32);
	DebugLog(DbgInfo, szDebugString);
#endif

	return iRet;
}


int HookPortInitalization(IN HMODULE hModule)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	int iRet = 1;

	//
	//Check if is patched
	//
	HINSTANCE hDll = NULL;
	hDll = GetModuleHandleW(CONF_CloneAPI_CloneKERNEL32);
	if (hDll)
	{
		return 0;
	}

	////
	////Save [lpSandBoxName] to [g_szSandBoxName]
	////
	//lstrcpy(g_szSandBoxName, lpSandBoxName);
	//WideCharToMultiByte(CP_ACP, WC_COMPOSITECHECK, g_szSandBoxName, -1, g_strSandBoxName, sizeof(g_strSandBoxName), NULL, NULL);

	//
	//Get Current Module info
	//
	if (GetCurrentModuleInfo(hModule) == -1)
	{
		iRet = -1;
	}

	////
	////Log Process Information to RegistrySYNC
	////
	////[HKEY_USERS\SandBox_XXX\SYNC\PROC\(PID)]
	//LogProcessInfoToRegistrySYNC(lpSandBoxName, GetCurrentProcessId());

	//
	//Get CloneAPI file path
	//
	GetCloneAPIPath();

	//
	//Get CloenAPI Module info
	//
	GetCloenAPIModuleInfo();

	//
	//Init [Patched Process Table]
	//
	InitPatchedProcessTable();

	////
	////Get RestoreFilePath
	////
	//GetRestoreFilePath(lpSandBoxName);

	////
	////Get RestoreRegPath
	////
	//GetRestoreRegPath(lpSandBoxName);

	//
	//Self check
	//
	if (SelfCheck() == -1)
	{
		iRet = -1;
	}

	return iRet;
}


//
//BeginDispatch Functions
//
int BeginDispatchBySandBoxName()
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	int iRet = 0;

	//
	//Patch DLL
	//
	Dispatch_NTDLL_Start();

	//Dispatch_KERNEL32_Start();

	//Dispatch_USER32_Start();

	//Dispatch_ADVAPI32_Start();

	//ScanCurrentProcModule();

	return iRet;
}

BOOL IsTargetHookModule(IN LPCSTR target)
{
	CHAR szPath[MAX_PATH] = { 0 };
	if (!GetModuleFileNameA(NULL, szPath, MAX_PATH))
	{
		return FALSE;
	}

	char fname[_MAX_FNAME] = { 0 };
	_splitpath(szPath, 0, 0, fname, 0);
	if (strcmp(fname, target) == 0)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL HookPort_Start_DllMain(void)
{
	//
	//Initalization
	//
	int iInitState = -1;
	iInitState = HookPortInitalization(g_hinstDLL);

	if (iInitState == -1 ||
		iInitState == 0
		)
	{
#ifdef Dbg
		DebugLog(DbgInfo, L"ERROR");
#endif

		return FALSE;
	}

	//
	//Begin Dispatch
	//
	if (BeginDispatchBySandBoxName() == -1)
	{
#ifdef Dbg
		DebugLog(DbgInfo, L"ERROR");
#endif

		return FALSE;
	}

	return TRUE;
}

BOOL InitPatchedProcessTable(void)
{
	for (int i = 0; i<CONF_HookPort_MaxProcessCount; i++)
	{
		g_dwPatchedProcessId[i] = 0;
	}

	return TRUE;
}

int SelfCheck(void)
{
	//Return Value:
	//-1 = error
	//0 = warning
	//1 = succeed

	return 0;
}

BOOL IsBypassCaller(IN DWORD lpdwReturnAddress)
{
	//Return Value:
	//TRUE = bypass
	//FALSE = not bypass

	//
	//Check Return Address
	//

	//Current module
	if (lpdwReturnAddress >= g_dwCurrentModule_ImageBase &&
		lpdwReturnAddress <= g_dwCurrentModule_ImageHigh
		)
	{
		return TRUE;
	}

	//kernel32.dll
	if (lpdwReturnAddress >= g_dwCloneAPIModule_KERNEL32_ImageBase &&
		lpdwReturnAddress <= g_dwCloneAPIModule_KERNEL32_ImageHigh
		)
	{
		return TRUE;
	}

	//fltlib.dll
	if (lpdwReturnAddress >= g_dwCloneAPIModule_FLTLIB_ImageBase &&
		lpdwReturnAddress <= g_dwCloneAPIModule_FLTLIB_ImageHigh
		)
	{
		return TRUE;
	}

	//shell32.dll
	if (lpdwReturnAddress >= g_dwCloneAPIModule_SHELL32_ImageBase &&
		lpdwReturnAddress <= g_dwCloneAPIModule_SHELL32_ImageHigh
		)
	{
		return TRUE;
	}

	//advapi.dll
	if (lpdwReturnAddress >= g_dwCloneAPIModule_ADVAPI32_ImageBase &&
		lpdwReturnAddress <= g_dwCloneAPIModule_ADVAPI32_ImageHigh
		)
	{
		return TRUE;
	}

	return FALSE;
}


//
//Routines
//
DWORD GetProcAddressEx(IN HMODULE hModule, IN LPCSTR pProcName)
{
	return searchexport((DWORD)hModule, (DWORD)pProcName);
}

HINSTANCE GetKERNEL32ModHandle(void)
{
	return (HINSTANCE)getkernel32modhandle();
}

__declspec(naked)
DWORD __stdcall searchexport(IN DWORD modbase, IN DWORD szexportname)
{
	__asm
	{
		push ebp
			mov ebp, esp
			push ebx
			push esi
			push edi
			mov al, 0
			mov ecx, 256
			mov edi, szexportname
			cld
			repnz scasb
			mov edx, 256
			sub edx, ecx//edx:strlen
			mov ebx, modbase//ebx: hold the modbase.
			mov esi, [ebx + 60]//PEheaderRVA
			add esi, ebx
			add esi, 78h//datadirectory,no.1 is exporttable
			mov eax, [esi]//exptable RVA
			add eax, ebx
			mov esi, eax//exptable
			mov ecx, [esi + 24]//ecx:num_of_names
			mov eax, [esi + 32]
			add eax, ebx//eax:namestable

		looper : cmp ecx, 0
				 jz error
				 dec ecx
				 mov esi, [eax + 4 * ecx]
				 add esi, ebx
				 inc ecx
				 push ecx
				 mov ecx, edx
				 mov edi, szexportname
				 cld
				 repz cmpsb
				 pop ecx
				 jz found
				 dec ecx
				 jmp looper
			 found : dec ecx
					 mov esi, [ebx + 60]//PEheaderRVA
					 add esi, ebx
					 add esi, 78h//datadirectory,no.1 is exporttable
					 mov esi, [esi]//exptable RVA
					 add esi, ebx//exptable:esi
					 mov esi, [esi + 36]
					 add esi, ebx//Index table
					 mov ax, [esi + 2 * ecx]//got x
					 mov esi, [ebx + 60]//PEheaderRVA
					 add esi, ebx
					 add esi, 78h//datadirectory,no.1 is exporttable
					 mov esi, [esi]//exptable RVA
					 add esi, ebx//exptable:esi
					 mov esi, [esi + 28]
					 add esi, ebx//address table
					 xor edi, edi
					 movzx edi, ax
					 mov eax, [esi + edi * 4]
					 add eax, ebx
					 jmp end
				 error :
		xor eax, eax
			jmp end
		end :
		pop edi
			pop esi
			pop ebx
			pop ebp
			ret 8
	}
}

void* __stdcall getkernel32modhandle()
{
	//
	//Get Module Handle of [kernel32.dll]
	//
	//(1) FS寄存器 -> TEB结构
	//(2) TEB+0x30 -> PEB结构
	//(3) PEB+0x0c -> PEB_LDR_DATA
	//(4) PEB_LDR_DATA+0x1c -> Ntdll.dll
	//(5) Ntdll.dll+0x08 -> Kernel32.dll
	//
	//For example:
	//
	//__asm
	//{
	//	push esi
	//	mov eax, fs:0x30		//打开FS寄存器
	//	mov eax, [eax + 0x0c]	//得到PEB结构地址
	//	mov esi, [eax + 0x1c]	//得到PEB_LDR_DATA结构地址
	//	lodsd					//InInitializationOrderModuleList
	//	mov eax, [eax + 0x08]	//得到BaseAddress，即kernel32.dll基址
	//	pop esi
	//}

	__asm
	{
		push esi
			mov eax, fs:0x30		//打开FS寄存器
			mov eax, [eax + 0x0c]	//得到PEB结构地址
			mov esi, [eax + 0x1c]	//得到PEB_LDR_DATA结构地址
			lodsd					//InInitializationOrderModuleList
			mov eax, [eax + 0x08]	//得到BaseAddress，即kernel32.dll基址
			pop esi
	}
}

BOOL IsProcessPatched(IN DWORD dwProcessId, IN BOOL bQueryInCached)
{
	//Return Value:
	//TRUE = Patched
	//FALSE = not Patched
	BOOL bRet = FALSE;

	//
	//Query in cached of [Patched Process Table]
	//
	if (bQueryInCached == TRUE)
	{
		for (int i = 0; i<CONF_HookPort_MaxProcessCount; i++)
		{
			if (g_dwPatchedProcessId[i] == 0)
			{
				break;
			}
			if (g_dwPatchedProcessId[i] == dwProcessId)
			{
				bRet = TRUE;
				break;
			}
		}

		return bRet;
	}
	return bRet;
}

int ScanCurrentProcModule(void)
{
	HANDLE hSnapshot = NULL;
	MODULEENTRY32 moudle;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	moudle.dwSize = sizeof(MODULEENTRY32);
	Module32First(hSnapshot, &moudle);
	do
	{
		//
		//Ignore current module
		//
		if (moudle.hModule == g_hinstDLL)
		{
			continue;
		}
		//
		//Check if is target module
		//
		CheckTargetModule(moudle.szModule);
	} while (Module32Next(hSnapshot, &moudle));
	CloseHandle(hSnapshot);

	return 0;
}

int CheckTargetModule(IN const WCHAR * lpModuleName)
{
	//
	//List of Module will be patch
	//
	//ntdll.dll kernel32.dll user32.dll gdi32.dll advapi32.dll shell32.dll
	//ole32.dll ws2_32.dll crypt32.dll msi.dll sxs.dll secur32.dll hnetcfg.dll
	//setupapi.dll wtsapi32.dll pstorec.dll

	//	if( wcsicmp(lpModuleName,L"kernel32.dll") == 0 )
	//	{
	//#ifdef Dbg
	//		DebugLog(DbgInfo,L"kernel32.dll Patched!");
	//#endif
	//	}
	//
	//	if( wcsicmp(lpModuleName,L"user32.dll") == 0 )
	//	{
	//#ifdef Dbg
	//		DebugLog(DbgInfo,L"user32.dll Patched!");
	//#endif
	//	}
	//
	//	if( wcsicmp(lpModuleName,L"gdi32.dll") == 0 )
	//	{
	//#ifdef Dbg
	//		DebugLog(DbgInfo,L"gdi32.dll Patched!");
	//#endif
	//	}
	//
	//	if( wcsicmp(lpModuleName,L"advapi32.dll") == 0 )
	//	{
	//#ifdef Dbg
	//		DebugLog(DbgInfo,L"advapi32.dll Patched!");
	//#endif
	//	}
	//
	//	if( wcsicmp(lpModuleName,L"shell32.dll") == 0 )
	//	{
	//#ifdef Dbg
	//		DebugLog(DbgInfo,L"shell32.dll Patched!");
	//#endif
	//	}

	return 0;
}



//
//Dll Export [GetHookPortControlInit]
//
//extern "C" __declspec(dllexport) BOOL GetHookPortControlInit(CHookPortControlInterface** lpParameter)   
//{
//	*lpParameter = (CHookPortControlInterface*)&g_hDllModule;
//
//	return TRUE;
//}
//
//BOOL CHookPortControlInit::InitHookPort(LPCWSTR lpSandBoxName)
//{
//	//
//	//Initalization
//	//
//	int iInitState = -1;
//	iInitState = HookPortInitalization(g_hinstDLL,lpSandBoxName);
//
//	if( iInitState == -1 ||
//		iInitState == 0
//		)
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"ERROR");
//#endif
//
//		return FALSE;
//	}
//
//	//
//	//Begin Dispatch
//	//
//	if( BeginDispatchBySandBoxName(lpSandBoxName) == -1 )
//	{
//#ifdef Dbg
//		DebugLog(DbgInfo,L"ERROR");
//#endif
//
//		return FALSE;
//	}
//
//	return TRUE;
//}

//
//Dll Export [HookPort_Start]
//
extern "C" __declspec(dllexport) BOOL HookPort_Start(LPVOID lpParameter)
{
	//
	//Initalization
	//
	int iInitState = -1;
	iInitState = HookPortInitalization(g_hinstDLL);

	if (iInitState == -1 ||
		iInitState == 0
		)
	{
#ifdef Dbg
		DebugLog(DbgInfo, L"ERROR");
#endif

		return FALSE;
	}

	//
	//Begin Dispatch
	//
	if (BeginDispatchBySandBoxName() == -1)
	{
#ifdef Dbg
		DebugLog(DbgInfo, L"ERROR");
#endif

		return FALSE;
	}

	//
	//Resume Current Process
	//
	ResumeCurrentProcess();

	//
	//Convert thread to a GUI thread
	//
	ConvertToGUIThread();

	return TRUE;
}

int ResumeCurrentProcess(void)
{
	HANDLE hSnapshot = NULL;
	THREADENTRY32 ThreadSnapshot;
	HANDLE hThread = NULL;

	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, GetCurrentProcessId());
	ThreadSnapshot.dwSize = sizeof(THREADENTRY32);

	Thread32First(hSnapshot, &ThreadSnapshot);
	do
	{
		if (ThreadSnapshot.th32ThreadID != GetCurrentThreadId() &&
			ThreadSnapshot.th32OwnerProcessID == GetCurrentProcessId()
			)
		{
			hThread = CAPI_OpenThread(THREAD_SUSPEND_RESUME, 0, ThreadSnapshot.th32ThreadID);

			PULONG SuspendCount = 0;
			//NtResumeThread(hThread, SuspendCount);
			ResumeThread(hThread);

			CloseHandle(hThread);
		}
	} while (Thread32Next(hSnapshot, &ThreadSnapshot));
	CloseHandle(hSnapshot);

	return 0;
}

int ConvertToGUIThread(void)
{
	WNDCLASSEX wc;
	MSG msg;
	const WCHAR	ClassName[] = L"[SandBox]";
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hInstance = g_hinstDLL;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = &ClassName[0];
	RegisterClassEx(&wc);

	msg.hwnd = CreateWindowEx(
		WS_EX_TOOLWINDOW,
		&ClassName[0],
		L"HookPort GUI Thread",
		0,
		0,
		0,
		1,
		1,
		HWND_DESKTOP,
		NULL,
		g_hinstDLL,
		0
		);

	ShowWindow(msg.hwnd, SW_HIDE);
	UpdateWindow(msg.hwnd);

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
	{
					// Resize the browser object to fit the window
					//ResizeBrowser(hwnd, LOWORD(lParam), HIWORD(lParam));
					return(0);
	}

	case WM_CREATE:
	{
					  // Embed the browser object into our host window. We need do this only
					  // once. Note that the browser object will start calling some of our
					  // IOleInPlaceFrame and IOleClientSite functions as soon as we start
					  // calling browser object functions in EmbedBrowserObject().
					  //if (EmbedBrowserObject(hwnd)) return(-1);

					  // Another window created with an embedded browser object
					  //++WindowCount;

					  // Success
					  return(0);
	}

	case WM_DESTROY:
	{
					   // Detach the browser object from this window, and free resources.
					   //UnEmbedBrowserObject(hwnd);

					   // One less window
					   //--WindowCount;

					   // If all the windows are now closed, quit this app
					   //if(WindowCount==0)PostQuitMessage(0);

					   return(TRUE);
	}


	}

	return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}