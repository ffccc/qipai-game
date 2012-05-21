#ifndef CMD_UPGRADE_HEAD_FILE
#define CMD_UPGRADE_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						ID_GAME_BRIDGE									//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("������Ϸ")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

//��Ϸ״̬
#define GS_UG_FREE					GS_FREE								//����״̬
#define GS_UG_CALL					(GS_PLAYING+1)						//����״̬
#define GS_UG_SCORE					(GS_PLAYING+2)						//�з�״̬
#define GS_UG_PLAY					(GS_PLAYING+3)						//��Ϸ״̬

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_SEND_CARD				101									//�����˿�
#define SUB_S_GAME_PLAY				102									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_THROW_RESULT			104									//˦�ƽ��
#define SUB_S_TURN_BALANCE			105									//һ��ͳ��
#define SUB_S_GAME_END				106									//��Ϸ����
#define SUB_S_CALL_SCORE			107									//�з�����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
};


//�з�״̬
struct CMD_S_StatusScore
{
	//��Ϸ����
	WORD							wBankerUser;						//ׯ���û�
	BYTE							bBossScore;							//ׯ�ҷ���
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//���Ʊ���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�

	
	//�з���Ϣ
	BYTE							bScoreInfo[4];						//�з���Ϣ
	WORD							wCallScoreUser;						//�з��û�
	WORD				 			wCurrentUser;						//��ǰ���

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	
};


//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbMainColor;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wFirstOutUser;						//�����û�

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	//�˿˱���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�

	//���Ʊ���
	BYTE							cbOutCardCount[4];					//������Ŀ
	BYTE							cbOutCardData[4][52];				//�����б�

	//�÷ֱ���
	BYTE							cbScoreCardCount;					//�˿���Ŀ
	BYTE							cbScoreCardData[48];				//�÷��˿�
};

//�ȴ�״̬
struct CMD_S_StatusWait
{
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
};


//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ���û�
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ
	BYTE							cbJuKuang;							//�ֿ�
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�
};

//�û��з�
struct CMD_S_CallScore
{
	WORD							bCallScoreUser;						//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCurrentScore;						//��ǰ�з�
	BYTE							bCurrentColor;						//��ǰ��ɫ
	BYTE							bMultiples;							//��Ϸ����
	BYTE							bLastScore;							//�ϴη���
	BYTE							bLastColor;							//�ϴλ�ɫ
	bool							bDbl;								//�ӱ���־
	bool							bReDbl;								//�ټӱ���־
};

//��Ϸ��ʼ
struct CMD_S_GamePlay
{
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							m_cbLandScore;						//��������
	BYTE							m_cbLandColor;						//������ɫ
	BYTE							bMultiples;							//��Ϸ����
	BYTE							bCardCount[4];						//�˿���Ŀ
	BYTE							bCardData[4][60];					//�˿��б� 
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//�˿���Ŀ
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[52];						//�˿��б�
	BYTE							bHandOutCardCount;					//�����˿���Ŀ
	BYTE							bHandOutCardData[60];				//�����˿��б� 
	bool							bHandOutFlag[4];					//���ֿ���
};

//˦�ƽ��
struct CMD_S_ThrowResult
{
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbThrowCardCount;					//�˿���Ŀ
	BYTE							cbResultCardCount;					//�˿���Ŀ
	BYTE							cbCardDataArray[104];				//�˿�����
};

//һ��ͳ��
struct CMD_TurnBalance
{
	WORD							wTurnWinner;						//һ��ʤ��
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBankerScore;						//ׯ�ҵ÷�
	BYTE							bOtherScore;						//���ҵ÷�
	bool							bHandOutFlag[4];					//���ֿ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lScore[4];							//�û��÷�
	WORD							wConcealTime;						//��Ϸ����
	BYTE							bBankerScore;						//ׯ�ҵ÷�
	BYTE							bOtherScore;						//���ҵ÷�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//�û�����
#define SUB_C_CALL_SCORE			2									//�û��з�
#define SUB_C_MULTIPLES				3									//�û��ӱ�

//�û��з�
struct CMD_C_CallScore
{
	BYTE							bBossScore;							//��������
	BYTE							bBossColor;							//������ɫ
};

//�û��ӱ�
struct CMD_C_OnMultiples
{
	BYTE							bMultiples;							//��Ϸ����
};

//�û�����
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[52];						//�˿��б�
	bool							bIsHandOut[4];							//�Ƿ����ֳ���]
	bool							bIsAutoOut;							//�Ƿ��Զ�����
};

//////////////////////////////////////////////////////////////////////////

#endif