#ifndef DLG_COLLOCATE_HEAD_FILE
#define DLG_COLLOCATE_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"

//////////////////////////////////////////////////////////////////////////////////

//��˵��
class CCollocateItem;

//����˵��
typedef CWHArray<CCollocateItem *> CCollocateItemArray;

//////////////////////////////////////////////////////////////////////////////////

//ѡ�����
class CCollocateBar : public CWnd
{
	//���Ϣ
protected:
	bool							m_bHovering;						//������־
	WORD							m_wItemDown;						//��������
	WORD							m_wItemHover;						//��������
	WORD							m_wItemActive;						//��������

	//������Ϣ
protected:
	CSize							m_SizeButton;						//��ť��С
	CRect							m_rcItemCreate;						//����λ��
	CCollocateItemArray				m_CollocateItemArray;				//��������

	//��������
public:
	//���캯��
	CCollocateBar();
	//��������
	virtual ~CCollocateBar();

	//���غ���
public:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//��������
public:
	//�������
	bool SaveParameter();
	//Ĭ�ϲ���
	bool DefaultParameter();

	//���ܺ���
public:
	//��������
	bool SetItemCreateRect(CRect rcItemCreate);
	//��������
	bool InsertCollocateItem(CCollocateItem * pCollocateItem);
	//��������
	bool SetActiveCollocateItem(CCollocateItem * pCollocateItem);

	//�ڲ�����
private:
	//�����л�
	WORD SwitchToButtonIndex(CPoint MousePoint);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//������Ϣ
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class CCollocateItem : public CDialog
{
	//��Ԫ����
	friend class CCollocateBar;
	friend class CDlgCollocate;

	//��������
protected:
	BYTE							m_cbImageIndex;						//ͼ����Ҫ

	//��������
public:
	//���캯��
	CCollocateItem(UINT uIDTemplate);
	//��������
	virtual ~CCollocateItem();

	//���غ���
protected:
	//ȷ����Ϣ
	virtual VOID OnOK() { GetParent()->PostMessage(WM_COMMAND,IDOK,0); }
	//ȡ����Ϣ
	virtual VOID OnCancel() { GetParent()->PostMessage(WM_COMMAND,IDCANCEL,0); }

	//���غ���
public:
	//�������
	virtual bool SaveParameter()=NULL;
	//Ĭ�ϲ���
	virtual bool DefaultParameter()=NULL;
	//���¿���
	virtual bool UpdateControlStatus()=NULL;

	//���ܺ���
public:
	//��������
	bool CreateCollocateItem(CWnd * pParentWnd, CRect rcCreate);

	//��Ϣ����
protected:
	//������Ϣ
	BOOL OnEraseBkgnd(CDC * pDC);
	//��ɫ��Ϣ
	HBRUSH OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//ͬ������
class CCollocateServer : public CCollocateItem
{
	//ʤ������
public:
	WORD							m_wMinWinRate;						//���ʤ��
	bool							m_bLimitWinRate;					//����ʤ��

	//��������
public:
	WORD							m_wMaxFleeRate;						//�������
	bool							m_bLimitFleeRate;					//���ƶ���

	//��������
public:
	LONG							m_lMaxGameScore;					//��߷��� 
	LONG							m_lMinGameScore;					//��ͷ���
	bool							m_bLimitGameScore;					//���Ʒ���

	//��������
public:
	bool							m_bLimitDetest;						//�������
	bool							m_bLimitSameIP;						//Ч���ַ

	//Я������
public:
	bool							m_bTakePassword;					//Я������
	TCHAR							m_szPassword[LEN_PASSWORD];			//��������

	//�༭�ؼ�
protected:
	CSkinEdit						m_edWinRate;						//�û�ʤ��
	CSkinEdit						m_edFleeRate;						//�û�����
	CSkinEdit						m_edScoreMin;						//��ͻ���
	CSkinEdit						m_edScoreMax;						//��߻���
	CSkinEdit						m_edPassword;						//��������

	//��������
public:
	//���캯��
	CCollocateServer();
	//��������
	virtual ~CCollocateServer();

	//���غ���
public:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//���ú���
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//�������
	virtual bool SaveParameter();
	//Ĭ�ϲ���
	virtual bool DefaultParameter();
	//���¿���
	virtual bool UpdateControlStatus();
};

//////////////////////////////////////////////////////////////////////////////////

//��������
class CCollocateProxy : public CCollocateItem
{
	//���ñ���
public:
	BYTE							m_cbProxyType;						//��������
	tagProxyServer					m_ProxyServer;						//������Ϣ

	//��ť�ؼ�
protected:
	CSkinButton						m_btProxyTest;						//���԰�ť
	CSkinComboBox					m_cmProxyType;						//��������

	//�༭�ؼ�
protected:
	CSkinEdit						m_edProxyPort;						//����˿�
	CSkinEdit						m_edProxyServer;					//�����ַ
	CSkinEdit						m_edProxyUserName;					//�����û�
	CSkinEdit						m_edProxyPassword;					//��������

	//��������
public:
	//���캯��
	CCollocateProxy();
	//��������
	virtual ~CCollocateProxy();

	//���غ���
public:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��������
	virtual BOOL OnInitDialog();

	//���غ���
public:
	//�������
	virtual bool SaveParameter();
	//Ĭ�ϲ���
	virtual bool DefaultParameter();
	//���¿���
	virtual bool UpdateControlStatus();

	//��Ϣ����
public:
	//�������
	VOID OnBnClickedProxyTest();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

//��Ϣ����
class CCollocateNotify : public CCollocateItem
{
	//���ñ���
public:
	WORD							m_wWhisperHotKey;					//˽���ȼ�
	bool							m_bAutoShowWhisper;					//��ʾ��Ϣ

	//ö�ٱ���
public:
	BYTE							m_cbInviteMode;						//����ģʽ
	BYTE							m_cbMessageMode;					//��Ϣģʽ

	//��������
public:
	//���캯��
	CCollocateNotify();
	//��������
	virtual ~CCollocateNotify();

	//���غ���
public:
	//�������
	virtual bool SaveParameter();
	//Ĭ�ϲ���
	virtual bool DefaultParameter();
	//���¿���
	virtual bool UpdateControlStatus();
};

//////////////////////////////////////////////////////////////////////////////////

//ϰ������
class CCollocateCustom : public CCollocateItem
{
	//���ñ���
public:
	bool							m_bSendWhisperByEnter;				//�س�����

	//��������
public:
	BYTE							m_cbActionHitAutoJoin;				//�Զ�����
	BYTE							m_cbActionLeftDoubleList;			//˫���б�

	//��������
public:
	//���캯��
	CCollocateCustom();
	//��������
	virtual ~CCollocateCustom();

	//���غ���
public:
	//�������
	virtual bool SaveParameter();
	//Ĭ�ϲ���
	virtual bool DefaultParameter();
	//���¿���
	virtual bool UpdateControlStatus();
};

//////////////////////////////////////////////////////////////////////////////////

//ϵͳ����
class CCollocateSystem : public CCollocateItem
{
	//���ñ���
public:
	WORD							m_wBossHotKey;						//�ϰ��ȼ�

	//���ñ���
public:
	bool							m_bAllowSound;						//��������
	bool							m_bAutoSitDown;						//�Զ�����
	bool							m_bSalienceTable;					//ͻ������
	bool							m_bSaveWhisperChat;					//����˽��
	bool							m_bNotifyUserInOut;					//������Ϣ
	bool							m_bNotifyFriendCome;				//������ʾ
	bool							m_bFullScreenBrowse;				//ȫ�����

	//��������
public:
	//���캯��
	CCollocateSystem();
	//��������
	virtual ~CCollocateSystem();

	//���غ���
public:
	//�������
	virtual bool SaveParameter();
	//Ĭ�ϲ���
	virtual bool DefaultParameter();
	//���¿���
	virtual bool UpdateControlStatus();
};

//////////////////////////////////////////////////////////////////////////////////

//���ô���
class CDlgCollocate : public CSkinDialog
{
	//���ñ���
protected:
	CParameterGame *				m_pParameterGame;					//��Ϸ����
	CParameterServer *				m_pParameterServer;					//��������

	//�ؼ�����
protected:
	CSkinButton						m_btOk;								//ȷ����ť
	CSkinButton						m_btCancel;							//ȡ����ť
	CSkinButton						m_btDefault;						//Ĭ�ϰ�ť
	CCollocateBar					m_CollocateBar;						//���ť

	//��������
public:
	CCollocateProxy					m_CollocateProxy;					//��������
	CCollocateSystem				m_CollocateSystem;					//ϵͳ����
	CCollocateCustom				m_CollocateCustom;					//ϰ������
	CCollocateNotify				m_CollocateNotify;					//��Ϣ����
	CCollocateServer				m_CollocateServer;					//��������

	//��������
public:
	//���캯��
	CDlgCollocate();
	//��������
	virtual ~CDlgCollocate();

	//���غ���
protected:
	//�ؼ���
	virtual VOID DoDataExchange(CDataExchange * pDX);
	//��������
	virtual BOOL OnInitDialog();
	//ȷ������
	virtual VOID OnOK();

	//���ܺ���
public:
	//���ò���
	bool InitCollocate(CParameterGame * pParameterGame, CParameterServer * pParameterServer);

	//��ť��Ϣ
protected:
	//Ĭ�ϰ�ť
	VOID OnBnClickedDefault();

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif