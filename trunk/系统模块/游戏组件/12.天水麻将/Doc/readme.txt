1��ɫ�ӡ������� ��2�Ρ�����
ׯ�ҡ�������
6+7��������

17*
14*��������

2�����ơ���������

3����ô��ׯ��������
������ׯ������

1�����������һ����������
2������Ĳ��ܺ�����������
�Լ�����Ĳ��ܺ���������

3.���ơ�����

4.ʣ10���ƣ��ͻ��ơ�������

5.����3�̾��⡣����


CARD_HEAP_DOUBLE_H.bmp
CARD_HEAP_DOUBLE_V.bmp

IDB_CARD_HEAP_DOUBLE_H

m_ImageHeapDoubleV

//�����齫
bool CHeapCard::SetCardData(WORD wMinusHeadCount, WORD wMinusLastCount, WORD wFullCount)

void CCardControl::SetBenchmarkPos(int nXPos, int nYPos, enXCollocateMode XCollocateMode, enYCollocateMode YCollocateMode)

					nXPos=m_ControlPoint.x;
					nYPos=m_ControlPoint.y+i*13;


				nXPos=m_ControlPoint.x;
					nYPos=m_ControlPoint.y+i*13;


//����ö��
enum enDirection
{
	Direction_East,					//����
	Direction_South,				//����
	Direction_West,					//����
	Direction_North,				//����
};


class CGameClientDlg : public CGameFrameDlg


	BYTE							m_cbLeftCardCount;					//ʣ����Ŀ
	
	//��ׯ����
	if (m_cbLeftCardCount==0)
		
		
	WORD							wSiceCount;		
	
	
						//���ӵ���		
						
						
						
	m_wSiceCount=MAKEWORD(rand()%6+1,rand()%6+1);
	
//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wSiceCount;							//���ӵ���
	
	
//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	LONG							lCellScore;							//��Ԫ����
	WORD							wSiceCount;							//���ӵ���
	
	
	
			StatusPlay.wSiceCount=m_wSiceCount;
			
			
LRESULT CGameClientDlg::OnShowSZ(WPARAM wParam, LPARAM lParam)


	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
				strTemp.Format("��Ϸ��ʼ");
				theApp.WriteLog(strFile, strTemp);
				CMD_S_GameStart * pGameStart=(CMD_S_GameStart *)pBuffer;
				cbSiceFirst=HIBYTE(pGameStart->wSiceCount) -1;
				cbSiceSecond=LOBYTE(pGameStart->wSiceCount) -1;

				memcpy( (char *)&myGameStart , (char *) pBuffer, wDataSize);
				PostMessage(IDM_SHOW_SZ,0,0);
				return true;
			//return OnSubGameStart(pBuffer,wDataSize);
		}
		
LRESULT CGameClientDlg::OnShowSZ(WPARAM wParam, LPARAM lParam)
{
	m_GameClientView.m_nDiceCount1 = cbSiceFirst;
	m_GameClientView.m_nDiceCount2 = cbSiceSecond;

	//strTemp.Format("m_nDiceCount1  =%d, m_GameClientView.m_nDiceCount2", m_GameClientView.m_nDiceCount1,m_GameClientView.m_nDiceCount2);
	//theApp.WriteLog(strFile, strTemp);
	//��������
	PlayGameSound(AfxGetInstanceHandle(),TEXT("GAME_START"));
	m_GameClientView.m_tDice = 1;
	AfxBeginThread( MyThreadProc, this);
	return 0;
}

//��ʾ�û�MJ
void CGameClientDlg::ShowUserMJ()
{
	m_GameClientView.m_tDice ++;
	m_GameClientView.UpdateGameView(NULL);

	OnSubGameStart(&myGameStart ,sizeof(myGameStart));
	
}

	int									m_nDiceCount1;					///������1
	int									m_nDiceCount2;					//������2
						
						
						
			m_wHeapHand=wTakeChairID;
			m_wHeapTail=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
			
			
	WORD							m_wHeapHand;						//����ͷ��
	WORD							m_wHeapTail;						//����β��
											
											
	WORD wTakeChairID=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
	m_wHeapTail=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
	
	
	
#define HEAP_FULL_COUNT				34	


//�齫����
#define HEAP_FULL_COUNT				34									//����ȫ��								//����ȫ��
34*4 = 136

17...

				
				
				//������Ŀ
				BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceSecond-1)*2:0);
				
		BYTE cbValidCount=HEAP_FULL_COUNT-m_cbHeapCardInfo[wTakeChairID][1]-((i==0)?(cbSiceSecond-1)*2:0);																								
		
		
IDB_CARD_HEAP_DOUBLE_H  BITMAP                  "res\\CARD_HEAP_DOUBLE_H.bmp"

	m_ImageHeapDoubleV.LoadFromResource(hInstance,IDB_CARD_HEAP_DOUBLE_V);
	m_ImageHeapDoubleH.LoadFromResource(hInstance,IDB_CARD_HEAP_DOUBLE_H);
	
	
				for (WORD i=wDoubleHeap;i<(wFinallyIndex+3);i++)
				{
					nXPos=(m_ControlPoint.x+ 50);
					nYPos=m_ControlPoint.y  +i*13;
					g_CardResource.m_ImageHeapDoubleV.AlphaDrawImage(pDC,nXPos,nYPos,RGB(255,0,255));
				}
				
	//�齫����
protected:
	WORD							m_wFullCount;						//ȫ����Ŀ
	WORD							m_wMinusHeadCount;					//ͷ����ȱ
	WORD							m_wMinusLastCount;					//β����ȱ
	
	
	CHeapCard						m_HeapCard[4];						//�����齫
	
	
		m_GameClientView.m_HeapCard[wViewChairID].SetCardData(m_cbHeapCardInfo[i][0],m_cbHeapCardInfo[i][1],HEAP_FULL_COUNT);

m_bHearStatus

bool CGameLogic::AnalyseCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, CAnalyseItemArray & AnalyseItemArray)

	//�����齫
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);


	//�����齫
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);

//�Ժ�����
BYTE CGameLogic::AnalyseChiHuCard(BYTE cbCardIndex[MAX_INDEX], tagWeaveItem WeaveItem[], BYTE cbWeaveCount, BYTE cbCurrentCard, WORD wChiHuRight, tagChiHuResult & ChiHuResult)

//�û�����
bool CTableFrameSink::OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)

	BYTE							m_cbCardIndex[GAME_PLAYER][MAX_INDEX];	//�û��齫

//.ʲô����������ƣ�����
1����ʼ���ơ�������
2�����ơ�����������
ACTION_EXPLAIN.bmp

IDB_ACTION_EXPLAIN      BITMAP                  "res\\ACTION_EXPLAIN.bmp"

	m_ImageActionExplain.SetLoadInfo(IDB_ACTION_EXPLAIN,hInstance);


	m_btTingCard.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()-10,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btGiveUp.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()*2-20,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btChiHu.SetWindowPos(NULL,rcRect.Width()-rcButton.Width()*3-30,nYPos,0,0,SWP_NOZORDER|SWP_NOSIZE);

//�����齫
void CControlWnd::SetControlInfo(BYTE cbCenterCard, BYTE cbActionMask, tagGangCardResult & GangCardResult)

	//��ť����
	m_btChiHu.EnableWindow(cbActionMask&WIK_CHI_HU);

#define WIK_CHI_HU					0x40								//�Ժ�����

0x01 & 0x40 = 

	AfxGetMainWnd()->PostMessage(IDM_CARD_OPERATE,WIK_TING_CARD,0);

	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
//�齫����
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)

	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	case SUB_C_OPERATE_CARD:	//������Ϣ

			return OnUserOperateCard(pUserData->wChairID,pOperateCard->cbOperateCode,pOperateCard->cbOperateCard);

1.ting ---->
2.

CControlWnd						m_ControlWnd;	

WIK_TING_CARD
OnUserOperateCard(WORD wChairID, BYTE cbOperateCode, BYTE cbOperateCard)

	//����
bool CGameClientDlg::OnSubTingCard(const void * pBuffer, WORD wDataSize)


	//�����齫
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,cbCardIndex,sizeof(cbCardIndexTemp));
	//�����齫
	AnalyseCard(cbCardIndexTemp,WeaveItem,cbWeaveCount,AnalyseItemArray);


AnalyseChiHuCard(m_cbCardIndex[i],m_WeaveItemArray[i],cbWeaveCount,cbCenterCard,wChiHuRight,ChiHuResult);



	//����齫
protected:
	BYTE							m_cbWeaveCount[GAME_PLAYER];		//�����Ŀ
	tagWeaveItem					m_WeaveItemArray[GAME_PLAYER][4];	//����齫


	WORD							m_wBankerUser;						//ׯ���û�
	WORD							m_wCurrentUser;						//��ǰ�û�

QQ:115891940
		

	WORD							m_wBankerUser;							//ׯ���û�