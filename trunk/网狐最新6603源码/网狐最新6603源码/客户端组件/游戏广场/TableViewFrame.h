#ifndef TABLE_VIEW_FRAME_HEAD_FILE
#define TABLE_VIEW_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GamePlazaHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
#define INDEX_ENTER_CHAIR			MAX_CHAIR							//��������

//��������
struct tagTableAttribute
{
	//���ӱ�־
	bool							bLocker;							//�����־
	bool							bPlaying;							//��Ϸ��־
	bool							bFocusFrame;						//��ܱ�־

	//����״̬
	WORD							wWatchCount;						//�Թ���Ŀ
	DWORD							dwTableOwnerID;						//��������

	//���Ա���
	WORD							wTableID;							//���Ӻ���
	WORD							wChairCount;						//������Ŀ
	IClientUserItem *				pIClientUserItem[MAX_CHAIR];		//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//������Դ
class CTableResource
{
	//��������
public:
	CSize							m_SizeChair;						//���ӳߴ�
	CSize							m_SizeTable;						//���ӳߴ�
	CSize							m_SizeEnter;						//����ߴ�

	//���Ա���
public:
	bool							m_bShowUser;						//��ʾ�û�
	bool							m_bShowEnter;						//��ʾ����
	bool							m_bShowChair;						//��ʾ����
	bool							m_bShowTable;						//��ʾ����
	bool							m_bShowFocus;						//��ʾ����
	bool							m_bCustomUser;						//�Զ��û�
	bool							m_bShowTableID;						//��ʾ����
	bool							m_bRenderImage;						//��Ⱦ��־

	//��Ŀ����
public:
	WORD							m_wTableItemCount;					//������Ŀ
	WORD							m_wChairItemCount;					//������Ŀ

	//������Ŀ
public:
	WORD							m_wUserGenreCount;					//������Ŀ
	WORD							m_wTableGenreCount;					//������Ŀ
	WORD							m_wChairGenreCount;					//������Ŀ

	//λ������
public:
	CPoint							m_ptLock;							//����λ��
	CPoint							m_ptEnter;							//����λ��
	CPoint							m_ptTableID;						//����λ��
	CPoint							m_ptReadyArray[MAX_CHAIR];			//׼��λ��

	//���λ��
public:
	CRect							m_rcNameArray[MAX_CHAIR];			//����λ��
	CRect							m_rcChairArray[MAX_CHAIR];			//����λ��

	//��ɫ����
public:
	COLORREF						m_crName;							//������ɫ
	COLORREF						m_crMember;							//��Ա��ɫ
	COLORREF						m_crMaster;							//������ɫ

	//��������
public:
	INT								m_nDrawStyle[MAX_CHAIR];			//�����ʽ
	TCHAR							m_szDirectory[MAX_PATH];			//��Դ·��

	//�ⲿ��Դ
public:
	CPngImage						m_ImageEnter;						//����λͼ
	CBitImage						m_ImageChair;						//����λͼ
	CBitImage						m_ImageTable;						//����λͼ
	CBitImage						m_ImageGround;						//����λͼ
	CPngImage						m_ImageCustomUser;					//�Զ�λͼ

	//�ڲ���Դ
public:
	CPngImage						m_ImageReady;						//׼��λͼ
	CPngImage						m_ImageLocker;						//����λͼ
	CPngImage						m_ImageNumberNB;					//����λͼ
	CPngImage						m_ImageNumberBK;					//���뱳��

	//��������
public:
	//���캯��
	CTableResource();
	//��������
	virtual ~CTableResource();

	//���ܺ���
public:
	//�ͷ���Դ
	VOID DestroyResource();
	//������Դ
	bool LoadResource(LPCTSTR pszResDirectory, WORD wChairCount);
	//������Դ
	bool UpdateResource(CSkinRenderManager * pSkinRenderManager);

	//��������
protected:
	//��ȡ��ֵ
	LONG ReadStringValue(LPCTSTR & pszSring);
	//��ȡ����
	bool ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);
	//��ȡ����
	bool ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);
	//��ȡ��ɫ
	bool ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile);
};

//////////////////////////////////////////////////////////////////////////////////

//������ͼ
class CTableView : public ITableView
{
	//״̬����
protected:
	bool							m_bMouseDown;						//�Ƿ���
	WORD							m_wHoverChairID;					//��������
	tagTableAttribute				m_TableAttribute;					//��������

	//����ӿ�
protected:
	ITableViewFrame *				m_pITableViewFrame;					//���ӽӿ�

	//��������
public:
	//���캯��
	CTableView();
	//��������
	virtual ~CTableView();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//��������
	virtual WORD GetNullChairCount(WORD & wNullChairID);
	//���ú���
	virtual VOID InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableViewFrame);

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wChairID);
	//������Ϣ
	virtual bool SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem);

	//״̬�ӿ�
public:
	//��갴��
	virtual VOID SetMouseDown(bool bMouseDown);
	//������
	virtual VOID SetFocusFrame(bool bFocusFrame);
	//����λ��
	virtual VOID SetHoverStatus(WORD wHoverChairID);
	//����״̬ 
	virtual VOID SetTableStatus(bool bPlaying, bool bLocker);

	//��ѯ�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag() { return m_TableAttribute.bPlaying; }
	//�����־
	virtual bool GetLockerFlag() { return m_TableAttribute.bLocker; }

	//���ܺ���
public:
	//�滭����
	VOID DrawTableView(CDC * pDC, DWORD dwServerRule, CTableResource * pTableResource);
	//�滭����
	VOID DrawTableName(CDC * pDC, WORD wChairID, LPCTSTR pszNickName, CTableResource * pTableResource);
};

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef CWHArray<CTableView *>		CTableViewArray;					//��������

//���ӿ��
class CTableViewFrame : public CWnd, public ITableViewFrame
{
	//��Ԫ����
	friend class CTableView;

	//״̬��Ϣ
protected:
	bool							m_bHovering;						//������־
	bool							m_bLMouseDown;						//�Ҽ���־
	bool							m_bRMouseDown;						//�����־

	//��������
protected:
	WORD							m_wDownTableID;						//���Ӻ���
	WORD							m_wDownChairID;						//���Ӻ���

	//���Ա���
protected:
	WORD							m_wTableCount;						//��Ϸ����
	WORD							m_wChairCount;						//������Ŀ
	DWORD							m_dwServerRule;						//�������

	//������Ϣ
protected:
	INT								m_nXExcursion;						//����ƫ��
	INT								m_nYExcursion;						//����ƫ��
	INT								m_nXTableCount;						//��������
	INT								m_nYTableCount;						//��������

	//������Ϣ
protected:
	INT								m_nScrollPos;						//����λ��
	INT								m_nScrollPosMax;					//���λ��
	INT								m_nVisibleLineCount;				//�ɼ�����

	//�ؼ�����
protected:
	CSkinScrollBar					m_SkinScrollBar;					//��������
	CTableResource					m_TableResource;					//������Դ
	CTableViewArray					m_TableViewArray;					//��������

	//����ӿ�
protected:
	ITableViewFrameSink *			m_pITableViewFrameSink;				//֪ͨ�ӿ�

	//��������
public:
	//���캯��
	CTableViewFrame();
	//��������
	virtual ~CTableViewFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��������
	virtual bool CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx);
	//��������
	virtual bool ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, LPCTSTR pszResDirectory);

	//��Ϣ�ӿ�
public:
	//������Ŀ
	virtual WORD GetTableCount() { return m_wTableCount; }
	//������Ŀ
	virtual WORD GetChairCount() { return m_wChairCount; }
	//��ȡ��Դ
	virtual CTableResource * GetTableResource() { return &m_TableResource; }

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wTableID, WORD wChairID);
	//������Ϣ
	virtual bool SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem);

	//״̬�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag(WORD wTableID);
	//�����־
	virtual bool GetLockerFlag(WORD wTableID);
	//������
	virtual VOID SetFocusFrame(WORD wTableID, bool bFocusFrame);
	//����״̬ 
	virtual VOID SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker);

	//��ͼ����
public:
	//���ӿ���
	virtual bool VisibleTable(WORD wTableID);
	//��������
	virtual bool FlashGameTable(WORD wTableID);
	//��������
	virtual bool FlashGameChair(WORD wTableID, WORD wChairID);

	//���ܽӿ�
public:
	//��������
	virtual bool UpdateTableView(WORD wTableID);
	//��ȡ����
	virtual ITableView * GetTableViewItem(WORD wTableID);
	//��������
	virtual WORD GetNullChairCount(WORD wTableID, WORD & wNullChairID);

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//��������
protected:
	//���Ӳ���
	WORD SwitchTableID(POINT MousePoint);
	//���Ӳ���
	WORD SwitchChairID(POINT MousePoint);
	//�����ؼ�
	VOID RectifyControl(INT nWidth, INT nHeight);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//λ����Ϣ
	VOID OnSize(UINT nType, INT cx, INT cy);
	//�����Ϣ
	VOID OnRButtonUp(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint MousePoint);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//�����Ϣ
	VOID OnRButtonDown(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	VOID OnLButtonDblClk(UINT nFlags, CPoint MousePoint);
	//�����Ϣ
	BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);
	//�����Ϣ
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint);
	//������Ϣ
	VOID OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);

	//�Զ���Ϣ
protected:
	//�¼���Ϣ
	LRESULT OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif