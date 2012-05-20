#ifndef CMD_GAME_HEAD_FILE
#define CMD_GAME_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//������
//��Ϸ����
#ifdef VIDEO_GAME
#define KIND_ID						504									//��Ϸ I D
#define GAME_NAME					TEXT("˫��"	)					//��Ϸ����
#define GAME_PLAYER					4									//��Ϸ����
#else
#define KIND_ID						109									//��Ϸ I D
#define GAME_NAME					TEXT("˫��"	)					//��Ϸ����
#define GAME_PLAYER					4									//��Ϸ����
#endif

//////////////////////////////////////////////////////////////////////////
//��Ŀ����

#define MAX_COUNT					27									//�����Ŀ
#define FULL_COUNT					108									//ȫ����Ŀ

//////////////////////////////////////////////////////////////////////////

//��ֵ����
#define	MASK_COLOR					0xF0								//��ɫ����
#define	MASK_VALUE					0x0F								//��ֵ����

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE					3									//��������
#define CT_DOUBLE_LINK				4									//��������
#define CT_THREE_LINK				5									//��������
#define CT_SHUNZI					6									//˳������
#define CT_BOMB						7									//ը������
#define CT_BOMB_TW					8									//����ը��
#define CT_BOMB_LINK				9									//��ը����

//////////////////////////////////////////////////////////////////////////
//״̬����

//��Ϸ״̬
#define GS_RF_FREE					GS_FREE								//�ȴ���ʼ
#define GS_RF_PLAYING				GS_PLAYING							//��Ϸ״̬

//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_S_GAME_START			100									//��Ϸ����
#define SUB_S_OUT_CARD				101									//�û�����
#define SUB_S_PASS_CARD				102									//�û�����
#define SUB_S_GAME_END				103									//��Ϸ����
#define SUB_S_TRUSTEE				104									//�û��й�

//����״̬
struct CMD_S_StatusFree
{
	//��Ԫ����
	LONG							lCellScore;							//��Ԫ����

	//ׯ���û�
	WORD							wBankerUser;						//ׯ���û�


};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONG							lCellScore;							//��Ԫ����
	WORD							wCurrentUser;						//��ǰ���

	//ׯ���û�
	WORD							wBankerUser;						//ׯ���û�

	//ʤ����Ϣ
	WORD							wWinCount;							//ʤ������
	WORD							wWinOrder[GAME_PLAYER];				//ʤ���б�
	bool							bTrustee[GAME_PLAYER];				//�Ƿ��й�


	//������Ϣ
	WORD							wTurnWiner;							//ʤ�����
	BYTE							cbTurnCardCount;					//������Ŀ
	BYTE							cbTurnCardData[MAX_COUNT];			//��������
	BYTE							cbMagicCardData[MAX_COUNT];			//����˿�

	//�˿���Ϣ
	BYTE							cbHandCardData[MAX_COUNT];			//�����˿�
	BYTE							cbHandCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE							cbPartnerCardData[MAX_COUNT];		//�������˿�	
};

//�����˿�
struct CMD_S_GameStart
{
	WORD				 			wCurrentUser;						//��ǰ���
	LONG							lCellScore;							//��Ԫ����
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
	BYTE							cbPartnerCardData[MAX_COUNT];		//�������˿�
	bool							bTrustee[GAME_PLAYER];						//�Ƿ��й�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							cbCardData[MAX_COUNT];				//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							cbTurnOver;							//һ�ֽ���
	WORD				 			wCurrentUser;						//��ǰ���
	WORD				 			wPassCardUser;						//�������
};

//��Ϸ����
struct CMD_S_GameEnd
{
	//��Ϸ�ɼ�
	LONG							lGameScore[GAME_PLAYER];			//��Ϸ����

	//�˿���Ϣ
	BYTE							cbCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							cbCardData[GAME_PLAYER][MAX_COUNT];	//�˿��б�
	LONG							lGameTax;							//��Ϸ˰��
};
//�û��й�
struct CMD_S_Trustee
{
	bool							bTrustee;							//�Ƿ��й�
	WORD							wChairID;							//�й��û�
};
//////////////////////////////////////////////////////////////////////////
//�����

#define SUB_C_OUT_CARD				1									//�û�����
#define SUB_C_PASS_CARD				2									//�û�����
#define SUB_C_TRUSTEE				3									//�û��й�

//�û�����
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[MAX_COUNT];				//�˿�����
};
//�û��й�
struct CMD_C_Trustee
{
	bool							bTrustee;							//�Ƿ��й�	
};
//////////////////////////////////////////////////////////////////////////

#endif