#ifndef CMD_SHOWHAND_HEAD_FILE
#define CMD_SHOWHAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#ifdef VIDEO_GAME
	#define KIND_ID						527									//��Ϸ I D
#else
	#define KIND_ID						26									//��Ϸ I D
#endif
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("�����走")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define PIECES						27									//�˿���ÿ��

//��Ϸ״̬
#define GS_RF_FREE					GS_FREE								//�ȴ���ʼ
#define GS_RF_PLAYING				GS_PLAYING							//����״̬

//��������
enum enRoomType
{
	enRoomType_Normal,													//��ͨ����
	enRoomType_All														//���ַ���
};


//�����׶�
enum enPayStatus
{
	enNone,																//��
	enJin_Gong,															//����
	enHui_Gong,															//�ع�
	enGet_Hui_Gong,														//��ȡ�ع�
	enKang_Gong															//����
};	
//��Ϸ����
struct Series
{
	BYTE                            bOurSeries;                         //�ҷ�����
	BYTE                            bOtherSeries;                       //�Է�����
	BYTE                            bCurrentSeries;                     //���ּ���
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			101									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				102									//�û�����
#define SUB_S_PASS_CARD				103									//��������
#define SUB_S_PAY_TRIBUTE           104                                 //�û�����
#define SUB_S_GAME_END				105									//��Ϸ����
#define SUB_S_END					106									//���̽���
#define SUB_S_REQUEST_QUIT			107									//�����˳�
#define SUB_S_ANSWER_QUIT           108									//��Ӧ�˳�
#define SUB_S_CARD					109									//�Լҵ���
#define SUB_S_SYSTEM_MESSAGE		110									//ϵͳ��Ϣ
#define SUB_S_LOOKON				111									//�Թ�����

//ϵͳ��Ϣ���ݰ�
struct CMD_S_MESSAGE
{
	WORD							wChairID;							//�����û�
	TCHAR							szBuffer[128];						//ϵͳ��Ϣ
};


//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay{
	LONG							lBaseScore;							//��������
	WORD							wBankerUser;						//ׯ���û�
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[27];						//�����˿�
	BYTE							bCardCount[4];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[27];					//�����б�
	LONG                            lAllTurnScore[4];					//�ֵܾ÷�
	LONG                            lLastTurnScore[4];					//�Ͼֵ÷�
	Series                          series;                             //��Ϸ����
	int                             m_iGameResult[4];                   //ʤ�����
	enRoomType						RoomType;							//��������
	WORD							wServerType;						//����������
};

//�����˿�
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ���û�
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[27];						//�˿��б�
	Series                          series;                             //��Ϸ����
	int                             m_iGameResult[4];                   //ʤ�����
	enRoomType						RoomType;							//��������
	WORD                            wServerType;						//����������
	bool							bLiangPai;							//�Ƿ�����
	BYTE							bLiangCard;							//����
};

//�Լҵ���
struct CMD_S_CARD
{
	WORD							wFromUser;							//�Լ�ID
	BYTE							bCardData[28];						//�˿�����
	BYTE							bCardCount;							//�˿˴�С
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//�˿���Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[27];						//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
};

//�û�����
struct CMD_S_PayTribute
{
	BYTE                            bPayType[4];						//��������
	enPayStatus						bPayStatus;						//�����׶�
	WORD                            wFromUser;                          //�����û�
	WORD                            wToUser;                            //�������û�
	WORD                            wCurrentUser;                       //��ǰ���
	BYTE                            bCard;                              //�����˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lGameTax;							//��Ϸ˰��
	LONG							lGameScore[4];						//��Ϸ����
	BYTE							bCardCount[4];						//�˿���Ŀ
	BYTE							bCardData[108];						//�˿��б�
	Series                          series;                             //��Ϸ����
	int                             m_iGameResult[4];                   //ʤ�����
};

//�������ݰ�
struct CMD_S_REQUEST
{
	WORD							wChairID;							//��λ��
	TCHAR							szBuffer[50];						//�˳�ԭ��
};

//����Ϣ
struct CMD_S_ANSWER
{
	WORD							wChairID;							//��λ��
	bool                            bAnswer;							//��Ӧ��
};
//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_PAY_TRIBUTE           4                                   //��Ϸ����
#define SUB_C_START					5									//��Ϸ��ʼ
#define SUB_C_HALFWAY_QUIT			6									//��;�˳�
#define SUB_C_REQUEST_QUIT			7									//ǿ���˳�
#define SUB_C_ANSWER_QUIT           8									//��Ӧ��
#define SUB_C_QUIT					9									//�û��뿪
#define SUB_C_SYSTEM_MESSAGE		10									//ϵͳ��Ϣ
#define SUB_C_LOOKON				11									//�Թ�����

//ϵͳ��Ϣ���ݰ�
struct CMD_C_MESSAGE
{
	WORD							wChairID;							//�����û�
	TCHAR							szBuffer[128];						//ϵͳ��Ϣ
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[27];						//�˿��б�
};

//�������ݰ�
struct CMD_C_REQUEST
{
	WORD							wChairID;							//��λ��
	TCHAR							szBuffer[50];						//�˳�ԭ��
};

//����Ϣ
struct CMD_C_ANSWER
{
	WORD							wChairID;							//��λ��
	bool                            bAnswer;							//��Ӧ��
};
//�û�����
struct CMD_C_PayTribute
{
	enPayStatus                     bPayType;							//��������
	WORD                            wUser;                              //�����û�
	WORD                            wToUser;							//�ܽ�����
	BYTE                            bCard;                              //�����˿�
};
//////////////////////////////////////////////////////////////////////////

#endif