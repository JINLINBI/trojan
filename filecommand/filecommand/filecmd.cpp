#include "stdafx.h"
#include <Shlwapi.h> 
#include <vector>
#pragma comment(lib,"shlwapi.lib")   
char	*g_reply;

typedef struct {
	char type;
	char data[1024];
}packet;

TCHAR	g_curDir[BUFSIZE];
TCHAR	g_cutCopyFile[BUFSIZE];
TCHAR	g_shortFile[BUFSIZE];
bool	is_cut;
vector<wstring> g_backDir;



bool genericCutCopyFileCommand(LPCWSTR filename, bool cut) {
	if (!PathFileExists(filename) || PathIsDirectoryW(filename))
		return false;

	wcscpy_s(g_shortFile, BUFSIZE, filename);
	is_cut = cut;

	return GetFullPathName(filename, BUFSIZE, g_cutCopyFile, NULL);
}

bool rmFileCommand(LPCWSTR filename) {
	return DeleteFile(filename);
}

bool copyFileCommand(LPCWSTR filename) {

	return genericCutCopyFileCommand(filename, false);
}


bool cutFileCommand(LPCWSTR filename) {

	return genericCutCopyFileCommand(filename, true);
}


bool newFileCommand(LPCWSTR filename, char* content) {
	HANDLE fd = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fd) {
		if (content)
			WriteFile(fd, content, strlen(content), NULL, NULL);
		CloseHandle(fd);

		return true;
	}

	return false;
}


bool pasteFileCommand() {
	TCHAR tempFilename[BUFSIZE];
	if (PathFileExists(g_shortFile) && is_cut)
		return false;

	GetFullPathName(g_shortFile, BUFSIZE, tempFilename, NULL);
	if (is_cut)
		MoveFile(g_cutCopyFile, tempFilename);
	else
		CopyFile(g_cutCopyFile, tempFilename, true);

	return true;
}


bool forcePasteFileCommand() {
	TCHAR tempFilename[BUFSIZE];
	GetFullPathName(g_shortFile, BUFSIZE, tempFilename, NULL);

	if (PathFileExists(g_shortFile) && is_cut) {
		DeleteFile(g_shortFile);
		return MoveFile(g_cutCopyFile, g_shortFile);
	}

	if (!is_cut)
		return CopyFile(g_cutCopyFile, g_shortFile, false);

	return false;
}


bool downloadFileCommand(LPCWSTR filename, SOCKET socketfd) {
	packet pac;
	int readedCount, leftlen;
	if (!PathFileExists(filename) || !socketfd)
		return false;

	HANDLE fd = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	leftlen = GetFileSize(fd, NULL);
	if (fd) {
		while (leftlen > 0) {
			ZeroMemory(&pac, sizeof(pac));
			ReadFile(fd, pac.data, 1024, (LPDWORD)&readedCount, NULL);
			leftlen -= readedCount;
			if (leftlen <= 0) {
				pac.type = 2;
				send(socketfd, (const char*)&pac, readedCount + 1, 0);
			}
			else {
				pac.type = 1;
				send(socketfd, (const char*)&pac, readedCount + 1, 0);
			}
		}
		CloseHandle(fd);
		return true;
	}
	return false;
}

bool uploadFileCommand(LPCWSTR filename, SOCKET socketfd) {
	packet pac;
	int readedCount, writtenlen;

	if (!socketfd)
		return false;

	HANDLE fd = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fd) {
		bool err = false;
		bool done = false;
		while (!err && !done) {
			readedCount = recv(socketfd, (char*)&pac, sizeof(pac), 0);
			if (readedCount > 0) {
				switch (pac.type) {
				case 1:
					WriteFile(fd, pac.data, sizeof(pac.data), (LPDWORD)&writtenlen, NULL);
					break;
				case 2:
					WriteFile(fd, pac.data, readedCount - 1, (LPDWORD)&writtenlen, NULL);
					done = true;
				case 0:
					err = true;
					break;
				}
			}
			else
				err = true;
		}
		CloseHandle(fd);
		return true;
	}

	CloseHandle(fd);
	return false;
}

bool saveCurDirCommand() {
	if (GetCurrentDirectory(BUFSIZE, g_curDir)) {
		g_backDir.push_back(wstring(g_curDir));
		return true;
	}

	return false;
}


void popLastDirCommand() {
	wcsncpy(g_curDir, g_backDir.back().c_str(), g_backDir.back().length());
	g_backDir.pop_back();
}


vector<wstring> findFile(wstring dir)
{
	WIN32_FIND_DATAW stFD;												//存放文件信息的结构体  
	HANDLE h;
	wstring temp;
	vector<wstring> vec;
	temp = dir + L"\\*";
	char xorBuf[16] = {
		0x05, 0x12, 0x06, 0x28,
		0x05, 0x17, 0x06, 0x08,
		0x05, 0x19, 0x73, 0x34,
		0x92, 0x23, 0x34, 0x82
	};
	char buf[16];
	int readCount;
	h = FindFirstFileW(temp.c_str(), &stFD);							//构建目录句柄  

	while (FindNextFileW(h, &stFD))										// 提取目录句柄对应目录所包含的文件  
	{
		temp = dir + L"\\" + stFD.cFileName;

		if (temp == dir + L"\\..")                                      //上一级路径  
			continue;

		else if (PathIsDirectoryW(temp.c_str()))						//包含子目录  
		{
			vec.push_back(wstring(L"/") + stFD.cFileName);
		}
		else {
			ZeroMemory(buf, sizeof(buf));
			HANDLE fd = CreateFile(stFD.cFileName, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			ReadFile(fd, buf, sizeof(buf), (LPDWORD)&readCount, 0);
			if (readCount < 0) {
				vec.push_back(stFD.cFileName);
			}
			else if (!memcmp(buf, xorBuf, sizeof(xorBuf))) {
					vec.push_back(wstring(L"?") + stFD.cFileName);
			}
			else {
					vec.push_back(stFD.cFileName);
				
			}
			CloseHandle(fd);
			
		}
			

	}

	return vec;
}


bool lsCommand() {
	vector<wstring> vec = findFile(wstring(L"./"));
	wstring s;
	string tempString;
	int argc = 0;
	char buf[16];
	char* c;
	bool getCurDir = false;
	if (g_reply)
		delete[] g_reply;
	if (GetCurrentDirectory(BUFSIZE, g_curDir)) {
		int t = WideCharToMultiByte(CP_ACP, 0, g_curDir, -1, NULL, 0, NULL, FALSE);
		_itoa_s(t, buf, 16, 10);
		c = new char[t];
		WideCharToMultiByte(CP_ACP, 0, g_curDir, -1, c, t, NULL, FALSE);
		tempString += '$' + string(buf) + "\r\n" + "!" + c + "\r\n";
		getCurDir = true;
		delete[] c;
	}
	for (auto i = vec.begin(); i != vec.end(); i++) {
		s = *i;
		int t = WideCharToMultiByte(CP_ACP, 0, s.c_str(), -1, NULL, 0, NULL, FALSE);
		_itoa_s(t - 1, buf, 16, 10);
		c = new char[t];
		WideCharToMultiByte(CP_ACP, 0, s.c_str(), -1, c, t, NULL, FALSE);
		tempString += '$' + string(buf) + "\r\n" + c + "\r\n";
		delete[] c;
	}
	if (getCurDir)
		_itoa_s(vec.size() + 1, buf, 16, 10);
	else if (vec.empty())
		return false;
	else
		_itoa_s(vec.size(), buf, 16, 10);
	g_reply = new char[strlen(buf) + 3 + tempString.length() + 1];
	memset(g_reply, 0, strlen(buf) + 3 + tempString.length() + 1);
	g_reply[0] = '*';
	strncpy(g_reply + 1, buf, strlen(buf));
	g_reply[strlen(buf) + 1] = '\r';
	g_reply[strlen(buf) + 2] = '\n';

	strncpy(strlen(buf) + 3 + g_reply, tempString.c_str(), tempString.length());
	return true;
}

bool pwdCommand(SOCKET CSocket) {
	string tempString;
	int argc = 0;
	char buf[16];
	char* c;
	if (!g_reply)
		delete[] g_reply;

	if (GetCurrentDirectory(BUFSIZE, g_curDir)) {
		int t = WideCharToMultiByte(CP_ACP, 0,g_curDir, -1, NULL, 0, NULL, FALSE);
		_itoa_s(t, buf, 16, 10);
		c = new char[t];
		WideCharToMultiByte(CP_ACP, 0, g_curDir, -1, c, t, NULL, FALSE);
		tempString += '$' + string(buf) + "\r\n" + c + "\r\n";
		delete[] c;
		g_reply = new char[strlen(buf) + 3 + tempString.length() + 1];
		memset(g_reply, 0, strlen(buf) + 3 + tempString.length() + 1);
		g_reply[0] = '*';
		strncpy(g_reply + 1, buf, strlen(buf));
		g_reply[strlen(buf) + 1] = '\r';
		g_reply[strlen(buf) + 2] = '\n';
		strncpy(strlen(buf) + 3 + g_reply, tempString.c_str(), tempString.length());
	}	


	return false;
}



bool mkdirCommand(LPCWSTR dir) {
	return CreateDirectory(dir, NULL);
}


bool rmDirCommand(LPCWSTR dir) {
	return RemoveDirectory(dir);
}


bool cdCommand(LPCWSTR dir) {
	bool success;

	if (PathIsDirectoryW(dir)) {
		if (!saveCurDirCommand())
			saveCurDirCommand();
		if (dir[1] != L':') {
			GetFullPathName(dir, wcslen(dir), g_curDir, NULL);
		}
		success = SetCurrentDirectory(dir);
		if (success)
			return true;
		popLastDirCommand();
	}

	return false;
}


bool backDirCommand() {
	bool success;
	if (g_backDir.empty())
		return false;

	wstring s = g_backDir.back();
	if (SetCurrentDirectory(s.c_str())) {
		popLastDirCommand();
		return true;
	}

	return false;
}


bool upDirCommand() {
	bool success;

	saveCurDirCommand();
	success = SetCurrentDirectory(L"..");
	if (success)
		return true;
	popLastDirCommand();

	return false;
}

bool execCommand(LPCWSTR filename) {

	ShellExecute(NULL, (LPCWSTR)L"open", filename, (LPCWSTR)L"", (LPCWSTR)L"", SW_NORMAL);
	return true;
}

