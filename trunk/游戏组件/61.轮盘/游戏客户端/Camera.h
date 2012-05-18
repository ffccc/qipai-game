//////////////////////////////////////////////////////////////////////
//������࣬��Ҫ���ڿ��ƣ�����2D��3D��ͼ�任����ͶӰ�任����
//ʵ�ּ������ָ������(���������ʼ�������ָ���һ��3D�ռ������)
//�������m_vEyePos�۲죬����Ϊ:��m_vEyePosˮƽƫ��Ϊyaw,����Ϊpitch
//////////////////////////////////////////////////////////////////////

#ifndef CAMERA_HEAD_FILE
#define CAMERA_HEAD_FILE
#pragma once

#include "d3dx9.h"

//////////////////////////////////////////////////////////////////////

//�������
class CCamera  
{
	//�ڲ�����
protected:
	float				m_Yaw,m_Pitch;	//ƫ�ƽǶ�
	float				m_Distance;		//�۲����
	long				m_scrWidth;		//��Ļ��
	long				m_scrHeight;	//��Ļ��
	D3DXVECTOR3			m_vEyePos;			//���λ��
	LPDIRECT3DDEVICE9	m_pDevice;		//3D�豸

public:
	CCamera();
	virtual ~CCamera();

	//���ܺ���
public:
	//�������
	void CreateCamera(LPDIRECT3DDEVICE9 pDevice);
	//�ͷź���
	void Release();
	//����λ��
	void SetPosition(D3DXVECTOR3 camposition);
	//��׼����
	void AimTo(D3DVECTOR *pLookAt);
	//����2d����
	void Update2DMatrix();
	//����3d����
	void Update3DMatrix();
	//�ı�ˮƽƫ��
	void YawPitchAt(float yaw,float pitch);
	//�ı䴹ֱ����
	void YawPitchPos(float yaw,float pitch);

	//�������
public:	
	//�ƶ���������(ʵ�������ƶ������)
	void SetFocus(D3DXVECTOR3 *vFocus);
	//�ƶ������
	void MovePosTo(D3DXVECTOR3 *pV);
	//�ƶ����������������ռ�,Ҳ�����������������ǰ���ƶ����Ͳ��������ƶ�
	void Move(float dSide,float dForward);

	//���ݻ�ȡ
public:
	//��Ļ���
	long GetScreenWidth()  {return m_scrWidth;	};
	//��Ļ�߶�
	long GetScreenHeight() {return m_scrHeight;	};
	//������������
	D3DVECTOR*	GetPos();	
	//������������
	void GetPos(D3DVECTOR *vPos);	
	//��ù۲������
	void GetCameraFocus(D3DVECTOR *pLookAt);	
	//��÷�������(��λ����)
	void GetDirection(D3DVECTOR *vDir) ;
	//�������ָ������(���������ʼ�������ָ���һ��3D�ռ������)
	void GetCursorRayDir(POINT ptCursor,D3DVECTOR *vPickRayDir);
};

#endif
