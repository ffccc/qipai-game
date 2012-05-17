#include "StdAfx.h"
#include "GlobalUnits.h"
#include "GameFrameAvatar.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CGameFrameAvatar *	CGameFrameAvatar::m_pGameFrameAvatar=NULL;			//引擎接口

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CGameFrameAvatar::CGameFrameAvatar()
{
	//设置变量
	m_wD3DLineCount=0;
	m_wD3DFullCount=0;

	//设置接口
	ASSERT(m_pGameFrameAvatar==NULL);
	if (m_pGameFrameAvatar==NULL) m_pGameFrameAvatar=this;

	return;
}

//析构函数
CGameFrameAvatar::~CGameFrameAvatar()
{
	//设置接口
	ASSERT(m_pGameFrameAvatar==this);
	if (m_pGameFrameAvatar==this) m_pGameFrameAvatar=NULL;

	//销毁资源
	if (m_D3DTextureAvatar.IsNull()==false) m_D3DTextureAvatar.Destory();

	return;
}

//配置函数
VOID CGameFrameAvatar::Initialization(CD3DDevice * pD3DDevice)
{
	//获取对象
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//获取资源
	tagAvatarResource AvatarResource;
	pGlobalUnits->m_FaceItemControlModule->GetAvatarResource(AvatarResource);

	//加载资源
	m_D3DTextureAvatar.LoadImage(pD3DDevice,AvatarResource.hResInstance,AvatarResource.pszResource,TEXT("PNG"));

	//设置变量
	m_wD3DLineCount=m_D3DTextureAvatar.GetWidth()/FACE_CX;
	m_wD3DFullCount=m_wD3DLineCount*m_D3DTextureAvatar.GetHeight()/(FACE_CY*2);

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID)
{
	//获取对象
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//获取接口
	IFaceItemControl * pIFaceItemControl=pGlobalUnits->m_FaceItemControlModule.GetInterface();

	//绘画头像
	ASSERT(pIFaceItemControl!=NULL);
	if (pIFaceItemControl!=NULL) pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,wFaceID);

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CDC * pDC, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	//获取对象
	ASSERT(CGlobalUnits::GetInstance()!=NULL);
	CGlobalUnits * pGlobalUnits=CGlobalUnits::GetInstance();

	//获取接口
	IFaceItemControl * pIFaceItemControl=pGlobalUnits->m_FaceItemControlModule.GetInterface();

#ifdef _DEBUG

	//调试头像
	if (pIClientUserItem==NULL)
	{
		pIFaceItemControl->DrawFaceNormal(pDC,nXPos,nYPos,WORD(0));
		return;
	}

#endif

	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	WORD wFaceID=pIClientUserItem->GetFaceID();
	bool bOffLine=(pIClientUserItem->GetUserStatus()==US_OFFLINE);

	//绘画头像
	if ((pUserInfo->dwCustomID!=0)&&(pCustomFaceInfo->dwDataSize!=0))
	{
		//绘画头像
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
		//变量定义
		WORD wFaceID=pUserInfo->wFaceID;
		if (wFaceID>=pIFaceItemControl->GetFaceCount()) wFaceID=0;

		//绘画头像
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

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, WORD wFaceID)
{
	//效验参数
	ASSERT(m_D3DTextureAvatar.IsNull()==false);
	if (m_D3DTextureAvatar.IsNull()==true) return;

	//调整参数
	if (wFaceID>=m_wD3DFullCount) wFaceID=0;

	//绘画头像
	INT nXImagePos=wFaceID%m_wD3DLineCount;
	INT nYImagePos=(wFaceID/m_wD3DLineCount)*2L;
	m_D3DTextureAvatar.DrawImage(pD3DDevice,nXPos,nYPos,FACE_CX,FACE_CY,nXImagePos*FACE_CX,nYImagePos*FACE_CY);

	return;
}

//绘画头像
VOID CGameFrameAvatar::DrawUserAvatar(CD3DDevice * pD3DDevice, INT nXPos, INT nYPos, IClientUserItem * pIClientUserItem)
{
	//效验参数
	ASSERT(m_D3DTextureAvatar.IsNull()==false);
	if (m_D3DTextureAvatar.IsNull()==true) return;

#ifdef _DEBUG

	//调试头像
	if (pIClientUserItem==NULL)
	{
		m_D3DTextureAvatar.DrawImage(pD3DDevice,nXPos,nYPos,FACE_CX,FACE_CY,0,0);
		return;
	}

#endif

	//效验参数
	ASSERT(pIClientUserItem!=NULL);
	if (pIClientUserItem==NULL) return;

	//变量定义
	tagUserInfo * pUserInfo=pIClientUserItem->GetUserInfo();
	tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

	WORD wFaceID=pIClientUserItem->GetFaceID();
	bool bOffLine=(pIClientUserItem->GetUserStatus()==US_OFFLINE);

	//绘画头像
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
		//变量定义
		WORD wFaceID=pUserInfo->wFaceID;
		if (wFaceID>=m_wD3DFullCount) wFaceID=0;

		//绘画头像
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
