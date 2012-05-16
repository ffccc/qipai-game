#include "stdafx.h"
#include"resource.h"
#include "DlgCustomFace.h"
#include "GameFrame.h"
#include "GlobalUnits.h"
#include "Zip.h"

////////////////////////////////////////////////////////////////////////////////////
//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ

#define BGROUND_COLOR			RGB(254,250,221)					//������ɫ
#define FRAME_TL_COLOR1			RGB(128,128,128)					//�߿���ɫ
#define FRAME_TL_COLOR2			RGB(64,64,64)						//�߿���ɫ
#define FRAME_RB_COLOR2			RGB(212,208,200)					//�߿���ɫ
#define FRAME_RB_COLOR1			RGB(212,208,200)					//�߿���ɫ

#define SMALL_FACE_WIDTH		32									//ͼƬ��С
#define SMALL_FACE_HEIGHT		32									//ͼƬ��С

#define LARGE_FACE_WIDTH		100									//ͼƬ��С
#define LARGE_FACE_HEIGHT		100									//ͼƬ��С

#define SMALL_FRAME_LEFT		235									//�߿�λ��
#define SMALL_FRAME_RIGHT		(SMALL_FRAME_LEFT+SMALL_FACE_WIDTH)	//�߿�λ��
#define SMALL_FRAME_TOP			105									//�߿�λ��
#define SMALL_FRAME_BOTTOM		(SMALL_FRAME_TOP+SMALL_FACE_HEIGHT)	//�߿�λ��

#define LARGE_FRAME_LEFT		50									//�߿�λ��
#define LARGE_FRAME_RIGHT		(LARGE_FRAME_LEFT+LARGE_FACE_WIDTH)	//�߿�λ��
#define LARGE_FRAME_TOP			75									//�߿�λ��
#define LARGE_FRAME_BOTTOM		(LARGE_FRAME_TOP+LARGE_FACE_HEIGHT)	//�߿�λ��

#define FRAME_SPACE				2									//�߿���

////////////////////////////////////////////////////////////////////////////////////


BEGIN_MESSAGE_MAP(CDlgCustomFace, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BROWSER,OnBnClickedBrowser)
	ON_BN_CLICKED(IDC_UPLOAD,OnBnClickedUpload) 
	ON_BN_CLICKED(IDC_DELETE,OnBnClickedDelete) 
	ON_WM_CTLCOLOR()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////////////

//���캯��
CDlgCustomFace::CDlgCustomFace(CWnd* pParent): CDialog(IDD_CUSTOM_FACE, pParent), CSkinWndObject(this)
{
	//���ñ���
	m_pIClientSocket = NULL;
	m_enOperateStatus=enOperateStatus_NULL;
}

//�鹹����
CDlgCustomFace::~CDlgCustomFace()
{
	//�ͷ���Դ
	if ( !m_Image.IsNull() ) m_Image.Destroy();
	if (m_brBkground.m_hObject != NULL ) m_brBkground.DeleteObject();
}

//��������
void CDlgCustomFace::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BROWSER, m_btBrowser);
	DDX_Control(pDX, IDC_UPLOAD, m_btUpload);
	DDX_Control(pDX, IDC_DELETE, m_btDelete);
}

//��ʼ����
BOOL CDlgCustomFace::OnInitDialog()
{
	__super::OnInitDialog();

	//����ˢ��
	m_brBkground.CreateSolidBrush(BGROUND_COLOR);

	//���ñ���
	SetWindowText(TEXT("�Զ���ͷ��"));

	//���¿ؼ�
	UpdateControls();

	//��ʼ������
	__super::InitSkinObject();

	return TRUE;  
}

//�滭����
void CDlgCustomFace::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcRect;
	GetClientRect(&rcRect);
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();

	//�滭�ڿ�
	CPen BorderPen(PS_SOLID,1,m_SkinAttribute.m_crInsideBorder);
	CPen * pOldPen=dc.SelectObject(&BorderPen);
	CRect rcDrawRect(nXExcursion,nYExcursion,rcRect.Width()-2*nXExcursion,rcRect.Height());
	dc.SelectObject(m_SkinAttribute.m_brBackGround);
	if (m_bMaxSize==true) rcDrawRect.DeflateRect(3,3,3,2);
	else rcDrawRect.DeflateRect(3,4,3,4);
	dc.RoundRect(&rcDrawRect,CPoint(8,8));
	dc.SelectObject(pOldPen);
	BorderPen.DeleteObject();

	//�滭�߿�
	CRect rcSmallFace(SMALL_FRAME_LEFT-FRAME_SPACE, SMALL_FRAME_TOP-FRAME_SPACE, SMALL_FRAME_RIGHT+FRAME_SPACE, SMALL_FRAME_BOTTOM+FRAME_SPACE);
	dc.Draw3dRect(rcSmallFace, FRAME_TL_COLOR1, FRAME_RB_COLOR1);
	rcSmallFace.DeflateRect(1,1,1,1);
	dc.Draw3dRect(rcSmallFace, FRAME_TL_COLOR2, FRAME_RB_COLOR2);

	//�滭�߿�
	CRect rcLargeFace(LARGE_FRAME_LEFT-FRAME_SPACE, LARGE_FRAME_TOP-FRAME_SPACE, LARGE_FRAME_RIGHT+FRAME_SPACE, LARGE_FRAME_BOTTOM+FRAME_SPACE);
	dc.Draw3dRect(rcLargeFace, FRAME_TL_COLOR1, FRAME_RB_COLOR1);
	rcLargeFace.DeflateRect(1,1,1,1);
	dc.Draw3dRect(rcLargeFace, FRAME_TL_COLOR2, FRAME_RB_COLOR2);

	//�滭ͷ��
	tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
	if ( !m_Image.IsNull() )
	{
		CDC *pDC = GetDC() ;

		//�����С
		CRect rcSmallFace(SMALL_FRAME_LEFT, SMALL_FRAME_TOP, SMALL_FRAME_RIGHT, SMALL_FRAME_BOTTOM);
		CRect rcLargeFace(LARGE_FRAME_LEFT, LARGE_FRAME_TOP, LARGE_FRAME_RIGHT, LARGE_FRAME_BOTTOM);

		//����ģʽ
		dc.SetStretchBltMode(HALFTONE);

		//�滭ͼƬ
		m_Image.StretchBlt(dc.GetSafeHdc(), rcSmallFace);
		m_Image.StretchBlt(dc.GetSafeHdc(), rcLargeFace);

		//�ͷ���Դ
		ReleaseDC(pDC);
	}
	else if(GlobalUserData.dwUserID!=0 && GlobalUserData.dwCustomFaceVer!=0)
	{
		CDC *pDC = GetDC() ;
		g_GlobalUnits.m_UserFaceRes->DrawNormalFace(pDC,SMALL_FRAME_LEFT,SMALL_FRAME_TOP,
			GlobalUserData.wFaceID,GlobalUserData.dwUserID,GlobalUserData.dwCustomFaceVer);

		g_GlobalUnits.m_UserFaceRes->DrawCustomBigFace(pDC,LARGE_FRAME_LEFT,LARGE_FRAME_TOP,
			GlobalUserData.dwUserID,GlobalUserData.dwCustomFaceVer);

		//�ͷ���Դ
		ReleaseDC(pDC);
	}

	//�滭����
	__super::DrawSkinView(&dc);
}

//���ͼƬ
void CDlgCustomFace::OnBnClickedBrowser()
{
	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("֧�ָ�ʽ(*.bmp;*.jpg;*.gif;*.png)|*.bmp; *.jpg; *.jpeg; *.jpe;*.jfif;*.gif;*.png|�����ļ� (*.*)|*.*||");
	CFileDialog FileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter);

	if(IDOK == FileDlg.DoModal())
	{
		//����ͼƬ
		CString strName = FileDlg.GetPathName() ;

		//�ͷ��ж�
		if ( !m_Image.IsNull() ) m_Image.Destroy();

		HRESULT hResult = m_Image.Load(strName);

		//�ɹ��ж�
		if ( SUCCEEDED(hResult) )
		{
			//���½���
			CRect rcSmallFace(SMALL_FRAME_LEFT, SMALL_FRAME_TOP, SMALL_FRAME_RIGHT, SMALL_FRAME_BOTTOM);
			CRect rcLargeFace(LARGE_FRAME_LEFT, LARGE_FRAME_TOP, LARGE_FRAME_RIGHT, LARGE_FRAME_BOTTOM);
			InvalidateRect(rcSmallFace, FALSE);
			InvalidateRect(rcLargeFace, FALSE);
		}
		else
		{
			ShowMessage(TEXT("����ͼƬʧ�ܣ�������ѡ��"));
			return ;
		}
	}

	//���¿ؼ�
	UpdateControls();
}

//�ϴ�ͼƬ
void CDlgCustomFace::OnBnClickedUpload()
{
	if ( m_Image.IsNull() )
	{
		ShowMessage(TEXT("��ѡ��ͼƬ��"));
		return ;
	}

	//����״̬
	m_enOperateStatus = enOperateStatus_Upload;

	//��������
	if ( ConnectServer() == false )
	{
		//����״̬
		m_enOperateStatus = enOperateStatus_NULL;
		return;
	}

	//��ȡDC
	CDC *pDC = GetDC();

	LONG lBmpSize = LARGE_FACE_HEIGHT * (LARGE_FACE_WIDTH*3+LARGE_FACE_WIDTH%4) ;
	BYTE *pByteBmpData = (BYTE *)VirtualAlloc(NULL,lBmpSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	//����λͼ
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bih;

	bfh.bfOffBits=sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);
	bfh.bfReserved1=0;
	bfh.bfReserved2=0;
	bfh.bfType = 0x4D42;
	bfh.bfSize=lBmpSize+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER) ;

	bih.biBitCount=24;
	bih.biClrImportant=0;
	bih.biClrUsed=0;
	bih.biCompression=BI_RGB;
	bih.biHeight=LARGE_FACE_HEIGHT;
	bih.biPlanes=1;
	bih.biSize=sizeof(bih);
	bih.biSizeImage=0;
	bih.biWidth=LARGE_FACE_WIDTH;
	bih.biXPelsPerMeter=0;
	bih.biYPelsPerMeter=0;	

	//��������
	WORD wWidth = (LARGE_FACE_WIDTH*3+LARGE_FACE_WIDTH%4);
	for ( int nCol = 0, nRealCol = LARGE_FACE_HEIGHT-1; nCol < LARGE_FACE_HEIGHT; ++nCol, --nRealCol )
		for ( WORD wRow = 0, wByteIdex = 0; wRow < LARGE_FACE_WIDTH; wRow++, wByteIdex+=3)
		{
			COLORREF clr = pDC->GetPixel(LARGE_FRAME_LEFT+wRow, LARGE_FRAME_TOP+nCol);
			pByteBmpData[nRealCol*wWidth+wByteIdex] = GetBValue(clr);
			pByteBmpData[nRealCol*wWidth+wByteIdex+1] = GetGValue(clr);
			pByteBmpData[nRealCol*wWidth+wByteIdex+2] = GetRValue(clr);
		}

	//����Ŀ¼
	CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");
	CreateDirectory(strDirName, NULL) ;
	CString strFileName;
	strFileName = TEXT("\\MyFace.bmp");

	//д���ļ�
	CFile fileMyFace;
	try
	{
		if ( fileMyFace.Open(strDirName + strFileName, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
		{
			//д��λͼ
			fileMyFace.Write(&bfh, sizeof(bfh));
			fileMyFace.Write(&bih, sizeof(bih));
			fileMyFace.Write(pByteBmpData, lBmpSize);

			//�ر��ļ�
			fileMyFace.Close();
		}

		//ѹ���ļ�
		CZip oZip( strDirName + strFileName );
		oZip.SwapSize( strDirName+TEXT("\\MyFace.zip") );
	}
	catch(...){}			

	//�ͷ���Դ
	ReleaseDC(pDC);
	VirtualFree(pByteBmpData, lBmpSize , MEM_DECOMMIT) ;

	//���¿ؼ�
	UpdateControls();
}

//ɾ��ͼƬ
void CDlgCustomFace::OnBnClickedDelete()
{
	//����״̬
	m_enOperateStatus = enOperateStatus_Delete;

	//��������
	if ( ConnectServer() == false )
	{
		//����״̬
		m_enOperateStatus = enOperateStatus_NULL;
		return;
	}

	//�ͷ���Դ
	if ( ! m_Image.IsNull() ) m_Image.Destroy();

	//���¿ؼ�
	UpdateControls();

	//���½���
	InvalidateRect(NULL);
}

//��ʾ��Ϣ
void CDlgCustomFace::ShowMessage(TCHAR *pMessage)
{
	ShowInformationEx(pMessage,0,MB_ICONQUESTION,TEXT("�Զ���ͷ��"));

	return;
}

//��������
bool CDlgCustomFace::ConnectServer()
{
	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT && m_enOperateStatus != enOperateStatus_NULL )
	{
		ShowMessage(TEXT("�������ڽ��У����Ժ�"));
		return false;
	}

	if ( m_pIClientSocket != NULL && m_pIClientSocket->GetSocketStatus() == SOCKET_STATUS_CONNECT )//?GetSocketStatus
	{
		SendData();
		return true;
	}

	//��ȡ����
	CGameFrame *pGameFrame = (CGameFrame *)AfxGetMainWnd() ;
	CPlazaViewItem * pPlazaViewItem = &(pGameFrame->m_DlgGamePlaza) ;
	if ( m_pIClientSocket == NULL )
	{

		//�Ϸ��ж�
		if ( pPlazaViewItem->m_ClientSocket.GetInterface() == NULL ) 
		{
			ShowMessage(TEXT("���������û�д����������µ�¼��"));
			return false;
		}

		//���ñ���
		m_pIClientSocket = (ITCPSocket*)pPlazaViewItem->m_ClientSocket.GetInterface();
	}

	//��ַ����
	CRegKey RegServer;
	LPCTSTR pszServerIP=pPlazaViewItem->m_DlgLogon.GetLogonServer();
	TCHAR szRegServer[256]=TEXT(""),szServerAddr[64]=TEXT("");
	_snprintf(szRegServer,sizeof(szRegServer),TEXT("%s\\%s"),REG_LOGON_SERVER,pszServerIP);

	if (RegServer.Open(HKEY_CURRENT_USER,szRegServer,KEY_READ)==ERROR_SUCCESS)
	{
		TCHAR szReadData[1024]=TEXT("");
		DWORD dwReadData=0L,dwDataType=0L,dwDataSize=sizeof(szReadData);
		LONG lErrorCode=RegServer.QueryValue(TEXT("ServerAddr"),&dwDataType,szReadData,&dwDataSize);
		if (lErrorCode==ERROR_SUCCESS) 
		{
			CXOREncrypt::CrevasseData(szReadData,szServerAddr,sizeof(szServerAddr));
			pszServerIP=szServerAddr;
		}
	}

	//���ӷ�����
	try
	{
		//���ӷ�����
		m_pIClientSocket->CloseSocket();
		if (m_pIClientSocket->Connect(pszServerIP,PORT_LOGON_SERVER)!=ERROR_SUCCESS)
		{
			throw TEXT("���������Ӵ��󣬿��������ϵͳ��û�гɹ����������磡");
		}
	}
	catch (LPCTSTR pszError)
	{
		ShowMessage((TCHAR*)pszError);
		return false;
	}

	return true;
}

int g_nUploadCount = 0;
//��������
bool CDlgCustomFace::SendData()
{
	g_nUploadCount = 0;
	//״̬�ж�
	ASSERT(m_enOperateStatus != enOperateStatus_NULL);
	if ( m_enOperateStatus == enOperateStatus_NULL ) return false;

	if ( m_pIClientSocket == NULL || m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) return false;

	if ( m_enOperateStatus == enOperateStatus_Upload )
	{
		//��ȡ�ļ�
		CString strDirName = CString(g_GlobalUnits.GetWorkDirectory()) + TEXT("\\CustomFace");
		CString strFileName;
		strFileName = TEXT("\\MyFace.zip");

		CFile fileMyFace;
		try
		{
			//��������
			CMD_GP_UploadCustomFace UploadCustomFace;
			UploadCustomFace.dwUserID = g_GlobalUnits.GetGolbalUserData().dwUserID;
			UploadCustomFace.bFirstPacket=true;

			if ( fileMyFace.Open(strDirName + strFileName, CFile::modeRead|CFile::typeBinary) )
			{				
				UINT nReadSize = 0;
				do
				{
					//��ȡ����
					nReadSize=fileMyFace.Read(UploadCustomFace.bFaceData, CountArray(UploadCustomFace.bFaceData));
					UploadCustomFace.wCurrentSize=nReadSize;

					//����ж�
					if ( nReadSize != CountArray(UploadCustomFace.bFaceData) ) UploadCustomFace.bLastPacket=true;
					else UploadCustomFace.bLastPacket=false;

					//�����ж�
					if ( m_pIClientSocket->GetSocketStatus() != SOCKET_STATUS_CONNECT ) break;

					//��������
					WORD wSendSize = sizeof(UploadCustomFace) - sizeof(UploadCustomFace.bFaceData) + nReadSize;
					m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_USER_UPLOAD_FACE, &UploadCustomFace, wSendSize) ;

					Sleep(500);

					//���ñ�ʶ
					UploadCustomFace.bFirstPacket=false;

				} while(nReadSize==CountArray(UploadCustomFace.bFaceData));

				//�ر��ļ�
				fileMyFace.Close();

				//ɾ���ļ�
				CFile::Remove(strDirName + strFileName);
			}
			else
			{
				ShowMessage(TEXT("�ļ�����ʧ��"));
			}
		}
		catch(...){}
	}
	else if ( m_enOperateStatus == enOperateStatus_Delete )
	{
		CMD_GP_CustomFaceDelete CustomFaceDelete;
		ZeroMemory(&CustomFaceDelete, sizeof(CustomFaceDelete));

		tagGlobalUserData &GlobalUserData = g_GlobalUnits.GetGolbalUserData();
		CustomFaceDelete.dwUserID = GlobalUserData.dwUserID;
		CustomFaceDelete.dwFaceVer = GlobalUserData.dwCustomFaceVer;

		//Ͷ����Ϣ
		m_pIClientSocket->SendData(MDM_GP_USER, SUB_GP_CUSTOM_FACE_DELETE, &CustomFaceDelete, sizeof(CustomFaceDelete));        

		//Ԥ��ɾ��
		g_GlobalUnits.GetGolbalUserData().dwCustomFaceVer=0;

		//���½���
		InvalidateRect(NULL);
	}

	//����״̬
	m_enOperateStatus = enOperateStatus_NULL ;

	return true;
}

//���¿ؼ�
void CDlgCustomFace::UpdateControls()
{
	//��������
	tagGlobalUserData & GlobalUserInfo=g_GlobalUnits.GetGolbalUserData();

	if ( GlobalUserInfo.dwCustomFaceVer!=0 ) (GetDlgItem(IDC_DELETE))->EnableWindow(TRUE);
	else (GetDlgItem(IDC_DELETE))->EnableWindow(FALSE);

	if ( m_Image.IsNull() ) (GetDlgItem(IDC_UPLOAD))->EnableWindow(FALSE);
	else (GetDlgItem(IDC_UPLOAD))->EnableWindow(TRUE);
}

//��ɫ����
HBRUSH CDlgCustomFace::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
	case CTLCOLOR_STATIC:
		{
			if(pWnd->m_hWnd==GetDlgItem(IDC_WARIMG_MSG)->m_hWnd)
			{
				pDC->SetTextColor(RGB(0,0,255));
			}
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetBkColor(BGROUND_COLOR);
			return m_SkinAttribute.m_brBackGround;
		}
	}
	return __super::OnCtlColor(pDC,pWnd,nCtlColor);
}

//��ʾ��Ϣ
void CDlgCustomFace::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	//���¿ؼ�
	if ( bShow ) UpdateControls();
}

//������Ϣ
void CDlgCustomFace::OnClose()
{
	//�ͷ���Դ
	if ( ! m_Image.IsNull() ) m_Image.Destroy();

	m_enOperateStatus = enOperateStatus_NULL;

	CDialog::OnClose();
}

//�����Ϣ
void CDlgCustomFace::OnLButtonDown(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonDown(nFlags, point);
	OnLMouseDownEvent(point);
}

//�����Ϣ
void CDlgCustomFace::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonUp(nFlags, point);
	OnLMouseUpEvent(point);
}

//�����Ϣ
void CDlgCustomFace::OnMouseMove(UINT nFlags, CPoint point)
{
	CDialog::OnMouseMove(nFlags, point);
	OnMouseMoveEvent(point);
}
////////////////////////////////////////////////////////////////////////////////////