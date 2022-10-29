#pragma once
#include"obj.h"
#include<locale.h>
#include<vector>

bool commonInit(CString fjm, CString zcm, CString title, CString webHook, CString sendMsg, CString sc, CString matching);

#define msgfindpic WM_USER+200


/// @brief		判断进程是否以管理员身份运行
	/// @param[in]	dwProcessId : 进程ID
	/// @param[in]	bCurrentProcess : 是否是当前进程
	///	@remark		bCurrentProcess为true时,dwProcessId无效
	///	@return		是管理员身份则返回true
bool IsRunAsAdministrator(DWORD dwProcessId, bool bCurrentProcess);

/**
* 资源回收的时候调用
*/
void closeApp();

/**
*	写入配置
**/
bool writeConfig(CString keyName, CString value);

CString readConfig(CString keyName);

CString fjmKey();

CString zcmKey();

CString titleKey();

CString webhookKey();

CString sendMsgKey();
CString scKey();
CString matchingKey();

bool fidPic(CString title,CString sc, double matching);

CStringA UTF16_UTF8(const CStringW& utf16);
CStringW UTF8_UTF16(const CStringA& utf8);


void Get_Image_List(CString strPath, std::vector<CString>&vectList);