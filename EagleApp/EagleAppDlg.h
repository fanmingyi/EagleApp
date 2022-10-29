
// EagleAppDlg.h: 头文件
//

#pragma once


// CEagleAppDlg 对话框
class CEagleAppDlg : public CDialogEx
{
// 构造
public:
	CEagleAppDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EAGLEAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	CString zcm;
	CString fjm;
	CString hwndTitle;
	CString webHook;
	CString findMsg;
	CString sc;
	CString matching;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
		virtual void OnCancel();
public:
	afx_msg void OnBnClickedButton1();
	CEdit editZcm;
	virtual void OnOK();
	CEdit editFjm;
protected:
//	afx_msg LRESULT OnFindpicmsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgfindpic(WPARAM wParam, LPARAM lParam);
public:
	CButton btnSub;
	CEdit editTitle;
	CEdit editWebhook;
	CEdit editSendMsgEdi;
	CEdit editMatching;
	CEdit editSC;
};
