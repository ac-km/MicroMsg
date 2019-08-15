#pragma once
#include<Windows.h>
class CLOG
{
public:
	CLOG(void);
	~CLOG(void);
	char *GetPath(void){return this->PATH;};
	char *GetTemp(void){return this->TEMP;};
	bool LogControl(char *);
private:
	char PATH[MAX_PATH];
	char TEMP[MAX_PATH];
};

