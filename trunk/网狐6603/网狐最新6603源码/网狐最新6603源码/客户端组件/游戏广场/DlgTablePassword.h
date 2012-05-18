#ifndef DLG_TABLE_PASSWORD_HEAD_FILE
#define DLG_TABLE_PASSWORD_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////

//��������
class CDlgTablePassword : public CSkinDialog
{
	//��������
public:
	TCHAR								m_szString[256];				//��ʾ��Ϣ
	TCHAR								m_szPassword[LEN_PASSWORD];		//��������

	//�ؼ�����
protected:
	CSkinButton							m_btOk;							//ȷ����ť
	CSkinButton							m_btCancel;						//ȡ����ť

	//�༭�ؼ�
protected:
	CStatic								m_stString;						//��ʾ�ؼ�
	CSkinEdit							m_edPassword;					//����ؼ�

	//��������
public:
	//���캯��
	CDlgTablePassword();
	//��������
	virtual ~CDlgTablePassword();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//���ú���
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();
	//�滭��Ϣ
	virtual VOID OnDrawClientArea(CDC * pDC, INT nWidth, INT nHeight);

	//���ܺ���
public:
	//������ʾ
	VOID SetPromptString(LPCTSTR pszString);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif