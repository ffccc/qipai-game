#include "StdAfx.h"
#include "WndValidateCode.h"

//////////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CWndValidateCode, CStatic)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////

//���캯��
CWndValidateCode::CWndValidateCode()
{
	//���ñ���
	m_bValidateCode=false;
	ZeroMemory(m_szValidateCode,sizeof(m_szValidateCode));

	return;
}

//��������
CWndValidateCode::~CWndValidateCode()
{
}

//������֤
VOID CWndValidateCode::RandValidateCode()
{
	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//������֤
	for (INT i=0;i<CountArray(m_szValidateCode);i++)
	{
		m_szValidateCode[i]=TEXT('0')+rand()%10;
	}

	//����ͼƬ
	CreateValidateCode(m_ImageValidateCode,rcClient.Width(),rcClient.Height());

	//���½���
	RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);

	return;
}

//Ч����֤
bool CWndValidateCode::CheckValidateCode(LPCTSTR pszValidateCode)
{
	//������֤
	if (pszValidateCode==NULL) return false;
	if (lstrlen(pszValidateCode)!=VALIDATE_COUNT) return false;

	//�ַ��Ա�
	for (BYTE i=0;i<VALIDATE_COUNT;i++)
	{
		if (pszValidateCode[i]!=m_szValidateCode[i])
		{
			return false;
		}
	}

	return true;
}

//����ͼƬ
VOID CWndValidateCode::CreateValidateCode(CImage & ImageValidateCode, INT nWidth, INT nHeight)
{
	//����ͼƬ
	if (ImageValidateCode.IsNull()==false)
	{
		ImageValidateCode.Destroy();
	}

	//����ͼƬ
	ASSERT((nWidth>0)&&((nHeight>0)));
	ImageValidateCode.Create(nWidth,nHeight,32);

	//���� DC
	CImageDC BufferDC(ImageValidateCode);
	CDC * pBufferDC=CDC::FromHandle(BufferDC);

	//��䱳��
	pBufferDC->FillSolidRect(1,1,nWidth-2,nHeight-2,RGB(255,255,255));
	pBufferDC->Draw3dRect(0,0,nWidth,nHeight,RGB(255,255,255),RGB(125,125,125));

	//�������
	for (INT nXPos=1;nXPos<nWidth-1;nXPos+=2)
	{
		for (INT nYPos=1;nYPos<nHeight-1;nYPos+=2)
		{
			pBufferDC->SetPixel(nXPos,nYPos,RGB(rand()%255,rand()%255,rand()%255));
		}
	}

	//���� DC
	pBufferDC->SetBkMode(TRANSPARENT);
	pBufferDC->SetTextColor(RGB(0,0,0));

	//��������
	INT nXSpace=nWidth/CountArray(m_szValidateCode);

	//��ʾ����
	for (BYTE i=0;i<CountArray(m_szValidateCode);i++)
	{
		//λ�ö���
		CRect rcDraw;
		rcDraw.SetRect(i*nXSpace,0,(i+1)*nXSpace,nHeight);

		//��������
		CFont DrawFont;
		DrawFont.CreateFont(-14,0,0,0,800,0,0,0,134,3,2,ANTIALIASED_QUALITY,2,TEXT("����"));

		//��ʾ����
		pBufferDC->SelectObject(DrawFont);
		pBufferDC->DrawText(&m_szValidateCode[i],1,&rcDraw,DT_SINGLELINE|DT_VCENTER|DT_CENTER);

		//������Դ
		pBufferDC->SelectObject(CSkinResourceManager::GetInstance()->GetDefaultFont());
	}

	return;
}

//�ػ���Ϣ
VOID CWndValidateCode::OnPaint()
{
	//��������
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	if (m_ImageValidateCode.IsNull()==false)
	{
		//���� DC
		CImageDC BufferDC(m_ImageValidateCode);
		CDC * pBufferDC=CDC::FromHandle(BufferDC);

		//�滭����
		m_ImageValidateCode.BitBlt(dc,0,0,rcClient.Width(),rcClient.Height(),0,0,SRCCOPY);
	}
	else
	{
		//Ĭ�ϱ���
		dc.FillSolidRect(&rcClient,CSkinDialog::m_SkinAttribute.m_crBackGround);
	}

	return;
}

//λ����Ϣ
VOID CWndValidateCode::OnSize(UINT nType, INT cx, INT cy) 
{
	__super::OnSize(nType, cx, cy);

	//�����ؼ�
	if (m_bValidateCode==true)
	{
		//��ȡλ��
		CRect rcClient;
		GetClientRect(&rcClient);

		//����ͼƬ
		CreateValidateCode(m_ImageValidateCode,rcClient.Width(),rcClient.Height());

		//���½���
		RedrawWindow(NULL,NULL,RDW_INVALIDATE|RDW_UPDATENOW|RDW_ERASE|RDW_ERASENOW);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////
