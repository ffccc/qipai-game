#include "StdAfx.h"
#include "CallScoreInfo.h"

//////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CCallScoreInfo, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////

//���캯��
CCallScoreInfo::CCallScoreInfo()
{
	
	//��������
	ZeroMemory(m_lScore,sizeof(m_lScore));
	ZeroMemory(m_lColor,sizeof(m_lColor));
	ZeroMemory(m_szUserName,sizeof(m_szUserName));
	
	//������Դ
	m_ImageBack.SetLoadInfo(IDB_CALL_SCORE,AfxGetInstanceHandle());
	m_ImageColorFlag.SetLoadInfo(IDB_COLOR_FLAG,AfxGetInstanceHandle());

	return;
}

//��������
CCallScoreInfo::~CCallScoreInfo()
{
}

//������Ϣ
int CCallScoreInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (__super::OnCreate(lpCreateStruct)==-1) return -1;

	//���ñ���
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	//�ƶ�����
	CImageHandle ImageHandle(&m_ImageBack);
	SetWindowPos(NULL,0,0,m_ImageBack.GetWidth(),m_ImageBack.GetHeight(),SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

//�����Ϣ
void CCallScoreInfo::OnLButtonDown(UINT nFlags, CPoint Point)
{
	__super::OnLButtonDown(nFlags,Point);

	//��Ϣģ��
	PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(Point.x,Point.y));

	return;
}

//�ػ�����
void CCallScoreInfo::OnPaint() 
{
	CPaintDC dc(this); 

	//������Դ
	CImageHandle HandleBack(&m_ImageBack);
	
	//�滭����
	m_ImageBack.BitBlt(dc,0,0);


	//���� DC
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(0,0,160));
	dc.SelectObject(&CSkinAttribute::m_DefaultFont);

	BYTE bNum=0;
	
	//��ʾ��Ϣ
	TCHAR szBuffer[128]=TEXT("");
	//��ʾ����
	for (WORD i=0;i<CountArray(m_szUserName);i++)
	{
		if (m_szUserName[i][0]!=0)
		{
			WORD k=i;
			//if (i==1) k=3;
			//if (i==3) k=1;
			//�������
			CRect rcName(k*40+10,10,k*40+50,60);
			dc.DrawText(m_szUserName[i],lstrlen(m_szUserName[i]),&rcName,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
			
			//�������
			for (WORD j=0;j<60;j++)
			{
				if(m_lScore[i][j]!=0) 
				{
					CRect rcScore(k*40+2,(j+1)*19+10,k*40+50,(j+1)*19+60);
					switch (m_lScore[i][j])
					{
					case 200:
						{
							_snprintf(szBuffer,sizeof(szBuffer)," X");
							break;
						}
						
					case 210:
						{
							_snprintf(szBuffer,sizeof(szBuffer)," XX");
							break;
						}
					case 240:
						{
							_snprintf(szBuffer,sizeof(szBuffer)," --");
							break;
						}
					default:
						{
							if (m_lScore[i][j]!=250)
							{
								_snprintf(szBuffer,sizeof(szBuffer),TEXT("%ld"),m_lScore[i][j]);
							}
							else
							{
								_snprintf(szBuffer,sizeof(szBuffer)," PASS");
							}
							break;
						}
					}
					dc.DrawText(szBuffer,lstrlen(szBuffer),&rcScore,DT_CENTER|DT_VCENTER|DT_SINGLELINE|DT_END_ELLIPSIS);
					
					//255���� 200�ӱ� 210�ټӱ�
					if (m_lColor[i][j]!=255&&m_lColor[i][j]!=200&&m_lColor[i][j]!=210&&m_lColor[i][j]!=240)
					{
						//������Դ
						CImageHandle HandleColorFlag(&m_ImageColorFlag);
						int nColorImageWidth=m_ImageColorFlag.GetWidth()/6;
						int nColorImageHeight=m_ImageColorFlag.GetHeight();

						//�滭��־
						int nXPos=k*40+30;
						int nYPos=(j+1)*19+27;
						m_ImageColorFlag.AlphaDrawImage(&dc,nXPos,nYPos,nColorImageWidth,nColorImageHeight,nColorImageWidth*(m_lColor[i][j]>>4),0,RGB(255,0,255));
					}
				
				}
			}
		}
	}

	return;
}

//���û���
void CCallScoreInfo::SetCallGameInfo(WORD wChairID, BYTE bColor,BYTE bScore)
{
	if (wChairID==INVALID_CHAIR)
	{
		for(WORD i=0;i<4;i++)
		{
			for (WORD j=0;j<60;j++)
			{
				m_lScore[i][j]=0;
				m_lColor[i][j]=255;
			}
		}
	}
	else
	{
		for(WORD i=0;i<4;i++)
		{
			if (i==wChairID)
			{
				for (WORD j=0;j<60;j++)
				{
					if(m_lScore[i][j]==0) 
					{
						m_lScore[i][j]=bScore;
						m_lColor[i][j]=bColor;
						break;
					}
				}
			}
		}
	}
		
	//ˢ�½���
	Invalidate(FALSE);
	return;
}

//�����û�����
void CCallScoreInfo::SetCallUserName(WORD wChairID,LPCTSTR pszUserName)
{
	lstrcpyn(m_szUserName[wChairID],pszUserName,CountArray(m_szUserName[wChairID]));
	//ˢ�½���
	Invalidate(FALSE);
	return;
}

//////////////////////////////////////////////////////////////////////////
