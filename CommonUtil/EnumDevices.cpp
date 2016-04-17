#include "EnumDevices.h"


bool EnumDevices::EnumAllDevices(CArray<DeviceInfo,DeviceInfo&> &adi)
{
	HDEVINFO hDevInfo;
	SP_DEVINFO_DATA spDevInfoData;
	DWORD i;

	printf("Displaying the Installed Devices!\n");

	//获取所有设备信息（设备管理器中有的）
	hDevInfo = SetupDiGetClassDevs(NULL, NULL, NULL, DIGCF_ALLCLASSES | DIGCF_PRESENT);

	if(hDevInfo == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
	adi.RemoveAll();

	for (i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &spDevInfoData); i++)
	{
		TCHAR szClass[MAX_PATH] = {0};
		TCHAR szFname[MAX_PATH] = {0};
		TCHAR szDesc[MAX_PATH] = {0};
		TCHAR szLocInfo[MAX_PATH] = {0};
		TCHAR szClassDesc[MAX_PATH] = {0};
		DWORD dwRequireSize = 0;

		
		//查询设备类别名称
		SetupDiGetDeviceRegistryProperty(
			hDevInfo,&spDevInfoData, SPDRP_CLASS, NULL, (PBYTE)szClass, MAX_PATH, 0);
		
		//查询设备类别描述信息
		SetupDiGetClassDescription(&spDevInfoData.ClassGuid, szClassDesc, MAX_PATH, &dwRequireSize);

		//查询设备描述信息
		SetupDiGetDeviceRegistryProperty(
			hDevInfo, &spDevInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)szDesc, MAX_PATH - 1, 0);
		
		//查询设备友好的名称(这个为空时使用设备描述信息)
		SetupDiGetDeviceRegistryProperty(
			hDevInfo, &spDevInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)szFname, MAX_PATH - 1, 0);

		//设备管理器里面的位置信息
		SetupDiGetDeviceRegistryProperty(
			hDevInfo, &spDevInfoData, SPDRP_LOCATION_INFORMATION, NULL, (PBYTE)szLocInfo, MAX_PATH - 1, 0);
		
		DeviceInfo di;
		di.guid = spDevInfoData.ClassGuid;
		di.strClassName = szClass;
		di.strClassDesc = szClassDesc;

		//如果FriendlyName不为空则使用FriendlyName作为设备的名称
		if(szFname[0] != '\0')
		{
			di.strName = szFname;
		}
		else
		{
			di.strName = szDesc;
		}
		adi.Add(di);

		/*
		printf("GUID:{%.8X-%.4X-%.4X--%.2X%.2X-%.2X%.2X%.2X%.2X%.2X%.2X}\n",
			spDevInfoData.ClassGuid.Data1, 
			spDevInfoData.ClassGuid.Data2, 
			spDevInfoData.ClassGuid.Data3,
			spDevInfoData.ClassGuid.Data4[0], 
			spDevInfoData.ClassGuid.Data4[1], 
			spDevInfoData.ClassGuid.Data4[2],
			spDevInfoData.ClassGuid.Data4[3],
			spDevInfoData.ClassGuid.Data4[4],
			spDevInfoData.ClassGuid.Data4[5],
			spDevInfoData.ClassGuid.Data4[6],
			spDevInfoData.ClassGuid.Data4[7]);*/

		/*
		printf("Class:%s\n", szClass);
		printf("ClassDesc:%s\n", szClassDesc);
		printf("FriendName:%s\n", szFname);
		printf("Desc:%s\n", szDesc);
		printf("LocalInfo:%s\n", szLocInfo);
		printf("DI NAME:%s\n", di.strName.GetBuffer(0));
		*/
	}

	if(hDevInfo != INVALID_HANDLE_VALUE)
	{
		//释放资源
		SetupDiDestroyDeviceInfoList(hDevInfo);
	}

	return true;
}