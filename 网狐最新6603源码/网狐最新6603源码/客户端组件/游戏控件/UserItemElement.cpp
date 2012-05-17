#include "StdAfx.h"
#include "UserItemElement.h"

//////////////////////////////////////////////////////////////////////////////////
//常理定义

#define MAX_EXPERIENCE				5									//最大经验

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CUserItemElement * CUserItemElement::m_pUserItemElement=NULL;			//对象指针

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CUserItemElement::CUserItemElement()
{
	//设置变量
	m_pIUserOrderParser=NULL;
	m_pIFaceItemControl=NULL;

	//设置对象
	ASSERT(m_pUserItemElement==NULL);
	if (m_pUserItemElement==NULL) m_pUserItemElement=this;

	return;
}

//析构函数
CUserItemElement::~CUserItemElement()
{
	//释放对象
	ASSERT(m_pUserItemElement==this);
	if (m_pUserItemElement==this) m_pUserItemElement=NULL;

	return;
}

//设置接口
VOID CUserItemElement::SetUserOrderParser(IUserOrderParser * pIUserOrderParser)
{
	//设置变量
	m_pIUserOrderParser=pIUserOrderParser;

	return;
}

//设置接口
VOID CUserItemElement::SetFaceItemControl(IFaceItemControl * pIFaceItemControl)
{
	//设置变量
	m_pIFaceItemControl=pIFaceItemControl;

	return;
}

//绘画经验
VOID CUserItemElement::DrawExperience(CDC * pDC, INT nXPos, INT nYPos, DWORD dwExperience)
{
	if (dwExperience>0L)
	{
		//加载资源
		CPngImage ImageExperience;
		ImageExperience.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("USER_EXPERIENCE"));

		//获取属性
		CSize SizeExperience;
		SizeExperience.SetSize(ImageExperience.GetWidth()/3,ImageExperience.GetHeight());

		//获取经验
		BYTE cbOrderIndex[MAX_EXPERIENCE];
		WORD wExperienceLevel=GetExperienceLevel(dwExperience);
		WORD wIndexCount=GetExperienceIndex(wExperienceLevel,cbOrderIndex,CountArray(cbOrderIndex));

		//绘画经验
		for (WORD i=0;i<wIndexCount;i++)
		{
			INT nXDrawPos=nXPos+i*(SizeExperience.cx+2L);
			ImageExperience.DrawImage(pDC,nXDrawPos,nYPos,SizeExperience.cx,SizeExperience.cy,cbOrderIndex[i]*SizeExperience.cx,0);
		}
	}

	return;
}

//构造名片
VOID CUserItemElement::ConstructNameCard(IClientUserItem * pIClientUserItem, IGameLevelParser * pIGameLevelParser, CImage&ImageNameCard)
{
	//创建名片
	CSize ImageSize;
	ImageSize.SetSize(NAME_CARD_CX,NAME_CARD_CY);
	ImageNameCard.Create(ImageSize.cx,ImageSize.cy,32);

	//变量定义
	TCHAR szString[128]=TEXT("");
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//设置 DC
	CDC * pDC=CDC::FromHandle(ImageNameCard.GetDC());
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//设置 DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(CSkinMenuKernel::m_SkinAttribute.m_crNormalText);

	//位置定义
	CRect rcNickName(60,5,ImageSize.cx-5,5+12);
	CRect rcGameLevel(60,25,ImageSize.cx-5,25+12);
	CRect rcExperience(60,45,ImageSize.cx-5,45+12);
	CRect rcUserGameID(60,65,ImageSize.cx-5,65+12);

	//绘画背景
	pDC->FillSolidRect(0,0,ImageSize.cx,ImageSize.cy,CSkinMenuKernel::m_SkinAttribute.m_crNormalBack);

	//绘画头像
	if ((pCustomFaceInfo->dwDataSize!=0L)&&(pIClientUserItem->GetCustomID()!=0L))
	{
		m_pIFaceItemControl->DrawFaceItemFrame(pDC,2,16,ITEM_FRAME_NORMAL);
		m_pIFaceItemControl->DrawFaceNormal(pDC,2,16,pCustomFaceInfo->dwCustomFace);
	}
	else
	{
		m_pIFaceItemControl->DrawFaceItemFrame(pDC,2,16,ITEM_FRAME_NORMAL);
		m_pIFaceItemControl->DrawFaceNormal(pDC,2,16,pIClientUserItem->GetFaceID());
	}

	//用户信息
	LPCTSTR pszMember=m_pIUserOrderParser->GetMemberDescribe(pIClientUserItem->GetMemberOrder());
	LPCTSTR pszMaster=m_pIUserOrderParser->GetMasterDescribe(pIClientUserItem->GetMasterOrder());

	//用户信息
	LPCTSTR pszUserOrder=(pszMaster[0]!=0)?pszMaster:((pszMember[0]!=0)?pszMember:TEXT("普通玩家"));
	_sntprintf(szString,CountArray(szString),TEXT("昵称：%s [ %s ]"),pIClientUserItem->GetNickName(),pszUserOrder);
	pDC->DrawText(szString,lstrlen(szString),&rcNickName,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//游戏等级
	LPCTSTR pszKindName=pIGameLevelParser->GetKindName();
	LPCTSTR pszGameLevel=pIGameLevelParser->GetLevelDescribe(pIClientUserItem);
	_sntprintf(szString,CountArray(szString),TEXT("级别：%s [ %s ]"),pszGameLevel,pszKindName);
	pDC->DrawText(szString,lstrlen(szString),&rcGameLevel,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//用户经验
	DWORD dwExperience=pIClientUserItem->GetUserExperience();
	_sntprintf(szString,CountArray(szString),TEXT("经验：%d 级"),GetExperienceLevel(dwExperience));
	INT nExperienceLength=pDC->DrawText(szString,lstrlen(szString),&rcExperience,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//经验标志
	if (dwExperience>0L)
	{
		DrawExperience(pDC,rcExperience.left+nExperienceLength*6+15,rcExperience.top-4L,dwExperience);
	}

	//用户标识
	_sntprintf(szString,CountArray(szString),TEXT("号码：%ld"),pIClientUserItem->GetGameID());
	pDC->DrawText(szString,lstrlen(szString),&rcUserGameID,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//释放资源
	ImageNameCard.ReleaseDC();

	return;
}

//密码等级
BYTE CUserItemElement::GetPasswordLevel(LPCTSTR pszPassword)
{
	//变量定义
	BYTE cbPasswordInfo[4]={0,0,0,0};
	UINT uPasswordLength=lstrlen(pszPassword);

	//长度判断
	if (lstrlen(pszPassword)<LEN_LESS_PASSWORD)
	{
		return PASSWORD_LEVEL_0;
	}

	//小写字母
	for (UINT i=0;i<uPasswordLength;i++)
	{
		//小写字母
		if ((pszPassword[i]>=TEXT('a'))&&((pszPassword[i]<=TEXT('z'))))
		{
			cbPasswordInfo[0]=1;
			continue;
		}

		//大写字母
		if ((pszPassword[i]>=TEXT('A'))&&((pszPassword[i]<=TEXT('Z'))))
		{
			cbPasswordInfo[1]=1;
			continue;
		}

		//数字字符
		if ((pszPassword[i]>=TEXT('0'))&&((pszPassword[i]<=TEXT('9'))))
		{
			cbPasswordInfo[2]=1;
			continue;
		}

		//其他字符
		cbPasswordInfo[3]=1;
	}

	//判断等级
	BYTE cbLevelCount=0;
	for (BYTE i=0;i<CountArray(cbPasswordInfo);i++)
	{
		if (cbPasswordInfo[i]>0)
		{
			cbLevelCount++;
		}
	}

	return PASSWORD_LEVEL_0+__min(cbLevelCount,(PASSWORD_LEVEL_3-PASSWORD_LEVEL_0));
}

//获取等级
WORD CUserItemElement::GetExperienceLevel(DWORD dwExperience)
{
	if (dwExperience>0L)
	{
		//变量定义
		DWORD dwIncrease=0L;
		DWORD dwLevelValue=0L;

		//等级计算
		for (WORD wUserLevel=1;wUserLevel<60L;wUserLevel++)
		{
			dwIncrease+=wUserLevel*30L;
			dwLevelValue=dwLevelValue+dwIncrease;
			if (dwExperience<dwLevelValue) break;
		}

		return wUserLevel;
	}

	return 0L;
}

//等级信息
WORD CUserItemElement::GetExperienceIndex(WORD wUserLevel, BYTE cbOrderIndex[], WORD wMaxCount)
{
	//变量定义
	WORD wIndexCount=0;
	WORD wLevelVaule[]={1L,5L,10L};

	//设置等级
	for (WORD i=0;i<wMaxCount;i++)
	{
		//获取索引
		for (INT j=0;j<CountArray(wLevelVaule);j++)
		{
			if (wUserLevel>=wLevelVaule[CountArray(wLevelVaule)-j-1])
			{
				//设置变量
				wUserLevel-=wLevelVaule[CountArray(wLevelVaule)-j-1];
				cbOrderIndex[wIndexCount++]=CountArray(wLevelVaule)-j-1;

				break;
			}
		}

		//终止判断
		if (wUserLevel==0L) break;
	}

	return wIndexCount;
}

//效验帐号
bool CUserItemElement::EfficacyAccounts(LPCTSTR pszAccounts, TCHAR szDescribe[], WORD wMaxCount)
{
	//变量定义
	INT nCharLength=lstrlen(pszAccounts);

	//长度判断
	if (nCharLength<LEN_LESS_ACCOUNTS)
	{
		//变量定义
		INT nChinessCount=0;

		//计算字符
		for (INT i=0;i<nCharLength;i++)
		{
			if (pszAccounts[i]>=0x80)
			{
				nChinessCount++;
			}
		}

		//长度判断
		if ((nChinessCount+nCharLength)<LEN_LESS_ACCOUNTS)
		{
			_sntprintf(szDescribe,wMaxCount,TEXT("游戏帐号必须大于 %ld 位以上，请重新输入！"),LEN_LESS_ACCOUNTS);
			return false;
		}
	}

	return true;
}

//效验昵称
bool CUserItemElement::EfficacyNickName(LPCTSTR pszNickName, TCHAR szDescribe[], WORD wMaxCount)
{
	//变量定义
	INT nCharLength=lstrlen(pszNickName);
	INT nDataLength=nCharLength*sizeof(TCHAR);

	//长度判断
	if (nCharLength<LEN_LESS_NICKNAME)
	{
		//变量定义
		WORD nChinessCount=0;

		//计算字符
		for (INT i=0;i<nCharLength;i++)
		{
			if (pszNickName[i]>=0x80)
			{
				nChinessCount++;
			}
		}

		//长度判断
		if ((nChinessCount+nCharLength)<LEN_LESS_NICKNAME)
		{
			_sntprintf(szDescribe,wMaxCount,TEXT("游戏昵称必须大于 %ld 位以上，请重新输入！"),LEN_LESS_NICKNAME);
			return false;
		}
	}

	return true;
}

//效验密码
bool CUserItemElement::EfficacyPassword(LPCTSTR pszPassword, TCHAR szDescribe[], WORD wMaxCount)
{
	//变量定义
	INT nCharLength=lstrlen(pszPassword);
	INT nDataLength=nCharLength*sizeof(TCHAR);

	//长度判断
	if (nCharLength<LEN_LESS_PASSWORD)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("密码必须大于 %ld 位以上，请重新输入！"),LEN_LESS_PASSWORD);
		return false;
	}

	return true;
}

//效验证件
bool CUserItemElement::EfficacyPassPortID(LPCTSTR pszPassPortID, TCHAR szDescribe[], WORD wMaxCount)
{
	//变量定义
	bool bNumber=true;
	UINT nCharLength=lstrlen(pszPassPortID);

	//数字判断
	for (UINT i=0;i<nCharLength;i++)
	{
		if ((pszPassPortID[i]<TEXT('0'))||(pszPassPortID[i]>TEXT('9')))
		{
			bNumber=false;
			break;
		}
	}

	//合法判断
	if ((bNumber==false)||((nCharLength!=0L)&&(nCharLength!=15L)&&(nCharLength!=18L)))
	{
		lstrcpyn(szDescribe,TEXT("身份证号码必须为 15 位或者 18 位数字，请重新输入！"),wMaxCount);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
