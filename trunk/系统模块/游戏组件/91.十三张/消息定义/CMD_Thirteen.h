#ifndef CMD_LIANGFUTOU_HEAD_FILE
#define CMD_LIANGFUTOU_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
//�����궨��
#ifdef VIDEO_GAME
#define KIND_ID							12									//��Ϸ I D
#else
#define KIND_ID							13									//��Ϸ I D
#endif
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("ʮ����")					    //��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define HAND_CARD_COUNT					13									//�˿���Ŀ

//��Ϸ״̬
#define GS_WK_FREE				    	GS_FREE								//�ȴ���ʼ
#define GS_WK_SETCHIP			    	GS_PLAYING+1						//������ע
#define GS_WK_PLAYING			    	GS_PLAYING+2						//��Ϸ����
#define GS_WK_CALL_BANKER				GS_PLAYING + 3						//��ׯ״̬

//�ֶ�����
enum enSegmentType
{
	enFront ,																//ǰ������
	enMid ,																	//�ж�����
	enBack,																	//�������
	enAllSeg,																//�����˿�
	enErr,																	//��������
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ
#define SUB_S_SEND_CARD					100									//��������
#define SUB_S_SETCHIP					101									//������ע
#define SUB_S_SETSEGMENT				102									//���÷ֶ�
#define SUB_S_SHOW_CARD					103									//���̯��
#define SUB_S_GAME_END					104									//��Ϸ����
#define SUB_S_COMPARE_CARD				105									//�Ƚ��˿�
#define SUB_S_SHOW_CARD_TIME			106									//̯��ʱ��
#define SUB_S_GAME_START				107									//��Ϸ��ʼ
#define SUB_S_START_CALL_BANKER			108									//��ʼ��ׯ									
#define SUB_S_CALL_BANKER				109									//��ҽ�ׯ
#define SUB_S_END_ALL_NO_CALL_BANKER	110									//������ׯ

//////////////////////////////////////////////////////////////////////////

//��ʼ��ׯ
struct CMD_S_StartCallBanker
{
	WORD								wCurrentCaller;						//��ǰ���
};

//��ҽ�ׯ
struct CMD_S_CallBanker
{
	WORD								wLastCaller ;						//�ѽ����
	bool								bCallBanker ;						//��ׯ��ʶ
	WORD								wCurrentCaller;						//��ǰ���
} ;

//̯��ʱ��
struct CMD_S_ShowCardTime
{
	BYTE								bShowCardTime;						//̯��ʱ��
	WORD								wBanker;							//ׯ�����
	LONG								wMaxChip;							//�����ע
};

//��ע״̬
struct  CMD_S_StatusSetChip
{
	LONG								nChip[GAME_PLAYER];					//��ע��С 
	BYTE								bHandCardData[HAND_CARD_COUNT];		//�˿�����
	WORD								wBanker;							//ׯ�����
	LONG								wMaxChip;							//�����ע
	LONG								wUserToltalChip;					//�ܹ����

};

//��ׯ״̬
struct CMD_S_StatusCallBanker
{
	WORD								wCurrentCaller;						//��ǰ���
} ;

//�����˿�
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						 //��ǰ���
	WORD								wBanker;							 //ׯ�����
	BYTE								bCardData[HAND_CARD_COUNT];			 //�����˿�
	LONG								wMaxChip;							//�����ע
	LONG								wUserToltalChip;					//�ܹ����
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG								lGameTax;							//��Ϸ˰��
	LONG								lGameScore[4];						//��Ϸ����
	BYTE								bEndMode;							//������ʽ
};

//������ע
struct CMD_S_SetChip
{
	WORD								wCurrentUser;						//��ע���
	LONG								nChip;								//��ע��С
};

//���÷ֶ�
struct CMD_S_SetSegment
{
    WORD								wCurrentUser;						//��ǰ���
	enSegmentType						SegmentType;						//�ֶ�����
};

//���̯��
struct CMD_S_ShowCard
{
	WORD								wCurrentUser;						//��ǰ���
	BYTE								bFrontCard[3];						//ǰ���˿�
	BYTE								bMidCard[5];						//�ж��˿�
	BYTE								bBackCard[5];						//����˿�
	bool								bCanSeeShowCard;					//�ܷ���
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	LONG								nChip[GAME_PLAYER];					//��ע��С 
	BYTE								bHandCardData[HAND_CARD_COUNT];		//�˿�����
	BYTE								bSegmentCard[GAME_PLAYER][3][5];	//�ֶ��˿�
	bool								bFinishSegment[GAME_PLAYER];		//��ɷֶ�
	WORD								wBanker;							//ׯ�����
	LONG								wMaxChip;							//�����ע
	LONG								wUserToltalChip;					//�ܹ����
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ
#define SUB_C_SETCHIP					300									//ѹע����
#define SUB_C_SEGCARD					301									//�ֶ�����
#define SUB_C_SHOWCARD					302									//���̯��
#define SUB_C_COMPLETE_COMPARE			303									//��ɱȽ�
#define SEB_C_SHOW_CARD_TIME			304									//̯��ʱ��
#define SUB_C_BAO_PAI					305									//��������
#define SUB_C_CALL_BANKER				306									//��ҽ�ׯ
#define SUB_C_NO_CALL_BANKER			307									//��Ҳ���


//̯��ʱ��
struct CMD_C_ShowCardTime
{
	BYTE								bShowCardTime;						//̯��ʱ��
	LONG								wMaxChip;							//�����ע
};

//����ѹע
struct CMD_C_SetChip
{
	LONG								nChip;								//ѹע��С
};

//���÷ֶ�
struct CMD_C_SegCard
{
	enSegmentType						SegmentType;						//�ֶ�����
};

//�ֶ���Ϣ
struct CMD_C_ShowCard
{
	BYTE								bFrontCard[3];						//ǰ���˿�
	BYTE								bMidCard[5];						//�ж��˿�
	BYTE								bBackCard[5];						//����˿�
};
//////////////////////////////////////////////////////////////////////////

#endif
