#pragma once

#include "GameEngineHead.h"
#include "DirectShowBasic.h"

class GAME_ENGINE_CLASS CD3DMusic
{

public:
	CD3DMusic(void);
	~CD3DMusic(void);

private:
	bool			m_bBackTime;
	CString			mFilePath;
	CString			GetAppPath();
	BOOL			RegisterFilter(LPCTSTR  inFilterAx);

public:
	//------------------------------------------
	// ��������

	//************************************
	// Method:    ��������
	// FullName:  CD3DMusic::Start
	// Access:    public 
	// Returns:   bool
	// Parameter: LPCTSTR  filename	�ļ��� (���������Լ�����)
	//************************************
	bool Start(LPCTSTR filename);

	//************************************
	// Method:    ��ͣ����
	// FullName:  CD3DMusic::Pause
	// Access:    public 
	// Returns:   bool
	// Parameter: LPCTSTR  filename	�ļ��� (���ΪNULL ��ȫ����ͣ)
	//************************************
	bool Pause(LPCTSTR filename  = NULL);

	//************************************
	// Method:    ��������(��Ҫ�ȵ���Start�����Ժ����ʹ�� ������ͣ��ֹͣ������)
	// FullName:  CD3DMusic::Run
	// Access:    public 
	// Returns:   bool
	// Parameter: LPCTSTR  filename	�ļ��� (���ΪNULL ��ȫ��ֹͣ����ͣ����������)
	//************************************
	bool Run(LPCTSTR filename  = NULL);

	//************************************
	// Method:    ֹͣ����(ֹͣ�Լ��Ѵ���������ɾ��,��Ҫ�ٲ�������Ҫ����Start���벥��)
	// FullName:  CD3DMusic::Stop
	// Access:    public 
	// Returns:   bool
	// Parameter: LPCTSTR  filename �ļ��� (���ΪNULL ��ȫ��ֹͣ)
	//************************************
	bool Stop(LPCTSTR filename  = NULL);

	//************************************
	// Method:    ����������С
	// FullName:  CD3DMusic::ControlVolume
	// Access:    public 
	// Returns:   void
	// Parameter: int ibulk			������С(-10000 ~ 0)0���
	//************************************
	void ControlVolume(int ibulk);



	//----------------------------------------------------------
	//�����б�

	//************************************
	// Method:    �����Ŀ
	// FullName:  CD3DMusic::ListAdd
	// Access:    public 
	// Returns:   void
	// Parameter: LPCTSTR  filename  �ļ���
	//************************************
	void ListAdd(LPCTSTR filename);


	//************************************
	// Method:    ��ʼ��������
	// FullName:  CD3DMusic::Back_Start
	// Access:    public 
	// Returns:   void
	//************************************
	void ListStart();


	//************************************
	// Method:    ������б�������
	// FullName:  CD3DMusic::ListClear
	// Access:    public 
	// Returns:   void
	//************************************
	void ListClear();

	//************************************
	// Method:    ���ű�������(�ڱ�ֹͣ����ͣ��ʱ����ò���)
	// FullName:  CD3DMusic::ListRun
	// Access:    public 
	// Returns:   void
	//************************************
	void ListRun();


	//************************************
	// Method:    ��ͣ��������(ֻ�ܱ�Back_Run()��������)
	// FullName:  CD3DMusic::ListPause
	// Access:    public 
	// Returns:   void
	//************************************
	void ListPause();

	//************************************
	// Method:    ֹͣ��������(ֻ�ǰ�������0,������Back_Run()�ٴβ���)
	// FullName:  CD3DMusic::ListStop
	// Access:    public 
	// Returns:   void
	//************************************
	void ListStop();


	//************************************
	// Method:    ���Ʊ���������С
	// FullName:  CD3DMusic::ListVolume
	// Access:    public 
	// Returns:   void
	// Parameter: int ibulk			������С(-10000 ~ 0)0���
	//************************************
	void ListVolume(int ibulk);
};