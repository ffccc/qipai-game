#include "Stdafx.h"
#include "Resource.h"
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
	m_wKindID=0;
	m_wGameGenre=0;
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
		AfxSetResourceHandle(GetModuleHandle(GAME_FRAME_DLL_NAME));
		Image.LoadBitmap(IDB_USER_STATUS);
		AfxSetResourceHandle(GetModuleHandle(NULL));
		Image.GetBitmap(&ImageInfo);
		m_StatusImage.Create(IMGAE_SPACE,ImageInfo.bmHeight,ILC_COLOR16|ILC_MASK,0,0);
		m_StatusImage.Add(&Image,RGB(255,0,255));
	}
	SetImageList(&m_StatusImage,LVSIL_SMALL);

	//������ɫ
	SetBkColor(RGB(10,130,180));
	SetTextColor(RGB(232,249,253));
	SetTextBkColor(RGB(10,130,180));

	//�����д���
	WORD wGameType=GAME_GENRE_SCORE;
	LPCTSTR lpszCmdLine=AfxGetApp()->m_lpCmdLine;
	if (lpszCmdLine[0]!=0)
	{
		//��� TOKEN
		int nStringLength=0;
		CString strRoomToken;
		LPCTSTR pszRoomToken=TEXT("/ServerType:");
		LPCTSTR lpszBeginString=lpszCmdLine;
		while (true)
		{
			LPCTSTR lpszEndString=_tcschr(lpszBeginString,TEXT(' '));
			nStringLength=(lpszEndString==NULL)?lstrlen(lpszBeginString):(int)(lpszEndString-lpszBeginString);

			//�жϱ�ʶ
			const int nTokenLength=lstrlen(pszRoomToken);
			if ((nStringLength>=nTokenLength)&&(memcmp(lpszBeginString,pszRoomToken,nTokenLength*sizeof(TCHAR))==0))
			{
				CopyMemory(strRoomToken.GetBufferSetLength(nStringLength-nTokenLength),lpszBeginString+nTokenLength,
					(nStringLength-nTokenLength)*sizeof(TCHAR));
				strRoomToken.ReleaseBuffer();
				wGameType=(WORD)atol(strRoomToken);
				break;
			}

			//���ñ���
			if (lpszEndString==NULL) break;
			lpszBeginString=(lpszEndString+1);
		}
	}

	//�����б�
	if (wGameType==GAME_GENRE_GOLD)
	{
		m_wColumnCount=15;
		InsertColumn(0,TEXT("�û���"),LVCFMT_LEFT,115);
		InsertColumn(1,TEXT("��Ϸ ID"),LVCFMT_LEFT,60);
		InsertColumn(2,TEXT("��Ϸ��"),LVCFMT_LEFT,60);
		InsertColumn(3,TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(4,TEXT("�û��ȼ�"),LVCFMT_LEFT,85);
		InsertColumn(5,TEXT("����ֵ"),LVCFMT_LEFT,50);
		InsertColumn(6,TEXT("�ܾ�"),LVCFMT_LEFT,60);
		InsertColumn(7,TEXT("ʤ��"),LVCFMT_LEFT,60);
		InsertColumn(8,TEXT("���"),LVCFMT_LEFT,60);
		InsertColumn(9,TEXT("�;�"),LVCFMT_LEFT,60);
		InsertColumn(10,TEXT("�Ӿ�"),LVCFMT_LEFT,60);
		InsertColumn(11,TEXT("ʤ��"),LVCFMT_LEFT,60);
		InsertColumn(12,TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(13,TEXT("����ǩ��"),LVCFMT_LEFT,200);
		InsertColumn(14,TEXT("�Թ��û�"),LVCFMT_LEFT,90);

		m_wDataDescribe[0]=DTP_USER_ACCOUNTS;
		m_wDataDescribe[1]=DTP_GAME_ID;
		m_wDataDescribe[2]=DTP_USER_SCORE;
		m_wDataDescribe[3]=DTP_LOVE_LINESS;
		m_wDataDescribe[4]=DTP_GAME_LEVEL;
		m_wDataDescribe[5]=DTP_USER_EXPERIENCE;
		m_wDataDescribe[6]=DTP_USER_PLAY_COUNT;
		m_wDataDescribe[7]=DTP_USER_WIN;
		m_wDataDescribe[8]=DTP_USER_LOST;
		m_wDataDescribe[9]=DTP_USER_DRAW;
		m_wDataDescribe[10]=DTP_USER_FLEE;
		m_wDataDescribe[11]=DTP_WIN_RATE;
		m_wDataDescribe[12]=DTP_FLEE_RATE;
		m_wDataDescribe[13]=DTP_UNDER_WRITE;
		m_wDataDescribe[14]=DTP_LOOK_ON_USER;

	}
	else if (wGameType==GAME_GENRE_EDUCATE)
	{
		m_wColumnCount=15;										//	m_wColumnCount=16;
		InsertColumn(0,TEXT("�û���"),LVCFMT_LEFT,115);			//	InsertColumn(0,TEXT("�û���"),LVCFMT_LEFT,115);	
		InsertColumn(1,TEXT("��Ϸ ID"),LVCFMT_LEFT,60);			//	InsertColumn(1,TEXT("��Ϸ ID"),LVCFMT_LEFT,60);
		InsertColumn(2,TEXT("��ϰ��"),LVCFMT_LEFT,60);			//	InsertColumn(2,TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(3,TEXT("����"),LVCFMT_LEFT,60);			//	InsertColumn(3, TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(4,TEXT("�û��ȼ�"),LVCFMT_LEFT,85);		//	InsertColumn(4, TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(5,TEXT("����ֵ"),LVCFMT_LEFT,50);			//	InsertColumn(5, TEXT("�û��ȼ�"),LVCFMT_LEFT,85);
		InsertColumn(6,TEXT("�ܾ�"),LVCFMT_LEFT,60);			//	InsertColumn(6, TEXT("����ֵ"),LVCFMT_LEFT,50);
		InsertColumn(7,TEXT("ʤ��"),LVCFMT_LEFT,60);			//	InsertColumn(7, TEXT("�ܾ�"),LVCFMT_LEFT,60);
		InsertColumn(8,TEXT("���"),LVCFMT_LEFT,60);			//	InsertColumn(8, TEXT("ʤ��"),LVCFMT_LEFT,60);
		InsertColumn(9,TEXT("�;�"),LVCFMT_LEFT,60);			//	InsertColumn(9, TEXT("���"),LVCFMT_LEFT,60);
		InsertColumn(10,TEXT("�Ӿ�"),LVCFMT_LEFT,60);			//	InsertColumn(10,TEXT("�;�"),LVCFMT_LEFT,60);
		InsertColumn(11,TEXT("ʤ��"),LVCFMT_LEFT,60);			//	InsertColumn(11,TEXT("�Ӿ�"),LVCFMT_LEFT,60);
		InsertColumn(12,TEXT("����"),LVCFMT_LEFT,60);			//	InsertColumn(12,TEXT("ʤ��"),LVCFMT_LEFT,60);
		InsertColumn(13,TEXT("����ǩ��"),LVCFMT_LEFT,200);		//	InsertColumn(13,TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(14,TEXT("�Թ��û�"),LVCFMT_LEFT,90);		//	InsertColumn(14,TEXT("����ǩ��"),LVCFMT_LEFT,200);
																//	InsertColumn(15,TEXT("�Թ��û�"),LVCFMT_LEFT,90);
		m_wDataDescribe[0]=DTP_USER_ACCOUNTS;					//	
		m_wDataDescribe[1]=DTP_GAME_ID;							//	m_wDataDescribe[0]=DTP_USER_ACCOUNTS;
		m_wDataDescribe[2]=DTP_USER_SCORE;						//	m_wDataDescribe[1]=DTP_GAME_ID;
		m_wDataDescribe[3]=DTP_LOVE_LINESS;						//	m_wDataDescribe[2]=DTP_USER_SCORE;
		m_wDataDescribe[4]=DTP_GAME_LEVEL;						//	m_wDataDescribe[3]=DTP_USER_GOLD;
		m_wDataDescribe[5]=DTP_USER_EXPERIENCE;					//	m_wDataDescribe[4]=DTP_LOVE_LINESS;
		m_wDataDescribe[6]=DTP_USER_PLAY_COUNT;					//	m_wDataDescribe[5]=DTP_GAME_LEVEL;
		m_wDataDescribe[7]=DTP_USER_WIN;						//	m_wDataDescribe[6]=DTP_USER_EXPERIENCE;
		m_wDataDescribe[8]=DTP_USER_LOST;						//	m_wDataDescribe[7]=DTP_USER_PLAY_COUNT;
		m_wDataDescribe[9]=DTP_USER_DRAW;						//	m_wDataDescribe[8]=DTP_USER_WIN;
		m_wDataDescribe[10]=DTP_USER_FLEE;						//	m_wDataDescribe[9]=DTP_USER_LOST;
		m_wDataDescribe[11]=DTP_WIN_RATE;						//	m_wDataDescribe[10]=DTP_USER_DRAW;
		m_wDataDescribe[12]=DTP_FLEE_RATE;						//	m_wDataDescribe[11]=DTP_USER_FLEE;
		m_wDataDescribe[13]=DTP_UNDER_WRITE;					//	m_wDataDescribe[12]=DTP_WIN_RATE;
		m_wDataDescribe[14]=DTP_LOOK_ON_USER;					//	m_wDataDescribe[13]=DTP_FLEE_RATE;
	}
	else if (wGameType==GAME_GENRE_MATCH)
	{
		m_wColumnCount=15;										//	m_wColumnCount=16;
		InsertColumn(0,TEXT("�û���"),LVCFMT_LEFT,115);			//	InsertColumn(0,TEXT("�û���"),LVCFMT_LEFT,115);	
		InsertColumn(1,TEXT("��Ϸ ID"),LVCFMT_LEFT,60);			//	InsertColumn(1,TEXT("��Ϸ ID"),LVCFMT_LEFT,60);
		InsertColumn(2,TEXT("����"),LVCFMT_LEFT,60);			//	InsertColumn(2,TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(3,TEXT("����"),LVCFMT_LEFT,60);			//	InsertColumn(3, TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(4,TEXT("�û��ȼ�"),LVCFMT_LEFT,85);		//	InsertColumn(4, TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(5,TEXT("����ֵ"),LVCFMT_LEFT,50);			//	InsertColumn(5, TEXT("�û��ȼ�"),LVCFMT_LEFT,85);
		InsertColumn(6,TEXT("�ܾ�"),LVCFMT_LEFT,60);			//	InsertColumn(6, TEXT("����ֵ"),LVCFMT_LEFT,50);
		InsertColumn(7,TEXT("ʤ��"),LVCFMT_LEFT,60);			//	InsertColumn(7, TEXT("�ܾ�"),LVCFMT_LEFT,60);
		InsertColumn(8,TEXT("���"),LVCFMT_LEFT,60);			//	InsertColumn(8, TEXT("ʤ��"),LVCFMT_LEFT,60);
		InsertColumn(9,TEXT("�;�"),LVCFMT_LEFT,60);			//	InsertColumn(9, TEXT("���"),LVCFMT_LEFT,60);
		InsertColumn(10,TEXT("�Ӿ�"),LVCFMT_LEFT,60);			//	InsertColumn(10,TEXT("�;�"),LVCFMT_LEFT,60);
		InsertColumn(11,TEXT("ʤ��"),LVCFMT_LEFT,60);			//	InsertColumn(11,TEXT("�Ӿ�"),LVCFMT_LEFT,60);
		InsertColumn(12,TEXT("����"),LVCFMT_LEFT,60);			//	InsertColumn(12,TEXT("ʤ��"),LVCFMT_LEFT,60);
		InsertColumn(13,TEXT("����ǩ��"),LVCFMT_LEFT,200);		//	InsertColumn(13,TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(14,TEXT("�Թ��û�"),LVCFMT_LEFT,90);		//	InsertColumn(14,TEXT("����ǩ��"),LVCFMT_LEFT,200);
																//	InsertColumn(15,TEXT("�Թ��û�"),LVCFMT_LEFT,90);
		m_wDataDescribe[0]=DTP_USER_ACCOUNTS;					//	
		m_wDataDescribe[1]=DTP_GAME_ID;							//	m_wDataDescribe[0]=DTP_USER_ACCOUNTS;
		m_wDataDescribe[2]=DTP_USER_SCORE;						//	m_wDataDescribe[1]=DTP_GAME_ID;
		m_wDataDescribe[3]=DTP_LOVE_LINESS;						//	m_wDataDescribe[2]=DTP_USER_SCORE;
		m_wDataDescribe[4]=DTP_GAME_LEVEL;						//	m_wDataDescribe[3]=DTP_USER_GOLD;
		m_wDataDescribe[5]=DTP_USER_EXPERIENCE;					//	m_wDataDescribe[4]=DTP_LOVE_LINESS;
		m_wDataDescribe[6]=DTP_USER_PLAY_COUNT;					//	m_wDataDescribe[5]=DTP_GAME_LEVEL;
		m_wDataDescribe[7]=DTP_USER_WIN;						//	m_wDataDescribe[6]=DTP_USER_EXPERIENCE;
		m_wDataDescribe[8]=DTP_USER_LOST;						//	m_wDataDescribe[7]=DTP_USER_PLAY_COUNT;
		m_wDataDescribe[9]=DTP_USER_DRAW;						//	m_wDataDescribe[8]=DTP_USER_WIN;
		m_wDataDescribe[10]=DTP_USER_FLEE;						//	m_wDataDescribe[9]=DTP_USER_LOST;
		m_wDataDescribe[11]=DTP_WIN_RATE;						//	m_wDataDescribe[10]=DTP_USER_DRAW;
		m_wDataDescribe[12]=DTP_FLEE_RATE;						//	m_wDataDescribe[11]=DTP_USER_FLEE;
		m_wDataDescribe[13]=DTP_UNDER_WRITE;					//	m_wDataDescribe[12]=DTP_WIN_RATE;
		m_wDataDescribe[14]=DTP_LOOK_ON_USER;					//	m_wDataDescribe[13]=DTP_FLEE_RATE;
	}
	else
	{
		m_wColumnCount=15;										//	m_wColumnCount=16;
		InsertColumn(0,TEXT("�û���"),LVCFMT_LEFT,115);			//	InsertColumn(0,TEXT("�û���"),LVCFMT_LEFT,115);	
		InsertColumn(1,TEXT("��Ϸ ID"),LVCFMT_LEFT,60);			//	InsertColumn(1,TEXT("��Ϸ ID"),LVCFMT_LEFT,60);
		InsertColumn(2,TEXT("����"),LVCFMT_LEFT,60);			//	InsertColumn(2,TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(3,TEXT("����"),LVCFMT_LEFT,60);			//	InsertColumn(3, TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(4,TEXT("�û��ȼ�"),LVCFMT_LEFT,85);		//	InsertColumn(4, TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(5,TEXT("����ֵ"),LVCFMT_LEFT,50);			//	InsertColumn(5, TEXT("�û��ȼ�"),LVCFMT_LEFT,85);
		InsertColumn(6,TEXT("�ܾ�"),LVCFMT_LEFT,60);			//	InsertColumn(6, TEXT("����ֵ"),LVCFMT_LEFT,50);
		InsertColumn(7,TEXT("ʤ��"),LVCFMT_LEFT,60);			//	InsertColumn(7, TEXT("�ܾ�"),LVCFMT_LEFT,60);
		InsertColumn(8,TEXT("���"),LVCFMT_LEFT,60);			//	InsertColumn(8, TEXT("ʤ��"),LVCFMT_LEFT,60);
		InsertColumn(9,TEXT("�;�"),LVCFMT_LEFT,60);			//	InsertColumn(9, TEXT("���"),LVCFMT_LEFT,60);
		InsertColumn(10,TEXT("�Ӿ�"),LVCFMT_LEFT,60);			//	InsertColumn(10,TEXT("�;�"),LVCFMT_LEFT,60);
		InsertColumn(11,TEXT("ʤ��"),LVCFMT_LEFT,60);			//	InsertColumn(11,TEXT("�Ӿ�"),LVCFMT_LEFT,60);
		InsertColumn(12,TEXT("����"),LVCFMT_LEFT,60);			//	InsertColumn(12,TEXT("ʤ��"),LVCFMT_LEFT,60);
		InsertColumn(13,TEXT("����ǩ��"),LVCFMT_LEFT,200);		//	InsertColumn(13,TEXT("����"),LVCFMT_LEFT,60);
		InsertColumn(14,TEXT("�Թ��û�"),LVCFMT_LEFT,90);		//	InsertColumn(14,TEXT("����ǩ��"),LVCFMT_LEFT,200);
																//	InsertColumn(15,TEXT("�Թ��û�"),LVCFMT_LEFT,90);
		m_wDataDescribe[0]=DTP_USER_ACCOUNTS;					//	
		m_wDataDescribe[1]=DTP_GAME_ID;							//	m_wDataDescribe[0]=DTP_USER_ACCOUNTS;
		m_wDataDescribe[2]=DTP_USER_SCORE;						//	m_wDataDescribe[1]=DTP_GAME_ID;
		m_wDataDescribe[3]=DTP_LOVE_LINESS;						//	m_wDataDescribe[2]=DTP_USER_SCORE;
		m_wDataDescribe[4]=DTP_GAME_LEVEL;						//	m_wDataDescribe[3]=DTP_USER_GOLD;
		m_wDataDescribe[5]=DTP_USER_EXPERIENCE;					//	m_wDataDescribe[4]=DTP_LOVE_LINESS;
		m_wDataDescribe[6]=DTP_USER_PLAY_COUNT;					//	m_wDataDescribe[5]=DTP_GAME_LEVEL;
		m_wDataDescribe[7]=DTP_USER_WIN;						//	m_wDataDescribe[6]=DTP_USER_EXPERIENCE;
		m_wDataDescribe[8]=DTP_USER_LOST;						//	m_wDataDescribe[7]=DTP_USER_PLAY_COUNT;
		m_wDataDescribe[9]=DTP_USER_DRAW;						//	m_wDataDescribe[8]=DTP_USER_WIN;
		m_wDataDescribe[10]=DTP_USER_FLEE;						//	m_wDataDescribe[9]=DTP_USER_LOST;
		m_wDataDescribe[11]=DTP_WIN_RATE;						//	m_wDataDescribe[10]=DTP_USER_DRAW;
		m_wDataDescribe[12]=DTP_FLEE_RATE;						//	m_wDataDescribe[11]=DTP_USER_FLEE;
		m_wDataDescribe[13]=DTP_UNDER_WRITE;					//	m_wDataDescribe[12]=DTP_WIN_RATE;
		m_wDataDescribe[14]=DTP_LOOK_ON_USER;					//	m_wDataDescribe[13]=DTP_FLEE_RATE;
	}															//	m_wDataDescribe[14]=DTP_UNDER_WRITE;
																//	m_wDataDescribe[15]=DTP_LOOK_ON_USER;
	//���صȼ�
	m_GameRankManagerHelper.CreateInstance();
	if (m_GameRankManagerHelper.GetInterface()!=NULL)
	{
		m_GameRankManagerHelper->LoadGameLevel(AfxGetApp()->m_pszExeName);
	}

	return true;
}

//�����û�
bool CUserListView::InsertUserItem(tagUserData * pUserData)
{
	//Ч��״̬
	ASSERT(pUserData!=NULL);
	if (GetSafeHwnd()==NULL) return false;

	//�����û�
	LVFINDINFO FindInfo;
	memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pUserData;
	int iItem=FindItem(&FindInfo);

	//�û���
	if (iItem==-1)
	{
		int nImageStation=GetImageStation(pUserData);
		int nInsertStation=GetInsertStation(pUserData);
		iItem=InsertItem(nInsertStation,pUserData->szName,nImageStation);
		SetItemData(iItem,(DWORD_PTR)pUserData);
	}

	//��д����
	FillUserListItem(iItem,pUserData);

	return true;
}

//�����û�
bool CUserListView::UpdateUserItem(tagUserData * pUserData)
{
	//Ч��״̬
	ASSERT(pUserData!=NULL);
	if (GetSafeHwnd()==NULL) return false;
	
	//�����û�
	LVFINDINFO FindInfo;
	memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pUserData;
	int iItem=FindItem(&FindInfo);
	if (iItem!=-1)
	{
		FillUserListItem(iItem,pUserData);
		return true;
	}
	
	return false;
}

//ɾ���û�
bool CUserListView::DeleteUserItem(tagUserData * pUserData)
{
	//Ч��״̬
	ASSERT(pUserData!=NULL);
	if (GetSafeHwnd()==NULL) return false;
	
	//�����û�
	LVFINDINFO FindInfo;
	memset(&FindInfo,0,sizeof(FindInfo));
	FindInfo.flags=LVFI_PARAM;
	FindInfo.lParam=(LPARAM)pUserData;
	int iItem=FindItem(&FindInfo);
	if (iItem!=-1)
	{
		DeleteItem(iItem);
		return true;
	}

	return false;
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
	tagUserData *pIUserItem = (tagUserData *)GetItemData(iItem);

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
			else if ( 0 < pIUserItem->cbMemberOrder && (DTP_USER_ACCOUNTS == m_wDataDescribe[i])) pDC->SetTextColor(RGB(255,0,0));
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
			else if ( 0 < pIUserItem->cbMemberOrder && DTP_GAME_ID == m_wDataDescribe[i]) pDC->SetTextColor(RGB(255,0,0));
			else pDC->SetTextColor(RGB(0,0,0));

			GetItemText(iItem,i,szBuffer,sizeof(szBuffer));

			CRect rcTempItem =rcSubItem;
			rcTempItem.left +=2; 
			rcTempItem.right +=2; 
			rcTempItem.top +=3; 
			pDC->DrawText(szBuffer,lstrlen(szBuffer),&rcTempItem,DT_LEFT|DT_END_ELLIPSIS);
		}
	}

	//�滭ѡ��
	if (lpDrawItemStruct->itemState&ODS_FOCUS) pDC->DrawFocusRect(&rcItem);

	return;
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
int CUserListView::GetInsertStation(tagUserData * pUserData)
{
	return GetItemCount();
}

//��д����
void CUserListView::FillUserListItem(int iItem, tagUserData * pUserData)
{
	//�����û�
	if (m_wColumnCount==0)return;

	//�û�ͷ��
	int nImageStation=GetImageStation(pUserData);
	SetItem(iItem,0,LVIF_IMAGE|LVIF_TEXT,GetDisplayText(pUserData,0),nImageStation,0,0,0);

	for (WORD i=1;i<m_wColumnCount-1;i++) 
	{
		SetItemText(iItem,i,GetDisplayText(pUserData,i));
	}

	//��ȡ����
	int nItemCount = GetItemCount();

	//�Թ���Ϣ
	if(nItemCount>1)
	{
		int j=0;
		for ( ; j<nItemCount;j++)
		{
			//��ȡ���
			tagUserData *pLookUserData = (tagUserData *)GetItemData(j);
			if(pLookUserData->cbUserStatus!=US_LOOKON)continue;

			int i=0;
			for ( ; i<nItemCount;i++)
			{
				if(i==j)continue;
				tagUserData *pTargetUserData = (tagUserData *)GetItemData(i);
				if ( pTargetUserData == NULL ) continue;
				if ( pTargetUserData->dwUserID == pLookUserData->dwUserID ) continue;
				if ( pTargetUserData->wChairID != pLookUserData->wChairID ) continue;
				if (pTargetUserData->cbUserStatus==US_LOOKON) continue;
				SetItemText(j,m_wColumnCount-1,pTargetUserData->szName);
				break;
			}
			if(i==nItemCount)SetItemText(j,m_wColumnCount-1,TEXT("���û����뿪"));
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////

//�������Ϣ
void CUserListView::OnColumnclick(NMHDR * pNMHDR, LRESULT * pResult) 
{
	NM_LISTVIEW * pNMListView=(NM_LISTVIEW *)pNMHDR;

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

	return;
}

//���к���
int CALLBACK CUserListView::SortFun(LPARAM lParam1, LPARAM lParam2, LPARAM lParamList)
{
	//�������
	tagUserData * pUserData1=(tagUserData *)lParam1;
	tagUserData * pUserData2=(tagUserData *)lParam2;
    tagListSortInfo * pSortInfo=(tagListSortInfo *)lParamList;
	CUserListView * pListCtrl=pSortInfo->pRoomListCtrl;
	ASSERT(pUserData1!=NULL);
	ASSERT(pUserData2!=NULL);
	ASSERT(pSortInfo!=NULL);

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
	case DTP_UNDER_WRITE:
		{
			int iCompRes=strcmp(pUserData1->szUnderWrite,pUserData2->szUnderWrite);
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
			long lAllCount1=pUserData1->lWinCount+pUserData1->lLostCount+pUserData1->lDrawCount+pUserData1->lFleeCount;
			long lAllCount2=pUserData2->lWinCount+pUserData2->lLostCount+pUserData2->lDrawCount+pUserData2->lFleeCount;
			double dFleeRate1=0.0,dFleeRate2=0.0;
			if (lAllCount1!=0) dFleeRate1=(double)(pUserData1->lFleeCount)/(double)(lAllCount1);
			if (lAllCount2!=0) dFleeRate2=(double)(pUserData2->lFleeCount)/(double)(lAllCount2);
			int iCompRes=dFleeRate1>dFleeRate2?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	case DTP_USER_PLAY_COUNT:
		{
			long lAllCount1=pUserData1->lWinCount+pUserData1->lLostCount+pUserData1->lDrawCount+pUserData1->lFleeCount;
			long lAllCount2=pUserData2->lWinCount+pUserData2->lLostCount+pUserData2->lDrawCount+pUserData2->lFleeCount;
			int iCompRes=lAllCount1>lAllCount2?1:-1;
			return (pSortInfo->cbAscendSort)?iCompRes:-iCompRes;
		}
	}

	return 0;
}

//��ȡ�ַ�
LPCTSTR CUserListView::GetDisplayText(tagUserData * pUserData, WORD wColumnPos)
{
	//��������
	static TCHAR szBuffer[512];

	//������ʾ
	switch (m_wDataDescribe[wColumnPos])
	{
	case DTP_USER_ID:			//�û� ID
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->dwUserID);
			return szBuffer;
		}
	case DTP_GAME_ID:			//��Ϸ ID
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->dwGameID);
			return szBuffer;
		}
	case DTP_INSURE_SCORE:		//�洢���
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->lInsureScore);
			return szBuffer;
		}
	case DTP_GAME_GOLD:			//��Ϸ���
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->lGameGold);
			return szBuffer;
		}
	case DTP_LOVE_LINESS:		//�û�����
		{
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),pUserData->lLoveliness);
			return szBuffer;
		}
	case DTP_USER_ACCOUNTS:		//�û�����
		{
			return pUserData->szName;
		}
	case DTP_UNDER_WRITE:		//����ǩ��
		{
			return pUserData->szUnderWrite;
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
			LONG lAllCount=pUserData->lWinCount+pUserData->lLostCount+pUserData->lDrawCount+pUserData->lFleeCount;
			_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),lAllCount);
			return szBuffer;
		}
	case DTP_WIN_RATE:			//�û�ʤ��
		{
			if (pUserData->lWinCount>0)
			{
				LONG lAllCount=pUserData->lWinCount+pUserData->lLostCount+pUserData->lDrawCount+pUserData->lFleeCount;
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
				LONG lAllCount=pUserData->lWinCount+pUserData->lLostCount+pUserData->lDrawCount+pUserData->lFleeCount;
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
				LONG lAllCount=pUserData->lWinCount+pUserData->lLostCount+pUserData->lDrawCount+pUserData->lFleeCount;
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
				LONG lAllCount=pUserData->lWinCount+pUserData->lLostCount+pUserData->lDrawCount+pUserData->lFleeCount;
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
			if (m_GameRankManagerHelper.GetInterface()!=NULL)
			{
				LPCTSTR pszGameRank=m_GameRankManagerHelper->GetLevelDescribe(pUserData->lScore);
				lstrcpyn(szBuffer,pszGameRank,sizeof(szBuffer));
			}
			return szBuffer;
		}
	case DTP_USER_GROUP_NAME:	//��������
		{
			return pUserData->szGroupName;
		}
	case DTP_LOOK_ON_USER:		//�Թ��û�
		{
			//if ( pUserData->cbUserStatus == US_LOOKON )
			//{
			//	//��ȡ����
			//	int nItemCount = GetItemCount();

			//	//��ȡ���
			//	for ( int nIndex = 0; nIndex < nItemCount; ++nIndex )
			//	{
			//		tagUserData *pTargetUserData = (tagUserData *)GetItemData(nIndex);
			//		if ( pTargetUserData == NULL ) continue;
			//		if ( pTargetUserData->dwUserID == pUserData->dwUserID ) continue;
			//		if ( pTargetUserData->wChairID == pUserData->wChairID ) return pTargetUserData->szName;
			//	}
			//}

			return NULL;
		}
	}

	//Ĭ����ʾ
	szBuffer[0]=0;

	return szBuffer;
}

//////////////////////////////////////////////////////////////////////////
