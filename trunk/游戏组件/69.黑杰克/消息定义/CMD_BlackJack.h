#ifndef CMD_BLACKJACK_FILE
#define CMD_BLACKJACK_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							21						//��Ϸ I D
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("�ڽܿ�")						//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����
#define MIN_TAX_NUM						100L									//��͵������ֲ���˰

//�汾����
#define		MAXCLIENTVER			    MAKELONG(0,4)
#define		LESSCLIENTVER			    0
 
//��Ϸ״̬����
#define GS_FREE  						0									//����״̬
#define	GS_PLAYING				        100									//��Ϸ״̬
#define GS_ADDGOLDING   				101									//��ע״̬

//����ԭ��
#define GO_NORMAL_FINISH			    0x10								//��������

//���״̬
#define NO_CARD					        0									//����״̬
#define INIT_CARD						1									//��ʼ״̬
#define CUT_CARD						2                                   //����״̬
#define STOP_CARD						3									//ͣ��״̬
#define	BRUST_CARD						4									//����״̬
#define	DOUBLE_GOLD						5									//˫��״̬

//////////////////////////////////////////////////////////////////////////
//����������ṹ

//�����붨��

#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_GOLD					101									//��ע���
#define SUB_S_INIT_CARD					102									//��ʼ����
#define SUB_S_CUT_CARD					103 								//������Ϣ
#define SUB_S_CUT_CARD_MARK 			104 								//�л�����
#define SUB_S_SEND_CARD					105									//������Ϣ
#define SUB_S_STOP_CARD				    106									//�û�ͣ��
#define SUB_S_BRUST_CARD				107									//������Ϣ
#define SUB_S_BANKER_SEND_CARD   		108									//ׯ�ҷ���
#define SUB_S_DOUBLE_GOLD   	    	109									//ׯ�ҷ���
#define SUB_S_USER_LEFT     	    	110									//�м�ǿ��
#define SUB_S_GAME_END					111									//��Ϸ����

//��Ϣ�ṹ��

//����״̬
struct CMD_S_StatusFree
{
	LONG								dwBasicGold;						//�������
};

//��ע״̬
struct CMD_S_StatusAddGold
{
	WORD				 				wCurrentUser;						//��ǰ���
	WORD						        wBankerStation;				    	//ׯ��λ��
	BOOL						        bAddGoldEnd[GAME_PLAYER];	    	//��ע����
	LONG								dwBasicGold;						//�������
	LONG								lMaxGold;							//�����ע
	LONG								lBasicGold;							//�������
	LONG								lTableGold[2*GAME_PLAYER];			//������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD				 				wCurrentUser;						//��ǰ���
	WORD						        wBankerStation;				    	//ׯ��λ��
	BYTE								cbPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE								cbTableCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE								cbTableCardArray[GAME_PLAYER][13];	//�˿�����
	LONG								lMaxGold;							//�����ע
	LONG								lBasicGold;							//�������
	LONG								lTableGold[2*GAME_PLAYER];			//������
	BOOL								bBankerGetCard;						//ׯ��Ҫ��
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{	
	WORD						        wBankerStation;				    	//ׯ��λ��
	BYTE								cbFundusCard;						//�����˿�
	WORD				 				wCurrentUser;						//��ǰ���
	LONG								lMaxGold;							//�����ע
	LONG								lTurnBasicGold;						//������ע
	BYTE								cbCardData[GAME_PLAYER];			//�û��˿�
};

//��ע���
struct CMD_S_AddGold
{
	BOOL                                bDoubleGold;                        //�Ƿ�˫��
	WORD								wCurrentUser;						//��ǰ�û�
	LONG								lLastAddGold;						//��ע��Ŀ
};

//��ʼ����
struct CMD_S_InitCard
{
	BYTE								cbFundusCard;						//�����˿�
	BOOL								bCanCutCard;                        //�ܷ����
	BYTE								cbCardData[GAME_PLAYER];			//�û��˿�
	BYTE								cbCardSum;	            			//�˿���ֵ
};

//�������ݰ�
struct CMD_S_SendCard
{
	WORD								wCurrentUser;						//��ǰ�û�
	BYTE								cbUserCard;						    //�û��˿�
	WORD                                wGetCardUser;                       //Ҫ�����
	BYTE								cbCardSum;	            			//�˿���ֵ
	BOOL								bCutCard;	            			//�û����ƣ�0:�����ƣ�1:�����ƣ�
};

//�û�����
struct CMD_S_CutCard
{
	WORD								wCutCardUser;						//�����û�
	BYTE								cbFirstCard;					   	//�����˿�
	BYTE								cbSecondCard;						//�����˿�	
	BYTE								cbFirstCardSum;					   	//�����˿�
	BYTE								cbSecondCardSum;					//�����˿�	
};

//�û�ͣ��
struct CMD_S_StopCard
{
	WORD								wStopUser;							//ͣ���û�
	BOOL								bDoubleBrsut;						//˫������
};

//�û�����
struct CMD_S_BrustCard
{
	WORD								wBrustUser;							//�����û�
};

//�û�ǿ��
struct CMD_S_UserLeft
{
	WORD								wUserChairID;						//ǿ���û�
	BOOL								bAddGoldEnd;						//��ע����
};

//��Ϸ����
struct CMD_S_GameEnd
{
	LONG								lTax;								//��Ϸ˰��
	LONG								lGameGold[GAME_PLAYER];				//��Ϸ�÷�
	BYTE								cbUserCard[GAME_PLAYER];			//�û��˿�
	BYTE								cbCutCard[GAME_PLAYER];		    	//�����˿�
	BYTE								cbCardSum[GAME_PLAYER];	   	    	//�˿���ֵ
	BYTE								cbCutCardSum[GAME_PLAYER];	     	//�˿���ֵ
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

//�����붨��
#define SUB_C_ADD_GOLD					1									//�û���ע
#define SUB_C_CANCEL_GOLD				2   								//ȡ����ע
#define SUB_C_GET_CARD					3									//�û�Ҫ��
#define SUB_C_STOP_CARD					4									//�û�ͣ��
#define SUB_C_DOUBLE_GOLD				5									//�û�˫��
#define SUB_C_CUT_CARD	    			6									//�û�����

//��Ϣ�ṹ��

//�û���ע
struct CMD_C_AddGold
{	
	BOOL                                bDoubleGold;                        //�Ƿ�˫��
	LONG								lGold;								//��ע��Ŀ
};

//�û�Ҫ��
struct CMD_C_GetCard
{
	BOOL								bCutCard;	            			//�û����ƣ�0:�����ƣ�1:�����ƣ�
};

//�û�Ҫ��
struct CMD_C_StopCard
{
	BOOL								bCutCard;	            			//�û����ƣ�0:�����ƣ�1:�����ƣ�
};

//////////////////////////////////////////////////////////////////////////

#endif