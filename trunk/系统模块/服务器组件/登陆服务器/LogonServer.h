#pragma once

#ifndef __AFXWIN_H__
	#error �ڰ������� PCH �Ĵ��ļ�֮ǰ������stdafx.h��
#endif

#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//Ӧ�ó������
class CLogonServerApp : public CWinApp
{
	//��������
public:
	TCHAR								m_szDirWork[MAX_PATH];			//����Ŀ¼

	//��������
public:
	//���캯�� 
	CLogonServerApp();

	//���غ���
public:
	//��ʼ������
	virtual BOOL InitInstance();

	//��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////
