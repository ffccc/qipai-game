// ****************************************************************************
// *******   FileName: RunTimeLog.h   ***************************************
// ****************************************************************************

#ifndef __RunTimeLogInclude
#define __RunTimeLogInclude
//Include Something here

 

 // *******************  Start of include  *****************************
#include <shellapi.h>

typedef class RunTimeLog
{

public:

   RunTimeLog();
   ~RunTimeLog();  // ��������
   void last(bool show);
   void last();
   int  showResult(DWORD delay);

   bool  group , nobuff, Lastshow;  // ����ʹ��Ϣ��ʱ�����

 

private:
  // ���ں���ָ�����������
 typedef  WINSHELLAPI int  (WINAPI  SHELLFILE) ( LPSHFILEOPSTRUCT );

   bool  active, dolog, nosystime;
   DWORD offset;        // ָ���ڴ�ʹ��ƫ��
   DWORD BuffSize;
   DWORD NumToWrite;
   LPSTR SysTick;
   HANDLE  FileHandle;
   char  * lpBuff;      // ���������ڴ�
   char  logfile[512];


  LPSTR  dwNum2Str(DWORD val);

public:

   DWORD  hlstrcpy(LPSTR  Str1, LPSTR Str2, DWORD Len );
   DWORD  hlstrcat(LPSTR  Str, LPSTR Plus, DWORD StrLen, DWORD PlusLen );
   LPSTR  GetDate();
   LPSTR  GetTime();
   LPSTR  GetSysTick(DWORD * len );
   int    write(LPSTR lpStr);
   int    numberwrite(LPSTR lpStr, DWORD val);
   int    WriteLog(void);
   void   msg(LPSTR Str);
   void   LogOutput(TCHAR *szFormat, ...);
 

} RunTimeLog,*PRunTimeLog;
// End of  class  RunTimeLog

//at Include last
#endif //#ifndef __RunTimeLogInclude
// *********************  End of include  ***************************
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////

