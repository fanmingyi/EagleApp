
// EagleAppDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "EagleApp.h"
#include "EagleAppDlg.h"
#include "afxdialogex.h"
#include"EagleCommon.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEagleAppDlg 对话框



CEagleAppDlg::CEagleAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EAGLEAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEagleAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, EDIT_ZCM, editZcm);
	DDX_Control(pDX, EDIT_FJM, editFjm);
	DDX_Control(pDX, IDC_BUTTON1, btnSub);
	DDX_Control(pDX, EDIT_TITLE, editTitle);
	DDX_Control(pDX, EDIT_DD_WEBHOOK, editWebhook);
	DDX_Control(pDX, Edit_Send_Msg, editSendMsgEdi);
	DDX_Control(pDX, ET_MATCHING, editMatching);
	DDX_Control(pDX, ET_SC, editSC);
}

BEGIN_MESSAGE_MAP(CEagleAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CEagleAppDlg::OnBnClickedButton1)
	ON_MESSAGE(msgfindpic, &CEagleAppDlg::OnMsgfindpic)
END_MESSAGE_MAP()


// CEagleAppDlg 消息处理程序

BOOL CEagleAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	fjm = readConfig(fjmKey());
	zcm = readConfig(zcmKey());
	hwndTitle = readConfig(titleKey());
	webHook = readConfig(webhookKey());
	findMsg = readConfig(sendMsgKey());
	sc = readConfig(scKey());
	matching = readConfig(matchingKey());

	editFjm.SetWindowText(fjm);
	editZcm.SetWindowText(zcm);
	editTitle.SetWindowText(hwndTitle);
	editWebhook.SetWindowText(webHook);
	editSendMsgEdi.SetWindowText(findMsg);
	editSC.SetWindowText(sc);
	editMatching.SetWindowText(matching);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CEagleAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEagleAppDlg::OnPaint()
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
HCURSOR CEagleAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CEagleAppDlg::OnCancel()
{
	CDialogEx::OnCancel();
	closeApp();
}

void CALLBACK MyTimeProc(
	HWND hwnd,
	UINT message,
	UINT idTimer,
	DWORD dwTime)
{

	OutputDebugString(_T("定时器"));
	//fidPic();
}
int findpic = false;
UINT MyThreadFunction(LPVOID pParam)
{

	CString hwndTitle = readConfig(titleKey());
	CString sc = readConfig(scKey());
	CString matching = readConfig(matchingKey());
	double matchingN = _wtof(matching);
	while (true)
	{
		while (findpic)
		{
			Sleep(1000);
			OutputDebugString(_T("成功查找 但是主线程未处理\r\n"));
		}



		auto ret = fidPic(hwndTitle,sc, matchingN);
		if (ret)
		{
			findpic = true;
			PostMessage(HWND(pParam), msgfindpic, 0, 0);
		}
		Sleep(1000);

	}

}
/// <summary>
/// 点击确定按钮
/// </summary>
void CEagleAppDlg::OnBnClickedButton1()
{
	editZcm.GetWindowText(zcm);
	editFjm.GetWindowText(fjm);
	editTitle.GetWindowText(hwndTitle);
	editWebhook.GetWindowText(webHook);
	editSendMsgEdi.GetWindowText(findMsg);
	editSC.GetWindowText(sc);
	editMatching.GetWindowText(matching);
	if (sc.IsEmpty())
	{
		sc = "000000";
		editSC.SetWindowText(sc);
	}
	if (matching.IsEmpty())
	{
		matching = "0.8";
		editMatching.SetWindowText(matching);
	}

	auto ret = commonInit(fjm, zcm, hwndTitle, webHook, findMsg,sc, matching);
	if (!ret)
	{
		AfxMessageBox(_T("初始化失败\r\n"));
		ExitProcess(EXIT_FAILURE);
	}
	auto searT = AfxBeginThread(MyThreadFunction, GetSafeHwnd());
	btnSub.EnableWindow(false);
	btnSub.SetWindowTextW(_T("已启动"));

	editFjm.EnableWindow(false);
	//PostMessage(msgfindpic,0,0);
	//SetTimer(123, 10000, MyTimeProc);

}


void CEagleAppDlg::OnOK()
{

}



#include<cpr/cpr.h>
afx_msg LRESULT CEagleAppDlg::OnMsgfindpic(WPARAM wParam, LPARAM lParam)
{

	CStringA aUrl(webHook);

	CStringA aSendMsg(findMsg);
	CString uContent;
	uContent.Format(_T("{\"msgtype\": \"text\",\"text\": {\"content\":\"msg:%s\"}}"), findMsg);
	cpr::Response r = cpr::Post(
		cpr::Url{ aUrl },
		cpr::Header{ {"Content-Type", " application/json"} },
		cpr::Body{ UTF16_UTF8(uContent)}

	);
	r.status_code;                  // 200
	r.header["content-type"];       // application/json; charset=utf-8
	r.text;                         // JSON text string

	AfxMessageBox(_T("发现预期界面 确定后继续查找\r\n"));

	findpic = false;

	return 0;
}
