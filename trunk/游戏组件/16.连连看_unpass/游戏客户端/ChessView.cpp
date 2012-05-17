// ChessView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "ChessView.h"
#include ".\chessview.h"

// CChessView

CChessView::CChessView()
{
	InitChessView();
}

CChessView::~CChessView()
{
	//�ͷ�����ͼ����Դ
}


BEGIN_MESSAGE_MAP(CChessView, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CChessView ��Ϣ�������


int CChessView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	SetClassLong(m_hWnd,GCL_HBRBACKGROUND,NULL);

	HINSTANCE hInstance=AfxGetInstanceHandle();

	//�ƶ�����
	SetWindowPos(NULL,0,0,100,100,SWP_NOMOVE|SWP_NOZORDER);

	return 0;
}

void CChessView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	return;
}


void CChessView::OnLButtonDown(UINT nFlags, CPoint point)
{

}

void CChessView::InitChessView(void)
{
	_snprintf(m_per,sizeof(m_per),TEXT(""));
	_snprintf(m_class,sizeof(m_class),TEXT(""));

	m_ExerciseMode = false;
	m_isLose = false;
	m_first = true;
	m_score = 0;
	m_Iper = 0;
	m_StartPos = ERROR_POINT;
	m_EndPos = ERROR_POINT;
	m_result = ERROR_POINT;
	m_Bobm = false;
	_snprintf(m_scorestring,sizeof(m_scorestring),TEXT("�÷�:0"));

	m_BakeHit = false;
	m_BakeHitCount = 0;

	for(int i = 0; i<82; i++)
	{
		memset(&m_Chess[i],0,sizeof(m_Chess[i]));
	}

	for(int i=0; i<82; i++)
	{
		m_Result[i] = 0;
		m_tResult[i] = 0;
	}

	for(int i = 0; i<3; i++)
	{
		m_nextball[i] = 0;
        memset(&m_nextballcolor[i],0,sizeof(m_nextballcolor[i]));
		m_tnextball[i] = 0;
		memset(&m_tnextballcolor[i],0,sizeof(m_tnextballcolor[i]));
	}

	for(int i=0; i<5; i++)
	{
		m_firstball[i] = 0;
		memset(&m_firstballcolor[i],0,sizeof(m_firstballcolor[i]));
	}
	for(int i=0; i<9; i++)
	{
		memset(&m_DeadBall[i],0,sizeof(m_DeadBall[i]));
	}
	

}
//����
void CChessView::TakeTheService()
{
	int i=3;
	
	srand(time(NULL));

	//��һ�η���Ҫ����5��С��
	if(m_first)
	{
		i = 5;
		while(i)
		{
			int temp;
			int colorkind;
			colorkind = rand()%7+1;
			temp = rand()%80+1;
			if(m_Chess[temp].color==0)
			{
				m_firstball[i-1] = temp;
				m_firstballcolor[i-1].color = colorkind;

				m_Chess[temp].color = colorkind;
				//�Ƿ�ɹ���ȥ
				int t = m_Logic.IsSuccess(m_Chess,m_SuccessArray,temp);
				if(t)
				{
					if(!m_ExerciseMode)
					{
						::AfxGetMainWnd()->SendMessage(ID_SUCCESS,temp,t);
						::SendMessage(this->GetParent()->m_hWnd,ID_PSUCCESS,temp,t);
					}
					else
					{
						::SendMessage(this->GetParent()->m_hWnd,ID_PSUCCESS,temp,t);
					}
				}
				else
					m_BakeHit = false;
				--i;
			}
		}
		//���ͷ�����Ϣ
		if(!m_ExerciseMode)
		::AfxGetMainWnd()->SendMessage(ID_FIRSTBALL,0,0);
		m_first = false;		
	}

	//��һ��ѡ����С����ɫ
	for(int i=0; i<3; i++)
	{
		int colorkind;
		int myrand = rand()%40;
		if(!myrand && !m_ExerciseMode)
		{
			myrand = rand()%4+1;
			m_nextballcolor[i].time = 15;
		}
		else
			myrand = 0;

		colorkind = rand()%7+1;
		m_nextballcolor[i].color=colorkind;
		m_nextballcolor[i].prop = myrand;
	}
	//������һ��С����ɫ��Ϣ
	if(!m_ExerciseMode)
		::AfxGetMainWnd()->SendMessage(ID_GETNEXTBALLCOLOR,0,0);
}

void CChessView::SuccessProcess(BYTE succ_array[], BYTE point)
{
	for(int i=0; i<9; i++)
		memset(&m_DeadBall,0,sizeof(m_DeadBall));
	//
	int c=0;
	memcpy(&m_DeadBall[c],&m_Chess[point],sizeof(m_Chess[point]));
	m_DeadBall[c++].standby = point;

	CString msg;
	msg.Format("%d",m_Chess[point].color);
	//AfxMessageBox(msg);
	//�����������С��
	memset(&m_Chess[point],0,sizeof(m_Chess[point]));
	for(int i=0; succ_array[i]!=0; i++)
	{
		memcpy(&m_DeadBall[c],&m_Chess[succ_array[i]],sizeof(m_Chess[succ_array[i]]));
		m_DeadBall[c++].standby = succ_array[i];
		memset(&m_Chess[succ_array[i]],0,sizeof(m_Chess[succ_array[i]]));
	}
}

void CChessView::DisplayTakeTheService(BYTE nball[3],DBall nballcolor[3])
{
	
	memcpy(m_nextball, nball,sizeof(nball));
	memcpy(m_nextballcolor,nballcolor, sizeof(nballcolor));

	//��С���������
	for(int i=0; i<3; i++)
	{
		memcpy(&m_Chess[m_nextball[i]], &m_nextballcolor[i],sizeof(m_nextballcolor[i]));
		//�ж��Ƿ�ɹ���ȥ
		int t = m_Logic.IsSuccess(m_Chess,m_SuccessArray,m_nextball[i]);
		if(t)
		{
			if(!m_ExerciseMode)
			{
				::AfxGetMainWnd()->SendMessage(ID_SUCCESS,m_nextball[i],t);
				::SendMessage(this->GetParent()->m_hWnd,ID_PSUCCESS,m_nextball[i],t);
			}
			else
			{
				::SendMessage(this->GetParent()->m_hWnd,ID_PSUCCESS,m_nextball[i],t);
			}
		}
		else
			m_BakeHit = false;
	}
}

bool CChessView::IsMove(void)
{
	//�ж��Ƿ�߱��ƶ�������
	if(m_StartPos == ERROR_POINT || m_EndPos == ERROR_POINT )
	{
		AfxMessageBox("�����ÿ�ʼ��ͽ�����");
		return true;
	}

	//�Ƿ��п��Ե����·��
	int tempchess[82];
	for(int i=1; i<82; i++)
	{
		tempchess[i] = m_Chess[i].color;
	}
	if(m_Logic.Allpairs(tempchess, m_Result, m_StartPos, m_EndPos))
		//������򷵻�
		return true;
	else
	{
		return false;
	}
}

// �ͷ�ը��
void CChessView::BobmFunc(BYTE point,BYTE bombarray[])
{
	//����ը���ķ�Χ
	m_Logic.BobmFunc(m_Chess,point,bombarray);
	//ȡ��ը��״̬
	m_Bobm = false;
}

// �ƶ��ͷ���
void CChessView::MoveBall(BYTE end, DBall scolor)
{	
	//�ƶ�֮������һ��״̬
	m_StartPos = ERROR_POINT;
	m_EndPos = ERROR_POINT;
	
	//��С������ƶ���Ŀ��λ��
	memcpy(&m_Chess[end],&scolor,sizeof(scolor));

	//������Գɹ���Ϣ�����ͳɹ���Ϣ�����ٷ���
	int t = m_Logic.IsSuccess(m_Chess,m_SuccessArray,end);
	if(t)
	{
		if(!m_ExerciseMode)
		{
		::AfxGetMainWnd()->SendMessage(ID_SUCCESS,end,t);
		::SendMessage(this->GetParent()->m_hWnd,ID_PSUCCESS,end,t);
		}
		else
		{
			::SendMessage(this->GetParent()->m_hWnd,ID_PSUCCESS,end,t);
		}
	}
	else
	{
		m_BakeHit = false;

		//ѡ�����λ��
		BallRandFuc(m_Chess,m_nextball,3);

		//���ͷ�����Ϣ
		if(!m_ExerciseMode)
		{
			::AfxGetMainWnd()->SendMessage(ID_TAKE,0,0);
			//��С���������
			DisplayTakeTheService(m_nextball, m_nextballcolor);
			//ѡ����һ��С�����ɫ
			TakeTheService();
		}
		else
		{
			//��С���������
			DisplayTakeTheService(m_nextball, m_nextballcolor);
			//ѡ����һ��С�����ɫ
			TakeTheService();
		}
	}
}

void CChessView::OnTimer(UINT nIDEvent)
{

}

// ���̿�λ���������
bool CChessView::BallRandFuc(DBall  wChess[], BYTE  wNextBall[], int num)
{
	BYTE NullPos[81];
	int index=0;
	for(int i=1; i<82; i++)
	{
		if(wChess[i].color==0)
			NullPos[index++] = i;
	}
	int temp0=ERROR_POINT;
	int temp1=ERROR_POINT;
	
	for(int i=0; i<num;)
	{
		int pos = NullPos[rand()%index];
		
		if(temp0!=pos && temp1!=pos)
		{
			wNextBall[i++] = pos;
			if(i==1)
				temp0 = pos;
			if(i==2)
				temp1 = pos;
		}
	}
	return false;
}
