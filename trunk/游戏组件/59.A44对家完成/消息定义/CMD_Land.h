#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��
#ifdef VIDEO_GAME
#define KIND_ID						615									//��Ϸ I D
#else
#define KIND_ID						611									//��Ϸ I D
#endif

#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("A44�Լ�")						//��Ϸ����

//��Ϸ״̬
#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ
#define GS_WK_SCORE					GS_PLAYING							//�з�״̬
#define GS_WK_PLAYING				GS_PLAYING+1						//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_LAND_SCORE			101									//�з�����
#define SUB_S_GAME_START			102									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_PASS_CARD				104									//��������
#define SUB_S_GAME_END				105									//��Ϸ����
//////////////////////////////////////////////////////////////////////////
#define SUB_S_GAME_CHA				106									//����
#define SUB_S_GAME_NOCHA			107									//��������
#define SUB_S_GAME_DIAN				108									//����
#define SUB_S_GAME_NODIAN			109									//��������

#define SUB_S_LI_GUN_REQUEST        110                                 //��������
#define SUB_S_JUE_GUN               111                                 //���Ծ��
#define SUB_S_JUE_GUN_REQUEST       112                                 //�������
#define SUB_S_LI_GUN                113                                 //��������
//////////////////////////////////////////////////////////////////////////
//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lBaseScore;							//��������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[GAME_PLAYER][15];			//�����˿�
	BYTE							bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[15];					//�����б�
	bool							bUserTrustee[GAME_PLAYER];			//����й�
	bool							gou;								//��
	bool							dian;								//��
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[25];						//�˿��б�
};

//�����˿�
struct CMD_S_SendAllCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[GAME_PLAYER][15];			//�˿��б�
	BYTE							cbCardNum[GAME_PLAYER];				//�û����˿���Ŀ
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD				 			wCurrentUser;						//��ǰ���	
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[15];						//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG							lGameTax;							//��Ϸ˰��
	LONG							lGameScore[GAME_PLAYER];			//��Ϸ����
	BYTE							bCardCount[GAME_PLAYER];			//�˿���Ŀ
	BYTE							bCardData[54];						//�˿��б� 
};

/////////////////////////////////////////��ӵ����ݰ�/////////////////////////////////////
//��������
struct CMD_S_LiGunRequest
{
	WORD                            wRequest;
};

//����������
struct CMD_S_CanJueGun
{
	WORD                            wCanJueChair[2];
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_LAND_SCORE			1									//�û��з�
#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������
#define SUB_C_TRUSTEE				4									//�й���Ϣ
//////////////////////////////////////////////////////////////////////////
#define SUB_C_GAME_CHA				6									//����
#define SUB_C_GAME_NOCHA			7									//��������
#define SUB_C_GAME_DIAN				8									//����
#define SUB_C_GAME_NODIAN			9									//��������

#define SUB_C_LI_GUN                10                                  //����
#define SUB_C_JUE_GUN               11                                  //���
#define SUB_C_AGREE                 12                                  //�Ƿ�ͬ�������
#define SUB_C_LI_GUN_SUPERTIME      13                                  //������ʱ
#define SUB_C_JUE_GUN_SUPERTIME     14                                  //�����ʱ
#define SUB_C_LI_GUN_ANSWER         15                                  //������
#define SUB_C_NOT_LI_GUN            16                                  //������
#define SUB_C_NOT_JUE_GUN           17                                  //�����
//////////////////////////////////////////////////////////////////////////
//�йܽṹ
struct CMD_C_UserTrustee 
{
	WORD							wUserChairID;						//�������
	bool							bTrustee;							//�йܱ�ʶ
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[15];						//�˿��б�
};

////////////////////////////////////////////////��ӵ����ݰ�//////////////////////////////////////////
//�������
struct CMD_C_LiGun
{
	WORD                            wLiGunUserChairID;
};

struct CMD_C_LiGunAnswer
{
	BYTE                            cbAnswer;                          //��ʶ��1Ϊͬ�⣬0Ϊ��ͬ��
	WORD                            wAgreeChairID;                     //ͬ�⣨��ͬ�⣩��ҵ���λ��
	BYTE                            cbLiOrJue;                         //��ʶ��1Ϊ������2Ϊ���
};

struct CMD_C_NotLiOrJue
{
	WORD                            wNotLiOrJueChair;
};

//////////////////////////////////////////////////////////////////////////

#endif