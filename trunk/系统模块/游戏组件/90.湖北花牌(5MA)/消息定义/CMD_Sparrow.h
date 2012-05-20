#ifndef CMD_SPARROW_HEAD_FILE
#define CMD_SPARROW_HEAD_FILE

//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						3001								//��Ϸ I D
#define GAME_PLAYER					3									//��Ϸ����
#define GAME_NAME					TEXT("���������徫������")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_MATCH)	//��Ϸ����

//��Ϸ״̬
#define GS_MJ_FREE					GS_FREE								//����״̬
#define GS_MJ_PLAY					(GS_PLAYING+1)						//��Ϸ״̬
#define GS_MJ_HAI_DI				10

//////////////////////////////////////////////////////////////////////////

//�������
struct CMD_WeaveItem
{
	BYTE				cbWeaveKind;					//�������
	BYTE				cbCenterCard;					//�����˿�
	BYTE				cbPublicCard;					//������־
	WORD				wProvideUser;					//��Ӧ�û�
};

//��������������
struct CMD_HuCardItem
{
	BYTE                 cbCenterCard[8];               //8����ϵ�������ֵ
	BYTE                 cbWeaveKind[8];                //8����ϵĲ�������
	BYTE                 cbYaKou[2];                    //һ��Ѿ�ڵ�ֵ
	BYTE                 cbHuScore;                     //���Ƶķ���
	BYTE                 cbGoldCard;                    //���Ƶ�������
};

//��������������
struct CMD_TongCardItem
{
	BYTE				cbSiTongCount;					//�ĲصĴ���
	BYTE				cbSiTongCardData[6];			//�Ĳض�Ӧ��������

	BYTE				cbWuTongCount;					//��صĴ���
	BYTE				cbWuTongCardData[5];			//��ض�Ӧ��������
};

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				101									//��������
#define SUB_S_SEND_CARD				102									//�����˿�
#define SUB_S_OPERATE_NOTIFY		103									//������ʾ
#define SUB_S_OPERATE_RESULT		104									//��������
#define SUB_S_GAME_END				106									//��Ϸ����

#define SUB_S_HIDE  				107								    //��������
#define SUB_S_DECIDE_BANKER 		108									//��ׯ

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
#define SUB_S_HD					110									//������ʾ
#define SUB_S_ACTION                109                                 //ת������
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

//��ׯ�ṹ��
struct CMD_S_DecideBanker
{
	BYTE          bUserID;                  //�����Ƶ��û�
	BYTE          bCardData;                //���͵Ķ�ׯ������
};

//11-29
////////////////////babala
//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD				 wCurrentUser;					//��ǰ�ɲ������û�
	BYTE				 cbCurrentAction;				//��ǰ�û��ɽ��еĲ���

	WORD				 wBankerUser;					//ׯ���û�
	BYTE				 cbCardData[26];				//�˿�����

	BYTE                 cbCenterCard[8];               //8����ϵĲ�������
	BYTE                 cbWeaveKind[8];                //8����ϵĲ�������
	BYTE                 cbYaKou[2];                    //Ѿ�ڵ�ֵ
	BYTE                 cbHuScore;                     //Ѿ�ڵ�ֵ
	BYTE                 cbGoldCard;                    //Ѿ�ڵ�ֵ
};

/////////////////////babala
//���ƽṹ��
struct CMD_S_HIDE
{
	WORD				  wTongUserCount;				//������ҵĸ���
	BYTE				  cbUserAction;				    //�û�����
};

//��������
struct CMD_S_OperateResult
{
	WORD					wOperateUser;				//�����û�
	WORD					wProvideUser;				//��Ӧ�û�
	BYTE					cbOperateCode;				//������
	BYTE					cbOperateCard;				//�����˿�
	BYTE                    cbHuaCount;				    //���Ƹ���

	WORD				    wCurrentUser;					//��ǰ�ɲ������û�
	BYTE				    cbCurrentAction;					//��ǰ�û��ɽ��еĲ���

};

//��Ϸ״̬
struct CMD_S_StatusFree
{
	LONG							lCellScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONG					lCellScore;							//��Ԫ����
	WORD					wBankerUser;						//ׯ���û�
	WORD					wCurrentUser;						//��ǰ�û�

	//״̬����
	BYTE					cbActionCard;						//�����˿�
	BYTE					cbActionMask;						//��������

	//������Ϣ
	WORD					wOutCardUser;						//�����û�
	BYTE					cbOutCardData;						//�����˿�
	BYTE					cbDiscardCount[3];					//������Ŀ
	BYTE					cbDiscardCard[3][34];				//������¼

	//�˿�����
	BYTE					cbCardCount;						//�˿���Ŀ
	BYTE					cbCardData[36];						//�˿��б�
	BYTE                    cbUserCardCount[3];                 //�û��������������
	//����˿�
	BYTE					cbWeaveCount[3];					//�����Ŀ
	CMD_WeaveItem			WeaveItemArray[3][8];				//����˿�
	BYTE                    cbHuaWeaveCardIndex[3][5];          //������ϵĻ��Ƶĸ���

	BYTE                    bSiTongCount;                       //��ͳ�Ĵ���
	BYTE                    bSiTongIndex[8];                    //��ͳ������

	BYTE                    bWuTongCount;                       //��ͳ�Ĵ���
	BYTE                    bWuTongIndex[8];                    //��ͳ������

	BYTE					cbLeftCardCount;					//ʣ����Ŀ

	bool                    bGameStart;                         //�����Ϸ�Ƿ�ʼ
	WORD				    wFirstCurrentUser;					//��ǰ�ɲ������û�
	BYTE				    cbFirstCurrentAction;				//��ǰ�û��ɽ��еĲ���
	
	//�������,��Ϊ��֪����Ϣ���Ƿ��ܴ��ṹ�壬��˾ͰѺ�����ϲ𿪷Ž���
	BYTE                    cbCenterCard[8];                     //8����ϵĲ�������
	BYTE                    cbWeaveKind[8];                      //8����ϵĲ�������
	BYTE                    cbYaKou[2];                          //Ѿ�ڵ�ֵ
	BYTE                    cbHuScore;                           //Ѿ�ڵ�ֵ
	BYTE                    cbGoldCard;                          //Ѿ�ڵ�ֵ

};

//��������
struct CMD_S_OutCard
{
	WORD							wOutCardUser;						//�����û�
	BYTE							cbOutCardData;						//�����˿�
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE                            cbSendCount;                        //ÿ�η��͵�������
	BYTE							cbCardData[2];						//�˿�����
	BYTE							cbActionMask;						//��������
	WORD							wCurrentUser;						//��ǰ�û�
	bool							bIsHD;								//�Ƿ񺣵�

	//�������ܺ��Ƶ���Ϣ
	BYTE                            cbCenterCard[8];                     //8����ϵĲ�������
	BYTE                            cbWeaveKind[8];                      //8����ϵĲ�������
	BYTE                            cbYaKou[2];                          //һ��Ѿ�ڵ�ֵ
	BYTE                            cbHuScore;                           //���Ƶķ�ֵ
	BYTE                            cbGoldCard;                          //���Ƶ�������
};

//11-29
//������ʾ
struct CMD_S_OperateNotify
{
	WORD							wResumeUser;						//��ԭ�û�
	BYTE							cbActionMask;						//��������
	BYTE							cbActionCard;						//�����˿�

	//�������ܺ��Ƶ���Ϣ
	BYTE                            cbCenterCard[8];                     //8����ϵĲ�������
	BYTE                            cbWeaveKind[8];                     //8����ϵĲ�������
	BYTE                            cbYaKou[2];                         //Ѿ�ڵ�ֵ
	BYTE                            cbHuScore;                         //Ѿ�ڵ�ֵ
	BYTE                            cbGoldCard;                         //Ѿ�ڵ�ֵ
};


//��Ϸ����
struct CMD_S_GameEnd
{
	BYTE							cbChiHuCard;						//�Ժ��˿�
	WORD							wProvideUser;						//�����û�
	LONG							lGameScore[3];						//��Ϸ����

	BYTE							cbCardCount[3];						//�˿���Ŀ
	BYTE							cbCardData[3][36];					//�˿�����
	BYTE							bGoldCard;							//����
	LONG							lTax;								//��Ϸ˰��
	WORD							wChiHuUserCount;					//��������
	bool							bIsQuit;							//�Ƿ�ǿ��
	WORD                            wWinner;                            //�������
//lHuPoint
	LONG							lHuPoint; 						//������
	//�������йغ�����ϵ���Ϣ
	BYTE							cbCenterCard[8];					//8����ϵ�������
	BYTE							cbWeaveKind[8];						//8���������
	BYTE							cbYaKou[2];							//Ѿ��ֵ
	BYTE							cbHuaWeaveCount[5];					//������л��Ƶĸ���
	BYTE							cbHuaHandCount[5];					//�������л��Ƶĸ���

};


//������ʾ
struct CMD_S_HD
{
	WORD							wTargetUser;						//��һ���ĺ����û�
	bool							bIsCanHD;							//�ܷ�ĺ���	
	//BYTE							bHDCard;							//������
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CARD				1									//��������
#define SUB_C_OPERATE_CARD			2									//�����˿�

#define SUB_C_SORT_READY		    3							       //�������



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
#define SUB_C_HD			        4							//���ײ���
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////


//������ɽṹ��
struct CMD_C_SortReady
{
	WORD                            cbSortUserID;                                //����õ���Һ�
	BYTE							cbCardData[9][5];							//�źõ��˿�����
};


//��������
struct CMD_C_OperateCard
{
	BYTE							cbOperateCode;						//��������
	BYTE							cbOperateCard;						//�����˿�
};

//��������
struct CMD_C_OutCardHua
{

	BYTE							cbCardData;							//������˿�����
	BYTE                            cbUserHandData[9][5];                   //�û������ź����������
};


//��������
struct CMD_C_OutCard
{
	BYTE							  cbCardData;  						//�˿�����
};


//���ײ���
struct CMD_C_HD
{
	bool							cbHDAction;							//�Ƿ�Ҫ����
	WORD							wTargetUser;						//�����û�	
};

//////////////////////////////////////////////////////////////////////////

#endif