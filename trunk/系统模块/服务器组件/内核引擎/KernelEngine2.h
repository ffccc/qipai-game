#ifndef SERVER_KERNEL_HEAD_FILE2
#define SERVER_KERNEL_HEAD_FILE2
//////////////////////////////////////////////////////////////////////////
//�����ļ�

#include <Afxmt.h>
#include <Comutil.h>
#include <ICrsint.h>
#include <Process.h>
#include <WinSock2.h>

//ƽ̨�ļ�
#include "..\..\ģ���\Template.h"
#include "..\..\�����ļ�\Constant.h"
#include "..\..\�����ļ�\GlobalDef.h"
#include "..\..\�������\��������\ComService.h"

//////////////////////////////////////////////////////////////////////////
//ADO ����

#import "MSADO15.DLL" rename_namespace("ADOCG") rename("EOF","EndOfFile")
using namespace ADOCG;

typedef _com_error						CComError;						//COM ����
typedef _variant_t						CDBVarValue;					//���ݿ���ֵ




//////////////////////////////////////////////////////////////////////////
//��������

//��������
#ifndef KERNEL_ENGINE_CLASS
	#ifdef  KERNEL_ENGINE_DLL
		#define KERNEL_ENGINE_CLASS _declspec(dllexport)
	#else
		#define KERNEL_ENGINE_CLASS _declspec(dllimport)
	#endif
#endif

//ģ�鶨��
#ifndef _DEBUG
	#define KERNEL_ENGINE_DLL_NAME	TEXT("KernelEngine.dll")			//��� DLL ����
#else
	#define KERNEL_ENGINE_DLL_NAME	TEXT("KernelEngineD.dll")			//��� DLL ����
#endif

//������ӵ�start
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//�����궨��
#define EVENT_LEVEL_COUNT			4									//�¼��ȼ�
#define MAX_QUEUE_PACKET			10240								//������
#define INDEX_ALL_SOCKET			0xFFFF								//��������
#define TIMER_REPEAT_TIMER			DWORD(-1)							//�ظ�����
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//������ӵ�end

//
////����ȼ�
//enum enTraceLevel
//{
//	TraceLevel_Info					=0,									//��Ϣ��Ϣ
//	TraceLevel_Normal				=1,									//��ͨ��Ϣ
//	TraceLevel_Warning				=2,									//������Ϣ
//	TraceLevel_Exception			=3,									//�쳣��Ϣ
//	TraceLevel_Debug				=4,									//������Ϣ
//};
//

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

////����ȼ�
//enum enTraceLevel
//{
//	Level_Normal					=0,									//��ͨ��Ϣ
//	Level_Warning					=1,									//������Ϣ
//	Level_Exception					=2,									//�쳣��Ϣ
//	Level_Debug						=3,									//������Ϣ
//};


//////////////////////////////////////////////////////////////////////////
//ϵͳ����

//��������
#define TIME_CELL					200									//ʱ�䵥Ԫ
#define TIMES_INFINITY				DWORD(-1)							//���޴���
#define MAX_ASYNCHRONISM_DATA		8192								//�첽����

//////////////////////////////////////////////////////////////////////////
//���綨��

//���Ӵ���
#define CONNECT_SUCCESS				0									//���ӳɹ�
#define CONNECT_FAILURE				1									//����ʧ��
#define CONNECT_EXCEPTION			2									//�����쳣

//�ر�ԭ��
#define SHUT_REASON_INSIDE			0									//�ڲ�ԭ��
#define SHUT_REASON_NORMAL			1									//�����ر�
#define SHUT_REASON_REMOTE			2									//Զ�̹ر�
#define SHUT_REASON_TIME_OUT		3									//���糬ʱ
#define SHUT_REASON_EXCEPTION		4									//�쳣�ر�

//////////////////////////////////////////////////////////////////////////
//ö�ٶ���

//����ȼ�
enum enTraceLevel
{
	TraceLevel_Info					=0,									//��Ϣ��Ϣ
	TraceLevel_Normal				=1,									//��ͨ��Ϣ
	TraceLevel_Warning				=2,									//������Ϣ
	TraceLevel_Exception			=3,									//�쳣��Ϣ
	TraceLevel_Debug				=4,									//������Ϣ
};

//SQL �쳣����
enum enSQLException
{
	SQLException_None				=0,									//û���쳣
	SQLException_Connect			=1,									//���Ӵ���
	SQLException_Syntax				=2,									//�﷨����
};

//����״̬
enum enTCPSocketStatus
{
	TCPSocketStatus_Idle,			//����״̬
	TCPSocketStatus_Connecting,		//����״̬
	TCPSocketStatus_Connected,		//�ɹ�����
};


//�¼���ʶ
#define EVENT_CONTROL				0x0001								//������Ϣ
#define EVENT_TIMER					0x0002								//��ʱ������


//��ʱ���¼�
struct NTY_TimerEvent
{
	WORD							wTimerID;							//��ʱ�� ID
	WPARAM							wBindParam;							//�󶨲���
};

//���ݿ������¼�
struct NTY_DataBaseEvent
{
	WORD							wIndex;								//��������
	WORD							wRoundID;							//�����ʶ
	WORD							wRequestID;							//�����ʶ
};


#endif