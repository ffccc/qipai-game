#ifndef PROMPT_VIEW_HEAD_FILE
#define PROMPT_VIEW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//��ʾ��
class CPrompt : public CDialog
{
	//���ݱ���
protected:
	TCHAR							m_tStr[1024];				//�԰�

public:
	//���캯��
	CPrompt();   
	//��������
	virtual ~CPrompt();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();

	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//������Ϣ
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//
public:
	//���ö԰�
	void SetString(TCHAR szString[]);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////

#endif