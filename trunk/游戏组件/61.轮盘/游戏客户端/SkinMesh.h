#ifndef SKIN_MESH_HEAD_FILE
#define SKIN_MESH_HEAD_FILE
#pragma once


//����ģ����
class CSkinMesh  
{
	//����������ת��ΪDWORD
	inline DWORD F2DW(FLOAT f) {return *((DWORD*)&f);}

	//�ڲ�����
private:
	D3DXVECTOR3			m_vPos;					//ģ��λ��
	D3DXVECTOR3			m_vMin,m_vMax;			//AABB���е���С������㣬��������ϵ
	DWORD				m_dwNumOfMaterials;		//��������
	TCHAR				m_szResourcePath[260];	//��Դ·��
	D3DMATERIAL9		m_SetMaterials;			//���ò���

	LPDIRECT3DDEVICE9	m_pDevice;				//3d�豸
	LPD3DXMESH			m_pMesh;				//����ģ��
	D3DMATERIAL9*		m_pMeshMaterials;		//ģ�Ͳ���
	LPDIRECT3DTEXTURE9*	m_pMeshTextures;		//��������
	LPDIRECT3DTEXTURE9 m_pBumpTextures;			//��͹����
	LPDIRECT3DTEXTURE9 m_pReflectTextures;		//��������

	//ID3DXEffect*		m_pEffect;
	//��Ⱦ״̬
private:
	float				m_fModelScale;			//���Ŵ�С
	float				m_fRotateSpeed;			//��ת�ٶ�
	bool				m_bTranslucenceRender;	//��͸����Ⱦ
	BYTE				m_bFillMode;			//��Ⱦģʽ
	BYTE				m_bShadeMode;			//��ɫģʽ
	bool				m_bUserSetMaterial;

	D3DXMATRIX			matScale,matRot;
	//��������
public:
	CSkinMesh();
	virtual ~CSkinMesh();	

	D3DXMATRIX GetRotate(){return matRot;}
public:
	//���غ���
	void SetLoadInfo(LPDIRECT3DDEVICE9 pDevice,LPTSTR strMesh,BOOL bOptimize=TRUE);
	//���ò���
	void SetModelMaterial(COLORREF diffuse=0xFFFFFF,COLORREF ambient=0,COLORREF specular=0,COLORREF Emisive=0,float power=0);
	//���ð�͹����
	void SetBumpTexture(LPDIRECT3DTEXTURE9 bumpTexture);
	//���÷�������
	void SetReflectTexture(LPDIRECT3DTEXTURE9 reflectTexture);
	//�ݿ�
	BOOL InterSect( D3DVECTOR *pRayOrig,D3DVECTOR *pRayDir,D3DVECTOR* pVRet,DWORD dwFlag=0);
	//�趨λ��
	void SetPosition(float x,float y,float z);
	void SetPosition(D3DXVECTOR3 vPos);
	//��ȡλ��
	D3DXVECTOR3 GetPosition(){return m_vPos;}
	//������Ⱦ��ʽ
	void SetRenderState(float ModelScal=1,float RotateSpeed=0,bool TranslucenceRender=false,BYTE FillMode=3,BYTE ShadeMode=2);
	//��Ⱦ����
	void Render();
	//�ͷź���
	void Release();
	//ģ���Ż�
	void Optimize();
	//ȡ�ð�Χ��
	void GetBoundingBox(D3DXVECTOR3 *pVmin,D3DXVECTOR3 *pVmax);
	//ʵ�ַ�����ײ���
	bool CollisionTest(CSkinMesh *pMesh);

private:
	//����͸����Ⱦ
	void OpenTranslucenceSet();
	//�ر�͸����Ⱦ
	void CloseTranslucenceSet();
	//������������
	void OpenReflect();
	//�رշ�������
	void CloseReflect();
	//������͹����
	void OpenBump();
};

#endif
