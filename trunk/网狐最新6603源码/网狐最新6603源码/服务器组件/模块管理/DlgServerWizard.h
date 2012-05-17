#ifndef DLG_SERVER_WIZARD_HEAD_FILE
#define DLG_SERVER_WIZARD_HEAD_FILE

#pragma once

#include "ModuleListControl.h"
#include "ModuleInfoManager.h"
#include "ServerInfoManager.h"
#include "ModuleManagerHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CDlgServerWizard;

//////////////////////////////////////////////////////////////////////////////////

//���û���
class MODULE_MANAGER_CLASS CDlgServerOptionItem : public CDialog
{
	//��Ԫ����
	friend class CDlgServerWizardItem2;

	//��������
protected:
	CDlgServerWizard *				m_pDlgServerWizard;					//��ָ��
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��������
	tagGameServiceOption *			m_pGameServiceOption;				//��������

	//��������
protected:
	//���캯��
	CDlgServerOptionItem(UINT nIDTemplate);
	//��������
	virtual ~CDlgServerOptionItem();

	//���غ���
protected:
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//�ӿں���
protected:
	//��������
	virtual bool SaveInputInfo()=NULL;
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight)=NULL;

	//���ܺ���
private:
	//��������
	bool SaveItemData();
	//��ʾ����
	bool ShowOptionItem(const CRect & rcRect, CWnd * pParentWnd);

	//��Ϣ����
private:
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class MODULE_MANAGER_CLASS CDlgServerOptionItem1 : public CDlgServerOptionItem
{
	//��������
public:
	//���캯��
	CDlgServerOptionItem1();
	//��������
	virtual ~CDlgServerOptionItem1();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//��������
	virtual bool SaveInputInfo();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);

	//���ܺ���
protected:
	//����ؼ�
	VOID InitCtrlWindow();
	//��������
	VOID UpdateDataBaseName();

	//�ؼ�����
protected:
	//ѡ��ı�
	VOID OnSelchangeServerType();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class MODULE_MANAGER_CLASS CDlgServerOptionItem2 : public CDlgServerOptionItem
{
	//��������
public:
	//���캯��
	CDlgServerOptionItem2();
	//��������
	virtual ~CDlgServerOptionItem2();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//��������
	virtual bool SaveInputInfo();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);

	//���ܺ���
protected:
	//����ؼ�
	VOID InitCtrlWindow();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class MODULE_MANAGER_CLASS CDlgServerOptionItem3 : public CDlgServerOptionItem
{
	//��������
public:
	//���캯��
	CDlgServerOptionItem3();
	//��������
	virtual ~CDlgServerOptionItem3();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//��������
	virtual bool SaveInputInfo();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class MODULE_MANAGER_CLASS CDlgServerOptionItemCustom : public CDlgServerOptionItem
{
	//�ؼ�����
protected:
	HWND							m_hCustomRule;						//���ƹ���
	IGameServiceCustomRule *		m_pIGameServiceCustomRule;			//�Զ�����

	//��������
public:
	//���캯��
	CDlgServerOptionItemCustom();
	//��������
	virtual ~CDlgServerOptionItemCustom();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//��������
	virtual bool SaveInputInfo();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);

	//��Ϣ����
private:
	//������Ϣ
	VOID OnNcDestroy();
	//������Ϣ
	VOID OnSetFocus(CWnd * pNewWnd);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//�򵼻���
class MODULE_MANAGER_CLASS CDlgServerWizardItem : public CDialog
{
	//��Ԫ����
	friend class CDlgServerWizard;

	//��������
protected:
	CDlgServerWizard *				m_pDlgServerWizard;					//��ָ��
	tagGameServiceAttrib *			m_pGameServiceAttrib;				//��������
	tagGameServiceOption *			m_pGameServiceOption;				//��������

	//��������
protected:
	//���캯��
	CDlgServerWizardItem(UINT nIDTemplate);
	//��������
	virtual ~CDlgServerWizardItem();

	//���غ���
protected:
	//ȷ������
	virtual VOID OnOK();
	//ȡ����Ϣ
	virtual VOID OnCancel();

	//�ӿں���
protected:
	//��������
	virtual bool SaveInputInfo()=NULL;
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight)=NULL;

	//���ܺ���
private:
	//��������
	bool SaveItemData();
	//������
	bool ShowWizardItem(const CRect & rcRect, CWnd * pParentWnd);

	//��Ϣ����
private:
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϸѡ��
class MODULE_MANAGER_CLASS CDlgServerWizardItem1 : public CDlgServerWizardItem
{
	//��Ԫ����
	friend class CDlgServerOptionItem1;

	//�б����
protected:
	CStatic							m_StaticPrompt;						//��ʾ�ؼ�
	CModuleInfoBuffer				m_ModuleInfoBuffer;					//ģ����Ϣ
	CModuleListControl				m_ModuleListControl;				//ģ���б�
	CModuleInfoManager				m_ModuleInfoManager;				//ģ�����

	//�������
protected:
	CGameServiceManagerHelper		m_GameServiceManager;				//��Ϸģ��

	//��������
public:
	//���캯��
	CDlgServerWizardItem1();
	//��������
	virtual ~CDlgServerWizardItem1();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//��������
	virtual bool SaveInputInfo();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);

	//���ܺ���
protected:
	//����ģ��
	bool LoadDBModuleItem();

	//�ؼ���Ϣ
protected:
	//˫���б�
	VOID OnNMDblclkModuleList(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class MODULE_MANAGER_CLASS CDlgServerWizardItem2 : public CDlgServerWizardItem
{
	//��������
protected:
	WORD							m_wItemCount;						//������Ŀ
	WORD							m_wActiveIndex;						//�����
	CDlgServerOptionItem *			m_pOptionItem[4];					//��ָ��

	//�ؼ�����
protected:
	CTabCtrl						m_TabCtrl;							//ѡ��ؼ�
	CDlgServerOptionItem1			m_ServerOptionItem1;				//���ò���
	CDlgServerOptionItem2			m_ServerOptionItem2;				//���ò���
	CDlgServerOptionItem3			m_ServerOptionItem3;				//���ò���
	CDlgServerOptionItemCustom		m_ServerOptionItemCustom;			//��������

	//��������
public:
	//���캯��
	CDlgServerWizardItem2();
	//��������
	virtual ~CDlgServerWizardItem2();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//��������
	virtual bool SaveInputInfo();
	//�����ؼ�
	virtual VOID RectifyControl(INT nWidth, INT nHeight);

	//���ܺ���
public:
	//��������
	bool ActiveOptionItem(WORD wIndex);

	//�ؼ���Ϣ
protected:
	//������Ϣ
	VOID OnSetFocus(CWnd * pNewWnd);
	//ѡ��ı�
	VOID OnTcnSelchangeTabCtrl(NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//������
class MODULE_MANAGER_CLASS CDlgServerWizard : public CDialog
{
	//��Ԫ����
	friend class CDlgServerWizardItem2;
	friend class CDlgServerOptionItem1;
	friend class CDlgServerOptionItemCustom;

	//��������
protected:
	WORD							m_wActiveIndex;						//�����
	CDlgServerWizardItem *			m_pWizardItem[2];					//��ָ��

	//������Ϣ
public:
	tagModuleInitParameter			m_ModuleInitParameter;				//���ò���

	//�ؼ�����
protected:
	CDlgServerWizardItem1			m_ServerWizardItem1;				//���ò���
	CDlgServerWizardItem2			m_ServerWizardItem2;				//���ò���

	//�ӿڱ���
protected:
	IGameServiceManager *			m_pIGameServiceManager;				//�������
	IGameServiceCustomRule *		m_pIGameServiceCustomRule;			//�Զ�����

	//��������
public:
	//���캯��
	CDlgServerWizard();
	//��������
	virtual ~CDlgServerWizard();

	//���غ���
public:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//���ú���
public:
	//��������
	bool CreateGameServer();
	//��������
	VOID SetWizardParameter(IGameServiceManager * pIGameServiceManager, tagGameServiceOption * pGameServiceOption);

	//��������
private:
	//������
	bool ActiveWizardItem(WORD wIndex);

	//��Ϣӳ��
public:
	//��һ��
	VOID OnBnClickedLast();
	//��һ��
	VOID OnBnClickedNext();
	//��ɰ�ť
	VOID OnBnClickedFinish();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif