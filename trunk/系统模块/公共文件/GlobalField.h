#ifndef GLOBAL_FIELD_HEAD_FILE
#define GLOBAL_FIELD_HEAD_FILE

#include "GlobalDef.h"

//////////////////////////////////////////////////////////////////////////

//��������ͷ
struct tagDataDescribe
{
	WORD							wDataSize;						//���ݴ�С
	WORD							wDataDescribe;					//��������
};

//////////////////////////////////////////////////////////////////////////

//Ĭ����Ϣ
#define DTP_NULL					0								//��Ч����

//������Ϣ
//������Ϣ
#define DTP_USER_ID					1								//�û� I D
#define DTP_GAME_ID					2								//�û� I D
#define	DTP_USER_ACCOUNTS			3								//�û��ʺ�
#define	DTP_USER_PASS				4								//�û�����
#define DTP_USER_GENDER				5								//�û��Ա�
#define DTP_USER_FACE				6								//�û�ͷ��
#define DTP_USER_RIGHT				7								//�û�Ȩ��
#define DTP_MASTER_RIGHT			8								//����Ȩ��
#define	DTP_UNDER_WRITE				9								//����ǩ��
#define	DTP_LOVE_LINESS				10								//�û�����

//״̬��Ϣ
#define DTP_USER_STATUS				100								//�û�״̬
#define DTP_USER_TABLE				101								//��Ϸ����
#define DTP_USER_CHAIR				102								//���Ӻ���
#define DTP_USER_PING				103								//������ʱ
#define DTP_USER_INDEX				104								//�û�����
#define DTP_USER_ROUND				105								//ѭ������
#define DTP_LOOK_ON_USER			106								//�Թ����

//������Ϣ
#define DTP_USER_WIN				200								//ʤ������
#define DTP_USER_LOST				201								//�������
#define DTP_USER_DRAW				202								//�;�����
#define DTP_USER_FLEE				203								//�Ӿ�����
#define DTP_WIN_RATE				204								//�û�ʤ��
#define DTP_LOST_RATE				205								//�û�����
#define DTP_DRAW_RATE				206								//�û�����
#define DTP_FLEE_RATE				207								//�û�����
#define DTP_USER_TAX				208								//�û�˰��
#define DTP_INSURE_SCORE			209								//�洢���
#define DTP_GAME_GOLD				210								//��Ϸ���
#define DTP_USER_SCORE				211								//�û�����
#define DTP_USER_PLAY_COUNT			212								//�ܾ�����
#define DTP_USER_EXPERIENCE			213								//�û�����
#define DTP_GAME_LEVEL				214								//��Ϸ�ȼ�
#define DTP_COMMUNITY_LEVEL			215								//�����ȼ�

//��չ��Ϣ
#define DTP_USER_GROUP_ID			300								//���� I D
#define DTP_USER_GROUP_NAME			301								//��������
#define DTP_USER_NOTE				302								//�û���ע
#define DTP_USER_DESCRIBE			303								//�û�����
#define DTP_USER_COMPANION			304								//�û���ϵ

//ϵͳ��Ϣ
#define DTP_COMPUTER_ID				1000							//��������
#define DTP_STATION_PAGE			1001							//վ��ҳ��
#define DTP_OPTION_BUFFER			1002							//������Ϣ

//////////////////////////////////////////////////////////////////////////

//���ݰ�������
class CSendPacketHelper
{
	//��������
protected:
	WORD								m_wDataSize;					//���ݴ�С
	WORD								m_wBufferSize;					//�����С
	BYTE								* m_pcbBuffer;					//����ָ��

	//��������
public:
	//���캯��
	CSendPacketHelper(void * pcbBuffer, WORD wBufferSize)
	{
		m_wDataSize=0;
		m_wBufferSize=wBufferSize;
		m_pcbBuffer=(BYTE *)pcbBuffer;
	}
	//��������
	virtual ~CSendPacketHelper() {}

	//���ܺ���
public:
	//��������
	void CleanData() { m_wDataSize=0; }
	//��ȡ��С
	WORD GetDataSize() { return m_wDataSize; }
	//��ȡ����
	void * GetDataBuffer() { return m_pcbBuffer; }
	//��������
	bool AddPacket(void * pData, WORD wDataSize, WORD wDataType)
	{
		//Ч���С
		ASSERT((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)<=m_wBufferSize);
		if ((wDataSize+sizeof(tagDataDescribe)+m_wDataSize)>m_wBufferSize) return false;

		//��������
		ASSERT(m_pcbBuffer!=NULL);
		tagDataDescribe * pDataDescribe=(tagDataDescribe *)(m_pcbBuffer+m_wDataSize);
		pDataDescribe->wDataSize=wDataSize;
		pDataDescribe->wDataDescribe=wDataType;

		//��������
		if (wDataSize>0)
		{
			ASSERT(pData!=NULL);
			CopyMemory(pDataDescribe+1,pData,wDataSize);
		}

		//��������
		m_wDataSize+=sizeof(tagDataDescribe)+wDataSize;
	
		return true;
	}
};

/////////////////////////////////////////////////////////////////////////////////////////

//���ݰ�������
class CRecvPacketHelper
{
	//��������
protected:
	WORD								m_wDataPos;						//���ݵ�
	WORD								m_wDataSize;					//���ݴ�С
	BYTE								* m_pcbBuffer;					//����ָ��

	//��������
public:
	//���캯��
	CRecvPacketHelper(LPCVOID pcbBuffer, WORD wDataSize)
	{
		m_wDataPos=0;
		m_wDataSize=wDataSize;
		m_pcbBuffer=(BYTE *)pcbBuffer;
	}
	//��������
	virtual ~CRecvPacketHelper() {}

	//���ܺ���
public:
	//��ȡ����
	void * GetData(tagDataDescribe & DataDescribe)
	{
		//Ч������
		if (m_wDataPos>=m_wDataSize) 
		{
			ASSERT(m_wDataPos==m_wDataSize);
			DataDescribe.wDataSize=0;
			DataDescribe.wDataDescribe=DTP_NULL;
			return NULL;
		}

		//��ȡ����
		ASSERT((m_wDataPos+sizeof(tagDataDescribe))<=m_wDataSize);
		CopyMemory(&DataDescribe,m_pcbBuffer+m_wDataPos,sizeof(tagDataDescribe));
		ASSERT((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)<=m_wDataSize);

		//Ч������
		if ((m_wDataPos+sizeof(tagDataDescribe)+DataDescribe.wDataSize)>m_wDataSize)
		{
			DataDescribe.wDataSize=0;
			DataDescribe.wDataDescribe=DTP_NULL;
			return NULL;
		}

		//��������
		void * pData=NULL;
		if (DataDescribe.wDataSize>0) pData=m_pcbBuffer+m_wDataPos+sizeof(tagDataDescribe);
		m_wDataPos+=sizeof(tagDataDescribe)+DataDescribe.wDataSize;
		return pData;
	};
};

//////////////////////////////////////////////////////////////////////////

#endif