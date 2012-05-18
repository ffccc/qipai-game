#include "Stdafx.h"
#include "Resource.h"
#include "GamePlaza.h"
#include "GlobalUnits.h"
#include "TableViewFrame.h"

//////////////////////////////////////////////////////////////////////////////////

//横行格式
#define	NT_LEFT						0x01								//左对齐
#define	NT_RIGHT					0x02								//右对齐

//纵向格式
#define	NT_TOP						0x10								//上对齐
#define	NT_BOTTOM					0x20								//下对齐

//滚动定义
#define WHELL_TIMES					2									//滚轮倍数
#define SCROLL_POS_LINE				10									//滚动像素

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTableViewFrame, CWnd)

	//系统消息
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()

	//自定消息
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_PLATFORM_EVENT, OnMessagePlatformEvent)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableResource::CTableResource()
{
	//属性变量
	m_bShowUser=true;
	m_bShowChair=true;
	m_bShowTable=true;
	m_bShowFocus=true;
	m_bShowEnter=false;
	m_bCustomUser=false;
	m_bShowTableID=true;
	m_bRenderImage=true;

	//数目定义
	m_wTableItemCount=0;
	m_wChairItemCount=0;

	//类型数目
	m_wUserGenreCount=0;
	m_wTableGenreCount=0;
	m_wChairGenreCount=0;

	//颜色属性
	m_crName=RGB(0,0,0);
	m_crMember=RGB(0,0,0);
	m_crMaster=RGB(0,0,0);

	//桌子属性
	m_SizeChair.SetSize(0,0);
	m_SizeTable.SetSize(0,0);
	m_SizeEnter.SetSize(0,0);

	//位置变量
	m_ptLock.SetPoint(0,0);
	m_ptEnter.SetPoint(0,0);
	m_ptTableID.SetPoint(0,0);

	//位置变量
	for (WORD i=0;i<MAX_CHAIR;i++)
	{
		m_ptReadyArray[i].SetPoint(0,0);
		m_rcNameArray[i].SetRect(0,0,0,0);
		m_rcChairArray[i].SetRect(0,0,0,0);
	}

	//辅助变量
	ZeroMemory(m_nDrawStyle,sizeof(m_nDrawStyle));
	ZeroMemory(m_szDirectory,sizeof(m_szDirectory));

	//加载资源
	m_ImageReady.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_READY"));
	m_ImageLocker.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_LOCKER"));
	m_ImageNumberBK.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_NUMBER_BK"));
	m_ImageNumberNB.LoadImage(AfxGetInstanceHandle(),TEXT("TABLE_NUMBER_NB"));

	return;
}

//析构函数
CTableResource::~CTableResource()
{
	//释放资源
	DestroyResource();

	return;
}

//释放资源
VOID CTableResource::DestroyResource()
{
	//桌子属性
	m_SizeChair.SetSize(0,0);
	m_SizeTable.SetSize(0,0);

	//数目定义
	m_wTableItemCount=0;
	m_wChairItemCount=0;

	//输出控制
	ZeroMemory(m_nDrawStyle,sizeof(m_nDrawStyle));

	//位置属性
	ZeroMemory(&m_ptLock,sizeof(m_ptLock));
	ZeroMemory(m_rcNameArray,sizeof(m_rcNameArray));
	ZeroMemory(m_rcChairArray,sizeof(m_rcChairArray));
	ZeroMemory(m_ptReadyArray,sizeof(m_ptReadyArray));

	//颜色属性
	m_crName=RGB(0,0,0);
	m_crMember=RGB(0,0,0);
	m_crMaster=RGB(0,0,0);

	//释放位图
	m_ImageChair.Destroy();
	m_ImageTable.Destroy();
	m_ImageGround.Destroy();

	//释放位图
	m_ImageEnter.DestroyImage();
	m_ImageCustomUser.DestroyImage();

	return;
}

//加载资源
bool CTableResource::LoadResource(LPCTSTR pszResDirectory, WORD wChairCount)
{
	//效验参数
	ASSERT((pszResDirectory!=NULL)&&(wChairCount<=MAX_CHAIR));
	if ((pszResDirectory==NULL)||(wChairCount>MAX_CHAIR)) return false;

	//资源路径
	TCHAR szDirectory[MAX_PATH]=TEXT("");
	CWHService::GetWorkDirectory(szDirectory,CountArray(szDirectory));
	_sntprintf(m_szDirectory,CountArray(m_szDirectory),TEXT("%s\\%s"),szDirectory,pszResDirectory);

	//参数路径
	TCHAR szParameterFile[MAX_PATH]=TEXT("");
	_sntprintf(szParameterFile,CountArray(szParameterFile),TEXT("%s\\%s\\TableResource.INI"),szDirectory,pszResDirectory);

	//颜色变量
	ReadColor(m_crName,TEXT("Color"),TEXT("Color_Name"),szParameterFile);
	ReadColor(m_crMember,TEXT("Color"),TEXT("Color_Member"),szParameterFile);
	ReadColor(m_crMaster,TEXT("Color"),TEXT("Color_Master"),szParameterFile);

	//位置坐标
	ReadPoint(m_ptLock,TEXT("Position"),TEXT("Point_Lock"),szParameterFile);
	ReadPoint(m_ptEnter,TEXT("Position"),TEXT("Point_Enter"),szParameterFile);
	ReadPoint(m_ptTableID,TEXT("Position"),TEXT("Point_TableID"),szParameterFile);

	//显示属性
	m_bShowUser=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowUser"),1L,szParameterFile)==TRUE);
	m_bShowChair=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowChair"),1L,szParameterFile)==TRUE);
	m_bShowTable=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowTable"),1L,szParameterFile)==TRUE);
	m_bShowFocus=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowFocus"),1L,szParameterFile)==TRUE);
	m_bShowEnter=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowEnter"),0L,szParameterFile)==TRUE);
	m_bCustomUser=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("CustomUser"),0L,szParameterFile)==TRUE);
	m_bShowTableID=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("ShowTableID"),1L,szParameterFile)==TRUE);
	m_bRenderImage=(GetPrivateProfileInt(TEXT("Attribute"),TEXT("RenderImage"),1L,szParameterFile)==TRUE);

	//数目属性
	m_wTableItemCount=GetPrivateProfileInt(TEXT("Attribute"),TEXT("TableItemCount"),2L,szParameterFile);
	m_wChairItemCount=GetPrivateProfileInt(TEXT("Attribute"),TEXT("ChairItemCount"),wChairCount,szParameterFile);

	//类型数目
	m_wUserGenreCount=GetPrivateProfileInt(TEXT("Attribute"),TEXT("UserGenreCount"),1L,szParameterFile);
	m_wTableGenreCount=GetPrivateProfileInt(TEXT("Attribute"),TEXT("TableGenreCount"),1L,szParameterFile);
	m_wChairGenreCount=GetPrivateProfileInt(TEXT("Attribute"),TEXT("ChairGenreCount"),1L,szParameterFile);

	//更新资源
	if (UpdateResource(CSkinRenderManager::GetInstance())==false)
	{
		return false;
	}

	//进入尺寸
	if (m_ImageEnter.IsNull()==false)
	{
		m_SizeEnter.SetSize(m_ImageEnter.GetWidth()/4,m_ImageEnter.GetHeight());
	}

	//椅子尺寸
	if (m_ImageChair.IsNull()==false)
	{
		m_SizeChair.SetSize(m_ImageChair.GetWidth()/m_wChairItemCount,m_ImageChair.GetHeight()/m_wChairGenreCount);
	}

	//桌子尺寸
	if (m_ImageTable.IsNull()==false)
	{
		m_SizeTable.SetSize(m_ImageTable.GetWidth()/m_wTableItemCount,m_ImageTable.GetHeight()/m_wTableGenreCount);
	}

	//用户尺寸
	if (m_ImageCustomUser.IsNull()==false)
	{
		m_SizeChair.SetSize(m_ImageCustomUser.GetWidth()/m_wChairItemCount,m_ImageCustomUser.GetHeight()/m_wUserGenreCount);
	}

	//位置坐标
	if (m_bShowTable==true)
	{
		for (WORD i=0;i<wChairCount;i++)
		{
			//变量定义
			TCHAR szSubItem[64]=TEXT("");

			//用户属性
			if (m_bShowUser==true)
			{
				//名字位置
				_sntprintf(szSubItem,CountArray(szSubItem),TEXT("Rect_Name%d"),i+1);
				ReadRect(m_rcNameArray[i],TEXT("Position"),szSubItem,szParameterFile);

				//准备位置
				_sntprintf(szSubItem,CountArray(szSubItem),TEXT("Point_Ready%d"),i+1);
				ReadPoint(m_ptReadyArray[i],TEXT("Position"),szSubItem,szParameterFile);

				//控制输出
				_sntprintf(szSubItem,CountArray(szSubItem),TEXT("DrawStyle%d"),i+1);
				m_nDrawStyle[i]=GetPrivateProfileInt(TEXT("Position"),szSubItem,0L,szParameterFile);

				//椅子位置
				POINT ChairPoint;
				_sntprintf(szSubItem,CountArray(szSubItem),TEXT("Point_Chair%d"),i+1);
				ReadPoint(ChairPoint,TEXT("Position"),szSubItem,szParameterFile);

				//椅子框架
				m_rcChairArray[i].top=ChairPoint.y;
				m_rcChairArray[i].left=ChairPoint.x;
				m_rcChairArray[i].right=m_rcChairArray[i].left+m_SizeChair.cx;
				m_rcChairArray[i].bottom=m_rcChairArray[i].top+m_SizeChair.cy;
			}
		}
	}

	return true;
}

//更新资源
bool CTableResource::UpdateResource(CSkinRenderManager * pSkinRenderManager)
{
	//构造路径
	TCHAR szImageGround[MAX_PATH]=TEXT("");
	_sntprintf(szImageGround,CountArray(szImageGround),TEXT("%s\\Ground.bmp"),m_szDirectory);

	//加载背景
	if (m_ImageGround.IsNull()==false)
	{
		m_ImageGround.Destroy();
	}

	//加载资源
	if (FAILED(m_ImageGround.Load(szImageGround)))
	{
		m_ImageGround.LoadFromResource(AfxGetInstanceHandle(),IDB_TABLE_GROUND);
	}

	//渲染资源
	if (m_bRenderImage==true)
	{
		pSkinRenderManager->RenderImage(m_ImageGround);
	}

	//进入位图
	if ((m_bShowEnter==true)&&(m_ImageEnter.IsNull()==true))
	{
		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\Enter.png"),m_szDirectory);

		//加载资源
		if (m_ImageEnter.LoadImage(szImagePath)==false) return false;
	}

	//用户位图
	if ((m_bCustomUser==true)&&(m_ImageCustomUser.IsNull()==true))
	{
		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\CustomUser.png"),m_szDirectory);

		//加载资源
		if (m_ImageCustomUser.LoadImage(szImagePath)==false) return false;
	}

	//构造桌子
	if (m_bShowTable==true)
	{
		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\Table.png"),m_szDirectory);

		//加载资源
		CPngImage ImageTable;
		if (ImageTable.LoadImage(szImagePath)==false) return false;

		//创建资源
		if (m_ImageTable.IsNull()==true)
		{
			m_ImageTable.Create(ImageTable.GetWidth(),ImageTable.GetHeight(),32);
		}

		//填充背景
		CImageDC TableDC(m_ImageTable);
		CDC * pTableDC=CDC::FromHandle(TableDC);
		m_ImageGround.DrawImageTile(pTableDC,0,0,ImageTable.GetWidth(),ImageTable.GetHeight());

		//构造桌子
		ImageTable.DrawImage(pTableDC,0,0,ImageTable.GetWidth(),ImageTable.GetHeight(),0,0);
	}

	//创建椅子
	if ((m_bShowChair==true)&&(m_bShowTable==true))
	{
		//构造路径
		TCHAR szImagePath[MAX_PATH]=TEXT("");
		_sntprintf(szImagePath,CountArray(szImagePath),TEXT("%s\\Chair.png"),m_szDirectory);

		//加载资源
		CPngImage ImageChair;
		if (ImageChair.LoadImage(szImagePath)==false) return false;

		//创建资源
		if (m_ImageChair.IsNull()==true)
		{
			m_ImageChair.Create(ImageChair.GetWidth(),ImageChair.GetHeight(),32);
		}

		//填充背景
		CImageDC ChairDC(m_ImageChair);
		CDC * pChairDC=CDC::FromHandle(ChairDC);
		m_ImageGround.DrawImageTile(pChairDC,0,0,ImageChair.GetWidth(),ImageChair.GetHeight());

		//构造椅子
		ImageChair.DrawImage(pChairDC,0,0,ImageChair.GetWidth(),ImageChair.GetHeight(),0,0);
	}

	return true;
}

//读取数值
LONG CTableResource::ReadStringValue(LPCTSTR & pszSring)
{
	//效验参数
	ASSERT((pszSring!=NULL)&&(pszSring[0]!=0));
	if ((pszSring==NULL)||(pszSring[0]==0)) return 0L;

	//寻找开始
	while (((pszSring[0]>0)&&(pszSring[0]<TEXT('0')))||(pszSring[0]>TEXT('9'))) pszSring++;

	//读取数值
	LONG lValue=0L;
	while ((pszSring[0]>=TEXT('0'))&&(pszSring[0]<=TEXT('9')))
	{
		lValue=lValue*10L+pszSring[0]-TEXT('0');
		++pszSring;
	}

	return lValue;
}

//读取矩形
bool CTableResource::ReadRect(RECT & ValueRect, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile)
{
	//设置变量
	TCHAR szReadData[64]=TEXT("");
	ZeroMemory(&ValueRect,sizeof(ValueRect));

	//读取字符
	GetPrivateProfileString(pszItem,pszSubItem,TEXT(""),szReadData,CountArray(szReadData),pszIniFile);
	if (szReadData[0]!=0)
	{
		//读取变量
		LPCTSTR pszString=szReadData;
		ValueRect.left=ReadStringValue(pszString);
		ValueRect.top=ReadStringValue(pszString);
		ValueRect.right=ReadStringValue(pszString);
		ValueRect.bottom=ReadStringValue(pszString);

		return true;
	}

	return false;
}

//读取坐标
bool CTableResource::ReadPoint(POINT & ValuePoint, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile)
{
	//设置变量
	TCHAR szReadData[64]=TEXT("");
	ZeroMemory(&ValuePoint,sizeof(ValuePoint));

	//读取字符
	GetPrivateProfileString(pszItem,pszSubItem,TEXT(""),szReadData,CountArray(szReadData),pszIniFile);
	if (szReadData[0]!=0)
	{
		//读取变量
		LPCTSTR pszString=szReadData;
		ValuePoint.x=ReadStringValue(pszString);
		ValuePoint.y=ReadStringValue(pszString);

		return true;
	}

	return false;
}

//读取颜色
bool CTableResource::ReadColor(COLORREF & ValueColor, LPCTSTR pszItem, LPCTSTR pszSubItem, LPCTSTR pszIniFile)
{
	//设置变量
	TCHAR szReadData[64]=TEXT("");
	ZeroMemory(&ValueColor,sizeof(ValueColor));

	//读取字符
	GetPrivateProfileString(pszItem,pszSubItem,TEXT(""),szReadData,CountArray(szReadData),pszIniFile);
	if (szReadData[0]!=0)
	{
		//读取变量
		LPCTSTR pszString=szReadData;
		ValueColor=RGB(ReadStringValue(pszString),ReadStringValue(pszString),ReadStringValue(pszString));

		return true;
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableView::CTableView()
{
	//组件接口
	m_pITableViewFrame=NULL;

	//状态变量
	m_bMouseDown=false;
	m_wHoverChairID=INVALID_CHAIR;
	ZeroMemory(&m_TableAttribute,sizeof(m_TableAttribute));

	return;
}

//析构函数
CTableView::~CTableView()
{
}

//接口查询
VOID * CTableView::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableView,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableView,Guid,dwQueryVer);
	return NULL;
}

//空椅子数
WORD CTableView::GetNullChairCount(WORD & wNullChairID)
{
	//设置变量
	wNullChairID=INVALID_CHAIR;

	//寻找位置
	WORD wNullCount=0;
	for (WORD i=0;i<m_TableAttribute.wChairCount;i++)
	{
		if (m_TableAttribute.pIClientUserItem[i]==NULL)
		{
			//设置数目
			wNullCount++;

			//设置结果
			if (wNullChairID==INVALID_CHAIR) wNullChairID=i;
		}
	}

	return wNullCount;
}

//配置函数
VOID CTableView::InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableViewFrame)
{
	//设置属性
	m_TableAttribute.wTableID=wTableID;
	m_TableAttribute.wChairCount=wChairCount;

	//设置接口
	m_pITableViewFrame=QUERY_OBJECT_PTR_INTERFACE(pITableViewFrame,ITableViewFrame);

	return;
}

//获取用户
IClientUserItem * CTableView::GetClientUserItem(WORD wChairID)
{
	//效验参数
	ASSERT(wChairID<m_TableAttribute.wChairCount);
	if (wChairID>=m_TableAttribute.wChairCount) return NULL;

	//获取用户
	return m_TableAttribute.pIClientUserItem[wChairID];
}

//设置信息
bool CTableView::SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem)
{
	//效验参数
	ASSERT(wChairID<m_TableAttribute.wChairCount);
	if (wChairID>=m_TableAttribute.wChairCount) return false;

	//设置用户
	m_TableAttribute.pIClientUserItem[wChairID]=pIClientUserItem;

	//更新界面
	m_pITableViewFrame->UpdateTableView(m_TableAttribute.wTableID);

	return true;
}

//鼠标按下
VOID CTableView::SetMouseDown(bool bMouseDown)
{
	//设置标志
	if (m_bMouseDown!=bMouseDown)
	{
		//设置变量
		m_bMouseDown=bMouseDown; 

		//更新界面
		m_pITableViewFrame->UpdateTableView(m_TableAttribute.wTableID);
	}

	return;
}

//焦点框架
VOID CTableView::SetFocusFrame(bool bFocusFrame)
{
	//设置标志
	if (m_TableAttribute.bFocusFrame!=bFocusFrame)
	{
		//设置变量
		m_TableAttribute.bFocusFrame=bFocusFrame;

		//更新界面
		m_pITableViewFrame->UpdateTableView(m_TableAttribute.wTableID);
	}

	return;
}

//盘旋位置
VOID CTableView::SetHoverStatus(WORD wHoverChairID)
{
	//设置标志
	if (m_wHoverChairID!=wHoverChairID)
	{
		//设置变量
		m_wHoverChairID=wHoverChairID;

		//更新界面
		m_pITableViewFrame->UpdateTableView(m_TableAttribute.wTableID);
	}

	return;
}

//桌子状态 
VOID CTableView::SetTableStatus(bool bPlaying, bool bLocker)
{
	//设置标志
	if ((m_TableAttribute.bLocker!=bLocker)||(m_TableAttribute.bPlaying!=bPlaying))
	{
		//设置变量
		m_TableAttribute.bLocker=bLocker; 
		m_TableAttribute.bPlaying=bPlaying;

		//更新界面
		m_pITableViewFrame->UpdateTableView(m_TableAttribute.wTableID);
	}

	return;
}

//绘画函数
VOID CTableView::DrawTableView(CDC * pDC, DWORD dwServerRule, CTableResource * pTableResource)
{
	//椅子大小
	CSize SizeChair=pTableResource->m_SizeChair;
	CSize SizeTable=pTableResource->m_SizeTable;

	//变量定义
	CGlobalUserInfo * pGlobalUserInfo=CGlobalUserInfo::GetInstance();
	CParameterGlobal * pParameterGlobal=CParameterGlobal::GetInstance();
	tagGlobalUserData * pGlobalUserData=pGlobalUserInfo->GetGlobalUserData();

	//变量定义
	bool bMySelfTable=false;
	bool bHideUserInfo=CServerRule::IsAvertCheatMode(dwServerRule);

	//绘画桌子
	if ((bHideUserInfo==false)||(m_TableAttribute.wTableID==0))
	{
		INT nXPosTable=(m_TableAttribute.bPlaying==true)?SizeTable.cx:0;
		pTableResource->m_ImageTable.BitBlt(pDC->m_hDC,0,0,SizeTable.cx,SizeTable.cy,nXPosTable,0);
	}
	else
	{
		INT nXPosTable=SizeTable.cx;
		pTableResource->m_ImageTable.BitBlt(pDC->m_hDC,0,0,SizeTable.cx,SizeTable.cy,nXPosTable,0);
	}

	//桌面信息
	for (WORD i=0;i<m_TableAttribute.wChairCount;i++)
	{
		//变量定义
		CRect rcChair=pTableResource->m_rcChairArray[i];

		//绘画进入
		if ((pTableResource->m_bShowEnter==true)&&(pTableResource->m_ImageEnter.IsNull()==false))
		{
			//计算位置
			INT nImageIndex=0;
			if (m_TableAttribute.bPlaying==true) nImageIndex=1;
			if ((m_wHoverChairID==INDEX_ENTER_CHAIR)&&(m_TableAttribute.bPlaying==false)&&(m_bMouseDown==true)) nImageIndex=2;
			if ((m_wHoverChairID==INDEX_ENTER_CHAIR)&&(m_TableAttribute.bPlaying==false)&&(m_bMouseDown==false)) nImageIndex=3;

			//绘画标志
			CSize SizeEnter=pTableResource->m_SizeEnter;
			pTableResource->m_ImageEnter.DrawImage(pDC,pTableResource->m_ptEnter.x,pTableResource->m_ptEnter.y,SizeEnter.cx,SizeEnter.cy,
				SizeEnter.cx*nImageIndex,0);
		}

		//绘画椅子
		if ((bHideUserInfo==true)&&(pTableResource->m_bShowChair==true)&&(m_TableAttribute.wTableID==0))
		{
			ASSERT(pTableResource->m_ImageChair.IsNull()==false);
			pTableResource->m_ImageChair.BitBlt(pDC->m_hDC,rcChair.left,rcChair.top,SizeChair.cx,SizeChair.cy,SizeChair.cx*i,0);
		}

		//绘画椅子
		if ((bHideUserInfo==false)&&(pTableResource->m_bShowChair==true)&&(m_TableAttribute.pIClientUserItem[i]==NULL))
		{
			ASSERT(pTableResource->m_ImageChair.IsNull()==false);
			pTableResource->m_ImageChair.BitBlt(pDC->m_hDC,rcChair.left,rcChair.top,SizeChair.cx,SizeChair.cy,SizeChair.cx*i,0);
		}

		//绘画用户
		if (pTableResource->m_bShowUser==true)
		{
			//绘画用户
			if ((bHideUserInfo==false)&&(m_TableAttribute.pIClientUserItem[i]!=NULL))
			{
				//获取用户
				IClientUserItem * pIClientUserItem=m_TableAttribute.pIClientUserItem[i];
				tagCustomFaceInfo * pCustomFaceInfo=pIClientUserItem->GetCustomFaceInfo();

				//自己判断
				if (m_TableAttribute.pIClientUserItem[i]->GetUserID()==pGlobalUserData->dwUserID)
				{
					bMySelfTable=true;
				}

				//绘画头像
				if (pTableResource->m_bCustomUser==false)
				{
					//计算位置
					INT nXExcursion=(rcChair.Width()-FACE_CX)/2;
					INT nYExcursion=(rcChair.Height()-FACE_CY)/2;

					//绘画头像
					if ((pCustomFaceInfo->dwDataSize!=0L)&&(pIClientUserItem->GetCustomID()!=0L))
					{
						IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
						pIFaceItemControl->DrawFaceNormal(pDC,rcChair.left+nXExcursion,rcChair.top+nYExcursion,pCustomFaceInfo->dwCustomFace);
					}
					else
					{
						IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
						pIFaceItemControl->DrawFaceNormal(pDC,rcChair.left+nXExcursion,rcChair.top+nYExcursion,pIClientUserItem->GetFaceID());
					}
				}
				else
				{
					//自定头像
					INT nXImagePos=i*rcChair.Width();
					INT nYImagePos=(m_TableAttribute.pIClientUserItem[i]->GetGender()==GENDER_FEMALE)?rcChair.Height():0;
					pTableResource->m_ImageCustomUser.DrawImage(pDC,rcChair.left,rcChair.top,rcChair.Width(),rcChair.Height(),nXImagePos,nYImagePos);
				}

				//同位判断
				WORD wTableID=pIClientUserItem->GetTableID();
				WORD wChairID=pIClientUserItem->GetChairID();
				bool bSameStation=(wTableID==m_TableAttribute.wTableID)&&(wChairID==i);

				//同意标志
				if ((bSameStation==true)&&(m_TableAttribute.bPlaying==false)&&(pIClientUserItem->GetUserStatus()==US_READY))
				{
					INT nXPos=pTableResource->m_ptReadyArray[i].x;
					INT nYPos=pTableResource->m_ptReadyArray[i].y;
					pTableResource->m_ImageReady.DrawImage(pDC,nXPos,nYPos);
				}

				//用户名字
				DrawTableName(pDC,i,pIClientUserItem->GetNickName(),pTableResource);
			}

			//隐藏模式
			if ((bHideUserInfo==true)&&(pTableResource->m_bShowUser==true))
			{
				//用户头像
				if (m_TableAttribute.wTableID>0)
				{
					//计算位置
					INT nXExcursion=(rcChair.Width()-FACE_CX)/2;
					INT nYExcursion=(rcChair.Height()-FACE_CY)/2;

					//绘画头像
					IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
					pIFaceItemControl->DrawFaceNormal(pDC,rcChair.left+nXExcursion,rcChair.top+nYExcursion,((m_TableAttribute.wTableID*m_TableAttribute.wChairCount)+i)%pIFaceItemControl->GetFaceCount());
				}

				//用户名字
				WORD wTableID=m_TableAttribute.wTableID;
				DrawTableName(pDC,i,(wTableID==0)?TEXT("点击加入"):TEXT("分配位置"),pTableResource);
			}

			//绘画边框
			if ((bHideUserInfo==false)||(m_TableAttribute.wTableID==0))
			{
				//自定边框
				if ((pTableResource->m_bCustomUser==true)&&(m_wHoverChairID==i))
				{
					pDC->Draw3dRect(rcChair.left,rcChair.top,rcChair.Width(),rcChair.Height(),RGB(125,125,125),RGB(125,125,125));
				}

				//系统边框
				if ((pTableResource->m_bCustomUser==false)&&((m_wHoverChairID==i)||(m_TableAttribute.pIClientUserItem[i]!=NULL)))
				{
					//计算位置
					INT nXExcursion=(rcChair.Width()-FACE_CX)/2;
					INT nYExcursion=(rcChair.Height()-FACE_CY)/2;

					//计算类型
					BYTE cbFrameKind=ITEM_FRAME_NORMAL;
					if ((m_wHoverChairID==i)&&(m_bMouseDown==false)) cbFrameKind=ITEM_FRAME_HOVER;
					if ((m_wHoverChairID==i)&&(m_bMouseDown==true)) cbFrameKind=ITEM_FRAME_HITDOWN;

					//绘画框架
					IFaceItemControl * pIFaceItemControl=CFaceItemControl::GetInstance();
					pIFaceItemControl->DrawFaceItemFrame(pDC,rcChair.left+nXExcursion,rcChair.top+nYExcursion,cbFrameKind);
				}
			}
		}
	}

	//绘画锁图
	if (m_TableAttribute.bLocker==true)
	{
		INT nXPos=pTableResource->m_ptLock.x;
		INT nYPos=pTableResource->m_ptLock.y;
		pTableResource->m_ImageLocker.DrawImage(pDC,nXPos,nYPos);
	}

	//桌子号码
	if (pTableResource->m_bShowTableID==true)
	{
		//获取大小
		CSize SizeNumberBK;
		CSize SizeNumberNB;
		SizeNumberBK.SetSize(pTableResource->m_ImageNumberBK.GetWidth(),pTableResource->m_ImageNumberBK.GetHeight());
		SizeNumberNB.SetSize(pTableResource->m_ImageNumberNB.GetWidth()/10L,pTableResource->m_ImageNumberNB.GetHeight());

		//变量定义
		WORD wItemCount=0;
		WORD wNumberTemp=m_TableAttribute.wTableID+1;
		WORD wTableNumber=m_TableAttribute.wTableID+1;

		//计算数目
		do
		{
			wItemCount++;
			wNumberTemp/=10;
		} while (wNumberTemp>0);

		//绘画背景
		INT nXNumberBK=pTableResource->m_ptTableID.x-SizeNumberBK.cx/2;
		INT nYNumberBK=pTableResource->m_ptTableID.y-SizeNumberBK.cy/2;
		pTableResource->m_ImageNumberBK.DrawImage(pDC,nXNumberBK,nYNumberBK);

		//位置定义
		INT nYNumberNB=pTableResource->m_ptTableID.y-SizeNumberNB.cy/2;
		INT nXNumberNB=pTableResource->m_ptTableID.x+(wItemCount*SizeNumberNB.cx)/2-SizeNumberNB.cx;

		//绘画桌号
		for (INT i=0;i<wItemCount;i++)
		{
			//绘画号码
			WORD wNumber=wTableNumber%10;
			pTableResource->m_ImageNumberNB.DrawImage(pDC,nXNumberNB,nYNumberNB,SizeNumberNB.cx,SizeNumberNB.cy,wNumber*SizeNumberNB.cx,0);

			//设置变量
			wTableNumber/=10;
			nXNumberNB-=SizeNumberNB.cx;
		};
	}

	//绘画焦点
	if ((pTableResource->m_bShowFocus==true)&&(pParameterGlobal->m_bSalienceTable==true)&&(bMySelfTable==true))
	{
		pDC->Draw3dRect(0,0,SizeTable.cx,SizeTable.cy,RGB(125,125,125),RGB(125,125,125));
	}

	return;
}

//绘画名字
VOID CTableView::DrawTableName(CDC * pDC, WORD wChairID, LPCTSTR pszNickName, CTableResource * pTableResource)
{
	//用户名字
	CRect rcName=pTableResource->m_rcNameArray[wChairID];

	//绘图计算
	CRect rcDrawRect=rcName;
	INT nNameLength=lstrlen(pszNickName);
	INT nDrawStyte=pTableResource->m_nDrawStyle[wChairID];
	INT nDrawFormat=DT_WORDBREAK|DT_EDITCONTROL|DT_END_ELLIPSIS;
	INT nNameHeight=pDC->DrawText(pszNickName,nNameLength,&rcDrawRect,nDrawFormat|DT_CALCRECT);

	//横向位置
	if (nNameHeight<=12)
	{
		if (nDrawStyte&NT_LEFT) nDrawFormat|=DT_LEFT;
		else if (nDrawStyte&NT_RIGHT) nDrawFormat|=DT_RIGHT;
		else nDrawFormat|=(DT_CENTER|DT_SINGLELINE);
	}
	else nDrawFormat|=DT_LEFT;

	//竖向位置
	INT nYPos=0;
	if (nDrawStyte&NT_BOTTOM) nYPos=rcName.Height()-nNameHeight;
	else if (nDrawStyte&NT_TOP) nYPos=0;
	else nYPos=(rcName.Height()-nNameHeight)/2;

	//构造位置
	rcDrawRect=rcName;
	rcDrawRect.left=rcName.left;
	rcDrawRect.right=rcName.right;
	rcDrawRect.top=__max(rcName.top,rcName.top+nYPos);
	rcDrawRect.bottom=__min(rcName.bottom,rcDrawRect.top+nNameHeight);

	//用户名字
	pDC->SetTextColor(pTableResource->m_crName);
	pDC->DrawText(pszNickName,nNameLength,&rcDrawRect,nDrawFormat);

	return;
}

//////////////////////////////////////////////////////////////////////////////////

//构造函数
CTableViewFrame::CTableViewFrame()
{
	//状态变量
	m_bHovering=false;
	m_bLMouseDown=false;
	m_bRMouseDown=false;

	//属性变量
	m_wTableCount=0;
	m_wChairCount=0;
	m_dwServerRule=0;

	//滚动信息
	m_nXExcursion=0;
	m_nYExcursion=0;
	m_nXTableCount=0;
	m_nYTableCount=0;

	//滚动信息
	m_nScrollPos=0;
	m_nScrollPosMax=0;	
	m_nVisibleLineCount=0;

	//索引变量
	m_wDownChairID=INVALID_CHAIR;
	m_wDownTableID=INVALID_TABLE;

	//设置指针
	m_pITableViewFrameSink=NULL;

	return;
}

//析构函数
CTableViewFrame::~CTableViewFrame()
{
	//删除桌子
	for (INT_PTR i=0;i<m_TableViewArray.GetCount();i++)
	{
		CTableView * pTableView=m_TableViewArray[i];
		if (pTableView!=NULL) SafeDelete(pTableView);
	}

	//删除数组
	m_TableViewArray.RemoveAll();

	return;
}

//接口查询
VOID * CTableViewFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableViewFrame,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableViewFrame,Guid,dwQueryVer);
	return NULL;
}

//创建函数
bool CTableViewFrame::CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx)
{
	//设置变量
	m_bHovering=false;
	m_bLMouseDown=false;
	m_bRMouseDown=false;
	m_wDownChairID=INVALID_CHAIR;
	m_wDownTableID=INVALID_TABLE;

	//设置接口
	ASSERT(QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableViewFrameSink)!=NULL);
	m_pITableViewFrameSink=QUERY_OBJECT_PTR_INTERFACE(pIUnknownEx,ITableViewFrameSink);

	//创建窗口
	CRect rcCreate(0,0,0,0);
	Create(NULL,NULL,WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,rcCreate,pParentWnd,uWndID);

	return true;
}

//配置函数
bool CTableViewFrame::ConfigTableFrame(WORD wTableCount, WORD wChairCount, DWORD dwServerRule, LPCTSTR pszResDirectory)
{
	//效验参数
	ASSERT(wChairCount<=MAX_CHAIR);
	ASSERT((pszResDirectory!=NULL)&&(pszResDirectory[0]!=0));

	//设置滚动
	m_SkinScrollBar.InitScroolBar(this);

	//加载资源
	if (m_TableResource.LoadResource(pszResDirectory,wChairCount)==false)
	{
		return false;
	}

	//设置变量
	m_wTableCount=wTableCount;
	m_wChairCount=wChairCount;
	m_dwServerRule=dwServerRule;
	m_TableViewArray.SetSize(m_wTableCount);
	ZeroMemory(m_TableViewArray.GetData(),m_wTableCount*sizeof(CTableView *));

	//创建桌子
	for (WORD i=0;i<m_wTableCount;i++)
	{
		m_TableViewArray[i]=new CTableView;
		m_TableViewArray[i]->InitTableView(i,wChairCount,this);
	}

	//调整位置
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyControl(rcClient.Width(),rcClient.Height());

	//注册事件
	CPlatformEvent * pPlatformEvent=CPlatformEvent::GetInstance();
	if (pPlatformEvent!=NULL) pPlatformEvent->RegisterEventWnd(m_hWnd);

	return true;
}

//获取用户
IClientUserItem * CTableViewFrame::GetClientUserItem(WORD wTableID, WORD wChairID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取用户
	if (pITableView!=NULL)
	{
		return pITableView->GetClientUserItem(wChairID);
	}

	return NULL;
}

//设置信息
bool CTableViewFrame::SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem)
{
	ITableView * pITableView=GetTableViewItem(wTableID);
	if (pITableView!=NULL) pITableView->SetClientUserItem(wChairID,pIClientUserItem);
	return true;
}

//游戏标志
bool CTableViewFrame::GetPlayFlag(WORD wTableID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取标志
	if (pITableView!=NULL)
	{
		return pITableView->GetPlayFlag();
	}

	return false;
}

//密码标志
bool CTableViewFrame::GetLockerFlag(WORD wTableID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取标志
	if (pITableView!=NULL)
	{
		return pITableView->GetLockerFlag();
	}

	return false;
}

//焦点框架
VOID CTableViewFrame::SetFocusFrame(WORD wTableID, bool bFocusFrame)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//设置标志
	if (pITableView!=NULL) pITableView->SetFocusFrame(bFocusFrame);

	return;
}

//桌子状态 
VOID CTableViewFrame::SetTableStatus(WORD wTableID, bool bPlaying, bool bLocker)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//设置标志
	if (pITableView!=NULL) pITableView->SetTableStatus(bPlaying,bLocker);

	return;
}

//桌子可视
bool CTableViewFrame::VisibleTable(WORD wTableID)
{
	//效验参数
	ASSERT(wTableID<m_wTableCount);
	if (wTableID>=m_wTableCount) return false;

	//计算位置
	INT nLastPos=m_nScrollPos;
	INT nXPos=(wTableID%m_nXTableCount)*m_TableResource.m_SizeTable.cx+m_nXExcursion;
	INT nYPos=(wTableID/m_nXTableCount)*m_TableResource.m_SizeTable.cy-m_nScrollPos;

	//构造位置
	CRect rcTable,rcClient;
	GetClientRect(&rcClient);
	rcTable.SetRect(nXPos,nYPos,nXPos+m_TableResource.m_SizeTable.cx,nYPos+m_TableResource.m_SizeTable.cy);

	//位置调整
	if (rcTable.top<0L) m_nScrollPos+=rcTable.top;
	if ((rcTable.top>0)&&(rcTable.bottom>rcClient.bottom)) m_nScrollPos+=rcTable.bottom-rcClient.bottom;
	m_nScrollPos=__max(0,__min(m_nScrollPos,m_nScrollPosMax-m_nVisibleLineCount*m_TableResource.m_SizeTable.cy));

	//滚动视图
	if (nLastPos!=m_nScrollPos)
	{
		SetScrollPos(SB_VERT,m_nScrollPos);
		ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);
	}

	return true;
}

//闪动桌子
bool CTableViewFrame::FlashGameTable(WORD wTableID)
{
	//获取桌子
	ITableView * pITableView=GetTableViewItem(wTableID);

	//错误判断
	if (pITableView==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	INT nXItem=wTableID%m_nXTableCount;
	INT nYItem=wTableID/m_nXTableCount;

	//计算位置
	CRect rcTable;
	rcTable.left=nXItem*m_TableResource.m_SizeTable.cx+m_nXExcursion;
	rcTable.top=nYItem*m_TableResource.m_SizeTable.cy-m_nScrollPos;
	rcTable.right=(nXItem+1)*m_TableResource.m_SizeTable.cx+m_nXExcursion;
	rcTable.bottom=(nYItem+1)*m_TableResource.m_SizeTable.cy+m_nYExcursion-m_nScrollPos;
	
	//定义参数
	INT nFlashTimes=20,nStepWidth=4;
	INT nExpandPos=nFlashTimes*nStepWidth;
	rcTable.InflateRect(nExpandPos,nExpandPos);

	//更新窗口
	UpdateWindow();

	//循环绘画
	CWindowDC WindowDC(this);
	CPen LinePen(PS_SOLID,0,GetSysColor(COLOR_BTNHILIGHT));

	//设置环境
	WindowDC.SetROP2(R2_XORPEN);
	WindowDC.SelectObject(&LinePen);

	//循环绘画
	for (INT nTimes=nFlashTimes;nTimes>=0;nTimes--)
	{
		//绘画线条
		WindowDC.MoveTo(rcTable.left,rcTable.top);
		WindowDC.LineTo(rcTable.right,rcTable.top);
		WindowDC.LineTo(rcTable.right,rcTable.bottom);
		WindowDC.LineTo(rcTable.left,rcTable.bottom);
		WindowDC.LineTo(rcTable.left,rcTable.top);

		//绘画暂停
		Sleep((nFlashTimes-nTimes)*2);

		//绘画线条
		WindowDC.MoveTo(rcTable.left,rcTable.top);
		WindowDC.LineTo(rcTable.right,rcTable.top);
		WindowDC.LineTo(rcTable.right,rcTable.bottom);
		WindowDC.LineTo(rcTable.left,rcTable.bottom);
		WindowDC.LineTo(rcTable.left,rcTable.top);

		//设置矩形
		rcTable.DeflateRect(nStepWidth,nStepWidth);
	}

	//释放资源
	WindowDC.DeleteDC();
	LinePen.DeleteObject();

	return true;
}

//闪动椅子
bool CTableViewFrame::FlashGameChair(WORD wTableID, WORD wChairID)
{
	//获取桌子
	ITableView * pITableView=GetTableViewItem(wTableID);

	//错误判断
	if (pITableView==NULL)
	{
		ASSERT(FALSE);
		return false;
	}

	//变量定义
	INT nXItem=wTableID%m_nXTableCount;
	INT nYItem=wTableID/m_nXTableCount;
	CRect rcChair=m_TableResource.m_rcChairArray[wChairID];

	//获取位置
	INT nXTablePos=nXItem*m_TableResource.m_SizeTable.cx;
	INT nYTablePos=nYItem*m_TableResource.m_SizeTable.cy;
	rcChair.OffsetRect(nXTablePos+m_nXExcursion,nYTablePos+m_nYExcursion-m_nScrollPos);

	//定义参数
	INT nFlashTimes=20,nStepWidth=4;
	INT nExpandPos=nFlashTimes*nStepWidth;
	rcChair.InflateRect(nExpandPos,nExpandPos);

	//更新窗口
	UpdateWindow();

	//循环绘画
	CClientDC ClientDC(this);
	CPen LinePen(PS_SOLID,0,RGB(255,255,255));

	//设置环境
	ClientDC.SetROP2(R2_XORPEN);
	CPen * pOldPen=ClientDC.SelectObject(&LinePen);

	//循环绘画
	for (INT nTimes=nFlashTimes;nTimes>=0;nTimes--)
	{
		//绘画线条
		ClientDC.MoveTo(rcChair.left,rcChair.top);
		ClientDC.LineTo(rcChair.right,rcChair.top);
		ClientDC.LineTo(rcChair.right,rcChair.bottom);
		ClientDC.LineTo(rcChair.left,rcChair.bottom);
		ClientDC.LineTo(rcChair.left,rcChair.top);

		//绘画暂停
		Sleep((nFlashTimes-nTimes)*2);

		//绘画线条
		ClientDC.MoveTo(rcChair.left,rcChair.top);
		ClientDC.LineTo(rcChair.right,rcChair.top);
		ClientDC.LineTo(rcChair.right,rcChair.bottom);
		ClientDC.LineTo(rcChair.left,rcChair.bottom);
		ClientDC.LineTo(rcChair.left,rcChair.top);

		//设置矩形
		rcChair.DeflateRect(nStepWidth,nStepWidth);
	}

	//释放资源
	ClientDC.SelectObject(pOldPen);

	return true;
}

//更新桌子
bool CTableViewFrame::UpdateTableView(WORD wTableID)
{
	//获取桌子
	ITableView * pITableView=GetTableViewItem(wTableID);
	if ((pITableView==NULL)||(m_nXTableCount==0)) return false;

	//变量定义
	INT nXItem=wTableID%m_nXTableCount;
	INT nYItem=wTableID/m_nXTableCount;

	//更新桌子
	CRect rcTable;
	rcTable.left=nXItem*m_TableResource.m_SizeTable.cx+m_nXExcursion;
	rcTable.right=(nXItem+1)*m_TableResource.m_SizeTable.cx+m_nXExcursion;
	rcTable.top=nYItem*m_TableResource.m_SizeTable.cy+m_nYExcursion-m_nScrollPos;
	rcTable.bottom=(nYItem+1)*m_TableResource.m_SizeTable.cy+m_nYExcursion-m_nScrollPos;

	//更新桌子
	RedrawWindow(&rcTable,NULL,RDW_INVALIDATE|RDW_ERASE|RDW_ERASENOW|RDW_UPDATENOW);

	return true;
}

//获取桌子
ITableView * CTableViewFrame::GetTableViewItem(WORD wTableID)
{
	//获取桌子
	if (wTableID!=INVALID_TABLE)
	{
		//效验参数
		ASSERT(wTableID<m_TableViewArray.GetCount());
		if (wTableID>=m_TableViewArray.GetCount()) return NULL;

		//获取桌子
		ITableView * pITableView=m_TableViewArray[wTableID];

		return pITableView;
	}

	return NULL;
}

//空椅子数
WORD CTableViewFrame::GetNullChairCount(WORD wTableID, WORD & wNullChairID)
{
	//获取桌子
	ASSERT(GetTableViewItem(wTableID)!=NULL);
	ITableView * pITableView=GetTableViewItem(wTableID);

	//获取状态
	if (pITableView!=NULL)
	{
		return pITableView->GetNullChairCount(wNullChairID);
	}

	return 0;
}

//重画消息
VOID CTableViewFrame::OnPaint()
{
	//创建 DC
	CPaintDC dc(this);

	//获取区域
	CRect rcClip;
	CRect rcClient;
	dc.GetClipBox(&rcClip);
	GetClientRect(&rcClient);

	//绘画桌子
	if ((m_TableViewArray.GetCount()>0L)&&(m_TableResource.m_bShowTable==true))
	{
		//定义变量
		CSize SizeTable=m_TableResource.m_SizeTable;

		//建立缓冲
		CImage ImageBuffer;
		ImageBuffer.Create(SizeTable.cx,SizeTable.cy,32);

		//创建 DC
		CImageDC BufferDC(ImageBuffer);
		CDC * pDCBuffer=CDC::FromHandle(BufferDC);

		//设置环境
		pDCBuffer->SetBkMode(TRANSPARENT);
		pDCBuffer->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());

		//绘画准备
		INT nXStartItem=(rcClip.left-m_nXExcursion)/SizeTable.cx;
		INT nYStartItem=(m_nScrollPos-m_nYExcursion+rcClip.top)/SizeTable.cy;
		INT nXConcludeItem=(rcClip.right-m_nXExcursion+SizeTable.cx-1)/SizeTable.cx;
		INT nYConcludeItem=(m_nScrollPos-m_nYExcursion+rcClip.bottom+SizeTable.cy-1)/SizeTable.cy;

		//绘画桌子
		for (INT nYIndex=nYStartItem;nYIndex<nYConcludeItem;nYIndex++)
		{
			for (INT nXIndex=nXStartItem;nXIndex<nXConcludeItem;nXIndex++)
			{
				//位置定义
				INT nIndex=nYIndex*m_nXTableCount+nXIndex;
				INT nXTablePos=nXIndex*SizeTable.cx+m_nXExcursion;
				INT nYTablePos=nYIndex*SizeTable.cy+m_nYExcursion-m_nScrollPos;

				//绘画桌子
				if ((nIndex>=0)&&(nXIndex<m_nXTableCount)&&(nIndex<m_wTableCount))
				{
					//绘画桌子
					m_TableViewArray[nIndex]->DrawTableView(pDCBuffer,m_dwServerRule,&m_TableResource);

					//绘画界面
					dc.BitBlt(nXTablePos,nYTablePos,SizeTable.cx,SizeTable.cy,pDCBuffer,0,0,SRCCOPY);
				}
				else
				{
					//绘画空隙
					m_TableResource.m_ImageGround.DrawImageTile(&dc,nXTablePos,nYTablePos,SizeTable.cx,SizeTable.cy);
				}
			}
		}

		//绘画空隙
		if (m_nXExcursion>rcClip.left)
		{
			INT nTileWidth=m_nXExcursion-rcClip.left;
			m_TableResource.m_ImageGround.DrawImageTile(&dc,rcClip.left,rcClip.top,nTileWidth,rcClip.Height());
		}

		//绘画空隙
		if ((m_nYExcursion-m_nScrollPos)>rcClip.top)
		{
			INT nTileHeight=(m_nYExcursion-m_nScrollPos)-rcClip.top;
			m_TableResource.m_ImageGround.DrawImageTile(&dc,rcClip.left,rcClip.top,rcClip.Width(),nTileHeight);
		}
	}
	else 
	{
		//绘画空隙
		if (m_TableResource.m_ImageGround.IsNull()==false)
		{
			m_TableResource.m_ImageGround.DrawImageTile(&dc,rcClip);
		}
		else
		{
			dc.FillSolidRect(&rcClip,RGB(0,0,0));
		}
	}

	return;
}

//位置消息
VOID CTableViewFrame::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType, cx, cy);

	//调整控件
	RectifyControl(cx,cy);

	return;
}

//消息解释
BOOL CTableViewFrame::PreTranslateMessage(MSG * pMsg)
{
	//消息过滤
	if (pMsg->message==WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_UP:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_LINEUP,m_nScrollPos),NULL);
				return TRUE;
			}
		case VK_DOWN:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_LINEDOWN,m_nScrollPos),NULL);
				return TRUE;
			}
		case VK_PRIOR:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEUP,m_nScrollPos),NULL);
				return TRUE;
			}
		case VK_NEXT:
		case VK_SPACE:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_PAGEDOWN,m_nScrollPos),NULL);
				return TRUE;
			}
		case VK_HOME:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_TOP,m_nScrollPos),NULL);
				return TRUE;
			}
		case VK_END:
			{
				SendMessage(WM_VSCROLL,MAKELONG(SB_BOTTOM,m_nScrollPos),NULL);
				return TRUE;
			}
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

//滚动消息
VOID CTableViewFrame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
	//设置焦点
	SetFocus();

	//获取参数
	CRect rcClient;
	GetClientRect(&rcClient);
	INT nLastPos=m_nScrollPos;

	//移动坐标
	switch (nSBCode)
	{
	case SB_TOP:
		{
			m_nScrollPos=0;
			break;
		}
	case SB_BOTTOM:
		{
			m_nScrollPos=m_nScrollPosMax-m_nVisibleLineCount*m_TableResource.m_SizeTable.cy;
			break;
		}
	case SB_LINEUP:
		{
			m_nScrollPos-=SCROLL_POS_LINE;
			break;
		}
	case SB_PAGEUP:
		{
			m_nScrollPos-=rcClient.bottom;
			break;
		}
	case SB_LINEDOWN:
		{
			m_nScrollPos+=SCROLL_POS_LINE;
			break;
		}
	case SB_PAGEDOWN:
		{
			m_nScrollPos+=rcClient.bottom;
			break;
		}
	case SB_THUMBTRACK:
		{
			m_nScrollPos=nPos;
			break;
		}
	}
	
	//调整位置
	m_nScrollPos=__min(m_nScrollPos,m_nScrollPosMax-rcClient.bottom);
	if (m_nScrollPos<0) m_nScrollPos=0;

	//滚动窗口
	if (nLastPos!=m_nScrollPos)
	{
		SetScrollPos(SB_VERT,m_nScrollPos);
		ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);
	}

	return;
}

//鼠标消息
VOID CTableViewFrame::OnLButtonDblClk(UINT nFlags, CPoint MousePoint)
{
	//设置焦点
	SetFocus();

	//事件通知
	m_pITableViewFrameSink->OnDButtonHitTable(m_wDownTableID);

	return;
}

//鼠标消息
LRESULT CTableViewFrame::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	//设置变量
	m_bHovering=false;

	//设置界面
	if (m_wDownTableID!=INVALID_TABLE) 
	{
		//设置变量
		WORD wTableID=m_wDownTableID;
		m_wDownTableID=INVALID_TABLE;

		//设置界面
		ITableView * pITableView=GetTableViewItem(wTableID);
		if (pITableView!=NULL) pITableView->SetHoverStatus(INVALID_CHAIR);
	}

	return 0;
}

//鼠标消息
VOID CTableViewFrame::OnLButtonDown(UINT nFlags, CPoint MousePoint)
{
	//设置焦点
	SetFocus();

	//动作处理
	if ((m_bRMouseDown==false)&&(m_bLMouseDown==false))
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_bLMouseDown=true;
		m_wDownTableID=SwitchTableID(MousePoint);

		//获取桌子
		ITableView * pITableView=NULL;
		if (m_wDownTableID!=INVALID_TABLE) pITableView=GetTableViewItem(m_wDownTableID);

		//动作处理
		if (pITableView!=NULL)
		{
			//位置定义
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_nXExcursion)%m_TableResource.m_SizeTable.cx;
			TablePoint.y=(MousePoint.y-m_nYExcursion+m_nScrollPos)%m_TableResource.m_SizeTable.cy;

			//按键测试
			m_wDownChairID=SwitchChairID(TablePoint);

			//结果处理
			if (m_wDownChairID!=INVALID_CHAIR) 
			{
				pITableView->SetMouseDown(m_bLMouseDown);
				pITableView->SetHoverStatus(m_wDownChairID);
			}
		}
	}

	return;
}

//鼠标消息
VOID CTableViewFrame::OnLButtonUp(UINT nFlags, CPoint MousePoint)
{
	if ((m_bLMouseDown==true)&&(m_bRMouseDown==false))
	{
		//释放捕获
		ReleaseCapture();

		//设置变量
		m_bLMouseDown=false;

		//变量定义
		WORD wChairID=INVALID_CHAIR;
		WORD wTableID=SwitchTableID(MousePoint);

		//清理桌子
		if (m_wDownTableID!=INVALID_TABLE)
		{
			//获取桌子
			ASSERT(GetTableViewItem(m_wDownTableID)!=NULL);
			ITableView * pITableView=GetTableViewItem(m_wDownTableID);

			//设置状态
			pITableView->SetMouseDown(false);
			pITableView->SetHoverStatus(INVALID_CHAIR);
		}

		//获取位置
		if ((wTableID!=INVALID_TABLE)&&(wTableID==m_wDownTableID))
		{
			//获取桌子
			ASSERT(GetTableViewItem(wTableID)!=NULL);
			ITableView * pITableView=GetTableViewItem(wTableID);

			//计算位置
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_nXExcursion)%m_TableResource.m_SizeTable.cx;
			TablePoint.y=(MousePoint.y-m_nYExcursion+m_nScrollPos)%m_TableResource.m_SizeTable.cy;

			//椅子测试
			wChairID=SwitchChairID(TablePoint);
		}

		//结果处理
		if ((wTableID==m_wDownTableID)&&(wChairID==m_wDownChairID))
		{
			//设置变量
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;

			//事件通知
			m_pITableViewFrameSink->OnLButtonHitTable(wTableID,wChairID);
		}
		else
		{
			//设置变量
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
		}
	}

	return;
}

//鼠标消息
VOID CTableViewFrame::OnRButtonDown(UINT nFlags, CPoint MousePoint)
{
	//设置焦点
	SetFocus();

	//动作处理
	if ((m_bRMouseDown==false)&&(m_bLMouseDown==false))
	{
		//鼠标扑获
		SetCapture();

		//设置变量
		m_bRMouseDown=true;

		//桌子测试
		m_wDownTableID=SwitchTableID(MousePoint);

		//设置状态
		if (m_wDownTableID!=INVALID_TABLE)
		{
			//获取桌子
			ASSERT(GetTableViewItem(m_wDownTableID)!=NULL);
			ITableView * pITableView=GetTableViewItem(m_wDownTableID);

			//计算位置
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_nXExcursion)%m_TableResource.m_SizeTable.cx;
			TablePoint.y=(MousePoint.y-m_nYExcursion+m_nScrollPos)%m_TableResource.m_SizeTable.cy;

			//椅子测试
			m_wDownChairID=SwitchChairID(TablePoint);

			//结果处理
			if (m_wDownChairID!=INVALID_CHAIR)
			{
				pITableView->SetMouseDown(m_bRMouseDown);
				pITableView->SetHoverStatus(m_wDownChairID);
			}
		}
	}

	return;
}

//鼠标消息
VOID CTableViewFrame::OnRButtonUp(UINT nFlags, CPoint MousePoint)
{
	if (m_bRMouseDown==true)
	{
		//释放捕获
		ReleaseCapture();

		//设置变量
		m_bRMouseDown=false;

		//变量定义
		WORD wChairID=INVALID_CHAIR;
		WORD wTableID=SwitchTableID(MousePoint);

		//清理状态
		if (m_wDownTableID!=INVALID_TABLE)
		{
			//获取桌子
			ASSERT(GetTableViewItem(m_wDownTableID)!=NULL);
			ITableView * pITableView=GetTableViewItem(m_wDownTableID);

			//设置状态
			pITableView->SetMouseDown(false);
			pITableView->SetHoverStatus(INVALID_CHAIR);
		}

		//获取位置
		if ((wTableID!=INVALID_TABLE)&&(wTableID==m_wDownTableID))
		{
			//获取桌子
			ASSERT(GetTableViewItem(wTableID)!=NULL);
			ITableView * pITableView=GetTableViewItem(wTableID);

			//计算位置
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_nXExcursion)%m_TableResource.m_SizeTable.cx;
			TablePoint.y=(MousePoint.y+m_nScrollPos)%m_TableResource.m_SizeTable.cy;

			//椅子测试
			wChairID=SwitchChairID(TablePoint);
		}

		//结果处理
		if ((wTableID==m_wDownTableID)&&(wChairID==m_wDownChairID))
		{
			//设置变量
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;

			//事件通知
			m_pITableViewFrameSink->OnRButtonHitTable(wTableID,wChairID);
		}
		else
		{
			//设置变量
			m_wDownTableID=INVALID_TABLE;
			m_wDownChairID=INVALID_CHAIR;
		}
	}

	return;
}

//鼠标消息
BOOL CTableViewFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint MousePoint)
{
	//获取位置
	CRect rcClient;
	GetClientRect(&rcClient);

	//设置滚动
	INT nLastPos=m_nScrollPos;
	INT nMaxPos=m_nScrollPosMax-rcClient.bottom;
	m_nScrollPos=__max(__min(m_nScrollPos-zDelta/WHELL_TIMES,nMaxPos),0L);

	//滚动窗口
	SetScrollPos(SB_VERT,m_nScrollPos);
	ScrollWindow(0,nLastPos-m_nScrollPos,NULL,NULL);

	return TRUE;
}

//光标消息
BOOL CTableViewFrame::OnSetCursor(CWnd * pWnd, UINT nHitTest, UINT uMessage)
{
	//进入判断
	if (m_bHovering==false)
	{
		//设置变量
		m_bHovering=true;

		//变量定义
		TRACKMOUSEEVENT TrackMouseEvent;
		ZeroMemory(&TrackMouseEvent,sizeof(TrackMouseEvent));

		//注册消息
		TrackMouseEvent.hwndTrack=m_hWnd;
		TrackMouseEvent.dwFlags=TME_LEAVE;
		TrackMouseEvent.dwHoverTime=HOVER_DEFAULT;
		TrackMouseEvent.cbSize=sizeof(TrackMouseEvent);

		//注册事件
		_TrackMouseEvent(&TrackMouseEvent);
	}

	//状态判断
	if ((m_bRMouseDown==false)&&(m_bLMouseDown==false))
	{
		//获取光标
		CPoint MousePoint;
		GetCursorPos(&MousePoint);
		ScreenToClient(&MousePoint);

		//变量定义
		WORD wNewChairID=INVALID_CHAIR;
		WORD wNewTableID=SwitchTableID(MousePoint);

		//获取椅子
		if (wNewTableID!=INVALID_TABLE)
		{
			//变量定义
			CPoint TablePoint;
			TablePoint.x=(MousePoint.x-m_nXExcursion)%m_TableResource.m_SizeTable.cx;
			TablePoint.y=(MousePoint.y-m_nYExcursion+m_nScrollPos)%m_TableResource.m_SizeTable.cy;

			//获取椅子
			wNewChairID=SwitchChairID(TablePoint);
		}
		
		//设置盘旋
		if (((wNewTableID!=m_wDownTableID)||(wNewChairID!=m_wDownChairID))&&((wNewTableID!=INVALID_CHAIR)||(m_wDownChairID!=INVALID_CHAIR)))
		{
			//清理盘旋
			if (m_wDownTableID!=INVALID_TABLE)
			{
				GetTableViewItem(m_wDownTableID)->SetHoverStatus(INVALID_CHAIR);
			}

			//设置盘旋
			if ((wNewTableID!=INVALID_TABLE)&&(wNewChairID!=INVALID_CHAIR))
			{
				GetTableViewItem(wNewTableID)->SetHoverStatus(wNewChairID);
			}
		}
		
		//设置变量
		m_wDownTableID=wNewTableID;
		m_wDownChairID=wNewChairID;

		//设置光标
		if ((m_wDownTableID!=INVALID_TABLE)&&(m_wDownChairID!=INVALID_CHAIR))
		{
			SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_HAND_CUR)));
			return TRUE;
		}
	}

	return __super::OnSetCursor(pWnd,nHitTest,uMessage);
}

//桌子测试
WORD CTableViewFrame::SwitchTableID(POINT MousePoint)
{
	//状态效验
	if (m_TableResource.m_SizeTable.cx==0) return INVALID_TABLE;
	if (m_TableResource.m_SizeTable.cy==0) return INVALID_TABLE;

	//位置计算
	INT nXItem=(MousePoint.x-m_nXExcursion)/m_TableResource.m_SizeTable.cx;
	INT nYItem=(MousePoint.y-m_nYExcursion+m_nScrollPos)/m_TableResource.m_SizeTable.cy;

	//结果判断
	if ((nYItem>=0)&&(nXItem>=0)&&(nYItem<m_nYTableCount)&&(nXItem<m_nXTableCount))
	{
		//设置结果
		WORD wResultItem=nYItem*m_nXTableCount+nXItem;

		//结果判断
		if (wResultItem<m_wTableCount)
		{
			bool bHideUserInfo=CServerRule::IsAvertCheatMode(m_dwServerRule);
			if ((wResultItem==0)||(bHideUserInfo==false)) return wResultItem;
		}
	}

	return INVALID_TABLE;
}

//椅子测试
WORD CTableViewFrame::SwitchChairID(POINT MousePoint)
{
	//位置搜索
	if ((m_TableResource.m_bShowUser==true)||(m_TableResource.m_bShowChair==true))
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			//横向判断
			if (MousePoint.x<m_TableResource.m_rcChairArray[i].left) continue;
			if (MousePoint.x>m_TableResource.m_rcChairArray[i].right) continue;

			//竖向判断
			if (MousePoint.y<m_TableResource.m_rcChairArray[i].top) continue;
			if (MousePoint.y>m_TableResource.m_rcChairArray[i].bottom) continue;

			return i;
		}
	}

	//进入区域
	if (m_TableResource.m_bShowEnter==true)
	{
		//变量定义
		bool bOutSide=false;

		//横向判断
		if (MousePoint.x<m_TableResource.m_ptEnter.x) bOutSide=true;
		if (MousePoint.x>m_TableResource.m_ptEnter.x+m_TableResource.m_SizeEnter.cx) bOutSide=true;

		//竖向判断
		if (MousePoint.y<m_TableResource.m_ptEnter.y) bOutSide=true;
		if (MousePoint.y>m_TableResource.m_ptEnter.y+m_TableResource.m_SizeEnter.cy) bOutSide=true;

		//结果判断
		if (bOutSide==false)
		{
			return INDEX_ENTER_CHAIR;
		}
	}

	return INVALID_CHAIR;
}

//调整控件
VOID CTableViewFrame::RectifyControl(INT nWidth, INT nHeight)
{
	//调整位置
	if ((m_wTableCount!=0)&&(m_TableResource.m_SizeTable.cx>0L)&&(m_TableResource.m_SizeTable.cy>0L))
	{
		//变量定义
		INT nViewWidth=__max(m_TableResource.m_SizeTable.cx,nWidth);
		INT nViewHeight=__max(m_TableResource.m_SizeTable.cy,nHeight);

		//视图参数
		m_nXTableCount=nViewWidth/m_TableResource.m_SizeTable.cx;
		m_nYTableCount=(m_wTableCount+m_nXTableCount-1)/m_nXTableCount;
		m_nVisibleLineCount=nViewHeight/m_TableResource.m_SizeTable.cy;

		//偏移位置
		if (m_wTableCount==1)
		{
			m_nXExcursion=(nViewWidth-m_TableResource.m_SizeTable.cx)/2;
			m_nYExcursion=(m_nVisibleLineCount>=m_nYTableCount)?(nHeight-m_nYTableCount*m_TableResource.m_SizeTable.cy)/2:0;
		}
		else
		{
			m_nYExcursion=0L;
			m_nXExcursion=(nViewWidth-m_nXTableCount*m_TableResource.m_SizeTable.cx)/2;
		}
	}
	
	//滚动数据
	m_nScrollPosMax=m_nYTableCount*m_TableResource.m_SizeTable.cy;
	m_nScrollPos=__max(0,__min(m_nScrollPos,m_nScrollPosMax-m_nVisibleLineCount*m_TableResource.m_SizeTable.cy));
	
	//设置滚动
	SCROLLINFO ScrollInfoVert;
	ScrollInfoVert.cbSize=sizeof(ScrollInfoVert);
	ScrollInfoVert.fMask=SIF_RANGE|SIF_PAGE|SIF_POS;
	ScrollInfoVert.nMin=0;
	ScrollInfoVert.nMax=m_nScrollPosMax;
	ScrollInfoVert.nPage=nHeight;
	ScrollInfoVert.nPos=m_nScrollPos;
	SetScrollInfo(SB_VERT,&ScrollInfoVert,FALSE);

	return;
}

//事件消息
LRESULT CTableViewFrame::OnMessagePlatformEvent(WPARAM wParam, LPARAM lParam)
{
	//界面更新
	if (wParam==EVENT_SKIN_CONFIG_UPDATE)
	{
		//渲染判断
		if (m_TableResource.m_bRenderImage==true)
		{
			//更新资源
			CSkinRenderManager * pSkinRenderManager=CSkinRenderManager::GetInstance();
			if (pSkinRenderManager!=NULL) m_TableResource.UpdateResource(pSkinRenderManager);

			//更新界面
			RedrawWindow(NULL,NULL,RDW_ERASE|RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASENOW);
		}

		return 0;
	}

	return 0L;
}

//////////////////////////////////////////////////////////////////////////////////
