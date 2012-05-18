#ifndef CMD_SHOWHAND_HEAD_FILE
#define CMD_SHOWHAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							301									//��Ϸ I D
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("240��Ϸ")					//��Ϸ����
#define GAME_GENRE						GAME_GENRE_SCORE					//��Ϸ����


//��Ϸ״̬
#define	GS_SEND_CARD					GS_FREE+10
#define GS_WAIT_PUBLIC					GS_FREE+11				//�ȴ�����
#define	GS_OUT_CARD						GS_FREE+14				//����

#define								TOTAL_CARDS_CNT		92
#define								BACK_CARD_CNT		0
#define								PLAYER_CARD_CNT		(TOTAL_CARDS_CNT-BACK_CARD_CNT)/GAME_PLAYER	  

//�˿˻�ɫ
#define UG_FANG_KUAI				0x00			//����	0000 0000
#define UG_MEI_HUA					0x10			//÷��	0001 0000
#define UG_HONG_TAO					0x20			//����	0010 0000
#define UG_HEI_TAO					0x30			//����	0011 0000
#define UG_NT_CARD					0x40			//��ͨ����	0100 0000
#define UG_NT_CARD_EX				0x50			//����
#define UG_NT_CARD_KING				0x60			//��С��
#define UG_ERROR_HUA				0xF0			//����  1111 0000


//����������Ϣ
#define IDM_PUBLIC_CARD					WM_USER+500

//������
#define	SUB_CS_FINISH_ASHIBIT				49				//���ճ��

#define	SUB_SC_BEGIN_SEND					51				//��ʼ����
#define SUB_SC_SEND_CARD					52				//������Ϣ
#define SUB_NT_CARD							53				//�û�����
#define SUB_SC_SEND_FINISH					54				//�������
#define SUB_CS_SEND_FINISH					57				//�������

#define SUB_SC_BEGIN_OUT					59				//��ʼ��Ϸ
#define SUB_OUT_CARD						60				//�û�����
#define SUB_SC_BATCH_OUT					62				//�������ƣ����һ�֣�
#define SUB_SC_GAME_POINT					63				//�û�����
#define	SUB_SC_GAME_END						65				//��Ϸ����


#define SUB_SC_THROW_CARD					67				//˦��


#define	SUB_CS_LOOK_INFO					66				//�Ƿ������Թ�

//////////////////////////////////////////////////////////////////////////


struct GS_FreeStruct
{
	 char	buf[2];
};

struct GS_SendStruct
{
	WORD	wBanker;									//ׯ��
	WORD	wPublic;									//������
	
	BYTE	bPublicCardFlower;							//���ƻ�ɫ
	BYTE	bPublicCardCnt;								//����
	BYTE	bPublicCard[PLAYER_CARD_CNT];	
	BYTE	bAllMeCardCnt;								
	BYTE	bAllMeCard[PLAYER_CARD_CNT];


	BOOL	bIsCanLook;									//�Ƿ������Թ�

	
	char	buf[2];
};

struct GS_WaitStruct
{
	WORD	wBanker;									//ׯ��
	WORD	wPublic;									//������

	BYTE	bPublicCardFlower;							//���ƻ�ɫ
	BYTE	bPublicCardCnt;								//����
	BYTE	bPublicCard[PLAYER_CARD_CNT];				
	BYTE	bMeCardCnt;	
	BYTE	bMeCard[PLAYER_CARD_CNT+BACK_CARD_CNT];
	
	BOOL	bIsCanLook;									//�Ƿ������Թ�

	char	buf[2];
};

struct GS_GiveStruct
{
	WORD	wBanker;									//ׯ��
	WORD	wPublic;									//������
	BYTE	bPublicCardFlower;							//���ƻ�ɫ

	BYTE	bMeCardCnt;	
	BYTE	bMeCard[PLAYER_CARD_CNT+BACK_CARD_CNT];

	BOOL	bIsCanLook;									//�Ƿ������Թ�
	
	char	buf[2];
};

struct GS_OutStruct
{
	WORD	wBanker;									//ׯ��
	WORD	wPublic;									//������
	WORD	wCurSeat;									//��ǰ������
	WORD	wThrowSeat;									// ˦���ߣ�����һ�ֵ����ȳ�����)
	BYTE	bPublicCardFlower;							//���ƻ�ɫ

	BYTE	bPublicCardCnt;
	BYTE	bPublicCard[2];								//��������
	BYTE	bFirstCardCnt;
	BYTE	bFirstCard[PLAYER_CARD_CNT];

	BYTE	bScoreCardCnt[GAME_PLAYER];
	BYTE	bScoreCard[GAME_PLAYER][25];				//����
	BYTE	bMeCardCnt;	
	BYTE	bMeCard[PLAYER_CARD_CNT];
	BYTE	bOutCardCnt[GAME_PLAYER];
	BYTE	bOutCard[GAME_PLAYER][PLAYER_CARD_CNT];		   //����
	
	int		nCurScore[GAME_PLAYER];							//��ǰ�÷�
	BOOL	bIsCanLook;									   //�Ƿ������Թ�

	BOOL	bIsHaveScore;
	BYTE	bMaxCardCnt;
	BYTE	bMaxCard[PLAYER_CARD_CNT];		   //�����
	BYTE	bOutHandCnt;									//���˼�����
	BYTE	bLastOutCardCnt[PLAYER_CARD_CNT][GAME_PLAYER];
	BYTE	bLastOutCard[PLAYER_CARD_CNT][GAME_PLAYER][PLAYER_CARD_CNT];	 //��һ�ֳ���
	
	char	buf[2];
};

struct ServerBeginSend
{
	BOOL				bIsResend;						//�Ƿ������¿�ʼ����
	BYTE				bAllCard[PLAYER_CARD_CNT];		//һ�η�����������ҵ���
	BOOL				bIsCanLook;						//�Ƿ������Թ�
};


//���������ݰ�  �������������
struct ClientNtStruct
{
	BYTE				bCardCount;						//������Ŀ
	BYTE				bCard[2];						//����
};

//���������ݰ�  ������ͻ��ˣ�
struct ServerNtStruct
{
	WORD				wNTSeat;						//������
	BYTE				bHuaKind;						//�Ʊ��
	BYTE				bCardCount;						//������Ŀ
	BYTE				bCard[2];						//����
};



//��ʼ����
struct ServerBeginOut
{
	WORD	wFirstOutSeat;									//���ȳ�����
	
};

//�û�˦��ʧ�����ݰ� ������ͻ��ˣ�
struct ServerThrowCard
{
	WORD				wNextSeat;						//��һ��������
	BYTE				iCardCount;						//�˿���Ŀ
	BYTE				bDeskStation;					//��ǰ������
	BYTE				iResultCount;					//����˿���Ŀ
	BYTE				iCardList[78];					//�˿���Ϣ
};


//���ƣ��ͻ��ˣ�
struct ClientOutCard
{
	WORD	wSeat;											//������
	BYTE	bOutCardCnt;									//���Ƹ���
	BYTE	bOutCard[PLAYER_CARD_CNT];						//������
};
struct ServerOutCard
{
	WORD	wSeat;											//���Ƶ���
	WORD	wNextSeat;										//��һ��������
	BOOL	bIsLastOut;										//�Ƿ������һ������
	BYTE	bOutCardCnt;									//���Ƹ���
	BYTE	bOutCard[PLAYER_CARD_CNT];						//������
};

//�����ƣ���ʾʣ��ȫ�����ƣ�
struct ServerBatchOut
{
	BYTE	bOutCardCnt[GAME_PLAYER];						//���Ƹ���
	BYTE	bOutCard[GAME_PLAYER][PLAYER_CARD_CNT];			//������
};

struct ServerGamePoint
{
	WORD	wWiner;											//Ӯ����
	BYTE	bScoreValue;									//��ֵ
	BYTE	bScoreCardCnt;									
	BYTE	bScoreCard[PLAYER_CARD_CNT];
};
struct ClientLookInfo
{
	BOOL	bIsCanLook;
};
//��Ϸ����
struct ServerGameEnd
{
	BOOL	bIsForceExit;									//�Ƿ���ǿ��
	int		iScore[GAME_PLAYER];							//���÷�(Ӯ�ҵĵ÷�д�����ݿ�)
	char	buf[2];
};

//////////////////////////////////////////////////////////////////////////

#endif