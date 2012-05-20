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
   ~RunTimeLog();  // 析构函数
   void last(bool show);
   void last();
   int  showResult(DWORD delay);

   bool  group , nobuff, Lastshow;  // 用于使信息按时间分组

 

private:
  // 关于函数指针的类型声明
 typedef  WINSHELLAPI int  (WINAPI  SHELLFILE) ( LPSHFILEOPSTRUCT );

   bool  active, dolog, nosystime;
   DWORD offset;        // 指出内存使用偏移
   DWORD BuffSize;
   DWORD NumToWrite;
   LPSTR SysTick;
   HANDLE  FileHandle;
   char  * lpBuff;      // 用于申请内存
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

