#pragma once

//////////////////////////////////////////////////////////////////////////

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WINVER
#define WINVER 0x0400
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif						

#ifndef _WIN32_WINDOWS	
#define _WIN32_WINDOWS 0x0410
#endif

#ifndef _WIN32_IE
#define _WIN32_IE 0x0400
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS

#define _AFX_ALL_WARNINGS

#include <afxwin.h> 
#include <afxext.h> 
#include <afxdisp.h>

#include <afxdtctl.h>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>
#endif

//////////////////////////////////////////////////////////////////////////

//ϵͳͷ�ļ�
#include "Nb30.h"
#include "Afxmt.h"
#include "AfxHtml.h"
#include "AfxInet.h"

//ȫ��ͷ�ļ�
#include "..\..\�����ļ�\Constant.h"
#include "..\..\�����ļ�\GlobalDef.h"
#include "..\..\�����ļ�\GlobalField.h"
#include "..\..\�����ļ�\GlobalFrame.h"
#include "..\..\�����ļ�\GlobalRight.h"

//����ͷ�ļ�
#include "..\..\��Ϣ����\CMD_Game.h"
#include "..\..\��Ϣ����\CMD_Plaza.h"
#include "..\..\��Ϣ����\CMD_Video.h"

//ģ���
#include "..\..\ģ���\Template.h"

//���ͷ�ļ�
#include "..\ͷ�����\UserFace.h"
#include "..\�������\DownLoad.h"
#include "..\��Ϸ�ȼ�\GameRank.h"
#include "..\��ϵ����\Companion.h"
#include "..\�ŵ�ģ��\ChannelModule.h"
#include "..\�ͻ��˹���\ClientShare.h"
#include "..\�������\NetworkServiceHead.h"
#include "..\�ʻ�����\PropertyModule.h"
#include "..\..\�������\��������\ComService.h"
#include "..\..\�������\����ؼ�\SkinControls.h"
#include "..\..\�������\������Դ\SkinResourceModule.h"

//////////////////////////////////////////////////////////////////////////
