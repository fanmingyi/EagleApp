#include "pch.h"
#include "EagleCommon.h"

static dmsoft* g_dm = nullptr;


typedef  int(__stdcall* SetDllPathW)(CString path, int mode);

bool commonInit(CString fjm, CString zcm, CString title, CString webHook, CString sendMsg,CString sc,CString matching)
{

	
	if (!IsRunAsAdministrator(0, true))
	{
		AfxMessageBox(_T("管理员身份运行"));
		return false;
	}
	// 初始化COM(mta)
	CoInitializeEx(NULL, 0);


	// 设置本地字符集为gbk
	setlocale(LC_ALL, "chs");

	HMODULE h = LoadLibrary(_T("DmReg.dll"));
	auto funSetDll = (SetDllPathW)GetProcAddress(h, (char*)0x2);

	auto regDllRet = funSetDll(_T("./dm.dll"), 0);

	if (regDllRet != 1)
	{
		return false;
	}
	g_dm = new dmsoft;
	if (g_dm == NULL)
	{
		return 1;
	}

	TCHAR* fjmChar = _tcsdup(fjm);
	TCHAR* zcmChar = _tcsdup(zcm);
	auto d = _tcslen(fjmChar);
	// 注册
	long dm_ret = g_dm->Reg(zcmChar, fjmChar);
	free(fjmChar);
	free(zcmChar);
	if (dm_ret != 1)
	{
		CString str;
		str.Format(_T("注册失败:%d\n"), dm_ret);
		AfxMessageBox(str);
		_tprintf(_T("注册失败:%d\n"), dm_ret);
		delete g_dm;
		g_dm = nullptr;
		return false;
	}
	writeConfig(fjmKey(), fjm);
	writeConfig(zcmKey(), zcm);
	writeConfig(titleKey(), title);
	writeConfig(webhookKey(), webHook);
	writeConfig(sendMsgKey(), sendMsg);
	writeConfig(scKey(), sc);
	writeConfig(matchingKey(), matching);
	
	

	return true;
}


/// @brief		判断进程是否以管理员身份运行
	/// @param[in]	dwProcessId : 进程ID
	/// @param[in]	bCurrentProcess : 是否是当前进程
	///	@remark		bCurrentProcess为true时,dwProcessId无效
	///	@return		是管理员身份则返回true
bool IsRunAsAdministrator(DWORD dwProcessId, bool bCurrentProcess)
{
	bool bIsElevated = false;
	HANDLE hToken = NULL;

	DWORDLONG dwlConditionMask = 0;
	OSVERSIONINFOEX WinVersion;
	ZeroMemory(&WinVersion, sizeof(OSVERSIONINFOEX));
	WinVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	WinVersion.dwMajorVersion = 6;
	VER_SET_CONDITION(dwlConditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);

	BOOL flag = VerifyVersionInfo(
		&WinVersion,
		VER_MAJORVERSION,
		dwlConditionMask
	);

	if (!flag)//不是VISTA、Windows7
		return (false);

	do
	{
		HANDLE hProcess = NULL;
		if (bCurrentProcess)
		{
			hProcess = GetCurrentProcess();
		}
		else
		{
			hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcessId);
		}

		if (!hProcess)
		{
			bIsElevated = false;
			break;
		}

		HANDLE hToken = NULL;
		if (!::OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
		{
			bIsElevated = false;
			::CloseHandle(hProcess);
			break;
		}

		TOKEN_ELEVATION elevation;
		DWORD dwRetLen = 0;
		if (!::GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwRetLen))
		{
			bIsElevated = false;
			::CloseHandle(hToken);
			::CloseHandle(hProcess);
			break;
		}

		if (dwRetLen == sizeof(elevation))
		{
			bIsElevated = elevation.TokenIsElevated;
		}
		::CloseHandle(hToken);
		::CloseHandle(hProcess);

	} while (false);

	return bIsElevated;
}

void closeApp()
{

	if (g_dm == nullptr)
	{
		delete g_dm;
		g_dm = nullptr;
	}
}

TCHAR* configPath = _T("Software\\EagleApp");

bool writeConfig(CString keyName, CString value)
{

	HKEY openkey = NULL;

	LONG nRet = RegCreateKey(HKEY_CURRENT_USER, configPath, &openkey);

	if (nRet != ERROR_SUCCESS)
	{
		return false;
	}
	//LPTSTR lpszData = new TCHAR[value.GetLength() + 1]; // setvalue is CString type...
	//_tcscpy(lpszData, value);
	nRet = RegSetValueEx(openkey, keyName, 0, REG_SZ, (CONST BYTE*)value.GetBuffer(), ((value.GetLength() + 1) * sizeof(TCHAR)));
	//delete[] lpszData;// don't forget to do this.

	if (nRet != ERROR_SUCCESS)
	{
		return false;
	}

	return true;
}
CString readConfig(CString keyName)
{
	CString ret;

	HKEY openkey = NULL;

	LONG nRet = RegCreateKey(HKEY_CURRENT_USER, configPath, &openkey);

	if (nRet != ERROR_SUCCESS)
	{
		return ret;
	}
	//LPTSTR lpszData = new TCHAR[value.GetLength() + 1]; // setvalue is CString type...
	//_tcscpy(lpszData, value);
	TCHAR* t = new TCHAR[300];
	DWORD tsize;
	nRet = RegGetValue(openkey, NULL, keyName, RRF_RT_REG_SZ, NULL, t, &tsize);



	if (nRet == ERROR_SUCCESS)
	{
		ret = t;
	}
	delete[] t;
	return ret;
}

CString fjmKey()
{
	return CString("FJM");
}

CString zcmKey()
{
	return CString("ZCM");
}

CString titleKey()
{
	return CString("TITLE");
}

CString webhookKey()
{
	return CString("WebHook");
}

CString sendMsgKey()
{
	return CString("sendMsgKey");
}

CString scKey()
{
	return CString("scKey");
}

CString matchingKey()
{
	return CString("matchingKey");
}

#include<math.h>

bool fidPic(CString title, CString sc, double matching) {

	
	long hwnd = g_dm->FindWindow(_T(""), title);

//long ret2 =  g_dm->BindWindowEx(hwnd, _T("gdi"), _T("normal"), _T("normal"), nullptr, 0);


	//long x1, y1, x2, y2;
	long mWidth, mHei;
	mWidth=g_dm->GetScreenWidth();
	mHei =g_dm->GetScreenHeight();
	//long  d = g_dm->GetClientRect(hwnd, &x1, &y1, &x2, &y2);
	//g_dm->GetClientSize(hwnd, &mWidth, &mHei);
	//g_dm->SetClientSize(hwnd, mWidth/2, mHei/2);
	/*if (abs(x2 - x1) <= 0 || (x2 <= 0 && x1 <= 0))
	{
		return false;
	}*/
	std::vector<CString> findList;
	Get_Image_List(_T("./findimg/*.bmp"), findList);
	CString findPath;
	bool firstIMG =true ;
	//iterator遍历
	for (std::vector<CString>::iterator it = findList.begin(); it != findList.end(); it++)
	{
		if (firstIMG)
		{
			findPath += *it;
			firstIMG = false;
		}else{
			findPath +=_T("|")+ (*it) ;

		}
	}
	//CString dm_ret = g_dm->FindPicE(0, 0, g_dm->GetScreenWidth(), g_dm->GetScreenHeight(), _T("./find.bmp"), _T("000000"), 0.5, 0);
	//CString dm_ret = g_dm->FindPicE(0, 0, 2000,2000, _T("./find.bmp"), _T("000000"), 0.5, 0);
	
	g_dm->EnableFindPicMultithread(1);
	g_dm->SetFindPicMultithreadCount(1);

	OutputDebugString(_T("\r\n开始查找\r\n"));
	CString dm_ret = g_dm->FindPicE(0, 0, mWidth, mHei, findPath, sc, matching, 0);
	//CString dm_ret = g_dm->FindPicE(x1, y1, x2, y2, findPath, _T("000000"), 0.8, 0);
	OutputDebugString(_T("\r\n查找完成\r\n"));
	OutputDebugString(dm_ret);
	int searchStart = 0;
	auto ret = dm_ret.Tokenize(_T("|"), searchStart);
	int i = _ttoi(ret);
	if (i >= 0)
	{
		OutputDebugString(_T("找到:\r\n"));
		return true;
	}
	else {
		OutputDebugString(_T("没找到了:\r\n"));
	}
	return false;


}

CStringA UTF16_UTF8(const CStringW& utf16)
{
	if (utf16.IsEmpty()) return "";
	CStringA utf8;
	int cc = 0;

	if ((cc = WideCharToMultiByte(CP_UTF8, 0, utf16, -1, NULL, 0, 0, 0) - 1) > 0)
	{
		char* buf = utf8.GetBuffer(cc);
		if (buf) WideCharToMultiByte(CP_UTF8, 0, utf16, -1, buf, cc, 0, 0);
		utf8.ReleaseBuffer();
	}
	return utf8;
}

CStringW UTF8_UTF16(const CStringA& utf8)
{
	if (utf8.IsEmpty()) return L"";
	CStringW utf16;
	int cc = 0;

	if ((cc = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0) - 1) > 0)
	{
		wchar_t* buf = utf16.GetBuffer(cc);
		if (buf) MultiByteToWideChar(CP_UTF8, 0, utf8, -1, buf, cc);
		utf16.ReleaseBuffer();
	}
	return utf16;
}

void Get_Image_List(CString strPath, std::vector<CString>& vectList)
{
	// 遍历文件夹
	CFileFind finder;
	CString   strFindFile;
	CString   strFindPath;
	strFindPath.Format(_T("%s"), strPath.GetBuffer());
	BOOL bWorking = finder.FindFile(strFindPath);
	
	while (bWorking)
	{
		//寻找下一个文件
		bWorking = finder.FindNextFile();
		//判断是否是目录 并且 判断目录是否是“.”或“..”  
		//详细介绍见[注解1]
		if (finder.IsDirectory() && !finder.IsDots())
		{
			continue;
		}
		//获取当前文件路径
		strFindFile = finder.GetFilePath();

		OutputDebugString(strFindFile);
		vectList.push_back(strFindFile);
	}
	finder.Close();
}
