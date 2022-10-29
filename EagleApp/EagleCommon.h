#pragma once
#include"obj.h"
#include<locale.h>
#include<vector>

bool commonInit(CString fjm, CString zcm, CString title, CString webHook, CString sendMsg, CString sc, CString matching);

#define msgfindpic WM_USER+200


/// @brief		�жϽ����Ƿ��Թ���Ա�������
	/// @param[in]	dwProcessId : ����ID
	/// @param[in]	bCurrentProcess : �Ƿ��ǵ�ǰ����
	///	@remark		bCurrentProcessΪtrueʱ,dwProcessId��Ч
	///	@return		�ǹ���Ա����򷵻�true
bool IsRunAsAdministrator(DWORD dwProcessId, bool bCurrentProcess);

/**
* ��Դ���յ�ʱ�����
*/
void closeApp();

/**
*	д������
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