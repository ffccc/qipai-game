#include "StdAfx.h"
#include "GlobalUnits.h"
#include "GameFrameAvatar.h"

//////////////////////////////////////////////////////////////////////////////////
//��̬����

CGameFrameAvatar *	CGameFrameAvatar::m_pGameFrameAvatar=NULL;			//����ӿ�

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CGameFrameAvatar::CGameFrameAvatar()
{
	//���ñ���
	m_wD3DLineCount=0;
	m_wD3DFullCount=0;

	//���ýӿ�
	ASSERT(m_pGameFrameAvatar==NULL);
	if (m_pGameFrameAvatar==NULL) m_pGameFrameAvatar=this;

	return;
}

//��������
CGameFrameAvatar::~CGameFrameAvatar()
{
	//���ýӿ�
	ASSERT(m_pGameFrameAvatar==this);
	if (m_pGameFrameAvatar==this) m_pGameFrameAvatar=NULL;

	//������Դ
	if (m_D3DTextureAvatar.IsNull()==false) m_D3DTextureAvatar.Destory();

	return;
}

//���ú���
VOID CGameFrameAvatar::Initialization(CD3DDevice * pD3DDevice)
{
	//��ȡ����
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��ȡ��Դ
	tagAvatarResource AvatarResource;
	pGlobalUnits->m_FaceItemControlModule->GetAvatarResource(AvatarResource);

	//������Դ
	m_D3DTextureAvatar.LoadImage(pD3DDevice,AvatarResource.hResInstance,AvatarResource.pszResource,TEXT("PNG"));

	//���ñ���
	m_wD3DLineCount=m_D3DTextureAvatar.GetWidth()/FACE_CX;
	m_wD3DFullCount=m_wD3DLineCount*m_D3DTextureAvatar.GetHeight()/(FACE_CY*2);

	return;
}

//�滭ͷ��
VOID CGameFrameAvatar::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID)
{
	//��ȡ����
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��ȡ�ӿ�
	IFaceItemControl * pIFaceItemControl=pGlobalUnits->m_FaceItemControlModule.GetInterface();

	//�滭ͷ��
	ASSERT(pIFaceItemControl!=NULL);
	if (pIFaceItemControl!=NULL) pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,wFaceID);

	return;
}

//�滭ͷ��
VOID CGameFrameAvatar::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	//��ȡ����
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//��ȡ�ӿ�
	IFaceItemControl * pIFaceItemControl=pGlobalUnits->m_FaceItemControlModule.GetInterface();

#ifdef _DEBUG

	//����ͷ��
	if (pIClientUserItem==NULL)
	{
		pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,WORD(0));
		return;
	}

#endif

	//Ч�����
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	WORD wFaceID=pIClientUserItem->GetFaceID();
	bool bOffLine=(pIClientUserItem->GetUserStatus()==US_OFFLINE);

	//�滭ͷ��
	if ((pUserInfo->dwCustomID!=0)&&(pCustomFaceInfo->dwDataSize!=0))
	{
		//�滭ͷ��
		if (bOffLine==false)
		{
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,pCustomFaceInfo->dwCustomFace);
		}
		else
		{
			pIFaceItemControl->DrawFaceOffLine(pDC,nXPos,nYPos,pCustomFaceInfo->dwCustomFace);
		}
	}
	else
	{
		//��������
		WORD wFaceID=pUserInfo->wFaceID;
		if (wFaceID>=pIFaceItemControl->GetFaceCount()) wFaceID=0;

		//�滭ͷ��
		if (bOffLine==false)
		{
			pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,wFaceID);
		}
		else
		{
			pIFaceItemControl->DrawFaceOffLine(pDC,nXPos,nYPos,wFaceID);
		}
	}

	return;
}

//�滭ͷ��
VOID CGameFrameAvatar::DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, WORD wFaceID)
{
	//Ч�����
	ASSERT(m_D3DTextureAvatar.IsNull()==false);
	if (m_D3DTextureAvatar.IsNull()==true) return;

	//��������
	if (wFaceID>=m_wD3DFullCount) wFaceID=0;

	//�滭ͷ��
	INT nXImagePos=wFaceID%m_wD3DLineCount;
	INT nYImagePos=(wFaceID/m_wD3DLineCount)*2L;
	m_D3DTextureAvatar.DrawImage(pD3DDevice,nXPos,nYPos,FACE_CX,FACE_CY,nXImagePos*FACE_CX,nYImagePos*FACE_CY);

	return;
}

//�滭ͷ��
VOID CGameFrameAvatar::DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	//Ч�����
	ASSERT(m_D3DTextureAvatar.IsNull()==false);
	if (m_D3DTextureAvatar.IsNull()==true) return;

#ifdef _DEBUG

	//����ͷ��
	if (pIClientUserItem==NULL)
	{
		m_D3DTextureAvatar.DrawImage(pD3DDevice,nXPos,nYPos,FACE_CX,FACE_CY,0,0);
		return;
	}

#endif

	//Ч�����
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//��������
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	WORD wFaceID=pIClientUserItem->GetFaceID();
	bool bOffLine=(pIClientUserItem->GetUserStatus()==US_OFFLINE);

	//�滭ͷ��
	if ((pUserInfo->dwCustomID!=0)&&(pCustomFaceInfo->dwDataSize!=0))
	{
		if (pUserInfo->cbUserStatus!=US_OFFLINE)
		{
		}
		else
		{
		}
	}
	else
	{
		//��������
		WORD wFaceID=pUserInfo->wFaceID;
		if (wFaceID>=m_wD3DFullCount) wFaceID=0;

		//�滭ͷ��
		if (pUserInfo->cbUserStatus!=US_OFFLINE)
		{
			INT nXImagePos=wFaceID%m_wD3DLineCount;
			INT nYImagePos=(wFaceID/m_wD3DLineCount)*2L;
			m_D3DTextureAvatar.DrawImage(pD3DDevice,nXPos,nYPos,FACE_CX,FACE_CY,nXImagePos*FACE_CX,nYImagePos*FACE_CY);
		}
		else
		{
			INT nXImagePos=wFaceID%m_wD3DLineCount;
			INT nYImagePos=(wFaceID/m_wD3DLineCount)*2L;
			m_D3DTextureAvatar.DrawImage(pD3DDevice,nXPos,nYPos,FACE_CX,FACE_CY,nXImagePos*FACE_CX,nYImagePos*FACE_CY+FACE_CY);
		}
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
