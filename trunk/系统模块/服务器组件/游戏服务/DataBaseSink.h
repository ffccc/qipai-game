#ifndef DATA_BASE_SINK_HEAD_FILE
#define DATA_BASE_SINK_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameServiceExport.h"

//////////////////////////////////////////////////////////////////////////
//�ṹ����

//���ݿ����
struct tagDataBaseSinkParameter
{
	tagDataBaseInfo						* pGameUserDBInfo;				//������Ϣ
	tagDataBaseInfo						* pGameScoreDBInfo;				//������Ϣ
	tagGameServiceAttrib				* pGameServiceAttrib;			//��������
	tagGameServiceOption				* pGameServiceOption;			//������Ϣ
};

//////////////////////////////////////////////////////////////////////////
//���ݿ��������ݰ�

//���ݿ������ʶ
#define	DBR_GR_LOGON_BY_USERID			1								//I D ��¼
#define DBR_GR_WRITE_GAME_SCORE			2								//�뿪����
#define DBR_GR_LEAVE_GAME_SERVER		3								//�޸Ļ���
#define DBR_GR_GAME_SCORE_RECORD		4								//���ּ�¼
#define DBR_GR_LOAD_ANDROID				5								//�����û�
#define DBR_GR_LIMIT_ACCOUNTS			6								//�����ʻ�
#define DBR_GR_SET_USER_RIGHT			7								//����Ȩ��

#define DBR_GR_SEND_GIFT				8								//�����ʻ�

#define DBR_GR_BUY_PROPERTY				9								//������Ϣ
#define DBR_GR_LOAD_PROPERTY			10								//���ص���
#define DBR_GR_WRITE_PROPERTY			11								//д����
#define DBR_GR_EXCHANGE_CHARM			12								//�һ�����

#define DBR_GR_BANK_GET_GOLD			13								//��ȡ���
#define DBR_GR_BANK_STORAGE_GOLD		14								//�洢���

//#define DBR_GR_USER_SIGN_UP				12								//��ұ���

//���ݿ������ʶ
#define DBR_GR_LOGON_SUCCESS			100								//��¼�ɹ�
#define DBR_GR_LOGON_ERROR				101								//��¼ʧ��
#define DBR_GR_ANDROID_USER				102								//������Ϣ
#define DBR_GR_USER_RIGHT_RESULT		103								//Ȩ�޽��

#define DBR_GR_PROPERTY_ATTRIBUTE		106								//���سɹ�
#define DBR_GR_USER_PROPERTY			107								//��ҵ���
//#define DBR_GR_EXCHANGE_RESULT			108								//�һ����
#define DBR_GR_LOAD_PROP_FINISHI		109								//��ɼ���
#define DBR_GR_FLOWER_ATTRIBUTE			110								//��ɼ���
//#define DBR_GR_SEND_GIFT_RESULT			103								//���ͽ��

//�����ʻ�
struct DBR_GR_SendGift
{
	DWORD								dwSendUserID;					//������ID
	DWORD								dwRcvUserID;					//������ID
	WORD								wGiftID;						//����	ID
	DWORD								dwClientIP;						//��ҵ�ַ
	WORD								wFlowerCount;					//�ʻ���Ŀ
	LONG								lSendUserCharm;					//��������
	LONG								lRcvUserCharm;					//��������
	LONG								lFlowerPrice;					//���͸���
};

//���߽ṹ
struct DBR_GR_Property
{
	DWORD								dwUserID;						//���ID
	DWORD								dwTargetUserID;					//Ŀ�����
	int									nPropertyID;					//����ID
	DWORD								dwCurCount;						//��ǰ��Ŀ
	DWORD								dwPachurseCount;				//������Ŀ
	DWORD								dwOnceCount;					//������Ŀ
	LONG								lPrice;							//���߼۸�
	DWORD								dwClientIP;						//���IP
};

//���߼�¼
struct DBR_GR_WriteProperty
{
	DWORD								dwUserID;						//���ID
	int									nPropertyID;					//����ID
	DWORD								lUseableTime;					//����ʱ��
	DWORD								dwClientIP;						//���ӵ�ַ
};

//���ͽ��
struct DBR_GR_SendGiftResult
{
	DWORD								dwSendUserID;					//������ID
	DWORD								dwRcvUserID;					//������ID
	WORD								wGiftID;						//����	ID
	bool								bSuccess;						//�ɹ���ʶ
};

//��ȡ���
struct DBR_GR_BankDrawoutGold
{
	DWORD								DrawoutGoldCount;						//��ȡ��Ŀ
	DWORD								dwUserID;						//���ID
	DWORD								dwClientIP;						//���IP
};

//�洢���
struct DBR_GR_BankStorage
{
	DWORD								lStorageCount;					//�洢��Ŀ
	DWORD								dwUserID;						//���ID
	DWORD								dwClientIP;						//���IP
};

//ID ��¼
struct DBR_GR_LogonByUserID
{
	DWORD								dwUserID;						//�û� I D
	DWORD								dwClientIP;						//���ӵ�ַ
	TCHAR								szPassWord[PASS_LEN];			//��¼����
	TCHAR								szComputerID[COMPUTER_ID_LEN];	//��������
};

//�����ʻ�
struct DBR_GR_LimitAccounts
{
	DWORD								dwUserID;						//�û� I D
	DWORD								dwMasterUserID;					//���� I D
	DWORD								dwMasterClientIP;				//���ӵ�ַ
};

//����Ȩ��
struct DBR_GR_SetUserRight
{
	//Ȩ�޷�Χ
	BYTE								cbGame;							//������Ϸ
	BYTE								cbAccounts;						//�����ʺ�

	//Ȩ����Ϣ
	DWORD								dwUserID;						//�û� I D
	DWORD								dwUserRight;					//�û�Ȩ��
	DWORD								dwMasterUserID;					//���� I D
	DWORD								dwMasterClientIP;				//���ӵ�ַ
};

//Ȩ�޽��
struct DBR_GR_UserRightResult
{
	//Ȩ�޷�Χ
	bool								bGameSuccess;					//�ɹ���ʶ
	bool								bAccountsSuccess;				//�ɹ���ʶ

	//Ȩ����Ϣ
	DWORD								dwUserID;						//�û� I D
	DWORD								dwMasterUserID;					//���� I D
	DWORD								dwUserRight;					//�û�Ȩ��

};

//�����û�
struct DBO_GR_AndroidUser
{
	LONG								lResultCode;					//�������
	WORD								wAndroidCount;					//�û���Ŀ
	DWORD								dwAndroidID[MAX_ANDROID];		//�û� I D
};

//��¼�ɹ�
struct DBR_GR_LogonSuccess
{
	//������Ϣ
	WORD								wFaceID;						//ͷ������
	DWORD								dwCustomFaceVer;				//�Զ�ͷ��
	DWORD								dwUserID;						//�û� I D
	DWORD								dwGameID;						//�û� I D
	DWORD								dwGroupID;						//��������
	DWORD								dwUserRight;					//�û��ȼ�
	LONG								lLoveliness;					//�û�����
	DWORD								dwMasterRight;					//����Ȩ��
	TCHAR								szAccounts[NAME_LEN];			//��¼�ʺ�
	TCHAR								szPassWord[PASS_LEN];			//��¼����
	TCHAR								szGroupName[GROUP_LEN];			//��������
	TCHAR								szUnderWrite[UNDER_WRITE_LEN];	//����ǩ��

	//�û�����
	BYTE								cbGender;						//�û��Ա�
	BYTE								cbMemberOrder;					//��Ա�ȼ�
	BYTE								cbMasterOrder;					//����ȼ�

	//������Ϣ
	LONG								lScore;							//�û�����
	LONG								lInsureScore;					//�洢���
	LONG								lGameGold;						//��Ϸ���
	LONG								lWinCount;						//ʤ������
	LONG								lLostCount;						//ʧ������
	LONG								lDrawCount;						//�;�����
	LONG								lFleeCount;						//������Ŀ
	LONG								lExperience;					//�û�����
};

//��¼ʧ��
struct DBR_GR_LogonError
{
	LONG								lErrorCode;						//�������
	TCHAR								szErrorDescribe[128];			//������Ϣ
};

//�޸Ļ���
struct DBR_GR_WriteUserScore
{
	//�û�����
	DWORD								dwUserID;						//�û���ַ
	DWORD								dwClientIP;						//���ӵ�ַ
	DWORD								dwPlayTimeCount;				//��Ϸʱ��
	DWORD								dwOnlineTimeCount;				//����ʱ��

	//������Ϣ
	LONG								lRevenue;						//��Ϸ˰��
	tagUserScore						ScoreModifyInfo;				//�޸Ļ���
};

//�뿪����
struct DBR_GR_LeaveGameServer
{
	//�û�����
	DWORD								dwUserID;						//�û���ַ
	DWORD								dwClientIP;						//���ӵ�ַ
	DWORD								dwPlayTimeCount;				//��Ϸʱ��
	DWORD								dwOnlineTimeCount;				//����ʱ��
	LONG								lLoveliness;					//�û�����

	//������Ϣ
	LONG								lRevenue;						//��Ϸ˰��
	tagUserScore						ScoreModifyInfo;				//�޸Ļ���
};

//��������
struct DBR_GR_PropertyAttribute
{
	WORD						wPropertyID;						//����ID
	DWORD						dwPropCount1;						//������Ŀ
	DWORD						dwPropCount2;						//������Ŀ
	DWORD						dwPropCount3;						//������Ŀ
	DWORD						dwPropCount4;						//������Ŀ
	DWORD						dwPropCount5;						//������Ŀ
	DWORD						dwPropCount6;						//������Ŀ
	DWORD						dwPropCount7;						//������Ŀ
	DWORD						dwPropCount8;						//������Ŀ
	DWORD						dwPropCount9;						//������Ŀ
	DWORD						dwPropCount10;						//������Ŀ
	LONG						lPrice1;							//���߼۸�
	LONG						lPrice2;							//���߼۸�
	LONG						lPrice3;							//���߼۸�
	LONG						lPrice4;							//���߼۸�
	LONG						lPrice5;							//���߼۸�
	LONG						lPrice6;							//���߼۸�
	LONG						lPrice7;							//���߼۸�
	LONG						lPrice8;							//���߼۸�
	LONG						lPrice9;							//���߼۸�
	LONG						lPrice10;							//���߼۸�
	BYTE						cbDiscount;							//��Ա�ۿ�
};

//�ʻ�����
struct DBR_GR_FlowerAttribute
{
	int							nFlowerID;							//�ʻ�ID
	LONG						lPrice;								//�ʻ��۸�
	LONG						lSendUserCharm;						//��������
	LONG						lRcvUserCharm;						//��������
	BYTE						cbDiscount;							//��Ա�ۿ�
};

//�һ��ṹ
struct DBR_GR_ExchangeLoveliness
{
	DWORD								dwUserID;						//���ID
	LONG								lLoveliness;					//������ֵ
	DWORD								lGoldValue;						//�����ֵ
	DWORD								dwClientIP;						//���IP
};

////�һ����
//struct DBR_GR_ExchangeCharmResult
//{
//	BYTE								cbResultCode;					//�������
//	DWORD								dwUserID;						//���ID
//	LONG								lLoveliness;					//������ֵ
//	DWORD								lGoldValue;						//�����ֵ
//};

//��ҵ���
struct DBR_GR_UserProperty
{
	DWORD								dwUserID;						//���ID
	INT									nPropertyID;					//����ID
	DWORD								dwCurCount;						//��ǰ��Ŀ
};

//////////////////////////////////////////////////////////////////////////

//���ݿ����湳��
class CDataBaseSink : public IDataBaseEngineSink
{
	//��Ԫ����
	friend class CGameService;

	//���ñ���
protected:
	tagDataBaseInfo						* m_pGameUserDBInfo;			//������Ϣ
	tagDataBaseInfo						* m_pGameScoreDBInfo;			//������Ϣ
	tagGameServiceAttrib				* m_pGameServiceAttrib;			//��������
	tagGameServiceOption				* m_pGameServiceOption;			//������Ϣ
	IDataBaseEngineEvent *				m_pIDataBaseEngineEvent;		//�����¼�

	//�������
protected:
	CDataBaseAide						m_AccountsDBAide;				//�û����ݿ�
	CDataBaseAide						m_GameScoreDBAide;				//�������ݿ�
	CDataBaseHelper						m_AccountsDBModule;				//�û����ݿ�
	CDataBaseHelper						m_GameScoreDBModule;			//�������ݿ�

	//��������
public:
	//���캯��
	CDataBaseSink();
	//��������
	virtual ~CDataBaseSink();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual void __cdecl Release() { delete this; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//ϵͳ�¼�
public:
	//�����¼�
	virtual bool __cdecl OnDataBaseEngineStart(IUnknownEx * pIUnknownEx);
	//ֹͣ�¼�
	virtual bool __cdecl OnDataBaseEngineStop(IUnknownEx * pIUnknownEx);

	//�ں��¼�
public:
	//ʱ���¼�
	virtual bool __cdecl OnDataBaseEngineTimer(DWORD dwTimerID, WPARAM dwBindParameter) { return true; }
	//�����¼�
	virtual bool __cdecl OnDataBaseEngineControl(WORD wControlID, VOID * pData, WORD wDataSize) { return true; }
	//�����¼�
	virtual bool __cdecl OnDataBaseEngineRequest(WORD wRequestID, DWORD dwContextID, VOID * pData, WORD wDataSize);

	//������
private:
	//��¼����
	bool OnRequestLogon(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//д������
	bool OnRequestWriteUserScore(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�뿪����
	bool OnRequestLeaveGameServer(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ʻ�
	bool OnRequestLimitAccounts(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//����Ȩ��
	bool OnRequestSetUserRight(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����û�
	bool OnRequestLoadAndroid(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����ʻ�
	bool OnRequestSendGift(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//������Ϣ
	bool OnRequestProperty(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//д�����
	bool OnWriteProperty(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//���ص���
	bool OnLoadProperty(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�����һ�
	bool OnExchangeCharm(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//��ȡ���
	bool OnBankDrawoutGold(DWORD dwContextID, VOID * pData, WORD wDataSize);
	//�洢���
	bool OnBankStorageGold(DWORD dwContextID, VOID * pData, WORD wDataSize);

	//�洢����
protected:
	//I D �洢����
	LONG SPLogonByUserID(DWORD dwUserID, LPCTSTR pszPassword, DWORD dwClientIP, LPCTSTR pszComputerID);
	//д�ִ洢����
	LONG SPWriteUserScore(DWORD dwUserID, DWORD dwPlayTimeCount, DWORD dwOnLineTimeCount, DWORD dwClientIP, LONG lRevenue, tagUserScore & UserScore);
	//�뿪�洢����
	LONG SPLeaveGameServer(DWORD dwUserID, DWORD dwPlayTimeCount, DWORD dwOnLineTimeCount, DWORD dwClientIP, LONG lRevenue, LONG lLoveliness, tagUserScore & UserScore);
	//���Ŵ洢����
	LONG SPCongealAccounts(DWORD dwUserID, DWORD dwMasterUserID, DWORD dwClientIP);
	//Ȩ�޴洢����
	LONG SPSetUserGameRight(DWORD dwUserID, DWORD dwUserRight, DWORD dwMasterUserID, DWORD dwClientIP);
	//Ȩ�޴洢����
	LONG SPSetUserAccountsRight(DWORD dwUserID, DWORD dwUserRight, DWORD dwMasterUserID, DWORD dwClientIP);
	//��¼�洢����
	LONG SPRecordGiftGrant(DWORD dwSendUserID, DWORD dwRcvUserID, WORD wGiftID, DWORD dwClientIP, DWORD dwSendUserLoveliness, DWORD dwRcvUserLoveliness, LONG lValue, int nFlowerCount);
	//����洢����
	LONG SPBuyProperty(DWORD dwUserID, DWORD dwTargetUserID, int nPropertyID, DWORD dwCurCount, DWORD dwOnceCount, DWORD dwPachurCount, LONG lPrice, DWORD dwClientIP);
	//д��洢����
	LONG SPWriteProperty(DWORD dwUserID, int nPropertyID, DWORD dwUseableTime, DWORD dwClientIP);
	//���ش洢����
	LONG SPLoadPropertyAttribute();
	//���ش洢����
	LONG SPLoadFlowerAttribute();
	//���ش洢����
	LONG SPLoadUserProperty(DWORD dwUserID);
	//�һ��洢����
	LONG SPExchangeCharm(DWORD dwUserID, LONG lLoveliness, LONG lGoldValue, DWORD dwClientIP);
	//��ȡ�洢����
	LONG SPBankDrawoutGold(DWORD dwUserID, DWORD lSwapScore, DWORD dwClientIP);
	//��ȡ�洢����
	LONG SPBankStorageGold(DWORD dwUserID, DWORD lSwapScore, DWORD dwClientIP);
};

//////////////////////////////////////////////////////////////////////////

#endif