#include "StdAfx.h"
#include "DiceCastAnimation.h"


DiceCastAnimation::DiceCastAnimation()
{	
	//Reset();
	m_usRandIndex_W =0;
	m_cbStatus = NO_ANIMATION;
}

	//初始化棋盘
void DiceCastAnimation::InitDiceCast(cGraphics *pGraphics){
	if(!m_DiceTexture.LoadTextureFromResource(pGraphics, IDB_DICE, 252, 44, 0xFFFF00FF)){
		MessageBox(NULL, TEXT("初始化骰子动画失败！"), TEXT(""), MB_OK);
	}
}

void DiceCastAnimation::CleanUp(void){
	m_DiceTexture.Free();
}

void DiceCastAnimation::Reset(BYTE cbMoveFlag)
{
	switch(cbMoveFlag){
	case DICE_LEFT_MOVE:
		m_usXShift =0;
		break;
	case DICE_RIGHT_MOVE:
		m_usXShift =710;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
	
	m_usYShift = 0;
	m_fRotationAngle = 0.0f;
	m_fAngleReduceAcceler = 2.0f;
	//m_fXScale = m_fYScale = 1.0f;
	m_lDownCount = 0;
	m_cbStatus = NO_ANIMATION;
//	m_lStopCount = 1000;
}


/*void DiceCastAnimation::ResetNoAnimate()
{
	m_usRandIndex_W = m_usXShift= m_usYShift = 0;
	m_fRotationAngle = 0.0f;
	m_fAngleReduceAcceler = 2.0f;
	//m_fXScale = m_fYScale = 1.0f;
	m_lDownCount = 0;
	m_cbStatus = NO_ANIMATION;
	m_lStopCount = 1000;
}*/


void DiceCastAnimation::Draw(cGraphics *pGraphics, DWORD ElapseTime, BYTE cbMoveFlag){
	static D3DXMATRIX  tempD3DXMatrix, tempMultiplyMatrix;	

	if(!m_DiceTexture.GetTextureCOM() || m_cbStatus == NOT_ACTIVE || !pGraphics || !pGraphics->GetSpriteCOM()) return ;
	if( m_cbStatus != NO_ANIMATION && m_cbStatus != NO_ANIMATION_WAIT){
		if(m_usXShift <355 && cbMoveFlag == DICE_LEFT_MOVE || m_usXShift >355 && cbMoveFlag == DICE_RIGHT_MOVE) {

			switch(cbMoveFlag){
			case DICE_LEFT_MOVE:
				m_usXShift += (USHORT)ElapseTime;
				if(m_usXShift > 355) m_usXShift = 355;
				m_usYShift = (m_usXShift-180)*(m_usXShift-180)/300 +200;
				break;
			case DICE_RIGHT_MOVE:
				m_usXShift -= (USHORT)ElapseTime;
				if(m_usXShift < 355) m_usXShift = 355;
				m_usYShift = (m_usXShift-530)*(m_usXShift-530)/300 +200;
				break;
			default:
				ASSERT(FALSE);
				break;
			}
			//m_fXScale += (float)(ElapseTime /1000.0);
		//	m_fYScale += (float)(ElapseTime /1000.0);
			m_fRotationAngle+= (float)(ElapseTime /10.0);

		} else {
			//if(m_cbStatus == SLOWING_STAGE){
				if(m_cbResult > MAX_RANDNUM || m_cbResult <= 0)return;
				m_fRotationAngle -= m_fAngleReduceAcceler;
				if(m_fAngleReduceAcceler>0.8f) m_fAngleReduceAcceler -= 0.08f;
				if(m_lDownCount < 0xFF && m_fRotationAngle <= 3.0f ){ 
					m_usRandIndex_W = (m_cbResult -1) * 42;
					m_lDownCount = 0x7FFFFFFF;
				} else if(m_fRotationAngle <= 0.0f) {
					m_fRotationAngle = 0.0f;
				//	m_cbStatus = STOP_STAGE;
					m_cbStatus = NO_ANIMATION;
				}			
			/*} else if(m_cbStatus == STOP_STAGE){
				m_lStopCount -= ElapseTime;
				if( m_lStopCount <0)m_cbStatus = NO_ANIMATION;
			}*/
		}

		if(m_lDownCount <= 0 && m_fRotationAngle >3.0f){
			m_usRandIndex_W = rand()%MAX_RANDNUM *42;
			m_lDownCount = 200;
			//RetailOutput("\nDownCount = %d", m_DiceCast.m_lDownCount);
		}
		m_lDownCount -= ElapseTime;
		//D3DXMatrixIdentity(&m_DiceCast.m_MatrixTransform);
		//D3DXMatrixIdentity(&tempMultiplyMatrix);
		D3DXMatrixTranslation(&tempMultiplyMatrix, m_usXShift,m_usYShift ,0);

		//D3DXMatrixIdentity(&m_MatrixTransform);
	//	D3DXMatrixScaling(&m_MatrixTransform, m_fXScale,m_fYScale,1);
	//	D3DXMatrixMultiply(&m_MatrixTransform, &m_MatrixTransform, &tempMultiplyMatrix);
		//D3DXMatrixIdentity(&tempMultiplyMatrix);
		D3DXMatrixRotationZ(&m_MatrixTransform, m_fRotationAngle);
		D3DXMatrixMultiply(&m_MatrixTransform, &m_MatrixTransform, &tempMultiplyMatrix);
		pGraphics->GetSpriteCOM()->GetTransform(&tempD3DXMatrix);
		pGraphics->GetSpriteCOM()->SetTransform(&m_MatrixTransform);

		m_DiceTexture.Blit(&CRect(m_usRandIndex_W ,0,m_usRandIndex_W+42,44), &D3DXVECTOR3(21, 22, 0), 0, 0);
		pGraphics->GetSpriteCOM()->SetTransform(&tempD3DXMatrix);
	} else {
	//	pGraphics->GetSpriteCOM()->GetTransform(&tempD3DXMatrix);
	//	pGraphics->GetSpriteCOM()->SetTransform(&m_MatrixTransform);
		m_DiceTexture.Blit(&CRect(m_usRandIndex_W ,0,m_usRandIndex_W+42,44), &D3DXVECTOR3(21, 22, 0), 355, 302);
		//pGraphics->GetSpriteCOM()->SetTransform(&tempD3DXMatrix);
	}
}
