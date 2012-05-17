#include "StdAfx.h"
#include "UserItemElement.h"

//////////////////////////////////////////////////////////////////////////////////
//������

#define MAX_EXPERIENCE				5									//�����

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CUserItemElement * CUserItemElement::m_pUserItemElement=NULL;			//����ָ��

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CUserItemElement::CUserItemElement()
{
	//���ñ���
	m_pIUserOrderParser=NULL;
	m_pIFaceItemControl=NULL;

	//���ö���
	ASSERT(m_pUserItemElement==NULL);
	if (m_pUserItemElement==NULL) m_pUserItemElement=this;

	return;
}

//��������
CUserItemElement::~CUserItemElement()
{
	//�ͷŶ���
	ASSERT(m_pUserItemElement==this);
	if (m_pUserItemElement==this) m_pUserItemElement=NULL;

	return;
}

//���ýӿ�
VOID CUserItemElement::SetUserOrderParser(IUserOrderParser * pIUserOrderParser)
{
	//���ñ���
	m_pIUserOrderParser=pIUserOrderParser;

	return;
}

//���ýӿ�
VOID CUserItemElement::SetFaceItemControl(IFaceItemControl * pIFaceItemControl)
{
	//���ñ���
	m_pIFaceItemControl=pIFaceItemControl;

	return;
}

//�滭����
VOID CUserItemElement::DrawExperience(CDC * pDC, INT nXPos, INT nYPos, DWORD dwExperience)
{
	if (dwExperience>0L)
	{
		//������Դ
		CPngImage ImageExperience;
		ImageExperience.LoadImage(GetModuleHandle(SHARE_CONTROL_DLL_NAME),TEXT("USER_EXPERIENCE"));

		//��ȡ����
		CSize SizeExperience;
		SizeExperience.SetSize(ImageExperience.GetWidth()/3,ImageExperience.GetHeight());

		//��ȡ����
		BYTE cbOrderIndex[MAX_EXPERIENCE];
		WORD wExperienceLevel=GetExperienceLevel(dwExperience);
		WORD wIndexCount=GetExperienceIndex(wExperienceLevel,cbOrderIndex,CountArray(cbOrderIndex));

		//�滭����
		for (WORD i=0;i<wIndexCount;i++)
		{
			INT nXDrawPos=nXPos+i*(SizeExperience.cx+2L);
			ImageExperience.DrawImage(pDC,nXDrawPos,nYPos,SizeExperience.cx,SizeExperience.cy,cbOrderIndex[i]*SizeExperience.cx,0);
		}
	}

	return;
}

//������Ƭ
VOID CUserItemElement::ConstructNameCard(IClientUserItem * pIClientUserItem, IGameLevelParser * pIGameLevelParser, CImage&ImageNameCard)
{
	//������Ƭ
	CSize ImageSize;
	ImageSize.SetSize(NAME_CARD_CX,NAME_CARD_CY);
	ImageNameCard.Create(ImageSize.cx,ImageSize.cy,32);

	//��������
	TCHAR szString[128]=TEXT("");
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	//���� DC
	CDC * pDC=CDC::FromHandle(ImageNameCard.GetDC());
	pDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

	//���� DC
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(CSkinMenuKernel::m_SkinAttribute.m_crNormalText);

	//λ�ö���
	CRect rcNickName(60,5,ImageSize.cx-5,5+12);
	CRect rcGameLevel(60,25,ImageSize.cx-5,25+12);
	CRect rcExperience(60,45,ImageSize.cx-5,45+12);
	CRect rcUserGameID(60,65,ImageSize.cx-5,65+12);

	//�滭����
	pDC->FillSolidRect(0,0,ImageSize.cx,ImageSize.cy,CSkinMenuKernel::m_SkinAttribute.m_crNormalBack);

	//�滭ͷ��
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

	//�û���Ϣ
	LPCTSTR pszMember=m_pIUserOrderParser->GetMemberDescribe(pIClientUserItem->GetMemberOrder());
	LPCTSTR pszMaster=m_pIUserOrderParser->GetMasterDescribe(pIClientUserItem->GetMasterOrder());

	//�û���Ϣ
	LPCTSTR pszUserOrder=(pszMaster[0]!=0)?pszMaster:((pszMember[0]!=0)?pszMember:TEXT("��ͨ���"));
	_sntprintf(szString,CountArray(szString),TEXT("�ǳƣ�%s [ %s ]"),pIClientUserItem->GetNickName(),pszUserOrder);
	pDC->DrawText(szString,lstrlen(szString),&rcNickName,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//��Ϸ�ȼ�
	LPCTSTR pszKindName=pIGameLevelParser->GetKindName();
	LPCTSTR pszGameLevel=pIGameLevelParser->GetLevelDescribe(pIClientUserItem);
	_sntprintf(szString,CountArray(szString),TEXT("����%s [ %s ]"),pszGameLevel,pszKindName);
	pDC->DrawText(szString,lstrlen(szString),&rcGameLevel,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//�û�����
	DWORD dwExperience=pIClientUserItem->GetUserExperience();
	_sntprintf(szString,CountArray(szString),TEXT("���飺%d ��"),GetExperienceLevel(dwExperience));
	INT nExperienceLength=pDC->DrawText(szString,lstrlen(szString),&rcExperience,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//�����־
	if (dwExperience>0L)
	{
		DrawExperience(pDC,rcExperience.left+nExperienceLength*6+15,rcExperience.top-4L,dwExperience);
	}

	//�û���ʶ
	_sntprintf(szString,CountArray(szString),TEXT("���룺%ld"),pIClientUserItem->GetGameID());
	pDC->DrawText(szString,lstrlen(szString),&rcUserGameID,DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);

	//�ͷ���Դ
	ImageNameCard.ReleaseDC();

	return;
}

//����ȼ�
BYTE CUserItemElement::GetPasswordLevel(LPCTSTR pszPassword)
{
	//��������
	BYTE cbPasswordInfo[4]={0,0,0,0};
	UINT uPasswordLength=lstrlen(pszPassword);

	//�����ж�
	if (lstrlen(pszPassword)<LEN_LESS_PASSWORD)
	{
		return PASSWORD_LEVEL_0;
	}

	//Сд��ĸ
	for (UINT i=0;i<uPasswordLength;i++)
	{
		//Сд��ĸ
		if ((pszPassword[i]>=TEXT('a'))&&((pszPassword[i]<=TEXT('z'))))
		{
			cbPasswordInfo[0]=1;
			continue;
		}

		//��д��ĸ
		if ((pszPassword[i]>=TEXT('A'))&&((pszPassword[i]<=TEXT('Z'))))
		{
			cbPasswordInfo[1]=1;
			continue;
		}

		//�����ַ�
		if ((pszPassword[i]>=TEXT('0'))&&((pszPassword[i]<=TEXT('9'))))
		{
			cbPasswordInfo[2]=1;
			continue;
		}

		//�����ַ�
		cbPasswordInfo[3]=1;
	}

	//�жϵȼ�
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

//��ȡ�ȼ�
WORD CUserItemElement::GetExperienceLevel(DWORD dwExperience)
{
	if (dwExperience>0L)
	{
		//��������
		DWORD dwIncrease=0L;
		DWORD dwLevelValue=0L;

		//�ȼ�����
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

//�ȼ���Ϣ
WORD CUserItemElement::GetExperienceIndex(WORD wUserLevel, BYTE cbOrderIndex[], WORD wMaxCount)
{
	//��������
	WORD wIndexCount=0;
	WORD wLevelVaule[]={1L,5L,10L};

	//���õȼ�
	for (WORD i=0;i<wMaxCount;i++)
	{
		//��ȡ����
		for (INT j=0;j<CountArray(wLevelVaule);j++)
		{
			if (wUserLevel>=wLevelVaule[CountArray(wLevelVaule)-j-1])
			{
				//���ñ���
				wUserLevel-=wLevelVaule[CountArray(wLevelVaule)-j-1];
				cbOrderIndex[wIndexCount++]=CountArray(wLevelVaule)-j-1;

				break;
			}
		}

		//��ֹ�ж�
		if (wUserLevel==0L) break;
	}

	return wIndexCount;
}

//Ч���ʺ�
bool CUserItemElement::EfficacyAccounts(LPCTSTR pszAccounts, TCHAR szDescribe[], WORD wMaxCount)
{
	//��������
	INT nCharLength=lstrlen(pszAccounts);

	//�����ж�
	if (nCharLength<LEN_LESS_ACCOUNTS)
	{
		//��������
		INT nChinessCount=0;

		//�����ַ�
		for (INT i=0;i<nCharLength;i++)
		{
			if (pszAccounts[i]>=0x80)
			{
				nChinessCount++;
			}
		}

		//�����ж�
		if ((nChinessCount+nCharLength)<LEN_LESS_ACCOUNTS)
		{
			_sntprintf(szDescribe,wMaxCount,TEXT("��Ϸ�ʺű������ %ld λ���ϣ����������룡"),LEN_LESS_ACCOUNTS);
			return false;
		}
	}

	return true;
}

//Ч���ǳ�
bool CUserItemElement::EfficacyNickName(LPCTSTR pszNickName, TCHAR szDescribe[], WORD wMaxCount)
{
	//��������
	INT nCharLength=lstrlen(pszNickName);
	INT nDataLength=nCharLength*sizeof(TCHAR);

	//�����ж�
	if (nCharLength<LEN_LESS_NICKNAME)
	{
		//��������
		WORD nChinessCount=0;

		//�����ַ�
		for (INT i=0;i<nCharLength;i++)
		{
			if (pszNickName[i]>=0x80)
			{
				nChinessCount++;
			}
		}

		//�����ж�
		if ((nChinessCount+nCharLength)<LEN_LESS_NICKNAME)
		{
			_sntprintf(szDescribe,wMaxCount,TEXT("��Ϸ�ǳƱ������ %ld λ���ϣ����������룡"),LEN_LESS_NICKNAME);
			return false;
		}
	}

	return true;
}

//Ч������
bool CUserItemElement::EfficacyPassword(LPCTSTR pszPassword, TCHAR szDescribe[], WORD wMaxCount)
{
	//��������
	INT nCharLength=lstrlen(pszPassword);
	INT nDataLength=nCharLength*sizeof(TCHAR);

	//�����ж�
	if (nCharLength<LEN_LESS_PASSWORD)
	{
		_sntprintf(szDescribe,wMaxCount,TEXT("���������� %ld λ���ϣ����������룡"),LEN_LESS_PASSWORD);
		return false;
	}

	return true;
}

//Ч��֤��
bool CUserItemElement::EfficacyPassPortID(LPCTSTR pszPassPortID, TCHAR szDescribe[], WORD wMaxCount)
{
	//��������
	bool bNumber=true;
	UINT nCharLength=lstrlen(pszPassPortID);

	//�����ж�
	for (UINT i=0;i<nCharLength;i++)
	{
		if ((pszPassPortID[i]<TEXT('0'))||(pszPassPortID[i]>TEXT('9')))
		{
			bNumber=false;
			break;
		}
	}

	//�Ϸ��ж�
	if ((bNumber==false)||((nCharLength!=0L)&&(nCharLength!=15L)&&(nCharLength!=18L)))
	{
		lstrcpyn(szDescribe,TEXT("���֤�������Ϊ 15 λ���� 18 λ���֣����������룡"),wMaxCount);
		return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////////////
