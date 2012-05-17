#include "StdAfx.h"
#include "Resource.h"
#include "UserItemElement.h"
#include "UserListControl.h"

//////////////////////////////////////////////////////////////////////////////////

//排序位置
#define SORT_AFTER					1									//后面位置
#define SORT_FRONT					-1									//前面位置

//列表属性
#define LOCK_COUNT					3									//锁定列数
#define ITEM_HEIGHT					130									//列表高度

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CImageList			CUserListControl::m_ImageUserStatus;				//状态位图

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CUserListControl, CSkinListCtrl)
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CUserListControl::CUserListControl()
{
	//资源变量
	m_dwMySelfUserID=0L;
	m_bAscendSort=false;
	m_bCreateColumn=false;

	//接口变量
	m_pIGameLevelParser=NULL;
	m_pIGameUserManager=NULL;

	//默认定义
	m_ColorUserList.crSelectTX=COLOR_SELECT_TX;
	m_ColorUserList.crSelectBK=COLOR_SELECT_BK;
	m_ColorUserList.crNormalTX=COLOR_NORMAL_TX;
	m_ColorUserList.crNormalBK=COLOR_NORMAL_BK;

	//玩家定义
	m_ColorUserList.crMyselfTX=COLOR_MYSELF_TX;
	m_ColorUserList.crMyselfBK=COLOR_MYSELF_BK;
	m_ColorUserList.crMasterTX=COLOR_MASTER_TX;
	m_ColorUserList.crMasterBK=COLOR_MASTER_BK;
	m_ColorUserList.crMemberTX=COLOR_MEMBER_TX;
	m_ColorUserList.crMemberBK=COLOR_MEMBER_BK;

	//列表信息
	m_wColumnCount=0;
	ZeroMemory(m_cbDataDescribe,sizeof(m_cbDataDescribe));

	//设置控件
	m_SkinHeaderCtrl.SetLockCount(LOCK_COUNT);
	m_SkinHeaderCtrl.SetItemHeight(ITEM_HEIGHT);

	return;
}

//析构函数
CUserListControl::~CUserListControl()
{
}

//控件绑定
VOID CUserListControl::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	//加载状态
	if (m_ImageUserStatus.m_hImageList==NULL)
	{
		//加载图片
		CBitmap Image;
		BITMAP ImageInfo;
		Image.LoadBitmap(IDB_USER_STATUS_IMAGE);

		//创建图片
		Image.GetBitmap(&ImageInfo);
		m_ImageUserStatus.Create(USER_STATUS_CX,ImageInfo.bmHeight,ILC_COLOR16|ILC_MASK,0,0);

		//插入图片
		m_ImageUserStatus.Add(&Image,RGB(255,0,255));
	}

	//设置控件
	SetBkColor(m_ColorUserList.crNormalBK);
	SetImageList(&m_ImageUserStatus,LVSIL_SMALL);

	//占位标志
	m_cbDataDescribe[m_wColumnCount]=UD_NULL;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,0,0);

	//用户标志
	m_cbDataDescribe[m_wColumnCount]=UD_IMAGE_FLAG;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,USER_STATUS_CX,0);

	//用户状态
	m_cbDataDescribe[m_wColumnCount]=UD_IMAGE_STATUS;
	InsertColumn(m_wColumnCount++,TEXT(""),LVCFMT_CENTER,USER_STATUS_CX,0);

	return;
}

//获取位置
INT CUserListControl::GetInsertIndex(VOID * pItemData)
{
	//变量定义
	INT nItemCount=GetItemCount();
	IClientUserItem * pIClientUserItem=GetClientUserItem(pItemData);

	//自己判断
	if (pIClientUserItem->GetUserID()==m_dwMySelfUserID)
	{
		return 0L;
	}

	//普通用户
	BYTE cbCompanion=pIClientUserItem->GetUserCompanion();
	if ((pIClientUserItem->GetMasterOrder()==0)&&(pIClientUserItem->GetMemberOrder()==0)&&(cbCompanion!=CP_FRIEND))
	{
		return nItemCount;
	}

	//变量定义
	IClientUserItem * pIUserItemTemp=NULL;
	LONG nImportOrder=pIClientUserItem->GetImportOrder();

	//获取位置
	for (INT i=0;i<nItemCount;i++)
	{
		//获取用户
		DWORD_PTR dwItemData=GetItemData(i);
		IClientUserItem * pIUserItemTemp=GetClientUserItem((VOID *)dwItemData);

		//自己判断
		if (pIUserItemTemp->GetUserID()==m_dwMySelfUserID) continue;

		//对比等级
		if (nImportOrder>pIUserItemTemp->GetImportOrder()) return i;
	}

	return nItemCount;
}

//排列数据
INT CUserListControl::SortItemData(LPARAM lParam1, LPARAM lParam2, WORD wColumnIndex, bool bAscendSort)
{
	//定义变量
	IClientUserItem * pIClientUserItem1=GetClientUserItem((VOID *)lParam1);
	IClientUserItem * pIClientUserItem2=GetClientUserItem((VOID *)lParam2);

	//自己置顶
	if (pIClientUserItem1->GetUserID()==m_dwMySelfUserID) return SORT_FRONT;
	if (pIClientUserItem2->GetUserID()==m_dwMySelfUserID) return SORT_AFTER;

	//对比数据
	switch (m_cbDataDescribe[wColumnIndex])
	{
	case UD_GAME_ID:		//游戏标识
		{
			//获取变量
			DWORD dwGameID1=pIClientUserItem1->GetGameID();
			DWORD dwGameID2=pIClientUserItem2->GetGameID();

			//对比数据
			INT nResult=dwGameID1>dwGameID2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_USER_ID:		//用户标识
		{
			//获取变量
			DWORD dwUserID1=pIClientUserItem1->GetUserID();
			DWORD dwUserID2=pIClientUserItem2->GetUserID();

			//对比数据
			INT nResult=dwUserID1>dwUserID2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_NICKNAME:		//用户昵称
		{
			//获取变量
			BYTE cbCompanion1=pIClientUserItem1->GetUserCompanion();
			BYTE cbCompanion2=pIClientUserItem2->GetUserCompanion();

			//好友置顶
			if (((cbCompanion1==CP_FRIEND)&&(cbCompanion2!=CP_FRIEND))
				||((cbCompanion1!=CP_FRIEND)&&(cbCompanion2==CP_FRIEND)))
			{
				return (cbCompanion1==CP_FRIEND)?SORT_FRONT:SORT_AFTER;
			}

			//获取变量
			BYTE cbMasterRight1=pIClientUserItem1->GetMasterOrder();
			BYTE cbMasterRight2=pIClientUserItem2->GetMasterOrder();

			//管理置顶
			if (cbMasterRight1!=cbMasterRight2)
			{
				return (cbMasterRight1==cbMasterRight2)?SORT_FRONT:SORT_AFTER;
			}
			
			//获取变量
			BYTE cbMemberRight1=pIClientUserItem1->GetMemberOrder();
			BYTE cbMemberRight2=pIClientUserItem2->GetMemberOrder();

			//会员置顶
			if (cbMemberRight1!=cbMemberRight2)
			{
				return (cbMemberRight1==cbMemberRight2)?SORT_FRONT:SORT_AFTER;
			}

			//获取变量
			LPCTSTR pszNickName1=pIClientUserItem1->GetNickName();
			LPCTSTR pszNickName2=pIClientUserItem2->GetNickName();
			
			//对比数据
			INT nResult=lstrcmp(pszNickName1,pszNickName2);
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_GENDER:			//用户性别
		{
			//获取变量
			BYTE cbGender1=pIClientUserItem1->GetGender();
			BYTE cbGender2=pIClientUserItem2->GetGender();

			//对比数据
			INT nResult=cbGender1>cbGender2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_GROUP_NAME:		//社团名字
		{
			//获取变量
			LPCTSTR pszGroupName1=pIClientUserItem1->GetGroupName();
			LPCTSTR pszGroupName2=pIClientUserItem2->GetGroupName();

			//对比数据
			INT nResult=lstrcmp(pszGroupName1,pszGroupName2);
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_UNDER_WRITE:	//个性签名
		{
			//获取变量
			LPCTSTR pszUnderWrite1=pIClientUserItem1->GetUnderWrite();
			LPCTSTR pszUnderWrite2=pIClientUserItem2->GetUnderWrite();

			//对比数据
			INT nResult=lstrcmp(pszUnderWrite1,pszUnderWrite2);
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_TABLE:			//桌子号码
		{
			//获取变量
			WORD wTableID1=pIClientUserItem1->GetTableID();
			WORD wTableID2=pIClientUserItem2->GetTableID();

			//对比数据
			INT nResult=wTableID1>wTableID2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_CHAIR:			//椅子号码
		{
			//获取变量
			WORD wChairID1=pIClientUserItem1->GetChairID();
			WORD wChairID2=pIClientUserItem2->GetChairID();

			//对比数据
			INT nResult=wChairID1>wChairID2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_SCORE:			//用户积分
		{
			//获取变量
			SCORE lScore1=pIClientUserItem1->GetUserScore();
			SCORE lScore2=pIClientUserItem2->GetUserScore();

			//对比数据
			INT nResult=lScore1>lScore2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_GRADE:			//用户成绩
		{
			//获取变量
			SCORE lBanker1=pIClientUserItem1->GetUserGrade();
			SCORE lBanker2=pIClientUserItem2->GetUserGrade();

			//对比数据
			INT nResult=lBanker1>lBanker2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_USER_MEDAL:		//用户奖牌
		{
			//获取变量
			DWORD dwUserMedal1=pIClientUserItem1->GetUserMedal();
			DWORD dwUserMedal2=pIClientUserItem2->GetUserMedal();

			//对比数据
			INT nResult=dwUserMedal1>dwUserMedal2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_EXPERIENCE:		//用户经验
		{
			//获取变量
			DWORD dwExperience1=pIClientUserItem1->GetUserExperience();
			DWORD dwExperience2=pIClientUserItem2->GetUserExperience();

			//对比数据
			INT nResult=dwExperience1>dwExperience2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_LOVELINESS:		//用户魅力
		{
			//获取变量
			DWORD dwLoveLiness1=pIClientUserItem1->GetUserLoveLiness();
			DWORD dwLoveLiness2=pIClientUserItem2->GetUserLoveLiness();

			//对比数据
			INT nResult=dwLoveLiness1>dwLoveLiness2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_WIN_COUNT:		//胜局盘数
		{
			//获取变量
			LONG lWinCount1=pIClientUserItem1->GetUserWinCount();
			LONG lWinCount2=pIClientUserItem2->GetUserWinCount();

			//对比数据
			INT nResult=lWinCount1>lWinCount2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_LOST_COUNT:		//输局盘数
		{
			//获取变量
			LONG lLostCount1=pIClientUserItem1->GetUserLostCount();
			LONG lLostCount2=pIClientUserItem2->GetUserLostCount();

			//对比数据
			INT nResult=lLostCount1>lLostCount2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_DRAW_COUNT:		//和局盘数
		{
			//获取变量
			LONG lDrawCount1=pIClientUserItem1->GetUserDrawCount();
			LONG lDrawCount2=pIClientUserItem2->GetUserDrawCount();

			//对比数据
			INT nResult=lDrawCount1>lDrawCount2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_FLEE_COUNT:		//逃局盘数
		{
			//获取变量
			LONG lFleeCount1=pIClientUserItem1->GetUserFleeCount();
			LONG lFleeCount2=pIClientUserItem2->GetUserFleeCount();

			//对比数据
			INT nResult=lFleeCount1>lFleeCount2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_PLAY_COUNT:		//游戏局数
		{
			//获取变量
			LONG lPlayCount1=pIClientUserItem1->GetUserPlayCount();
			LONG lPlayCount2=pIClientUserItem2->GetUserPlayCount();

			//对比数据
			INT nResult=lPlayCount1>lPlayCount2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_WIN_RATE:		//用户胜率
		{
			//获取变量
			FLOAT fWinRate1=pIClientUserItem1->GetUserWinRate();
			FLOAT fWinRate2=pIClientUserItem2->GetUserWinRate();

			//对比数据
			INT nResult=fWinRate1>fWinRate2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_LOST_RATE:		//用户输率
		{
			//获取变量
			FLOAT fLostRate1=pIClientUserItem1->GetUserLostRate();
			FLOAT fLostRate2=pIClientUserItem2->GetUserLostRate();

			//对比数据
			INT nResult=fLostRate1>fLostRate2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_DRAW_RATE:		//用户和率
		{
			//获取变量
			FLOAT fDrawRate1=pIClientUserItem1->GetUserDrawRate();
			FLOAT fDrawRate2=pIClientUserItem2->GetUserDrawRate();

			//对比数据
			INT nResult=fDrawRate1>fDrawRate2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_FLEE_RATE:		//用户逃率
		{
			//获取变量
			FLOAT fFleeRate1=pIClientUserItem1->GetUserFleeRate();
			FLOAT fFleeRate2=pIClientUserItem2->GetUserFleeRate();

			//对比数据
			INT nResult=fFleeRate1>fFleeRate2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_GAME_LEVEL:		//游戏等级
		{
			//获取变量
			SCORE lScore1=pIClientUserItem1->GetUserScore();
			SCORE lScore2=pIClientUserItem2->GetUserScore();

			//对比数据
			INT nResult=lScore1>lScore2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_NOTE_INFO:		//用户备注
		{
			//获取变量
			LPCTSTR pszNoteInfo1=pIClientUserItem1->GetUserNoteInfo();
			LPCTSTR pszNoteInfo2=pIClientUserItem2->GetUserNoteInfo();

			//对比数据
			INT nResult=lstrcmp(pszNoteInfo1,pszNoteInfo2);
			return (bAscendSort==true)?nResult:-nResult;
		}
	case UD_IMAGE_STATUS:	//用户状态
		{
			//获取变量
			BYTE cbUserStatus1=pIClientUserItem1->GetUserStatus();
			BYTE cbUserStatus2=pIClientUserItem2->GetUserStatus();

			//对比数据
			INT nResult=cbUserStatus1>cbUserStatus2?SORT_AFTER:SORT_FRONT;
			return (bAscendSort==true)?nResult:-nResult;
		}
	}

	return 0L;
}

//描述字符
VOID CUserListControl::ConstructString(VOID * pItemData, WORD wColumnIndex, LPTSTR pszString, WORD wMaxCount)
{
	//效验参数
	ASSERT(wColumnIndex<m_wColumnCount);
	if (wColumnIndex>=m_wColumnCount) return;

	//变量定义
	ASSERT(GetClientUserItem(pItemData)!=NULL);
	IClientUserItem * pIClientUserItem=GetClientUserItem(pItemData);

	//构造字符
	switch (m_cbDataDescribe[wColumnIndex])
	{
	case UD_GAME_ID:		//游戏标识
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetGameID());
			break;
		}
	case UD_USER_ID:		//用户标识
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserID());
			break;
		}
	case UD_NICKNAME:		//用户昵称
		{
			_sntprintf(pszString,wMaxCount,TEXT("%s"),pIClientUserItem->GetNickName());
			break;
		}
	case UD_GENDER:			//用户性别
		{
			BYTE cbGender=pIClientUserItem->GetGender();
			lstrcpyn(pszString,(cbGender==GENDER_MANKIND)?TEXT("男"):TEXT("女"),wMaxCount);
			break;
		}
	case UD_GROUP_NAME:		//社团名字
		{
			_sntprintf(pszString,wMaxCount,TEXT("%s"),pIClientUserItem->GetGroupName());
			break;
		}
	case UD_UNDER_WRITE:	//个性签名
		{
			_sntprintf(pszString,wMaxCount,TEXT("%s"),pIClientUserItem->GetUnderWrite());
			break;
		}
	case UD_TABLE:			//桌子号码
		{
			WORD wTableID=pIClientUserItem->GetTableID();
			if (wTableID!=INVALID_TABLE) _sntprintf(pszString,wMaxCount,TEXT("%ld"),wTableID+1);
			break;
		}
	case UD_CHAIR:			//椅子号码
		{
			WORD wChairID=pIClientUserItem->GetChairID();
			if (wChairID!=INVALID_TABLE) _sntprintf(pszString,wMaxCount,TEXT("%ld"),wChairID+1);
			break;
		}
	case UD_SCORE:			//用户积分
		{
			_sntprintf(pszString,wMaxCount,TEXT("%I64d"),pIClientUserItem->GetUserScore());
			break;
		}
	case UD_GRADE:			//用户成绩
		{
			_sntprintf(pszString,wMaxCount,TEXT("%I64d"),pIClientUserItem->GetUserGrade());
			break;
		}
	case UD_USER_MEDAL:		//用户奖牌
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserMedal());
			break;
		}
	case UD_EXPERIENCE:		//用户经验
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserExperience());
			break;
		}
	case UD_LOVELINESS:		//用户魅力
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserLoveLiness());
			break;
		}
	case UD_WIN_COUNT:		//胜局盘数
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserWinCount());
			break;
		}
	case UD_LOST_COUNT:		//输局盘数
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserLostCount());
			break;
		}
	case UD_DRAW_COUNT:		//和局盘数
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserDrawCount());
			break;
		}
	case UD_FLEE_COUNT:		//逃局盘数
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserFleeCount());
			break;
		}
	case UD_PLAY_COUNT:		//游戏局数
		{
			_sntprintf(pszString,wMaxCount,TEXT("%ld"),pIClientUserItem->GetUserPlayCount());
			break;
		}
	case UD_WIN_RATE:		//用户胜率
		{
			FLOAT fWinRate=pIClientUserItem->GetUserWinRate();
			if (fWinRate!=0.0f) _sntprintf(pszString,wMaxCount,TEXT("%4.2f%%"),fWinRate);
			break;
		}
	case UD_LOST_RATE:		//用户输率
		{
			FLOAT fLostRate=pIClientUserItem->GetUserLostRate();
			if (fLostRate==0.0f) _sntprintf(pszString,wMaxCount,TEXT("%4.2f%%"),fLostRate);
			break;
		}
	case UD_DRAW_RATE:		//用户和率
		{
			FLOAT fDrawRate=pIClientUserItem->GetUserDrawRate();
			if (fDrawRate==0.0f) _sntprintf(pszString,wMaxCount,TEXT("%4.2f%%"),fDrawRate);
			break;
		}
	case UD_FLEE_RATE:		//用户逃率
		{
			FLOAT fFleeRate=pIClientUserItem->GetUserFleeRate();
			if (fFleeRate==0.0f) _sntprintf(pszString,wMaxCount,TEXT("%4.2f%%"),fFleeRate);
			break;
		}
	case UD_GAME_LEVEL:		//游戏等级
		{
			//无效处理
			if (m_pIGameLevelParser==NULL)
			{
				ASSERT(FALSE);
				break;
			}
				
			//获取等级
			_sntprintf(pszString,wMaxCount,TEXT("%s"),m_pIGameLevelParser->GetLevelDescribe(pIClientUserItem));

			break;
		}
	case UD_NOTE_INFO:		//用户备注
		{
			lstrcpyn(pszString,pIClientUserItem->GetUserNoteInfo(),wMaxCount);
			break;
		}
	case UD_LOOKON_USER:	//旁观用户
		{
			//无效处理
			if (m_pIGameUserManager==NULL)
			{
				ASSERT(FALSE);
				break;
			}

			//获取用户
			if (pIClientUserItem->GetUserStatus()==US_LOOKON)
			{
				//获取信息
				WORD wChairID=pIClientUserItem->GetChairID();
				IClientUserItem * pITableUserItem=m_pIGameUserManager->GetTableUserItem(wChairID);

				//结果处理
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

//获取颜色
VOID CUserListControl::GetItemColor(LPDRAWITEMSTRUCT lpDrawItemStruct, COLORREF&crColorText, COLORREF&crColorBack)
{
	//定制变量
	ASSERT(GetClientUserItem((VOID *)lpDrawItemStruct->itemData)!=NULL);
	IClientUserItem * pIClientUserItem=GetClientUserItem((VOID *)lpDrawItemStruct->itemData);

	//颜色定制
	if (lpDrawItemStruct->itemState&ODS_SELECTED)
	{
		//选择颜色
		crColorText=m_ColorUserList.crSelectTX;
		crColorBack=m_ColorUserList.crSelectBK;
	}
	else if (pIClientUserItem->GetUserID()==m_dwMySelfUserID)
	{
		//自己颜色
		crColorText=m_ColorUserList.crMyselfTX;
		crColorBack=m_ColorUserList.crMasterBK;
	}
	else if (pIClientUserItem->GetMasterOrder()!=0)
	{
		//管理颜色
		crColorText=m_ColorUserList.crMasterTX;
		crColorBack=m_ColorUserList.crMasterBK;
	}
	else if (pIClientUserItem->GetMemberOrder()!=0)
	{
		//会员颜色
		crColorText=m_ColorUserList.crMemberTX;
		crColorBack=m_ColorUserList.crMemberBK;
	}
	else
	{
		//默认颜色
		crColorText=m_ColorUserList.crNormalTX;
		crColorBack=m_ColorUserList.crNormalBK;
	}

	return;
}

//绘画数据
VOID CUserListControl::DrawCustomItem(CDC * pDC, LPDRAWITEMSTRUCT lpDrawItemStruct, CRect&rcSubItem, INT nColumnIndex)
{
	//参数效验
	ASSERT(nColumnIndex<m_wColumnCount);
	if (nColumnIndex>=m_wColumnCount) return;

	//变量定义
	INT nItemID=lpDrawItemStruct->itemID;
	IClientUserItem * pIClientUserItem=GetClientUserItem((VOID *)lpDrawItemStruct->itemData);

	//绘画信息
	switch (m_cbDataDescribe[nColumnIndex])
	{
	case UD_IMAGE_FLAG:		//用户标志
		{
			//变量定义
			UINT uImageIndex=INDEX_NORMAL;
			BYTE cbCompanion=pIClientUserItem->GetUserCompanion();
			tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();

			//索引计算
			if ((cbCompanion==CP_FRIEND)&&(pIClientUserItem->GetUserID()!=m_dwMySelfUserID)) uImageIndex=INDEX_FRIEND;
			if ((cbCompanion==CP_DETEST)&&(pIClientUserItem->GetUserID()!=m_dwMySelfUserID)) uImageIndex=INDEX_FRIEND+1;
			if ((uImageIndex==INDEX_NORMAL)&&(pIClientUserItem->GetMasterOrder()>0)) uImageIndex=INDEX_MASTER+(pUserInfo->cbMasterOrder-1);
			if ((uImageIndex==INDEX_NORMAL)&&(pIClientUserItem->GetMemberOrder()>0)) uImageIndex=INDEX_MEMBER+(pUserInfo->cbMemberOrder-1);

			//绘画标志
			m_ImageUserStatus.Draw(pDC,uImageIndex,CPoint(rcSubItem.left,rcSubItem.top),ILD_TRANSPARENT);

			break;
		}
	case UD_IMAGE_STATUS:	//用户状态
		{
			//索引计算
			BYTE cbGender=pIClientUserItem->GetGender();
			BYTE cbUserStatus=pIClientUserItem->GetUserStatus();
			UINT uImageIndex=INDEX_STATUS+cbGender*COUNT_STATUS+cbUserStatus-US_FREE;

			//绘画标志
			m_ImageUserStatus.Draw(pDC,uImageIndex,CPoint(rcSubItem.left,rcSubItem.top),ILD_TRANSPARENT);

			break;
		}
	case UD_EXPERIENCE:	//用户经验
		{
			//变量定义
			ASSERT(CUserItemElement::GetInstance()!=NULL);
			CUserItemElement * pUserItemElement=CUserItemElement::GetInstance();

			//绘画经验
			DWORD dwExperience=pIClientUserItem->GetUserExperience();
			pUserItemElement->DrawExperience(pDC,rcSubItem.left,rcSubItem.top,dwExperience);

			break;
		}
	default:			//默认调用
		{
			__super::DrawCustomItem(pDC,lpDrawItemStruct,rcSubItem,nColumnIndex);
		}
	}

	return;
}

//解释用户
IClientUserItem * CUserListControl::GetClientUserItem(VOID * pItemData)
{
	return (IClientUserItem *)pItemData;
}

//设置接口
bool CUserListControl::SetMySelfUserID(DWORD dwMySelfUserID)
{
	//设置变量
	m_dwMySelfUserID=dwMySelfUserID;

	return true;
}

//设置接口
bool CUserListControl::SetGameLevelParser(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGameLevelParser)!=NULL);
		m_pIGameLevelParser=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGameLevelParser);

		//成功判断
		if (m_pIGameLevelParser==NULL) return false;
	}
	else m_pIGameLevelParser=NULL;

	return true;
}

//设置接口
bool CUserListControl::SetGameUserManager(IUnknownEx * pIUnknownEx)
{
	//设置接口
	if (pIUnknownEx!=NULL)
	{
		//查询接口
		ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGameUserManager)!=NULL);
		m_pIGameUserManager=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,IGameUserManager);

		//成功判断
		if (m_pIGameUserManager==NULL) return false;
	}
	else m_pIGameUserManager=NULL;

	return true;
}

//设置颜色
bool CUserListControl::SetColorUserList(tagColorUserList&ColorUserList)
{
	//设置变量
	m_ColorUserList=ColorUserList;

	//更新界面
	if (m_hWnd!=NULL)
	{
		//设置背景
		SetBkColor(m_ColorUserList.crNormalBK);

		//重画窗口
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}

	return true;
}

//设置列表
bool CUserListControl::SetColumnDescribe(tagColumnItem ColumnItem[], WORD wColumnCount)
{
	//构造判断
	if (m_bCreateColumn==true) return true;

	//调整参数
	ASSERT(wColumnCount<(CountArray(m_cbDataDescribe)-m_wColumnCount));
	wColumnCount=__min(CountArray(m_cbDataDescribe)-m_wColumnCount,wColumnCount);

	//设置变量
	m_bCreateColumn=true;

	//设置列表
	for (WORD i=0;i<wColumnCount;i++)
	{
		m_cbDataDescribe[m_wColumnCount]=ColumnItem[i].cbDataDescribe;
		InsertColumn(m_wColumnCount++,ColumnItem[i].szColumnName,LVCFMT_LEFT,ColumnItem[i].cbColumnWidth);
	}

	return true;
}

//销毁消息
VOID CUserListControl::OnNcDestroy()
{
	//资源变量
	m_dwMySelfUserID=0L;
	m_bCreateColumn=false;

	//接口变量
	m_pIGameLevelParser=NULL;
	m_pIGameUserManager=NULL;

	//列表信息
	m_wColumnCount=0;
	ZeroMemory(m_cbDataDescribe,sizeof(m_cbDataDescribe));

	__super::OnNcDestroy();
}

//////////////////////////////////////////////////////////////////////////////////
