#pragma once

#include "Resource.h"
// CHintInfo �Ի���

class CHintInfo : public CDialog
{
	DECLARE_DYNAMIC(CHintInfo)

public:
	CHintInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHintInfo();

// �Ի�������
	enum { IDD = IDD_HINT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
public:
	void	SetShowInfo(CString& str);

	DECLARE_MESSAGE_MAP()
};
