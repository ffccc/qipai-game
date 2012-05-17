#include "StdAfx.h"
#include "Resource.h"
#include "UserItemElement.h"
#include "UserListControl.h"

//////////////////////////////////////////////////////////////////////////////////

//����λ��
#define SORT_AFTER					1									//����λ��
#define SORT_FRONT					-1									//ǰ��λ��

//�б�����
#define LOCK_COUNT					3									//��������
#define ITEM_HEIGHT					130									//�б�߶�

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CImageList			CUserListControl::m_ImageUserStatus;				//״̬λͼ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUserListControl, CSkinListCtrl)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CUserListControl::CUserListControl()
{
	//��Դ����
	m_dwMySelfUserID=0L;
	m_bAscendSort=false;
	m_bCreateColumn=false;

	//�ӿڱ���
	m_pIGameLevelParser=NULL;
	m_pIGameUserManager=NULL;

	//Ĭ�϶���
	m_ColorUserList.crSelectTX=COLOR_SELECT_TX;
	m_ColorUserList.crSelectBK=COLOR_SELECT_BK;
	m_ColorUserList.crNormalTX=COLOR_NORMAL_TX;
	m_ColorUserList.crNormalBK=COLOR_NORMAL_BK;

	//��Ҷ���
	m_ColorUserList.crMyselfTX=COLOR_MYSELF_TX;
	m_ColorUserList.crMyselfBK=COLOR_MYSELF_BK;
	m_ColorUserList.crMasterTX=COLOR_MASTER_TX;
	m_ColorUserList.crMasterBK=COLOR_MASTER_BK;
	m_ColorUserList.crMemberTX=COLOR_MEMBER_TX;
	m_ColorUserList.crMemberBK=COLOR_MEMBER_BK;

	//�б���Ϣ
	m_wColumnCount=0;
	ZeroMemory(m_cbDataDescribe,sizeof(m_cbDataDescribe));

	//���ÿؼ�
	m_SkinHeaderCtrl.SetLockCount(LOCK_COUNT);
	m_SkinHeaderCtrl.SetItemHeight(ITEM_HEIGHT);

	return;
}

//��������
CUserListControl::~CUserListControl()
{
}

//�ؼ���
VOID CUserListControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//����״̬
	if (m_ImageUserStatus.m_hImageList==NULL)
	{
		//����ͼƬ
		CBitmap Image;
		BITMAP ImageInfo;
		Image.LoadBitmap(IDB_USER_STATUS_IMAGE);

		//����ͼƬ
		Image.GetBitmap(&ImageInfo);
		m_ImageUserStatus.Create(USER_STATUS_CX,ImageInfo.bmHeight,ILC_COLOR16|ILC_MASK,0,0);

		//����ͼƬ
		m_ImageUserStatus.Add(&Image,RGB(255,0,255));
	}

	//���ÿؼ�
	SetBkColor(m_ColorUserList.crNormalBK);
	SetImageList(&m_ImageUserStatus,LVSIL_SMALL);

	//ռλ��־
	m_cbDataDescribe[m_wColumnCount]=UD_NULL;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,0,0);

	//�û���־
	m_cbDataDescribe[m_wColumnCount]=UD_IMAGE_FLAG;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,USER_STATUS_CX,0);

	//�û�״̬
	m_cbDataDescribe[m_wColumnCount]=UD_IMAGE_STATUS;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,USER_STATUS_CX,0);

	return;
}

//��ȡλ��
INT CUserListControl::GetInsertIndex(VOID * pItemData)
{
	//��������
	INT nItemCount=GetItemCount();
	IClientUserItem * pIClientUserItem=GetClientUserItem(pItemData);

	//�Լ��ж�
	if (pIClientUserItem->GetUserID()==m_dwMySelfUserID)
	{
		return 0L;
	}

	//��ͨ�û�
	BYTE cbCompanion=pIClientUserItem->GetUserCompanion();
	if ((pIClientUserItem->GetMasterOrder()==0)&&(pIClientUserItem->GetMemberOrder()==0)&&(cbCompanion!=CP_FRIEND))
	{
		return nItemCount;
	}

	//��������
	IClientUserItem * pIUserItemTemp=NULL;
	LONG nImportOrder=pIClientUserItem->GetImportOrder();

	//��ȡλ��
	for (INT i=0;i<nItemCount;i++)
	{
		//��ȡ�û�
		DWORD_PTR dwItemData=GetItemData(i);
		IClientUserItem * pIUserItemTemp=GetClientUserItem((VOID *)dwItemData);

		//�Լ��ж�
		if (pIUserItemTemp->GetUserID()==m_dwMySelfUserID) continue;

		//�Աȵȼ�
		if (nImportOrder>pIUserItemTemp->GetImportOrder()) return i;
	}

	return nItemCount;
}

//��������
INT CUserListControl::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort)
{
	//�������
	IClientUserItem * pIClientUserItem1=GetClientUserItem((VOID *)lParam1);
	IClientUserItem * pIClientUserItem2=GetClientUserItem((VOID *)lParam2);

	//�Լ��ö�
	if (pIClientUserItem1->GetUserID()==m_dwMySelfUserID) return SORT_FRONT;
	if (pIClientUserItem2->GetUserID()==m_dwMySelfUserID) return SORT_AFTER;

	//�Ա�����
	switch (m_cbDataDescribe[wColumnIndex])
	{
	case UD_GAME_ID:		//��Ϸ��ʶ
		{
			//��ȡ����
			DWORD dwGameID1=pIClientUserItem1->GetGameID();
			DWORD dwGameID2=pIClientUserItem2->GetGameID();

			//�Ա�����
			INT nResult=dwGameID1>dwGameID2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_USER_ID:		//�û���ʶ
		{
			//��ȡ����
			DWORD dwUserID1=pIClientUserItem1->GetUserID();
			DWORD dwUserID2=pIClientUserItem2->GetUserID();

			//�Ա�����
			INT nResult=dwUserID1>dwUserID2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_NICKNAME:		//�û��ǳ�
		{
			//��ȡ����
			BYTE cbCompanion1=pIClientUserItem1->GetUserCompanion();
			BYTE cbCompanion2=pIClientUserItem2->GetUserCompanion();

			//�����ö�
			if (((cbCompanion1==CP_FRIEND)&&(cbCompanion2!=CP_FRIEND))
				||((cbCompanion1!=CP_FRIEND)&&(cbCompanion2==CP_FRIEND)))
			{
				return (cbCompanion1==CP_FRIEND)?SORT_FRONT:SORT_AFTER;
			}

			//��ȡ����
			BYTE cbMasterRight1=pIClientUserItem1->GetMasterOrder();
			BYTE cbMasterRight2=pIClientUserItem2->GetMasterOrder();

			//�����ö�
			if (cbMasterRight1!=cbMasterRight2)
			{
				return (cbMasterRight1==cbMasterRight2)?SORT_FRONT:SORT_AFTER;
			}
			
			//��ȡ����
			BYTE cbMemberRight1=pIClientUserItem1->GetMemberOrder();
			BYTE cbMemberRight2=pIClientUserItem2->GetMemberOrder();

			//��Ա�ö�
			if (cbMemberRight1!=cbMemberRight2)
			{
				return (cbMemberRight1==cbMemberRight2)?SORT_FRONT:SORT_AFTER;
			}

			//��ȡ����
			LPCTSTR pszNickName1=pIClientUserItem1->GetNickName();
			LPCTSTR pszNickName2=pIClientUserItem2->GetNickName();
			
			//�Ա�����
			INT nResult=lstrcmp(pszNickName1,pszNickName2);
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_GENDER:			//�û��Ա�
		{
			//��ȡ����
			BYTE cbGender1=pIClientUserItem1->GetGender();
			BYTE cbGender2=pIClientUserItem2->GetGender();

			//�Ա�����
			INT nResult=cbGender1>cbGender2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_GROUP_NAME:		//��������
		{
			//��ȡ����
			LPCTSTR pszGroupName1=pIClientUserItem1->GetGroupName();
			LPCTSTR pszGroupName2=pIClientUserItem2->GetGroupName();

			//�Ա�����
			INT nResult=lstrcmp(pszGroupName1,pszGroupName2);
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_UNDER_WRITE:	//����ǩ��
		{
			//��ȡ����
			LPCTSTR pszUnderWrite1=pIClientUserItem1->GetUnderWrite();
			LPCTSTR pszUnderWrite2=pIClientUserItem2->GetUnderWrite();

			//�Ա�����
			INT nResult=lstrcmp(pszUnderWrite1,pszUnderWrite2);
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_TABLE:			//���Ӻ���
		{
			//��ȡ����
			WORD wTableID1=pIClientUserItem1->GetTableID();
			WORD wTableID2=pIClientUserItem2->GetTableID();

			//�Ա�����
			INT nResult=wTableID1>wTableID2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_CHAIR:			//���Ӻ���
		{
			//��ȡ����
			WORD wChairID1=pIClientUserItem1->GetChairID();
			WORD wChairID2=pIClientUserItem2->GetChairID();

			//�Ա�����
			INT nResult=wChairID1>wChairID2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_SCORE:			//�û�����
		{
			//��ȡ����
			SCORE lScore1=pIClientUserItem1->GetUserScore();
			SCORE lScore2=pIClientUserItem2->GetUserScore();

			//�Ա�����
			INT nResult=lScore1>lScore2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_GRADE:			//�û��ɼ�
		{
			//��ȡ����
			SCORE lBanker1=pIClientUserItem1->GetUserGrade();
			SCORE lBanker2=pIClientUserItem2->GetUserGrade();

			//�Ա�����
			INT nResult=lBanker1>lBanker2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_USER_MEDAL:		//�û�����
		{
			//��ȡ����
			DWORD dwUserMedal1=pIClientUserItem1->GetUserMedal();
			DWORD dwUserMedal2=pIClientUserItem2->GetUserMedal();

			//�Ա�����
			INT nResult=dwUserMedal1>dwUserMedal2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_EXPERIENCE:		//�û�����
		{
			//��ȡ����
			DWORD dwExperience1=pIClientUserItem1->GetUserExperience();
			DWORD dwExperience2=pIClientUserItem2->GetUserExperience();

			//�Ա�����
			INT nResult=dwExperience1>dwExperience2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_LOVELINESS:		//�û�����
		{
			//��ȡ����
			DWORD dwLoveLiness1=pIClientUserItem1->GetUserLoveLiness();
			DWORD dwLoveLiness2=pIClientUserItem2->GetUserLoveLiness();

			//�Ա�����
			INT nResult=dwLoveLiness1>dwLoveLiness2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_WIN_COUNT:		//ʤ������
		{
			//��ȡ����
			LONG lWinCount1=pIClientUserItem1->GetUserWinCount();
			LONG lWinCount2=pIClientUserItem2->GetUserWinCount();

			//�Ա�����
			INT nResult=lWinCount1>lWinCount2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_LOST_COUNT:		//�������
		{
			//��ȡ����
			LONG lLostCount1=pIClientUserItem1->GetUserLostCount();
			LONG lLostCount2=pIClientUserItem2->GetUserLostCount();

			//�Ա�����
			INT nResult=lLostCount1>lLostCount2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_DRAW_COUNT:		//�;�����
		{
			//��ȡ����
			LONG lDrawCount1=pIClientUserItem1->GetUserDrawCount();
			LONG lDrawCount2=pIClientUserItem2->GetUserDrawCount();

			//�Ա�����
			INT nResult=lDrawCount1>lDrawCount2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_FLEE_COUNT:		//�Ӿ�����
		{
			//��ȡ����
			LONG lFleeCount1=pIClientUserItem1->GetUserFleeCount();
			LONG lFleeCount2=pIClientUserItem2->GetUserFleeCount();

			//�Ա�����
			INT nResult=lFleeCount1>lFleeCount2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_PLAY_COUNT:		//��Ϸ����
		{
			//��ȡ����
			LONG lPlayCount1=pIClientUserItem1->GetUserPlayCount();
			LONG lPlayCount2=pIClientUserItem2->GetUserPlayCount();

			//�Ա�����
			INT nResult=lPlayCount1>lPlayCount2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_WIN_RATE:		//�û�ʤ��
		{
			//��ȡ����
			FLOAT fWinRate1=pIClientUserItem1->GetUserWinRate();
			FLOAT fWinRate2=pIClientUserItem2->GetUserWinRate();

			//�Ա�����
			INT nResult=fWinRate1>fWinRate2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_LOST_RATE:		//�û�����
		{
			//��ȡ����
			FLOAT fLostRate1=pIClientUserItem1->GetUserLostRate();
			FLOAT fLostRate2=pIClientUserItem2->GetUserLostRate();

			//�Ա�����
			INT nResult=fLostRate1>fLostRate2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_DRAW_RATE:		//�û�����
		{
			//��ȡ����
			FLOAT fDrawRate1=pIClientUserItem1->GetUserDrawRate();
			FLOAT fDrawRate2=pIClientUserItem2->GetUserDrawRate();

			//�Ա�����
			INT nResult=fDrawRate1>fDrawRate2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_FLEE_RATE:		//�û�����
		{
			//��ȡ����
			FLOAT fFleeRate1=pIClientUserItem1->GetUserFleeRate();
			FLOAT fFleeRate2=pIClientUserItem2->GetUserFleeRate();

			//�Ա�����
			INT nResult=fFleeRate1>fFleeRate2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_GAME_LEVEL:		//��Ϸ�ȼ�
		{
			//��ȡ����
			SCORE lScore1=pIClientUserItem1->GetUserScore();
			SCORE lScore2=pIClientUserItem2->GetUserScore();

			//�Ա�����
			INT nResult=lScore1>lScore2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_NOTE_INFO:		//�û���ע
		{
			//��ȡ����
			LPCTSTR pszNoteInfo1=pIClientUserItem1->GetUserNoteInfo();
			LPCTSTR pszNoteInfo2=pIClientUserItem2->GetUserNoteInfo();

			//�Ա�����
			INT nResult=lstrcmp(pszNoteInfo1,pszNoteInfo2);
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_IMAGE_STATUS:	//�û�״̬
		{
			//��ȡ����
			BYTE cbUserStatus1=pIClientUserItem1->GetUserStatus();
			BYTE cbUserStatus2=pIClientUserItem2->GetUserStatus();

			//�Ա�����
			INT nResult=cbUserStatus1>cbUserStatus2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	}

	return 0L;
}

//�����ַ�
VOID CUserListControl::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//Ч�����
	ASSERT(wColumnIndex<m_wColumnCount);
	if (wColumnIndex>=m_wColumnCount) return;

	//��������
	ASSERT(GetClientUserItem(pItemData)!=NULL);
	IClientUserItem * pIClientUserItem=GetClientUserItem(pItemData);

	//�����ַ�
	switch (m_cbDataDescribe[wColumnIndex])
	{
	case UD_GAME_ID:		//��Ϸ��ʶ
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetGameID());
			break;
		}
	case UD_USER_ID:		//�û���ʶ
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserID());
			break;
		}
	case UD_NICKNAME:		//�û��ǳ�
		{
			_sntprintf(pszString,wMaxCount,TEXT("%s"),pIClientUserItem->GetNickName());
			break;
		}
	case UD_GENDER:			//�û��Ա�
		{
			BYTE cbGender=pIClientUserItem->GetGender();
			lstrcpyn(pszString,(cbGender==GENDER_MANKIND)?TEXT("��"):TEXT("Ů"),wMaxCount);
			break;
		}
	case UD_GROUP_NAME:		//��������
		{
			_sntprintf(pszString,wMaxCount,TEXT("%s"),pIClientUserItem->GetGroupName());
			break;
		}
	case UD_UNDER_WRITE:	//����ǩ��
		{
			_sntprintf(pszString,wMaxCount,TEXT("%s"),pIClientUserItem->GetUnderWrite());
			break;
		}
	case UD_TABLE:			//���Ӻ���
		{
			WORD wTableID=pIClientUserItem->GetTableID();
			if (wTableID!=INVALID_TABLE) _sntprintf(pszString,wMaxCount,TEXT("%ld"),wTableID+1);
			break;
		}
	case UD_CHAIR:			//���Ӻ���
		{
			WORD wChairID=pIClientUserItem->GetChairID();
			if (wChairID!=INVALID_TABLE) _sntprintf(pszString,wMaxCount,TEXT("%ld"),wChairID+1);
			break;
		}
	case UD_SCORE:			//�û�����
		{
			_sntprintf(pszString,wMaxCount,TEXT("%I64d"),pIClientUserItem->GetUserScore());
			break;
		}
	case UD_GRADE:			//�û��ɼ�
		{
			_sntprintf(pszString,wMaxCount,TEXT("%I64d"),pIClientUserItem->GetUserGrade());
			break;
		}
	case UD_USER_MEDAL:		//�û�����
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserMedal());
			break;
		}
	case UD_EXPERIENCE:		//�û�����
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserExperience());
			break;
		}
	case UD_LOVELINESS:		//�û�����
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserLoveLiness());
			break;
		}
	case UD_WIN_COUNT:		//ʤ������
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserWinCount());
			break;
		}
	case UD_LOST_COUNT:		//�������
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserLostCount());
			break;
		}
	case UD_DRAW_COUNT:		//�;�����
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserDrawCount());
			break;
		}
	case UD_FLEE_COUNT:		//�Ӿ�����
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserFleeCount());
			break;
		}
	case UD_PLAY_COUNT:		//��Ϸ����
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserPlayCount());
			break;
		}
	case UD_WIN_RATE:		//�û�ʤ��
		{
			FLOAT fWinRate=pIClientUserItem->GetUserWinRate();
			if (fWinRate!=0.0f) _sntprintf(pszString,wMaxCount,TEXT("%4.2f%%"),fWinRate);
			break;
		}
	case UD_LOST_RATE:		//�û�����
		{
			FLOAT fLostRate=pIClientUserItem->GetUserLostRate();
			if (fLostRate==0.0f) _sntprintf(pszString,wMaxCount,TEXT("%4.2f%%"),fLostRate);
			break;
		}
	case UD_DRAW_RATE:		//�û�����
		{
			FLOAT fDrawRate=pIClientUserItem->GetUserDrawRate();
			if (fDrawRate==0.0f) _sntprintf(pszString,wMaxCount,TEXT("%4.2f%%"),fDrawRate);
			break;
		}
	case UD_FLEE_RATE:		//�û�����
		{
			FLOAT fFleeRate=pIClientUserItem->GetUserFleeRate();
			if (fFleeRate==0.0f) _sntprintf(pszString,wMaxCount,TEXT("%4.2f%%"),fFleeRate);
			break;
		}
	case UD_GAME_LEVEL:		//��Ϸ�ȼ�
		{
			//��Ч����
			if (m_pIGameLevelParser==NULL)
			{
				ASSERT(FALSE);
				break;
			}
				
			//��ȡ�ȼ�
			_sntprintf(pszString,wMaxCount,TEXT("%s"),m_pIGameLevelParser->GetLevelDescribe(pIClientUserItem));

			break;
		}
	case UD_NOTE_INFO:		//�û���ע
		{
			lstrcpyn(pszString,pIClientUserItem->GetUserNoteInfo(),wMaxCount);
			break;
		}
	case UD_LOOKON_USER:	//�Թ��û�
		{
			//��Ч����
			if (m_pIGameUserManager==NULL)
			{
				ASSERT(FALSE);
				break;
			}

			//��ȡ�û�
			if (pIClientUserItem->GetUserStatus()==US_LOOKON)
			{
				//��ȡ��Ϣ
				WORD wChairID=pIClientUserItem->GetChairID();
				IClientUserItem * pITableUserItem=m_pIGameUserManager->GetTableUserItem(wChairID);

				//�������
				if ((pITableUserItem!=NULL)&&(pITableUserItem!=pIClientUserItem))
				{
					lstrcpyn(pszString,pITableUserItem->GetNickName(),wMaxCount);
				}
			}

			break;
		}
	}

	return;
}

//��ȡ��ɫ
VOID CUserListControl::GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF&crColorText, COLORREF&crColorBack)
{
	//���Ʊ���
	ASSERT(GetClientUserItem((VOID *)lpDrawItemStruct->itemData)!=NULL);
	IClientUserItem * pIClientUserItem=GetClientUserItem((VOID *)lpDrawItemStruct->itemData);

	//��ɫ����
	if (lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		//ѡ����ɫ
		crColorText=m_ColorUserList.crSelectTX;
		crColorBack=m_ColorUserList.crSelectBK;
	}
	else if (pIClientUserItem->GetUserID()==m_dwMySelfUserID)
	{
		//�Լ���ɫ
		crColorText=m_ColorUserList.crMyselfTX;
		crColorBack=m_ColorUserList.crMasterBK;
	}
	else if (pIClientUserItem->GetMasterOrder()!=0)
	{
		//������ɫ
		crColorText=m_ColorUserList.crMasterTX;
		crColorBack=m_ColorUserList.crMasterBK;
	}
	else if (pIClientUserItem->GetMemberOrder()!=0)
	{
		//��Ա��ɫ
		crColorText=m_ColorUserList.crMemberTX;
		crColorBack=m_ColorUserList.crMemberBK;
	}
	else
	{
		//Ĭ����ɫ
		crColorText=m_ColorUserList.crNormalTX;
		crColorBack=m_ColorUserList.crNormalBK;
	}

	return;
}

//�滭����
VOID CUserListControl::DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect&rcSubItem, INT nColumnIndex)
{
	//����Ч��
	ASSERT(nColumnIndex<m_wColumnCount);
	if (nColumnIndex>=m_wColumnCount) return;

	//��������
	INT nItemID=lpDrawItemStruct->itemID;
	IClientUserItem * pIClientUserItem=GetClientUserItem((VOID *)lpDrawItemStruct->itemData);

	//�滭��Ϣ
	switch (m_cbDataDescribe[nColumnIndex])
	{
	case UD_IMAGE_FLAG:		//�û���־
		{
			//��������
			UINT uImageIndex=INDEX_NORMAL;
			BYTE cbCompanion=pIClientUserItem->GetUserCompanion();
			tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

			//��������
			if ((cbCompanion==CP_FRIEND)&&(pIClientUserItem->GetUserID()!=m_dwMySelfUserID)) uImageIndex=INDEX_FRIEND;
			if ((cbCompanion==CP_DETEST)&&(pIClientUserItem->GetUserID()!=m_dwMySelfUserID)) uImageIndex=INDEX_FRIEND+1;
			if ((uImageIndex==INDEX_NORMAL)&&(pIClientUserItem->GetMasterOrder()>0)) uImageIndex=INDEX_MASTER+(pUserInfo->cbMasterOrder-1);
			if ((uImageIndex==INDEX_NORMAL)&&(pIClientUserItem->GetMemberOrder()>0)) uImageIndex=INDEX_MEMBER+(pUserInfo->cbMemberOrder-1);

			//�滭��־
			m_ImageUserStatus.Draw(pDC,uImageIndex,CPoint(rcSubItem.left,rcSubItem.top),ILD_TRANSPARENT);

			break;
		}
	case UD_IMAGE_STATUS:	//�û�״̬
		{
			//��������
			BYTE cbGender=pIClientUserItem->GetGender();
			BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
			UINT uImageIndex=INDEX_STATUS+cbGender*COUNT_STATUS+cbUserStatus-US_FREE;

			//�滭��־
			m_ImageUserStatus.Draw(pDC,uImageIndex,CPoint(rcSubItem.left,rcSubItem.top),ILD_TRANSPARENT);

			break;
		}
	case UD_EXPERIENCE:	//�û�����
		{
			//��������
			ASSERT(CUserItemElement::GetInstance()!=NULL);
			CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

			//�滭����
			DWORD dwExperience=pIClientUserItem->GetUserExperience();
			pUserItemElement->DrawExperience(pDC,rcSubItem.left,rcSubItem.top,dwExperience);

			break;
		}
	default:			//Ĭ�ϵ���
		{
			__super::DrawCustomItem(pDC,lpDrawItemStruct,rcSubItem,nColumnIndex);
		}
	}

	return;
}

//�����û�
IClientUserItem * CUserListControl::GetClientUserItem(VOID * pItemData)
{
	return (IClientUserItem *)pItemData;
}

//���ýӿ�
bool CUserListControl::SetMySelfUserID(DWORD dwMySelfUserID)
{
	//���ñ���
	m_dwMySelfUserID=dwMySelfUserID;

	return true;
}

//���ýӿ�
bool CUserListControl::SetGameLevelParser(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGameLevelParser)!=NULL);
		m_pIGameLevelParser=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGameLevelParser);

		//�ɹ��ж�
		if (m_pIGameLevelParser==NULL) return false;
	}
	else m_pIGameLevelParser=NULL;

	return true;
}

//���ýӿ�
bool CUserListControl::SetGameUserManager(IUnknownEx * pIUnknownEx)
{
	//���ýӿ�
	if (pIUnknownEx!=NULL)
	{
		//��ѯ�ӿ�
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGameUserManager)!=NULL);
		m_pIGameUserManager=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGameUserManager);

		//�ɹ��ж�
		if (m_pIGameUserManager==NULL) return false;
	}
	else m_pIGameUserManager=NULL;

	return true;
}

//������ɫ
bool CUserListControl::SetColorUserList(tagColorUserList&ColorUserList)
{
	//���ñ���
	m_ColorUserList=ColorUserList;

	//���½���
	if (m_hWnd!=NULL)
	{
		//���ñ���
		SetBkColor(m_ColorUserList.crNormalBK);

		//�ػ�����
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}

	return true;
}

//�����б�
bool CUserListControl::SetColumnDescribe(tagColumnItem ColumnItem[], WORD wColumnCount)
{
	//�����ж�
	if (m_bCreateColumn==true) return true;

	//��������
	ASSERT(wColumnCount<(CountArray(m_cbDataDescribe)-m_wColumnCount));
	wColumnCount=__min(CountArray(m_cbDataDescribe)-m_wColumnCount,wColumnCount);

	//���ñ���
	m_bCreateColumn=true;

	//�����б�
	for (WORD i=0;i<wColumnCount;i++)
	{
		m_cbDataDescribe[m_wColumnCount]=ColumnItem[i].cbDataDescribe;
		InsertColumn(m_wColumnCount++,ColumnItem[i].szColumnName,LVCFMT_LEFT,ColumnItem[i].cbColumnWidth);
	}

	return true;
}

//������Ϣ
VOID CUserListControl::OnNcDestroy()
{
	//��Դ����
	m_dwMySelfUserID=0L;
	m_bCreateColumn=false;

	//�ӿڱ���
	m_pIGameLevelParser=NULL;
	m_pIGameUserManager=NULL;

	//�б���Ϣ
	m_wColumnCount=0;
	ZeroMemory(m_cbDataDescribe,sizeof(m_cbDataDescribe));

	__super::OnNcDestroy();
}

//////////////////////////////////////////////////////////////////////////////////
