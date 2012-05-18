#include "StdAfx.h"
#include "Resource.h"
#include "FaceItemControl.h"

//////////////////////////////////////////////////////////////////////////////////
//静态变量

CFaceItemControl *	CFaceItemControl::m_pFaceItemControl=NULL;			//头像接口

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CFaceItemControl::CFaceItemControl()
{
	//设置对象
	ASSERT(m_pFaceItemControl==NULL);
	if (m_pFaceItemControl==NULL) m_pFaceItemControl=this;

	//加载资源
	HINSTANCE hResInstance=GetModuleHandle(AVATAR_CONTROL_DLL_NAME);
	m_ImageUserFace.LoadImage(hResInstance,TEXT("USER_FACE_IMAGE"));
	m_ImageItemFrame.LoadImage(hResInstance,TEXT("FACE_ITEM_FRAME"));

	//设置变量
	m_wLineCount=m_ImageUserFace.GetWidth()/FACE_CX;
	m_wFullCount=m_wLineCount*m_ImageUserFace.GetHeight()/(FACE_CY*2);

	//设置大小
	m_SizeItemFrame.SetSize(m_ImageItemFrame.GetWidth()/3,m_ImageItemFrame.GetHeight());

	return;
}

//析构函数
CFaceItemControl::~CFaceItemControl()
{
	//设置对象
	ASSERT(m_pFaceItemControl==this);
	if (m_pFaceItemControl==this) m_pFaceItemControl=NULL;

	return;
}

//接口查询
VOID * CFaceItemControl::QueryInterface(REFGUID Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IFaceItemControl,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IFaceItemControl,Guid,dwQueryVer);
	return NULL;
}

//头像数目
WORD CFaceItemControl::GetFaceCount()
{
	return m_wFullCount;
}

//获取尺寸
VOID CFaceItemControl::GetFaceItemSize(CSize & SizeFace)
{
	//设置变量
	SizeFace.SetSize(FACE_CX,FACE_CY);

	return;
}

//获取资源
VOID CFaceItemControl::GetAvatarResource(tagAvatarResource & AvatarResource)
{
	//设置变量
	AvatarResource.pszResource=TEXT("USER_FACE_IMAGE");
	AvatarResource.hResInstance=GetModuleHandle(AVATAR_CONTROL_DLL_NAME);

	return;
}

//绘画框架
VOID CFaceItemControl::DrawFaceItemFrame(CDC * pDC, INT nXPos, INT nYPos, BYTE cbFrameKind)
{
	//效验参数
	ASSERT(cbFrameKind<=ITEM_FRAME_HITDOWN);
	if (cbFrameKind>ITEM_FRAME_HITDOWN) return;

	//绘画框架
	INT nXImagePos=m_SizeItemFrame.cx*cbFrameKind;
	m_ImageItemFrame.DrawImage(pDC,nXPos-5,nYPos-5,m_SizeItemFrame.cx,m_SizeItemFrame.cy,nXImagePos,0);

	return;
}

//绘画头像
VOID CFaceItemControl::DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID)
{
	//调整参数
	if (wFaceID>=m_wFullCount) wFaceID=0;

	//绘画头像
	INT nXImagePos=wFaceID%m_wLineCount;
	INT nYImagePos=(wFaceID/m_wLineCount)*2L;
	m_ImageUserFace.DrawImage(pDC,nXPos,nYPos,FACE_CX,FACE_CY,nXImagePos*FACE_CX,nYImagePos*FACE_CY);

	return;
}

//绘画头像
VOID CFaceItemControl::DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, WORD wFaceID)
{
	//调整参数
	if (wFaceID>=m_wFullCount) wFaceID=0;

	//绘画头像
	INT nXImagePos=wFaceID%m_wLineCount;
	INT nYImagePos=(wFaceID/m_wLineCount)*2L;
	m_ImageUserFace.DrawImage(pDC,nXPos,nYPos,FACE_CX,FACE_CY,nXImagePos*FACE_CX,nYImagePos*FACE_CY+FACE_CY);

	return;
}

//绘画头像
VOID CFaceItemControl::DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, DWORD dwCustomFace[FACE_CX*FACE_CY])
{
	//创建位图
	CImage ImageCustomFace;
	ImageCustomFace.Create(FACE_CX,FACE_CY,32);

	//获取数据
	INT nImagePitch=ImageCustomFace.GetPitch();
	LPBYTE cbBitCustomFace=(LPBYTE)ImageCustomFace.GetBits();

	//创建区域
	for (INT nYImagePos=0;nYImagePos<FACE_CY;nYImagePos++)
	{
		for (INT nXImagePos=0;nXImagePos<FACE_CX;nXImagePos++)
		{
			//设置颜色
			DWORD dwImageTarget=nYImagePos*nImagePitch+nXImagePos*4L;
			*(COLORREF *)(cbBitCustomFace+dwImageTarget)=dwCustomFace[nYImagePos*FACE_CX+nXImagePos];
		}
	}

	//绘画界面
	ImageCustomFace.Draw(pDC->m_hDC,nXPos,nYPos);

	return;
}

//绘画头像
VOID CFaceItemControl::DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, DWORD dwCustomFace[FACE_CX*FACE_CY])
{
	//创建位图
	CImage ImageCustomFace;
	ImageCustomFace.Create(FACE_CX,FACE_CY,32);

	//获取数据
	INT nImagePitch=ImageCustomFace.GetPitch();
	LPBYTE cbBitCustomFace=(LPBYTE)ImageCustomFace.GetBits();

	//创建区域
	for (INT nYImagePos=0;nYImagePos<FACE_CY;nYImagePos++)
	{
		for (INT nXImagePos=0;nXImagePos<FACE_CX;nXImagePos++)
		{
			//设置颜色
			DWORD dwImageTarget=nYImagePos*nImagePitch+nXImagePos*4L;
			COLORREF crImageTarget=dwCustomFace[nYImagePos*FACE_CX+nXImagePos];

			//提取颜色
			BYTE cbColorR=GetRValue(crImageTarget);
			BYTE cbColorG=GetGValue(crImageTarget);
			BYTE cbColorB=GetBValue(crImageTarget);
			BYTE cbColorGray=(BYTE)(cbColorR*0.30+cbColorG*0.59+cbColorB*0.11);

			//设置颜色
			*(COLORREF *)(cbBitCustomFace+dwImageTarget)=RGB(cbColorGray,cbColorGray,cbColorGray);
		}
	}

	//绘画界面
	ImageCustomFace.Draw(pDC->m_hDC,nXPos,nYPos);

	return;
}

//绘画头像
VOID CFaceItemControl::DrawFaceNormal(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, WORD wFaceID)
{
	//调整参数
	if (wFaceID>=m_wFullCount) wFaceID=0;

	//绘画头像
	INT nXImagePos=wFaceID%m_wLineCount;
	INT nYImagePos=(wFaceID/m_wLineCount)*2L;
	m_ImageUserFace.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nXImagePos*FACE_CX,nYImagePos*FACE_CY,FACE_CX,FACE_CY);

	return;
}

//绘画头像
VOID CFaceItemControl::DrawFaceOffLine(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight, WORD wFaceID)
{
	//调整参数
	if (wFaceID>=m_wFullCount) wFaceID=0;

	//绘画头像
	INT nXImagePos=wFaceID%m_wLineCount;
	INT nYImagePos=(wFaceID/m_wLineCount)*2L;
	m_ImageUserFace.DrawImage(pDC,nXPos,nYPos,nWidth,nHeight,nXImagePos*FACE_CX,nYImagePos*FACE_CY+FACE_CY,FACE_CX,FACE_CY);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//组件创建函数
DECLARE_CREATE_MODULE(FaceItemControl);

//////////////////////////////////////////////////////////////////////////////////
