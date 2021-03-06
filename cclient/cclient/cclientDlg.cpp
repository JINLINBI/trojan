// cclientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "cclient.h"
#include "cclientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcclientDlg 对话框



CcclientDlg::CcclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CcclientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//m_hIcon = AfxGetApp()->LoadIcon(IDI_FILE);
	WSADATA WSADa;
	WSAStartup(0x0202, &WSADa);

}
CcclientDlg::~CcclientDlg() {
	WSACleanup();
}

void CcclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_ip);
	DDX_Control(pDX, IDC_PORTEDIT, m_port);
	DDX_Control(pDX, IDC_PASSEDIT, m_pass);
	DDX_Control(pDX, IDC_PWD, m_pwd);
	DDX_Control(pDX, IDC_LOGIN, m_login);
	DDX_Control(pDX, IDC_LIST1, m_clientlist);
	DDX_Control(pDX, IDC_LIST2, m_filelist);
}

BEGIN_MESSAGE_MAP(CcclientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_QUIT, &CcclientDlg::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_LOGIN, &CcclientDlg::OnBnClickedLogin)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CcclientDlg::OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CcclientDlg::OnDblclkList2)
	ON_BN_CLICKED(IDC_REFLASH, &CcclientDlg::OnBnClickedReflash)
	ON_EN_CHANGE(IDC_PASSEDIT, &CcclientDlg::OnEnChangePassedit)
	ON_BN_CLICKED(IDC_BACK, &CcclientDlg::OnBnClickedBack)
	ON_BN_CLICKED(IDC_UP, &CcclientDlg::OnBnClickedUp)
	ON_BN_CLICKED(IDC_GO, &CcclientDlg::OnBnClickedGo)
	ON_NOTIFY(NM_RCLICK, IDC_LIST2, &CcclientDlg::OnRclickList2)
	ON_COMMAND(ID_32771, &CcclientDlg::OnCopyFile)
	ON_COMMAND(ID_32772, &CcclientDlg::OnCutFile)
	ON_COMMAND(ID_32773, &CcclientDlg::OnPasteFile)
	ON_COMMAND(ID_32774, &CcclientDlg::OnRemoveFile)
	ON_COMMAND(ID_32776, &CcclientDlg::OnEncryFile)
	ON_COMMAND(ID_32777, &CcclientDlg::OnUnlockFile)
	ON_BN_CLICKED(IDC_BUTTONREFLASH, &CcclientDlg::OnBnClickedButtonreflash)
	ON_BN_CLICKED(IDC_BUTTONNEW, &CcclientDlg::OnBnClickedButtonnew)
	ON_COMMAND(ID_DOWNLOAD, &CcclientDlg::OnDownload)
	ON_EN_SETFOCUS(IDC_PWD, &CcclientDlg::OnSetfocusPwd)
	ON_WM_DROPFILES()
	ON_COMMAND(ID_32780, &CcclientDlg::OnExecCommand)
END_MESSAGE_MAP()


// CcclientDlg 消息处理程序

BOOL CcclientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标


	DWORD dwStyle = m_clientlist.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）

	m_clientlist.SetExtendedStyle(dwStyle);

	m_clientlist.InsertColumn(0, L"IP", LVCFMT_LEFT, 85);//插入列
	m_clientlist.InsertColumn(1, L"PORT", LVCFMT_LEFT, 50);
	m_clientlist.InsertColumn(2, L"GUID", LVCFMT_LEFT, 250);
	dwStyle = m_filelist.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）

	m_filelist.SetExtendedStyle(dwStyle);
	m_filelist.InsertColumn(0, L"FileName", LVCFMT_LEFT, 210);
	m_filelist.InsertColumn(1, L"FileType", LVCFMT_LEFT, 80);
	il.Create(16, 16, ILC_COLOR32, 1, 1);
	il.Add(AfxGetApp()->LoadIcon(IDI_FILE));
	il.Add(AfxGetApp()->LoadIcon(IDI_FILEMANAGER));
	il.Add(AfxGetApp()->LoadIcon(IDI_LOCK));
	m_filelist.SetImageList(&il, LVSIL_SMALL);
	// TODO:  在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CcclientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CcclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




char** splitArgs(char* cmd, int &total);
void freeArgs(char** argvs, int &total);


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
	if (cmd == NULL ||  cmd[len] != '\r')
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
		else {
			err = 1;
			break;
		}
			
		done++;
	}
 	if (err) {
		if (done && !strncmp(argv[0], "!", 1)) {
			// 用于显示目录，显示部分目录，不释放
			argc = done;
		}
		else
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
	if(cmd[0] != '\0' && time--) {
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


void CcclientDlg::OnBnClickedQuit() {
	exit(0);
}

void CcclientDlg::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码

	// TODO:  在此添加控件通知处理程序代码
	CString str;
	sockaddr_in SockAddrIn;
	char buff[1024 * 16];



	//GetDlgItemText(, IDC_IPADDRESS1, buff, 24);
	DWORD ip;
	m_login.GetWindowText(str);
	if (str.Compare(L"登陆")) {
		m_login.SetWindowTextW(L"登陆");
		closesocket(CSocket);
		return;
	}

	m_ip.GetAddress(ip);
	SockAddrIn.sin_family = AF_INET;
	SockAddrIn.sin_addr.s_addr = htonl(ip);
	SockAddrIn.sin_port = htons(GetDlgItemInt(IDC_PORTEDIT));
	CSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, 0);
	connect(CSocket, (struct sockaddr*)&SockAddrIn, sizeof(SockAddrIn));
	m_pass.GetWindowText(str);
	int n = WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)str.GetBuffer(0), -1, NULL, 0, NULL, NULL);
	char *pBuffer = (char*)malloc(n * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, (LPCWSTR)str.GetBuffer(0), -1, pBuffer, n * sizeof(char), NULL, NULL);

	sprintf(buff, "enable 2 %s\r\n", pBuffer);
	send(CSocket, buff, strlen(buff), 0);
	ZeroMemory(buff, sizeof(buff));
	recv(CSocket, buff, sizeof(buff), 0);
	if (!strncmp(buff, "+OK\r\n", 5)) {
		m_login.SetWindowTextW(L"注销");
		MessageBox(L"登陆成功",L"提示", MB_OK);
	}
	else
		MessageBox(L"请检查连接信息", L"登陆失败", MB_OK);
	free(pBuffer);

}


void CcclientDlg::OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第一个子项的文本   
		m_clientlist.GetItemText(pNMListView->iItem, 2, guid, 64);
		// 将选择的语言显示与编辑框中   
		int n = WideCharToMultiByte(CP_ACP, 0, guid, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, guid, -1, cguid, 64, NULL, NULL);

		lsCommand();
	}
}


void CcclientDlg::OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;
	char buf[1024 * 16];
	TCHAR argvBuf[1024];
	if (-1 != pNMListView->iItem)        // 如果iItem不是-1，就说明有列表项被选择   
	{
		// 获取被选择列表项第一个子项的文本   
		TCHAR filename[1024];

		m_filelist.GetItemText(pNMListView->iItem, 1, filename, 1024);
		if (!wcscmp(filename, L"文件夹")) {
			m_filelist.GetItemText(pNMListView->iItem, 0, filename, 1024);
			cdCommand(filename);
			lsCommand();
		}
	}
}


void CcclientDlg::OnBnClickedReflash()
{
	// TODO: 在此添加控件通知处理程序代码

	char buff[1024 * 16];
	int argc;
	m_clientlist.DeleteAllItems();
	sprintf(buff, "getclients\r\n");
	int count = send(CSocket, buff, strlen(buff), 0);
	if (count < 0)
		return;

	ZeroMemory(buff, sizeof(buff));
	int data = recv(CSocket, buff, sizeof(buff), 0);
	if (data <= 0) {
		m_login.SetWindowTextW(L"登陆");
		MessageBox(L"请检查连接信息", L"失败", MB_OK);
		return;
	}

	char** argv = splitArgs(buff, argc);
	for (int i = 0; i < argc && argv; i++) {
		int n = MultiByteToWideChar(CP_ACP, 0, argv[i], -1, NULL, 0);
		wchar_t *pBuffer = (wchar_t*)malloc(n * sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP, 0, argv[i], -1, pBuffer, n * sizeof(char));

		
		wchar_t *end1 = wcschr(pBuffer, L';') + 1;
		wchar_t *end2 = wcschr(end1, L';') + 1;
		int length = wcslen(pBuffer);
		for (int j = 0; j < length; j++) {
			if (pBuffer[j] == L';')
				pBuffer[j] = L'\0';
		}
		m_clientlist.InsertItem(i, pBuffer);
		m_clientlist.SetItemText(i, 1, end1);
		m_clientlist.SetItemText(i, 2, end2);

		free(pBuffer);
	}

	freeArgs(argv, argc);
}


void CcclientDlg::OnEnChangePassedit()
{

}

bool CcclientDlg::backCommand()
{
	
	if (cguid) {
		char sendbuf[64];
		sprintf(sendbuf, "back %s\r\n", cguid);
		int count = send(CSocket, sendbuf, strlen(sendbuf), 0);
		if (count < 0) {
			return false;
		}
		ZeroMemory(sendbuf, sizeof(sendbuf));
		count = recv(CSocket, sendbuf, sizeof(sendbuf), 0);
		if (count < 0) {
			return false;
		}
		if (!strcmp(sendbuf, "+OK\r\n")) {
			return true;
		}
			
	}
	return false;
}

bool CcclientDlg::lsCommand()
{
	char buf[1024 * 16 * 4];
	TCHAR argvBuf[1024];
	if (cguid) {
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "ls %s\r\n", cguid);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			return false;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (count < 0) {
			return false;
		}
		if (!strncmp(buf, "-", 1))
			return false;
		char **argv = splitArgs(buf, count);
		if (!argv || count <= 0)
			return false;
		m_filelist.DeleteAllItems();
		int j= 0;
		int length = MultiByteToWideChar(CP_ACP, 0, argv[0], -1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, argv[0], -1, argvBuf, length);
		if (argvBuf[0] == L'!') {
			m_pwd.SetWindowText(argvBuf + 1);
			j++;
		}
		else
			j = 0;
		for (int i = 0; i < count - j && argv; i++) {
			length = MultiByteToWideChar(CP_ACP, 0, argv[i + j], -1, NULL, 0);
			MultiByteToWideChar(CP_ACP, 0, argv[i + j], -1, argvBuf, length);

			if (argvBuf[0] == L'/') {
				m_filelist.InsertItem(i, argvBuf + 1, 1);
				m_filelist.SetItemText(i, 1, L"文件夹");
			}
			else if (argvBuf[0] == L'?') {
				m_filelist.InsertItem(i, argvBuf + 1, 2);
				m_filelist.SetItemText(i, 1, L"加密文件");
			}
			else {
				m_filelist.InsertItem(i, argvBuf, 0);
				m_filelist.SetItemText(i, 1, L"普通文件");
			}
		}
		freeArgs(argv, count);
	}
		
	return false;
}

bool CcclientDlg::upCommand()
{
	if (cguid) {
		char sendbuf[64];
		sprintf(sendbuf, "up %s\r\n", cguid);
		send(CSocket, sendbuf, strlen(sendbuf), 0);
		ZeroMemory(sendbuf, sizeof(sendbuf));
		recv(CSocket, sendbuf, sizeof(sendbuf), 0);
		if (!strcmp(sendbuf, "+OK\r\n")) {
			return true;
		}	
	}
	return false;
}

bool CcclientDlg::cdCommand(LPCWSTR directory)
{

	char buf[1024];
	TCHAR argvBuf[1024];

	int n = WideCharToMultiByte(CP_ACP, 0, directory, -1, NULL, 0, NULL, NULL);
	char *pBuffer = (char*)malloc(n * sizeof(char));
	WideCharToMultiByte(CP_ACP, 0, directory, -1, pBuffer, n * sizeof(char), NULL, NULL);

	if (cguid) {
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "cd  %s \"%s\"\r\n", cguid, pBuffer);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			return false;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (count < 0) {
			return false;
		}
		if (!strncmp(buf, "-", 1))
			return false;
		if (strcmp(buf, "+OK\r\n"))
			return true;
	}

	return false;
}


void CcclientDlg::OnBnClickedBack()
{
	// TODO: 在此添加控件通知处理程序代码
	backCommand();
	lsCommand();
}


void CcclientDlg::OnBnClickedUp()
{
	// TODO: 在此添加控件通知处理程序代码
	upCommand();
	lsCommand();
}


void CcclientDlg::OnBnClickedGo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_pwd.GetWindowText(str);

	if (cdCommand(str)){
		m_pwd.SetWindowText(str);
	}
	else
		lsCommand();
}


void CcclientDlg::OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	CMenu menu;
	//添加线程操作  
	VERIFY(menu.LoadMenu(IDR_MENU1));           //MENU的文件名称  
	CMenu* popup = menu.GetSubMenu(0);
	ASSERT(popup != NULL);
	popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	//下面的两行代码主要是为了后面的操作为准备的  
	//获取列表视图控件中第一个被选择项的位置    
	POSITION pstion = m_filelist.GetFirstSelectedItemPosition();
	//该函数获取由pos指定的列表项的索引，然后将pos设置为下一个位置的POSITION值  
	m_nIndex = m_filelist.GetNextSelectedItem(pstion);

}


void CcclientDlg::OnCopyFile()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR filename[1024];
	char buf[1024];
	TCHAR argvBuf[1024];
	

	if (cguid) {
		m_filelist.GetItemText(m_nIndex, 1, filename, 1024);
		if (!wcscmp(filename, L"文件夹")) {
			return;
		}
		m_filelist.GetItemText(m_nIndex, 0, filename, 1024);


		int n = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		char *pBuffer = (char*)malloc(n * sizeof(char));
		WideCharToMultiByte(CP_ACP, 0, filename, -1, pBuffer, n * sizeof(char), NULL, NULL);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "copy  %s \"%s\"\r\n", cguid, pBuffer);
		free(pBuffer);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			return ;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (strcmp(buf, "+OK\r\n")) {
			is_copy_cut = true;
			return;
		}
	}
	return;
}


void CcclientDlg::OnCutFile()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR filename[1024];
	char buf[1024];
	TCHAR argvBuf[1024];


	if (cguid) {
		m_filelist.GetItemText(m_nIndex, 1, filename, 1024);
		if (!wcscmp(filename, L"文件夹")) {
			return;
		}
		m_filelist.GetItemText(m_nIndex, 0, filename, 1024);

		int n = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		char *pBuffer = (char*)malloc(n * sizeof(char));
		WideCharToMultiByte(CP_ACP, 0, filename, -1, pBuffer, n * sizeof(char), NULL, NULL);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "cut  %s \"%s\"\r\n", cguid, pBuffer);
		free(pBuffer);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			is_copy_cut = false;
			return;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (count < 0 && !strncmp(buf, "-", 1)) {
			is_copy_cut = false;
			return;
		}
		if (strcmp(buf, "+OK\r\n")) {
			is_copy_cut = true;
			return;
		}
	}
	is_copy_cut = false;
	return;
}


void CcclientDlg::OnPasteFile()
{

	// TODO: 在此添加命令处理程序代码
	char buf[1024];
	if (cguid) {
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "paste  %s\r\n", cguid);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count <= 0) {
			is_copy_cut = false;
			return;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);

	}
	is_copy_cut = false;
	lsCommand();
	return;
}


void CcclientDlg::OnRemoveFile()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR filename[1024];
	char buf[1024];
	TCHAR argvBuf[1024];
	bool is_fileDir = false;
	if (cguid) {
		m_filelist.GetItemText(m_nIndex, 1, filename, 1024);
		if (!wcscmp(filename, L"文件夹")) {
			is_fileDir = true;
		}
		m_filelist.GetItemText(m_nIndex, 0, filename, 1024);

		int n = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		char *pBuffer = (char*)malloc(n * sizeof(char));
		WideCharToMultiByte(CP_ACP, 0, filename, -1, pBuffer, n * sizeof(char), NULL, NULL);

		memset(buf, 0, sizeof(buf));
		if(is_fileDir)
			sprintf(buf, "rmdir  %s \"%s\"\r\n", cguid, pBuffer);
		else
			sprintf(buf, "rm  %s \"%s\"\r\n", cguid, pBuffer);
		free(pBuffer);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			return;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (count < 0 && !strncmp(buf, "-", 1)) {
			return;
		}
	}
	
	lsCommand();
	return;
}


void CcclientDlg::OnEncryFile()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR filename[1024];
	char buf[1024];
	TCHAR argvBuf[1024];
	if (cguid) {
		m_filelist.GetItemText(m_nIndex, 1, filename, 1024);
		if (!wcscmp(filename, L"文件夹")) {
			return;
		}
		m_filelist.GetItemText(m_nIndex, 0, filename, 1024);

		int n = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		char *pBuffer = (char*)malloc(n * sizeof(char));
		WideCharToMultiByte(CP_ACP, 0, filename, -1, pBuffer, n * sizeof(char), NULL, NULL);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "lock  %s \"%s\"\r\n", cguid, pBuffer);
		free(pBuffer);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			return;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (count < 0 || !strncmp(buf, "-", 1)) {
			lsCommand();
			return;
		}
	}

	lsCommand();
	return;
}


void CcclientDlg::OnUnlockFile()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR filename[1024];
	char buf[1024];
	TCHAR argvBuf[1024];
	if (cguid) {
		m_filelist.GetItemText(m_nIndex, 1, filename, 1024);
		if (!wcscmp(filename, L"文件夹")) {
			return;
		}
		m_filelist.GetItemText(m_nIndex, 0, filename, 1024);

		int n = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		char *pBuffer = (char*)malloc(n * sizeof(char));
		WideCharToMultiByte(CP_ACP, 0, filename, -1, pBuffer, n * sizeof(char), NULL, NULL);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "unlock  %s \"%s\"\r\n", cguid, pBuffer);
		free(pBuffer);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			return;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (count < 0 || !strncmp(buf, "-", 1)) {
			lsCommand();
			return;
		}
	}
	lsCommand();
	return;
}


void CcclientDlg::OnBnClickedButtonreflash()
{
	// TODO: 在此添加控件通知处理程序代码
	lsCommand();
}


void CcclientDlg::OnBnClickedButtonnew()
{
	if (cguid) {
		int count = m_filelist.GetItemCount();
		TCHAR filename[128] = L"新建文件.txt";
		TCHAR getFilename[128];
		int find = 0, done = 1;
		do{
			done = 1;
			for (int i = 0; i < count; i++) {
				m_filelist.GetItemText(i, 1, getFilename, 1024);
				if (!wcscmp(filename, L"文件夹")) {
					continue;
				}
				m_filelist.GetItemText(i, 0, getFilename, 1024);

				if (!wcscmp(filename, getFilename)) {
					done = 0;
					find++;
					wsprintfW(filename, L"新建文件（%d）.txt", find);
					break;
				}
			}
		} while (find && !done);
		int n = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		char *pBuffer = (char*)malloc(n * sizeof(char));
		WideCharToMultiByte(CP_ACP, 0, filename, -1, pBuffer, n * sizeof(char), NULL, NULL);

		char buf[1024];
		sprintf(buf, "new %s \"%s\"\r\n", cguid, pBuffer);
		send(CSocket, buf, strlen(buf), 0);
		recv(CSocket, buf, sizeof(buf), 0);
		free(pBuffer);
	}
	lsCommand();
	return;
}


typedef struct {
	char type;
	int len;
	char data[1024];
}packet;

void CcclientDlg::OnDownload()
{
	// TODO: 在此添加命令处理程序代码
	TCHAR filename[1024];
	char buf[1024];
	TCHAR argvBuf[1024];
	if (cguid) {
		m_filelist.GetItemText(m_nIndex, 1, filename, 1024);
		if (!wcscmp(filename, L"文件夹")) {
			return;
		}
		m_filelist.GetItemText(m_nIndex, 0, filename, 1024);

		int n = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		char *pBuffer = (char*)malloc(n * sizeof(char));
		WideCharToMultiByte(CP_ACP, 0, filename, -1, pBuffer, n * sizeof(char), NULL, NULL);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "download  %s \"%s\"\r\n", cguid, pBuffer);
		free(pBuffer);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count <= 0) {
			return;
		}
			
		packet pac;
		
		memset(&pac, 0, sizeof(pac));
		count = recv(CSocket, (char*)&pac, sizeof(pac), 0);
		if (count <= 0 || pac.type != '+') {
			MessageBox(L"权限不够或文件已转移，不能打开文件！", L"提示");
			return;
		}
		HANDLE fd = CreateFile(filename, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

		while (1) {
			memset(&pac, 0, sizeof(pac));
			count = recv(CSocket, (char*)&pac, sizeof(pac), 0);
			switch (pac.type) {
			case 1:
				WriteFile(fd, pac.data, pac.len, NULL, NULL);
				break;
			default:
				CloseHandle(fd);
				return;
			}
		}
		CloseHandle(fd);
	}
	
	return;
}


void CcclientDlg::OnSetfocusPwd()
{
	// 编辑器获取焦点时设置gobtn为默认回车按钮
}


void CcclientDlg::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFilePathName[_MAX_PATH + 1] = { 0 };
	
	UINT count = DragQueryFile(hDropInfo, 0xffffffff, NULL, 0);
	for (UINT nIndex = 0; nIndex < count; ++nIndex) {
		DragQueryFile(hDropInfo, nIndex, szFilePathName, _MAX_PATH);
		uploadCommand(szFilePathName);
	}

	CDialogEx::OnDropFiles(hDropInfo);
}


bool CcclientDlg::uploadCommand(LPTSTR filename)
{
	// TODO: 在此添加命令处理程序代码
	char buf[BUFSIZ];
	char pBuffer[_MAX_DIR];
	if (cguid) {
		int n = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		char *pBuffer = (char*)malloc(n * sizeof(char));
		WideCharToMultiByte(CP_ACP, 0, filename, -1, pBuffer, n * sizeof(char), NULL, NULL);


		char* f = strrchr(pBuffer, '\\');
		if (f)
			return false;
		memset(buf, 0, sizeof(buf));
		sprintf(buf, "upload  %s \"%s\"\r\n", cguid, filename);
		free(f);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0 || buf[0] != '+') return false;

		packet pac;

		HANDLE fd = CreateFile(filename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		int leftlen = GetFileSize(fd, NULL);
		int readedCount = 0;
		if (fd) {
			while (leftlen > 0) {
				ReadFile(fd, pac.data, 1024, (LPDWORD)&readedCount, NULL);
				leftlen -= readedCount;
				if (leftlen <= 0) {
					pac.type = 2;
					send(CSocket, (const char*)&pac, readedCount + 1, 0);
				}
				else {
					pac.type = 1;
					send(CSocket, (const char*)&pac, sizeof(pac), 0);
				}
			}
			return true;
		}

		CloseHandle(fd);
	}

	return false;
}

void CcclientDlg::OnExecCommand()
{
	TCHAR filename[1024];
	char buf[1024];
	TCHAR argvBuf[1024];
	if (cguid) {
		m_filelist.GetItemText(m_nIndex, 1, filename, 1024);
		if (!wcscmp(filename, L"文件夹") || !wcscmp(filename, L"加密文件")) {
			return;
		}
		m_filelist.GetItemText(m_nIndex, 0, filename, 1024);

		int n = WideCharToMultiByte(CP_ACP, 0, filename, -1, NULL, 0, NULL, NULL);
		char *pBuffer = (char*)malloc(n * sizeof(char));
		WideCharToMultiByte(CP_ACP, 0, filename, -1, pBuffer, n * sizeof(char), NULL, NULL);

		memset(buf, 0, sizeof(buf));
		sprintf(buf, "exe  %s \"%s\"\r\n", cguid, pBuffer);
		free(pBuffer);
		int count = send(CSocket, buf, strlen(buf), 0);
		if (count < 0) {
			return;
		}
		memset(buf, 0, sizeof(buf));
		count = recv(CSocket, buf, sizeof(buf), 0);
		if (count < 0 || !strncmp(buf, "-", 1)) {
			lsCommand();
			return;
		}
	}
	lsCommand();
	return;
}
