#include "StdAfx.h"
#include "GameClient.h"
#include "JettonControl.h"

//////////////////////////////////////////////////////////////////////////

//���Զ���
#define LAYER_COUNT					6									//������
#define LAYER_HEIGHT				5									//ÿ��߶�

#define RAND_WIDTH					150									//�滭����
#define	RAND_HEIGHT					120									//�滭����

#define MAX_DRAW					50									//�滭��������

//////////////////////////////////////////////////////////////////////////

//���캯��
CJettonControl::CJettonControl()
{
	//���ñ���
	m_lScore=0L;
	m_BenchmarkPos.SetPoint(0,0);
	m_DrawMode = enDrawMode_Layer;
	m_lDrawScore = 0L;

	//����λͼ
	HINSTANCE hInst = AfxGetInstanceHandle();
	m_PngJetton.LoadImage(hInst,TEXT("JETTON_VIEW"));

	return;
}

//��������
CJettonControl::~CJettonControl()
{
}

//�滭�ؼ�
VOID CJettonControl::DrawJettonControl(CDC * pDC)
{
	//�滭�ж� 
	if (m_lScore<=0L) return;

	//��ȡ����
	INT nJettonHeight=m_PngJetton.GetHeight();
	INT nJettonWidth=m_PngJetton.GetWidth()/JETTON_COUNT;

	//���ģʽ
	if( enDrawMode_Layer == m_DrawMode )
	{
		//��������
		LONG lScore=m_lScore;
		LONG lScoreArray[]={1L,5L,10L,50L,100L,500L,1000L,5000L,10000L,50000L,100000L,500000L,1000000L,5000000L};

		//�������
		for (WORD i=0;i<LAYER_COUNT;i++)
		{
			//��ȡ����
			INT nImageIndex=0;
			for (INT j=0;j<CountArray(lScoreArray);j++)
			{
				if (lScore>=lScoreArray[CountArray(lScoreArray)-j-1])
				{
					//���ñ���
					nImageIndex=CountArray(lScoreArray)-j-1;
					lScore-=lScoreArray[CountArray(lScoreArray)-j-1];
					break;
				}
			}

			//�滭����
			INT nXPos=m_BenchmarkPos.x-nJettonWidth/2;
			INT nYPos=m_BenchmarkPos.y-nJettonHeight/2-i*LAYER_HEIGHT;
			m_PngJetton.DrawImage(pDC,nXPos,nYPos,nJettonWidth,nJettonHeight,nJettonWidth*nImageIndex,0);

			//��ֹ�ж�
			if (lScore==0L) break;
		}
	}
	//����ѷ�ģʽ
	else
	{
		INT nXPos,nYPos;
		WORD i,j = 0;
		for( i = 0; i < m_JetonDraw.GetCount() && j < MAX_DRAW; i++ )
		{
			tagJettonDraw &jd = m_JetonDraw[i];
			for( WORD k = 0; k < jd.wDrawCount; k++ )
			{
				nXPos = m_BenchmarkPos.x + m_DrawExcusions[j].x - nJettonWidth/2;
				nYPos = m_BenchmarkPos.y + m_DrawExcusions[j].y - nJettonHeight/2;
				//�滭����
				m_PngJetton.DrawImage(pDC,nXPos,nYPos,nJettonWidth,nJettonHeight,nJettonWidth*jd.wDrawIndex,0);
				j++;
			}
		}
	}

	return;
}

//��׼λ��
VOID CJettonControl::SetBenchmarkPos(INT nXPos, INT nYPos)
{
	//���ñ���
	m_BenchmarkPos.SetPoint(nXPos,nYPos);

	return;
}

//���ó���
VOID CJettonControl::SetScore(LONG lScore)
{
	if( m_lScore != lScore )
	{
		m_lScore = lScore;
		RectifyControl();
	}
}

//��ӳ���
void CJettonControl::AddScore(LONG lScore)
{
	m_lScore += lScore;
	RectifyControl();
}

//�����ؼ�
void CJettonControl::RectifyControl()
{
	//m_lScoreΪ0,������ѷ�ģʽʱ
	if( m_lScore == 0 || enDrawMode_Rand == m_DrawMode && 
		m_lDrawScore != m_lScore )
	{
		LONG lDrawScore;
		if( m_lDrawScore > m_lScore )
		{
			lDrawScore = m_lScore;
			m_JetonDraw.RemoveAll();
			m_DrawExcusions.RemoveAll();
		}
		else lDrawScore = m_lScore - m_lDrawScore;
		m_lDrawScore = m_lScore;
		if( lDrawScore > 0L )
		{
			LONG lJettons[] = {1L,5L,10L,50L,100L,500L,1000L,5000L,10000L,50000L,100000L,500000L,1000000L,5000000L};
			for( int i = CountArray(lJettons)-1; ; i-- )
			{
				if( lDrawScore >= lJettons[i] )
				{
					//�滭�ṹ
					tagJettonDraw jd;
					jd.wDrawCount = (WORD)(lDrawScore/lJettons[i]);
					jd.wDrawIndex = i;
					m_JetonDraw.Add(jd);
					lDrawScore -= jd.wDrawCount*lJettons[i];
					//���λ��
					for( WORD j = 0; j < jd.wDrawCount; j++ )
					{
						CPoint pt(rand()%RAND_WIDTH,rand()%RAND_HEIGHT);
						m_DrawExcusions.Add(pt);
					}
				}
				if( lDrawScore == 0 ) break;
			}
		}
	}
}

//���û滭ģʽ
void CJettonControl::SetDrawMode( enDrawMode DrawMode )
{
	if( m_DrawMode != DrawMode )
	{
		m_DrawMode = DrawMode;
		RectifyControl();
	}
}

//////////////////////////////////////////////////////////////////////////
