#ifndef IGAME_PLAZA_HEAD_FILE
#define IGAME_PLAZA_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

#define VER_IViewItem INTERFACE_VERSION(1,1)
static const GUID IID_IViewItem={0xb15641a1,0xb3b0,0x4d8c,0xb4,0xaf,0xa7,0x3a,0x5f,0xc,0xac,0x54};

//��ͼ��ӿ�
interface IViewItem : public IUnknownEx
{
	//��ȡ���
	virtual HWND __cdecl GetWindowHandle()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IPlazaViewItem INTERFACE_VERSION(1,1)
static const GUID IID_IPlazaViewItem={0x9538925b,0x19a8,0x433b,0xa4,0x2a,0x9d,0x23,0x5f,0x72,0x79,0x81};

//�㳡��ͼ��ӿ�
interface IPlazaViewItem : public IViewItem
{
	//��¼��Ϣ
	virtual bool __cdecl SendLogonMessage()=NULL;
	//������Ϣ
	virtual bool __cdecl SendConnectMessage()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IRoomViewItem INTERFACE_VERSION(2,1)
static const GUID IID_IRoomViewItem={0xa3af02ff,0x89f0,0x4b30,0x82,0x6,0xda,0x1d,0x19,0x11,0x58,0x29};

//������ͼ��ӿ�
interface IRoomViewItem : public IViewItem
{
	//�ر�ѯ��
	virtual bool __cdecl QueryCloseRoom()=NULL;
	//�رշ���
	virtual void __cdecl CloseRoomViewItem()=NULL;;
	//������Ϣ
	virtual tagGameKind * __cdecl GetKindInfo()=NULL;
	//������Ϣ
	virtual tagGameServer * __cdecl GetServerInfo()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IUserItem INTERFACE_VERSION(1,1)
static const GUID IID_IUserItem={0x2850679d,0xb76c,0x435d,0x81,0x3b,0xd3,0x8,0x9e,0x91,0x16,0x1f};

//�û���Ϣ
interface IUserItem : public IUnknownEx
{
	//����ӿ�
public:
	//�����ж�
	virtual bool __cdecl IsActive()=NULL;

	//���Խӿ�
public:
	//�û� I D
	virtual DWORD __cdecl GetUserID()=NULL;
	//�û�����
	virtual LPCTSTR __cdecl GetUserName()=NULL;
	//��Ϸ����
	virtual LONG __cdecl GetUserPlayCount()=NULL;
	//��ȡ�û�
	virtual tagUserData * __cdecl GetUserData()=NULL;

	//���ýӿ�
public:
	//���û���
	virtual void __cdecl SetUserScore(const tagUserScore * pUserScore)=NULL;
	//����״̬
	virtual void __cdecl SetUserStatus(const tagUserStatus * pUserStatus)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IScoreParser INTERFACE_VERSION(1,1)
static const GUID IID_IScoreParser={0xa7a3b158,0x7d57,0x4c67,0x8d,0x5b,0x0,0xab,0xe5,0x50,0x7f,0x42};

//��������
interface IScoreParser : public IUnknownEx
{
	//���÷�������
	virtual bool __cdecl SetScoreDescribe(WORD wDataDescribe[], WORD wDescribeCount)=NULL;
	//�����û�����
	virtual bool __cdecl UpdateUserScore(IUserItem * pIUserItem, BYTE cbScoreBuffer[], WORD wDataSize)=NULL;
	//�Ƿ���ڷ���
	virtual bool __cdecl ExistPoint()=NULL;
	//�Ƿ���ڽ��
	virtual bool __cdecl ExistGold()=NULL;
	//�Ƿ����ʤ��
	virtual bool __cdecl ExistWinCount()=NULL;
	//�Ƿ�������
	virtual bool __cdecl ExistLostCount()=NULL;
	//�Ƿ���ں;�
	virtual bool __cdecl ExistDrawCount()=NULL;
	//�Ƿ�����Ӿ�
	virtual bool __cdecl ExistFleeCount()=NULL;
	//�Ƿ���ھ���
	virtual bool __cdecl ExistExperience()=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IClientUserManager INTERFACE_VERSION(1,1)
static const GUID IID_IClientUserManager={0xe55db51d,0x46a8,0x443e,0xa8,0x93,0x28,0xdc,0xc4,0xad,0x52,0x20};

//�û�����
interface IClientUserManager : public IUnknownEx
{
	//����ӿ�
public:
	//���ûص��ӿ�
	virtual bool __cdecl SetUserManagerSink(IUnknownEx * pIUnknownEx)=NULL;
	//�����û�
	virtual IUserItem * __cdecl ActiveUserItem(tagUserData & UserData)=NULL;
	//ɾ���û�
	virtual bool __cdecl DeleteUserItem(IUserItem * pIUserItem)=NULL;
	//���»���
	virtual bool __cdecl UpdateUserItemScore(IUserItem * pIUserItem, const tagUserScore * pUserScore)=NULL;
	//����״̬
	virtual bool __cdecl UpdateUserItemStatus(IUserItem * pIUserItem, const tagUserStatus * pUserStatus)=NULL;

	//��Ϣ�ӿ�
public:
	//��ȡ����
	virtual DWORD __cdecl GetOnLineCount()=NULL;

	//���ҽӿ�
public:
	//ö���û�
	virtual IUserItem * __cdecl EnumUserItem(WORD wEnumIndex)=NULL;
	//�����û�
	virtual IUserItem * __cdecl SearchUserByUserID(DWORD dwUserID)=NULL;
	//�����û�
	virtual IUserItem * __cdecl SearchUserByGameID(DWORD dwUserID)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#define VER_IUserManagerSink INTERFACE_VERSION(1,1)
static const GUID IID_IUserManagerSink={0xd46b0de0,0xb1fc,0x4835,0x8e,0x85,0x37,0x9a,0x7e,0xf3,0xb3,0xa0};

//�û��ص�
interface IUserManagerSink : public IUnknownEx
{
	//�û�����
	virtual void __cdecl OnUserItemAcitve(IUserItem * pIUserItem)=NULL;
	//�û�����
	virtual void __cdecl OnUserItemUpdate(IUserItem * pIUserItem)=NULL;
	//�û�ɾ��
	virtual void __cdecl OnUserItemDelete(IUserItem * pIUserItem)=NULL;
};

//////////////////////////////////////////////////////////////////////////

#endif