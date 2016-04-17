#pragma once

#include "stdafx.h"
#include <WinSock2.h>
#include <iphlpapi.h>
#include <cstdio>
#include <string>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

class NetUtil
{
public:
	NetUtil(void);
	~NetUtil(void);

	void GetNameAndIp();

	bool GetNameByIp(char ip[32], unsigned char* name);
	bool GetMacByIp(char ip[32], unsigned char *mac);
};

