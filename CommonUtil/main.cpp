// First.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CmdUtil.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	CString strCmd = _T("C:\\Users\\Public\\Documents\\test.bat  \"FFFFFF\" \"COM1\"");

	CString output;
	CmdUtil::Execute(strCmd, output);

	TCHAR *chs= output.GetBuffer();

	cout << chs << endl;
	
	return 0;
}

