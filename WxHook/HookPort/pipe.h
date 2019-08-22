#pragma once
#include <windows.h>  
#include <conio.h>  
#include<string>
 

void pipe_start_thread(DWORD custom_type,DWORD code,std::string &msg);

DWORD WINAPI pipe_client(LPVOID lpParam);

