#include "CmdUtil.h"

bool CmdUtil::Execute(CString strCmd, CString &strOut)
{
	HANDLE hRead,hWrite;
	SECURITY_ATTRIBUTES sa;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	//Zero the structures
	strOut.Empty();
	ZeroMemory(&sa, sizeof(sa));
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	
	sa.nLength = sizeof( SECURITY_ATTRIBUTES );
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	//create unamed pipe
	if (!CreatePipe(&hRead,&hWrite,&sa,0))
	{
		strOut = "Error on CreatePipe!";
		return false;
	}

	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	
	if (!CreateProcess(NULL,strCmd.GetBuffer(), NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi))
	{
		strOut = "Error on CreateProcess!";
		return false;
	}
	strCmd.ReleaseBuffer();
	CloseHandle(hWrite);

	const int BUFF_SIZE = 2048;
	TCHAR buffer[BUFF_SIZE];
	DWORD byteRead = 0;
	DWORD byteAvail = 0;
	while(true)
	{
		// Check the pipe info
		if(!PeekNamedPipe(hRead, buffer, BUFF_SIZE, &byteAvail, 0, 0))
		{
			break;
		}
		memset(buffer, 0, BUFF_SIZE);

		if(byteAvail)
		{
			if (ReadFile(hRead, buffer, BUFF_SIZE, &byteRead, NULL) == NULL || byteRead == 0)
			{
				break;
			}
			strOut += buffer;
		}
		else
		{
			//judge whether the process has exit
			if ( WaitForSingleObject(pi.hProcess, 0) == WAIT_OBJECT_0 ) 
			{  
				break;  
			}
		}
	}
	return true;
}