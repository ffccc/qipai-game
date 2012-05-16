#ifndef HTML_BROWSER_HEAD_FILE
#define HTML_BROWSER_HEAD_FILE

#pragma once

#include "ComService.h"

//////////////////////////////////////////////////////////////////////////

//�������ͼ
class COM_SERVICE_CLASS CHtmlBrower : public CHtmlView
{
	//��������
protected:
	bool							m_bEnableBrowser;					//�Ƿ�����
	CString							m_strNavigateUrl;					//ҳ���ַ

	//��������
public:
	//���캯��
	CHtmlBrower();
	//��������
	virtual ~CHtmlBrower();

	//���ܺ���
public:
	//ת����ҳ
	void Navigate(LPCTSTR pszUrl);
	//������ҳ
	bool EnableBrowser(bool bEnableBrowser);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif