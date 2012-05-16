#include "StdAfx.h"
#include "resource.h"
#include "math.h"
#include "FlowerEffectControl.h"
#include ".\flowereffectcontrol.h"
///////////////////////////////////////////////////////////////////////////////////

//ʱ�䶨��
#define IDI_FLOWER_START				3000							//�ʻ���ʱ��

//��������
#define LWA_COLORKEY					0x00000001
#define WS_EX_LAYERED					0x00080000

///////////////////////////////////////////////////////////////////////////////////

//���캯��
BEGIN_MESSAGE_MAP(CFlowerEffectControl, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////

//���캯��
CFlowerEffectControl::CFlowerEffectControl(): CDialog(IDD_FLOWER_EFFECT)
{
	m_pFlowerEffectThread = new CFlowerEffectThread;
}

//��������
CFlowerEffectControl::~CFlowerEffectControl()
{
	if(m_pFlowerEffectThread!=NULL)
	{
		delete m_pFlowerEffectThread;
		m_pFlowerEffectThread=NULL;
	}
}

//�ؼ���
void CFlowerEffectControl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//��������
bool CFlowerEffectControl::StartFlowerEffect(UINT uFlowerEffectID)
{
	//�����߳�
	m_pFlowerEffectThread->InitThread(GetSafeHwnd());
	m_pFlowerEffectThread->StartFlowerEffect(uFlowerEffectID);

	return true;
}

//��������
BOOL CFlowerEffectControl::CreateControl(CWnd *pParent)
{
	return Create(IDD_FLOWER_EFFECT, pParent);
}

//��ʼ����
BOOL CFlowerEffectControl::OnInitDialog()
{
	__super::OnInitDialog();

	return TRUE; 
}

//�滭����
void CFlowerEffectControl::OnPaint()
{
	CPaintDC dc(this); 
}
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

//���캯��
CFlowerEffectThread::CFlowerEffectThread(void)
{
	//��ʼ����
	m_hFlowerEffectControl=NULL;
	m_wFlowerID=0;
}

//��������
CFlowerEffectThread::~CFlowerEffectThread(void)
{
	ConcludeThread(0);
}

//��ʼ�¼�
bool CFlowerEffectThread::OnEventThreadStrat()
{
	return true;
}

//ֹͣ�¼�
bool CFlowerEffectThread::OnEventThreadConclude()
{
	//��ʼ����
	m_hFlowerEffectControl=NULL;
	m_wFlowerID=0;

	return true;
}

//��ʼ����
void CFlowerEffectThread::InitThread(HWND hWnd)
{
	if ( m_hFlowerEffectControl != NULL ) return ;

	//�����ֵ
	m_hFlowerEffectControl = hWnd;

	//���ô�С
	CRect rcFlowerEffectControl;
	::GetWindowRect(m_hFlowerEffectControl, &rcFlowerEffectControl);
	if ( m_ImageBuffer.IsNull() == false )
		m_ImageBuffer.Destroy();
	m_ImageBuffer.Create(rcFlowerEffectControl.Width(),rcFlowerEffectControl.Height(),24);

	//��ȡ����
	CRect ClientRect;
	GetClientRect(m_hFlowerEffectControl, ClientRect);
	CPoint ptLeftTop(ClientRect.left,ClientRect.top),ptRightBottom(ClientRect.right,ClientRect.bottom);
	ClientToScreen(m_hFlowerEffectControl, &ptLeftTop);
	ClientToScreen(m_hFlowerEffectControl, &ptRightBottom);

	//��ȡDC
	if ( m_ImageBufferScreen.IsNull() )
        m_ImageBufferScreen.Create(ClientRect.Width(),ClientRect.Height(),24);
	CDC *pBufferDC=CDC::FromHandle(m_ImageBufferScreen.GetDC());

	//������Ļ
	HWND hScreenWnd= GetDesktopWindow();
	if ( hScreenWnd!=NULL )
	{
		HDC hScreenDC = GetWindowDC(hScreenWnd);

		if ( hScreenDC!=NULL )
		{
			CDC * pDC=CDC::FromHandle(hScreenDC);
			pBufferDC->BitBlt(0,0,ptRightBottom.x-ptLeftTop.x,ptRightBottom.y-ptLeftTop.y,pDC,ptLeftTop.x,ptLeftTop.y,SRCCOPY);

			ReleaseDC(hScreenWnd,hScreenDC);
		}
	}

	//�ͷ�DC
	m_ImageBufferScreen.ReleaseDC();

	return;
}

//��������
bool CFlowerEffectThread::StartFlowerEffect(WORD wFlowerID)
{
	//Ч�����
	ASSERT( wFlowerID < FLOWER_COUNT*2 );
	if ( wFlowerID >= FLOWER_COUNT*2 ) return false;

	m_wFlowerID = wFlowerID;

	//�����߳�
	if ( IsRuning() == false ) StartThread();

	return true;
}

//���к���
bool CFlowerEffectThread::OnEventThreadRun()
{
	//����ͼƬ
	CPngImage ImageFlower;
	CString strInfo=TEXT("GIFT_"),strImage;
	strImage.Format(TEXT("%s%ld"),strInfo,m_wFlowerID/2+1);
	ImageFlower.LoadImage(GetModuleHandle(PROPERTY_MODULE_DLL_NAME),strImage);
	if(ImageFlower.IsNull())return false;

	//�����С
	CRect ClientRect;
	::GetWindowRect(m_hFlowerEffectControl, &ClientRect);

	//�滭��С
	int nCartoonWidth = 150;
	int nCartoonCount = ImageFlower.GetWidth() / nCartoonWidth;

	//����ɢƵ
	MoveWindow(m_hFlowerEffectControl,0,0,0,0,false);
	::ShowWindow(m_hFlowerEffectControl, SW_SHOW);

	//�滭ͼƬ
	for ( int nCartoonIndex=0; nCartoonIndex < nCartoonCount; ++nCartoonIndex )
	{		
		//�豸���
		HDC hDC=GetDC(m_hFlowerEffectControl);
		if ( hDC==NULL ) return false;

		//ϵ��D C
		CDC	TempDC;
		TempDC.Attach(hDC);

		//�ڴ��豸
		CDC *pMemoryDC = CDC::FromHandle(m_ImageBuffer.GetDC());
		CDC *pScreenDC = CDC::FromHandle(m_ImageBufferScreen.GetDC());

		//�滭����
		pMemoryDC->BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),pScreenDC,0,0,SRCCOPY);

		//�滭����
		ImageFlower.DrawImage(pMemoryDC, 0, 0, ClientRect.Width(), ClientRect.Height(),
			nCartoonWidth * nCartoonIndex,0,nCartoonWidth, ImageFlower.GetHeight());

		TempDC.BitBlt(0, 0, ClientRect.Width(), ClientRect.Height(), pMemoryDC, 0, 0, SRCCOPY);

		//��ʾ����
		if (nCartoonIndex==0) MoveWindow(m_hFlowerEffectControl,ClientRect.left,ClientRect.top,ClientRect.Width(),ClientRect.Height(),false);

		//�ͷ��豸
		TempDC.Detach();
		ReleaseDC(m_hFlowerEffectControl, hDC);
		hDC=NULL;
		m_ImageBuffer.ReleaseDC();
		m_ImageBufferScreen.ReleaseDC();
		pMemoryDC=NULL;

		//�߳�˯��
		Sleep(120);
	}

	//���ش���
	::ShowWindow(m_hFlowerEffectControl, SW_HIDE);

	return false;
}

//���к���
bool CFlowerEffectThread::RepetitionRunBMP()
{
	return false;
}

//���к���
bool CFlowerEffectThread::RepetitionRunPNG()
{
	return false;
}

///////////////////////////////////////////////////////////////////////////////////



