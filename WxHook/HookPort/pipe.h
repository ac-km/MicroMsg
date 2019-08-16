#pragma once
#include <windows.h>  
#include <conio.h>  
#include<string>
 

void pipe_start_thread(std::string &msg);

DWORD WINAPI pipe_client(LPVOID lpParam);

