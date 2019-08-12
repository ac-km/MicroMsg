#pragma once



//
//Define
//



//
//Install
//
#define CONF_SoftwareRegName "RainbowSandbox"
#define CONF_SandBoxNameLength 16



//
//Registry [SandBox]
//
#define CONF_SoftwareReg_SandBox "SandBox"

#define CONF_SoftwareReg_SandBox_View "View"
#define CONF_SoftwareReg_SandBox_Restore "Restore"
#define CONF_SoftwareReg_SandBox_CleanUp "CleanUp"
#define CONF_SoftwareReg_SandBox_Begin "Begin"
#define CONF_SoftwareReg_SandBox_Stop "Stop"
#define CONF_SoftwareReg_SandBox_File "File"
#define CONF_SoftwareReg_SandBox_Limit "Limit"
#define CONF_SoftwareReg_SandBox_Resource "Resource"
#define CONF_SoftwareReg_SandBox_Software "Software"

#define CONF_SoftwareReg_SandBox_Registry "Registry"

#define CONF_SoftwareReg_SandBox_SYNC "SYNC"
#define CONF_SoftwareReg_SandBox_SYNC_PROC "PROC"

#define CONF_SoftwareReg_SandBox_HKEY_CLASSES_ROOT "ROOT"
#define CONF_SoftwareReg_SandBox_HKEY_CURRENT_USER "USER"
#define CONF_SoftwareReg_SandBox_HKEY_LOCAL_MACHINE "MACHINE"
#define CONF_SoftwareReg_SandBox_HKEY_USERS "USER"
#define CONF_SoftwareReg_SandBox_HKEY_CURRENT_CONFIG "CONFIG"

#define CONF_SoftwareReg_SandBox_DeletedFileMark 122913408005200000//1990.07.02-05:20:00

#define CONF_SandBox_RegHive_FileName "RegHive.hiv"
#define CONF_SandBox_RegHive_UpdateName "UpdateRegHive.hiv"

#define CONF_SandBox_Drive_Name "Drive"



//
//Registry [Config]
//
#define CONF_SoftwareReg_Config "Config"
#define CONF_SoftwareReg_Config_RestorePath "RestorePath"



//
//Launcher
//
#define CONF_Launcher_FileName "Launcher.exe"

#define CONF_Launcher_Startup_TimeOut 500

#define CONF_Launcher_StartupPath_ValueName "StartupPath"
#define CONF_Launcher_StartupInfo_ValueName "StartupInfo"



//
//HookPort
//
#define CONF_HookPort_FileName "HookPort.dl"
#define CONF_HookPort_MaxProcessCount 256

#define MAX_NAME MAX_PATH*2



//
//CloneAPI
//
#define CONF_CloneAPI_DirectoryName L"CloneAPI"

#define CONF_CloneAPI_CloneKERNEL32 L"clone_kernel32.dl"
#define CONF_CloneAPI_CloneFLTLIB L"clone_fltlib.dl"
#define CONF_CloneAPI_CloneSHELL32 L"clone_shell32.dl"
#define CONF_CloneAPI_CloneADVAPI32 L"clone_advapi32.dl"