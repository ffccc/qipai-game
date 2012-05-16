#include "stdafx.h"
#include "resource.h"
#include "PropertyItem.h"
#include ".\propertyitem.h"
///////////////////////////////////////////////////////////////////////////////////

//��ʱ��I D
#define IDI_PRO_ITEM_UPDATE_VIEW	1								//���½���
#define TIME_PRO_ITEM_UPDATE_VIEW	200								//���½���

//�궨��
#define DEF_INSIDEBORDER_COLOR	RGB(176,20,1)						//Ĭ����ɫ
#define BGROUND_COLOR			RGB(254,250,221)					//������ɫ

///////////////////////////////////////////////////////////////////////////////////

//��Ϣӳ��
BEGIN_MESSAGE_MAP(CPropertyItem, CSkinDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_CBN_SELCHANGE(IDC_TARGET_USER, OnCbnSelchangeTargetUser)
	ON_CBN_SELCHANGE(IDC_PROPERTY_TYPE, OnCbnSelchangePropType)
	ON_EN_CHANGE(IDC_PURCHASE_COUNT, OnEnChangePurchaseCount)
	ON_WM_TIMER()	
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////

//��Ϣӳ��
BEGIN_MESSAGE_MAP(CFlowerSetting, CSkinDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()	
	ON_EN_CHANGE(IDC_FLOWER_COUNT, OnEnChangeFlowerCount)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CPropertyItem::CPropertyItem(CWnd* pParent): CSkinDialogEx(IDD_PROPERTY_ITEM, pParent)
{
	m_nPropertyID=PROPERTY_COUNT;
	m_pIPurchaseInfo=NULL;
	m_nPropCount=1;
	m_nPropertyType=0;
}

//��������
CPropertyItem::~CPropertyItem()
{
}

//�ؼ���
void CPropertyItem::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_TARGET_USER, m_ComboBoxTargetUser);
	DDX_Control(pDX, IDC_PROPERTY_TYPE, m_ComboBoxPropType);
	DDX_Text(pDX, IDC_NORMAL_USER_PRICE, m_strNormalUserPrice);
	DDX_Text(pDX, IDC_MENBER_USER_PRICE, m_strMemberUserPrice);
	DDX_Text(pDX, IDC_GOLD_DESCRIBE, m_strGoldDescribe);
	DDX_Text(pDX, IDC_RESIDUAL_TIME, m_strProContent);
}

//���õ���
void CPropertyItem::SetPropertyID(int nPropertyID)
{
	//�Ϸ��ж�
	ASSERT(nPropertyID <PROPERTY_COUNT );
	if ( PROPERTY_COUNT <= nPropertyID ) return;

	//����I D
	m_bPlazaRoom = true;
	m_nPropertyID = nPropertyID;

	//ɾ����Ϣ
	while ( 0 < m_ComboBoxTargetUser.GetCount() ) m_ComboBoxTargetUser.DeleteString(0);
	while ( 0 < m_ComboBoxPropType.GetCount() ) m_ComboBoxPropType.DeleteString(0);

	//�������
	WORD wMeItem = 0;
	for ( WORD wChairID = 0; wChairID < MAX_CHAIR; ++wChairID )
	{
		const tagUserData *pUserData = m_pIPurchaseInfo->GetUserInfo(wChairID);
		if ( pUserData == NULL ) continue;
		if(m_bPlazaRoom)m_bPlazaRoom = false;

		int nIndex = m_ComboBoxTargetUser.AddString(pUserData->szName);

		if ( pUserData->dwUserID == m_pIPurchaseInfo->GetMeUserInfo()->dwUserID ) wMeItem = nIndex;
		m_ComboBoxTargetUser.SetItemData(nIndex, pUserData->dwUserID);
	}

	//�ؼ��ж�
	GetDlgItem(IDC_PRO_USER_ID)->ShowWindow((!m_bPlazaRoom)?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_TARGET_USER)->ShowWindow((m_bPlazaRoom)?SW_HIDE:SW_SHOW);
	if(m_bPlazaRoom)
	{
		GetDlgItem(IDC_PRO_USER_ID)->SetWindowText(TEXT("���������Ͷ�����û���"));
		GetDlgItem(IDC_PRO_USER_ID)->SetFocus();
		((CEdit*)GetDlgItem(IDC_PRO_USER_ID))->SetSel(0,-1);
	}

	if(!m_bPlazaRoom)
	{
		//�����Թ�
		WORD wIndex = 0;
		tagUserData *pLookonUserdata = NULL;
		do
		{
			pLookonUserdata = m_pIPurchaseInfo->EnumLookonUser(wIndex++);
			if ( pLookonUserdata == NULL ) break;

			int nItem = m_ComboBoxTargetUser.AddString(pLookonUserdata->szName);

			if ( pLookonUserdata->dwUserID == m_pIPurchaseInfo->GetMeUserInfo()->dwUserID ) wMeItem = nItem;
			m_ComboBoxTargetUser.SetItemData(nItem, pLookonUserdata->dwUserID);

		}while(pLookonUserdata != NULL);

		m_ComboBoxTargetUser.SetCurSel(wMeItem);
	}

	//���ñ���
	tagPropertyInfo const &PropertyInfo = CPropertyBar::m_PropertyInfo[m_nPropertyID];
	DWORD dwPropertyCount[] = {PropertyInfo.dwPropCount1, PropertyInfo.dwPropCount2, PropertyInfo.dwPropCount3, PropertyInfo.dwPropCount4,
		PropertyInfo.dwPropCount5, PropertyInfo.dwPropCount6, PropertyInfo.dwPropCount7, PropertyInfo.dwPropCount8, PropertyInfo.dwPropCount9, PropertyInfo.dwPropCount10};

	LONG lPropertyPrice[] = {PropertyInfo.lPrice1, PropertyInfo.lPrice2, PropertyInfo.lPrice3, PropertyInfo.lPrice4,PropertyInfo.lPrice5,
		PropertyInfo.lPrice6, PropertyInfo.lPrice7, PropertyInfo.lPrice8, PropertyInfo.lPrice9, PropertyInfo.lPrice10};


	//��������
	CString strPropType;
	m_ComboBoxPropType.EnableWindow(TRUE);
	if ( m_nPropertyID != PROP_FLEE && m_nPropertyID != PROP_NEGAGIVE && m_nPropertyID != PROP_BUGLE )
	{
		//���ÿؼ�		
		DWORD dwDays=0, dwHours=0, dwMinutes=0, dwSeconds=0;
		for (int nIndex = 0; nIndex < CountArray(dwPropertyCount); ++nIndex )
		{
			if ( 0 < lPropertyPrice[nIndex] && 0 < dwPropertyCount[nIndex] )
			{
				dwDays = CalcDays(dwPropertyCount[nIndex]);
				dwHours = CalcHours(dwPropertyCount[nIndex]);
				dwMinutes = CalcMinutes(dwPropertyCount[nIndex]);
				dwSeconds = CalcSeconds(dwPropertyCount[nIndex]);
				if ( 0 != dwDays ) strPropType.Format(TEXT("%ld��"), dwDays);
				else if ( 0 != dwHours ) strPropType.Format(TEXT("%ldСʱ"), dwHours);
				else if ( 0 != dwMinutes ) strPropType.Format(TEXT("%ld����"), dwMinutes);
				else if ( 0 != dwSeconds ) strPropType.Format(TEXT("%ld��"), dwSeconds);
				else
				{
					ASSERT(FALSE);
					return;
				}

				//������Ϣ
				int nItem = m_ComboBoxPropType.InsertString(-1, strPropType);
				m_ComboBoxPropType.SetItemData(nItem, dwPropertyCount[nIndex]);
			}
		}
		m_ComboBoxPropType.SetCurSel(0);
	}
	else if ( m_nPropertyID == PROP_BUGLE )
	{
		for (int nIndex = 0; nIndex < CountArray(dwPropertyCount); ++nIndex )
		{
			if ( 0 < lPropertyPrice[nIndex] && 0 < dwPropertyCount[nIndex] )
			{
				strPropType.Format(TEXT("%ld��"), dwPropertyCount[nIndex]);

				//������Ϣ
				int nItem = m_ComboBoxPropType.InsertString(-1, strPropType);
				m_ComboBoxPropType.SetItemData(nItem, dwPropertyCount[nIndex]);
			}
		}
		m_ComboBoxPropType.SetCurSel(0);
	}
	else m_ComboBoxPropType.EnableWindow(FALSE); //����������

	//��������
	if(m_ComboBoxPropType.IsWindowEnabled()==TRUE)OnCbnSelchangePropType();

	//������Ŀ
	m_nPropCount = 1;
	GetDlgItem(IDC_PURCHASE_COUNT)->SetWindowText(TEXT("1"));

	//���½���
	CRect Rect(10,38,427,151);
	InvalidateRect(Rect);

	return;
}

//�滭����
void CPropertyItem::OnPaint()
{
	CPaintDC dc(this); 

	//�滭����
	DrawCaption(&dc);

	//��ȡλ��
	CRect ClientRect;
	GetClientRect(&ClientRect);
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();

	//�滭����
	COLORREF Color = RGB(161,212,226);
	dc.FillSolidRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,4,Color);
	dc.FillSolidRect(nXExcursion,ClientRect.Height()-8,ClientRect.Width()-2*nXExcursion,7,Color);
	dc.FillSolidRect(nXExcursion,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,Color);
	dc.FillSolidRect(ClientRect.Width()-nXExcursion-12,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,Color);

	//�滭�߿�
	DrawBorder(&dc);

	//����ͼƬ
	CBitmap Bitmap;

	//����ͼƬ
	AfxSetResourceHandle(CPropertyBar::m_pPropertyBar->m_ReshInstance);
	if ( Bitmap.LoadBitmap(CPropertyBar::m_pPropertyBar->m_PropertyViewImage.uPropertyFormer[m_nPropertyID]) )
	{
		//������Դ
		CDC dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap *pOldBitmap = dcMem.SelectObject(&Bitmap);

		//�滭ͼƬ
		BITMAP bm;
		Bitmap.GetBitmap(&bm);
		dc.TransparentBlt(10, 28+5, bm.bmWidth, bm.bmHeight, &dcMem, 0, 0, bm.bmWidth, bm.bmHeight,RGB(255,255,255)); 
		
		//�Ƿ���Դ
		dcMem.SelectObject(pOldBitmap);
		Bitmap.DeleteObject();
		dcMem.DeleteDC();
	}
	AfxSetResourceHandle(GetModuleHandle(NULL));

	//��ȡλ��
	CRect rcStaticName, rcStaticDes;
	GetDlgItem(IDC_PROPERTY_NAME)->GetWindowRect(rcStaticName);
	GetDlgItem(IDC_PROPERTY_DESCRIBE)->GetWindowRect(rcStaticDes);
	ScreenToClient(&rcStaticName);
	ScreenToClient(&rcStaticDes);

	//��������
	CRect rcPropertyName(rcStaticName.right+3, rcStaticName.top, rcStaticName.right+200, rcStaticName.bottom+100);
	CRect rcPropertyDes(rcStaticDes.right+3, rcStaticDes.top, rcStaticDes.right+200, rcStaticDes.bottom+70);
	if(m_nPropertyID>=PROP_MEMBER_1)rcPropertyDes.right+=30;

	//��������
	CFont font;
	VERIFY(font.CreateFont(
		15,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial"));                 // lpszFacename

	//��������
	dc.SetBkMode(TRANSPARENT);
	CFont* def_font = dc.SelectObject(&font);
	
	//������Ϣ
	UINT nFormat = DT_LEFT | DT_TOP | DT_WORDBREAK;
	dc.DrawText(g_PropertyDescribe[m_nPropertyID].szName, rcPropertyName, nFormat);
	dc.DrawText(g_PropertyDescribe[m_nPropertyID].szDescribe, rcPropertyDes, nFormat);

	//ɾ����Դ
	dc.SelectObject(def_font);
	font.DeleteObject();
}

//��ʼ����
BOOL CPropertyItem::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("����"));

	//��ʱ����
	SetTimer(IDI_PRO_ITEM_UPDATE_VIEW,TIME_PRO_ITEM_UPDATE_VIEW,NULL);

	return TRUE; 
}

//�������
void CPropertyItem::SetPurchaseInfoSink(IPurchaseInfo *pIPurchaseInfo)
{
	m_pIPurchaseInfo = pIPurchaseInfo;

	return;
}

//ȷ�ϵ���
void CPropertyItem::OnBnClickedOk()
{
	//�Ϸ��ж�
	if ( m_nPropCount <= 0 || m_nPropCount > MAX_PROPERTY_COUNT )
	{
		CString strError;
		strError.Format(TEXT("������Ŀ�������0��С�ڵ���%d"), MAX_PROPERTY_COUNT);
		ShowInformationEx(strError,0,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
		GetDlgItem(IDC_PURCHASE_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_PURCHASE_COUNT))->SetSel(0,-1);

		return ;
	}

	//�����û�
	DWORD dwTargetUserID=0;
	tagUserData *pTargetUserData=NULL;
	CString strTargetName;
	if(!m_bPlazaRoom)
	{
		int nSelectItem = m_ComboBoxTargetUser.GetCurSel();
		if ( CB_ERR == nSelectItem ) return;

		//��ȡ��Ϣ
		dwTargetUserID = DWORD(m_ComboBoxTargetUser.GetItemData(nSelectItem));
		m_ComboBoxTargetUser.GetLBText(nSelectItem, strTargetName);
		pTargetUserData = m_pIPurchaseInfo->SearchUserItem(dwTargetUserID);
	}
	else
	{
		//��ȡ��Ϣ
		GetDlgItem(IDC_PRO_USER_ID)->GetWindowText(strTargetName);
		pTargetUserData = m_pIPurchaseInfo->SearchUserItem(strTargetName);
		if (pTargetUserData!=NULL)dwTargetUserID=pTargetUserData->dwUserID;
	}

	//�ж�����
	if ( pTargetUserData == NULL )
	{
		CString strMessage;
		strMessage.Format(TEXT("[ % ]�Ѿ��뿪��������ѡ��"), strTargetName);
		ShowInformation(strMessage,0,MB_ICONINFORMATION);
		return;
	}

	//�����ж�
	WORD wGameGenre= m_pIPurchaseInfo->GetGameGenre();

	if ( wGameGenre == GAME_GENRE_GOLD && ( m_nPropertyID==PROP_NEGAGIVE || m_nPropertyID == PROP_DOUBLE || m_nPropertyID == PROP_FOURDOLD ))
	{
		ShowInformation(TEXT("��Ϸ�ҷ��䲻����ʹ�ô˹��ܣ�"),0,MB_ICONINFORMATION);
		return ;
	}
	if ( wGameGenre == GAME_GENRE_MATCH && ( m_nPropertyID==PROP_NEGAGIVE || m_nPropertyID == PROP_DOUBLE ||
		m_nPropertyID == PROP_FOURDOLD || m_nPropertyID == PROP_BUGLE ))
	{
		ShowInformation(TEXT("�������䲻����ʹ�ô˹��ܣ�"),0,MB_ICONINFORMATION);
		return ;
	}

	//��ȡ���
	const tagUserData *pMeUserData = m_pIPurchaseInfo->GetMeUserInfo();

	//��ȡ�۸�
	LONG lNormalPrice = 0, lMemberPrice = 0;
	GetPropertyPrice(lNormalPrice, lMemberPrice);
	
	//�жϽ��
	LONG lMePrice = pMeUserData->cbMemberOrder ? lMemberPrice : lNormalPrice;
	LONG lMeScore = pMeUserData->lInsureScore;
	if ( lMeScore < lMePrice )
	{
		if ( IDYES == ShowInformation(TEXT("������Ϸ�Ҳ��㣬�Ƿ��ֵ��"),0,MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2) )	
		{
			ShellExecute(NULL,TEXT("open"),TEXT("http://221.234.25.72"),NULL,NULL,SW_SHOWDEFAULT);
		}
		
		return;
	}

	//��������
	CMD_GF_Property PropertyHeadInfo;
	ZeroMemory(&PropertyHeadInfo, sizeof(PropertyHeadInfo));
	PropertyHeadInfo.cbSendLocation = m_pIPurchaseInfo->GetLocation();
	PropertyHeadInfo.nPropertyID = m_nPropertyID;
	PropertyHeadInfo.dwSourceUserID = pMeUserData->dwUserID;
	PropertyHeadInfo.dwTargetUserID = dwTargetUserID;
	PropertyHeadInfo.dwPachurseCount = m_nPropCount;
	PropertyHeadInfo.dwOnceCount = (DWORD)m_nPropertyType;

	//��������
	m_pIPurchaseInfo->SendData(MDM_GF_PRESENT, SUB_GF_PROPERTY, &PropertyHeadInfo, sizeof(PropertyHeadInfo));

	OnOK();
	return;
}

//��ʾ��Ϣ
void CPropertyItem::ShowMessage(TCHAR *pszMessage)
{
	ShowInformationEx(pszMessage,0,MB_ICONINFORMATION,TEXT("����"));

	return;
}

//������ɫ
HBRUSH CPropertyItem::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSkinDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor == CTLCOLOR_EDIT )
	{
		pDC->SetTextColor(RGB(0,0,0));
	}
	return hbr;
}

//���¿ؼ�
void CPropertyItem::UpdateView()
{
	//���±���
	UpdateData(TRUE);

	if ( PROPERTY_COUNT <= m_nPropertyID ) return;

	//��ȡ����
	const tagUserData *pMeUserData = m_pIPurchaseInfo->GetMeUserInfo();
	DWORD dwResidualTime=pMeUserData->dwPropResidualTime[m_nPropertyID];

	//��ʾ����
	CString strResidualTime;
	CString strText=(m_nPropertyID==PROP_BUGLE)?(TEXT("��������   ")):(TEXT("��Ч�ڣ�   "));
	if ( dwResidualTime > 0 )
	{
		if ( m_nPropertyID != PROP_BUGLE )
		{
			DWORD dwDays = CalcDays(dwResidualTime);
			DWORD dwHours = CalcHours(dwResidualTime);
			DWORD dwMinutes = CalcMinutes(dwResidualTime);
			DWORD dwSeconds = CalcSeconds(dwResidualTime);
			CString strTmp;
			if ( dwDays != 0 )
			{
				strTmp.Format(TEXT("%ld��"), dwDays);
				strResidualTime += strTmp;
			}
			if ( dwHours != 0 )
			{
				strTmp.Format(TEXT("%ldСʱ"), dwHours);
				strResidualTime += strTmp;
			}
			if ( dwMinutes != 0 )
			{
				strTmp.Format(TEXT("%ld����"), dwMinutes);
				strResidualTime += strTmp;
			}
			if ( dwSeconds != 0 )
			{
				strTmp.Format(TEXT("%ld��"), dwSeconds);
				strResidualTime += strTmp;
			}
		}
		else strResidualTime.Format(TEXT("%ld"), dwResidualTime);
	}
	else strResidualTime = TEXT("δ����");
	strText+=strResidualTime;
	m_strProContent.Format(strText);

	//��������
	LONG lNormalPrice = 0;
	LONG lMemberPrice = 0;

	//��ȡ�۸�
	GetPropertyPrice(lNormalPrice, lMemberPrice);

	m_strNormalUserPrice.Format(TEXT("��ͨ�û���֧����Ϸ�ң�%ld"), lNormalPrice);
	m_strMemberUserPrice.Format(TEXT("��Ա�û���֧����Ϸ�ң�%ld"), lMemberPrice);	

	LONG lMePrice = pMeUserData->cbMemberOrder ? lMemberPrice : lNormalPrice;
	LONG lMeScore = pMeUserData->lInsureScore;
	if ( lMeScore < lMePrice )
		m_strGoldDescribe.Format(TEXT("�������д�%ld�����ȴ����ٹ���"), lMeScore);
	else
        m_strGoldDescribe.Format(TEXT("�������д�%ld��������ߺ󣬴��ʣ�ࣺ%ld"), lMeScore, lMeScore-lMePrice);

	//���±���
	UpdateData(FALSE);

	return;
}

//ѡ���û�
void CPropertyItem::SetUserDataInfo(tagUserData *pUserData)
{
	if(pUserData==NULL)return;

	//���±���
	UpdateData(TRUE);

	if(m_ComboBoxTargetUser.IsWindowVisible()==TRUE)
	{
		//�������
		INT nFineIndex=CB_ERR;
		for(INT i=0;i<m_ComboBoxTargetUser.GetCount();i++)
		{
			CString csUserName;
			m_ComboBoxTargetUser.GetLBText(i,csUserName);
			if(csUserName==pUserData->szName)
			{
				nFineIndex = i;
				break;
			}
		}

		//�������
		if(nFineIndex==CB_ERR)
		{
			nFineIndex = m_ComboBoxTargetUser.AddString(pUserData->szName);
			m_ComboBoxTargetUser.SetItemData(nFineIndex, pUserData->dwUserID);
		}

		//��ǰѡ��
		if(nFineIndex == m_ComboBoxTargetUser.GetCurSel()) return;

		//�������
		m_ComboBoxTargetUser.SetCurSel(nFineIndex);
	}
	else
	{
		GetDlgItem(IDC_PRO_USER_ID)->SetWindowText(pUserData->szName);
	}

	//���±���
	UpdateData(FALSE);
	return;
}

//ѡ����Ϣ
void CPropertyItem::OnCbnSelchangeTargetUser()
{
	//��ǰѡ��
	int nSelectItem = m_ComboBoxTargetUser.GetCurSel();
	if ( CB_ERR == nSelectItem ) return;

	//��ȡ���
	DWORD dwTargetUserID = DWORD(m_ComboBoxTargetUser.GetItemData(nSelectItem));
	tagUserData *pTargetUserData = m_pIPurchaseInfo->SearchUserItem(dwTargetUserID);

	//�뿪�ж�
	if ( pTargetUserData == NULL )
	{
		//ɾ�����
		m_ComboBoxTargetUser.DeleteString(nSelectItem);

		//��ʾ��Ϣ
		CString strMessage, strTargetName;
		m_ComboBoxTargetUser.GetLBText(nSelectItem, strTargetName);
		strMessage.Format(TEXT("[ % ]�Ѿ��뿪��������ѡ��"), strTargetName);
		ShowInformation(strMessage,0,MB_ICONINFORMATION);
		return;
	}

	return;
}

//��ť��Ϣ
void CPropertyItem::OnCbnSelchangePropType()
{
	//��������
	int nItem = m_ComboBoxPropType.GetCurSel();
	m_nPropertyType = (int)m_ComboBoxPropType.GetItemData(nItem);

	return;
}

//������Ϣ
void CPropertyItem::OnEnChangePurchaseCount()
{
	CString strCount;
	GetDlgItem(IDC_PURCHASE_COUNT)->GetWindowText(strCount);
	m_nPropCount=_tstoi(strCount);

	//�Ϸ��ж�
	if ( m_nPropCount <= 0 || m_nPropCount > MAX_PROPERTY_COUNT )
	{
		CString strError;
		strError.Format(TEXT("������Ŀ�������0��С�ڵ���%d"), MAX_PROPERTY_COUNT);
		ShowInformationEx(strError,0,MB_ICONINFORMATION,TEXT("ϵͳ��Ϣ"));
		SetDlgItemInt(IDC_PURCHASE_COUNT,1);
		GetDlgItem(IDC_PURCHASE_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_PURCHASE_COUNT))->SetSel(0,-1);

		return ;
	}

	return;
}

//ʱ����Ϣ
void CPropertyItem::OnTimer(UINT nIDEvent)
{
	//ʱ����Ϣ
	switch(nIDEvent)
	{
	case IDI_PRO_ITEM_UPDATE_VIEW:		//���½���
		{
			//���½���
			UpdateView();

			return;
		}
	}

	CSkinDialogEx::OnTimer(nIDEvent);
}

//������Ϣ
BOOL CPropertyItem::PreTranslateMessage(MSG* pMsg)
{
	//������ĸ
	if(GetFocus()==GetDlgItem(IDC_PURCHASE_COUNT) && pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��������
DWORD CPropertyItem::CalcDays(DWORD dwTime)
{
	DWORD dwDays = 0;
	dwDays = dwTime / 86400;

	return dwDays;	
}

//����Сʱ��
DWORD CPropertyItem::CalcHours(DWORD dwTime)
{
	DWORD dwDays = 0, dwHours = 0;
	dwDays = dwTime / 86400;
	dwHours = (dwTime - dwDays * 86400 ) / 3600;
	 
	return dwHours;	
}

//���������
DWORD CPropertyItem::CalcMinutes(DWORD dwTime)
{
	DWORD dwDays = 0, dwHours = 0, dwMinutes = 0;
	dwDays = dwTime / 86400;
	dwHours = (dwTime - dwDays * 86400 ) / 3600;
	dwMinutes = (dwTime - dwDays * 86400 - dwHours * 3600 ) / 60;
	 
	return dwMinutes;
}

//��������
DWORD CPropertyItem::CalcSeconds(DWORD dwTime)
{
	DWORD dwDays = 0, dwHours = 0, dwMinutes = 0, dwSeconds = 0;
	dwDays = dwTime / 86400;
	dwHours = (dwTime - dwDays * 86400 ) / 3600;
	dwMinutes = (dwTime - dwDays * 86400 - dwHours * 3600 ) / 60;
	dwSeconds = dwTime - dwDays * 86400 - dwHours * 3600 - dwMinutes * 60;
	 
	return dwSeconds;
}

//��ȡ�۸�
void CPropertyItem::GetPropertyPrice(LONG &lNormalPrice, LONG &lMemberPrice)
{
	//��ȡ���
	const tagUserData *pMeUserData = m_pIPurchaseInfo->GetMeUserInfo();

	//���ñ���
	tagPropertyInfo const &PropertyInfo = CPropertyBar::m_PropertyInfo[m_nPropertyID];
	DWORD dwPropertyCount[] = {PropertyInfo.dwPropCount1, PropertyInfo.dwPropCount2, PropertyInfo.dwPropCount3, PropertyInfo.dwPropCount4,
		PropertyInfo.dwPropCount5, PropertyInfo.dwPropCount6, PropertyInfo.dwPropCount7, PropertyInfo.dwPropCount8, PropertyInfo.dwPropCount9, PropertyInfo.dwPropCount10};

	LONG lPropertyPrice[] = {PropertyInfo.lPrice1, PropertyInfo.lPrice2, PropertyInfo.lPrice3, PropertyInfo.lPrice4,PropertyInfo.lPrice5,
		PropertyInfo.lPrice6, PropertyInfo.lPrice7, PropertyInfo.lPrice8, PropertyInfo.lPrice9, PropertyInfo.lPrice10};

	//��ʼ����
	lNormalPrice = lPropertyPrice[0];
	lMemberPrice = lPropertyPrice[0];

	//���ܸ���
	if ( m_nPropertyID != PROP_FLEE && m_nPropertyID != PROP_NEGAGIVE )
	{
		for ( int nIndex = 0; nIndex < CountArray(dwPropertyCount); ++nIndex )
		{
			if ( dwPropertyCount[nIndex] == m_nPropertyType )
			{
				lNormalPrice = m_nPropCount * lPropertyPrice[nIndex];
				lMemberPrice = LONG(lNormalPrice * (CPropertyBar::m_PropertyInfo[m_nPropertyID].cbDiscount / 100.));
				break;
			}
		}
	}
	else
	{
		lNormalPrice = lPropertyPrice[0] * m_nPropCount;
		lMemberPrice = LONG(lNormalPrice * (CPropertyBar::m_PropertyInfo[m_nPropertyID].cbDiscount / 100.));
	}

	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////

//���캯��
CFlowerSetting::CFlowerSetting(CWnd* pParent): CSkinDialogEx(IDD_PROPERTY_ITEM, pParent)
{
	m_nFlowerID=FLOWER_COUNT;
	m_pIPurchaseInfo=NULL;
	m_nFlowerCount=1;
	//dwTargetUserID=0;
}

//��������
CFlowerSetting::~CFlowerSetting()
{
}

//�󶨺���
void CFlowerSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TARGET_USER, m_ComboBoxTargetUser);
	DDX_Text(pDX, IDC_NORMAL_USER_PRICE, m_strNormalUserPrice);
	DDX_Text(pDX, IDC_MENBER_USER_PRICE, m_strMemberUserPrice);
	DDX_Text(pDX, IDC_GOLD_DESCRIBE, m_strGoldDescribe);
	DDX_Control(pDX, IDOK, m_btOK);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
}

//�������
void CFlowerSetting::SetPurchaseInfoSink(IPurchaseInfo *pIPurchaseInfo)
{
	m_pIPurchaseInfo = pIPurchaseInfo;

	return;
}

//����ID
void CFlowerSetting::SetFlowerID(int nFlowerID) 
{
	//�Ϸ��ж�
	ASSERT(nFlowerID <FLOWER_COUNT );
	if ( FLOWER_COUNT <= nFlowerID ) return;

	//����I D
	m_bPlazaRoom = true;
	m_nFlowerID = nFlowerID;

	//ɾ����Ϣ
	while ( 0 < m_ComboBoxTargetUser.GetCount() ) m_ComboBoxTargetUser.DeleteString(0);

	//���ñ���
	CString strTitle;
	strTitle.Format(TEXT("����[ %s ]"), g_FlowerDescribe[m_nFlowerID].szName);
	SetWindowText(strTitle);

	//�ӿ��ж�
	ASSERT( m_pIPurchaseInfo != NULL );
	if ( m_pIPurchaseInfo == NULL ) return ;

	//�������
	int nTargetItem = 0;
	for ( WORD wChairID = 0; wChairID < MAX_CHAIR; ++wChairID )
	{
		const tagUserData *pUserData = m_pIPurchaseInfo->GetUserInfo(wChairID);
		if ( pUserData == NULL ) continue;
		if(m_bPlazaRoom)m_bPlazaRoom = false;

		int nIndex = m_ComboBoxTargetUser.AddString(pUserData->szName);
		m_ComboBoxTargetUser.SetItemData(nIndex, pUserData->dwUserID);
		if ( pUserData->dwUserID == m_pIPurchaseInfo->GetMeUserInfo()->dwUserID ) nTargetItem = nIndex;
	}

	if(!m_bPlazaRoom)
	{
		//�����Թ�
		WORD wIndex = 0;
		tagUserData *pLookonUserdata = NULL;
		do
		{
			pLookonUserdata = m_pIPurchaseInfo->EnumLookonUser(wIndex++);
			if ( pLookonUserdata == NULL ) break;

			int nItem = m_ComboBoxTargetUser.AddString(pLookonUserdata->szName);

			if ( pLookonUserdata->dwUserID == m_pIPurchaseInfo->GetMeUserInfo()->dwUserID ) nTargetItem = nItem;
			m_ComboBoxTargetUser.SetItemData(nItem, pLookonUserdata->dwUserID);

		}while(pLookonUserdata != NULL);
		m_ComboBoxTargetUser.SetCurSel(nTargetItem);
	}

	GetDlgItem(IDC_FLOWER_COUNT)->SetFocus();

	//�ؼ��ж�
	GetDlgItem(IDC_PRO_USER_ID)->ShowWindow((!m_bPlazaRoom)?SW_HIDE:SW_SHOW);
	GetDlgItem(IDC_TARGET_USER)->ShowWindow((m_bPlazaRoom)?SW_HIDE:SW_SHOW);
	if(m_bPlazaRoom)
	{
		GetDlgItem(IDC_PRO_USER_ID)->SetWindowText(TEXT("���������Ͷ�����û���"));
		GetDlgItem(IDC_PRO_USER_ID)->SetFocus();
		((CEdit*)GetDlgItem(IDC_PRO_USER_ID))->SetSel(0,-1);
	}

	//������Ŀ
	m_nFlowerCount=1;
	GetDlgItem(IDC_FLOWER_COUNT)->SetWindowText(TEXT("1"));

	//���±���
	CRect Rect(0,0,150,30);
	InvalidateRect(Rect);

	return;
}

//��ť��Ϣ
void CFlowerSetting::OnBnClickedOk()
{
	//�Ϸ��ж�
	if ( m_nFlowerCount <= 0 || m_nFlowerCount > MAX_FLOWER_COUNT )
	{
		CString strError;
		strError.Format(TEXT("������Ŀ�������0��С�ڵ���%d"), MAX_FLOWER_COUNT);
		ShowInformation(strError,0,MB_ICONINFORMATION);
		GetDlgItem(IDC_FLOWER_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_FLOWER_COUNT))->SetSel(0,-1);
		return ;
	}

	//�����û�
	DWORD dwTargetUserID=0;
	tagUserData *pSelectedUserData =NULL;
	if(!m_bPlazaRoom)
	{
		int nSelectedItem = m_ComboBoxTargetUser.GetCurSel();
		if ( CB_ERR != nSelectedItem )
		{
			dwTargetUserID = (DWORD)m_ComboBoxTargetUser.GetItemData(nSelectedItem);
			tagUserData *pTargetUserData = m_pIPurchaseInfo->SearchUserItem(dwTargetUserID);
			if ( pTargetUserData == NULL )
			{
				m_ComboBoxTargetUser.DeleteString(nSelectedItem);
				ShowInformation(TEXT("��Ҫ���͵�����Ѿ��뿪��������ѡ���������"),0,MB_ICONINFORMATION);
				m_ComboBoxTargetUser.SetFocus();
				return;
			}
		}
		else
		{
			ShowInformation(TEXT("��ѡ���������"),0,MB_ICONINFORMATION);
			m_ComboBoxTargetUser.SetFocus();
		}
		pSelectedUserData = m_pIPurchaseInfo->SearchUserItem(dwTargetUserID) ;
	}
	else
	{
		//��ȡ��Ϣ
		CString strTargetName;
		GetDlgItem(IDC_PRO_USER_ID)->GetWindowText(strTargetName);
		pSelectedUserData = m_pIPurchaseInfo->SearchUserItem(strTargetName);
		if (pSelectedUserData!=NULL)dwTargetUserID=pSelectedUserData->dwUserID;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////
	//�����ж�
	if ( pSelectedUserData == NULL )
	{
		ShowInformation(TEXT("�Է��Ѿ��뿪������ʧ�ܣ�"),0,MB_ICONINFORMATION);
		return ;
	}

	//��ȡ���
	const tagUserData *pMeUserData = m_pIPurchaseInfo->GetMeUserInfo();
	
	//������Ϣ
	LONG lNormalPrice = CPropertyBar::m_FlowerInfo[m_nFlowerID].lPrice * m_nFlowerCount;
	LONG lMemberPrice = LONG(lNormalPrice * (CPropertyBar::m_FlowerInfo[m_nFlowerID].cbDiscount / 100.));

	LONG lMePrice = pMeUserData->cbMemberOrder ? lMemberPrice : lNormalPrice;
	LONG lMeScore = pMeUserData->lInsureScore;
	if ( lMeScore < lMePrice )
	{
		if ( IDYES == ShowInformation(TEXT("������Ϸ�Ҳ��㣬�Ƿ��ֵ��"),0,MB_ICONQUESTION |MB_YESNO|MB_DEFBUTTON2))	
		{
			ShellExecute(NULL,TEXT("open"),TEXT("http://221.234.25.72"),NULL,NULL,SW_SHOWDEFAULT);
		}

		return;
	}

	//��������
	CMD_GF_Gift Gift;
	ZeroMemory( &Gift, sizeof( Gift ) );
	Gift.cbSendLocation = m_pIPurchaseInfo->GetLocation();
	Gift.dwRcvUserID = pSelectedUserData->dwUserID;
	Gift.dwSendUserID = m_pIPurchaseInfo->GetMeUserInfo()->dwUserID;
	Gift.wFlowerCount = m_nFlowerCount;
	Gift.wGiftID = m_nFlowerID;

	//������Ϣ
	m_pIPurchaseInfo->SendData( MDM_GF_PRESENT,SUB_GF_FLOWER,&Gift, sizeof(Gift));

	OnOK();
	return;
}

//�ر���Ϣ
void CFlowerSetting::OnBnClickedCancel()
{
	OnCancel();
	return;
}

//��ʼ����
BOOL CFlowerSetting::OnInitDialog()
{
	CSkinDialogEx::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("����"));

	//��ʱ����
	SetTimer(IDI_PRO_ITEM_UPDATE_VIEW,TIME_PRO_ITEM_UPDATE_VIEW,NULL);

	return TRUE; 
}

//������ɫ
HBRUSH CFlowerSetting::OnCtlColor(CDC * pDC, CWnd * pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CSkinDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor == CTLCOLOR_EDIT )
	{
		pDC->SetTextColor(RGB(0,0,0));
	}
	return hbr;
}

//���½���
void CFlowerSetting::UpdateView()
{
	//���±���
	UpdateData(TRUE);

	//�Ϸ��ж�
	if ( FLOWER_COUNT <= m_nFlowerID ) return ;

	//��ȡ���
	const tagUserData *pMeUserData = m_pIPurchaseInfo->GetMeUserInfo();
	
	//������Ϣ
	LONG lNormalPrice = CPropertyBar::m_FlowerInfo[m_nFlowerID].lPrice * m_nFlowerCount;
	LONG lMemberPrice = LONG(lNormalPrice * (CPropertyBar::m_FlowerInfo[m_nFlowerID].cbDiscount / 100.));
	m_strNormalUserPrice.Format(TEXT("��ͨ�û���֧����Ϸ�ң�%ld"), lNormalPrice);
	m_strMemberUserPrice.Format(TEXT("��Ա�û���֧����Ϸ�ң�%ld"), lMemberPrice);	
	
	LONG lMePrice = pMeUserData->cbMemberOrder ? lMemberPrice : lNormalPrice;
	LONG lMeScore = pMeUserData->lInsureScore;
	if ( lMeScore < lMePrice )
		m_strGoldDescribe.Format(TEXT("�������д�%ld�����ȴ����ٹ���"), lMeScore);
	else
		m_strGoldDescribe.Format(TEXT("�������д�%ld��������ߺ󣬴��ʣ�ࣺ%ld"), lMeScore, lMeScore-lMePrice);

	//���±���
	UpdateData(FALSE);
}

//ѡ���û�
void CFlowerSetting::SetUserDataInfo(tagUserData *pUserData)
{
	if(pUserData==NULL)return;

	//���±���
	UpdateData(TRUE);

	if(m_ComboBoxTargetUser.IsWindowVisible()==TRUE)
	{
		//�������
		INT nFineIndex=CB_ERR;
		for(INT i=0;i<m_ComboBoxTargetUser.GetCount();i++)
		{
			CString csUserName;
			m_ComboBoxTargetUser.GetLBText(i,csUserName);
			if(csUserName==pUserData->szName)
			{
				nFineIndex = i;
				break;
			}
		}

		//�������
		if(nFineIndex==CB_ERR)
		{
			nFineIndex = m_ComboBoxTargetUser.AddString(pUserData->szName);
			m_ComboBoxTargetUser.SetItemData(nFineIndex, pUserData->dwUserID);
		}

		//��ǰѡ��
		if(nFineIndex == m_ComboBoxTargetUser.GetCurSel()) return;

		//�������
		m_ComboBoxTargetUser.SetCurSel(nFineIndex);
	}
	else
	{
		GetDlgItem(IDC_PRO_USER_ID)->SetWindowText(pUserData->szName);
	}

	//���±���
	UpdateData(FALSE);
	return;
}

//�滭����
void CFlowerSetting::OnPaint()
{
	CPaintDC dc(this); 

	//�滭����
	DrawCaption(&dc);

	CRect ClientRect;
	GetClientRect(&ClientRect);
	int nXExcursion=GetXExcursionPos();
	int nYExcursion=GetYExcursionPos();

	//�滭����
	COLORREF Color = RGB(161,212,226);
	dc.FillSolidRect(nXExcursion,nYExcursion,ClientRect.Width()-2*nXExcursion,4,Color);
	dc.FillSolidRect(nXExcursion,ClientRect.Height()-8,ClientRect.Width()-2*nXExcursion,7,Color);
	dc.FillSolidRect(nXExcursion,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,Color);
	dc.FillSolidRect(ClientRect.Width()-nXExcursion-12,nYExcursion+4,12,ClientRect.Height()-nYExcursion-9,Color);

	//�滭�߿�
	DrawBorder(&dc);
}

//ʱ����Ϣ
void CFlowerSetting::OnTimer(UINT nIDEvent)
{
	//ʱ����Ϣ
	switch(nIDEvent)
	{
	case IDI_PRO_ITEM_UPDATE_VIEW:		//���½���
		{
			//���½���
			UpdateView();

			return;
		}
	}

	CSkinDialogEx::OnTimer(nIDEvent);
}

//������Ϣ
BOOL CFlowerSetting::PreTranslateMessage(MSG* pMsg)
{
	//������ĸ
	if(GetFocus()==GetDlgItem(IDC_FLOWER_COUNT) && pMsg->message==WM_CHAR)
	{
		BYTE bMesValue = (BYTE)(pMsg->wParam);
		BYTE bTemp = bMesValue-'0';
		if((bTemp<0 || bTemp>9) && bMesValue!=VK_BACK) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//������Ϣ
void CFlowerSetting::OnEnChangeFlowerCount()
{
	CString strCount;
	GetDlgItem(IDC_FLOWER_COUNT)->GetWindowText(strCount);
	m_nFlowerCount=_tstoi(strCount);

	//�Ϸ��ж�
	if ( m_nFlowerCount <= 0 || m_nFlowerCount > MAX_FLOWER_COUNT )
	{
		CString strError;
		strError.Format(TEXT("������Ŀ�������0��С�ڵ���%d"), MAX_FLOWER_COUNT);
		ShowInformation(strError,0,MB_ICONINFORMATION);
		SetDlgItemInt(IDC_FLOWER_COUNT,1);
		GetDlgItem(IDC_FLOWER_COUNT)->SetFocus();
		((CEdit*)GetDlgItem(IDC_FLOWER_COUNT))->SetSel(0,-1);

		return ;
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
