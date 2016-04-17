#pragma once

#include "stdafx.h"
#include <SetupAPI.h>
#include <afxtempl.h>

using namespace std;

#pragma comment(lib, "setupapi.lib")

class DeviceInfo {
public:
	GUID guid;
	CString strName;
	CString strClassName;
	CString strClassDesc;

	//����"<"�����������ֱ����sort������������
	bool operator<(DeviceInfo &deviceInfo)
	{
		if(strClassName.Compare(deviceInfo.strClassName) < 0)
		{
			return true;
		}
		return false;
	}
};

class AFX_EXT_CLASS EnumDevices
{
public:
	static bool EnumAllDevices(CArray<DeviceInfo,DeviceInfo&> &adi);
};

