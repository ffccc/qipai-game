#include "StdAfx.h"
#include <d3dx9.h>
#include "SkinMesh.h"
#include <mmsystem.h>

//���캯��
CSkinMesh::CSkinMesh()
{
	//���ñ���
	m_pMesh=NULL;
	m_pDevice=NULL;
	m_dwNumOfMaterials=0;
	m_pMeshMaterials=NULL;
	m_pMeshTextures=NULL;
	m_pBumpTextures=NULL;
	m_pReflectTextures=NULL;
	ZeroMemory(&m_SetMaterials,sizeof(D3DMATERIAL9));
	ZeroMemory(m_szResourcePath,sizeof(m_szResourcePath));

	//m_pEffect=NULL;

	//��Ⱦ״̬
	m_fModelScale=1;			
	m_fRotateSpeed=0;			
	m_bTranslucenceRender=false;	
	m_bFillMode=3;			
	m_bShadeMode=2;	
	m_bUserSetMaterial=false;
}

//��������
CSkinMesh::~CSkinMesh()
{	
	for(DWORD i=0; i<m_dwNumOfMaterials; i++ )
    {
		SAFE_RELEASE(m_pMeshTextures[i]);
    }
	SAFE_DELETE_ARRAY(m_pMeshTextures);
	SAFE_DELETE_ARRAY(m_pMeshMaterials);
	SAFE_RELEASE(m_pBumpTextures);
	SAFE_RELEASE(m_pReflectTextures);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pDevice);
}
//�ͷź���
void CSkinMesh::Release()
{
	for(DWORD i=0; i<m_dwNumOfMaterials; i++ )
    {
		SAFE_RELEASE(m_pMeshTextures[i]);
    }
	SAFE_DELETE_ARRAY(m_pMeshTextures);
	SAFE_DELETE_ARRAY(m_pMeshMaterials);
	SAFE_RELEASE(m_pBumpTextures);
	SAFE_RELEASE(m_pReflectTextures);

	SAFE_RELEASE(m_pMesh);
	//SAFE_RELEASE(m_pDevice);

		 if(m_pDevice) { 
		 int xx=(m_pDevice)->Release(); 
		 (m_pDevice)=NULL; } 
}

//���غ���
void CSkinMesh::SetLoadInfo(LPDIRECT3DDEVICE9 pDevice,LPTSTR strMesh,BOOL bOptimize)
{
	//D3DXCreateEffectFromFile(pDevice,"EffectParam.fx",NULL,NULL,0,NULL,&m_pEffect,NULL);
 //       
	//D3DXHANDLE hTech;
 //   m_pEffect->FindNextValidTechnique( NULL, &hTech );
 //   m_pEffect->SetTechnique( hTech );    
	//
	//m_pEffect->GetParameterByName( NULL, "g_txScene" );

	LPD3DXBUFFER adjBuffer		 = NULL;
	LPD3DXBUFFER pD3DXMtrlBuffer = NULL;	//������ָ��
	D3DXMATERIAL* d3dxMaterials;			//�������в��ʵ�ָ��
	m_pDevice=pDevice;
	m_vPos.x=0;
	m_vPos.y=0;
	m_vPos.z=0;

	//ȡ����ԴĿ¼
	ZeroMemory(m_szResourcePath,sizeof(m_szResourcePath));
	for(int i=lstrlen(strMesh);i>0;i--)
	{
		if(strMesh[i]==TEXT('\\'))
			break;
	}
	for(int j=0;j<i;j++)
	{
		m_szResourcePath[j]=strMesh[j];
	}
	m_szResourcePath[j]=TEXT('\\');
	
	//��������
	LRESULT ret=D3DXLoadMeshFromX(strMesh,D3DXMESH_MANAGED,m_pDevice,&adjBuffer,&pD3DXMtrlBuffer,NULL,&m_dwNumOfMaterials,&m_pMesh);

	if(m_pMesh==NULL) throw 0 ;

#ifdef _DEBUG
	if(ret!=D3D_OK)
	{	
		CString strMeshName;
		strMeshName.Format(TEXT("����ģ�� %s ʧ�ܣ�\n�������ļ��������·������"),strMesh);
		AfxMessageBox(strMeshName);
		return;
	}
#endif

    d3dxMaterials =	(D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();	//��ȡ���ʵ�ַ
	
    m_pMeshMaterials = new D3DMATERIAL9[m_dwNumOfMaterials];			//����ָ������
    m_pMeshTextures  = new LPDIRECT3DTEXTURE9[m_dwNumOfMaterials];		//����ָ������
	
    for(DWORD i=0; i<m_dwNumOfMaterials; i++ )
    {
        m_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;					//�ӻ������п�������
        m_pMeshMaterials[i].Ambient = m_pMeshMaterials[i].Diffuse;		//���û���������
        m_pMeshTextures[i]  = NULL;										//��ʼ�ɿ�
		
        //���ݲ����е������ļ�����������
		TCHAR szBuffer[64];
		_snprintf(szBuffer,sizeof(szBuffer),TEXT("%s%s"),m_szResourcePath,d3dxMaterials[i].pTextureFilename);
		D3DXCreateTextureFromFile( m_pDevice,szBuffer, &m_pMeshTextures[i] );
    }

	ret = m_pMesh->OptimizeInplace(	D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_COMPACT|D3DXMESHOPT_VERTEXCACHE,(DWORD*)adjBuffer->GetBufferPointer(), 0, 0, 0);

	//�ͷŲ��ʻ�����
	if(pD3DXMtrlBuffer) pD3DXMtrlBuffer->Release();	pD3DXMtrlBuffer=NULL;
	
	if(bOptimize) Optimize(); 
}

//���÷�������
void CSkinMesh::SetReflectTexture(LPDIRECT3DTEXTURE9 reflectTexture)
{

	if(m_pReflectTextures==NULL) 
	{
		m_pReflectTextures=reflectTexture;
		m_pReflectTextures->AddRef();
	}

}
//���ð�͹����
void CSkinMesh::SetBumpTexture(LPDIRECT3DTEXTURE9 bumpTexture)
{	
	if(m_pBumpTextures==NULL)
	{
		m_pBumpTextures=bumpTexture;
		m_pBumpTextures->AddRef();
	}

}
//�Ż�����
void CSkinMesh::Optimize()
{
	LPD3DXMESH pOptimizedMesh;
	m_pMesh->Optimize(D3DXMESHOPT_COMPACT|D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_DONOTSPLIT|D3DXMESHOPT_DEVICEINDEPENDENT,NULL,NULL,NULL,NULL,&pOptimizedMesh);
	m_pMesh->Release();
	m_pMesh=pOptimizedMesh;
}
//��ײ���
BOOL CSkinMesh::InterSect( D3DVECTOR *pRayOrig,D3DVECTOR *pRayDir,D3DVECTOR* pVRet,DWORD dwFlag)
{
	return S_OK;
}

//������Ⱦ��ʽ
void CSkinMesh::SetRenderState(float ModelScal,float RotateSpeed,bool TranslucenceRender,BYTE FillMode,BYTE ShadeMode)
{
	m_fModelScale=ModelScal;			
	m_fRotateSpeed=RotateSpeed;			
	m_bTranslucenceRender=TranslucenceRender;
	m_bFillMode=FillMode;			
	m_bShadeMode=ShadeMode;			
}
void CSkinMesh::SetModelMaterial(COLORREF diffuse,COLORREF ambient,COLORREF specular,COLORREF Emisive,float power)
{
	m_bUserSetMaterial=true;
	//////////////////////////////////////////////////////////////////////////////////////

	m_SetMaterials.Diffuse.r=GetRValue(diffuse);
	m_SetMaterials.Diffuse.g=GetGValue(diffuse);
	m_SetMaterials.Diffuse.b=GetBValue(diffuse);
	m_SetMaterials.Diffuse.a=1.0f;

	m_SetMaterials.Ambient.r=GetRValue(ambient);
	m_SetMaterials.Ambient.g=GetGValue(ambient);
	m_SetMaterials.Ambient.b=GetBValue(ambient);
	m_SetMaterials.Ambient.a=1.0f;

	m_SetMaterials.Specular.r=GetRValue(specular);
	m_SetMaterials.Specular.g=GetGValue(specular);
	m_SetMaterials.Specular.b=GetBValue(specular);
	m_SetMaterials.Specular.a=1.0f;

	m_SetMaterials.Emissive.r=GetRValue(Emisive);
	m_SetMaterials.Emissive.g=GetGValue(Emisive);
	m_SetMaterials.Emissive.b=GetBValue(Emisive);
	m_SetMaterials.Emissive.a=0.0f;

	m_SetMaterials.Power=power;
	m_pDevice->SetMaterial(&m_SetMaterials);
	
//////////////////////////////////////////////////////////////////////////////////////
}


//��Ⱦ����
void CSkinMesh::Render()
{
	//�豸δ���û���ģ�ͼ��ز��ɹ�
	if(m_pDevice==NULL||m_pMesh==NULL)	return;
	if(m_bUserSetMaterial) m_pDevice->SetRenderState(D3DRS_SPECULARENABLE,TRUE);

	//��͸����Ⱦ
	if(m_bTranslucenceRender) OpenTranslucenceSet();

	//��Ⱦģʽ
	//FillMode:		1:����Ⱦ;  2:����Ⱦ; 3:����Ⱦ. Ĭ��Ϊ3
	//ShadeMode:	1:û������; 2:û������; 3:û������ Ĭ��Ϊ2
	m_pDevice->SetRenderState(D3DRS_FILLMODE,m_bFillMode);
	m_pDevice->SetRenderState(D3DRS_SHADEMODE ,m_bShadeMode);
	m_pDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

	D3DXMATRIX matWorld;


	//ģ����ת����
	float sp=m_fRotateSpeed*timeGetTime()/500.0f;

	D3DXMatrixRotationY((D3DXMATRIX*)&matRot,sp);//y����ת
	D3DXMatrixScaling((D3DXMATRIX*)&matScale,m_fModelScale,m_fModelScale,m_fModelScale);
	D3DXMatrixMultiply(&matWorld,&matRot,&matScale);

	matWorld._41=m_vPos.x;//�趨ģ��λ��
	matWorld._42=m_vPos.y;
	matWorld._43=m_vPos.z;
	m_pDevice->SetTransform(D3DTS_WORLD,&matWorld); 


	//��ʼ��Ⱦ
	m_pDevice->SetFVF(m_pMesh->GetFVF());
	for (DWORD i=0;i<m_dwNumOfMaterials;i++)
	{
		m_pDevice->SetMaterial(&m_pMeshMaterials[i]);
		m_pDevice->SetTexture(0,m_pMeshTextures[i]);

		if(m_pMeshMaterials[i].Power>0) m_pDevice->SetRenderState(D3DRS_SPECULARENABLE,TRUE);
		//�����ֶ����ò���
		if(m_bUserSetMaterial)	m_pDevice->SetMaterial(&m_SetMaterials);
		//���ڷ�������
		if(m_pReflectTextures!=NULL) OpenReflect();
		//���ڰ�͹����
		if(m_pBumpTextures!=NULL) OpenBump();

		m_pMesh->DrawSubset(i);
	}

	//�ر�����
	if(m_bTranslucenceRender) CloseTranslucenceSet();
	CloseReflect();

	m_pDevice->SetRenderState(D3DRS_SPECULARENABLE,FALSE);

	//���㷽��
	D3DXVECTOR3* pVertices;
	m_pMesh->LockVertexBuffer(0,(void**)&pVertices);
	D3DXComputeBoundingBox(pVertices,m_pMesh->GetNumVertices(),m_pMesh->GetNumBytesPerVertex(),&m_vMin,&m_vMax); 
	//D3DXVECTOR3 vCtr;
 //   float fRadius;
	//D3DXComputeBoundingSphere(pVertices,m_pMesh->GetNumVertices(),m_pMesh->GetNumBytesPerVertex(),&vCtr,&fRadius); 
	GetBoundingBox(&m_vMin,&m_vMax);
	m_pMesh->UnlockVertexBuffer(); 
} 

//�趨λ��
void CSkinMesh::SetPosition(float x,float y,float z)
{	
	m_vPos.x=x;
	m_vPos.y=y;
	m_vPos.z=z;
	return ;
};
//�趨λ��
void CSkinMesh::SetPosition(D3DXVECTOR3 vPos)
{
	m_vPos.x=vPos.x;
	m_vPos.y=vPos.y;
	m_vPos.z=vPos.z;
	return ;
}
//��ȡ��Χ��
void CSkinMesh::GetBoundingBox(D3DXVECTOR3 *pVmin,D3DXVECTOR3 *pVmax)
{
	*pVmin=m_vMin+m_vPos;
	*pVmax=m_vMax+m_vPos;
}
//aabb��ײ���
bool CSkinMesh::CollisionTest(CSkinMesh *pMesh)
{
	return (m_vMin.x<pMesh->m_vMax.x && m_vMin.y<pMesh->m_vMax.y && m_vMin.z<pMesh->m_vMax.z &&
			m_vMax.x>pMesh->m_vMin.x &&	m_vMax.y>pMesh->m_vMin.y &&	m_vMax.z>pMesh->m_vMin.z );
}
//�򿪰�͸����Ⱦ����
void CSkinMesh::OpenTranslucenceSet()
{
		m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE );
		m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
		m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE );
		m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
		m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
}
//�رհ�͸����Ⱦ����
void CSkinMesh::CloseTranslucenceSet()
{
		m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
		m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
		m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR  );
		m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_SRCCOLOR  );
		m_pDevice->SetTextureStageState(0,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
		m_pDevice->SetTextureStageState(0,D3DTSS_COLOROP,D3DTOP_MODULATE);
}
//������������
void CSkinMesh::OpenReflect()
{
	//m_pDevice->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);
	//m_pDevice->SetTextureStageState(1, D3DTSS_COLOROP,   D3DTOP_BLENDFACTORALPHA);
	//m_pDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0x80ffffff);
	m_pDevice->SetTexture(1,m_pReflectTextures);
	m_pDevice->SetTextureStageState(1,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_SPHEREMAP);
	m_pDevice->SetTextureStageState(1,D3DTSS_TEXTURETRANSFORMFLAGS,D3DTTFF_COUNT2);
	m_pDevice->SetTextureStageState(1,D3DTSS_COLORARG2,D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(1,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(1,D3DTSS_COLOROP,D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(1,D3DTSS_ALPHAOP,D3DTOP_DISABLE);
}
//�رշ�������
void CSkinMesh::CloseReflect()
{
	////ȡ�õ�ǰ�豸����
	//DWORD a,b,c,d,e,f;
	//m_pDevice->GetTextureStageState(1,D3DTSS_TEXCOORDINDEX,&a);
	//m_pDevice->GetTextureStageState(1,D3DTSS_TEXTURETRANSFORMFLAGS,&b);
	//m_pDevice->GetTextureStageState(1,D3DTSS_COLORARG2,&c);
	//m_pDevice->GetTextureStageState(1,D3DTSS_COLORARG1,&d);
	//m_pDevice->GetTextureStageState(1,D3DTSS_COLOROP,&e);
	//m_pDevice->GetTextureStageState(1,D3DTSS_ALPHAOP,&f);
	m_pDevice->SetTexture(1,NULL);
	m_pDevice->SetTextureStageState(1,D3DTSS_TEXCOORDINDEX,1);
	m_pDevice->SetTextureStageState(1,D3DTSS_TEXTURETRANSFORMFLAGS,0);
	m_pDevice->SetTextureStageState(1,D3DTSS_COLORARG2,1);
	m_pDevice->SetTextureStageState(1,D3DTSS_COLORARG1,2);
	m_pDevice->SetTextureStageState(1,D3DTSS_COLOROP,1);
	m_pDevice->SetTextureStageState(1,D3DTSS_ALPHAOP,1);
}
//������͹����
void CSkinMesh::OpenBump()
{
	m_pDevice->SetTexture(2,m_pBumpTextures);
	m_pDevice->SetTextureStageState(2,D3DTSS_TEXCOORDINDEX,D3DTSS_TCI_PASSTHRU);
	m_pDevice->SetTextureStageState(2,D3DTSS_COLOROP,D3DTOP_BUMPENVMAPLUMINANCE);
	m_pDevice->SetTextureStageState(2,D3DTSS_COLORARG1,D3DTA_TEXTURE);
	m_pDevice->SetTextureStageState(2,D3DTSS_COLORARG2,D3DTA_CURRENT);
	m_pDevice->SetTextureStageState(2,D3DTSS_BUMPENVMAT00,F2DW(0.5f));
	m_pDevice->SetTextureStageState(2,D3DTSS_BUMPENVMAT01,F2DW(0.0f));
	m_pDevice->SetTextureStageState(2,D3DTSS_BUMPENVMAT10,F2DW(0.0f));
	m_pDevice->SetTextureStageState(2,D3DTSS_BUMPENVMAT11,F2DW(0.5f));
	m_pDevice->SetTextureStageState(2,D3DTSS_BUMPENVLSCALE,F2DW(4.0f));
	m_pDevice->SetTextureStageState(2,D3DTSS_BUMPENVLOFFSET,F2DW(0.0f));
	m_pDevice->SetSamplerState(2,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(2,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
}