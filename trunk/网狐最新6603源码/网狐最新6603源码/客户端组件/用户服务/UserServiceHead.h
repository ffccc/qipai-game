#ifndef USER_SERVICE_HEAD_FILE
#define USER_SERVICE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//MFC �ļ�
#include <Nb30.h>
#include <Afxmt.h>
#include <AtlBase.h>

//ƽ̨�ļ�
#include "..\..\ȫ�ֶ���\Platform.h"

//����ļ�
#include "..\..\�������\�������\ServiceCoreHead.h"

//////////////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef USER_SERVICE_CLASS
	#ifdef  USER_SERVICE_DLL
		#define USER_SERVICE_CLASS _declspec(dllexport)
	#else
		#define USER_SERVICE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define USER_SERVICE_DLL_NAME	TEXT("UserService.dll")				//�������
#else
	#define USER_SERVICE_DLL_NAME	TEXT("UserServiceD.dll")			//�������
#endif

//////////////////////////////////////////////////////////////////////////////////

//��Ƭ����
#define NAME_CARD_CY				80									//��Ƭ�߶�
#define NAME_CARD_CX				260									//��Ƭ���

//��ϵ��Ϣ
struct tagCompanionInfo
{
	DWORD							dwUserID;							//�û���ʶ
	DWORD							dwGameID;							//��Ϸ��ʶ
	BYTE							cbCompanion;						//�û���ϵ
	TCHAR							szUserNote[LEN_USERNOTE];			//�û���ע
	TCHAR							szNickName[LEN_NICKNAME];			//�û��ǳ�
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IClientUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IClientUserItem={0x7c3908c4,0xa275,0x4abb,0x008b,0x78,0x09,0x4d,0xc9,0xf1,0x29,0x56};
#else
	#define VER_IClientUserItem INTERFACE_VERSION(1,1)
	static const GUID IID_IClientUserItem={0x22dfa4e3,0xa6b9,0x4716,0x0086,0x46,0xf8,0xb1,0x3b,0x51,0x2d,0x7b};
#endif

//�û��ӿ�
interface IClientUserItem : public IUnknownEx
{
	//���Խӿ�
public:
	//��Ҫ�ȼ�
	virtual LONG GetImportOrder()=NULL;
	//�û���Ϣ
	virtual tagUserInfo * GetUserInfo()=NULL;
	//�Զ�ͷ��
	virtual tagCustomFaceInfo * GetCustomFaceInfo()=NULL;

	//ͷ����Ϣ
public:
	//ͷ������
	virtual WORD GetFaceID()=NULL;
	//�Զ�����
	virtual DWORD GetCustomID()=NULL;

	//������Ϣ
public:
	//�û��Ա�
	virtual BYTE GetGender()=NULL;
	//�û���ʶ
	virtual DWORD GetUserID()=NULL;
	//��Ϸ��ʶ
	virtual DWORD GetGameID()=NULL;
	//���ű�ʶ
	virtual DWORD GetGroupID()=NULL;
	//�û��ǳ�
	virtual LPCTSTR GetNickName()=NULL;
	//��������
	virtual LPCTSTR GetGroupName()=NULL;
	//����ǩ��
	virtual LPCTSTR GetUnderWrite()=NULL;

	//��������
public:
	//�û�����
	virtual DWORD GetUserMedal()=NULL;
	//������ֵ
	virtual DWORD GetUserExperience()=NULL;
	//������ֵ
	virtual DWORD GetUserLoveLiness()=NULL;

	//�ȼ���Ϣ
public:
	//��Ա�ȼ�
	virtual BYTE GetMemberOrder()=NULL;
	//����ȼ�
	virtual BYTE GetMasterOrder()=NULL;

	//�û�״̬
public:
	//�û�����
	virtual WORD GetTableID()=NULL;
	//�û�����
	virtual WORD GetChairID()=NULL;
	//�û�״̬
	virtual BYTE GetUserStatus()=NULL;

	//��Ϸ��Ϣ
public:
	//������ֵ
	virtual SCORE GetUserScore()=NULL;
	//�ɼ���ֵ
	virtual SCORE GetUserGrade()=NULL;
	//������ֵ
	virtual SCORE GetUserInsure()=NULL;

	//��Ϸ��Ϣ
public:
	//ʤ������
	virtual DWORD GetUserWinCount()=NULL;
	//ʧ������
	virtual DWORD GetUserLostCount()=NULL;
	//�;�����
	virtual DWORD GetUserDrawCount()=NULL;
	//��������
	virtual DWORD GetUserFleeCount()=NULL;
	//��Ϸ����
	virtual DWORD GetUserPlayCount()=NULL;

	//������Ϣ
public:
	//�û�ʤ��
	virtual float GetUserWinRate()=NULL;
	//�û�����
	virtual float GetUserLostRate()=NULL;
	//�û�����
	virtual float GetUserDrawRate()=NULL;
	//�û�����
	virtual float GetUserFleeRate()=NULL;

	//�û���ϵ
public:
	//��ȡ��ϵ
	virtual BYTE GetUserCompanion()=NULL;
	//���ù�ϵ
	virtual VOID SetUserCompanion(BYTE cbCompanion)=NULL;

	//�û���ע
public:
	//��ȡ��ע
	virtual LPCTSTR GetUserNoteInfo()=NULL;
	//���ñ�ע
	virtual VOID SetUserNoteInfo(LPCTSTR pszUserNote)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUserManagerSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUserManagerSink={0x28e4435e,0xdcb8,0x4ad1,0x008a,0x58,0x60,0xe8,0xc4,0xd4,0x94,0xc7};
#else
	#define VER_IUserManagerSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUserManagerSink={0x6b48deaf,0x28f0,0x4cd4,0x00bf,0x08,0x0d,0x99,0x93,0x4e,0x5d,0x37};
#endif

//����֪ͨ
interface IUserManagerSink : public IUnknownEx
{
	//�û�����
	virtual VOID OnUserItemAcitve(IClientUserItem * pIClientUserItem)=NULL;
	//�û�ɾ��
	virtual VOID OnUserItemDelete(IClientUserItem * pIClientUserItem)=NULL;
	//�û�����
	virtual VOID OnUserFaceUpdate(IClientUserItem * pIClientUserItem)=NULL;
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserScore & LastScore)=NULL;
	//�û�����
	virtual VOID OnUserItemUpdate(IClientUserItem * pIClientUserItem, tagUserStatus & LastStatus)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IPlazaUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IPlazaUserManager={0xa2992ab9,0x2fdd,0x4f9f,0x00bb,0xac,0xcf,0x8b,0x60,0x70,0x80,0xcd};
#else
	#define VER_IPlazaUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IPlazaUserManager={0xe733fb10,0x4b3c,0x442e,0x008c,0xc2,0x37,0x42,0x8b,0x67,0x35,0x0b};
#endif

//�û�����
interface IPlazaUserManager : public IUnknownEx
{
	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetUserInformation(IUnknownEx * pIUnknownEx)=NULL;
	//���ýӿ�
	virtual bool SetUserManagerSink(IUnknownEx * pIUnknownEx)=NULL;

	//����ӿ�
public:
	//ɾ���û�
	virtual bool DeleteUserItem(IClientUserItem * pIClientUserItem)=NULL;
	//�����û�
	virtual IClientUserItem * ActiveUserItem(tagUserInfo & UserInfo, tagCustomFaceInfo & CustomFaceInfo)=NULL;

	//���½ӿ�
public:
	//���»���
	virtual bool UpdateUserItemScore(IClientUserItem * pIClientUserItem, tagUserScore * const pUserScore)=NULL;
	//����״̬
	virtual bool UpdateUserItemStatus(IClientUserItem * pIClientUserItem, tagUserStatus * const pUserStatus)=NULL;
	//����ͷ��
	virtual bool UpdateUserCustomFace(IClientUserItem * pIClientUserItem, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)=NULL;

	//���ҽӿ�
public:
	//ö���û�
	virtual IClientUserItem * EnumUserItem(WORD wEnumIndex)=NULL;
	//�����û�
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID)=NULL;
	//�����û�
	virtual IClientUserItem * SearchUserByGameID(DWORD dwGameID)=NULL;
	//�����û�
	virtual IClientUserItem * SearchUserByNickName(LPCTSTR pszNickName)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IGameUserManager={0xb0e334b9,0xf693,0x4c5f,0x0087,0x0f,0x66,0x77,0x0a,0xb9,0x93,0xb2};
#else
	#define VER_IGameUserManager INTERFACE_VERSION(1,1)
	static const GUID IID_IGameUserManager={0x2e5d87ff,0x3150,0x4897,0x008c,0x1f,0xbb,0x3d,0x29,0xfc,0x51,0x2e};
#endif

//�û�����
interface IGameUserManager : public IUnknownEx
{
	//���ýӿ�
public:
	//���ýӿ�
	virtual bool SetUserManagerSink(IUnknownEx * pIUnknownEx)=NULL;
	//��ȡ�ӿ�
	virtual VOID * GetUserManagerSink(REFGUID Guid, DWORD dwQueryVer)=NULL;

	//����ӿ�
public:
	//�����û�
	virtual bool ResetUserItem()=NULL;
	//ɾ���û�
	virtual bool DeleteUserItem(IClientUserItem * pIClientUserItem)=NULL;
	//�����û�
	virtual IClientUserItem * ActiveUserItem(tagUserInfo & UserInfo, tagCustomFaceInfo & CustomFaceInfo, BYTE cbCompanion, LPCTSTR pszUserNote)=NULL;

	//���½ӿ�
public:
	//���»���
	virtual bool UpdateUserItemScore(IClientUserItem * pIClientUserItem, tagUserScore * const pUserScore)=NULL;
	//����״̬
	virtual bool UpdateUserItemStatus(IClientUserItem * pIClientUserItem, tagUserStatus * const pUserStatus)=NULL;
	//����ͷ��
	virtual bool UpdateUserCustomFace(IClientUserItem * pIClientUserItem, DWORD dwCustomID, tagCustomFaceInfo & CustomFaceInfo)=NULL;

	//��ѯ�ӿ�
public:
	//��Ϸ�û�
	virtual IClientUserItem * GetTableUserItem(WORD wChariID)=NULL;
	//�Թ��û�
	virtual IClientUserItem * EnumLookonUserItem(WORD wEnumIndex)=NULL;

	//���ҽӿ�
public:
	//�����û�
	virtual IClientUserItem * SearchUserByUserID(DWORD dwUserID)=NULL;
	//�����û�
	virtual IClientUserItem * SearchUserByGameID(DWORD dwGameID)=NULL;
	//�����û�
	virtual IClientUserItem * SearchUserByNickName(LPCTSTR pszNickName)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IGameLevelParser INTERFACE_VERSION(1,1)
	static const GUID IID_IGameLevelParser={0x12823198,0x27b6,0x46c5,0x009c,0x2c,0x9c,0x37,0x08,0x58,0xbf,0xfa};
#else
	#define VER_IGameLevelParser INTERFACE_VERSION(1,1)
	static const GUID IID_IGameLevelParser={0x05d71a6a,0x4378,0x4213,0x00a1,0x51,0x0f,0x96,0x85,0x22,0x71,0x58};
#endif

//�ȼ�����
interface IGameLevelParser : public IUnknownEx
{
	//���ýӿ�
public:
	//��Ϸ����
	virtual LPCTSTR GetKindName()=NULL;
	//��Ϸ����
	virtual VOID SetKindName(LPCTSTR pszKindName)=NULL;

	//�����ӿ�
public:
	//�ȼ�����
	virtual LPCTSTR GetLevelDescribe(SCORE lScore)=NULL;
	//�ȼ�����
	virtual LPCTSTR GetLevelDescribe(IClientUserItem * pIClientUserItem)=NULL;

	//���ݽӿ�
public:
	//��ȡ�ȼ�
	virtual WORD GetGameLevelItem(tagLevelItem LevelItem[], WORD wMaxCount)=NULL;
	//���õȼ�
	virtual bool SetGameLevelItem(tagLevelItem LevelItem[], WORD wItemCount)=NULL;
	//���صȼ�
	virtual bool LoadGameLevelItem(LPCTSTR pszKindName, LPCTSTR pszDirectory, WORD wGameGenre)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUserOrderParser INTERFACE_VERSION(1,1)
	static const GUID IID_IUserOrderParser={0xeff89495,0xe9da,0x4ea3,0x83,0x64,0xaa,0xb2,0xaf,0x7f,0x03,0xce};
#else
	#define VER_IUserOrderParser INTERFACE_VERSION(1,1)
	static const GUID IID_IUserOrderParser={0xac3b2420,0x672a,0x4b8d,0xb4,0xd6,0x9e,0xc7,0xbd,0xb8,0x9d,0x1a};
#endif

//�ȼ��ӿ�
interface IUserOrderParser : public IUnknownEx
{
	//���ýӿ�
public:
	//������Ŀ
	virtual BYTE GetMemberParserItemCount()=NULL;
	//������Ŀ
	virtual BYTE GetMasterParserItemCount()=NULL;
	//���ý���
	virtual VOID SetMemberParser(tagMemberItem MemberItem[], WORD wItemCount)=NULL;
	//���ý���
	virtual VOID SetMasterParser(tagMasterItem MasterItem[], WORD wItemCount)=NULL;

	//���ܽӿ�
public:
	//��ȡ����
	virtual LPCTSTR GetMemberDescribe(BYTE cbMemberOrder)=NULL;
	//��ȡ����
	virtual LPCTSTR GetMasterDescribe(BYTE cbMasterOrder)=NULL;
	//��ȡ����
	virtual BYTE GetMemberParser(tagMemberItem MemberItem[], WORD wItemCount)=NULL;
	//��ȡ����
	virtual BYTE GetMasterParser(tagMasterItem MasterItem[], WORD wItemCount)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUserInformation INTERFACE_VERSION(1,1)
	static const GUID IID_IUserInformation={0xb3bcac27,0x7e21,0x479a,0x00b7,0x49,0xa3,0xdc,0x79,0xfe,0x8d,0xd1};
#else
	#define VER_IUserInformation INTERFACE_VERSION(1,1)
	static const GUID IID_IUserInformation={0x77084681,0xf172,0x4352,0x00a3,0x27,0x11,0x22,0x17,0x03,0x3c,0x47};
#endif

//�û���Ϣ
interface IUserInformation : public IUnknownEx
{
	//����ӿ�
public:
	//������Ϣ
	virtual bool LoadInformation()=NULL;
	//���ýӿ�
	virtual bool SetUserCompanionSink(IUnknownEx * pIUnknownEx)=NULL;

	//��ϵ�ӿ�
public:
	//ö�ٹ�ϵ
	virtual tagCompanionInfo * EmunCompanionInfo(WORD wIndex)=NULL;
	//Ѱ�ҹ�ϵ
	virtual tagCompanionInfo * SearchCompanionInfo(DWORD dwUserID)=NULL;
	//�����ϵ
	virtual tagCompanionInfo * InsertCompanionInfo(IClientUserItem * pIClientUserItem, BYTE cbCompanion)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
	#define VER_IUserCompanionSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUserCompanionSink={0xd145ece3,0xe8be,0x4178,0x0083,0xa6,0x8f,0x5b,0x12,0x33,0x4c,0x82};
#else
	#define VER_IUserCompanionSink INTERFACE_VERSION(1,1)
	static const GUID IID_IUserCompanionSink={0xd687ffa9,0x21ca,0x47ec,0x008a,0x99,0x7d,0xb9,0xec,0xf0,0x6b,0x96};
#endif

//��ϵ�ص�
interface IUserCompanionSink : public IUnknownEx
{
	//����֪ͨ
	virtual bool OnCompanionInsert(tagCompanionInfo * pCompanionInfo)=NULL;
	//ɾ��֪ͨ
	virtual bool OnCompanionDelete(tagCompanionInfo * pCompanionInfo)=NULL;
	//����֪ͨ
	virtual bool OnCompanionUpdate(tagCompanionInfo * pCompanionInfo)=NULL;
};

//////////////////////////////////////////////////////////////////////////////////
//���������

DECLARE_MODULE_HELPER(UserOrderParser,USER_SERVICE_DLL_NAME,"CreateUserOrderParser")
DECLARE_MODULE_HELPER(GameLevelParser,USER_SERVICE_DLL_NAME,"CreateGameLevelParser")
DECLARE_MODULE_HELPER(GameUserManager,USER_SERVICE_DLL_NAME,"CreateGameUserManager")
DECLARE_MODULE_HELPER(PlazaUserManager,USER_SERVICE_DLL_NAME,"CreatePlazaUserManager")

//////////////////////////////////////////////////////////////////////////////////

#endif