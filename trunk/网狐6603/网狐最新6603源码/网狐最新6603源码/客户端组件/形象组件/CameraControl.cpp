#include "Stdafx.h"
#include "Resource.h"
#include "CameraControl.h"

//////////////////////////////////////////////////////////////////////////////////

#define IDC_CAMERA_WND				100									//���ڱ�ʶ

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCameraControl, CSkinDialog)
	ON_BN_CLICKED(IDC_BT_REPEAT,OnBnClickedRepeat)
	ON_BN_CLICKED(IDC_BT_CAMERA_DICT,OnBnClickedCameraDict)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CCameraControl::CCameraControl(CWnd * pParent) : CSkinDialog(IDD_DLG_CAMERA, pParent)
{
	//���ñ���
	m_bInit=false;
	m_hWndCamera=NULL;

	//���ñ���
	ZeroMemory(&m_CapStatus,sizeof(m_CapStatus));
	ZeroMemory(&m_CapTureparms,sizeof(m_CapTureparms));
	ZeroMemory(&m_CapDrivercaps,sizeof(m_CapDrivercaps));

	return;
}

//��������
CCameraControl::~CCameraControl()
{
}

//�ؼ���
VOID CCameraControl::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);

	//��ť�ؼ�
	DDX_Control(pDX, IDOK, m_btOk);
	DDX_Control(pDX, IDC_BT_REPEAT, m_btRepeat);
	DDX_Control(pDX, IDC_BT_CAMERA_DICT, m_btCameraDict);

	//�����ؼ�
	DDX_Control(pDX, IDC_STRING_CONTROL, m_StringControl);
	DDX_Control(pDX, IDC_CAMERA_CONTROL, m_CameraControl);
}

//��Ϣ����
BOOL CCameraControl::PreTranslateMessage(MSG * pMsg)
{
	//��������
	if ((pMsg->message==WM_KEYDOWN)&&((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)))
	{
		return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//���ú���
BOOL CCameraControl::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("��Ƶ����"));

	//������Դ
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//��ȡ��С
	CRect rcClient;
	m_CameraControl.GetClientRect(&rcClient);

	//��������
	DWORD dwStyle=WS_CHILD|WS_BORDER;
	m_hWndCamera=capCreateCaptureWindow(NULL,dwStyle,0,0,rcClient.Width(),rcClient.Height(),m_CameraControl,IDC_CAMERA_WND);

	//�����豸
	if (capDriverConnect(m_hWndCamera,0)==TRUE)
	{
		//��ȡ����
		capDriverGetCaps(m_hWndCamera,&m_CapDrivercaps,sizeof(m_CapDrivercaps));

		//����ж�
		if (m_CapDrivercaps.fCaptureInitialized==TRUE)
		{
			//���ñ���
			m_bInit=true;

			//��������
			capPreview(m_hWndCamera,TRUE);
			capPreviewRate(m_hWndCamera,30);

			//��ʾ����
			::ShowWindow(m_hWndCamera,SW_SHOW);

			//��ȡ״̬
			capGetStatus(m_hWndCamera,&m_CapStatus,sizeof(m_CapStatus));
		}
	}

	//���ÿؼ�
	if (m_bInit==true)
	{
		m_btCameraDict.EnableWindow(TRUE);
		m_StringControl.SetWindowText(TEXT("����ͷ��ʼ���ɹ�"));
	}
	else
	{
		//������Ϣ
		m_StringControl.SetWindowText(TEXT("û�а�װ����ͷ��������ͷ��ʼ��ʧ��"));
	}

	return TRUE; 
}

//���İ�ť
VOID CCameraControl::OnBnClickedRepeat()
{
	//���ô���
	capPreview(m_hWndCamera,TRUE);

	//���ð�ť
	m_btOk.EnableWindow(FALSE);
	m_btRepeat.EnableWindow(FALSE);
	m_btCameraDict.EnableWindow(TRUE);

	return;
}

//���հ�ť
VOID CCameraControl::OnBnClickedCameraDict()
{
	//�ػ�ͼ��
	capGrabFrame(m_hWndCamera);
	capEditCopy(m_hWndCamera);

	//����ͼ��
	if (OpenClipboard()==FALSE)
	{
		ASSERT(FALSE);
		return;
	}

	//����ͼ��
	if (m_ImageResult.IsNull()==false)
	{
		m_ImageResult.Destroy();
	}

	//��������
	HBITMAP hImageResult=(HBITMAP)CopyImage(GetClipboardData(CF_BITMAP),IMAGE_BITMAP,0,0,LR_COPYRETURNORG);

	//�������
	if (hImageResult!=NULL)
	{
		//��������
		m_ImageResult.Attach(hImageResult);

		//��ռ���
		EmptyClipboard();
		CloseClipboard();

		//���ð�ť
		m_btOk.EnableWindow(TRUE);
		m_btRepeat.EnableWindow(TRUE);
		m_btCameraDict.EnableWindow(FALSE);

		return;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
