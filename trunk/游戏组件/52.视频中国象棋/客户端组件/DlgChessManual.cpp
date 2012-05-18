#include "StdAfx.h"
#include "GameClient.h"
#include "DlgChessManual.h"

//////////////////////////////////////////////////////////////////////////

//ʱ���ʶ
#define IDI_MANUAL_PALY				100									//���ײ���

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CDlgChessManual, CDialog)

	//ϵͳ��Ϣ
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()

	//���ư�ť
	ON_BN_CLICKED(IDC_HIDE, OnBnClickedHide)
	ON_BN_CLICKED(IDC_MANUAL_VIEW_BT_LOAD, OnBnClickedLoad)
	ON_BN_CLICKED(IDC_MANUAL_VIEW_BT_SAVE, OnBnClickedSave)
	ON_BN_CLICKED(IDC_MANUAL_VIEW_BT_NOTE, OnBnClickedNote)

	//���װ�ť
	ON_BN_CLICKED(IDC_MANUAL_VIEW_BT_HEAD, OnBnClickedHead)
	ON_BN_CLICKED(IDC_MANUAL_VIEW_BT_LAST, OnBnClickedLast)
	ON_BN_CLICKED(IDC_MANUAL_VIEW_BT_PLAY, OnBnClickedPlay)
	ON_BN_CLICKED(IDC_MANUAL_VIEW_BT_NEXT, OnBnClickedNext)
	ON_BN_CLICKED(IDC_MANUAL_VIEW_BT_TAIL, OnBnClickedTail)

	//�ؼ���Ϣ
	ON_LBN_SELCHANGE(IDC_MANUAL_VIEW_LIST, OnLbnSelchangeManualList)

END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CChessManualFile::CChessManualFile()
{
	//���ñ���
	m_wBankerUser=INVALID_CHAIR;
	ZeroMemory(m_szUserAccounts,sizeof(m_szUserAccounts));
	ZeroMemory(&m_ManualDateTime,sizeof(m_ManualDateTime));

	return;
}

//��������
CChessManualFile::~CChessManualFile()
{
}

//��ȡ�û�
TCHAR * CChessManualFile::GetUserAccounts(WORD wChairID)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return NULL;

	return m_szUserAccounts[wChairID];
}

//�����û�
bool CChessManualFile::SetUserAccounts(WORD wChairID, TCHAR * pszAccounts)
{
	//Ч�����
	ASSERT(wChairID<GAME_PLAYER);
	if (wChairID>=GAME_PLAYER) return false;

	//���ñ���
	lstrcpyn(m_szUserAccounts[wChairID],pszAccounts,CountArray(m_szUserAccounts[wChairID]));

	return true;
}

//��������
bool CChessManualFile::LoadChessManual(LPCTSTR pszFileName, CChessManualArray & ChessManualArray)
{
	//���ļ�
	CFile FileManual;
	if (FileManual.Open(pszFileName,CFile::modeRead)==FALSE) return false;

	//�ж��ļ�
	DWORD dwFileLength=(DWORD)FileManual.GetLength();
	if (dwFileLength<sizeof(tagChessManualHead)) return false;

	//��ȡ����
	tagChessManualHead ChessManualHead;
	FileManual.Read(&ChessManualHead,sizeof(ChessManualHead));

	//�汾Ч��
	ASSERT(ChessManualHead.wVersion==VER_MANUAL_FILE);
	if (ChessManualHead.wVersion!=VER_MANUAL_FILE) return false;

	//����Ч��
	ASSERT((ChessManualHead.wItemCount*sizeof(tagChessManual)+sizeof(tagChessManualHead))==dwFileLength);
	if ((ChessManualHead.wItemCount*sizeof(tagChessManual)+sizeof(tagChessManualHead))!=dwFileLength) return false;

	//���ñ���
	m_wBankerUser=ChessManualHead.wBankerUser;
	m_ManualDateTime=ChessManualHead.ManualDateTime;
	lstrcpyn(m_szUserAccounts[0],ChessManualHead.szUserAccounts[0],CountArray(m_szUserAccounts[0]));
	lstrcpyn(m_szUserAccounts[1],ChessManualHead.szUserAccounts[1],CountArray(m_szUserAccounts[1]));

	//��ȡ����
	ChessManualArray.RemoveAll();
	ChessManualArray.SetSize(ChessManualHead.wItemCount);
	FileManual.Read(ChessManualArray.GetData(),ChessManualHead.wItemCount*sizeof(tagChessManual));

	return true;
}

//��������
bool CChessManualFile::SaveChessManual(LPCTSTR pszFileName, CChessManualArray & ChessManualArray)
{
	//Ч�����
	ASSERT(m_wBankerUser<GAME_PLAYER);
	if (m_wBankerUser>=GAME_PLAYER) return false;

	//���ļ�
	CFile FileManual;
	if (FileManual.Open(pszFileName,CFile::modeWrite|CFile::modeCreate)==FALSE) return false;

	//��ȡʱ��
	SYSTEMTIME SystemTime;
	tagChessManualHead ChessManualHead;

	//���ñ���
	GetLocalTime(&SystemTime);
	ZeroMemory(&ChessManualHead,sizeof(ChessManualHead));

	//������Ϣ
	ChessManualHead.wVersion=VER_MANUAL_FILE;
	ChessManualHead.ManualDateTime=SystemTime;
	ChessManualHead.wItemCount=(WORD)ChessManualArray.GetCount();

	//��Ϸ��Ϣ
	ChessManualHead.wBankerUser=m_wBankerUser;
	lstrcpyn(ChessManualHead.szUserAccounts[0],m_szUserAccounts[0],CountArray(ChessManualHead.szUserAccounts[0]));
	lstrcpyn(ChessManualHead.szUserAccounts[1],m_szUserAccounts[1],CountArray(ChessManualHead.szUserAccounts[1]));

	//д���ļ�ͷ
	FileManual.SeekToBegin();
	FileManual.Write(&ChessManualHead,sizeof(ChessManualHead));

	//��������
	tagChessManual * pManualBuffer=ChessManualArray.GetData();
	FileManual.Write(pManualBuffer,ChessManualHead.wItemCount*sizeof(tagChessManual));

	//���ó���
	FileManual.SetLength(sizeof(ChessManualHead)+ChessManualHead.wItemCount*sizeof(tagChessManual));

	return true;
}

//////////////////////////////////////////////////////////////////////////

//���캯��
CDlgChessManual::CDlgChessManual() : CDialog(IDD_CHESS_MANUAL)
{
	//��������
	m_wStepCount=0;
	ZeroMemory(m_ChessItemView,sizeof(m_ChessItemView));
	ZeroMemory(m_ChessBoradView,sizeof(m_ChessBoradView));

	//�������
	m_ChessBorad.SetChessBoradMode(true);

	//������Դ
	m_ImageViewBack.SetLoadInfo(IDB_MANUAL_VIEW_BACK,AfxGetInstanceHandle());

	return;
}

//��������
CDlgChessManual::~CDlgChessManual()
{
}

//�ؼ���
VOID CDlgChessManual::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HIDE, m_btHide);
	DDX_Control(pDX, IDCANCEL, m_btCancel);
	DDX_Control(pDX, IDC_MANUAL_VIEW_LIST, m_ManualList);
	DDX_Control(pDX, IDC_MANUAL_VIEW_BT_LOAD, m_btManualLoad);
	DDX_Control(pDX, IDC_MANUAL_VIEW_BT_SAVE, m_btManualSave);
	DDX_Control(pDX, IDC_MANUAL_VIEW_BT_NOTE, m_btManualNote);
	DDX_Control(pDX, IDC_MANUAL_VIEW_BT_HEAD, m_btManualHead);
	DDX_Control(pDX, IDC_MANUAL_VIEW_BT_LAST, m_btManualLast);
	DDX_Control(pDX, IDC_MANUAL_VIEW_BT_PLAY, m_btManualPlay);
	DDX_Control(pDX, IDC_MANUAL_VIEW_BT_NEXT, m_btManualNext);
	DDX_Control(pDX, IDC_MANUAL_VIEW_BT_TAIL, m_btManualTail);
}

//��Ϣ����
BOOL CDlgChessManual::PreTranslateMessage(MSG * pMsg)
{
	//��ʾ��Ϣ
	if (m_ToolTipCtrl.m_hWnd!=NULL) 
	{
		m_ToolTipCtrl.RelayEvent(pMsg);
	}

	//��������
	if (pMsg->message==WM_KEYDOWN)
	{
		if ((pMsg->wParam==VK_ESCAPE)||(pMsg->wParam==VK_RETURN)) return TRUE;
	}

	return __super::PreTranslateMessage(pMsg);
}

//��ʼ������
BOOL CDlgChessManual::OnInitDialog()
{
	__super::OnInitDialog();

	//���ô���
	SetWindowText(TEXT("��������"));

	//��������
	CRgn RgnImage;
	CImageHandle HandleViewBack(&m_ImageViewBack);
	m_ImageViewBack.CreateImageRegion(RgnImage,RGB(255,0,255));

	//���ô���
	SetWindowRgn(RgnImage,TRUE);
	SetWindowPos(NULL,0,0,m_ImageViewBack.GetWidth(),m_ImageViewBack.GetHeight(),SWP_NOZORDER|SWP_NOMOVE);

	//���ð�ť
	HINSTANCE hResInstance=AfxGetInstanceHandle();
	m_btHide.SetButtonImage(IDB_MANUAL_VIEW_BT_HIDE,hResInstance,false);
	m_btCancel.SetButtonImage(IDB_MANUAL_VIEW_BT_CLOSE,hResInstance,false);
	m_btManualLoad.SetButtonImage(IDB_MANUAL_VIEW_BT_LOAD,hResInstance,false);
	m_btManualSave.SetButtonImage(IDB_MANUAL_VIEW_BT_SAVE,hResInstance,false);
	m_btManualNote.SetButtonImage(IDB_MANUAL_VIEW_BT_NOTE,hResInstance,false);
	m_btManualHead.SetButtonImage(IDB_MANUAL_VIEW_BT_HEAD,hResInstance,false);
	m_btManualLast.SetButtonImage(IDB_MANUAL_VIEW_BT_LAST,hResInstance,false);
	m_btManualPlay.SetButtonImage(IDB_MANUAL_VIEW_BT_PLAY,hResInstance,false);
	m_btManualNext.SetButtonImage(IDB_MANUAL_VIEW_BT_NEXT,hResInstance,false);
	m_btManualTail.SetButtonImage(IDB_MANUAL_VIEW_BT_TAIL,hResInstance,false);

	//��������
	m_wStepCount=0;
	m_GameLogic.ResetChessBorad();
	m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);

	//������ʾ
	m_ToolTipCtrl.Create(this);
	m_ToolTipCtrl.Activate(TRUE);

	//������ʾ
	m_ToolTipCtrl.AddTool(&m_btManualHead,TEXT("��ǰ"));
	m_ToolTipCtrl.AddTool(&m_btManualLast,TEXT("ǰһ��"));
	m_ToolTipCtrl.AddTool(&m_btManualPlay,TEXT("��������"));
	m_ToolTipCtrl.AddTool(&m_btManualNext,TEXT("��һ��"));
	m_ToolTipCtrl.AddTool(&m_btManualTail,TEXT("���"));

	//������ʾ
	m_ToolTipCtrl.AddTool(&m_btHide,TEXT("��С��"));
	m_ToolTipCtrl.AddTool(&m_btCancel,TEXT("�ر�"));
	m_ToolTipCtrl.AddTool(&m_btManualLoad,TEXT("������"));
	m_ToolTipCtrl.AddTool(&m_btManualSave,TEXT("��������"));
	m_ToolTipCtrl.AddTool(&m_btManualNote,TEXT("�༭���׽�˵"));

	//�����ؼ�
	CRect rcClient;
	GetClientRect(&rcClient);
	RectifyGameView(rcClient.Width(),rcClient.Height());

	return FALSE;
}

//ȷ����Ϣ
VOID CDlgChessManual::OnOK()
{
	//���ٴ���
	DestroyWindow();
	
	return;
}

//ȷ����Ϣ
VOID CDlgChessManual::OnCancel()
{
	//���ٴ���
	DestroyWindow();

	return;
}

//��������
bool CDlgChessManual::SetChessManual(CChessManualArray & ChessManualArray, CChessManualFile & ChessManualFile)
{
	//ֹͣ����
	KillTimer(IDI_MANUAL_PALY);
	m_ManualList.EnableWindow(TRUE);

	//��ԭ����
	m_wStepCount=0;
	m_ManualList.ResetContent();
	m_GameLogic.ResetChessBorad();

	//��������
	m_ChessBorad.SetChessBorad(m_GameLogic.m_ChessBorad);
	m_ChessBorad.SelectChessItem(INVALID_POS,INVALID_POS);
	m_ChessBorad.SetMoveChessTrace(INVALID_POS,INVALID_POS,INVALID_POS,INVALID_POS);

	//��������
	for (WORD i=0;i<ChessManualArray.GetCount();i++) 
	{
		//��ȡ����
		tagChessManual * pChessManualItem=&ChessManualArray[i];

		//��������
		BYTE cbXSourcePos=pChessManualItem->cbXSourceChessPos;
		BYTE cbYSourcePos=pChessManualItem->cbYSourceChessPos;
		BYTE cbXTargetPos=pChessManualItem->cbXTargetChessPos;
		BYTE cbYTargetPos=pChessManualItem->cbYTargetChessPos;

		//��������
		TCHAR szManualName[32]=TEXT("");
		tagMoveChessResult MoveChessResult;
		bool bSuccess=m_GameLogic.MoveChess(cbXSourcePos,cbYSourcePos,cbXTargetPos,cbYTargetPos,MoveChessResult,szManualName);

		//���׽���
		if (bSuccess==true) m_ManualList.AddString(szManualName);
	}

	//���ñ���
	if (&m_ChessManualFile!=&ChessManualFile)
	{
		m_ChessManualFile.SetBankerUser(ChessManualFile.GetBankerUser());
		m_ChessManualFile.SetUserAccounts(0,ChessManualFile.GetUserAccounts(0));
		m_ChessManualFile.SetUserAccounts(1,ChessManualFile.GetUserAccounts(1));
		m_ChessManualFile.SetManualDateTime(ChessManualFile.GetManualDateTime());
	}

	//���Ű�ť
	m_btManualPlay.EnableWindow((m_GameLogic.m_ChessManualArray.GetCount()>0)?TRUE:FALSE);

	//���¿���
	UpdateManualControl();

	//���½���
	Invalidate(FALSE);

	return true;
}

//���¿���
VOID CDlgChessManual::UpdateManualControl()
{
	//��������
	WORD wManualCount=(WORD)m_GameLogic.m_ChessManualArray.GetCount();

	//���ð�ť
	m_btManualNote.EnableWindow(m_wStepCount>0);
	m_btManualHead.EnableWindow(m_wStepCount>0);
	m_btManualLast.EnableWindow(m_wStepCount>0);
	m_btManualNext.EnableWindow(m_wStepCount<wManualCount);
	m_btManualTail.EnableWindow(m_wStepCount<wManualCount);

	//�����б�
	m_ManualList.SetCurSel((m_wStepCount>0)?(m_wStepCount-1):LB_ERR);

	return;
}

//���׶�λ
VOID CDlgChessManual::OrientationManual(WORD wStepCount)
{
	//��ʾ����
	if (m_wStepCount!=wStepCount)
	{
		//���ñ���
		m_wStepCount=wStepCount;

		//����״̬
		ZeroMemory(m_ChessItemView,sizeof(m_ChessItemView));
		ZeroMemory(m_ChessBoradView,sizeof(m_ChessBoradView));
		m_GameLogic.GetHistoryStatus(m_ChessItemView,m_ChessBoradView,wStepCount);

		//���ý���
		UpdateManualControl();
		m_ChessBorad.SetChessBorad(m_ChessBoradView);
		m_ChessBorad.SelectChessItem(INVALID_POS,INVALID_POS);

		//���ÿ��
		if (m_wStepCount>0)
		{
			tagChessManual * pChessManual=&m_GameLogic.m_ChessManualArray[wStepCount-1];
			m_ChessBorad.SetMoveChessTrace(pChessManual->cbXSourceChessPos,pChessManual->cbYSourceChessPos,
				pChessManual->cbXTargetChessPos,pChessManual->cbYTargetChessPos);
		}
		else
		{
			m_ChessBorad.SetMoveChessTrace(INVALID_POS,INVALID_POS,INVALID_POS,INVALID_POS);
		}

		//���¿���
		UpdateManualControl();

		//���½���
		Invalidate(FALSE);
	}

	return;
}

//�����ؼ�
VOID CDlgChessManual::RectifyGameView(INT nWidth, INT nHeight)
{
	//���̿ؼ�
	m_ChessBorad.SetBenchmarkPos(16,38);
	m_ManualList.SetWindowPos(NULL,461,175,160,288,SWP_NOZORDER);

	//�رհ�ť
	CRect rcHide;
	m_btHide.GetWindowRect(&rcHide);
	m_btHide.SetWindowPos(NULL,nWidth-rcHide.Width()*2-20,6,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btCancel.SetWindowPos(NULL,nWidth-rcHide.Width()-15,6,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//���ذ�ť
	m_btManualLoad.SetWindowPos(NULL,471,511,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btManualSave.SetWindowPos(NULL,543,511,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btManualNote.SetWindowPos(NULL,370,522,0,0,SWP_NOSIZE|SWP_NOZORDER);

	//���װ�ť
	m_btManualHead.SetWindowPos(NULL,464,481,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btManualLast.SetWindowPos(NULL,464+31,481,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btManualPlay.SetWindowPos(NULL,464+62,481,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btManualNext.SetWindowPos(NULL,464+93,481,0,0,SWP_NOSIZE|SWP_NOZORDER);
	m_btManualTail.SetWindowPos(NULL,464+124,481,0,0,SWP_NOSIZE|SWP_NOZORDER);

	return;
}

//���װ�ť
VOID CDlgChessManual::OnBnClickedHead()
{
	//���̶�λ
	OrientationManual(0);

	return;
}

//���װ�ť
VOID CDlgChessManual::OnBnClickedLast()
{
	//��ȡѡ��
	INT nCurrentSel=m_ManualList.GetCurSel();

	//���̶�λ
	if (nCurrentSel!=LB_ERR) OrientationManual(nCurrentSel);

	return;
}

//���װ�ť
VOID CDlgChessManual::OnBnClickedPlay()
{
	if (m_ManualList.IsWindowEnabled())
	{
		//��λ����
		if (m_wStepCount==m_GameLogic.m_ChessManualArray.GetCount()) OrientationManual(0);

		//��������
		m_ManualList.EnableWindow(FALSE);
		SetTimer(IDI_MANUAL_PALY,1000,NULL);
	}
	else
	{
		//ֹͣ����
		KillTimer(IDI_MANUAL_PALY);
		m_ManualList.EnableWindow(TRUE);
	}

	return;
}

//���װ�ť
VOID CDlgChessManual::OnBnClickedNext()
{
	//��ȡѡ��
	INT nCurrentSel=m_ManualList.GetCurSel();

	//���̶�λ
	OrientationManual((nCurrentSel==LB_ERR)?1:(nCurrentSel+2));

	return;
}

//���װ�ť
VOID CDlgChessManual::OnBnClickedTail()
{
	//���̶�λ
	OrientationManual(m_ManualList.GetCount());

	return;
}

//���ذ�ť
VOID CDlgChessManual::OnBnClickedHide()
{
	//���ش���
	ShowWindow(SW_MINIMIZE);

	return;
}

//�򿪰�ť
VOID CDlgChessManual::OnBnClickedLoad()
{
	//��ȡĿ¼
	TCHAR szFilePath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szFilePath),szFilePath);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("�й��������� ��*.CML��|*.CML||");
	CFileDialog DlgLoadManual(TRUE,TEXT("CML"),NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter,this);
	DlgLoadManual.m_pOFN->lpstrInitialDir=szFilePath;

	//ѡ���ļ�
	if (DlgLoadManual.DoModal()==IDOK)
	{
		//������
		CChessManualArray ChessManualArray;
		bool bSuccess=m_ChessManualFile.LoadChessManual(DlgLoadManual.GetPathName(),ChessManualArray);

		//�������
		if (bSuccess==false)
		{
			//CInformation Information(this);
			//Information.ShowMessageBox(TEXT("����"),TEXT("�й����塰���ס��ļ�����ʧ�ܣ�"),MB_OK|MB_ICONERROR);
		}
		else
		{
			m_btManualSave.EnableWindow(TRUE);
			SetChessManual(ChessManualArray,m_ChessManualFile);
		}
	}

	return;
}

//���水ť
VOID CDlgChessManual::OnBnClickedSave()
{
	//��ȡĿ¼
	TCHAR szFilePath[MAX_PATH]=TEXT("");
	GetCurrentDirectory(CountArray(szFilePath),szFilePath);

	//ѡ���ļ�
	LPCTSTR pszFilter=TEXT("�й��������� ��*.CML��|*.CML||");
	CFileDialog DlgSaveManual(FALSE,TEXT("CML"),NULL,OFN_NOCHANGEDIR|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,pszFilter,this);
	DlgSaveManual.m_pOFN->lpstrInitialDir=szFilePath;

	//ѡ���ļ�
	if (DlgSaveManual.DoModal()==IDOK)
	{
		//��������
		bool bSuccess=m_ChessManualFile.SaveChessManual(DlgSaveManual.GetPathName(),m_GameLogic.m_ChessManualArray);

		//�������
		if (bSuccess==false)
		{
			//CInformation Information(this);
			//Information.ShowMessageBox(TEXT("����"),TEXT("�й����塰���ס��ļ�����ʧ�ܣ�"),MB_OK|MB_ICONERROR);
		}
	}

	return;
}

//��˵��ť
VOID CDlgChessManual::OnBnClickedNote()
{
	//��ʾ��Ϣ
	//CInformation Information(this);
	//Information.ShowMessageBox(TEXT("��ʾ"),TEXT("�����Ǵ򿪡����׽�˵�༭�����İ�ť�����ǽ���δ���������汾���ṩ�˹��ܣ�"));

	return;
}

//�ػ�����
VOID CDlgChessManual::OnPaint()
{
	CPaintDC dc(this);

	//��ȡλ��
	CRect rcClient;
	GetClientRect(&rcClient);

	//��������
	CDC DCBuffer;
	CBitmap ImageBuffer;
	DCBuffer.CreateCompatibleDC(&dc);
	ImageBuffer.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	//���� DC
	DCBuffer.SetBkMode(TRANSPARENT);
	DCBuffer.SetTextColor(RGB(64,0,0));
	DCBuffer.SelectObject(&ImageBuffer);
	DCBuffer.SelectObject(CSkinResourceManager::GetDefaultFont());

	//�滭����
	CImageHandle HandleViewBack(&m_ImageViewBack);
	m_ImageViewBack.BitBlt(DCBuffer,0,0);

	//�滭����
	m_ChessBorad.DrawChessBorad(&DCBuffer);

	//�滭��Ϣ
	if (m_ChessManualFile.GetBankerUser()!=INVALID_CHAIR)
	{
		//����λ��
		CRect rcString;
		rcString.SetRect(470,55,617,67);

		//����ʱ��
		TCHAR szManualDate[64]=TEXT("");
		TCHAR szManualTime[64]=TEXT("");
		SYSTEMTIME SystemTime=m_ChessManualFile.GetManualDateTime();
		_sntprintf(szManualDate,CountChar(szManualDate),TEXT("���ڣ�%02d �� %02d ��"),SystemTime.wMonth,SystemTime.wDay);
		_sntprintf(szManualTime,CountChar(szManualTime),TEXT("ʱ�䣺%02d ʱ %02d ��"),SystemTime.wHour,SystemTime.wMinute);

		//�Ծ��û�
		TCHAR szAccounts[2][LEN_ACCOUNTS];
		WORD wBankerUser=m_ChessManualFile.GetBankerUser();
		CopyMemory(szAccounts[0],m_ChessManualFile.GetUserAccounts(wBankerUser),LEN_ACCOUNTS);
		CopyMemory(szAccounts[1],m_ChessManualFile.GetUserAccounts((wBankerUser+1)%GAME_PLAYER),LEN_ACCOUNTS);
		//CW2CT strAccounts1(m_ChessManualFile.GetUserAccounts(wBankerUser));
		//CW2CT strAccounts2(m_ChessManualFile.GetUserAccounts((wBankerUser+1)%GAME_PLAYER));
		//_sntprintf(szAccounts[0],CountChar(szAccounts[0]),TEXT("���壺%s"),(LPCTSTR)strAccounts1);
		//_sntprintf(szAccounts[1],CountChar(szAccounts[1]),TEXT("���壺%s"),(LPCTSTR)strAccounts2);

		//�Ծ�����
		DCBuffer.DrawText(szManualDate,lstrlen(szManualDate),rcString,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//�Ծ�ʱ��
		rcString.OffsetRect(0,25);
		DCBuffer.DrawText(szManualTime,lstrlen(szManualTime),rcString,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//�Ծ��û�
		rcString.OffsetRect(0,25);
		DCBuffer.DrawText(szAccounts[0],lstrlen(szAccounts[0]),rcString,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

		//�Ծ��û�
		rcString.OffsetRect(0,25);
		DCBuffer.DrawText(szAccounts[1],lstrlen(szAccounts[1]),rcString,DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	}

	//�滭����
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&DCBuffer,0,0,SRCCOPY);

	//������Դ
	DCBuffer.DeleteDC();
	ImageBuffer.DeleteObject();

	return;
}

//�滭����
BOOL CDlgChessManual::OnEraseBkgnd(CDC * pDC)
{
	//���´���
	Invalidate(FALSE);
	UpdateWindow();

	return TRUE;
}

//ʱ����Ϣ
VOID CDlgChessManual::OnTimer(UINT nIDEvent)
{
	//��������
	if (nIDEvent==IDI_MANUAL_PALY)
	{
		//��ȡ��Ŀ
		WORD wManualCount=(WORD)m_GameLogic.m_ChessManualArray.GetCount();

		//���ſ���
		if (m_wStepCount>=wManualCount)
		{
			KillTimer(IDI_MANUAL_PALY);
			m_ManualList.EnableWindow(TRUE);
		}
		else OrientationManual(m_wStepCount+1);

		return;
	}

	__super::OnTimer(nIDEvent);
}

//ѡ��ı�
VOID CDlgChessManual::OnLbnSelchangeManualList()
{
	//��ȡѡ��
	INT nCurrentSel=m_ManualList.GetCurSel();

	//���̶�λ
	OrientationManual(nCurrentSel+1);

	return;
}

//�����Ϣ
VOID CDlgChessManual::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//////////////////////////////////////////////////////////////////////////
