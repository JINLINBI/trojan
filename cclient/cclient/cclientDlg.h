
// cclientDlg.h: 头文件
//

#pragma once


// CcclientDlg 对话框
class CcclientDlg : public CDialogEx
{
// 构造
public:
	CcclientDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CcclientDlg();
// 对话框数据
	enum { IDD = IDD_CCLIENT_DIALOG };


	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CImageList il;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedQuit();
	afx_msg void OnDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedReflash();
	int m_nIndex;
	bool is_copy_cut;
	// 服务器ip
	CIPAddressCtrl m_ip;
	// 服务器端口
	CEdit m_port;
	// 服务器登陆密码
	CEdit m_pass;
	CEdit m_pwd;
	CButton m_login;
	SOCKET CSocket;
	TCHAR guid[64];
	char cguid[64];
	CListCtrl m_clientlist;
	CListCtrl m_filelist;
	afx_msg void OnEnChangePassedit();

public:
	bool backCommand();
	bool lsCommand();
	bool upCommand();
	bool cdCommand(LPCWSTR);
	/* file command */
	bool lockCommand();
	bool unlockCommand();
	bool uploadCommand(LPTSTR);

	bool delCommand();
	bool copyCommand();
	bool pasteCommand();
	afx_msg void OnBnClickedBack();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedGo();

	void getPwdCommand();
	afx_msg void OnRclickList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCopyFile();
	afx_msg void OnCutFile();
	afx_msg void OnPasteFile();
	afx_msg void OnRemoveFile();
	afx_msg void OnEncryFile();
	afx_msg void OnUnlockFile();
	afx_msg void OnBnClickedButtonreflash();
	afx_msg void OnBnClickedButtonnew();
	afx_msg void OnDownload();
	afx_msg void OnSetfocusPwd();
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnExecCommand();
};
