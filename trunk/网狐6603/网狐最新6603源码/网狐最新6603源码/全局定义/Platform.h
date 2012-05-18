#ifndef PLATFORM_HEAD_FILE
#define PLATFORM_HEAD_FILE

//////////////////////////////////////////////////////////////////////////////////
//�����ļ�

//�����ļ�
#include "Macro.h"
#include "Define.h"

//�ṹ�ļ�
#include "Struct.h"
#include "Packet.h"
#include "Property.h"

//ģ���ļ�
#include "Array.h"
#include "Module.h"
#include "PacketAide.h"
#include "ServerRule.h"
#include "RightDefine.h"

//////////////////////////////////////////////////////////////////////////////////

//����汾
#define VERSION_FRAME				PROCESS_VERSION(6,0,3)				//��ܰ汾
#define VERSION_PLAZA				PROCESS_VERSION(6,0,3)				//�����汾
#define VERSION_MOBILE				PROCESS_VERSION(6,0,3)				//�ֻ��汾

//�����汾
#define VERSION_FRAME_SDK			INTERFACE_VERSION(6,3)				//��ܰ汾

//////////////////////////////////////////////////////////////////////////////////
//�����汾

#ifndef _DEBUG

//ƽ̨����
const TCHAR	szProduct[]=TEXT("���ļ�԰");							    //��Ʒ����
const TCHAR szPlazaClass[]=TEXT("WHHNZDWPGamePlaza");					//�㳡����
const TCHAR szProductKey[]=TEXT("WHHNZDWPGamePlatform");				//��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("http://hnkaihe.com");					//��¼��ַ
const TCHAR szLogonServer[]=TEXT("www.hnkaihe.com");					//��¼��ַ
const TCHAR szPlatformLink[]=TEXT("http://www.hnkaihe.com");			//ƽ̨��վ

#else

//////////////////////////////////////////////////////////////////////////////////
//�ڲ�汾

//ƽ̨����
const TCHAR	szProduct[]=TEXT("��������ƽ̨");							//��Ʒ����
const TCHAR szPlazaClass[]=TEXT("WHQPGamePlaza");						//�㳡����
const TCHAR szProductKey[]=TEXT("WHQPGamePlatform");					//��Ʒ����

//��ַ����
const TCHAR szCookieUrl[]=TEXT("http://foxuc.net");						//��¼��ַ
const TCHAR szLogonServer[]=TEXT("qp2.foxuc.net");						//��¼��ַ
const TCHAR szPlatformLink[]=TEXT("http://qp2.foxuc.net");				//ƽ̨��վ

#endif

//////////////////////////////////////////////////////////////////////////////////

//���ݿ���
const TCHAR szPlatformDB[]=TEXT("QPPlatformDB");						//ƽ̨���ݿ�
const TCHAR szAccountsDB[]=TEXT("QPAccountsDB");						//�û����ݿ�
const TCHAR	szTreasureDB[]=TEXT("QPTreasureDB");						//�Ƹ����ݿ�
const TCHAR	szExerciseDB[]=TEXT("QPExerciseDB");						//��ϰ���ݿ�

//////////////////////////////////////////////////////////////////////////////////

//��Ȩ��Ϣ
const TCHAR szCompilation[]=TEXT("4B536A49-1246-45EB-8363-FF4C7DF645CC");

//////////////////////////////////////////////////////////////////////////////////

#endif