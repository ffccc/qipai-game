#include "StdAfx.h"
#include "Camera.h"

//////////////////////////////////////////////////////////////////////

//���캯��
CCamera::CCamera()
{
	m_Yaw=0;
	m_Pitch=0;
	m_Distance=150;
	m_pDevice=NULL;
}

//��������
CCamera::~CCamera()
{
	SAFE_RELEASE(m_pDevice);
}

//�������
void CCamera::CreateCamera(LPDIRECT3DDEVICE9 pDevice)
{
	//���ò���
	m_Yaw=0.417f;
	m_Pitch=-0.791f;
	m_Distance=200;
	m_pDevice=pDevice;

	//��ʼλ��
	D3DXVECTOR3 CreatePos;
	CreatePos.x=0;
	CreatePos.y=0;
	CreatePos.z=0;
	m_vEyePos=CreatePos;

	//������Ļ���
	D3DSURFACE_DESC m_d3dsdBackBuffer;
	LPDIRECT3DSURFACE9 pBackBuffer = NULL;
	m_pDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
	pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
	m_scrWidth=m_d3dsdBackBuffer.Width;
	m_scrHeight=m_d3dsdBackBuffer.Height;
}
void CCamera::Release()
{
	SAFE_RELEASE(m_pDevice);
}

//�趨λ��
void CCamera::SetPosition(D3DXVECTOR3 cameraPosition)
{
	m_vEyePos=cameraPosition;
	return ;
}

//����3D����
void CCamera::Update3DMatrix()
{
	if(m_pDevice==NULL) return;

	D3DXMATRIX matView;
	D3DXVECTOR3 vFocus;
	GetCameraFocus(&vFocus);
	D3DXMatrixLookAtLH(&matView,&m_vEyePos,&vFocus,&D3DXVECTOR3(0,1,0));

	//��(100,100,100)����(0,0,0)
	//�ڶ��������ǹ۲��λ��(100,100,100),�����������ǽ���λ��(0,0,0)
	//D3DXMatrixLookAtLH(&matView,&D3DXVECTOR3(100,100,100),&D3DXVECTOR3(0,0,0),&D3DXVECTOR3(0,1,0));

	m_pDevice->SetTransform(D3DTS_VIEW,&matView);

	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection,D3DX_PI/4,(float)m_scrWidth/m_scrHeight,1.0f,5000.0f);
	m_pDevice->SetTransform(D3DTS_PROJECTION,&matProjection);
};


//����2D����
void CCamera::Update2DMatrix()
{
	if(m_pDevice==NULL) return;

	RECT rect;
	m_pDevice->GetScissorRect(&rect);
	D3DXMATRIX matOrtho;
	D3DXMATRIX matIdentity;
	D3DVIEWPORT9 viewPort;
	m_pDevice->GetViewport(&viewPort);

	//��������ͶӰ����
	D3DXMatrixOrthoLH(&matOrtho,(float)m_scrWidth ,(float)m_scrHeight, 0.0f, 1000.0f);
	D3DXMatrixIdentity(&matIdentity);

	m_pDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	m_pDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	matIdentity._22=-1;
	matIdentity._41=float(-m_scrWidth/2);
	matIdentity._42=float( m_scrHeight/2);
	m_pDevice->SetTransform(D3DTS_VIEW, &matIdentity);

};

//��������Ź۲����ת������ƫת
void CCamera::YawPitchAt(float yaw,float pitch)
{
	//���Ź۲��仯
	D3DXVECTOR3 vFocus;
	GetCameraFocus(&vFocus); //�ȱ���任ǰ�Ľ�������

	m_Yaw+=yaw; //�ı�ƫλ�Ƕ�
	if(m_Pitch-pitch<D3DX_PI/3.0f && m_Pitch-pitch>-D3DX_PI/3.0f) //�ı�����
		m_Pitch-=pitch;

	m_vEyePos.x=vFocus.x-m_Distance*cosf(m_Pitch)*cosf(m_Yaw);	//�������������
	m_vEyePos.y=vFocus.y-m_Distance*sinf(m_Pitch);				//�������������
	m_vEyePos.z=vFocus.z-m_Distance*cosf(m_Pitch)*sinf(m_Yaw);	//�������������

}; 

//���Ź۲��任��λ
void CCamera::YawPitchPos(float yaw,float pitch)
{
	m_Yaw+=yaw;
	if(m_Pitch+pitch<D3DX_PI/3.0f && m_Pitch+pitch>-D3DX_PI/3.0f)
		m_Pitch+=pitch;

};

//�ƶ���������(ʵ�������ƶ������)
void CCamera::SetFocus(D3DXVECTOR3 *vFocus)
{
	m_vEyePos.x=vFocus->x-m_Distance*cosf(m_Pitch)*cosf(m_Yaw);
	m_vEyePos.y=vFocus->y-m_Distance*sinf(m_Pitch);
	m_vEyePos.z=vFocus->z-m_Distance*cosf(m_Pitch)*sinf(m_Yaw);
};

//�ƶ������
void CCamera::MovePosTo(D3DXVECTOR3 *pV)
{
	m_vEyePos=*pV; 
}

//�ƶ�,��ǰ/�����ƶ�����Ҫ�������������
void CCamera::Move(float dSide,float dForward)
{
	D3DXVECTOR3 vDir;
	D3DXVECTOR3 vSide;
	vDir.x=cosf(m_Pitch)*cosf(m_Yaw);
	vDir.y=sinf(m_Pitch);
	vDir.z=cosf(m_Pitch)*sinf(m_Yaw);

	D3DXVec3Cross(&vSide,&D3DXVECTOR3(0,1,0),&vDir);
	D3DXVec3Normalize(&vSide,&vSide);
	if(m_Distance-dForward>0)
	{
		m_Distance-=dForward;
		m_vEyePos+=vDir*dForward+ vSide*dSide;
	}
}

//�������
void CCamera::AimTo(D3DVECTOR *pLookAt)
{
	D3DVECTOR vDir;
	vDir.x=pLookAt->x-m_vEyePos.x;
	vDir.y=pLookAt->y-m_vEyePos.y;
	vDir.z=pLookAt->z-m_vEyePos.z;
	D3DXVec3Normalize((D3DXVECTOR3*)&vDir,(D3DXVECTOR3*)&vDir);
	m_Pitch=acosf(vDir.y);
	if(vDir.z!=0 ) m_Yaw=atan2f(vDir.z,vDir.x);
	else m_Yaw=D3DX_PI/2;
}

//ȡ�ù۲��λ��
void CCamera::GetCameraFocus(D3DVECTOR *pLookAt)
{
	pLookAt->x= m_vEyePos.x+m_Distance*cosf(m_Pitch)*cosf(m_Yaw);
	pLookAt->y= m_vEyePos.y+m_Distance*sinf(m_Pitch);
	pLookAt->z= m_vEyePos.z+m_Distance*cosf(m_Pitch)*sinf(m_Yaw);
};

//��÷�������(��λ����)
void CCamera::GetDirection(D3DVECTOR *vDir)
{
	vDir->x=cosf(m_Pitch)*cosf(m_Yaw);
	vDir->y=sinf(m_Pitch);
	vDir->z=cosf(m_Pitch)*sinf(m_Yaw);
};

//������������
D3DVECTOR* CCamera::GetPos()
{
	return &m_vEyePos;
}

//������������
void CCamera::GetPos(D3DVECTOR *vPos)
{ 
	vPos->x=m_vEyePos.x;
	vPos->y=m_vEyePos.y;
	vPos->z=m_vEyePos.z;
};

//-----------------------------------------------------------------
//����������꣬��������
//ʵ�ּ������ָ������(���������ʼ�������ָ���һ��3D�ռ������)
//-----------------------------------------------------------------
void CCamera::GetCursorRayDir(POINT ptCursor,D3DVECTOR *vPickRayDir)
{
	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj,D3DX_PI/4,(float)m_scrWidth/m_scrHeight,1.0f,6000.0f);
	//������Ļ�ռ������(-1,-1)��(1,1)
	D3DXVECTOR3 v;
	v.x = ( (-1+2* ptCursor.x /float( m_scrWidth) ) /*- 1*/ ) / matProj._11;
	v.y = ( ( 1-2* ptCursor.y /float( m_scrHeight)) /*- 1*/ ) / matProj._22;
	v.z = 1.0f;

	//�����ͼ�任����������
	D3DVECTOR vFocus;
	GetCameraFocus(&vFocus);
	D3DXMATRIXA16 matView, m;
	D3DXMatrixLookAtLH(&matView,(D3DXVECTOR3*)&m_vEyePos,(D3DXVECTOR3*)&vFocus,&D3DXVECTOR3(0,1,0));

	D3DXMatrixInverse( &m, NULL, &matView );

	//����������ת������������
	vPickRayDir->x = v.x*m._11 + v.y*m._21 + v.z*m._31;
	vPickRayDir->y = v.x*m._12 + v.y*m._22 + v.z*m._32;
	vPickRayDir->z = v.x*m._13 + v.y*m._23 + v.z*m._33;
};
