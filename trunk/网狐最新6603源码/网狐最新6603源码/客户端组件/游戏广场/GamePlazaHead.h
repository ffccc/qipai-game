#ifndef GAME_PLAZA_HEAD_HEAD_FILE
#define GAME_PLAZA_HEAD_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//����˵��

//��˵��
class CTableResource;

//�ӿ�˵��
interface ITableView;
interface ITableViewFrame;
interface IServerViewItem;

//�ṹ˵��
struct tagTableMirrorAttrib;

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IServerViewItem INTERFACE_VERSION(1,1)
	static const GUID IID_IServerViewItem={0xe1a242f0,0xed48,0x47c0,0xa4,0xa9,0x0b,0x6f,0x90,0x2f,0x12,0xbd};
#else
	#define VER_IServerViewItem INTERFACE_VERSION(1,1)
	static const GUID IID_IServerViewItem={0x404c4520,0x6f4e,0x44b0,0x8d,0xc7,0xe5,0x8e,0x78,0x72,0x31,0x89};
#endif

//����ӿ�
interface IServerViewItem : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IProcessManager INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessManager={0xf5a2e8a6,0xdb72,0x4a54,0xa0,0xb9,0xf1,0x6f,0x82,0xb9,0xa8,0x53};
#else
	#define VER_IProcessManager INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessManager={0x68b8eac6,0x366f,0x4c5c,0xb8,0xd1,0xf4,0x7e,0x13,0xf3,0x55,0x88};
#endif

//���̹���
interface IProcessManager : public IUnknownEx
{
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IProcessManagerSink INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessManagerSink={0x61ec395e,0xcb90,0x4965,0xb5,0x81,0x5b,0x22,0x49,0x8a,0xdd,0x6e};
#else
	#define VER_IProcessManagerSink INTERFACE_VERSION(1,1)
	static const GUID IID_IProcessManagerSink={0xf3a80930,0x377c,0x40ad,0xa1,0x65,0xf9,0x02,0x76,0x09,0x49,0x85};
#endif

//��Ϸ����
interface IProcessManagerSink : public IUnknownEx
{
	//���̴���
	virtual bool OnGameProcessError()=NULL;
	//���̹ر�
	virtual bool OnGameProcessClose(DWORD dwExitCode)=NULL;
	//��������
	virtual bool OnGameProcessCreate(bool bAlreadyExist)=NULL;
	//��������
	virtual bool OnProcessManagerData(WORD wMainCmdID, WORD wSubCmdID, VOID * pData, WORD wDataSize)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableView INTERFACE_VERSION(1,1)
	static const GUID IID_ITableView={0xa12ff990,0xa860,0x4406,0x94,0x6b,0x44,0x59,0xbe,0xa2,0x6f,0x82};
#else
	#define VER_ITableView INTERFACE_VERSION(1,1)
	static const GUID IID_ITableView={0xdcd15997,0xcbd4,0x4f9b,0xa5,0xe0,0x54,0x63,0x9c,0xb3,0xe5,0x47};
#endif

//������ͼ
interface ITableView : public IUnknownEx
{
	//���ܽӿ�
public:
	//��������
	virtual WORD GetNullChairCount(WORD & wNullChairID)=NULL;
	//���ú���
	virtual VOID InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableFrameView)=NULL;

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wChairID)=NULL;
	//������Ϣ
	virtual bool SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem)=NULL;

	//���ýӿ�
public:
	//��갴��
	virtual VOID SetMouseDown(bool bMouseDown)=NULL;
	//������
	virtual VOID SetFocusFrame(bool bFocusFrame)=NULL;
	//����λ��
	virtual VOID SetHoverStatus(WORD wHoverChairID)=NULL;
	//����״̬ 
	virtual VOID SetTableStatus(bool bPlaying, bool bLocker)=NULL;

	//��ѯ�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag()=NULL;
	//�����־
	virtual bool GetLockerFlag()=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableViewFrame INTERFACE_VERSION(1,1)
	static const GUID IID_ITableViewFrame={0xe63e8758,0x96b0,0x4ab3,0x92,0xfc,0x3a,0x29,0x82,0x18,0x84,0xf8};
#else
	#define VER_ITableViewFrame INTERFACE_VERSION(1,1)
	static const GUID IID_ITableViewFrame={0x067aff04,0x7527,0x41da,0x9b,0x83,0x0e,0x6f,0x41,0x8d,0xcd,0x87};
#endif

//���ӿ��
interface ITableViewFrame : public IUnknownEx
{
	//���ýӿ�
public:
	//��������
	virtual bool CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx)=NULL;
	//��������
	virtual bool ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, LPCTSTR pszResDirectory)=NULL;

	//��Ϣ�ӿ�
public:
	//������Ŀ
	virtual WORD GetTableCount()=NULL;
	//������Ŀ
	virtual WORD GetChairCount()=NULL;

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wTableID, WORD wChairID)=NULL;
	//������Ϣ
	virtual bool SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem)=NULL;

	//״̬�ӿ�
public:
	//��Ϸ��־
	virtual bool GetPlayFlag(WORD wTableID)=NULL;
	//�����־
	virtual bool GetLockerFlag(WORD wTableID)=NULL;
	//������
	virtual VOID SetFocusFrame(WORD wTableID, bool bFocusFrame)=NULL;
	//����״̬ 
	virtual VOID SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker)=NULL;

	//��ͼ����
public:
	//���ӿ���
	virtual bool VisibleTable(WORD wTableID)=NULL;
	//��������
	virtual bool FlashGameTable(WORD wTableID)=NULL;
	//��������
	virtual bool FlashGameChair(WORD wTableID, WORD wChairID)=NULL;

	//���ܽӿ�
public:
	//��������
	virtual bool UpdateTableView(WORD wTableID)=NULL;
	//��ȡ����
	virtual ITableView * GetTableViewItem(WORD wTableID)=NULL;
	//��������
	virtual WORD GetNullChairCount(WORD wTableID, WORD & wNullChairID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_ITableViewFrameSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITableViewFrameSink={0xb0b88935,0xc7fd,0x42ff,0x90,0x62,0x2a,0x2d,0xa9,0x35,0x5b,0xc8};
#else
	#define VER_ITableViewFrameSink INTERFACE_VERSION(1,1)
	static const GUID IID_ITableViewFrameSink={0xf52f47d4,0xeabf,0x49b5,0xa3,0x0c,0x37,0x61,0x3f,0x81,0x21,0x3c};
#endif

//����֪ͨ
interface ITableViewFrameSink : public IUnknownEx
{
	//���˫��
	virtual VOID OnDButtonHitTable(WORD wTableID)=NULL;
	//�������
	virtual VOID OnLButtonHitTable(WORD wTableID, WORD wChairID)=NULL;
	//�Ҽ�����
	virtual VOID OnRButtonHitTable(WORD wTableID, WORD wChairID)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#endif