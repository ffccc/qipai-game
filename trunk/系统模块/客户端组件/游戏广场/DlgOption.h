#ifndef DLG_OPTION_HEAD_FILE
#define DLG_OPTION_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "SystemOption.h"

//////////////////////////////////////////////////////////////////////////

//��˵��
class CDlgOptionItem;
typedef CArrayTemplate<CDlgOptionItem *> COptionsItemPtrArray;

//////////////////////////////////////////////////////////////////////////

//��ť������
class CButtonBar : public CWnd
{
	//��������
protected:
	INT_PTR								m_nHotItem;						//������
	INT_PTR								m_nActiveItem;					//������
	COptionsItemPtrArray				m_OptionsItemPtr;				//��ť����

	//������Ϣ
protected:
	CWnd								* m_pItemParent;				//�����
	CRect								m_rcItemRect;					//���ÿ�λ��
	
	//λͼ��Դ
protected:
	CSkinImage							m_ImageHot;						//��Դ����
	CSkinImage							m_ImageActive;					//��Դ����
	CSkinImage							m_ImageNormal;					//��Դ����
	CSkinImage							m_ImageBackGround;				//��Դ����
	
	//��������
public:
	//���캯��
	CButtonBar();
	//��������
	virtual ~CButtonBar();
	
	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	
	//���ܺ���
public:
	//������Ϣ
	bool InitButtonBar(const CRect & rcItemRect, CWnd * pItemParent);
	//��������
	bool SetActiveOptionItem(CDlgOptionItem * pDlgOptionItem);
	//��������
	bool InsertOptionItem(CDlgOptionItem * pDlgOptionItem);
	//������Դ
	bool UpdateSkinResource();
	
	//�ڲ�����
private:
	//��������
	INT_PTR ItemHitTest(CPoint MousePoint);
	
	//��Ϣ����
protected:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//��������
class CDlgOptionItem : public CDialog
{
	//��������
private:
	bool							m_bCreate;							//�Ƿ���
	const UINT						m_uIDTemplate;						//ģ���ʾ

	//��������
public:
	//���캯��
	CDlgOptionItem(UINT uIDTemplate);
	//��������
	virtual ~CDlgOptionItem();

	//���غ���
protected:
	//ȷ����Ϣ
	virtual void OnOK() { GetParent()->PostMessage(WM_COMMAND,IDOK,0); }
	//ȡ����Ϣ
	virtual void OnCancel() { GetParent()->PostMessage(WM_COMMAND,IDCANCEL,0); }

	//���غ���
public:
	//��������
	virtual bool SaveOptions()=NULL;
	//��ȡ����
	virtual LPCTSTR GetTitleText()=NULL;
	//��ȡ����
	virtual LPCTSTR GetButtonText()=NULL;

	//���ܺ���
public:
	//��������
	bool CreateOptionsItem(CWnd * pParentWmd);
	//�Ƿ�ѡ��
	bool IsButtonSelected(UINT uButtonID);
	//ѡ�а�ť
	void SetButtonSelected(UINT uButtonID, bool bSelected);
	
	//��Ϣ����
protected:
	//�ػ�����
	afx_msg void OnPaint();
	//������Ϣ
	afx_msg BOOL OnEraseBkgnd(CDC * pDC);
	//��ɫ��Ϣ
	afx_msg HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//���ò���
struct tagSystemParameter
{
	enInviteMode						enInviteMode;					//����ģʽ
	enAcountsRule						enAcountsRule;					//�ʻ�����
	bool								bShowInOutMessage;				//������Ϣ
	WORD								wBossHotKey;					//�ϰ��ȼ�
};

//ϵͳ����
class CDlgOptionSystem : public CDlgOptionItem
{
	//���ñ���
public:
	tagSystemParameter					m_SystemParameter;				//���ò���

	//��������
public:
	//���캯��
	CDlgOptionSystem();
	//��������
	virtual ~CDlgOptionSystem();

	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//��������
	virtual bool SaveOptions();
	//��ȡ����
	virtual LPCTSTR GetTitleText();
	//��ȡ����
	virtual LPCTSTR GetButtonText();
};

//////////////////////////////////////////////////////////////////////////

//���ò���
struct tagTableRuleParameter
{
	bool								bPassword;							//��������
	bool								bLimitWin;							//����ʤ��
	bool								bLimitFlee;							//���ƶ���
	bool								bLimitScore;						//���Ʒ���
	bool								bLimitDetest;						//�������
	bool								bCheckSameIP;						//Ч���ַ
	WORD								wWinRate;							//ʤ����
	WORD								wFleeRate;							//������
	LONG								lMaxScore;							//��߷��� 
	LONG								lLessScore;							//��ͷ���
	TCHAR								szPassword[PASS_LEN];				//��������
};

//ͬ������
class CDlgOptionTableRule : public CDlgOptionItem
{
	//���ñ���
public:
	tagTableRuleParameter				m_TableRuleParameter;				//���ò���

	//��������
public:
	//���캯��
	CDlgOptionTableRule();
	//��������
	virtual ~CDlgOptionTableRule();

	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//��������
	virtual bool SaveOptions();
	//��ȡ����
	virtual LPCTSTR GetTitleText();
	//��ȡ����
	virtual LPCTSTR GetButtonText();
};

//////////////////////////////////////////////////////////////////////////

//���öԻ�����
class CDlgOption : public CSkinDialogEx
{
	//���ñ���
protected:
	CGameOption							* m_pGameOption;				//��Ϸ����
	CServerOption						* m_pServerOption;				//��������

	//�ؼ�����
public:
	CButtonBar							m_ButtonBar;					//ѡ��ť
	CSkinButton							m_btOk;							//ȷ����ť
	CSkinButton							m_btCancel;						//ȡ����ť

	//��������
public:
	CDlgOptionSystem					m_SystemItem;					//ϵͳ����
	CDlgOptionTableRule					m_TableRuleItem;				//��������

	//��������
public:
	//���캯��
	CDlgOption(CGameOption * pGameOption, CServerOption * pServerOption);
	//��������
	virtual ~CDlgOption();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//��Ϣ����
public:
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//�����������
class CDlgEnterTablePass : public CSkinDialogEx
{
	//�������
private:
	LPTSTR								m_lpPassword;					//��������
	BYTE								m_cbPasswordLen;				//���볤��

	//�ؼ�����
public:
	CSkinButton							m_btOk;							//ȷ����ť
	CSkinButton							m_btCancel;						//ȡ����ť

	//��������
public:
	//���캯��
	CDlgEnterTablePass();
	//��������
	virtual ~CDlgEnterTablePass();

	//���غ���
protected:
	//�ؼ���
	virtual void DoDataExchange(CDataExchange * pDX);
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual void OnOK();
	//ȡ����Ϣ
	virtual void OnCancel();

	//���ܺ���
public:
	//���ñ���
	void SetPassword(LPTSTR lpPassword,BYTE cbPasswordLen);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif