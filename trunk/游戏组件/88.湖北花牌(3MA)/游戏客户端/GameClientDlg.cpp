#include "Stdafx.h"
#include "GameClient.h"
#include "GameOption.h"
#include "GameClientDlg.h"
#include "gameclientdlg.h"

//////////////////////////////////////////////////////////////////////////

//��Ϸ��ʱ��
#define IDI_START_GAME				200									//��ʼ��ʱ��
#define IDI_OPERATE_CARD			202									//������ʱ��
#define IDI_HD					    203								    //���׶�ʱ��

//��Ϸ��ʱ��
#define TIME_START_GAME				25									//��ʼ��ʱ��
#define TIME_HEAR_STATUS			10									//���ƶ�ʱ��
#define TIME_OPERATE_CARD			25									//������ʱ��
#define TIME_HD						20									//���׶�ʱ��

const BYTE INVALID_CARDINDEX  =  0xFF;			//��Ч������

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CGameClientDlg, CGameFrameDlg)

	ON_MESSAGE(IDM_TUOGUAN,OnTuoGuan)
	ON_MESSAGE(IDM_DISTUOGUAN,OnDisTuoGuan)

	ON_MESSAGE(IDM_START,OnStart)
	ON_MESSAGE(IDM_OUT_CARD,OnOutCard)
	ON_MESSAGE(IDM_CARD_OPERATE,OnCardOperate)
	ON_WM_TIMER()
	ON_MESSAGE(IDM_SEND_ONE_CARD_FINISH,OnSendOneCardFinish)
	ON_MESSAGE(IDM_SEND_CARD_FINISH,OnSendCardFinish)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CGameClientDlg::CGameClientDlg() : CGameFrameDlg(&m_GameClientView)
{
	//��Ϸ����
	m_cbSex = 0;              //Ĭ�ϵ���Ů��
	m_IsTuoGuan=false;
	m_cbSendCardFinish=false;

	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	b_IsHD=false;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	m_SiTongCount =0;
	m_WuTongCount =0;
	
	FillMemory(m_WuTongIndex,sizeof(m_WuTongIndex),INVALID_CARDINDEX);
	FillMemory(m_SiTongIndex,sizeof(m_SiTongIndex),INVALID_CARDINDEX);

	ZeroMemory(m_cbHuaCardIndex,sizeof(m_cbHuaCardIndex));
	ZeroMemory(m_cbHuaWeaveCardIndex,sizeof(m_cbHuaWeaveCardIndex));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));

	m_bGameStart = false;
	m_cbActionMask = 0;
	m_cbActionCard = 0;
	return;
}

//��������
CGameClientDlg::~CGameClientDlg()
{
}

//��ʼ����
bool CGameClientDlg::InitGameFrame()
{
	//���ñ���
	SetWindowText(TEXT("������������������"));

	//����ͼ��
	HICON hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	SetIcon(hIcon,TRUE);
	SetIcon(hIcon,FALSE);

	//������Դ
	g_CardResource.LoadResource();
	//�õ��û����õĲ�������
	CWinApp * pApp=AfxGetApp();
	m_wPlayNativeSound=pApp->GetProfileInt(TEXT("GlobalOption"),TEXT("PlayNativeSound0"),1);

	return true;
}

//���ÿ��--������Ŀǰû�е��õ�--!!!!!!!!!!!!!!!!!1
void CGameClientDlg::ResetGameFrame()
{
	m_IsTuoGuan=false;

	//��Ϸ����
	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;
	//m_wHDCurrentUser=INVALID_CHAIR;
	b_IsHD=false;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//����˿�
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	///////////////babala11_12wan
	m_SiTongCount =0;
	m_WuTongCount =0;
	for (BYTE i=0; i<8; i++)
	{
		m_WuTongIndex[i] = INVALID_CARDINDEX;
		m_SiTongIndex[i] = INVALID_CARDINDEX;
	}

	ZeroMemory(m_cbHuaCardIndex,sizeof(m_cbHuaCardIndex));

	//-----11_16xia
	ZeroMemory(m_cbHuaWeaveCardIndex,sizeof(m_cbHuaWeaveCardIndex));
	ZeroMemory(m_bCardCount,sizeof(m_bCardCount));

	m_bGameStart = false;
	m_cbActionMask = 0;
	m_cbActionCard = 0;

	return;
}

//��Ϸ����
void CGameClientDlg::OnGameOptionSet()
{
	//��������
	CGameOption GameOption;
	GameOption.m_bEnableSound=IsEnableSound();

	//�õ��û����õĲ�������
	CWinApp * pApp=AfxGetApp();
	GameOption.m_bPlayNativeSound=pApp->GetProfileInt(TEXT("GlobalOption"),TEXT("PlayNativeSound0"),1);

	
	//��������
	if (GameOption.DoModal()==IDOK)
	{
		EnableSound(GameOption.m_bEnableSound);
	}
	m_wPlayNativeSound=GameOption.m_bPlayNativeSound;

	return;
}

//ʱ����Ϣ
bool CGameClientDlg::OnTimerMessage(WORD wChairID, UINT nElapse, UINT nTimerID)
{
	switch (nTimerID)
	{
	case IDI_START_GAME:		//��ʼ��Ϸ
		{
			if (nElapse==0)
			{
				OnStart(0,0);
				return true;
			}
			if (nElapse<=5)
			{
				if(IsEnableSound()==true)
				{
					CString szSoundName=_T("");
					szSoundName="Sound\\";
					szSoundName+="GAME_WARN.wav";
					PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
				}
			}
			DisableOutCard();
			return true;
		}
	case IDI_OPERATE_CARD:		//������ʱ��
		{
			//�й��ж�
			if((nElapse<=TIME_OPERATE_CARD-2) && m_IsTuoGuan && IsLookonMode()==false)
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if ((wChairID==wMeChairID) && (m_cbSendCardFinish==true))
				{
					//�޸�Ϊ�����һ����������Ϊ0��ʱ�����Ѱ�ҵ�һ����0�������������һ����
					if (m_wCurrentUser == wMeChairID)
					{
						BYTE cbCardData = 0;
						for(int i = 8; i >= 0; i--)
						{
							//�����ǰ����Ϊ�ҵ��ĵ�һ��
							if(m_GameClientView.m_HandCard[i].m_cbCardCount != 0)
							{
								//�õ���������
								cbCardData=m_GameClientView.m_HandCard[i].m_CardItemArray[m_GameClientView.m_HandCard[i].m_cbCardCount-1].cbCardData;				
								m_GameClientView.m_HandCard[i].DeleteByArrayIndex(m_GameClientView.m_HandCard[i].m_cbCardCount-1);
								break;
							}
						}

						OnOutCard(cbCardData,cbCardData);
					}
					else 
					{
						OnCardOperate(WIK_NULL,0);
					}
				}
				//��ֹ�û����Ʋ���
				DisableOutCard();
				return true;
			}
			//��ʱ�ж�
			if ((nElapse==0)&&(IsLookonMode()==false))
			{
				//��ȡλ��
				WORD wMeChairID=GetMeChairID();

				//��������
				if ((wChairID==wMeChairID) && (m_cbSendCardFinish==true))
				{
					//�����������Ʋ�����ʱ������£�������һ����
					if (m_wCurrentUser == wMeChairID)
					{
						//�ҵ�����һ����
						BYTE cbCardData = 0;
						for(int i = 8; i >= 0; i--)
						{
							//�����ǰ����Ϊ�ҵ��ĵ�һ��
							if(m_GameClientView.m_HandCard[i].m_cbCardCount != 0)
							{
								//�õ���������
								cbCardData=m_GameClientView.m_HandCard[i].m_CardItemArray[m_GameClientView.m_HandCard[i].m_cbCardCount-1].cbCardData;
								m_GameClientView.m_HandCard[i].DeleteByArrayIndex(m_GameClientView.m_HandCard[i].m_cbCardCount-1);

								//��ǰ�Ѿ��õ�Ҫ�����ƣ�����������ѭ��
								break;
							}
						}
						//�����һ���ƴ��ȥ
						OnOutCard(cbCardData,cbCardData);
					}
					//����Ǳ��˳��ƶ�����ʱ��������������ͷ�����Ϣ
					else 
					{
						OnCardOperate(WIK_NULL,0);
					}
				}

				//��ֹ�û����Ʋ���
				DisableOutCard();
				return true;
			}

			//��������
			if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
			{
				if(IsEnableSound()==true)
				{
					CString szSoundName=_T("");
					szSoundName="Sound\\";
					szSoundName+="GAME_WARN.wav";
					PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
				}
			}
			return true;
		}
	}

	return false;
}

//�Թ�״̬
void CGameClientDlg::OnLookonChanged(bool bLookonUser, const void * pBuffer, WORD wDataSize)
{
}

//������Ϣ
bool CGameClientDlg::OnGameMessage(WORD wSubCmdID, const void * pBuffer, WORD wDataSize)
{
	switch (wSubCmdID)
	{
	case SUB_S_DECIDE_BANKER:	//��ׯ
		{
			return OnSubGameDecideBanker(pBuffer,wDataSize);
		}
	case SUB_S_GAME_START:		//��Ϸ��ʼ
		{
			return OnSubGameStart(pBuffer,wDataSize);
		}
	case SUB_S_OUT_CARD:		//�û�����
		{
			return OnSubOutCard(pBuffer,wDataSize);
		}
	case SUB_S_SEND_CARD:		//������Ϣ
		{
			return OnSubSendCard(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_NOTIFY:	//������ʾ
		{
			return OnSubOperateNotify(pBuffer,wDataSize);
		}
	case SUB_S_OPERATE_RESULT:	//�������
		{
			return OnSubOperateResult(pBuffer,wDataSize);
		}
	case SUB_S_GAME_END:		//��Ϸ����
		{
			return OnSubGameEnd(pBuffer,wDataSize);
		}
	}

	return true;
}

//��Ϸ����
bool CGameClientDlg::OnGameSceneMessage(BYTE cbGameStation, bool bLookonOther, const void * pBuffer, WORD wDataSize)
{
	switch (cbGameStation)
	{
	case GS_MJ_FREE:	//����״̬
		{
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusFree)) return false;
			CMD_S_StatusFree * pStatusFree=(CMD_S_StatusFree *)pBuffer;

			//��������
			m_wBankerUser=pStatusFree->wBankerUser;
			m_GameClientView.SetCellScore(pStatusFree->lCellScore);

			//���ÿؼ�
			if (IsLookonMode()==false)
			{
				m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
				m_GameClientView.m_btStart.SetFocus();
				SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
			}

			//���½���
			m_GameClientView.UpdateGameView(NULL);

			return true;
		}
	case GS_MJ_PLAY:	//��Ϸ״̬
		{
//////////////////////////////////////////////////////////////////////
			//Ч������
			if (wDataSize!=sizeof(CMD_S_StatusPlay)) return false;
			CMD_S_StatusPlay * pStatusPlay=(CMD_S_StatusPlay *)pBuffer;

			//��������
			WORD wViewChairID[GAME_PLAYER]={0,0,0};
			for (WORD i=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);
//----------�ָ���صı���ֵ------------------------------//
			//����ׯ��
			m_wBankerUser=pStatusPlay->wBankerUser;
			
			//�õ��û����õĲ�������
			CWinApp * pApp=AfxGetApp();
			m_wPlayNativeSound=pApp->GetProfileInt(TEXT("GlobalOption"),TEXT("PlayNativeSound0"),1);

			//�ָ����û����е���
			CopyMemory(m_bCardCount,pStatusPlay->cbUserCardCount,sizeof(m_bCardCount));

			//�ָ��û��Ĳص�����
			m_SiTongCount = pStatusPlay->bSiTongCount;
			CopyMemory(m_SiTongIndex,pStatusPlay->bSiTongIndex,sizeof(m_SiTongIndex));

			//�ָ��û���ص�����
			m_WuTongCount = pStatusPlay->bWuTongCount;
			CopyMemory(m_WuTongIndex,pStatusPlay->bWuTongIndex,sizeof(m_WuTongIndex));

			//�ָ����û�������Ƶ�����
			CopyMemory(m_cbWeaveCount,pStatusPlay->cbWeaveCount,sizeof(m_cbWeaveCount));
			CopyMemory(m_WeaveItemArray,pStatusPlay->WeaveItemArray,sizeof(m_WeaveItemArray));
			//�ָ�����Լ���������л��Ƶĸ���
			CopyMemory(m_cbHuaWeaveCardIndex,pStatusPlay->cbHuaWeaveCardIndex[GetMeChairID()],sizeof(m_cbHuaWeaveCardIndex));
			
			//�ָ��Լ��û������Ƶ���Ϣ
			m_GameLogic.SwitchToCardIndex(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbCardIndex);
			//�ָ��Լ��û����л�������
			m_GameLogic.GetHuaCard(pStatusPlay->cbCardData,pStatusPlay->cbCardCount,m_cbHuaCardIndex);

			//�ָ����û��ѳ��Ƶ���Ϣ
			CopyMemory(m_cbDiscardCard,pStatusPlay->cbDiscardCard,sizeof(m_cbDiscardCard));
			CopyMemory(m_cbDiscardCount,pStatusPlay->cbDiscardCount,sizeof(m_cbDiscardCount));

			//��Ϸ�Ƿ��ƿ�ʼ״̬
			m_bGameStart = pStatusPlay->bGameStart;

			//�ָ���ǰ�û�����Ϣ
			m_wCurrentUser=pStatusPlay->wCurrentUser;

			//�ָ���ǰ���Ƶ��û���������Ϣ
			m_wOutCardUser=pStatusPlay->wOutCardUser;
			m_cbOutCardData=pStatusPlay->cbOutCardData;

			//�ָ���ǰ�û��Ĳ�����ʾ����������Ϣ
			m_cbActionMask=pStatusPlay->cbActionMask;
			m_cbActionCard=pStatusPlay->cbActionCard;

			//��������
			m_GameClientView.SetCellScore(pStatusPlay->lCellScore);
			m_GameClientView.SetBankerUser(wViewChairID[m_wBankerUser]);
//---------------------------���������ݵĻָ�--------------------------------------//
			m_cbLeftCardCount=pStatusPlay->cbLeftCardCount;
			//�õ���������ͷ���Ƶ�λ��
		//	m_GameClientView.m_leaveHeapCardData=(WORD)m_cbLeftCardCount;
			//m_GameClientView.m_LHeapCard.SetCardData(m_cbLeftCardCount,0,110);
			//�õ���������β���Ƶ�λ��
			BYTE wMinusLastCount=0;
			//�û�����ʱҪ��β������
			for(BYTE i=0;i<GAME_PLAYER;i++)
			{
				for(BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					if(m_WeaveItemArray[i][j].cbWeaveKind==WIK_SAN_ZHAO)
					{
						wMinusLastCount++;
					}
					else if(m_WeaveItemArray[i][j].cbWeaveKind==WIK_SI_ZHAO)
					{
						wMinusLastCount+=2;
					}
				}
			}
			//�û�����ʱ��ҲҪ��β������
			wMinusLastCount+=pStatusPlay->bSiTongAllCount;
			wMinusLastCount+=pStatusPlay->bWuTongAllCount;

			m_GameClientView.m_wMinusLastCount=wMinusLastCount==0?0:wMinusLastCount-1;

			//�õ���������ͷ���Ƶ�λ��
			m_GameClientView.m_wMinusHeadCount=pStatusPlay->cbLeftCardCount+m_GameClientView.m_wMinusLastCount+1;
			m_GameClientView.m_LHeapCard.SetCardData(m_GameClientView.m_wMinusHeadCount,m_GameClientView.m_wMinusLastCount,110);


//--------------------���������û������ƵĻָ�------------------------------//
			for (BYTE i=0; i<GAME_PLAYER; i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				//�¼��û����û��ָ�������
				if(wViewChairID==1)   continue;

				//�ǵ�ǰ�û��Ļ�������һ���Ƶ�����ʾ������
				m_bCardCount[i]=pStatusPlay->cbUserCardCount[i];
				BYTE cbCardCount=m_wCurrentUser==i?m_bCardCount[i]-1:m_bCardCount[i];
				m_GameClientView.m_UserCard[wViewChairID].SetCardData(cbCardCount,(m_wCurrentUser==i));
			}
//--------------------�����û���������ݵĻָ�----------------------//			
			for (WORD i=0; i<GAME_PLAYER; i++)
			{
				//��ʼ���������
				for (WORD j=0; j<8; j++)
				{
					m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
					m_GameClientView.m_WeaveCard[i][j].SetDisplayItem(true);
				}
			}			
			//��������˿�����
			BYTE cbWeaveCard[5];
			//��ϻ�������Ҫ���ݣ���ʹ�ù����н������޸�
			BYTE cbHuaWeaveCardIndex[3][5];
			CopyMemory(cbHuaWeaveCardIndex,pStatusPlay->cbHuaWeaveCardIndex,sizeof(cbHuaWeaveCardIndex));

			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				for (BYTE j=0;j<m_cbWeaveCount[i];j++)
				{
					BYTE cbWeaveKind=m_WeaveItemArray[i][j].cbWeaveKind;
					BYTE cbCenterCard=m_WeaveItemArray[i][j].cbCenterCard;

					BYTE index = m_GameLogic.SwitchToCardIndex(cbCenterCard);
					//����Ǿ��ƣ����뻨�Ƶĸ���
					if (index == 0 || index == 2 || index == 4 || index == 6 || index == 8)
					{
						BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,cbWeaveCard,cbHuaWeaveCardIndex[i][index/2]);
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					}
					else
					{
						BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbCenterCard,cbWeaveCard,0);
						m_GameClientView.m_WeaveCard[wViewChairID[i]][j].SetCardData(cbWeaveCard,cbWeaveCardCount);
					}
				}
			}
//---------------------�����û��ѳ������Ļָ�----------------------------//
			for (WORD i=0;i<GAME_PLAYER;i++)
			{
				WORD wViewChairID=SwitchViewChairID(i);
				m_GameClientView.m_DiscardCard[wViewChairID].SetCardData(m_cbDiscardCard[i],m_cbDiscardCount[i]);
			}
//--------------------���ڳ��Ƶ���ʾ�ָ�-------------------------//
			//�г����û��Ļ�����ʾ����Ƶ���Ϣ
			if (m_wOutCardUser!=INVALID_CHAIR)
			{
				WORD wOutChairID=SwitchViewChairID(m_wOutCardUser);
				m_GameClientView.SetOutCardInfo(wOutChairID,m_cbOutCardData);
			}
//-------------------�ؽ�����Ϸ�����й��Զ�ȡ��-----------------//
			m_IsTuoGuan=false;
			if(IsLookonMode()==false)
			{
				m_GameClientView.m_btTuoGuan.ShowWindow(SW_SHOW);
				m_GameClientView.m_btDisTuoGuan.ShowWindow(SW_HIDE);
			}
//--------------------�û��Լ����и������ƵĻָ�--------------------//
			//�����û��Ÿ����е�����
			StartSendAfterFace();

			//������û���ô��ֹ�����
			DisableOutCard();

//-------------��ԭ���Ƶ����̹���-------------------------------------//
			//�����������ڵ�һ�ֵĲ��ƽ׶�
			if(m_bGameStart==false)
			{
				m_cbFirstCurrentAction=pStatusPlay->cbFirstCurrentAction;
				m_cbFirstCurrentUser=pStatusPlay->wFirstCurrentUser;

				//���ײ����û�Ϊׯ�û������������ΪWIK_NULLʱ���������û��ɲ���������������Ϸ
				if((m_cbFirstCurrentUser==m_wBankerUser) && (m_cbFirstCurrentAction==WIK_NULL))
				{
					m_bGameStart = true;
					m_wCurrentUser=m_wBankerUser;
				}
				else
				{
					m_bGameStart = false;
					m_wCurrentUser=INVALID_CHAIR;
				}

				//�������ܲ����ƣ��������Լ��Ļ�����Ҫ��ʾ������ʾ
				if ((m_bGameStart==false) && (m_cbFirstCurrentUser==GetMeChairID()))
				{
					//��ǰ�û���ׯ�û������ܺ��ƵĻ������Ⱥ���
					if((GetMeChairID()==m_wBankerUser) && (m_cbFirstCurrentAction&WIK_CHI_HU)!=0)
					{
						BYTE cbHuScore = pStatusPlay->cbHuScore;
						BYTE cbGoldCard = m_GameLogic.SwitchToCardData(pStatusPlay->cbGoldCard);

						tagHuCardWeave HuCardweave;
						ZeroMemory(&HuCardweave,sizeof(HuCardweave));
						GetHuCardWeave(pStatusPlay->cbCenterCard,pStatusPlay->cbWeaveKind,pStatusPlay->cbYaKou,HuCardweave);

						m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,cbHuScore,cbGoldCard);
					}
					// ���û��в��Ʋ���ʱ������ʾ�û�������ʾ
					else if((m_cbFirstCurrentAction&WIK_SI_TONG)!=0 || (m_cbFirstCurrentAction&WIK_WU_TONG)!=0)
					{
						ShowTongOperateControl(m_cbFirstCurrentAction,0);
					}

					//��ʾ���ƶ�ʱ��
					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				}
		
				//�������ܲ����ƣ��������Լ��Ļ�����Ҫ��ʾ�ȴ��Ķ�ʱ����Ϣ
				if((m_bGameStart==false) && (m_cbFirstCurrentUser!=GetMeChairID()))
				{
					SetGameTimer(m_cbFirstCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);

					m_GameClientView.SetStatusFlag(false,true);
				}

				//��������
				if(IsEnableSound()==true)
				{
					CString szSoundName=_T("");
					szSoundName="Sound\\";
					szSoundName+="GAME_START.wav";
					PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
				}

				//��û���û�����ʱ������ׯ�ҳ��ơ���ʱ������ʾ��ѡ��Ҫ�����Ƽ�����
				if (m_bGameStart == true && m_wCurrentUser == m_wBankerUser) 
				{
					//����Լ��ǵ�ǰ�û���ô��ʾ�����
					if(GetMeChairID()==m_wBankerUser)
					{
						if(IsLookonMode()==false) EnableOutCard();

						m_GameClientView.SetStatusFlag(true,false);
					}
					//����Լ����ǵ�ǰ�û���ô��ʾ�Ե�
					else
					{
						m_GameClientView.SetStatusFlag(false,true);
					}

					SetGameTimer(m_wBankerUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				}

				return true;
			}
			else
			{
				//��ʱ�����������û�������,Ӧ�����û��Դ��ƽ��в���������û�в���֮ǰ�����
				if(m_wCurrentUser==INVALID_CHAIR)
				{
					//�����û��Լ�Ҳ�в���ʱ���������ʾ������ʾ
					if((IsLookonMode() == false) && m_cbActionMask!=WIK_NULL)
					{
						//������
						ActiveGameFrame();

						//����к��Ʋ�������������������£�����ʾ����������
						if((m_cbActionMask & WIK_CHI_HU) != 0)
						{
							//���ε����ƺ����ý���
							m_cbActionMask = (m_cbActionMask & 0x7F);

							BYTE hScore = pStatusPlay->cbHuScore;
							BYTE GoldCard = m_GameLogic.SwitchToCardData(pStatusPlay->cbGoldCard);

							tagHuCardWeave HuCardweave;
							ZeroMemory(&HuCardweave,sizeof(HuCardweave));
							GetHuCardWeave(pStatusPlay->cbCenterCard,pStatusPlay->cbWeaveKind,pStatusPlay->cbYaKou,HuCardweave);
							
							m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,hScore,GoldCard);
						}
						//���û�к��Ʋ��������--��Ҫ��������������е����
						else 
						{
							m_GameClientView.m_ControlWnd.SetDuiZhaoOperateCardData(m_cbActionCard,m_cbActionMask,m_cbHuaCardIndex);
							
							m_cbActionMask=0;
							m_cbActionCard=0;
						}
						//����������ʱ��
						SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);

						//���½���
						m_GameClientView.UpdateGameView(NULL);

						return true;
					}
				}
				//��ǰ�û����û��Լ������
				else if((m_wCurrentUser==GetMeChairID()) && (IsLookonMode() == false))
				{
					//�����û�����
					EnableOutCard();

					//��ǰ�û��в������ʱ�������û����ܺ��ƻ����
					if(m_cbActionMask!=WIK_NULL)
					{
						//���к��Ʋ���ʱ���Ƚ��к��Ʋ���������в��Ʋ���
						if ((m_cbActionMask&WIK_CHI_HU)!=0)
						{
							//���ε����ƺ����ý���
							m_cbActionMask = (m_cbActionMask & 0x7F);

							tagHuCardWeave HuCardweave;
							ZeroMemory(&HuCardweave,sizeof(HuCardweave));

							BYTE hScore = pStatusPlay->cbHuScore;
							BYTE GoldCard = m_GameLogic.SwitchToCardData(pStatusPlay->cbGoldCard);
							GetHuCardWeave(pStatusPlay->cbCenterCard,pStatusPlay->cbWeaveKind,pStatusPlay->cbYaKou,HuCardweave);
							m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,hScore,GoldCard);	
						}
						//�жϳ��˺�֮���Ƿ�����������
						else
						{
							ShowTongOperateControl(m_cbActionMask,0);
							m_cbActionMask = 0;
						}
					}
					//�������û����ƺ��޲��������֮�󣬸��û�������
					else
					{
						//��ʾ�û�����
						m_GameClientView.SetStatusFlag(true,false);
					}
					//����������ʱ��
					SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					//���½���
					m_GameClientView.UpdateGameView(NULL);

					return true;
				}
				//ֻ�������˲���ʱ�����öԷ��Ķ�ʱ��
				else if(m_wCurrentUser!=INVALID_CHAIR && m_wCurrentUser!=GetMeChairID())
				{
					//����������ʱ��
					SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					//��ʾ�û���ȴ�
					m_GameClientView.SetStatusFlag(false,true);
					//���½���
					m_GameClientView.UpdateGameView(NULL);

					return true;
				}

				return true;
			}
		}

	}
	return false;
}


bool CGameClientDlg::OnSubGameDecideBanker(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_DecideBanker));
	if (wDataSize!=sizeof(CMD_S_DecideBanker)) return false;

	//��������
	CMD_S_DecideBanker * pDecideBandker=(CMD_S_DecideBanker *)pBuffer;

	//�õ���ׯ���ݽṹ����ҵ���ͼID��
	WORD wOperateViewID = SwitchViewChairID(pDecideBandker->bUserID);

	m_GameClientView.m_bDecideBanker = true;
	m_GameClientView.m_DecideBankerCard[wOperateViewID]=pDecideBandker->bCardData;

	SetTimer(1,1000,NULL);
	//m_GameClientView.m_DecideBankerCard[pDecideBandker->bUserID]=pDecideBandker->bCardData;

	return true;
}

//�������û����е��ư�һ���Ĺ���ֵ���ͬ������
void CGameClientDlg::StartSendAfterFace()
{
	//���ñ���--�����û������ľŸ���
	for(BYTE k=0;k<9;k++)
	{
		m_GameClientView.m_HandCard[k].SetPositively(true);
		m_GameClientView.m_HandCard[k].SetCardData(NULL,0);

		m_GameClientView.m_HandCard[k].ShowWindow(SW_SHOW);
		m_GameClientView.m_HandCard[k].m_bDisplayItem=true;
	}

	BYTE cbCardAreaCount=0;         //�����ҵ��ķ�����
	BYTE cbCardAreaIndex[9][9];     //����ÿ���������Ƶ�����
	BYTE cbCardAreaCardCount[9];    //����ÿ���������Ƶĸ���
	ZeroMemory(cbCardAreaIndex,sizeof(cbCardAreaIndex));
	ZeroMemory(cbCardAreaCardCount,sizeof(cbCardAreaCardCount));

	//���û������Ʊ��ݵ���ʱ����������
	BYTE cbCardIndexTemp[MAX_INDEX];
	CopyMemory(cbCardIndexTemp,m_cbCardIndex,sizeof(cbCardIndexTemp));

	//1���ҳ�����3�Ż����ϵ��ƣ����䵥�����뵽һ������
	for(BYTE cbIndex=0;cbIndex<MAX_INDEX;cbIndex++)
	{
		if(cbCardIndexTemp[cbIndex]>=3)
		{
			//���ҵ�������Ƶ������ŵ�������
			for(BYTE cbCount=0;cbCount<cbCardIndexTemp[cbIndex];cbCount++)
			{
				cbCardAreaIndex[cbCardAreaCount][cbCount]=cbIndex;
			}
			//����˷������Ƶĸ���
			cbCardAreaCardCount[cbCardAreaCount]=cbCardIndexTemp[cbIndex];
			//�������Ƶĸ�����Ϊ0
			cbCardIndexTemp[cbIndex]=0;
			//ȷ���ķ�������1
			cbCardAreaCount++;
		}
	}

	//2���ֱ�����ϴ��ˡ���֪�񡢿����ѡ�����ǧ���˾��ӡ���ʮ��
	//�����������3�Ż����ϵģ�Ҳ�����ŵ�һ������
	//�����6���������Ӧ������ֵ
	BYTE cbTargetCardIndex[6][3]={{12,13,14},{18,19,20},{10,11,0},
								{2,16,17},{7,8,21},{6,9,15}};
	//��һɨ���6�������ֻҪ��������о���3�����ϵģ��͵�����������
	for(BYTE i=0;i<6;i++)
	{
		//�õ�����������Ƶ��ܸ���
		BYTE cbCount=cbCardIndexTemp[cbTargetCardIndex[i][0]]+cbCardIndexTemp[cbTargetCardIndex[i][1]]+cbCardIndexTemp[cbTargetCardIndex[i][2]];
		if(cbCount>=3)
		{
			BYTE cbTemp=0;  //��¼�˷������Ƶĸ���
			//��һ�鿴����������Ƶ����
			for(BYTE s=0;s<3;s++)
			{
				//�������Ƶ��������뵽����������
				for(BYTE t=0;t<cbCardIndexTemp[cbTargetCardIndex[i][s]];t++)
				{
					cbCardAreaIndex[cbCardAreaCount][cbTemp++]=cbTargetCardIndex[i][s];
				}
				//������������Ӧ�ĸ�����Ϊ0
				cbCardIndexTemp[cbTargetCardIndex[i][s]]=0;
			}
			//��¼�������Ƶĸ����ͷ�����
			cbCardAreaCardCount[cbCardAreaCount]=cbTemp;
			cbCardAreaCount++;
		}
	}

	//3�������ƣ���5��һ�����
	BYTE cbTemp=0;   //��¼�˷������Ƶĸ���
	for(BYTE cbIndex=0;cbIndex<MAX_INDEX;cbIndex++)
	{
		if(cbCardIndexTemp[cbIndex]>0)
		{
			//�������Ƶ��������뵽����������
			for(BYTE cbCount=0;cbCount<cbCardIndexTemp[cbIndex];cbCount++)
			{
				cbCardAreaIndex[cbCardAreaCount][cbTemp++]=cbIndex;
			}
			//������������Ӧ�ĸ�����Ϊ0
			cbCardIndexTemp[cbIndex]=0;
			
			//���˷����и����ﵽ5ʱ��������һ������
			if(cbTemp>=5)
			{
				cbCardAreaCardCount[cbCardAreaCount]=cbTemp;
				cbCardAreaCount++;
				cbTemp=0;
			}
		}
	}
	//��ѭ������ʱ��Ҫ�����ļ�����Ҳ�ŵ�һ��������
	if(cbTemp>0)
	{
		cbCardAreaCardCount[cbCardAreaCount]=cbTemp;
		cbCardAreaCount++;
	}
	ASSERT(cbCardAreaCount<=9);

	//���������������ʾ����
	BYTE cbCardData[9];
	BYTE cbHuaCardIndexTemp[5];
	CopyMemory(cbHuaCardIndexTemp,m_cbHuaCardIndex,sizeof(cbHuaCardIndexTemp));
	for(BYTE cbCount=0;cbCount<cbCardAreaCount;cbCount++)
	{
		ZeroMemory(cbCardData,sizeof(cbCardData));
		m_GameLogic.SwitchToCardData(cbCardAreaIndex[cbCount],cbCardAreaCardCount[cbCount],cbCardData,cbHuaCardIndexTemp);
		m_GameClientView.m_HandCard[cbCount].SetCardData(cbCardData,cbCardAreaCardCount[cbCount]);
	}

	////�Թ۽���
	//if (bPlayerMode==false)
	//{
	//	m_GameClientView.m_TableCard[1].SetCardData(NULL,0);
	//	m_GameClientView.m_DiscardCard[1].SetCardData(NULL,0);
	//	//��8������������
	//	for(BYTE cbCount=0;cbCount<8;cbCount++)
	//	{
	//		m_GameClientView.m_WeaveCard[1][cbCount].SetCardData(NULL,0);
	//	}
	//}
}

//���ͻ����յ��������˵�������Ϸ��ʼ��Ϣʱ������õ��˺���
bool CGameClientDlg::OnSubGameStart(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameStart));
	if (wDataSize!=sizeof(CMD_S_GameStart)) return false;
	//��������
	CMD_S_GameStart *pGameStart=(CMD_S_GameStart *)pBuffer;
	//����״̬
	SetGameStatus(GS_MJ_PLAY);

	//������صı���
	
	m_wBankerUser=pGameStart->wBankerUser;
	m_cbLeftCardCount=MAX_REPERTORY-GAME_PLAYER*25-1;

	if((pGameStart->wCurrentUser==m_wBankerUser) && (pGameStart->cbCurrentAction==WIK_NULL))
	{
		m_bGameStart = true;
		m_wCurrentUser=m_wBankerUser;
	}
	else
	{
		m_bGameStart = false;
		m_wCurrentUser=INVALID_CHAIR;
	}

	//��ֹ�û�����Ҽ�����
	DisableOutCard();
	if(IsLookonMode()==true)
	{
		//�˿�����
		for (BYTE i=0;i<GAME_PLAYER;i++)
		{
			m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
			m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
			
			for(BYTE j=0;j<8;j++)
			{
				m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
			}
		}
	}
	m_GameClientView.m_IsStartSendCard=true;
	//m_GameClientView.m_leaveHeapCardData=m_cbLeftCardCount;
	m_GameClientView.m_LHeapCard.SetCardData(34,0,34);
	m_GameClientView.m_wMinusHeadCount=34;
	m_GameClientView.m_wMinusLastCount=0;

	//�������ҵ��˿�����
	for (BYTE i=0; i<GAME_PLAYER; i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);
		if (i == m_wBankerUser) 
		{
			m_bCardCount[i] = 26;
		}
		else
		{
			m_bCardCount[i] = 25;
		}
	}

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;

	//�����˿�
	BYTE cbCardCount=(GetMeChairID()==m_wBankerUser)?26:25;

	//�õ���������
	BYTE count = m_GameLogic.GetHuaCard(pGameStart->cbCardData,cbCardCount,m_cbHuaCardIndex);

	//�������������������ݣ����浽��ǰ�������������������
	m_GameLogic.SwitchToCardIndex(pGameStart->cbCardData,cbCardCount,m_cbCardIndex);

	//���ý���
	bool bPlayerMode=(IsLookonMode()==false);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

	if(IsLookonMode()==false)
	{
		m_GameClientView.m_btTuoGuan.ShowWindow(SW_SHOW);
		m_GameClientView.m_btDisTuoGuan.ShowWindow(SW_HIDE);
	}

	m_GameClientView.SetBankerUser(SwitchViewChairID(m_wBankerUser));

	//�Թ�ģʽ
	if (bPlayerMode==false)
	{
		m_GameClientView.SetHuangZhuang(false);
		m_GameClientView.SetHD(false);
		//���ó��ƺ͵ȴ���־
		m_GameClientView.SetStatusFlag(false,false);
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
		m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);
		m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);
	}

	//��ʼ���������
	for (WORD i=0; i<GAME_PLAYER; i++)
	{
		for (WORD j=0; j<8; j++)
		{
			//��Ͻ���
			m_GameClientView.m_WeaveCard[i][j].SetDisplayItem(true);
		}
	}

	//�����û��Ÿ����е�����
	StartSendAfterFace();

	//�����������û���������
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		//��������
		WORD wViewChairID=SwitchViewChairID(i);
		//����1��Ϊ��ͼ�еĵ�ǰ�û�
		if (wViewChairID!=1)
		{
			m_GameClientView.m_UserCard[wViewChairID].SetCardData(CountArray(pGameStart->cbCardData),(i==m_wBankerUser));
		}
	}
	m_cbFirstCurrentAction=pGameStart->cbCurrentAction;
	m_cbFirstCurrentUser=pGameStart->wCurrentUser;
	//��ǰ�����û����Լ��������в����Ļ�������ʾ������ʾ�Ի���
	if ((m_bGameStart==false) && (pGameStart->wCurrentUser==GetMeChairID()) && IsLookonMode()==false)
	{
		//��ǰ�û���ׯ�û������ܺ��ƵĻ������Ⱥ���
		if((GetMeChairID()==m_wBankerUser) && (pGameStart->cbCurrentAction&WIK_CHI_HU)!=0)
		{
			m_cbActionCard=0;
			m_cbActionMask=pGameStart->cbCurrentAction;

			tagHuCardWeave HuCardweave;
			ZeroMemory(&HuCardweave,sizeof(HuCardweave));

			BYTE hScore = pGameStart->cbHuScore;
			BYTE GoldCard = m_GameLogic.SwitchToCardData(pGameStart->cbGoldCard);
			
			GetHuCardWeave(pGameStart->cbCenterCard,pGameStart->cbWeaveKind,pGameStart->cbYaKou,HuCardweave);
			m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,hScore,GoldCard);
	
			//��ʾ���ƶ�ʱ��
			SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
		// 
		else if((pGameStart->cbCurrentAction&WIK_SI_TONG)!=0 || (pGameStart->cbCurrentAction&WIK_WU_TONG)!=0)
		{
			m_cbActionCard=0;
			m_cbActionMask=pGameStart->cbCurrentAction;

			ShowTongOperateControl(m_cbActionMask,0);
				
			//��ʾ���ƶ�ʱ��
			SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}
	}

	//�������ܲ����ƣ��������Լ��Ļ�����Ҫ��ʾ�ȴ��Ķ�ʱ����Ϣ
	if((m_bGameStart==false) && (pGameStart->wCurrentUser!=GetMeChairID()))
	{
		SetGameTimer(pGameStart->wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);

		m_GameClientView.SetStatusFlag(false,true);
	}
	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//������
	ActiveGameFrame();

	//��������
	if(IsEnableSound()==true)
	{
		CString szSoundName=_T("");
		szSoundName="Sound\\";
		szSoundName+="GAME_START.wav";
		PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);

	}
	
	//��һ��û���˲��ƣ�����ʾ��ǰ�û�����
	if ((bPlayerMode==true)&&(m_wCurrentUser!=INVALID_CHAIR))
	{
		WORD wMeChairID=GetMeChairID();

		//���������������Ժ���ʾ
		if (m_wCurrentUser==wMeChairID)
		{
			m_GameClientView.SetStatusFlag(true,false);
		}
		else
		{
			m_GameClientView.SetStatusFlag(false,true);
		}
	}

	//��û���û�����ʱ������ׯ�ҳ��Ƽ���ʱ��
	if (m_bGameStart == true) 
	{
		if(GetMeChairID()==m_wBankerUser && IsLookonMode()==false) EnableOutCard();

		SetGameTimer(m_wBankerUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}
	return true;
}

bool CGameClientDlg::OnSubOutCard(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OutCard));
	if (wDataSize!=sizeof(CMD_S_OutCard)) return false;

	//��Ϣ����
	CMD_S_OutCard * pOutCard=(CMD_S_OutCard *)pBuffer;

	//��������
	WORD wMeChairID=GetMeChairID();
	WORD wOutViewChairID=SwitchViewChairID(pOutCard->wOutCardUser);

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	m_wOutCardUser=pOutCard->wOutCardUser;
	m_cbOutCardData=pOutCard->cbOutCardData;

	//�����û�����������
	m_bCardCount[m_wOutCardUser] -=1;

	//�����û�
	if ((IsLookonMode()==true)||(pOutCard->wOutCardUser!=wMeChairID))
	{
		//��������
		KillGameTimer(IDI_OPERATE_CARD); 
		if(IsEnableSound()==true)
		{
			m_cbSex = GetUserData(m_wOutCardUser)->cbGender;
			PlayCardSound(pOutCard->cbOutCardData);
		}
		//���ƽ���
		m_GameClientView.SetUserAction(INVALID_CHAIR,0);
		m_GameClientView.SetOutCardInfo(wOutViewChairID,pOutCard->cbOutCardData);

		//���ùۿ���ҵ��˿�
		if (GetMeChairID()==m_wOutCardUser)
		{
			
			////ɾ���˿�
			//BYTE cbCardData[MAX_COUNT];
			m_GameLogic.RemoveCard(m_cbCardIndex,pOutCard->cbOutCardData);
			
			//�ӾŸ����������н���ɾ��
			for(BYTE AreaIndex=0;AreaIndex<9;AreaIndex++)
			{
				//��һ����ÿһ�������е�ÿһ����
				bool bDelete=false;
				for(BYTE cbCardIndex=0;cbCardIndex<m_GameClientView.m_HandCard[AreaIndex].m_cbCardCount;cbCardIndex++)
				{
					if(m_GameClientView.m_HandCard[AreaIndex].m_CardItemArray[cbCardIndex].cbCardData == pOutCard->cbOutCardData)
					{
						m_GameClientView.m_HandCard[AreaIndex].DeleteByArrayIndex(cbCardIndex);
						bDelete=true;
						break;  //�����������Ĳ��ҹ���
					}
				}
				if(bDelete==true) break;  //��ǰ���������Ƶ�ɾ������
			}
		}
		else
		{
			//WORD wUserIndex=(wOutViewChairID>2)?2:wOutViewChairID;
			m_GameClientView.m_UserCard[wOutViewChairID].SetCurrentCard(false);
		}
	}

	//������Թ��û���ô��ֹ�����
	if((IsLookonMode()==true))
	{
		//AfxMessageBox("�����Թ��û�������ֹ������");
		DisableOutCard();
	}
	m_GameClientView.SetStatusFlag(false,false);
	return true;
}

//������Ϣ
bool CGameClientDlg::OnSubSendCard(const void * pBuffer, WORD wDataSize)
{

	ASSERT(wDataSize==sizeof(CMD_S_SendCard));
	if (wDataSize!=sizeof(CMD_S_SendCard)) return false;

	//��������
	CMD_S_SendCard * pSendCard=(CMD_S_SendCard *)pBuffer;

	//������Թ��û���ô��ֹ�����
	if((IsLookonMode()==true))
	{
		DisableOutCard();
	} 

	//��һ�ֲ��ƹ����еķ��ƣ�һ���ǲ���
	if(m_bGameStart==false)
	{
		m_bTongCard=true;
	}
	//������Ϸ��ʼ��,����ǰ�û���ָ����ʱ,��һ�����û������Ƶĵ���----ֻҪ���˳����ƣ������յ�ǰ�û���
	else if(m_bGameStart==true && m_wCurrentUser==INVALID_CHAIR)
	{
		m_bTongCard=false;
	}
	//������Ϸ��ʼ��,����ǰ�û���ȷ�����û�ʱ,��һ�����û�����֮��Ĳ��Ƶĵ���
	else if(m_bGameStart==true && m_wCurrentUser!=INVALID_CHAIR)
	{
		m_bTongCard=true;
	}

	//�����˿�--�����ϸ��û����Ƶ���Ϣ
	if ((m_wOutCardUser!=INVALID_CHAIR)&&(m_cbOutCardData!=0))
	{
		//�����˿�
		WORD wOutViewChairID=SwitchViewChairID(m_wOutCardUser);
		m_GameClientView.m_DiscardCard[wOutViewChairID].AddCardItem(m_cbOutCardData);
		
		//���ñ���
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}

	//���뷢����һ����������ݱ�������
	m_cbSendCardFinish=false;
	m_wCurrentUser=pSendCard->wCurrentUser;

	//�Ǻ��׵����ʱ������������
	b_IsHD=pSendCard->bIsHD;
	if(b_IsHD==true)
	{
		m_bTongCard=false;
	}

	m_cbSendCount=pSendCard->cbSendCount;
	CopyMemory(m_cbSendCardData,pSendCard->cbCardData,sizeof(m_cbSendCardData));

	m_cbActionMask=pSendCard->cbActionMask;

	WORD wMeChairID=GetMeChairID();
	WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);

	//��ǰ�����û����Լ��������ܺ��ƵĻ�����Ҫ������Ƶ�����
	if ((m_wCurrentUser==wMeChairID) && ((m_cbActionMask&WIK_CHI_HU)!=0))
	{
		m_cbHuScore = pSendCard->cbHuScore;
		m_cbGoldCard = m_GameLogic.SwitchToCardData(pSendCard->cbGoldCard);

		CopyMemory(m_cbYaKou,pSendCard->cbYaKou,sizeof(m_cbYaKou));
		CopyMemory(m_cbCenterCard,pSendCard->cbCenterCard,sizeof(m_cbCenterCard));
		CopyMemory(m_cbWeaveKind,pSendCard->cbWeaveKind,sizeof(m_cbWeaveKind));
	}


//---------------���µĴ������𶯷��ƵĶ�����ʾ-----------------------
	if(IsLookonMode()==false)
	{
		CPoint ptStartPoint;
		CPoint ptTargetPoint[GAME_PLAYER];

		WORD nWidth=761;
		WORD nHeight=705;

		CRect rect;
		m_GameClientView.GetClientRect(&rect);

		ptTargetPoint[1].x=rect.Width()/2;
		ptTargetPoint[1].y=rect.Height()-200;
		ptTargetPoint[0].x=rect.Width()-200;
		ptTargetPoint[0].y=rect.Height()/2-80;
		ptTargetPoint[2].x=100;
		ptTargetPoint[2].y=rect.Height()/2-80;

		//�������ƶ���
		if(m_bTongCard==true)   //���ƶ�������,���Ǵ�β���õ�
		{
			for(WORD i=0;i<pSendCard->cbSendCount;i++)
			{
				WORD wMinusLastCount=m_GameClientView.m_wMinusLastCount;
				ptStartPoint.x=0+32+wMinusLastCount*6+198;			
				ptStartPoint.y=0+30+54;

				if(m_wCurrentUser==wMeChairID)
				{
					//ptTargetPoint[1].x=600;
					//ptTargetPoint[1].y=570;
					m_GameClientView.DispatchUserCard(1,ptStartPoint,ptTargetPoint[1],pSendCard->cbCardData[i],false);
				}
				else
				{
					//if(wViewChairID==0)
					//{
					//	BYTE cbCardCount=m_bCardCount[wMeChairID]-1;

					//	ptTargetPoint[0].x=nWidth-172+24;
					//	ptTargetPoint[0].y=nHeight/2-310+75+cbCardCount*12;
					//}
					//if(wViewChairID==2)
					//{
					//	BYTE cbCardCount=m_bCardCount[wMeChairID]-1;

					//	ptTargetPoint[2].x=100+90-65-24;
					//	ptTargetPoint[2].y=nHeight/2+160-cbCardCount*12-92;
					//}
					m_GameClientView.DispatchUserCard(wViewChairID,ptStartPoint,ptTargetPoint[wViewChairID],0,false);
				}
			}
		}
		else                 //������������,���Ǵ�ͷ���õ�
		{
			WORD wMinusHeadCount=m_GameClientView.m_wMinusHeadCount;
			ptStartPoint.x=0+32+54+wMinusHeadCount*6;			
			ptStartPoint.y=0+30+54;

			if(m_wCurrentUser==wMeChairID)
			{
				//ptTargetPoint[1].x=600;
				//ptTargetPoint[1].y=570;
				m_GameClientView.DispatchUserCard(1,ptStartPoint,ptTargetPoint[1],pSendCard->cbCardData[0],true);
			}
			else
			{
				//if(wViewChairID==0)
				//{
				//	BYTE cbCardCount=m_bCardCount[wMeChairID]-1;

				//	ptTargetPoint[0].x=nWidth-172+24;
				//	ptTargetPoint[0].y=nHeight/2-310+75+cbCardCount*12;
				//}
				//if(wViewChairID==2)
				//{
				//	BYTE cbCardCount=m_bCardCount[wMeChairID]-1;

				//	ptTargetPoint[2].x=100+90-65-24;
				//	ptTargetPoint[2].y=nHeight/2+160-cbCardCount*12-92;
				//}
				m_GameClientView.DispatchUserCard(wViewChairID,ptStartPoint,ptTargetPoint[wViewChairID],0,true);
			}
		}
	}


	return true;
/////////////////////////////////////////////////////////////////////////////
}

//������ʾ
bool CGameClientDlg::OnSubOperateNotify(const void * pBuffer, WORD wDataSize)
{
	//AfxMessageBox("CGameClientDlg::OnSubOperateNotify");
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_OperateNotify));
	if (wDataSize!=sizeof(CMD_S_OperateNotify)) return false;

	//��������
	CMD_S_OperateNotify * pOperateNotify=(CMD_S_OperateNotify *)pBuffer;

	//����Ա��˴�����ƣ�������������ͬ�û��Դ��Ƶ�������Ϣ�����ҷֱ��͸���Ӧ���û�
	//�ܵ��˴��Ļ���˵�����û��ܶԴ��ƽ��к������л����еĲ�����
	if ((IsLookonMode()==false)&&(pOperateNotify->cbActionMask!=WIK_NULL))
	{
		m_cbSendCardFinish=true;

		if(m_IsTuoGuan)
		{
			OnCardOperate(WIK_NULL,0);
			return true;
		}

		//�����Ƽ�Ϊ�����������
		m_cbActionMask=pOperateNotify->cbActionMask;
		m_cbActionCard=pOperateNotify->cbActionCard;

		//����к��Ʋ�������������������£�����ʾ����������
		if ((m_cbActionMask & WIK_CHI_HU) != 0)
		{
			//���ε����ƺ����ý���
			m_cbActionMask = (m_cbActionMask & 0x7F);

			BYTE hScore = pOperateNotify->cbHuScore;
			BYTE GoldCard = m_GameLogic.SwitchToCardData(pOperateNotify->cbGoldCard);

			tagHuCardWeave HuCardweave;
			ZeroMemory(&HuCardweave,sizeof(HuCardweave));
			GetHuCardWeave(pOperateNotify->cbCenterCard,pOperateNotify->cbWeaveKind,pOperateNotify->cbYaKou,HuCardweave);
			
			m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,hScore,GoldCard);
		}
		//���û�к��Ʋ��������--��Ҫ��������������е����
		else 
		{
			m_GameClientView.m_ControlWnd.SetDuiZhaoOperateCardData(m_cbActionCard,m_cbActionMask,m_cbHuaCardIndex);
			
			m_cbActionMask=0;
			m_cbActionCard=0;
		}
		ActiveGameFrame();

		SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		m_GameClientView.SetStatusFlag(false,false);
	}

	//������Թ��û���ô��ֹ�����
	if((IsLookonMode()==true))
	{
		//AfxMessageBox("�����Թ��û�������ֹ������");
		DisableOutCard();
	}
	m_GameClientView.UpdateGameView(NULL);
	return true;
}

//�������--���������صĲ�������Ĵ���
bool CGameClientDlg::OnSubOperateResult(const void * pBuffer, WORD wDataSize)
{
	//Ч����Ϣ
	ASSERT(wDataSize==sizeof(CMD_S_OperateResult));
	if (wDataSize!=sizeof(CMD_S_OperateResult)) return false;

	//��Ϣ����
	CMD_S_OperateResult * pOperateResult=(CMD_S_OperateResult *)pBuffer;

	//��������
	BYTE cbPublicCard=TRUE;   //����Ƿ�ȫ��������ʾ
	WORD wOperateUser=pOperateResult->wOperateUser;
	BYTE cbOperateCard=pOperateResult->cbOperateCard;
	WORD wOperateViewID=SwitchViewChairID(wOperateUser);

	//���Ʊ���
	if (pOperateResult->cbOperateCode!=WIK_NULL)
	{
		m_cbOutCardData=0;
		m_wOutCardUser=INVALID_CHAIR;
	}
		//������Թ��û���ô��ֹ�����
	if((IsLookonMode()==true))
	{
		//AfxMessageBox("�����Թ��û�������ֹ������");
		DisableOutCard();
	}

	//���в��ƴ���
	if ((pOperateResult->cbOperateCode & WIK_SI_TONG) != 0 || (pOperateResult->cbOperateCode & WIK_WU_TONG)!=0 || (pOperateResult->cbOperateCode & WIK_TONG_GIVE_UP)!=0 )
	{
		//1.������ͳ����--ֻ�п������û����������ŵ����
		if ((pOperateResult->cbOperateCode & WIK_SI_TONG) != 0)
		{
			//ֻ���ǵ�ǰ�����û������ܱ����Լ����Ĳ����
			if(GetMeChairID()==wOperateUser)
			{
				m_SiTongIndex[m_SiTongCount++] = m_GameLogic.SwitchToCardIndex(cbOperateCard);
			}
			WORD ViewChairID=SwitchViewChairID(wOperateUser);
			m_GameClientView.m_cbTongCount[ViewChairID]++;
		}
		//������ͳ����--�����������һ���û����������ƵĲز������������к�����Ƶ���ز���
		else if ((pOperateResult->cbOperateCode & WIK_WU_TONG) != 0)
		{
			//�õ���������Ƶ�����
			BYTE cbOperateindex = m_GameLogic.SwitchToCardIndex(cbOperateCard);

			//1���ж��Ƿ�����ϵ����--����ص�������������
			BYTE cbWeaveIndex = 0xFF;
			//�Բ����û������в������з����ж��Ƿ�����ϲ���
			for (BYTE i=0;i<m_cbWeaveCount[wOperateUser];i++)
			{
				BYTE cbWeaveKind=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
				BYTE cbCenterCardIndex = m_GameLogic.SwitchToCardIndex(m_WeaveItemArray[wOperateUser][i].cbCenterCard );
				//�û�����ֻ��1���ƣ�����������й����еĻ�������ʵ�ֽ���ص�������������
				if ((m_cbCardIndex[cbOperateindex]==1)&&(cbOperateindex == cbCenterCardIndex) && (cbWeaveKind==WIK_SAN_ZHAO))
				{
					cbWeaveIndex=i;
					//�޸Ĵ���ϵ������Ϣ
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=WIK_SI_ZHAO;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;
					
					break;
				}
			}

			//����û�����ֻ��һ���ƣ������������������������ͬ����
			if (cbWeaveIndex != 0xFF && m_cbCardIndex[cbOperateindex] == 1)
			{
				//��Ӧ��ҵ�������1
				m_bCardCount[wOperateUser] -= 1;

				//�����һ�������壬�ߣ���
				BYTE bHuaCard = (cbOperateindex == 0 || cbOperateindex == 2 || cbOperateindex == 4 || cbOperateindex == 6 || cbOperateindex == 8)?2:0;
				
				//�޸���Ͻ������ʾ���
				BYTE cbTongCard[5]={0,0,0,0,0},cbTongKind=WIK_SI_ZHAO;
				//�õ��������ƣ����ز������˿�����
				BYTE cbTongCardCount=m_GameLogic.GetWeaveCard(cbTongKind,cbOperateCard,cbTongCard,bHuaCard);
				//�������ó����û��������
				m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbTongCard,cbTongCardCount);	
				//������ƵĴ���
				WORD ViewChairID=SwitchViewChairID(wOperateUser);
				if(GetMeChairID()==wOperateUser)
				{
					m_GameClientView.m_cbTongCount[ViewChairID]++;
				}
				//ɾ���˿�
				if (GetMeChairID()==wOperateUser)
				{
					//�����һ�������壬�ߣ��ţ�����Ӧλ�õĻ�������Ϊ0
					if (cbOperateindex == 0 || cbOperateindex == 2 || cbOperateindex == 4 || cbOperateindex == 6 || cbOperateindex == 8)
					{
						m_cbHuaWeaveCardIndex[cbOperateindex/2] = 2;
						m_cbHuaCardIndex[cbOperateindex/2] = 0;
					}
					m_cbCardIndex[cbOperateindex] = 0;

					bool bDelete = false;
					//�������������ҵ��������ڵ����򣬲��Ҵ���ɾ��
					for (BYTE i=0; i<9; i++)
					{
						//ɨ��ÿ���������������
						for (BYTE j=0; j<m_GameClientView.m_HandCard[i].m_cbCardCount; j++)
						{
							if (m_GameLogic.SwitchToCardIndex(m_GameClientView.m_HandCard[i].m_CardItemArray[j].cbCardData) == cbOperateindex)
							{
								m_GameClientView.m_HandCard[i].DeleteCardData(m_GameClientView.m_HandCard[i].m_CardItemArray[j].cbCardData);
								bDelete = true;
								break;
							}
						}
						if(bDelete == true) break;
					}
				}
			}
			//���û����е���ʵ����ز���
			else
			{
				BYTE index=m_GameLogic.SwitchToCardIndex(cbOperateCard);

				if(GetMeChairID()==wOperateUser)
				{
					m_WuTongIndex[m_WuTongCount++] = index;

					//�²��޸ģ��������Ƶ�������������
				}
				bool IsSiTong=false;
				WORD ViewChairID=SwitchViewChairID(wOperateUser);

				for(BYTE j=0;j<m_SiTongCount;j++)
				{
					if(m_SiTongIndex[j]==index)
					{
						//
						m_GameClientView.m_cbTongCount[ViewChairID]++;
						IsSiTong=true;

					}
				}
				if(IsSiTong==false)
				{
					m_GameClientView.m_cbTongCount[ViewChairID]+=2;
				}

			}
		}

		//����ǵ�һȦͳ��ʲôҲ������ֻ����ʾͳ�Ʊ��֪ͨ�������
		if (m_bGameStart == false)
		{
			KillGameTimer(IDI_OPERATE_CARD);
			//ͳ�����,����ׯ�Ҷ�ʱ��
			if ((pOperateResult->wCurrentUser==m_wBankerUser) && (pOperateResult->cbCurrentAction==WIK_NULL))
			{
				m_bGameStart = true;
				m_wCurrentUser=m_wBankerUser;
				if(GetMeChairID()==m_wBankerUser) 
				{
					//����ׯ�ҳ��Ʋ���
					EnableOutCard();
					//��ʾׯ�ҿ�ʼ����
					m_GameClientView.SetStatusFlag(true,false);
				}

				//���ز�����ʾ����
				m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

				//����ׯ���û���˵��Ҳ���������˺�����ʾ��
				m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

				//��ʾ���Ʊ�־
				m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
				
				//���½���
				m_GameClientView.UpdateGameView(NULL);

				//��������
				if(IsEnableSound()==true)
				{
					m_cbSex = GetUserData(pOperateResult->wOperateUser)->cbGender;
					PlayActionSound(pOperateResult->cbOperateCode);
				}
			
					SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);

				return true;
			}

			//��һ�ֵ�ͳ��û���,���������ͻ�������ʱ,ֻ��ʾ�û��Ķ��� ������������
			m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);

			//���ڱ������û���˵�����������Ĳػ���ز������õ��ƺ󣬻����ܽ��в��ƵĲ���
			if(((pOperateResult->cbCurrentAction & WIK_SI_TONG) != 0 || (pOperateResult->cbCurrentAction & WIK_WU_TONG)!=0) && IsLookonMode()==false)
			{
				//���û��ǲ��Ʋز���ʱ���ŵ����ƴ������˴����˴����Ǵ����л��û������еĲ��ƴ���
				if(pOperateResult->wCurrentUser!=pOperateResult->wOperateUser)
				{
					if(GetMeChairID()==pOperateResult->wCurrentUser)
					{
						m_cbActionMask=pOperateResult->cbCurrentAction;

						m_GameClientView.SetStatusFlag(false,false);
						ShowTongOperateControl(m_cbActionMask,0);
						
						SetGameTimer(pOperateResult->wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					}
					else
					{
						m_GameClientView.SetStatusFlag(false,true);
						SetGameTimer(pOperateResult->wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
					}
				}
			}
			//���½���
			m_GameClientView.UpdateGameView(NULL);

			//��������
			if(IsEnableSound()==true)
			{
				m_cbSex = GetUserData(pOperateResult->wOperateUser)->cbGender;
				PlayActionSound(pOperateResult->cbOperateCode);
			}
			return true;
		}
		
		if ((pOperateResult->cbOperateCode & WIK_TONG_GIVE_UP)!=0)  return true;
	}
	//���ƣ����Ʋ���
	else if (pOperateResult->cbOperateCode != WIK_NULL)
	{
		//���ñ���
		m_wCurrentUser=pOperateResult->wOperateUser;

		//���������
		BYTE cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicCard=TRUE;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterCard=cbOperateCard;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult->cbOperateCode;
		m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult->wProvideUser;

		//����ƽ���
		BYTE cbWeaveCard[5]={0,0,0,0,0},cbWeaveKind=pOperateResult->cbOperateCode;
		BYTE cbWeaveCardCount=m_GameLogic.GetWeaveCard(cbWeaveKind,cbOperateCard,cbWeaveCard,pOperateResult->cbHuaCount);

		//��Ӧ��ҵ���������
		m_bCardCount[wOperateUser] -= cbWeaveCardCount-1;

		//���ó����û��������
		m_GameClientView.m_WeaveCard[wOperateViewID][cbWeaveIndex].SetCardData(cbWeaveCard,cbWeaveCardCount);

		//ɾ���˿�
		if (GetMeChairID()==wOperateUser)
		{/////////////////////////////����ĵ�4.13///////////////////
			if((IsLookonMode()==false) && (pOperateResult->cbOperateCode&WIK_DUI)!=0)	EnableOutCard();
			//�ӵ�ǰ��ҵ���ʱ�������ɾ��һ�ű����ṩ���ƣ��ٴ��Լ�����������������ɾ�����������
			BYTE bDelet = 0;
			//1��ɾ��������У����˴������������
			for (BYTE i=0; i<cbWeaveCardCount; i++)
			{
				if (cbWeaveCard[i] == cbOperateCard)
				{
					bDelet++;
					cbWeaveCard[i] = 0;
					//�����������ǰ��
					for (BYTE j=i; j<cbWeaveCardCount-1; j++)
					{
						cbWeaveCard[j] = cbWeaveCard[j+1];
					}
					//�����һ��Ϊ0
					cbWeaveCard[j] = 0;
					break;
				}
			}
			//2�����Լ�������������������ɾ����������Լ����е���
			m_GameLogic.RemoveCard(m_cbCardIndex,cbWeaveCard,cbWeaveCardCount-1);

			//3������������Ļ�������Ϊ0���ӵ�ǰ������л��������٣��ӵ���ϻ�����
			if (pOperateResult->cbHuaCount != 0)
			{
				BYTE index = m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard);
				m_cbHuaWeaveCardIndex[index/2] += pOperateResult->cbHuaCount;
				
				//��ʵ������������ʱ���Ǿ������Լ����еĻ���ȥ�б��˵���
				m_cbHuaCardIndex[index/2] = 0;
			}

			//4���ж���ִ�еĲ����Ƿ��ǲ����Ĳص���
			if ((pOperateResult->cbOperateCode & WIK_SI_ZHAO) != 0)
			{
				for (BYTE i=0; i<8;i++)
				{
					if (m_GameLogic.SwitchToCardIndex(pOperateResult->cbOperateCard) == m_SiTongIndex[i])
					{
					//	m_SiTongIndex[i] = INVALID_CARDINDEX;
						m_SiTongIndex[i] =(BYTE)(-1);
						m_SiTongCount--;
						break;
					}
				}
			}
			//5���Ӳ���������ɾ����ϵ���
		    
			////ÿ��ѭ����ɾ�����е�һ����
			for(BYTE cbDeletIndex=0;cbDeletIndex<(cbWeaveCardCount-1);cbDeletIndex++)
			{
				//�õ�����Ҫɾ������
				BYTE cbDeleteCardData=cbWeaveCard[cbDeletIndex];
				//�ӾŸ����������н���ɾ��
				for(BYTE AreaIndex=0;AreaIndex<9;AreaIndex++)
				{
					//��һ����ÿһ�������е�ÿһ����
					bool bDelete=false;
					for(BYTE cbCardIndex=0;cbCardIndex<m_GameClientView.m_HandCard[AreaIndex].m_cbCardCount;cbCardIndex++)
					{
						if(m_GameClientView.m_HandCard[AreaIndex].m_CardItemArray[cbCardIndex].cbCardData == cbDeleteCardData)
						{
							m_GameClientView.m_HandCard[AreaIndex].DeleteByArrayIndex(cbCardIndex);
							bDelete=true;
							break;  //�����������Ĳ��ҹ���
						}
					}
					if(bDelete==true) break;  //��ǰ���������Ƶ�ɾ������
				}
			}
		}
		else
		{
			m_GameClientView.m_UserCard[wOperateViewID].SetCardData(m_bCardCount[wOperateUser]-1,true);
		}
	}

	//����ǵ�ǰ��ҵĲ�������ʱ�������ز�����ʾ
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);

	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);
	m_GameClientView.SetUserAction(wOperateViewID,pOperateResult->cbOperateCode);
	//��������û����Լ���ô��ʾ�����

	if(m_wCurrentUser==GetMeChairID())
	{
		m_GameClientView.SetStatusFlag(true,false);
	}
	//����Լ����ǲ����û���ô��ʾ��ȴ�
	else
	{
		m_GameClientView.SetStatusFlag(false,true);
	}
	//���½���
	m_GameClientView.UpdateGameView(NULL);

	//��������
	if(IsEnableSound()==true)
	{
		m_cbSex = GetUserData(pOperateResult->wOperateUser)->cbGender;
		PlayActionSound(pOperateResult->cbOperateCode);
	}

	//����ʱ��
	if (m_wCurrentUser!=INVALID_CHAIR )
	{
		//����ʱ��
		SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
	}

	return true;
}



bool CGameClientDlg::OnSubGameEnd(const void * pBuffer, WORD wDataSize)
{
	//Ч������
	ASSERT(wDataSize==sizeof(CMD_S_GameEnd));
	if (wDataSize!=sizeof(CMD_S_GameEnd)) return false;

	char msg[60];
	memset(msg,0,sizeof(msg));

	//��Ϣ����
	CMD_S_GameEnd * pGameEnd=(CMD_S_GameEnd *)pBuffer;
	
	//������Ϸ״̬
	SetGameStatus(GS_MJ_FREE);
	m_GameClientView.SetStatusFlag(false,false);

	//ɾ����ʱ��
	KillGameTimer(IDI_OPERATE_CARD);
	if(IsLookonMode()==false)
	{
		SetGameTimer(GetMeChairID(),IDI_START_GAME,TIME_START_GAME);
	}
	
	m_IsTuoGuan=false;
	if(IsLookonMode()==false)
	{
		m_GameClientView.m_btTuoGuan.ShowWindow(SW_HIDE);
		m_GameClientView.m_btDisTuoGuan.ShowWindow(SW_HIDE);
	}
	ZeroMemory(m_GameClientView.m_cbTongCount,sizeof(m_GameClientView.m_cbTongCount));
	
	DisableOutCard();

	//���ÿؼ�
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);
	m_GameClientView.SetHD(false);

	tagGameScoreInfo GameScoreInfo;
	memset(&GameScoreInfo,0,sizeof(GameScoreInfo));

	GameScoreInfo.lHuPoint = pGameEnd->lHuPoint;
	GameScoreInfo.cbGoldCard = pGameEnd->bGoldCard;

	m_GameClientView.m_GameScoreWnd.SetTax(pGameEnd->lTax);

	//babala 11-30������ý�����������ƴ����ͻ���
	//��������,���˷��ڵĺ��ƣ��õ�������Ϣ��������ʾ�����Ĵ���
	if (pGameEnd->cbChiHuCard!=0 && pGameEnd->bIsQuit==false)
	{
		//PlayActionSound(WIK_CHI_HU);
		GameScoreInfo.wBankerUser=m_wBankerUser;
		GameScoreInfo.cbChiHuCard=pGameEnd->cbChiHuCard;
		GameScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		GameScoreInfo.cbGoldCard=pGameEnd->bGoldCard;
		GameScoreInfo.wWinner=(BYTE)pGameEnd->wWinner;

		InsertShtickString( TEXT( " ���ֽ������ɼ�ͳ�ƣ�" ), RGB( 255, 0, 0 ), true );

		CString strScore;
		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserData(i);
			GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];
			lstrcpyn(GameScoreInfo.szUserName[i],pUserData->szName,CountArray(GameScoreInfo.szUserName[i]));
			strScore.Format( TEXT( " %s ��%ld" ), pUserData->szName,pGameEnd->lGameScore[i]);
			InsertShtickString( strScore, RGB( 255, 0, 0 ), true );
		}
		//�����������
		ZeroMemory(&GameScoreInfo.HuCardWeave,sizeof(GameScoreInfo.HuCardWeave));
		GetHuCardWeave(pGameEnd->cbCenterCard,pGameEnd->cbWeaveKind,pGameEnd->cbYaKou,pGameEnd->cbHuaWeaveCount,pGameEnd->cbHuaHandCount,m_cbWeaveCount[pGameEnd->wWinner],GameScoreInfo.HuCardWeave);

		m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);
	}
	
	//��ׯ��ʱ�������ǽ����ʾ��ׯͼƬ
	if ((pGameEnd->cbChiHuCard==0)&&(pGameEnd->wChiHuUserCount==0)&&(pGameEnd->bIsQuit==false))
	{
		m_GameClientView.m_wMinusHeadCount=m_GameClientView.m_wMinusLastCount+1;
		m_GameClientView.SetHuangZhuang(true);
		m_GameClientView.SetStatusFlag(false,false);
	}
	//ǿ��
	else
	{
		GameScoreInfo.wBankerUser=m_wBankerUser;
		GameScoreInfo.cbChiHuCard=pGameEnd->cbChiHuCard;
		GameScoreInfo.wProvideUser=pGameEnd->wProvideUser;
		GameScoreInfo.cbGoldCard=pGameEnd->bGoldCard;
		GameScoreInfo.wWinner=(BYTE)pGameEnd->wWinner;
		GameScoreInfo.lHuPoint = pGameEnd->lHuPoint;

		for (WORD i=0;i<GAME_PLAYER;i++)
		{
			const tagUserData * pUserData=GetUserData(i);
			GameScoreInfo.lScore[i]=pGameEnd->lGameScore[i];

			lstrcpyn(GameScoreInfo.szUserName[i],pUserData->szName,CountArray(GameScoreInfo.szUserName[i]));
		}

		m_GameClientView.m_GameScoreWnd.SetScoreInfo(GameScoreInfo);
	}

	//�û��˿�
	for (WORD i=0;i<GAME_PLAYER;i++)
	{
		WORD wViewChairID=SwitchViewChairID(i);
		//����Ӯ�Һ��Ʊ�־
		if (pGameEnd->wWinner == i)
		{
			m_GameClientView.SetUserAction(wViewChairID,WIK_CHI_HU);
		}

		m_GameClientView.m_TableCard[wViewChairID].SetCardData(pGameEnd->cbCardData[i],pGameEnd->cbCardCount[i]);
	}

	//�����˿�
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);

	//��������
	LONG lScore=pGameEnd->lGameScore[GetMeChairID()];
	if (lScore>0L) 
	{
		m_cbSex = GetUserData(GetMeChairID())->cbGender;
		//��������
		if (m_cbSex == 1)
		{
			if(IsEnableSound()==true)
			{
				CString szSoundName=_T("");
				szSoundName="Sound\\boy\\";
				szSoundName+="��.wav";
				PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
			}
		}
		else  
		{
			if(IsEnableSound()==true)
			{
				CString szSoundName=_T("");
				szSoundName="Sound\\girl\\";
				szSoundName+="��.wav";
				PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
			}
		}
	}
	else if (lScore<0L) 
	{
		if(IsEnableSound()==true)
		{
			CString szSoundName=_T("");
			szSoundName="Sound\\";
			szSoundName+="GAME_LOST.wav";
			PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
		}
	}
	else
	{
		if(IsEnableSound()==true)
		{
			CString szSoundName=_T("");
			szSoundName="Sound\\";
			szSoundName+="GAME_END.wav";
			PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
		}
	}


	//���ý���
	if (IsLookonMode()==false)
	{
		m_GameClientView.m_btStart.ShowWindow(SW_SHOW);
	}

	//���½���
	m_GameClientView.UpdateGameView(NULL);

	return true;
}


//----------------------------------------------------------------------------------------
//��������
void CGameClientDlg::PlayCardSound(BYTE cbCardData)
{
	
	//��������
	CString szSoundName;

	if(m_wPlayNativeSound==1)
	{
		if(m_cbSex == 0)
		{
			szSoundName="Sound\\�ط���\\girl\\";
		}
		else if(m_cbSex == 1)
		{
			szSoundName="Sound\\�ط���\\boy\\";
		}
		else if(m_cbSex == 2)
		{
			szSoundName="Sound\\�ط���\\girl\\";
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		if(m_cbSex == 0)
		{
			szSoundName="Sound\\��ͨ��\\girl\\";
		}
		else if(m_cbSex == 1)
		{
			szSoundName="Sound\\��ͨ��\\boy\\";
		}
		else if(m_cbSex == 2)
		{
			szSoundName="Sound\\��ͨ��\\girl\\";
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	BYTE cbCardIndex = m_GameLogic.SwitchToCardIndex(cbCardData);
	//0--9
	if (cbCardIndex >=0 && cbCardIndex<10)
	{
		switch(cbCardIndex)
		{
		case 0:
			{
				szSoundName+="һ.wav";
				break;
			}
		case 1:
			{
				szSoundName+="��.wav";
				break;
			}
		case 2:
			{
				szSoundName+="��.wav";
				break;
			}
		case 3:
			{
				szSoundName+="��.wav";
				break;
			}
		case 4:
			{
				szSoundName+="��.wav";
				break;
			}
		case 5:
			{
				szSoundName+="��.wav";
				break;
			}
		case 6:
			{
				szSoundName+="��.wav";
				break;
			}
		case 7:
			{
				szSoundName+="��.wav";
				break;
			}
		case 8:
			{
				szSoundName+="��.wav";
				break;
			}
		case 9:
			{
				szSoundName+="ʮ.wav";
				break;
			}
		}
	}
	else
	{
		switch(cbCardIndex)
		{
		case 10:
			{
				szSoundName+="��.wav";

				break;
			}
		case 11:
			{
				szSoundName+="��.wav";
				break;
			}
		case 12:
			{
				szSoundName+="��.wav";
				break;
			}
		case 13:
			{
				szSoundName+="��.wav";
				break;
			}
		case 14:
			{
				szSoundName+="��.wav";
				break;
			}
		case 15:
			{
				szSoundName+="��.wav";
				break;
			}
		case 16:
			{
				szSoundName+="��.wav";
				break;
			}
		case 17:
			{
				szSoundName+="ǧ.wav";
				break;
			}
		case 18:
			{
				szSoundName+="��.wav";
				break;
			}
		case 19:
			{
				szSoundName+="֪.wav";
				break;
			}
		case 20:
			{
				szSoundName+="��.wav";
				break;
			}
		case 21:
			{
				szSoundName+="��.wav";
				break;
			}
		}
	}

	PlaySound(szSoundName,NULL,SND_FILENAME|SND_ASYNC);

	return;
}
//��������
void CGameClientDlg::PlayActionSound(BYTE cbAction)
{
	
	CString szSoundName=_T("");
	if(m_wPlayNativeSound==1)
	{
		if(m_cbSex == 0)
		{
			szSoundName="Sound\\�ط���\\girl\\";
		}
		else if(m_cbSex == 1)
		{
			szSoundName="Sound\\�ط���\\boy\\";
		}
		else if(m_cbSex == 2)
		{
			szSoundName="Sound\\�ط���\\girl\\";
		}
		else
		{
			ASSERT(FALSE);
		}
	}
	else
	{
		
		if(m_cbSex == 0)
		{
			szSoundName="Sound\\��ͨ��\\girl\\";
		}
		else if(m_cbSex == 1)
		{
			szSoundName="Sound\\��ͨ��\\boy\\";
		}
		else if(m_cbSex == 2)
		{
			szSoundName="Sound\\��ͨ��\\girl\\";
		}
		else
		{
			ASSERT(FALSE);
		}
	}

	switch (cbAction)
	{
	case WIK_NULL:		//ȡ��
		{
			break;
		}
	case WIK_DUI:		//����
		{
			szSoundName+="��.WAV";
			break;
		}
	case WIK_SAN_ZHAO:		//���У�����
		{
			szSoundName+="����.WAV";
			break;
		}
	case WIK_SI_ZHAO:
		{
			szSoundName+="��Բ.WAV";
			break;
		}
	case WIK_SI_TONG:
	case WIK_WU_TONG:
		{
			szSoundName+="��.WAV";
			break;
		}
	case WIK_CHI_HU:	//�Ժ�
		{
			szSoundName+="��.WAV";
			break;
		}
	}

	PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);

	return;
}

//��ʾ����
void CGameClientDlg::ShowTongOperateControl(BYTE cbUserAction, BYTE cbActionCard)
{
	//������ҵĲ������
	tagSiTongResult SiTongResult;
	tagWuTongResult WuTongResult;

	ZeroMemory(&SiTongResult,sizeof(SiTongResult));
	ZeroMemory(&WuTongResult,sizeof(WuTongResult));

	WORD wMeChairID=GetMeChairID();

	//����������������г�ȥ�ص��ƣ����ж϶��ƺ�����
	BYTE bTempIndex[MAX_INDEX];
	CopyMemory(bTempIndex,m_cbCardIndex,sizeof(bTempIndex));

	//ɾ������ص�������֮���ٷ����Ƿ�����ص����
	for (BYTE j=0; j<8; j++)
	{
		BYTE index = m_WuTongIndex[j];
		if (index != INVALID_CARDINDEX)
		{
			bTempIndex[index] = 0;
		}
	}
	m_GameLogic.AnalyseWuTong(bTempIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],WuTongResult);

	//��ɾ�����Ĳص�������֮���ٷ����Ƿ����Ĳص����
	for (BYTE j=0; j<8; j++)
	{
		BYTE index = m_SiTongIndex[j];
		if (index != INVALID_CARDINDEX)
		{
			bTempIndex[index] = 0;
		}
	}
	m_GameLogic.AnalyseSiTong(bTempIndex,SiTongResult);

	if((IsLookonMode()==false) && (SiTongResult.cbCardCount>0 || WuTongResult.cbCardCount>0))
	{
		//Ϊ�˽����й�������õ�
		m_cbSendCardFinish=true;

		m_GameClientView.m_ControlWnd.SetTongOperateCardData(cbActionCard,cbUserAction,SiTongResult,WuTongResult,m_cbHuaCardIndex);
	}
}

//�����������������������������ͣ��ó������������ݣ����浽���������
bool CGameClientDlg::GetHuCardWeave(BYTE cbCardIndex[],BYTE cbWeaveKind[],BYTE cbYaKou[],BYTE cbHuaWeaveCount[],BYTE cbHuaHandCount[],BYTE cbWeaveCount,tagHuCardWeave &HuCardWeave)
{	
	BYTE cbCount=0;
	for (BYTE i=0; i<cbWeaveCount; i++)
	{
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		//�����1,3,5,7,9,�����ϻ��ƽ��з���
		if (cbCardIndex[i] == 0 ||cbCardIndex[i] == 2 ||cbCardIndex[i] == 4 ||cbCardIndex[i] == 6 ||cbCardIndex[i] == 8 )
		{
			count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,cbHuaWeaveCount[cbCardIndex[i]/2]);
		}
		else
		{
			count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,0);
		}
		//�����õ��Ľ��
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}
	//��������Ƹ���������8
	ASSERT(i<= 8);
	
	//�������л�����ʱ����
	BYTE cbHuaCardIndex[5] = {0,0,0,0,0};
	CopyMemory(cbHuaCardIndex,cbHuaHandCount,sizeof(cbHuaCardIndex));
	//��־����Ƿ��Ƿ����������
	//(��Ӱڷŵ�ԭ�������ȷ��ڶ���,���Ƶ������,����ܷ���������ô��־��Ϊ��.���û�з���ǰ�������������ô���ٷ����������͵�����,�����û����ô�ͷ���Ѿ����)
	bool LZUse=false; 

	//�õ������Ƶ����
	for (; i<8; i++)
	{
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		if(cbWeaveKind[i]==WIK_QI_TA) continue;
		//1.WIK_QI_TA���
		if (cbWeaveKind[i] != WIK_QI_TA)
		{
			//�����1,3,5,7,9,�����ϻ��ƽ��з���
			if (cbCardIndex[i] == 0 ||cbCardIndex[i] == 2 ||cbCardIndex[i] == 4 ||cbCardIndex[i] == 6 ||cbCardIndex[i] == 8 )
			{
				count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,cbHuaCardIndex[cbCardIndex[i]/2]);
				BYTE num=0;
				switch(cbWeaveKind[i])
				{
				case WIK_WU_TONG:
					{
						num=5;
						break;
					}
				case WIK_SI_TONG:
					{
						num=4;
						break;
					}
				case WIK_DUI:
					{
						num=3;
						break;
					}
				}

				cbHuaCardIndex[cbCardIndex[i]/2] = 0;
			}
			else
			{
				count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,0);
			}

		}
		
		//�����õ��Ľ��
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}
	for(BYTE i=0;i<8;i++)
	{
		if(cbWeaveKind[i]!=WIK_QI_TA) continue;
		
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,0);

		//����У��
		//��������:һ��������,��(���ܺ������Ż���)
		if (cbCardIndex[i] == 0 ||cbCardIndex[i] == 2 ||cbCardIndex[i] == 4 ||cbCardIndex[i] == 6)
		{
			//�ж���������:һ�������������壩���壨�ߣ����ߣ��ţ��Ļ����Ƿ�Ϊ0
			if (cbHuaCardIndex[cbCardIndex[i]/2] != 0)
			{
				//У����0λ����
				cbCardData[0] = cbCardData[0]|0x10;
				cbHuaCardIndex[cbCardIndex[i]/2] -= 1;
			}
			//У����2λ����
			if (cbHuaCardIndex[cbCardIndex[i]/2+1] != 0)
			{
				cbCardData[2] = cbCardData[2]|0x10;
				cbHuaCardIndex[cbCardIndex[i]/2+1] -= 1;
			}
		}
		//�������� �����ģ�������(ֻ���ܺ���һ�Ż���),�жϻ��������壬��,���Ƿ�Ϊ0
		if (cbCardIndex[i] == 1 || cbCardIndex[i] == 3||cbCardIndex[i] == 5 || cbCardIndex[i] == 7)
		{
			//У���м�Ļ�������
			if (cbHuaCardIndex[(cbCardIndex[i]+1)/2] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[(cbCardIndex[i]+1)/2] -= 1;
			}
		}
		//�����ѣ���(10)
		if (cbCardIndex[i] == 10)
		{
			//�ж��Ƿ��л���
			if (cbHuaCardIndex[0] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[0] -= 1;
			}
		}
        //��ʮ��:��(15)
		if (cbCardIndex[i] == 15)
		{
			//�ж��Ƿ��л���
			if (cbHuaCardIndex[3] != 0)
			{
				cbCardData[0] = cbCardData[0]|0x10;
				cbHuaCardIndex[3] -= 1;
			}
		}
		//����ǧ:��(16)
		if (cbCardIndex[i] == 16)
		{
			//�ж����Ƿ��л���
			if (cbHuaCardIndex[1] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[1] -= 1;
			}
		}
		//�˾���:��(21)
		if (cbCardIndex[i] == 21)
		{
			//�ж��Ƿ��л���
			if (cbHuaCardIndex[4] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[4] -= 1;
			}
		}

		//�����õ��Ľ��
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}
	////����Ѿ������
	for (BYTE i=0; i<2; i++)
	{
		//1,3,5,7,9��ת��
		if (cbYaKou[i] == 0 ||cbYaKou[i] == 2 ||cbYaKou[i] == 4 ||cbYaKou[i] == 6 ||cbYaKou[i] == 8 )
		{
			//���ݻ��Ƹ�������ת��
			if (cbHuaCardIndex[cbYaKou[i]/2] != 0)
			{
				HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i])|0x10;
				cbHuaCardIndex[cbYaKou[i]/2]--;
			}
			//û�л���
			else
			{
				HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i]);
			}

		}
		//��1��3��5��7��9��ת��
		else
		{
			HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i]);
		}

	}
	return true;
}




//�����������������������������ͣ��ó������������ݣ����浽���������
bool CGameClientDlg::GetHuCardWeave(BYTE cbCardIndex[],BYTE cbWeaveKind[],BYTE cbYaKou[],tagHuCardWeave &HuCardWeave)
{
	//���ݵ�ǰ��Ҳ�������,����������,��ϻ���,�õ����������
	WORD wMeChairID=GetMeChairID();
	BYTE cbCount=0;
	for (BYTE i=0; i<m_cbWeaveCount[wMeChairID]; i++)
	{
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		//�����1,3,5,7,9,�����ϻ��ƽ��з���
		if (cbCardIndex[i] == 0 ||cbCardIndex[i] == 2 ||cbCardIndex[i] == 4 ||cbCardIndex[i] == 6 ||cbCardIndex[i] == 8 )
		{
			count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,m_cbHuaWeaveCardIndex[cbCardIndex[i]/2]);
		}
		else
		{
			count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,0);
		}
		//�����õ��Ľ��
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}
	//��������Ƹ���������8
	ASSERT(i<= 8);
	
	//�������л�����ʱ����
	BYTE cbHuaCardIndex[5] = {0,0,0,0,0};
	CopyMemory(cbHuaCardIndex,m_cbHuaCardIndex,sizeof(cbHuaCardIndex));
	//��־����Ƿ��Ƿ����������
	//(��Ӱڷŵ�ԭ�������ȷ��ڶ���,���Ƶ������,����ܷ���������ô��־��Ϊ��.���û�з���ǰ�������������ô���ٷ����������͵�����,�����û����ô�ͷ���Ѿ����)
	bool LZUse=false; 

	//�õ������Ƶ����
	for (; i<8; i++)
	{
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		if (cbWeaveKind[i] == WIK_QI_TA) continue;
		//1.��WIK_QI_TA���
		if (cbWeaveKind[i] != WIK_QI_TA)
		{
			//�����1,3,5,7,9,�����ϻ��ƽ��з���
			if (cbCardIndex[i] == 0 ||cbCardIndex[i] == 2 ||cbCardIndex[i] == 4 ||cbCardIndex[i] == 6 ||cbCardIndex[i] == 8 )
			{
				count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,cbHuaCardIndex[cbCardIndex[i]/2]);
				BYTE num=0;
				switch(cbWeaveKind[i])
				{
				case WIK_WU_TONG:
					{
						num=5;
						break;
					}
				case WIK_SI_TONG:
					{
						num=4;
						break;
					}
				case WIK_DUI:
					{
						num=3;
						break;
					}
				}

				cbHuaCardIndex[cbCardIndex[i]/2] = 0;
			}
			else
			{
				count = m_GameLogic.GetHuCardWeave(cbWeaveKind[i],cbCardIndex[i],cbCardData,0);
			}

		}
		//�����õ��Ľ��
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}

	for(BYTE j=0;j<8;j++)
	{
		
		BYTE cbCardData[] = {0,0,0,0,0};
		BYTE count = 0;
		if (cbWeaveKind[j] != WIK_QI_TA) continue;
		count = m_GameLogic.GetHuCardWeave(cbWeaveKind[j],cbCardIndex[j],cbCardData,0);

		//����У��
		//��������:һ��������,��(���ܺ������Ż���)
		if (cbCardIndex[j] == 0 ||cbCardIndex[j] == 2 ||cbCardIndex[j] == 4 ||cbCardIndex[j] == 6)
		{
			//�ж���������:һ�������������壩���壨�ߣ����ߣ��ţ��Ļ����Ƿ�Ϊ0
			if (cbHuaCardIndex[cbCardIndex[j]/2] != 0)
			{
				//У����0λ����
				cbCardData[0] = cbCardData[0]|0x10;
				cbHuaCardIndex[cbCardIndex[j]/2] -= 1;
			}
			//У����2λ����
			if (cbHuaCardIndex[cbCardIndex[j]/2+1] != 0)
			{
				cbCardData[2] = cbCardData[2]|0x10;
				cbHuaCardIndex[cbCardIndex[j]/2+1] -= 1;
			}
		}
		//�������� �����ģ�������(ֻ���ܺ���һ�Ż���),�жϻ��������壬��,���Ƿ�Ϊ0
		if (cbCardIndex[j] == 1 || cbCardIndex[j] == 3||cbCardIndex[j] == 5 || cbCardIndex[j] == 7)
		{
			//У���м�Ļ�������
			if (cbHuaCardIndex[(cbCardIndex[j]+1)/2] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[(cbCardIndex[j]+1)/2] -= 1;
			}
		}
		//�����ѣ���(10)
		if (cbCardIndex[j] == 10)
		{
			//�ж��Ƿ��л���
			if (cbHuaCardIndex[0] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[0] -= 1;
			}
		}
        //��ʮ��:��(15)
		if (cbCardIndex[j] == 15)
		{
			//�ж��Ƿ��л���
			if (cbHuaCardIndex[3] != 0)
			{
				cbCardData[0] = cbCardData[0]|0x10;
				cbHuaCardIndex[3] -= 1;
			}
		}
		//����ǧ:��(16)
		if (cbCardIndex[j] == 16)
		{
			//�ж����Ƿ��л���
			if (cbHuaCardIndex[1] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[1] -= 1;
			}
		}
		//�˾���:��(21)
		if (cbCardIndex[j] == 21)
		{
			//�ж��Ƿ��л���
			if (cbHuaCardIndex[4] != 0)
			{
				cbCardData[1] = cbCardData[1]|0x10;
				cbHuaCardIndex[4] -= 1;
			}
		}
		
		//�����õ��Ľ��
		CopyMemory(HuCardWeave.cbCardData[cbCount],cbCardData,sizeof(cbCardData));
		HuCardWeave.cbCardCount[cbCount++] = count;
	}
	////����Ѿ������
	for (BYTE i=0; i<2; i++)
	{
		//1,3,5,7,9��ת��
		if (cbYaKou[i] == 0 ||cbYaKou[i] == 2 ||cbYaKou[i] == 4 ||cbYaKou[i] == 6 ||cbYaKou[i] == 8 )
		{
			//���ݻ��Ƹ�������ת��
			if (cbHuaCardIndex[cbYaKou[i]/2] != 0)
			{
				HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i])|0x10;
				cbHuaCardIndex[cbYaKou[i]/2]--;
			}
			//û�л���
			else
			{
				HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i]);
			}

		}
		//��1��3��5��7��9��ת��
		else
		{
			HuCardWeave.cbYaKou[i] = m_GameLogic.SwitchToCardData(cbYaKou[i]);
		}

	}
	return true;
}


//��ʼ��ť
LRESULT CGameClientDlg::OnStart(WPARAM wParam, LPARAM lParam)
{
	//---------------��������Ǵ����е���ʾ�ؼ���Ϣ----------------------//

	//��������
	KillGameTimer(IDI_START_GAME);
	m_GameClientView.m_btStart.ShowWindow(SW_HIDE);

	for (BYTE i=0; i<9; i++)
	{
		m_GameClientView.m_HandCard[i].SetCardData(NULL,0);
	}

	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

	m_IsTuoGuan=false;
	m_cbSendCardFinish=false;

	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);

	m_GameClientView.m_IsStartSendCard=false;

	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_GameScoreWnd.ShowWindow(SW_HIDE);

	//���ý���
	m_GameClientView.SetHuangZhuang(false);
	m_GameClientView.SetHD(false);
	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetBankerUser(INVALID_CHAIR);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(INVALID_CHAIR,0);
	m_GameClientView.m_wMinusHeadCount=1;
	m_GameClientView.m_wMinusLastCount=0;

	//�˿�����
	m_GameClientView.m_UserCard[0].SetCardData(0,false);
	m_GameClientView.m_UserCard[2].SetCardData(0,false);
	m_GameClientView.m_UserCard[1].SetCardData(0,false);

	ZeroMemory(m_GameClientView.m_cbTongCount,sizeof(m_GameClientView.m_cbTongCount));
	//�����˿�
	m_GameClientView.m_leaveHeapCardData=0;

	//�˿�����
	for (BYTE i=0;i<GAME_PLAYER;i++)
	{
		m_GameClientView.m_TableCard[i].SetCardData(NULL,0);
		m_GameClientView.m_DiscardCard[i].SetCardData(NULL,0);
		
		for(BYTE j=0;j<8;j++)
		{
			m_GameClientView.m_WeaveCard[i][j].SetCardData(NULL,0);
		}
	}

	//---------------�����������Ϸ�еı�����Ϣ----------------------//

	m_wBankerUser=INVALID_CHAIR;
	m_wCurrentUser=INVALID_CHAIR;

	m_SiTongCount =0;
	m_WuTongCount =0;
	for (BYTE i=0; i<8; i++)
	{
		m_WuTongIndex[i] = INVALID_CARDINDEX;
		m_SiTongIndex[i] = INVALID_CARDINDEX;
	}

	//��ռ�¼���Ƶ����
	ZeroMemory(m_cbHuaCardIndex,sizeof(m_cbHuaCardIndex));
	ZeroMemory(m_cbHuaWeaveCardIndex,sizeof(m_cbHuaWeaveCardIndex));

	//��ռ�¼����Ƶ����
	ZeroMemory(m_cbWeaveCount,sizeof(m_cbWeaveCount));
	ZeroMemory(m_WeaveItemArray,sizeof(m_WeaveItemArray));

	m_bGameStart = false;

	//�˿˱���
	m_cbLeftCardCount=0;
	ZeroMemory(m_cbCardIndex,sizeof(m_cbCardIndex));

	b_IsHD=false;

	//������Ϣ
	m_cbOutCardData=0;
	m_wOutCardUser=INVALID_CHAIR;
	ZeroMemory(m_cbDiscardCard,sizeof(m_cbDiscardCard));
	ZeroMemory(m_cbDiscardCount,sizeof(m_cbDiscardCount));

	//������Ϣ
	SendUserReady(NULL,0);

	return 0;
}


//ȡ���йܰ�ť
LRESULT CGameClientDlg::OnDisTuoGuan(WPARAM wParam, LPARAM lParam)
{
    //�����ж�
	//if (m_bGameStart==true && (m_wCurrentUser!=GetMeChairID())) return 0;

	m_GameClientView.m_btTuoGuan.ShowWindow(SW_SHOW);
	m_GameClientView.m_btDisTuoGuan.ShowWindow(SW_HIDE);

	m_IsTuoGuan=false;

	return 0;
}

//�йܰ�ť
LRESULT CGameClientDlg::OnTuoGuan(WPARAM wParam, LPARAM lParam)
{	

	//�����ж�
	m_GameClientView.m_btTuoGuan.ShowWindow(SW_HIDE);
	m_GameClientView.m_btDisTuoGuan.ShowWindow(SW_SHOW);
	m_IsTuoGuan=true;

	
	return 0;
}

//���Ʋ���
LRESULT CGameClientDlg::OnOutCard(WPARAM wParam, LPARAM lParam)
{
	//�����ж�
	if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return 0;

	//���ñ���
	m_wCurrentUser=INVALID_CHAIR;
	BYTE cbOutCardData=(BYTE)wParam;

	m_cbActionMask = 0;
	m_cbActionCard = 0;

	BYTE cbOutCardIndex=m_GameLogic.SwitchToCardIndex(cbOutCardData);

	//������ȥ���ǻ��ƣ���������λ�õ�����-1
	if ( (cbOutCardData&MASK_COLOR) == 0x10 )
	{
		//���Ըû��Ʋ�Ϊ0
		ASSERT(m_cbHuaCardIndex[cbOutCardIndex/2] != 0);
		if (m_cbHuaCardIndex[cbOutCardIndex/2] == 0)
		{
			//AfxMessageBox("�Ŷ��������˼�����Ƴ����ˣ�");
			return 0;
		}

		//��������λ�ü�1
		m_cbHuaCardIndex[cbOutCardIndex/2]--;
	}

	//����������λ�ü�1
	m_cbCardIndex[cbOutCardIndex]--;

	//��ֹ�û����Ʋ���
	DisableOutCard();

	KillGameTimer(IDI_OPERATE_CARD);

	m_GameClientView.SetStatusFlag(false,false);
	m_GameClientView.SetUserAction(INVALID_CHAIR,0);
	m_GameClientView.SetOutCardInfo(1,cbOutCardData);

	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

	m_GameClientView.UpdateGameView(NULL);

	if(IsEnableSound()==true)
	{
		m_cbSex = GetUserData(GetMeChairID())->cbGender;
		//��������
		PlayCardSound(cbOutCardData);
	}
	//��������
	CMD_C_OutCard OutCard;
	OutCard.cbCardData=cbOutCardData;
	SendData(SUB_C_OUT_CARD,&OutCard,sizeof(OutCard));
	
	return 0;
}

//�˿˲���
LRESULT CGameClientDlg::OnCardOperate(WPARAM wParam, LPARAM lParam)
{
	//��������
	BYTE cbOperateCode=(BYTE)(wParam);
	BYTE cbOperateCard=(BYTE)(lParam);

	//�ǵ�һ�ֵĲ��ƹ������û��������������ת���ɷ������Ʋ����ٷ�����������
	if ((cbOperateCode==WIK_NULL) && m_bGameStart == false )
	{
		cbOperateCode=WIK_TONG_GIVE_UP;
	}

	//�û�������Ʋ����Ĵ���
	if (cbOperateCode==WIK_NULL)
	{
		//���ﴦ���û��Լ����Ƶ����--����ͬʱ�к�������Ƶ����--�û�����˺��Ƶ�������Ƶ���
		if(m_wCurrentUser==GetMeChairID())
		{
			//�����ҵ������֮�󣬲����벻Ϊ�գ�˵���ǵ���˺��ƶԻ����ϵ�������ʾ��������
			if (m_cbActionMask != 0 && IsLookonMode()==false)
			{
				//���غ��ƶԻ���
				m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

				ShowTongOperateControl(m_cbActionMask,0);
				m_cbActionMask = 0;
				
				return 0;
			}
			else
			{
				m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
				m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);
			}
			
		}
		//���ﴦ�������û����Ƶ����--����ͬʱ�к���������Ƶ����--�û�����˺��Ƶ���������Ƶ���
		else 
		{
			//�����ҵ������֮�󣬲����벻Ϊ�գ�˵���ǵ���˺��ƶԻ����ϵ�������ʾ��������
			if (m_cbActionMask != 0 && IsLookonMode()==false)
			{
				//���غ��ƶԻ���
				m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

				m_GameClientView.m_ControlWnd.SetDuiZhaoOperateCardData(m_cbActionCard,m_cbActionMask,m_cbHuaCardIndex);
				
				m_cbActionMask=0;
				m_cbActionCard=0;

				return 0;//������ʾ������ʾ
			}
			else
			{
				m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
				m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);
			}
		}
	}

	//��������
	KillGameTimer(IDI_OPERATE_CARD);
	m_GameClientView.SetStatusFlag(false,true);
	m_GameClientView.m_ControlWnd.ShowWindow(SW_HIDE);
	m_GameClientView.m_HuCardDlg.ShowWindow(SW_HIDE);

	//��������
	CMD_C_OperateCard OperateCard;
	OperateCard.cbOperateCode=cbOperateCode;
	OperateCard.cbOperateCard=cbOperateCard;
	SendData(SUB_C_OPERATE_CARD,&OperateCard,sizeof(OperateCard));

	return 0;
}

bool CGameClientDlg::SetWoindowData(BYTE cbCardData[],BYTE cbDataCount)
{
	if (m_wCurrentUser != GetMeChairID())
	{
		//AfxMessageBox("�������������������ֵ����ҷ���Ҳ����һ���Ƹ���~!!@");
	}
	
	BYTE addCount=0;
	for (int i=8; i>=0; i--)
	{
		///////////////???????????��Ҫ�޸ĵĵط�����9����ÿ��������Ƹ���
		if (m_GameClientView.m_HandCard[i].m_cbCardCount<9 && addCount<cbDataCount)
		{
			m_GameClientView.m_HandCard[i].AddCardData(cbCardData[addCount]);
			addCount++;
			
			//Ϊ�����»ػ�Ҫ�����жϸ�����	
			i=i+1;
		}			
	}

	return true;
}

bool CGameClientDlg::AddUserHandCard(BYTE cbCardData,BYTE cbDataCount)
{
	//if (m_wCurrentUser != GetMeChairID())
	//{
	//	//AfxMessageBox("�������������������ֵ����ҷ���Ҳ����һ���Ƹ���~!!@");
	//}
	
	for (int i=8; i>=0; i--)
	{
		///////////////???????????��Ҫ�޸ĵĵط�����9����ÿ��������Ƹ���
		if (m_GameClientView.m_HandCard[i].m_cbCardCount<9)
		{
			m_GameClientView.m_HandCard[i].AddCardData(cbCardData);
			
			//Ϊ�����»ػ�Ҫ�����жϸ�����	
			break;
		}			
	}

	//if (addCount != cbDataCount)
	//{
	//	CString sz;
	//	sz.Format("ʵ�����ӵ��Ƹ���Ϊ:%d,���������͹������Ƹ���Ϊ��%d",addCount,cbDataCount);
	//	//AfxMessageBox(sz);
	//}

	//if (m_GameClientView.m_HandCard[8].m_cbCardCount != 0)
	//{
	//	CString sz;
	//	sz.Format("ע�Ⱑ���ھ���������������Ϊ:%d",m_GameClientView.m_HandCard[8].m_wCardCount);
	//	//AfxMessageBox(sz);
	//}
	return true;
}

//���÷��ƴ�������Ӧ����¼�
void CGameClientDlg::EnableOutCard()
{

	for(BYTE i=0;i<9;i++)
	{
		m_GameClientView.m_HandCard[i].m_bCanOutCard=true;
	}
	
}
//���÷��ƴ��ڲ���Ӧ����¼�
void CGameClientDlg::DisableOutCard()
{
	for(BYTE i=0;i<9;i++)
	{
		m_GameClientView.m_HandCard[i].m_bCanOutCard=false;
	}
}
//�����ƶ�������
LRESULT CGameClientDlg::OnSendOneCardFinish(WPARAM wParam, LPARAM lParam)
{
	BYTE cbCardData=(BYTE)lParam;
	WORD wViewChairID=(WORD)wParam;

	if(m_bGameStart==false)
	{
		if(wViewChairID==1)
		{
			//���Ƽ��뵽��Ӧ�ķ�����
			AddUserHandCard(cbCardData,1);
		}
	}
	//������Ϸ��ʼ֮��Ĳ��Ʒ��ƵĶ������û������ƶ����ƵĶ���
	else if(m_bGameStart==true)
	{
		if(wViewChairID==1)
		{
			//���Ƽ��뵽��Ӧ�ķ�����
			AddUserHandCard(cbCardData,1);
		}
	}
	if(IsEnableSound()==true)
	{
		CString szSoundName=_T("");
		szSoundName="Sound\\";
		szSoundName+="SEND_CARD.wav";
		PlaySound((LPCTSTR)szSoundName,NULL,SND_FILENAME|SND_ASYNC|SND_SYNC);
	}
	return 0;
}

//�����ƶ�������
LRESULT CGameClientDlg::OnSendCardFinish(WPARAM wParam, LPARAM lParam)
{
	//������Թ��û���ô��ֹ�����
	if(IsLookonMode()==true)
	{
		DisableOutCard();
	}

	//��һ�ֵĲ��Ƶķ��ƶ����Ľ���
	if(m_bGameStart==false)
	{
		//�ڵ�һ�ֵĲ��ƹ����У����Ʒ���˭��˭�ڴ�������о��ǵ�ǰ�û�--��������֮�󣬻Ὣ����յ�
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		m_bCardCount[m_wCurrentUser] += m_cbSendCount;

		//�������ߵ��û�,��������������������ʱ��
		if (wViewChairID!=1)
		{
			m_GameClientView.m_UserCard[wViewChairID].AddCardCount(m_cbSendCount);

			if(m_cbActionMask!=WIK_NULL)
			{
				SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
			}
		}
		//��ǰ���
		else
		{
			//���ݷ��Ƶ��������б���
			for (BYTE i=0; i<m_cbSendCount; i++)
			{
				//���淢�������Ƶ�������
				m_cbCardIndex[m_GameLogic.SwitchToCardIndex(m_cbSendCardData[i])]++;
			}

			//���滨�Ƶ���������������
			m_GameLogic.SaveHuaCard(m_cbSendCardData,m_cbSendCount,m_cbHuaCardIndex);
			////���Ƽ��뵽��Ӧ�ķ�����
			//SetWoindowData(m_cbSendCardData,m_cbSendCount);

//���в��ƵĻ����������ʾ����
//----�ر�ǿ��˵����������Ҫ�ڴ���ʾ�û��ܷ���в��Ʋ����ġ�
//----�����ڵ�һ�ֵĲش�����˵��Ҫ�Ƚ��д���֮�󣬲����ټ�����ʾ������ʾ��
//----���ֽ��ز����ƶ��˲�����������λ�ý�����ʾ


			//����к��Ʋ�������ʾ����������
			if ((GetMeChairID()==m_wBankerUser) && (m_cbActionMask & WIK_CHI_HU)!=0)
			{
				tagHuCardWeave HuCardweave;
				ZeroMemory(&HuCardweave,sizeof(HuCardweave));
			
				GetHuCardWeave(m_cbCenterCard,m_cbWeaveKind,m_cbYaKou,HuCardweave);
				m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,m_cbHuScore,m_cbGoldCard);

				//��ʾ���ƶ�ʱ��
				SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
			}
			//�жϳ��˺�֮���Ƿ�����������
			else if((m_cbActionMask & WIK_SI_TONG)!=0 || (m_cbActionMask & WIK_WU_TONG)!=0)
			{
				ShowTongOperateControl(m_cbActionMask,0);
				m_cbActionMask = 0;

				SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
			}
		}

		//��������֮���ֽ���ǰ�û���ԭΪ��--�����ǵ�һ�ֵĲ���
		m_wCurrentUser =INVALID_CHAIR;
		
		//���½���
		m_GameClientView.UpdateGameView(NULL);

		m_cbSendCardFinish=true;
		return true;
	}
	//������Ϸ��ʼ֮��Ĳ��Ʒ��ƵĶ������û������ƶ����ƵĶ���
	else if(m_bGameStart==true)
	{

		//������ͼ�������ʾ������ǵ�ǰ��ң����˿˱��浽������������������У�����ֱ��������ҵ�ǰ�ƿ���
		WORD wViewChairID=SwitchViewChairID(m_wCurrentUser);
		m_bCardCount[m_wCurrentUser] += m_cbSendCount;

		//����ǵ�ǰ�û�
		if (m_wCurrentUser == GetMeChairID())
		{
			//���ݷ��Ƶ��������б���
			for (BYTE i=0; i<m_cbSendCount; i++)
			{
				//���淢�������Ƶ�������
				m_cbCardIndex[m_GameLogic.SwitchToCardIndex(m_cbSendCardData[i])]++;
			}
			//���滨�Ƶ���������������
			m_GameLogic.SaveHuaCard(m_cbSendCardData,m_cbSendCount,m_cbHuaCardIndex);
			////���Ƽ��뵽��Ӧ�ķ�����
			//SetWoindowData(m_cbSendCardData,m_cbSendCount);
		}
		//���ǵ�ǰ�û�
		else
		{
//�������������û������ƴ����˵��������������������������
//��Ϊ��ǰ�û�ʱ������һ����Ϊ��ǰ�ƣ���ʱ����������1�ŵ�
//ֻҪ�����ơ����ƺͲ�����ʱ�ܱ����Ƶ���Ŀһ���ԾͿ����ˡ�
			if (m_cbSendCount == 2 )
			{
				m_GameClientView.m_UserCard[wViewChairID].AddCardCount(1);
			}
			if(b_IsHD==false)
			{
				m_GameClientView.m_UserCard[wViewChairID].SetCurrentCard(true);
			}
		}

		//��ǰ�û�
		if (m_wCurrentUser==GetMeChairID())
		{
			//������
			ActiveGameFrame();

			//�����û����Ʋ���
			EnableOutCard();

			//��������
			if (m_cbActionMask!=WIK_NULL)
			{
				//�˴���Ƶ�˼·Ϊ��
/*
//       1�������Ƶ��û�����ʱ�����ͬʱ�к��Ʋ�������Ʋ����Ļ�������ʾ���Ʋ����Ի���
//   ֻ�е��û��������Ʋ���ʱ������ʾ���Ʋ����Ի���
//       2������ʾ���Ʋ����Ի���ʱ��Ҫ����m_cbActionMask�еĺ��Ʋ����룬�Ա��û�ѡ��
//   �������Ʋ���ʱ����OnCardOperat�������ٴ���ʾ���Ʋ����Ի���
//       3�������в��Ʋ���ʱ����ʾ���ƶԻ���֮��Ҳ�ǽ�m_cbActionMask��ա�
*/
				if ((m_cbActionMask&WIK_CHI_HU)!=0)
				{
					//���ε����ƺ����ý���
					m_cbActionMask = (m_cbActionMask & 0x7F);

					tagHuCardWeave HuCardweave;
					ZeroMemory(&HuCardweave,sizeof(HuCardweave));
				
					GetHuCardWeave(m_cbCenterCard,m_cbWeaveKind,m_cbYaKou,HuCardweave);
					m_GameClientView.m_HuCardDlg.SetHuCardWeave(HuCardweave,m_cbHuScore,m_cbGoldCard);

					//��ʾ���ƶ�ʱ��
					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				}
				//�жϳ��˺�֮���Ƿ�����������
				else 
				{
					ShowTongOperateControl(m_cbActionMask,0);
					m_cbActionMask = 0;

					SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);
				}
			}
			///Ӧ���Լ������ˣ�������ʾ
			else
			{
				SetGameTimer(GetMeChairID(),IDI_OPERATE_CARD,TIME_OPERATE_CARD);

				//������ʾ
				m_GameClientView.SetStatusFlag(true,false);
			}
		}

		//����ǰ�û��в���ʱ��ҲҪ���������û��Ķ�ʱ��
		if((m_cbActionMask!=WIK_NULL) && (GetMeChairID()!=m_wCurrentUser))
		{
			SetGameTimer(m_wCurrentUser,IDI_OPERATE_CARD,TIME_OPERATE_CARD);
		}

		//���½���
		m_GameClientView.UpdateGameView(NULL);

		m_cbSendCardFinish=true;

		return true;
	}

	m_cbSendCardFinish=true;
	return 0;
}
//////////////////////////////////////////////////////////////////////////

void CGameClientDlg::OnTimer(UINT nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CGameFrameDlg::OnTimer(nIDEvent);
}

BOOL CGameClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CGameFrameDlg::PreTranslateMessage(pMsg);
}

//������Ϸ��ʼ֮��Ľ���
void CGameClientDlg::GameStartedFace(void)
{

}
