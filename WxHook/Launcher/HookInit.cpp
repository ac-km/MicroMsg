#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "HookInit.h"
#include "../Common/DebugLog.h"


CHookInit::CHookInit()
{
}


CHookInit::~CHookInit()
{
}

BOOL CHookInit::Initalization()
{
	return TRUE;
}

BOOL CHookInit::AdjustPrivileges()
{
	BOOL iRet = TRUE;

	//
	//Adjust privileges
	//
	if (EnableSpecificPrivilege(TRUE, SE_BACKUP_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_CHANGE_NOTIFY_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_CREATE_PAGEFILE_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_DEBUG_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_INC_BASE_PRIORITY_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_INCREASE_QUOTA_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_LOAD_DRIVER_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_PROF_SINGLE_PROCESS_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_RESTORE_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_SECURITY_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_SHUTDOWN_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_SYSTEM_ENVIRONMENT_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_SYSTEMTIME_NAME) == FALSE)
	{
		iRet = FALSE;
	}
	if (EnableSpecificPrivilege(TRUE, SE_TAKE_OWNERSHIP_NAME) == FALSE)
	{
		iRet = FALSE;
	}

	return iRet;
}

BOOL CHookInit::EnableSpecificPrivilege(IN BOOL bEnable, IN CHAR * Name)
{
	BOOL bResult = FALSE;
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES TokenPrivileges;

	if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken) == 0)
	{
		return FALSE;
	}

	TokenPrivileges.PrivilegeCount = 1;
	TokenPrivileges.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
	bResult = LookupPrivilegeValue(NULL, Name, &TokenPrivileges.Privileges[0].Luid);

	if (!bResult)
	{
		CloseHandle(hToken);
		return FALSE;
	}

	bResult = AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, sizeof(TOKEN_PRIVILEGES), NULL, NULL);

	if (GetLastError() != ERROR_SUCCESS || !bResult)
	{
		CloseHandle(hToken);
		return FALSE;
	}

	CloseHandle(hToken);
	return TRUE;
}

BOOL CHookInit::SetAllPrivileges(IN BOOL bEnable)
{
	EnableSpecificPrivilege(bEnable, SE_ASSIGNPRIMARYTOKEN_NAME);
	EnableSpecificPrivilege(bEnable, SE_AUDIT_NAME);
	EnableSpecificPrivilege(bEnable, SE_BACKUP_NAME);
	EnableSpecificPrivilege(bEnable, SE_CHANGE_NOTIFY_NAME);
	EnableSpecificPrivilege(bEnable, SE_CREATE_PAGEFILE_NAME);
	EnableSpecificPrivilege(bEnable, SE_CREATE_PERMANENT_NAME);
	EnableSpecificPrivilege(bEnable, SE_CREATE_TOKEN_NAME);
	EnableSpecificPrivilege(bEnable, SE_DEBUG_NAME);
	EnableSpecificPrivilege(bEnable, SE_INC_BASE_PRIORITY_NAME);
	EnableSpecificPrivilege(bEnable, SE_INCREASE_QUOTA_NAME);
	EnableSpecificPrivilege(bEnable, SE_LOAD_DRIVER_NAME);
	EnableSpecificPrivilege(bEnable, SE_LOCK_MEMORY_NAME);
	EnableSpecificPrivilege(bEnable, SE_PROF_SINGLE_PROCESS_NAME);
	EnableSpecificPrivilege(bEnable, SE_REMOTE_SHUTDOWN_NAME);
	EnableSpecificPrivilege(bEnable, SE_RESTORE_NAME);
	EnableSpecificPrivilege(bEnable, SE_SECURITY_NAME);
	EnableSpecificPrivilege(bEnable, SE_SHUTDOWN_NAME);
	EnableSpecificPrivilege(bEnable, SE_SYSTEM_ENVIRONMENT_NAME);
	EnableSpecificPrivilege(bEnable, SE_SYSTEM_PROFILE_NAME);
	EnableSpecificPrivilege(bEnable, SE_SYSTEMTIME_NAME);
	EnableSpecificPrivilege(bEnable, SE_TAKE_OWNERSHIP_NAME);
	EnableSpecificPrivilege(bEnable, SE_TCB_NAME);
	EnableSpecificPrivilege(bEnable, SE_UNSOLICITED_INPUT_NAME);
	EnableSpecificPrivilege(bEnable, SE_MACHINE_ACCOUNT_NAME);

	return TRUE;
}

BOOL CHookInit::BeginHookPortRoutine()
{
	//
	//Get HookPort FilePath
	//
	CHAR szHookPortPath[MAX_PATH] = { NULL };
	GetModuleFileName(NULL, szHookPortPath, MAX_PATH);
	PathRemoveFileSpec(szHookPortPath);
	lstrcat(szHookPortPath, "\\");
	lstrcat(szHookPortPath, CONF_HookPort_FileName);

	//
	//Load HookPort
	//
	HINSTANCE hDLL = NULL;
	hDLL = LoadLibrary(szHookPortPath);
	if (!hDLL)
	{
#ifdef Dbg
		DebugLog(DbgInfo, "ERROR");
#endif

		return FALSE;
	}

	//	//
	//	//Call Function
	//	//
	//	PGetDllModule pGetDllModule = NULL;
	//	pGetDllModule = (PGetDllModule)GetProcAddress(hDLL, "GetHookPortControlInit");
	//
	//	if( pGetDllModule )
	//	{
	//		pGetDllModule(&g_hDllModule);
	//		g_hDllModule->InitHookPort(lpSandBoxName);
	//	}
	//	else
	//	{
	//#ifdef Dbg
	//		DebugLog(DbgInfo,L"ERROR");
	//#endif
	//
	//		return -1;
	//	}

	return TRUE;
}

BOOL CHookInit::BeginLauncherRunProgram(IN CONST CHAR * lpExeName)
{
	//
	//Create Process
	//
	DWORD dwCreateProcessRet = 0;

	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;
	ZeroMemory(&StartupInfo, sizeof(StartupInfo));
	StartupInfo.cb = sizeof(StartupInfo);
	StartupInfo.lpDesktop = NULL;
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_SHOWNORMAL;

	if (CreateProcess(
		NULL,
		(LPSTR)lpExeName,
		NULL,
		NULL,
		FALSE,
		NULL,
		NULL,
		NULL,
		&StartupInfo,
		&ProcessInfo
		))
	{
		CloseHandle(ProcessInfo.hThread);
		CloseHandle(ProcessInfo.hProcess);

		dwCreateProcessRet = 1;
	}
	else
	{
		HINSTANCE hRet = 0;
		hRet = ShellExecute(NULL, "open", lpExeName, NULL, NULL, SW_SHOWNORMAL);

		if ((int)hRet > 32)
		{
			dwCreateProcessRet = 1;
		}
		else
		{
#ifdef Dbg
			DebugLog(DbgInfo, "ERROR");
#endif
			return FALSE;
		}
	}
	return TRUE;
}