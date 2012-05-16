#ifndef VIDEO_SERVICE_CONTROL_HEAD_FILE
#define VIDEO_SERVICE_CONTROL_HEAD_FILE

#pragma once

#include "VideoSend.h"
#include "VideoRece.h"
#include "VideoServiceModule.h"

//////////////////////////////////////////////////////////////////////////

//��Ƶ����
class VIDEO_SERVICE_CLASS CVideoServiceControl : public CWnd
{
	//��Ԫ����
	friend class CVideoServiceManager;

	//�ں˱���
private:
	bool							m_bInitVideo;						//���ñ�־

	//�û���Ϣ
private:
	DWORD							m_dwUserID;							//�û���ʶ
	TCHAR							m_szAccounts[NAME_LEN];				//�û��ʺ�

	//״̬����
private:
	float							m_fVolumePercent;					//��������
	CRect							m_rcPaintVolume;					//��������

	//�˵�����
private:
	tagUserData						*m_pUserData;						//�����Ϣ

	//�ͻ�����
private:
	DWORD							m_dwTargerUserID;					//Ŀ�����

	//�豸��־
private:
	bool							m_bVidicon;							//��Ƶ�豸
	bool							m_bMicrophone;						//��Ƶ�豸
	bool							m_bOccupy;							//ռ���豸

	//���Ʊ���
private:
	bool							m_bSmallMode;						//С��ģʽ
	bool							m_bLocalMode;						//���ر�־
	bool							m_bEnableVideo;						//�Ƿ���Ƶ
	bool							m_bEnableAudio;						//�Ƿ���Ƶ

	//��ť�ؼ�
private:
	CSkinButton						m_btMore;							//���ఴť
	CSkinButton						m_btVideo;							//��Ƶ��ť
	CSkinButton						m_btAudio;							//��Ƶ��ť
	CSkinButton						m_btClone;							//��¡��ť
	CSkinButton						m_btFlower;							//�ͻ���ť

	//�ؼ�����
protected:
	CVideoSend						m_VideoSend;						//�������
	CVideoRece						m_VideoRece;						//�������
	CToolTipCtrl					m_ToolTipCtrl;						//��ʾ�ؼ�
	CFlowerEffectControl			m_FlowerEffectControl;				//�ʻ��ؼ�

	//ͼƬ��Դ
private:
	CSkinImage						m_ImageSmallPhoto;					//��Сͷ��
	CSkinImage						m_ImageLargePhoto;					//�Ŵ�ͷ��
	CSkinImage						m_ImageSmallFrame;					//��С���
	CSkinImage						m_ImageLargeFrame;					//�Ŵ���
	CSkinImage						m_ImageLargeVolume;					//�Ŵ���
	CSkinImage						m_ImageLargeVolumeEx;				//�Ŵ���
	CSkinImage						m_ImageSmallVolumeEx;				//�Ŵ���
	CSkinImage						m_ImageSmallVolume;					//�Ŵ���
	CSkinImage						m_ImageSmallDefFemale;				//Ĭ��ͷ��
	CSkinImage						m_ImageSmallDefMale;				//Ĭ��ͷ��
	CSkinImage						m_ImageLargeDefFemale;				//Ĭ��ͷ��
	CSkinImage						m_ImageLargeDefMale;				//Ĭ��ͷ��

	//��������
public:
	//���캯��
	CVideoServiceControl();
	//��������
	virtual ~CVideoServiceControl();

	//���غ���
protected:
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);

	//��Сģʽ
public:
	//��Сģʽ
	bool SetSmallMode(bool bSmallMode);
	//��Сģʽ
	bool IsSamllMode() { return m_bSmallMode; }

	//Զ���û�
public:
	//Զ���û�
	DWORD GetVideoUserID() { return m_dwUserID; }
	//Զ���û�
	bool SetVideoUserInfo(DWORD dwUserID, LPCTSTR pszAccounts);

	//�豸����
public:
	//��Ƶ�豸
	bool HaveVidicon() { return m_bVidicon; }
	//��Ƶ�豸
	bool HaveMicrophone() { return m_bMicrophone; }

	//���ƺ���
public:
	//������Ƶ
	bool InitVideoService(bool bLocalMode, bool bSmallMode);

	//�¼�����
public:
	//��������
	bool OnEventGameReset();
	//�û��뿪
	bool OnEventUserLeave(DWORD dwRemoteUserID);
	//�û�����
	bool OnEventUserEnter(DWORD dwRemoteUserID);
	//�û�����
	bool OnEventUserConfig(DWORD dwRemoteUserID, tagVideoNetwordInfo * pVideoNetwordInfo);
	//��ʾ��Ƶ
	bool OnShowLocalVD(bool bShow);
	//�ر���Ƶ
	bool OnEventCloseVideo();
	//����Ƶ
	bool OnEventOpenVideo();
	//�����ʻ�
	bool OnEventFlower(UINT uFlowerEffectID);

	//���ܺ���
public:
	//������Դ
	bool UpdateSkinResource();

	//�ڲ�����
protected:
	//��������
	VOID RectifyControl();

	//��Ϣӳ��
protected:
	//�ػ�����
	VOID OnPaint();
	//ʹ��֪ͨ
	VOID OnEventVideoEnable();
	//�滭����
	BOOL OnEraseBkgnd(CDC * pDC);
	//��������
	INT OnCreate(LPCREATESTRUCT lpCreateStruct);

	//���غ���
protected:
	//�˵�����
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	//��ť��Ϣ
protected:
	//���ఴť
	VOID OnBnClickedMore();
	//��Ƶ��ť
	VOID OnBnClickedVideo();
	//��Ƶ��ť
	VOID OnBnClickedAudio();
	//��¡��ť
	VOID OnBnClickedClone();
	//�ͻ���ť
	VOID OnBnClickedFlower();
	//��Ƶ��Ϣ
	void VolumeOutAV(unsigned long *Volume) ;

	//��Ϣ��
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP() 
};

//////////////////////////////////////////////////////////////////////////

//��Ƶ����
class VIDEO_SERVICE_CLASS CVideoServiceManager
{
	//��Ԫ����
	friend class CVideoServiceControl;

	//��Ƶ��Ϣ
protected:
	WORD							m_wVideoPort;						//��Ƶ�˿�
	DWORD							m_dwVideoServer;					//��Ƶ��ַ
	DWORD							m_dwLocalUserID;					//�����û�
	bool							m_bOffLine[MAX_CHAIR];				//��Ҷ���

	//�������
protected:
	IUserFaceRes *					m_pIUserFaceRes;					//ͷ��ӿ�
	IClientKernel *					m_pIClientKernel;					//�ں˽ӿ�
	IMessageProxy *					m_pIMessageProxy;					//��Ϣ����
	CVideoServiceControl *			m_VideoServiceControl[MAX_CHAIR];	//��Ƶ����

	//��Դ����
protected:
	static HINSTANCE				m_hResInstance;						//��Դ���
	static tagVideoFrameImage		m_VideoFrameImage;					//��Ƶ��Դ
	static tagPropertyMenuImage		m_PropertyMenuImage;				//�˵���Դ

	//��̬����
protected:
	static CVideoServiceManager *	m_pVideoServiceManager;				//�������

	//��������
public:
	//���캯��
	CVideoServiceManager();
	//��������
	virtual ~CVideoServiceManager();

	//���ú���
public:
	//���ýӿ�
	bool SetClientKernel(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	bool SetMessageProxy(IUnknownEx * pIUnknownEx);
	//���ýӿ�
	bool SetUserFaceRes(IUnknownEx * pIUnknownEx);
	//������Ϣ
	bool SetVideoServiceInfo(DWORD dwLoaclUserID, DWORD dwVideoServer, WORD wVideoPort);
	//���ö���
	bool SetVideoServiceControl(WORD wVideoIndex, CVideoServiceControl * pVideoServiceControl);

	//���ܺ���
public:
	//������Դ
	static void SetSkinResource(HINSTANCE hResInstance,tagVideoFrameImage *pVideoFrameImage,tagPropertyMenuImage *pPropertyMenuImage);
	//��ȡ�ؼ�
	CVideoServiceControl *GetVideoServiceControl(WORD wVideoIndex) {return m_VideoServiceControl[wVideoIndex];}

	//�¼��ӿ�
public:
	//��������
	bool OnEventGameReset();
	//�û��뿪
	bool OnEventUserLeave(WORD wVideoIndex, DWORD dwRemoteUserID);
	//�û�����
	bool OnEventUserEnter(WORD wVideoIndex, DWORD dwRemoteUserID);
	//�û�����
	bool OnEventUserConfig(WORD wVideoIndex, DWORD dwRemoteUserID, tagVideoNetwordInfo * pVideoNetwordInfo);
	//��ʾ��Ƶ
	bool OnShowLocalVD(bool bShow);
	//�����û�
	bool OnUpdateUserItem( WORD wVideoIndex, BYTE cbUserStatus, DWORD dwRemoteUserID );
	//�ر���Ƶ
	bool OnEventCloseVideo(WORD wVideoIndex);
	//����Ƶ
	bool OnEventOpenVideo(WORD wVideoIndex);
	//�����ʻ�
	bool OnEventFlower(WORD wVideoIndex, UINT uFlowerEffectID);

	//��̬����
public:
	//��ȡʵ��
	static CVideoServiceManager * GetInstance() { return m_pVideoServiceManager; }
};

//////////////////////////////////////////////////////////////////////////

#endif
