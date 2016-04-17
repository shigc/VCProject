#include "NetUtil.h"

using namespace std;

NetUtil::NetUtil(void)
{
}


NetUtil::~NetUtil(void)
{
}

void NetUtil::GetNameAndIp()  
{  

	struct   hostent   *host;  
	struct   in_addr   *ptr;  

	DWORD   dwScope = RESOURCE_CONTEXT;  
	NETRESOURCE   *NetResource = NULL;  
	HANDLE   hEnum;  
	WNetOpenEnum(dwScope, NULL, NULL, NULL, &hEnum);  

	WSADATA   wsaData;  
	WSAStartup(MAKEWORD(1, 1), &wsaData);  

	if (hEnum)  
	{  
		DWORD   Count = 0xFFFFFFFF;  
		DWORD   BufferSize = 10240;  
		LPVOID   Buffer = new char[10240];  
		WNetEnumResource(hEnum, &Count, Buffer, &BufferSize);  
		NetResource = (NETRESOURCE*)Buffer;  

		char szHostName[200];  

		for (unsigned int i = 0; i < BufferSize / sizeof(NETRESOURCE); i++, NetResource++)  
		{  
			if (NetResource->dwUsage == RESOURCEUSAGE_CONTAINER && NetResource->dwType == RESOURCETYPE_ANY)  
			{  
				if (NetResource->lpRemoteName)  
				{  
					CString   strFullName = NetResource->lpRemoteName;  
					if (0 == strFullName.Left(2).Compare(_T("\\\\")))  
						strFullName = strFullName.Right(strFullName.GetLength() - 2);  

					gethostname(szHostName, strlen(szHostName));  
					USES_CONVERSION;  
					char *pchar = T2A(strFullName.GetBuffer(0));  
					host = gethostbyname(pchar);  

					if (host == NULL)   continue;  
					ptr = (struct in_addr *)   host->h_addr_list[0];  

					string str = "";  
					for (int n = 0; n<4; n++)  
					{  
						CString addr;  
						if (n > 0)  
						{  
							str += ".";  
						}  
						int value = (unsigned int)((unsigned char*)host->h_addr_list[0])[n];  
						char p[20];  
						sprintf_s(p, "%d", value);  
						str.append(p);  
					}  

					std::cout <<"IP:"<< str <<" Name:"<<host->h_name<< std::endl;  
				}  
			}  
		}  
		delete   Buffer;  
		WNetCloseEnum(hEnum);  
	}  
	WSACleanup();  
} 
bool NetUtil::GetNameByIp(char ip[32], unsigned char* name)
{
	struct in_addr destIp;
	struct hostent* info;

	destIp.s_addr = inet_addr(ip);

	info = gethostbyaddr((char *)&destIp, 4, AF_INET);

	if(info != NULL)
	{
		strcpy((char *)name, info->h_name);
	}
	else
	{
		return false;
	}
	return true;
}
bool NetUtil::GetMacByIp(char ip[32], unsigned char *mac)
{
	struct in_addr destip;
	ULONG mac_address[2];
	ULONG mac_address_len = 6;

	destip.s_addr = inet_addr(ip);

	SendARP((IPAddr)destip.S_un.S_addr, 0, mac_address, &mac_address_len);

	if (mac_address_len)
	{
		BYTE* mac_address_buffer = (BYTE*)&mac_address;
		for (int i = 0; i < (int)mac_address_len; i++)
		{
			mac[i] = (char)mac_address_buffer[i];
		}
	}
	else
	{
		return false;
	}

	return true;
}