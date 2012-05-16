#include "Stdafx.h"
#include "Resource.h"
#include "GlobalUnits.h"
#include "UserListView.h"

//��̬����
CImageList		CUserListView::m_StatusImage;						//״̬λͼ

//������Ϣ�ṹ
struct tagListSortInfo
{
	int						nItemIndex;							//��������
	BYTE					cbAscendSort;						//�����־
	WORD					cbFieldKind;						//�ֶ�����
	CUserListView			* pRoomListCtrl;					//�б�ָ��
};

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUserListView, CSkinListCtrl)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CUserListView::CUserListView()
{
	m_uImageSpace=0;
	m_wColumnCount=0;
	m_cbAscendSort=FALSE;
	memset(m_wDataDescribe,0,sizeof(m_wDataDescribe));
}

//��������
CUserListView::~CUserListView()
{
}

//��ʼ���б�
bool CUserListView::InitUserListView()
{
	//���ر���ͼ
	if (m_StatusImage.GetSafeHandle()==NULL)
	{
		CBitmap Image;
		BITMAP ImageInfo;
		Image.LoadBitmap(IDB_USER_STATUS);
		Image.GetBitmap(&ImageInfo);
		m_StatusImage.Create(IMGAE_SPACE,ImageInfo.bmHeight,ILC_COLOR16|ILC_MASK,0,0);
		m_StatusImage.Add(&Image,RGB(255,0,255));
	}
	SetImageList(&m_StatusImage,LVSIL_SMALL);

	//������ɫ
	SetTextColor(RGB(10,10,10));
	SetBkColor(RGB(230,249,255));
	SetTextBkColor(RGB(230,249,255));

	return true;
}

//��������Ϣ
bool CUserListView::SetColumnDescribe(tagColumnItem ColumnItem[], WORD wColumnCount)
{
	//Ч��״̬
	ASSERT(ColumnItem!=NULL);
	if (GetSafeHwnd()==NULL) return false;

	//ɾ������Ϣ
	m_wColumnCount=0;
	WORD wTempCount=m_SkinHeadCtrl.GetItemCount();
	for (WORD i=0;i<wTempCount;i++) DeleteColumn(0);

	//��������
	wColumnCount=__min(MAX_COLUMN-1,wColumnCount);

	//��������Ϣ
	SetRedraw(FALSE);
	for (WORD i=0;i<wColumnCount;i++)
	{
		m_wDataDescribe[i]=ColumnItem[i].wDataDescribe;
		if (m_wColumnCount==0) InsertColumn(m_wColumnCount++,ColumnItem[i].szColumnName,LVCFMT_LEFT,ColumnItem[i].wColumnWidth+m_uImageSpace);
		else InsertColumn(m_wColumnCount++,ColumnItem[i].szColumnName,LVCFMT_LEFT,ColumnItem[i].wColumnWidth);
	}
	SetRedraw(TRUE);

	return true;
}

//�����û�
bool CUserListView::InsertUserItem(IUserItem * pIUserItem)
{
	//Ч��״̬
	ASSERT(pIUserItem!=NULL);
	if (GetSafeHwnd()==NULL) return false;
	
	//�����û�
	LVFINDINFO FindInfo;
	memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pIUserItem;
	int iItem=FindItem(&FindInfo);
	if (iItem!=-1)
	{
		UpdateUserItem(pIUserItem);
		return true;
	}

	//д���û�
	if (m_wColumnCount>0) 
	{
		int nImageStation=GetImageStation(pIUserItem);
		int nInsertStation=GetInsertStation(pIUserItem);
		iItem=InsertItem(LVIF_IMAGE|LVIF_TEXT|LVIF_PARAM,nInsertStation,GetDisplayText(pIUserItem,0),
			0,0,nImageStation,(LPARAM)pIUserItem);
	}
	for (WORD i=1;i<m_wColumnCount;i++)
	{
		SetItemText(iItem,i,GetDisplayText(pIUserItem,i));
	}

	//�����б�
	UpdataUserList();

	return true;
}

//�����û�
bool CUserListView::UpdateUserItem(IUserItem * pIUserItem)
{
	//Ч��״̬
	ASSERT(pIUserItem!=NULL);
	if (GetSafeHwnd()==NULL) return false;
	
	//�����û�
	LVFINDINFO FindInfo;
	memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pIUserItem;
	int iItem=FindItem(&FindInfo);
	if (iItem==-1) return false;

	//�����û�
	if (m_wColumnCount>0)
	{
		int nImageStation=GetImageStation(pIUserItem);
		SetItem(iItem,0,LVIF_IMAGE|LVIF_TEXT,GetDisplayText(pIUserItem,0),nImageStation,0,0,0);
	}
	for (WORD i=1;i<m_wColumnCount;i++) 
	{
		SetItemText(iItem,i,GetDisplayText(pIUserItem,i));
	}

	return true;
}

//ɾ���û�
bool CUserListView::DeleteUserItem(IUserItem * pIUserItem)
{
	//Ч��״̬
	ASSERT(pIUserItem!=NULL);
	if (GetSafeHwnd()==NULL) return false;
	
	//�����û�
	LVFINDINFO FindInfo;
	memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pIUserItem;
	int iItem=FindItem(&FindInfo);
	if (iItem!=-1)
	{
		DeleteItem(iItem);
		return true;
	}

	return false;
}

//�����ö�
bool CUserListView::UpdataUserList()
{
	//��������
	tagListSortInfo SortInfo;
	int iSubItem=0;
	SortInfo.cbAscendSort=m_cbAscendSort;
	SortInfo.pRoomListCtrl=this;
	SortInfo.nItemIndex=iSubItem;
	SortInfo.cbFieldKind=m_wDataDescribe[iSubItem];

	//�����б�
	SortItems(SortFun,(LPARAM)&SortInfo);

	return true;
}

//��ȡλͼ
int CUserListView::GetImageStation(IUserItem * pIUserItem)
{
	//��������
	tagUserData * pUserData=pIUserItem->GetUserData();
	return GetImageStation(pUserData);
}

//��ȡλͼ
int CUserListView::GetImageStation(tagUserData * pUserData)
{
	//��������
	BYTE cbUserStatus=pUserData->cbUserStatus;

	//Ȩ�޶Ա�
	int nRightExcursion=0;
	if (CUserRight::IsMatchUser(pUserData->dwUserRight)==true) nRightExcursion=9;
	else if (pUserData->cbCompanion==enCompanion_Friend) nRightExcursion=1;
	else if (pUserData->cbCompanion==enCompanion_Detest) nRightExcursion=2;
	else if (pUserData->cbMasterOrder!=0) nRightExcursion=pUserData->cbMasterOrder+7;
	else if (pUserData->cbMemberOrder!=0) nRightExcursion=pUserData->cbMemberOrder+2;

	//����ƫ��
	return nRightExcursion*6+(cbUserStatus>=US_FREE?(cbUserStatus-US_FREE):0);
}

//��ȡ����
int CUserListView::GetInsertStation(IUserItem * pIUserItem)
{
	return GetItemCount();
}

//��ȡ�ַ�
LPCTSTR CUserListView::GetDisplayText(IUserItem * pIUserItem, WORD wColumnPos)
{
	//Ч�����
	ASSERT(pIUserItem!=NULL);
	if (wColumnPos>=m_wColumnCount) return NULL;

	//��������
	static TCHAR szBuffer[512];
	tagUserData * pUserData=pIUserItem->GetUserData();

	//������ʾ
	switch (m_wDataDescribe[wColumnPos])
	{
	case DTP_USER_ID:			//�û� ID
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pIUserItem->GetUserData()->dwUserID);
			return szBuffer;
		}
	case DTP_GAME_ID:			//��Ϸ ID
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pIUserItem->GetUserData()->dwGameID);
			return szBuffer;
		}
	case DTP_INSURE_SCORE:		//�洢���
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pIUserItem->GetUserData()->lInsureScore);
			return szBuffer;
		}
	case DTP_GAME_GOLD:			//��Ϸ���
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pIUserItem->GetUserData()->lGameGold);
			return szBuffer;
		}
	case DTP_LOVE_LINESS:		//�û�����
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pIUserItem->GetUserData()->lLoveliness);
			return szBuffer;
		}
	case DTP_USER_ACCOUNTS:		//�û�����
		{
			return pIUserItem->GetUserData()->szName;
		}
	case DTP_UNDER_WRITE:		//����ǩ��
		{
			return pIUserItem->GetUserData()->szUnderWrite;
		}
	case DTP_USER_TABLE:		//��Ϸ����
		{
			WORD wTableID=pUserData->wTableID;
			if (wTableID!=INVALID_TABLE) _snprintf(szBuffer,sizeof(szBuffer),TEXT("%3d"),wTableID+1);
			else szBuffer[0]=0;
			return szBuffer;
		}
	case DTP_USER_SCORE:		//�û�����
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->lScore);
			return szBuffer;
		}
	case DTP_USER_EXPERIENCE:	//�û�����
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->lExperience);
			return szBuffer;
		}
	case DTP_USER_WIN:			//ʤ������
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->lWinCount);
			return szBuffer;
		}
	case DTP_USER_LOST:			//�������
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->lLostCount);
			return szBuffer;
		}
	case DTP_USER_DRAW:			//�;�����
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->lDrawCount);
			return szBuffer;
		}
	case DTP_USER_FLEE:			//�Ӿ�����
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->lFleeCount);
			return szBuffer;
		}
	case DTP_USER_PLAY_COUNT:	//�ܾ�����
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pIUserItem->GetUserPlayCount());
			return szBuffer;
		}
	case DTP_WIN_RATE:			//�û�ʤ��
		{
			if (pUserData->lWinCount>0)
			{
				LONG lAllCount=pIUserItem->GetUserPlayCount();
				double dWinRate=(double)(pUserData->lWinCount*100)/(double)lAllCount;
				if (dWinRate>=0.01) _snprintf(szBuffer,sizeof(szBuffer),TEXT("%5.2f%%"),dWinRate);
				else szBuffer[0]=0;
			}
			else szBuffer[0]=0;
			return szBuffer;
		}
	case DTP_LOST_RATE:			//�û�����
		{
			if (pUserData->lLostCount>0)
			{
				LONG lAllCount=pIUserItem->GetUserPlayCount();
				double dLostRate=(double)(pUserData->lLostCount*100)/(double)lAllCount;
				if (dLostRate>=0.01) _snprintf(szBuffer,sizeof(szBuffer),TEXT("%5.2f%%"),dLostRate);
				else szBuffer[0]=0;
			}
			else szBuffer[0]=0;
			return szBuffer;
		}
	case DTP_DRAW_RATE:			//�û�����
		{
			if (pUserData->lDrawCount>0)
			{
				LONG lAllCount=pIUserItem->GetUserPlayCount();
				double dDrawRate=(double)(pUserData->lDrawCount*100)/(double)lAllCount;
				if (dDrawRate>=0.01) _snprintf(szBuffer,sizeof(szBuffer),TEXT("%5.2f%%"),dDrawRate);
				else szBuffer[0]=0;
			}
			else szBuffer[0]=0;
			return szBuffer;
		}
	case DTP_FLEE_RATE:			//�û�����
		{
			if (pUserData->lFleeCount>0)
			{
				LONG lAllCount=pIUserItem->GetUserPlayCount();
				double dFleeRate=(double)(pUserData->lFleeCount*100)/(double)lAllCount;
				if (dFleeRate>=0.01) _snprintf(szBuffer,sizeof(szBuffer),TEXT("%5.2f%%"),dFleeRate);
				else szBuffer[0]=0;
			}
			else szBuffer[0]=0;
			return szBuffer;
		}
	case DTP_GAME_LEVEL:		//��Ϸ�ȼ�
		{
			szBuffer[0]=0;
			if (m_GameRankManager.GetInterface()!=NULL)
			{
				LPCTSTR pszGameRank=m_GameRankManager->GetLevelDescribe(pIUserItem->GetUserData()->lScore);
				lstrcpyn(szBuffer,pszGameRank,sizeof(szBuffer));
			}
			return szBuffer;
		}
	}

	//Ĭ����ʾ
	szBuffer[0]=0;

	return szBuffer;
}

//�滭����
void CUserListView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rcItem=lpDrawItemStruct->rcItem;
	CDC * pDC=CDC::FromHandle(lpDrawItemStruct->hDC);

	int iItem=lpDrawItemStruct->itemID;
	int iCount=GetHeaderCtrl()->GetItemCount();
	int iBeen=rcItem.left;
	TCHAR szBuffer[1024];
	IUserItem *pIUserItem = (IUserItem *)GetItemData(iItem);

	GetItemRect(iItem, rcItem, LVIR_LABEL);

	for (int i=0;i<iCount;i++)
	{
		CRect rcSubItem;
		int iWidth=GetColumnWidth(i);

		GetSubItemRect(iItem,i,LVIR_LABEL,rcSubItem);
		if ( i == 0 )
		{
			GetSubItemRect(iItem,i,LVIR_ICON,rcSubItem);

			if (lpDrawItemStruct->itemState&ODS_FOCUS) pDC->FillSolidRect(&rcSubItem,RGB(10,36,106)); 
			else pDC->FillSolidRect(&rcSubItem,GetBkColor());

			if (lpDrawItemStruct->itemState&ODS_FOCUS) pDC->SetTextColor(RGB(255,255,255));
			else if ( 0 < pIUserItem->GetUserData()->cbMemberOrder && DTP_USER_ACCOUNTS== m_wDataDescribe[i]) pDC->SetTextColor(RGB(255,0,0));
			else pDC->SetTextColor(RGB(0,0,0));

			//�滭��־
			UINT uImageIndex=GetImageStation(pIUserItem);
			m_StatusImage.Draw(pDC,uImageIndex,CPoint(rcSubItem.left,rcSubItem.top),ILD_TRANSPARENT);

			GetItemText(iItem,i,szBuffer,sizeof(szBuffer));

			GetSubItemRect(iItem,i,LVIR_LABEL,rcSubItem);

			if (lpDrawItemStruct->itemState&ODS_FOCUS) pDC->FillSolidRect(&rcSubItem,RGB(10,36,106)); 
			else pDC->FillSolidRect(&rcSubItem,GetBkColor());

			rcSubItem.top+=3;
			pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcSubItem,DT_LEFT|DT_END_ELLIPSIS);
		}
		else
		{		
			
			if (lpDrawItemStruct->itemState&ODS_FOCUS) pDC->FillSolidRect(&rcSubItem,RGB(10,36,106)); 
			else pDC->FillSolidRect(&rcSubItem,GetBkColor());

			if (lpDrawItemStruct->itemState&ODS_FOCUS) pDC->SetTextColor(RGB(255,255,255));
			else if ( 0 < pIUserItem->GetUserData()->cbMemberOrder && DTP_GAME_ID == m_wDataDescribe[i]) pDC->SetTextColor(RGB(255,0,0));
			else pDC->SetTextColor(RGB(0,0,0));

			GetItemText(iItem,i,szBuffer,sizeof(szBuffer));

			CRect rcTempItem =rcSubItem;
			rcTempItem.left +=2; 
			rcTempItem.right +=2; 
			rcTempItem.top+=3;
			pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcTempItem,DT_LEFT|DT_END_ELLIPSIS);
		}
	}

	//�滭ѡ��
	if (lpDrawItemStruct->itemState&ODS_FOCUS) pDC->DrawFocusRect(&rcItem);

	return;
}

//////////////////////////////////////////////////////////////////////////

//�������Ϣ
void CUserListView::OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult) 
{
	NM_LISTVIEW * pNMListView=(NM_LISTVIEW *)pNMHDR;

	if (m_wColumnCount!=0)
	{
		//��������
		tagListSortInfo SortInfo;
		int iSubItem=pNMListView->iSubItem;
		SortInfo.cbAscendSort=m_cbAscendSort;
		SortInfo.pRoomListCtrl=this;
		SortInfo.nItemIndex=iSubItem;
		SortInfo.cbFieldKind=m_wDataDescribe[iSubItem];

		//�����б�
		SortItems(SortFun,(LPARAM)&SortInfo);
		m_cbAscendSort=!m_cbAscendSort;
	}

	return;
}

//���к���
int CALLBACK CUserListView::SortFun(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList)
{
	//�������
	IUserItem * pUserItem1=(IUserItem *)lParam1;
	IUserItem * pUserItem2=(IUserItem *)lParam2;
	tagUserData * pUserData1=pUserItem1->GetUserData();
	tagUserData * pUserData2=pUserItem2->GetUserData();
    tagListSortInfo * pSortInfo=(tagListSortInfo *)lParamList;
	CUserListView * pListCtrl=pSortInfo->pRoomListCtrl;
	ASSERT(pUserItem1!=NULL);
	ASSERT(pUserItem2!=NULL);
	ASSERT(pSortInfo!=NULL);

	//�Լ����ö�
	tagGlobalUserData & GlobalInfo=g_GlobalUnits.GetGolbalUserData();
	if (pUserData1->dwUserID==GlobalInfo.dwUserID) return -1;
	if (pUserData2->dwUserID==GlobalInfo.dwUserID) return 1;

	//�Ա�����
	switch (pSortInfo->cbFieldKind)
	{
	case DTP_USER_ID:
		{
			int iCompRes=pUserData1->dwUserID>pUserData2->dwUserID?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_GAME_ID:
		{
			int iCompRes=pUserData1->dwGameID>pUserData2->dwGameID?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_LOVE_LINESS:
		{
			int iCompRes=pUserData1->lLoveliness>pUserData2->lLoveliness?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_ACCOUNTS:
		{
			//����Ա�ö�
			if (pUserData1->cbMasterOrder!=pUserData2->cbMasterOrder)
			{
				if (pUserData1->cbMasterOrder>pUserData2->cbMasterOrder) return -1;
				else return 1;
			}

			//���ѷ��ö�
			BYTE cbConnection1=pUserData1->cbCompanion;
			BYTE cbConnection2=pUserData2->cbCompanion;
			if (((cbConnection1==enCompanion_Friend)&&(cbConnection2!=enCompanion_Friend))
				||((cbConnection1!=enCompanion_Friend)&&(cbConnection2==enCompanion_Friend)))
			{
				if (cbConnection1==enCompanion_Friend) return -1;
				else return 1;
			}
			
			//��Ա�ö�
			if (pUserData1->cbMemberOrder!=pUserData2->cbMemberOrder)
			{
				if (pUserData1->cbMemberOrder>pUserData2->cbMemberOrder) return -1;
				else return 1;
			}

			int iCompRes=strcmp(pUserData1->szName,pUserData2->szName);
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_SCORE:
		{
			int iCompRes=pUserData1->lScore>pUserData2->lScore?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_INSURE_SCORE:
		{
			int iCompRes=pUserData1->lInsureScore>pUserData2->lInsureScore?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_GAME_LEVEL:
		{
			int iCompRes=pUserData1->lScore>pUserData2->lScore?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_UNDER_WRITE:
		{
			int iCompRes=strcmp(pUserData1->szUnderWrite,pUserData2->szUnderWrite);
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_TABLE:
		{
			int iCompRes=pUserData1->wTableID>pUserData2->wTableID?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_EXPERIENCE:
		{
			int iCompRes=pUserData1->lExperience>pUserData2->lExperience?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_GROUP_NAME:
		{
			int iCompRes=strcmp(pUserData1->szGroupName,pUserData2->szGroupName);
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_WIN:
		{
			int iCompRes=pUserData1->lWinCount>pUserData2->lWinCount?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_LOST:
		{
			int iCompRes=pUserData1->lLostCount>pUserData2->lLostCount?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_DRAW:
		{
			int iCompRes=pUserData1->lDrawCount>pUserData2->lDrawCount?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_FLEE:
		{
			int iCompRes=pUserData1->lFleeCount>pUserData2->lFleeCount?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_WIN_RATE:
		{
			long lAllCount1=pUserData1->lWinCount+pUserData1->lLostCount;
			long lAllCount2=pUserData2->lWinCount+pUserData2->lLostCount;
			double dWinRate1=0.0,dWinRate2=0.0;
			if (lAllCount1!=0) dWinRate1=(double)(pUserData1->lWinCount)/(double)(lAllCount1);
			if (lAllCount2!=0) dWinRate2=(double)(pUserData2->lWinCount)/(double)(lAllCount2);
			int iCompRes=dWinRate1>dWinRate2?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
			break;
		}
	case DTP_FLEE_RATE:
		{
			long lAllCount1=pUserItem1->GetUserPlayCount();
			long lAllCount2=pUserItem2->GetUserPlayCount();
			double dFleeRate1=0.0,dFleeRate2=0.0;
			if (lAllCount1!=0) dFleeRate1=(double)(pUserData1->lFleeCount)/(double)(lAllCount1);
			if (lAllCount2!=0) dFleeRate2=(double)(pUserData2->lFleeCount)/(double)(lAllCount2);
			int iCompRes=dFleeRate1>dFleeRate2?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_PLAY_COUNT:
		{
			long lAllCount1=pUserItem1->GetUserPlayCount();
			long lAllCount2=pUserItem2->GetUserPlayCount();
			int iCompRes=lAllCount1>lAllCount2?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	}

	return 0;
}
