#ifndef HINT_MSG_HEAD_FILE
#define HINT_MSG_HEAD_FILE

#pragma once

#include "ClientShare.h"

//////////////////////////////////////////////////////////////////////////

//��Ϣ����
class CLIENT_SHARE_CLASS CHintMsgBox : public CSkinDialogEx
{
	//���Ա���
protected:
	UINT							m_uType;							//��Ϣ����
	CString							m_strString;						//��ʾ��Ϣ
	CString							m_strCaption;						//��Ϣ����

	//��������
protected:
	UINT							m_nElapse;							//ʣ��ʱ��
	UINT							m_nBtCount;							//��ť��Ŀ
	CRect							m_rcString;							//�ַ�λ��
	HINSTANCE						m_hResInstance;						//��Դ���

	//�ؼ�����
protected:
	CSkinButton						m_btButton[3];						//��ť�ؼ�

	//��������
public:
	//���캯��
	CHintMsgBox(CWnd * pParentWnd=NULL);
	//��������
	virtual ~CHintMsgBox();

	//���غ���
protected:
	//��ʼ����Ϣ
	virtual BOOL OnInitDialog();
	//������Ϣ
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//���ܺ���
public:
	//��ʾ��Ϣ
	INT ShowMessageBox(LPCTSTR pszString, UINT uType=MB_OK, UINT nElapse=0);
	//��ʾ��Ϣ
	INT ShowMessageBox(LPCTSTR pszCaption, LPCTSTR pszString, UINT uType=MB_OK, UINT nElapse=0);
	//�߿���
	INT GetXBorder(){return 2;}
	//�߿�߶�
	INT GetYBorder(){return 2;}


	//�ڲ�����
private:
	//������ť
	VOID CreateBoxButton();

	//��Ϣӳ��
protected:
	//�滭��Ϣ
	VOID OnPaint();
	//ʱ����Ϣ
	VOID OnTimer(UINT_PTR nIDEvent);

	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////////

//��ʾ��Ϣ
extern "C" CLIENT_SHARE_CLASS INT __cdecl ShowInformation(LPCTSTR pszString, UINT nElapse, UINT uType);
extern "C" CLIENT_SHARE_CLASS INT __cdecl ShowInformationEx(LPCTSTR pszString, UINT nElapse, UINT uType ,LPCTSTR pszCaption);


//////////////////////////////////////////////////////////////////////////

#endif