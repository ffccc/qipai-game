#ifndef TABLE_FRAME_HEAD_FILE
#define TABLE_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "UserManager.h"

//////////////////////////////////////////////////////////////////////////

//�궨��
#define NOTE_LEN						64									//���ӱ�ע

//�ӿ�˵��
class CTableResource;
interface ITableFrameView;

//////////////////////////////////////////////////////////////////////////

//��Ϸ��������
struct tagTableState
{
	bool								bPass;								//�����־
	bool								bPlaying;							//��Ϸ��־
	WORD								wTableID;							//���Ӻ���
	WORD								wWatchCount;						//�Թ���Ŀ
	WORD								wChairCount;						//�����Ŀ
	DWORD								dwTableMasterID;					//̨�����
	TCHAR								szTableNote[NOTE_LEN];				//���ӱ�ע
	IUserItem							* pIUserItem[MAX_CHAIR];			//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITableView INTERFACE_VERSION(1,1)
static const GUID IID_ITableView={0xd10c8db6,0xb9d8,0x47c2,0x9c,0xb9,0x34,0x97,0x59,0xa1,0x87,0x77};

//���ӽӿ�
interface ITableView : IUnknownEx
{
	//��ʼ������
	virtual void __cdecl InitTableView(WORD wTableID, WORD wChairCount, ITableFrameView * pITableFrameView)=NULL;
	//�����û���Ϣ
	virtual bool __cdecl SetUserInfo(WORD wChairID, IUserItem * pIUserItem)=NULL;
	//������갴��
	virtual void __cdecl SetMouseDown(bool bMouseDown)=NULL;
	//���ý�����
	virtual void __cdecl SetFocusFrame(bool bFocusFrame)=NULL;
	//��������λ��
	virtual void __cdecl SetHoverChairID(WORD wHoverChairID)=NULL;
	//������Ϸ��־ 
	virtual void __cdecl SetPlayFlag(bool bPlaying)=NULL;
	//���������־
	virtual void __cdecl SetPassFlag(bool bPass)=NULL;
	//���ñ�ע
	virtual void __cdecl SetTableNote(LPCTSTR pszNote)=NULL;
	//��ȡ�û���Ϣ
	virtual IUserItem * __cdecl GetUserInfo(WORD wChairID)=NULL;
	//��ȡ��������
	virtual WORD __cdecl GetNullChairCount(WORD & wNullChairID)=NULL;
	//��ѯ������
	virtual bool __cdecl QueryFocusFrame()=NULL;
	//��ѯ����λ��
	virtual WORD __cdecl QueryHoverChairID()=NULL;
	//��ѯ��Ϸ��־
	virtual bool __cdecl QueryPlayFlag()=NULL;
	//��ѯ�����־
	virtual bool __cdecl QueryPassFlag()=NULL;
	//��ȡ��������
	virtual const tagTableState * GetTableAttrib()=NULL;
	//�滭����
	virtual void __cdecl DrawTable(CDC * pDC, bool bHideInfo, CTableResource * pTableResource, bool bShowUser)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITableFrameView INTERFACE_VERSION(1,1)
static const GUID IID_ITableFrameView={0x719fbd24,0x5b49,0x4714,0x95,0x16,0xca,0xd5,0xf1,0x52,0xe7,0x40};

//��ܽӿ�
interface ITableFrameView : public IUnknownEx
{
	//��������
	virtual bool __cdecl CreateTableFrame(CWnd * pParentWnd, UINT uWndID)=NULL;
	//���ú���
	virtual bool __cdecl InitTableFrame(WORD wTableCount, WORD wPlayerCount, bool bHideInfo, const tagGameKind * pGameKind, IUnknownEx * pIUnknownEx)=NULL;
	//���ٺ���
	virtual void __cdecl DestroyTableFrame()=NULL;
	//��������
	virtual void __cdecl UpdateTableView(WORD wTableID, bool bMustUpdate)=NULL;
	//��ȡ��Ϸ��ָ��
	virtual ITableView * __cdecl GetTableArrayPtr(WORD wTableID)=NULL;
	//������ʾ
	virtual bool __cdecl ShowUserInfo(bool bShowUser)=NULL;
	//������ͼ����
	virtual bool __cdecl EnsureVisibleTable(WORD wTableID)=NULL;
	//��������
	virtual bool __cdecl FlashTable(WORD wTableID)=NULL;
	//��������
	virtual bool __cdecl FlashChair(WORD wTableID, WORD wChairID)=NULL;
	//��ȡ������Ŀ
	virtual WORD __cdecl GetTableCount()=NULL;
	//��ȡ������Ŀ
	virtual WORD __cdecl GetChairCount()=NULL;
	//�����û���Ϣ
	virtual bool __cdecl SetUserInfo(WORD wTableID, WORD wChairID, IUserItem * pIUserItem)=NULL;
	//������Ϸ��־ 
	virtual void __cdecl SetPlayFlag(WORD wTableID, bool bPlaying)=NULL;
	//���������־
	virtual void __cdecl SetPassFlag(WORD wTableID, bool bPass)=NULL;
	//��ȡ�û���Ϣ
	virtual IUserItem * __cdecl GetUserInfo(WORD wTableID, WORD wChairID)=NULL;
	//��ȡ��������
	virtual WORD __cdecl GetNullChairCount(WORD wTableID, WORD & wNullChairID)=NULL;
	//��ѯ��Ϸ��־
	virtual bool __cdecl QueryPlayFlag(WORD wTableID)=NULL;
	//��ѯ�����־
	virtual bool __cdecl QueryPassFlag(WORD wTableID)=NULL;
	//��ȡ��������
	virtual const tagTableState * GetTableAttrib(WORD wTableID)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_ITableFrameSink INTERFACE_VERSION(1,1)
static const GUID IID_ITableFrameSink={0xc06ff71d,0x2c2,0x4e51,0x9d,0xe6,0x22,0x43,0x85,0x7c,0x16,0xda};

//��ܻص��ӿ�
interface ITableFrameSink : public IUnknownEx
{
	//����������
	virtual void __cdecl OnLButtonHitTable(WORD wTableID, WORD wChairID, bool bMirror)=NULL;
	//����Ҽ�����
	virtual void __cdecl OnRButtonHitTable(WORD wTableID, WORD wChairID, bool bMirror)=NULL;
	//���˫��
	virtual void __cdecl OnDoubleHitTable(WORD wTableID, bool bMirror)=NULL;
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ������Դ��
class CTableResource
{
	friend class CTableFrame;

	//��������
public:
	WORD								m_wWidth;							//���ӿ��
	WORD								m_wHeight;							//���Ӹ߶�
	COLORREF							m_crBackGround;						//������ɫ

	//λ�ñ���
public:
	CPoint								m_ptLock;							//��λ��
	CRect								m_rcNote;							//��עλ��
	CRect								m_rcTableID;						//����λ��
	CPoint								m_ptReady[MAX_CHAIR];				//׼��λ��
	CRect								m_rcName[MAX_CHAIR];				//����λ��
	CRect								m_rcChair[MAX_CHAIR];				//����λ��

	//��ɫ����
public:
	COLORREF							m_crName;							//������ɫ
	COLORREF							m_crNote;							//��ע��ɫ
	COLORREF							m_crTableID;						//������ɫ
	int									m_nDrawStyle[MAX_CHAIR];			//�����ʽ

	//��־����
public:
	bool								m_bDZShowHand;						//�����˿�

	//λͼ��Դ
public:
	CSkinImage							m_ImageLock;						//����λͼ
	CSkinImage							m_ImagePlay;						//��Ϸλͼ
	CSkinImage							m_ImageReady;						//׼��λͼ
	CSkinImage							m_ImageTable;						//����λͼ
	CSkinImage							m_ImagePeople;						//�û�λͼ

	//��������
protected:
	//���캯��
	CTableResource();
	//��������
	virtual ~CTableResource();

	//���ܺ���
protected:
	//������Դ
	bool LoadFromFiles(bool bHideInfo,LPCTSTR pszGameName, WORD wChairCount, WORD wKindID);
	//ж����Դ
	void DestroyResource();
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ��������
class CTableView : public ITableView
{
	//��������
protected:
	bool								m_bMouseDown;						//�Ƿ���
	bool								m_bFocusFrame;						//��ܱ�־
	WORD								m_wHoverChairID;					//��������
	tagTableState						m_TableState;						//��������
	ITableFrameView						* m_pITableFrameView;				//��ܽӿ�

	//��������
public:
	//���캯��
	CTableView();
	//��������
	virtual ~CTableView();

	//��������
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��ʼ������
	virtual void __cdecl InitTableView(WORD wTableID, WORD wChairCount, ITableFrameView * pITableFrameView);
	//�����û���Ϣ
	virtual bool __cdecl SetUserInfo(WORD wChairID, IUserItem * pIUserItem);
	//������갴��
	virtual void __cdecl SetMouseDown(bool bMouseDown);
	//���ý�����
	virtual void __cdecl SetFocusFrame(bool bFocusFrame);
	//��������λ��
	virtual void __cdecl SetHoverChairID(WORD wHoverChairID);
	//������Ϸ��־ 
	virtual void __cdecl SetPlayFlag(bool bPlaying);
	//���������־
	virtual void __cdecl SetPassFlag(bool bPass);
	//���ñ�ע
	virtual void __cdecl SetTableNote(LPCTSTR pszNote);

	//��ѯ�ӿ�
public:
	//��ȡ�û���Ϣ
	virtual IUserItem * __cdecl GetUserInfo(WORD wChairID);
	//��ȡ��������
	virtual WORD __cdecl GetNullChairCount(WORD & wNullChairID);
	//��ѯ������
	virtual bool __cdecl QueryFocusFrame() { return m_bFocusFrame; }
	//��ѯ����λ��
	virtual WORD __cdecl QueryHoverChairID() { return m_wHoverChairID; }
	//��ѯ��Ϸ��־
	virtual bool __cdecl QueryPlayFlag() { return m_TableState.bPlaying; }
	//��ѯ�����־
	virtual bool __cdecl QueryPassFlag() { return m_TableState.bPass; }
	//��ȡ��������
	virtual const tagTableState * GetTableAttrib() { return &m_TableState; }

	//�滭�ӿ�
public:
	//�滭����
	virtual void __cdecl DrawTable(CDC * pDC, bool bHideInfo, CTableResource * pTableResource, bool bShowUser);
};

//////////////////////////////////////////////////////////////////////////

//��Ϸ���ӿ��
class CTableFrame : public CWnd, public ITableFrameView
{
	//״̬��Ϣ
protected:
	WORD								m_wDownChairID;						//λ�ú���
	WORD								m_wDownTableID;						//���º���
	WORD								m_wFrameTableID;					//�߿����
	bool								m_bLeftMouseDown;					//��갴��
	bool								m_bRightMouseDown;					//��갴��

	//���ñ���
protected:
	bool								m_bShowUser;						//�Ƿ���ʾ
	bool								m_bHideInfo;						//������Ϣ
	WORD								m_wTableCount;						//��Ϸ����
	WORD								m_wChairCount;						//������Ŀ

	//������Ϣ
protected:
	int									m_iXExcursion;						//����ƫ��
	int									m_iCurrentPos;						//��ǰλ��
	int									m_iMaxScrollPos;					//���λ��
	int									m_iCountRow;						//��������
	int									m_iCountAllLine;					//��������
	int									m_iCountVisibleLine;				//�ɼ�����

	//�ؼ�����
protected:
	CTableResource						m_TableResource;					//������Դ
	CTableView							* m_pTableViewArray;				//��������
	ITableFrameSink						* m_pITableFrameSink;			//�ص��ӿ�

	//���ò���
protected:
	static int							m_iScrollPos;						//��������
	static int							m_iWhellTimes;						//���ֱ���

	//��������
public:
	//���캯��
	CTableFrame();
	//��������
	virtual ~CTableFrame();

	//������Ϣ
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��������
	virtual bool __cdecl CreateTableFrame(CWnd * pParentWnd, UINT uWndID);
	//���ú���
	virtual bool __cdecl InitTableFrame(WORD wTableCount, WORD wChairCount, bool bHideInfo, const tagGameKind * pGameKind, IUnknownEx * pIUnknownEx);
	//���ٺ���
	virtual void __cdecl DestroyTableFrame();

	//���ܽӿ�
public:
	//��������
	virtual void __cdecl UpdateTableView(WORD wTableID, bool bMustUpdate);
	//��ȡ��Ϸ��ָ��
	virtual ITableView * __cdecl GetTableArrayPtr(WORD wTableID);
	//������ʾ
	virtual bool __cdecl ShowUserInfo(bool bShowUser);
	//������ͼ����
	virtual bool __cdecl EnsureVisibleTable(WORD wTableID);
	//��������
	virtual bool __cdecl FlashTable(WORD wTableID);
	//��������
	virtual bool __cdecl FlashChair(WORD wTableID, WORD wChairID);

	//��Ϣ�ӿ�
public:
	//��ȡ������Ŀ
	virtual WORD __cdecl GetTableCount() { return m_wTableCount; }
	//��ȡ������Ŀ
	virtual WORD __cdecl GetChairCount() { return m_wChairCount; }
	//�����û���Ϣ
	virtual bool __cdecl SetUserInfo(WORD wTableID, WORD wChairID, IUserItem * pIUserItem);
	//������Ϸ��־ 
	virtual void __cdecl SetPlayFlag(WORD wTableID, bool bPlaying);
	//���������־
	virtual void __cdecl SetPassFlag(WORD wTableID, bool bPass);
	//��ȡ�û���Ϣ
	virtual IUserItem * __cdecl GetUserInfo(WORD wTableID, WORD wChairID);
	//��ȡ��������
	virtual WORD __cdecl GetNullChairCount(WORD wTableID, WORD & wNullChairID);
	//��ѯ��Ϸ��־
	virtual bool __cdecl QueryPlayFlag(WORD wTableID);
	//��ѯ�����־
	virtual bool __cdecl QueryPassFlag(WORD wTableID);
	//��ȡ��������
	virtual const tagTableState * GetTableAttrib(WORD wTableID);

    //��������
protected:
	//���Ӳ���
	WORD TableHitTest(POINT MousePoint);
	//���Ӳ���
	WORD ChairHitTest(POINT MousePoint);
	//��ԴĿ¼
	void GetGameResDirectory(const tagGameKind * pGameKind, TCHAR szResDirectory[], WORD wBufferCount);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	afx_msg void OnPaint();
	//λ����Ϣ
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//������Ϣ
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar);
	//�����Ϣ
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnLButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnRButtonDown(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg void OnRButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	//�����Ϣ
	afx_msg BOOL OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

#endif