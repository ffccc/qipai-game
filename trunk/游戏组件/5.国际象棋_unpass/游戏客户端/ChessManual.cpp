#include "Stdafx.h"
#include "Resource.h"
#include "ChessManual.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CChessManual, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_MANUAL_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_MANUAL_PRESERVE, OnPreserve)
	ON_BN_CLICKED(IDC_MANUAL_RELOAD, OnReLoad)
	ON_BN_CLICKED(IDC_MANUAL_FIRST, OnFirst)
	ON_BN_CLICKED(IDC_MANUAL_BEFORE, OnBefore)
	ON_BN_CLICKED(IDC_MANUAL_NEXT, OnNext)
	ON_BN_CLICKED(IDC_MANUAL_LAST, OnLast)
	ON_LBN_SELCHANGE(IDC_MANUAL_LIST, OnLbnSelchangeManualList)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CChessManual::CChessManual() : CDialog(IDD_MANUAL)
{
	CString strFile,strTemp;
	CTime tmCur = CTime::GetCurrentTime();
	CString strTime = tmCur.Format("%m%d");
	strFile.Format("log\\%sCGameClientDlg.log",strTime);

	strTemp.Format("into CGameFrameDlg()");
	WriteLog(strFile, strTemp);

	//���ñ���
	m_wChessStep=0L;
	m_pDlgGameLogic=NULL;

	//������Դ
	m_ImageBack.SetLoadInfo(IDB_MANUAL_BACK,AfxGetInstanceHandle());

	return;
}

//��������
CChessManual::~CChessManual()
{
}

//�ؼ���
void CChessManual::DoDataExchange(CDataExchange * pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_MANUAL_OPEN, m_btOpen);
	DDX_Control(pDX, IDC_MANUAL_PRESERVE, m_btPreserve);
	DDX_Control(pDX, IDC_MANUAL_RELOAD, m_btReLoad);
	DDX_Control(pDX, IDC_MANUAL_FIRST, m_btFirst);
	DDX_Control(pDX, IDC_MANUAL_BEFORE, m_btBefore);
	DDX_Control(pDX, IDC_MANUAL_NEXT, m_btNext);
	DDX_Control(pDX, IDC_MANUAL_LAST, m_btLast);
	DDX_Control(pDX, IDC_MANUAL_LIST, m_ManualList);
}

//��ʼ������
BOOL CChessManual::OnInitDialog()
{
	__super::OnInitDialog();

	//���ñ���
	SetWindowText(TEXT("�����������ף�"));

	//�ƶ�����
	CImageHandle ImageHandeBack(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	//��ȡ��С
	CRect rcClient;
	GetClientRect(&rcClient);

	//�����ؼ�
	const CSize & BoradSize=m_ChessBorad.GetChessBoradSize();
	m_ChessBorad.Create(NULL,NULL,WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),this,10);

	//���ð�ť
	HINSTANCE hInstance=AfxGetInstanceHandle();
	m_btOpen.SetButtonImage(IDB_MANUAL_BT_OPEN,hInstance,false);
	m_btPreserve.SetButtonImage(IDB_MANUAL_BT_PRESERVE,hInstance,false);
	m_btReLoad.SetButtonImage(IDB_MANUAL_BT_RELOAD,hInstance,false);
	m_btFirst.SetButtonImage(IDB_MANUAL_BT_FIRST,hInstance,false);
	m_btBefore.SetButtonImage(IDB_MANUAL_BT_BEFORE,hInstance,false);
	m_btNext.SetButtonImage(IDB_MANUAL_BT_NEXT,hInstance,false);
	m_btLast.SetButtonImage(IDB_MANUAL_BT_LAST,hInstance,false);
	m_btCancel.SetButtonImage(IDB_MANUAL_BT_CLOSE,hInstance,false);

	//����λ��
	CRect rcButton;
	m_btOpen.GetWindowRect(&rcButton);
	int nXButtonSpace=(rcClient.Width()-rcButton.Width()*8-40)/9;
	int nYPos=rcClient.Height()-rcButton.Height()-30;

	//�ƶ���ť
	HDWP hDwp=BeginDeferWindowPos(8);
	const UINT uFlags=SWP_NOACTIVATE|SWP_NOZORDER|SWP_NOCOPYBITS|SWP_NOSIZE;
	DeferWindowPos(hDwp,m_btOpen,NULL,20+nXButtonSpace,nYPos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btPreserve,NULL,20+nXButtonSpace*2+rcButton.Width(),nYPos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btReLoad,NULL,20+nXButtonSpace*3+rcButton.Width()*2,nYPos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btFirst,NULL,20+nXButtonSpace*4+rcButton.Width()*3,nYPos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btBefore,NULL,20+nXButtonSpace*5+rcButton.Width()*4,nYPos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btNext,NULL,20+nXButtonSpace*6+rcButton.Width()*5,nYPos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btLast,NULL,20+nXButtonSpace*7+rcButton.Width()*6,nYPos,0,0,uFlags);
	DeferWindowPos(hDwp,m_btCancel,NULL,20+nXButtonSpace*8+rcButton.Width()*7,nYPos,0,0,uFlags);
	EndDeferWindowPos(hDwp);

	//�ƶ��ؼ�
	m_ChessBorad.MoveWindow(29,29,BoradSize.cx,BoradSize.cy);
	m_ManualList.MoveWindow(BoradSize.cx+37,32,rcClient.Width()-BoradSize.cx-66,BoradSize.cy-8);

	//��������
	OnReLoad();

	return TRUE;
}

//�ػ�����
void CChessManual::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//�滭����
	CImageHandle ImageHandeBack(&m_ImageBack);
	m_ImageBack.BitBlt(dc,0,0);

	return;
}

//�滭����
BOOL CChessManual::OnEraseBkgnd(CDC * pDC)
{
	Invalidate(FALSE);
	UpdateWindow();
	return TRUE;
}

//ѡ��ı�
void CChessManual::OnLbnSelchangeManualList()
{
	int nSelItem=m_ManualList.GetCurSel();
	if ((nSelItem!=LB_ERR)&&((nSelItem+1)!=m_wChessStep))
	{
		//ǰ���ж�
		if (nSelItem>=m_wChessStep)
		{
			tagChessManual * pChessManual=NULL;
			for (WORD i=m_wChessStep;i<(nSelItem+1);i++)
			{
				pChessManual=&m_ChessManualInfo[i];
				m_GameLogic.MoveChess(pChessManual->cbXSourceChessPos,pChessManual->cbYSourceChessPos,pChessManual->cbXTargetChessPos,
					pChessManual->cbYTargetChessPos,pChessManual->cbSwitchChess);
			}
		}
		else m_GameLogic.RegretChess(m_wChessStep-nSelItem-1);

		//��������
		m_wChessStep=(nSelItem+1);
		m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

		//����ѡ��
		tagChessManual * pChessManual=&m_ChessManualInfo[m_wChessStep-1];
		m_ChessBorad.SetChessFrame(pChessManual->cbXSourceChessPos,pChessManual->cbYSourceChessPos,pChessManual->cbXTargetChessPos,pChessManual->cbYTargetChessPos);

		//���¿���
		UpdateControl();
	}

	return;
}

//���¿���
void CChessManual::UpdateControl()
{
	//�����б�
	m_ManualList.SetCurSel(m_wChessStep-1);

	//���ð�ť
	m_btFirst.EnableWindow(m_wChessStep>0);
	m_btBefore.EnableWindow(m_wChessStep>0);
	m_btNext.EnableWindow(m_wChessStep<m_ChessManualInfo.GetCount());
	m_btLast.EnableWindow(m_wChessStep<m_ChessManualInfo.GetCount());

	return;
}

//��������
void CChessManual::UpdateManualList()
{
	//��ԭ�б�
	while (m_ManualList.GetCount()>0) m_ManualList.DeleteString(0);

	//��������
	LPCTSTR pszChessDes[]={TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("��"),TEXT("ʿ")};

	//���ӽ���
	TCHAR szManualText[128];
	tagChessManual * pChessManual=NULL;
	for (INT_PTR i=0;i<m_ChessManualInfo.GetCount();i++)
	{
		pChessManual=&m_ChessManualInfo[i];
		_snprintf(szManualText,sizeof(szManualText),TEXT("%s%s [ %ld��%ld ]-[ %ld��%ld ]"),
			((i%2)==0)?TEXT("��"):TEXT("��"),pszChessDes[pChessManual->cbSourceChess-1],
			pChessManual->cbXSourceChessPos+1,pChessManual->cbYSourceChessPos+1,
			pChessManual->cbXTargetChessPos+1,pChessManual->cbYTargetChessPos+1);
		m_ManualList.AddString(szManualText);
	}

	return;
}

//�����Ϣ
void CChessManual::OnLButtonDown(UINT nFlags, CPoint point)
{
	__super::OnLButtonDown(nFlags,point);

	//������Ϣ
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));

	return;
}

//�򿪰�ť
void CChessManual::OnOpen()
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("�������������ļ� ��*.CHM��|*.CHM||");
	CFileDialog DlgOpenManual(TRUE,TEXT("CHM"),NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter);
	DlgOpenManual.m_pOFN->lpstrInitialDir=szPath;
	if (DlgOpenManual.DoModal()!=IDOK) return;

	//���ļ�
	CFile FileManual;
	if (FileManual.Open(DlgOpenManual.GetPathName(),CFile::modeRead)==FALSE)
	{
		AfxMessageBox(TEXT("�޷����ļ������״�ʧ�ܣ�"),MB_ICONSTOP);
		return;
	}

	//�ж��ļ�
	DWORD dwFileLength=(DWORD)FileManual.GetLength();
	if (dwFileLength<sizeof(tagChessManualHead))
	{
		AfxMessageBox(TEXT("�����ļ���ʽ�������״�ʧ�ܣ�"),MB_ICONSTOP);
		return;
	}

	//�ļ�ͷ
	tagChessManualHead ChessManualHead;
	FileManual.Read(&ChessManualHead,sizeof(ChessManualHead));

	//Ч��汾
	if (ChessManualHead.wVersion!=VER_MANUAL)
	{
		AfxMessageBox(TEXT("�����ļ��汾�������״�ʧ�ܣ�"),MB_ICONSTOP);
		return;
	}

	//Ч�鳤��
	if ((ChessManualHead.dwManualCount*sizeof(tagChessManual)+sizeof(tagChessManualHead))!=dwFileLength)
	{
		AfxMessageBox(TEXT("�����ļ����ȴ������״�ʧ�ܣ�"),MB_ICONSTOP);
		return;
	}

	//��ȡ����
	tagChessManual * pChessManual=NULL;
	m_ChessManualInfo.SetSize(ChessManualHead.dwManualCount);
	for (DWORD i=0;i<ChessManualHead.dwManualCount;i++)
	{
		pChessManual=&m_ChessManualInfo[i];
		FileManual.Read(pChessManual,sizeof(tagChessManual));
	}

	//�ر��ļ�
	FileManual.Close();

	//���ý���
	m_wChessStep=0;
	m_GameLogic.ResetChessBorad();
	m_ChessBorad.SetChessFrame(255,255,255,255);
	m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);
	m_ChessBorad.SetChessColor(ChessManualHead.cbChessColor);

	//�����б�
	UpdateManualList();

	//���¿���
	UpdateControl();

	return;
}

//���水ť
void CChessManual::OnPreserve()
{
	//��ȡĿ¼
	TCHAR szPath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(sizeof(szPath),szPath);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("�������������ļ� ��*.CHM��|*.CHM||");
	CFileDialog DlgSaveManual(FALSE,TEXT("CHM"),NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter);
	DlgSaveManual.m_pOFN->lpstrInitialDir=szPath;
	if (DlgSaveManual.DoModal()!=IDOK) return;

	//���ļ�
	CFile FileManual;
	if (FileManual.Open(DlgSaveManual.GetPathName(),CFile::modeWrite|CFile::modeCreate)==FALSE)
	{
		AfxMessageBox(TEXT("�޷������ļ������ױ���ʧ�ܣ�"),MB_ICONSTOP);
		return;
	}

	//�ļ�ͷ
	tagChessManualHead ChessManualHead;
	ChessManualHead.wVersion=VER_MANUAL;
	ChessManualHead.dwManualDate=(DWORD)time(NULL);
	ChessManualHead.dwManualCount=(DWORD)m_GameLogic.m_ChessManualInfo.GetCount();

	//д���ļ�ͷ
	FileManual.SeekToBegin();
	FileManual.Write(&ChessManualHead,sizeof(ChessManualHead));

	//��������
	tagChessManual * pChessManual=NULL;
	for (DWORD i=0;i<ChessManualHead.dwManualCount;i++)
	{
		pChessManual=&m_GameLogic.m_ChessManualInfo[i];
		FileManual.Write(pChessManual,sizeof(tagChessManual));
	}

	//�ر��ļ�
	FileManual.SetLength(sizeof(ChessManualHead)+ChessManualHead.dwManualCount*sizeof(tagChessManual));
	FileManual.Close();

	return;
}

//ˢ�°�ť
void CChessManual::OnReLoad()
{
	//��������
	INT_PTR nManualCount=m_pDlgGameLogic->m_ChessManualInfo.GetCount();
	m_ChessManualInfo.SetSize(m_pDlgGameLogic->m_ChessManualInfo.GetCount());
	CopyMemory(m_ChessManualInfo.GetData(),m_pDlgGameLogic->m_ChessManualInfo.GetData(),sizeof(tagChessManual)*nManualCount);

	//���̿���
	m_wChessStep=0;
	m_GameLogic.ResetChessBorad();

	//�����б�
	UpdateManualList();

	//�ߵ����
	if (nManualCount==0)
	{
		//���ý���
		m_ChessBorad.SetChessFrame(255,255,255,255);
		m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

		//���¿���
		UpdateControl();
	}
	else OnLast();

	return;
}

//��ǰ��ť
void CChessManual::OnFirst()
{
	//���ý���
	m_wChessStep=0;
	m_GameLogic.ResetChessBorad();
	m_ChessBorad.SetChessFrame(255,255,255,255);
	m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

	//���¿���
	UpdateControl();

	return;
}

//���ְ�ť
void CChessManual::OnBefore()
{
	//�������
	ASSERT(m_wChessStep>0);
	m_GameLogic.RegretChess(1);

	//���ٲ���
	m_wChessStep--;

	//��������
	m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

	if (m_wChessStep>0)
	{
		tagChessManual * pChessManual=&m_ChessManualInfo[m_wChessStep-1];
		m_ChessBorad.SetChessFrame(pChessManual->cbXSourceChessPos,pChessManual->cbYSourceChessPos,pChessManual->cbXTargetChessPos,pChessManual->cbYTargetChessPos);
	}
	else m_ChessBorad.SetChessFrame(255,255,255,255);

	//���¿���
	UpdateControl();

	return;
}

//���ְ�ť
void CChessManual::OnNext()
{
	//��ȡ����
	ASSERT(m_ChessManualInfo.GetCount()>m_wChessStep);
	tagChessManual * pChessManual=&m_ChessManualInfo[m_wChessStep];
	m_GameLogic.MoveChess(pChessManual->cbXSourceChessPos,pChessManual->cbYSourceChessPos,pChessManual->cbXTargetChessPos,
		pChessManual->cbYTargetChessPos,pChessManual->cbSwitchChess);

	//���Ӳ���
	m_wChessStep++;

	//��������
	m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);
	m_ChessBorad.SetChessFrame(pChessManual->cbXSourceChessPos,pChessManual->cbYSourceChessPos,pChessManual->cbXTargetChessPos,pChessManual->cbYTargetChessPos);

	//���¿���
	UpdateControl();

	return;
}

//���ť
void CChessManual::OnLast()
{
	//��������
	tagChessManual * pChessManual=NULL;
	for (WORD i=m_wChessStep;i<m_ChessManualInfo.GetCount();i++)
	{
		pChessManual=&m_ChessManualInfo[i];
		m_GameLogic.MoveChess(pChessManual->cbXSourceChessPos,pChessManual->cbYSourceChessPos,pChessManual->cbXTargetChessPos,
			pChessManual->cbYTargetChessPos,pChessManual->cbSwitchChess);
	}

	//���Ӳ���
	m_wChessStep=(WORD)m_ChessManualInfo.GetCount();

	//��������
	m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);
	m_ChessBorad.SetChessFrame(pChessManual->cbXSourceChessPos,pChessManual->cbYSourceChessPos,pChessManual->cbXTargetChessPos,pChessManual->cbYTargetChessPos);

	//���¿���
	UpdateControl();

	return;
}

//////////////////////////////////////////////////////////////////////////
