#pragma once
#ifndef RULESET_HEAD_FILE
#define RULESET_HEAD_FILE

#endif
#include "StdAfx.h"
#include "Resource.h"
#include "afxwin.h"

//��������
class CGameRuleSet : public CSkinDialogEx
{
	DECLARE_DYNAMIC(CGameRuleSet)
	//��������
public:

	enum						{ IDD = IDD_INFO };
	CSkinButton					m_Play;							//��ʼ��Ϸ��ť

	//��������
public:
	//���캯��
	CGameRuleSet(CWnd* pParent = NULL);   // ��׼���캯��
	//��������
	virtual ~CGameRuleSet();
	//��ʼ������
	virtual BOOL OnInitDialog();

protected:
	//DDX/DDV ֧��
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStratPlay();
	afx_msg void OnClose();
};
