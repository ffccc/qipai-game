#include "StdAfx.h"
#include "Resource.h"
#include "CameraControl.h"
#include "FaceCustomControl.h"

//////////////////////////////////////////////////////////////////////////////////

//�ؼ���ʶ
#define IDC_BT_TURN_LEFT			100									//��ת��ť
#define IDC_BT_TURN_RIGHT			101									//��ת��ť
#define IDC_BT_SHOW_VLINE			102									//���ư�ť
#define IDC_BT_LOAD_IMAGE			103									//����ͼƬ
#define IDC_BT_LOAD_CAMERA			104									//��������

//�ؼ���ʶ
#define IDC_IMAGE_EDIT_CTRL			200									//�༭�ؼ�

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CFaceItemCustomWnd, CWnd)

	//ϵͳ��Ϣ
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CREATE()

	//��ť��Ϣ
	ON_BN_CLICKED(IDC_BT_TURN_LEFT,OnBnClickedTurnLeft)
	ON_BN_CLICKED(IDC_BT_TURN_RIGHT,OnBnClickedTurnRight)
	ON_BN_CLICKED(IDC_BT_SHOW_VLINE,OnBnClickedShowVLine)
	ON_BN_CLICKED(IDC_BT_LOAD_IMAGE,OnBnClickedLoadImage)
	ON_BN_CLICKED(IDC_BT_LOAD_CAMERA,OnBnClickedLoadCamera)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CFaceItemCustomWnd::CFaceItemCustomWnd()
{
}

//��������
CFaceItemCustomWnd::~CFaceItemCustomWnd()
{
}

//�����ж�
bool CFaceItemCustomWnd::IsNull()
{
	return m_ImageEditorControl.IsNull();
}

//����ͼƬ
VOID CFaceItemCustomWnd::DestoryImage()
{
	//״̬�ж�
	if (m_ImageEditorControl.IsNull()==false)
	{
		//���ÿؼ�
		m_btTurnLeft.ShowWindow(SW_HIDE);
		m_btTurnRight.ShowWindow(SW_HIDE);
		m_btShowVLine.ShowWindow(SW_HIDE);

		//����ͼƬ
		m_ImageEditorControl.DestoryImage();
		m_ImageEditorControl.ShowWindow(SW_HIDE);

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_NOERASE|RDW_UPDATENOW);
	}

	return;
}

//���ýӿ�
VOID CFaceItemCustomWnd::SetImageEditorEvent(IImageEditorEvent * pIImageEditorEvent)
{
	//���ýӿ�
	m_ImageEditorControl.SetImageEditorEvent(pIImageEditorEvent);

	return;
}

//��ȡͼƬ
VOID CFaceItemCustomWnd::DrawEditImage(CDC * pDC, INT nXPos, INT nYPos, INT nWidth, INT nHeight)
{
	//��ȡͼƬ
	m_ImageEditorControl.DrawEditImage(pDC,nXPos,nYPos,nWidth,nHeight);

	return;
}

//�滭��Ϣ 
VOID CFaceItemCustomWnd::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//����ͼƬ
	CPngImage ImageFaceEditBack;
	ImageFaceEditBack.LoadImage(GetModuleHandle(AVATAR_CONTROL_DLL_NAME),TEXT("FACE_EDIT_BACK"));

	//�滭����
	ImageFaceEditBack.DrawImage(&dc,30,70);

	//��ȡ����
	ASSERT(CSkinResourceManager::GetInstance()!=NULL);
	CSkinResourceManager * pSkinResourceManager=CSkinResourceManager::GetInstance();

	//���û���
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(pSkinResourceManager->GetDefaultFont());

	//�滭����
	if (m_ImageEditorControl.IsWindowVisible()==FALSE)
	{
		dc.TextOut(60,150,TEXT("������ѡ������һ�ַ�ʽ����ͷ��:"));
		dc.TextOut(60,180,TEXT("������Ƭ��ѡ��һ�ű���ͼƬ�༭���ϴ�Ϊͷ��"));
		dc.TextOut(60,205,TEXT("����ͷ��ͨ������ͷ���ձ༭���ϴ�Ϊͷ��"));
	}

	return;
}

//λ����Ϣ
VOID CFaceItemCustomWnd::OnSize(UINT nType, INT cx, INT cy)
{
	__super::OnSize(nType,cx,cy);

	//��ȡ��С
	CRect rcLoad;
	m_btLoadImage.GetWindowRect(&rcLoad);

	//���ذ�ť
	m_btLoadImage.SetWindowPos(NULL,15,8,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btLoadCamera.SetWindowPos(NULL,25+rcLoad.Width(),8,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//��ȡ��С
	CRect rcControl;
	m_btShowVLine.GetWindowRect(&rcControl);

	//���ư�ť
	INT nXPos=(cx-rcControl.Width()*3-10)/2;
	m_btTurnLeft.SetWindowPos(NULL,nXPos,cy-27,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btTurnRight.SetWindowPos(NULL,nXPos+5+rcControl.Width(),cy-27,0,0,SWP_NOZORDER|SWP_NOSIZE);
	m_btShowVLine.SetWindowPos(NULL,nXPos+10+rcControl.Width()*2,cy-27,0,0,SWP_NOZORDER|SWP_NOSIZE);

	//�ƶ��ؼ�
	m_ImageEditorControl.SetWindowPos(NULL,8,rcLoad.Height()+18,cx-16,cy-rcLoad.Height()-rcControl.Height()-28,SWP_NOZORDER);

	return;
}

//������Ϣ
INT CFaceItemCustomWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//��������
	CRect rcCreate(0,0,0,0);
	HINSTANCE hInstance=GetModuleHandle(AVATAR_CONTROL_DLL_NAME);

	//�����ؼ�
	m_ImageEditorControl.Create(NULL,NULL,WS_CHILD,rcCreate,this,IDC_IMAGE_EDIT_CTRL);

	//������ť
	m_btTurnLeft.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_TURN_LEFT);
	m_btTurnRight.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_TURN_RIGHT);
	m_btShowVLine.Create(TEXT(""),WS_CHILD,rcCreate,this,IDC_BT_SHOW_VLINE);

	//������ť
	m_btLoadImage.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_LOAD_IMAGE);
	m_btLoadCamera.Create(TEXT(""),WS_CHILD|WS_VISIBLE,rcCreate,this,IDC_BT_LOAD_CAMERA);

	//����λͼ
	m_btLoadImage.SetButtonImage(IDB_BT_LOAD_IMAGE,hInstance,false,false);
	m_btLoadCamera.SetButtonImage(IDB_BT_LOAD_CAMERA,hInstance,false,false);

	//����λͼ
	m_btTurnLeft.SetButtonImage(IDB_BT_FACE_BACK,TEXT("BT_TURN_LEFT"),hInstance,false,false);
	m_btTurnRight.SetButtonImage(IDB_BT_FACE_BACK,TEXT("BT_TURN_RIGHT"),hInstance,false,false);
	m_btShowVLine.SetButtonImage(IDB_BT_FACE_BACK,TEXT("BT_SHOW_VLINE"),hInstance,false,false);

	return 0;
}

//��תͼ��
VOID CFaceItemCustomWnd::OnBnClickedTurnLeft()
{
	//ѡ��ͼƬ
	m_ImageEditorControl.SetRoundImage(ROUND_TYPE_LEFT);

	return;
}

//��תͼ��
VOID CFaceItemCustomWnd::OnBnClickedTurnRight()
{
	//ѡ��ͼƬ
	m_ImageEditorControl.SetRoundImage(ROUND_TYPE_RIGHT);

	return;
}

//��ʾ����
VOID CFaceItemCustomWnd::OnBnClickedShowVLine()
{
	//��������
	bool bShowVLine=m_ImageEditorControl.GetVLineStatus();
	m_ImageEditorControl.SetVLineStatus((bShowVLine==false));

	return;
}

//����ͼ��
VOID CFaceItemCustomWnd::OnBnClickedLoadImage()
{
	//��������
	CFileDialog FileDialog(TRUE,NULL,NULL,OFN_OVERWRITEPROMPT,TEXT("ͼ���ļ���*.BMP *.PNG *.JPG��|*.BMP;*.PNG;*JPG||"),this);

	//���ļ�
	if (FileDialog.DoModal()==IDOK)
	{
		//����ͼƬ
		if (m_ImageEditorControl.LoadEditImage(FileDialog.GetPathName())==false)
		{
			//���ð�ť
			m_btTurnLeft.ShowWindow(SW_HIDE);
			m_btTurnRight.ShowWindow(SW_HIDE);
			m_btShowVLine.ShowWindow(SW_HIDE);

			//���ÿؼ�
			m_ImageEditorControl.ShowWindow(SW_HIDE);

			//��ʾ��Ϣ
			CInformation Information(this);
			Information.ShowMessageBox(TEXT("ͼƬ����ʧ�ܣ���ѡ������ͼƬ���ԣ�"),MB_ICONERROR,0);
		}
		else
		{
			//���ð�ť
			m_btTurnLeft.ShowWindow(SW_SHOW);
			m_btTurnRight.ShowWindow(SW_SHOW);
			m_btShowVLine.ShowWindow(SW_SHOW);

			//���ÿؼ�
			m_ImageEditorControl.ShowWindow(SW_SHOW);
		}
	}

	return;
}

//����ͼ��
VOID CFaceItemCustomWnd::OnBnClickedLoadCamera()
{
	//��������
	CCameraControl CameraControl(this);

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(AVATAR_CONTROL_DLL_NAME));

	//����ͼƬ
	if (CameraControl.DoModal()==IDOK)
	{
		//����ͼƬ
		m_ImageEditorControl.ShowWindow(SW_SHOW);

		//����ͼƬ
		ASSERT(CameraControl.m_ImageResult.IsNull()==false);
		bool bSuccess=m_ImageEditorControl.LoadEditImage(CameraControl.m_ImageResult);

		//���ð�ť
		m_btTurnLeft.EnableWindow(bSuccess);
		m_btTurnRight.EnableWindow(bSuccess);
		m_btShowVLine.EnableWindow(bSuccess);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
