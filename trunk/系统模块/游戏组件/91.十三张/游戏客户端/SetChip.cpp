#include "Stdafx.h"
#include "SetChip.h"
#include ".\setchip.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CSetChip, CSkinDialogEx)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CSetChip::CSetChip() : CSkinDialogEx(IDD_SETCHIP)
{
	m_nChip	= 0 ;
	return;
}

//��������
CSetChip::~CSetChip()
{
}

//�ؼ���
void CSetChip::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//��ʼ������
BOOL CSetChip::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("ѹע"));

	//����Ѻע
	LONG lChipsArray[SET_CHIP_COUNT] ;

	//������֤
	ASSERT( m_wMaxChip > 0 ) ;
	if ( m_wMaxChip <= 0 ) return TRUE ;

	for ( BYTE cbChipIndex = 0; cbChipIndex < SET_CHIP_COUNT; ++cbChipIndex )
	{
		lChipsArray[cbChipIndex] = LONG ( ( cbChipIndex + 1.0 ) / SET_CHIP_COUNT * m_wMaxChip ) ;

		//���ٴ���
		if ( lChipsArray[cbChipIndex] > 100 )
		{
			LONG lHundredCount = lChipsArray[cbChipIndex] / 100 ;
			lChipsArray[cbChipIndex] = lHundredCount * 100 ;
		}
	}

	//����Ѻע
	CComboBox *pChipComBox = ( CComboBox* ) GetDlgItem( IDC_SETCHIP ) ;
	CString strChip ;

	for ( BYTE cbChipIndex = 0; cbChipIndex < SET_CHIP_COUNT; ++cbChipIndex )
	{
		strChip.Format( TEXT( "%ld" ), lChipsArray[cbChipIndex] ) ;
		pChipComBox->InsertString( cbChipIndex, strChip ) ;
	}
	pChipComBox->SetCurSel( 0 ) ;	

	pChipComBox->SetFocus() ;

	m_nChip = lChipsArray[0] ;

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CSetChip::OnBnClickedOk()
{
	//��ȡ����
	UpdateData(TRUE) ;

	//�õ�Ѻע
	CComboBox *pChipComBox = ( CComboBox* ) GetDlgItem( IDC_SETCHIP ) ;
	CString strChip ;

	int nCurSel = pChipComBox->GetCurSel() ;
	pChipComBox->GetLBText( nCurSel, strChip ) ;

	m_nChip = atoi( strChip ) ;

	OnOK();
}

void CSetChip::OnBnClickedCancel()
{
	OnCancel() ;
}


//��ʼѺע
void CSetChip::SetMaxChips( LONG lMaxChip ) 
{
	m_wMaxChip = lMaxChip ;
}
