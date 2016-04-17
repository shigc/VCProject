// First.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "CmdUtil.h"
#include "NetUtil.h"
#include "EnumDevices.h"
#include <algorithm>

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	CArray<DeviceInfo, DeviceInfo&> adi;
	EnumDevices::EnumAllDevices(adi);

	int size = adi.GetSize();
	sort(adi.GetData(), adi.GetData() + size);

	int i;

	CString lastName;
	for (i = 0; i < size; i++)
	{
		if(adi.GetAt(i).strClassName != lastName)
		{
			lastName = adi.GetAt(i).strClassName;
			printf("%s:\n",adi.GetAt(i).strClassDesc.GetBuffer(0));
		}
		//printf("CLASSNAME:%s\n" , adi.GetAt(i).strClassDesc.GetBuffer(0));
		printf("\t%s\n", adi.GetAt(i).strName.GetBuffer(0));
	}
	
	return 0;
}

