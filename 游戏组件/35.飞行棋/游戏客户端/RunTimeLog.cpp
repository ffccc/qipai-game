 // ****************************************************************************
 // *******   FileName: RunTimeLog.cpp   ***************************************
 // ******************   Version: 1.12   ***************************************
 // ****************************************************************************
 // 声明：该文件可以随便修改、使用 或 转载，但本人不对使用该类造成的任何后果负责
 // 作者：`海风   ************  E-mail：  free77@163.net************************
 // ****************************************************************************
 //  可以在函数内部定义你的类，如下的使用 new 的定义方法。 *********************
 //  RunTimeLog * rtlog = new RunTimeLog;   然后在最后  delete rtlog;  *********
 //  也能使用如下方法定义： RunTimeLog  log;  并最好显式调用 析构函数  *********
 // ****************************************************************************

 // 对外提供的接口 主要是 ::write(""); 用于写入信息到缓冲内存中，
 // 以及 ::last(); 用于最后结束部分，可以向 .log 文件写入缓冲内容
 // 也可以直接调用 ::WriteLog(); 来向 .log 文件写入缓冲内容(如果有的话)。
 // Ver 1.02 新增信息按时间分组功能，使用 group 可以控制是否分组
 // 已修正自动写缓冲到磁盘错误...  ，缓冲换成2048
#include "Stdafx.h"
#include <windows.h>
 #include "RunTimeLog.h"


   // 构造函数和析构函数
  RunTimeLog::RunTimeLog()  // 不带参数构造函数
  {return;
    nobuff=false;  Lastshow=false;   dolog=false;
    active=false;  nosystime=false;  group = true;

    offset=NULL;    NumToWrite=NULL;
    SysTick=NULL;   FileHandle=NULL;
    lpBuff=NULL;    BuffSize = 2048; // 设置 BuffSize 的大小

    bool   DelOld = false;        // 设定是否要扔掉旧日志
    DWORD  OverWrite = true;     // 设定是否要覆盖旧日志，如果上面没有丢的话
    WORD   ToRecyclebin = false;  // 设定是否把旧日志放到回收站

    OverWrite = OverWrite? CREATE_ALWAYS : CREATE_NEW; // 一个宏定义
    ToRecyclebin = ToRecyclebin ? FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION : FOF_SILENT | FOF_NOCONFIRMATION;

 

    //·申请可用的内存作缓冲空间，指向 lpBuff, 大小为 BuffSize

    // lpBuff = (char*)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, BuffSize);
     lpBuff = new char[BuffSize];
    // lpBuff = Buffer;

    if ( lpBuff != NULL )
    {
     //·取得进程文件名，变化成 .log 文件名，查找是否重复了，是则送到回收站
      GetModuleFileName( NULL,lpBuff,BuffSize);
      hlstrcpy(logfile, lpBuff, NULL);
      GetLongPathName( logfile, logfile, 512);  // 如果你在 Win98 中调用该函数，可能会给出短文件名格式
      UINT len = strlen(logfile);
      UINT i = 0;
       for ( i=len; i>0; i-- )
      { if (logfile[i]=='.') { 
		  srand(GetTickCount());
		  logfile[i] = '\0';
		  wsprintf(logfile, "%s%d.", logfile, rand());
		  break; } 
	  }
	  len = strlen(logfile);
      // 然后从后面去掉扩展名
      for ( i=len; i>0; i-- )
      { if (logfile[i]=='.') { hlstrcat(logfile, ".log", i, 4); len = len + 3; break; } }

 


     
    // 首先判断文件是否存在
    Loop: // 如果不能一次创建文件成功时用。
    if (( GetFileAttributes( logfile ) == -1 )||(GetLastError()==ERROR_FILE_NOT_FOUND))
       ;// msg("文件不存在");

    else if(DelOld)  // 该处设成 true 则要删除原文件

    {  // 发现文件已经存在了，需要扔到回收站里面去
    SHFILEOPSTRUCTA Fileop;
    LPSHFILEOPSTRUCT lpFileOp = &Fileop;
    memset( lpFileOp, NULL, sizeof(Fileop) );

 

    Fileop.fFlags = ToRecyclebin;
    Fileop.wFunc = FO_DELETE ;
    Fileop.pFrom = logfile;
    lpFileOp->hwnd = NULL;

    //*
    // 关于运行时连接的文件操作方式，定义了 函数的指针
    SHELLFILE * lpSHFileOperation = NULL;  // 函数指针定义
    HMODULE hlib = LoadLibrary( "shell32.dll" );
     if (hlib)
     {  
       lpSHFileOperation = (SHELLFILE *) GetProcAddress( hlib, "SHFileOperation" );
       if ( lpSHFileOperation )
       {
         /*if (*/ lpSHFileOperation(lpFileOp); //msg( "SHFileOperation 执行过程中出现了问题");
       }
       else msg("shell32.dll 中没有这个函数");
       if (! FreeLibrary( hlib )) msg("释放DLL失败");
     }
    else msg("没有 shell32.dll"); //*/

    }  // 扔垃圾完成


    //·创建一个新文件，写入开始记录的时间
     FileHandle = CreateFile( logfile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OverWrite, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle == INVALID_HANDLE_VALUE)
    {  // 当不能创建新文件的时候尝试增加文件后缀
       if (hlstrcat( logfile, ".log", NULL, 4) < 125)  goto Loop;
       else { msg("CreateFile 执行过程中出现了问题，放弃重试。" ); /*ExitProcess(0);*/}
    }
    else {  // 句柄有效的时候才执行
      SetFilePointer(FileHandle, GetFileSize(FileHandle,NULL), NULL, FILE_BEGIN);  // 设置原始大小

        WriteFile(FileHandle, "[Source Path & FileName:]  ", strlen("[Source Path & FileName:]  "), &NumToWrite, NULL);
        WriteFile(FileHandle, logfile, strlen(logfile), &NumToWrite, NULL);
        /*用于分行*/ WriteFile(FileHandle, "\x0d\x0a", strlen("\x0d\x0a"), &NumToWrite, NULL);

        wsprintf( lpBuff, "Record Start at  Date: %s  &  Time: %s .", GetDate(), GetTime() );

        SysTick = GetSysTick(&NumToWrite);  // 取得系统时间
        WriteFile(FileHandle, SysTick, NumToWrite, &NumToWrite, NULL);

        WriteFile(FileHandle, lpBuff, strlen(lpBuff), &NumToWrite, NULL);

        /*用于分行*/ WriteFile(FileHandle, "\x0d\x0a", strlen("\x0d\x0a"), &NumToWrite, NULL);

      if (!SetEndOfFile(FileHandle)) MessageBox(NULL, "设置结尾不成功", "??", MB_OK|MB_TOPMOST); // 写完数据置尾

      // CloseHandle( FileHandle );    FileHandle = INVALID_HANDLE_VALUE; // 收尾关闭句柄
       lpBuff[0] = NULL;
       } // 完成句柄有效时的执行
       active = true; dolog = true;
       }
     else active = false;
     nosystime = false;
     write("");

    return;
   }

  // ******************************************************************
    void RunTimeLog::last() { last(0); }
    void RunTimeLog::last(bool show)
   {
     group = false;

    // 写入结束记录的时间
     if (active)
     {
       WriteLog();
     if (lpBuff != NULL)
     {
     if (FileHandle == INVALID_HANDLE_VALUE) { msg("最后 WriteLog 过程中出现了问题"); ExitProcess(0);}
     else 
     {  // 句柄有效的时候才执行

     /*用于分行*/ WriteFile(FileHandle, "\x0d\x0a", strlen("\x0d\x0a"), &NumToWrite, NULL);
     SysTick = GetSysTick(&NumToWrite);  // 取得系统时间
     WriteFile(FileHandle, SysTick, NumToWrite, &NumToWrite, NULL);

     wsprintf( lpBuff, "Record  End  at  Date: %s  &  Time: %s .", GetDate(), GetTime() );
     WriteFile(FileHandle, lpBuff, strlen(lpBuff), &NumToWrite, NULL);

     /*用于分行*/ WriteFile(FileHandle, "\x0d\x0a", strlen("\x0d\x0a"), &NumToWrite, NULL);

     if (!SetEndOfFile(FileHandle)) msg( "设置结尾不成功"); // 写完数据置尾

     CloseHandle( FileHandle );    FileHandle = INVALID_HANDLE_VALUE; // 收尾关闭句柄
     lpBuff[0] = NULL;
      if (show) showResult(0);
     } // 完成句柄有效时的执行
     } // End of if (lpBuff != NULL)
      active = false; // dolog = false;
     } // End of if (active)      
     return ;
   }
  // ******************************************************************
   RunTimeLog::~RunTimeLog()  // 析构函数
      {
    //  msg("析构中...");

      last(Lastshow);
      // msg("准备释放内存");
      // HeapFree( GetProcessHeap(), NULL, lpBuff); lpBuff = NULL;
      dolog = false;  Sleep(100);
      delete lpBuff; lpBuff = NULL; dolog = false;

      return; 
      }  // End of ~RunTimeLog()
   // End of  构造函数和析构函数

 //***************************************************************

   // 数字 到 字符串 的输出,返回值是一个字符串的指针
  // 不存在不成功的情况，亦不处理负数
  // 第一个参数是你要转换的数字，第二个参数是指定转换的最小长度
  // 第三个参数是字符串，把第一个字符作为占位符，可以为NULL，则采用默认

  LPSTR  RunTimeLog::dwNum2Str(DWORD val)
  {
    static char NumStr[18]="";
//    _ultoa( val, NumStr, 10 );
    wsprintf( NumStr, "%lu", val );
    return NumStr;
  }
  /*/
  LPSTR  dwNum2Str(DWORD val, DWORD width, LPSTR lpstr)
     {
       // 以下是数字到 ascii 对照表
       static char num[11]={48,49,50,51,52,53,54,55,56,57,48};
       static DWORD base[11]=   // 以下是构建一个基数表
          { 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
       static char array[16];  // 用来装载字符串的
       // array[0]=0;  //截断字符串
       DWORD index, i, st;
       index=0; st = 0;

       if (width>10) width=10; //  width 不能大于 10

       // 根据第三个参数来改变占位符的值
       if (lpstr&&lstrlen(lpstr))
          num[10]=lpstr[0];
         else num[10]=num[0];

       // 为了防止零作为被除数
       if (val==0)
          {
            for ( i=width; i>1; i-- )  // 完成占位
               { array[index]=num[10];  index++; }
            array[index]=num[0]; array[index+1]=0; return array;
          } // End of if (val==0)

       // 正常操作的开始
       for ( i=10; i>0; i--)  // 这是有占位要求的循环
          {
           if (!st)
              {
               if (val>=base[i])
                  { st=1; i++; continue; }
                else if (!(i>width))  // 准备填写占位符
                   { array[index]=num[10];  index++; }
              }
          else
             { // { array[index]=num[val/base[i]];  val = val % base[i];  index++; msg(array); }
              _asm {
                mov  eax,  val      // 这是被除数
                mov  edx,  i      // 注意，数组下标一定要是寄存器
                sal  edx,  2      // 右移两位等于 i*4
                mov  ebx,  base[edx]
                xor  edx,  edx      // 做除法前一定要清空 edx
                div  ebx      // 做除法: 现在 dx 里面有余数，ax 里面则是商
                mov  val,  edx      // 保存余数
                add  eax,  30h      // 调整为 ASCii 码
                mov  ecx,  index
                mov  byte  ptr array[ecx], al    // 保存最后字符结果到数组
                add  index, 1       // 相当于 index ++
                 }  // End of _asm {
             }
          }  //End of for (DWORD i=...
       array[index]=0; // 最后截断字符串
       return array;
     }  // */
   // ******************************************************************

  //***** 自定义函数 ***********************************
  DWORD  RunTimeLog::hlstrcpy(LPSTR  Str1, LPSTR Str2, DWORD Len )
     {
       DWORD ii = 0;
       if (Len == 0)   // 如果没有给出长度，则自动算出长度
          { while(!(Str2[Len] == 0)) Len++; }
       if (! Len)        return 0;
       if (Str1==Str2)   return Len;
       if (Str1<Str2)
          { ii = 0;
            for ( ii; ii<Len; ii++ ) { Str1[ii] = Str2[ii]; } // 复制字符串
          }
       else{ ii = Len - 1;
             for ( ii; ii>0; ii-- )
                { Str1[ii] = Str2[ii]; }
                  Str1[0] = Str2[0];  // 补做一个循环
          }
       Str1[Len] = 0;  // 设置新字符串结尾
       return Len;
     }
   //******************************************************************
  DWORD  RunTimeLog::hlstrcat(LPSTR  Str, LPSTR Plus, DWORD StrLen, DWORD PlusLen )
     {
       if (StrLen == 0)   // 如果没有给出长度，则自动算出长度
       {   while(!(Str[StrLen] == 0)) StrLen++;   }
       if (PlusLen == 0)   // 如果没有给出长度，则自动算出长度
       {   while(!(Plus[PlusLen] == 0)) PlusLen++;   }
       DWORD ii = 0;  // 定义复制 plus 索引为 0
       for ( ii; ii < PlusLen; ii++ )  // 复制字符串
       {  Str[StrLen] = Plus[ii];   StrLen++;  }
          Str[StrLen] = 0;  // 设置新字符串结尾
       return StrLen;
     }

  //*******************************************************************
   LPSTR RunTimeLog::GetDate()
      {
        static char Date[15]; Date[0]=NULL;
        SYSTEMTIME SystemTime;
        GetLocalTime( &SystemTime );
        wsprintf(Date, "%d.%d.%d", SystemTime.wYear, SystemTime.wMonth, SystemTime.wDay);
        return Date;
      }
  //*******************************************************************
   LPSTR RunTimeLog::GetTime()
      {
        static char Time[15]; Time[0]=NULL;
        SYSTEMTIME SystemTime;
        GetLocalTime( &SystemTime );
        wsprintf(Time, "%d:%d:%d", SystemTime.wHour, SystemTime.wMinute, SystemTime.wSecond);
        return Time;
      }
  //*******************************************************************
  LPSTR RunTimeLog::GetSysTick(DWORD * len )
  {    //『御风而行』
       static char tick[20]="";
       static DWORD dwPasTick=GetTickCount(), dwCurTick=GetTickCount();
       dwPasTick = dwCurTick;
       dwCurTick = GetTickCount();
       tick[0]=NULL;  // 清除原来的字符串
       if ((dwPasTick != dwCurTick) && group ) lstrcat( tick, "\x0d\x0a" ); // 如果前后时间不一致，则插入分行
        lstrcat( tick, "[" ); // 预插入行首
       // lstrcat( tick, dwNum2Str(dwCurTick, NULL, NULL) );
        lstrcat( tick, dwNum2Str(dwCurTick) );
       *len = hlstrcat( tick, "] ", NULL, 2 );  // 补上结尾
       return tick;
     }
   // ******************************************************************

   int RunTimeLog::write(LPSTR lpStr)  // 重载为一个参数的函数
   {   DWORD len,lenth,TickLen;
       LPSTR temp = NULL;
       len = strlen(lpStr);
       if (len&&(!nosystime)) { temp = GetSysTick(&NumToWrite);  TickLen = NumToWrite; }
       else { TickLen = 0; }
       lenth = len + TickLen + 2;


       if ( lenth > BuffSize )
       { // msg("长度太大,无法写入缓冲");
          if (FileHandle != INVALID_HANDLE_VALUE)
          { 
             WriteFile(FileHandle, temp, TickLen, &NumToWrite, NULL); // 写入系统时间
             WriteFile(FileHandle, lpStr, len, &NumToWrite, NULL);    // 写入字符串内容
             /*用于分行*/ WriteFile(FileHandle, "\x0d\x0a", strlen("\x0d\x0a"), &NumToWrite, NULL);
             SetEndOfFile(FileHandle);
             return 1;
          } 
          return 0; // len 大于 BuffSize 而且文件句柄不可用
       } // End of if ( len > BuffSize )

       if (  (offset + lenth) > BuffSize) WriteLog();

       if (active) // 如果是活跃的才提供写入
       {   while (!dolog);   dolog = false;

           if (TickLen) offset = hlstrcat( lpBuff, temp, offset, TickLen );     // 写系统时间

           if (len) offset = hlstrcat( lpBuff, lpStr, offset, len );        // 写入字符串
           offset = hlstrcat( lpBuff, "\x0d\x0a", offset, NULL );  // 追加分行
           dolog = true;
           if (nobuff) WriteLog();
           return 1;
       }

       return 0;
   }
  // ******************************************************************
   void RunTimeLog::LogOutput(TCHAR *szFormat, ...)
   {return;
	   TCHAR szBuffer[1024];  // Large buffer for long filenames or URLs
	   const size_t NUMCHARS = sizeof(szBuffer) / sizeof(szBuffer[0]);
	   const int LASTCHAR = NUMCHARS - 1;

	   // Format the input string
	   va_list pArgs;
	   va_start(pArgs, szFormat);

	   // Use a bounded buffer size to prevent buffer overruns.  Limit count to
	   // character size minus one to allow for a NULL terminating character.
	   _vsntprintf(szBuffer, NUMCHARS - 1, szFormat, pArgs);
	   va_end(pArgs);

	   // Ensure that the formatted string is NULL-terminated
	   szBuffer[LASTCHAR] = TEXT('\0');
	   write(szBuffer);
	   WriteLog();
   }
  // ******************************************************************
   int RunTimeLog::WriteLog(void)
   {
       if (active && offset)
       {
       while (!dolog);
       dolog = false;


       if (FileHandle == INVALID_HANDLE_VALUE) { msg("最后 写入 LogFile 过程中出现问题" ); return 0;}
       else
       {  // 句柄有效的时候才执行
         WriteFile(FileHandle, lpBuff, offset, &NumToWrite, NULL);
         offset = 0; // 写入完以后记得要置 0
         lpBuff[0]=NULL; // lpBuff[1]=NULL;

         if (!SetEndOfFile(FileHandle)) msg( "设置结尾不成功"); // 写完数据置尾

       } // 完成句柄有效时的执行
        dolog = true;
           return 1;
       }
       else return 0;
   }
  // ******************************************************************
   void   RunTimeLog::msg(LPSTR Str) { MessageBox( NULL, Str, "??", MB_OK|MB_TOPMOST|MB_SETFOREGROUND );  return; }

  // ******************************************************************
   int   RunTimeLog::numberwrite(LPSTR lpStr, DWORD val)
   {
       char Temp[30];
       wsprintf( Temp, " 0x%X (%lu)", val, val );
       write(lpStr);
       if (offset) offset = offset -2;
       else { SetEndOfFile(FileHandle); SetFilePointer(FileHandle, GetFileSize(FileHandle,NULL)-2, NULL, FILE_BEGIN); }
       nosystime = true;
       write(Temp);
       nosystime = false;
       return 1L;
   }
   // ******************************************************************
   int   RunTimeLog::showResult(DWORD delay)
   {
      if (active)
      {
      WriteLog();
      while (!dolog);  dolog = false;

      GetWindowsDirectory( lpBuff, BuffSize);
      DWORD ExecLen = hlstrcat( lpBuff, "\\notepad.exe \"", NULL, NULL );
      ExecLen = hlstrcat( lpBuff, logfile, ExecLen, NULL );
      ExecLen = hlstrcat( lpBuff, "\"", NULL, NULL );


      PROCESS_INFORMATION pi;
      STARTUPINFO         si;
      memset(&si, 0, sizeof(si));
      si.cb           = sizeof(si);
      si.dwFlags      = STARTF_USESHOWWINDOW;
      si.wShowWindow  = SW_MAXIMIZE; // SW_SHOW;

      CreateProcess( NULL, lpBuff, NULL, NULL, false, 0, NULL, NULL, &si, &pi);


      lpBuff[0] = NULL;
      if(delay>100000) delay = 150;
      if (delay) Sleep(delay);
      dolog = true;
      return 1L;
      }  else return 0L;
   }


 // *************************** End of File *****************************