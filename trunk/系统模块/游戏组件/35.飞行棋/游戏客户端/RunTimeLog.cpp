 // ****************************************************************************
 // *******   FileName: RunTimeLog.cpp   ***************************************
 // ******************   Version: 1.12   ***************************************
 // ****************************************************************************
 // ���������ļ���������޸ġ�ʹ�� �� ת�أ������˲���ʹ�ø�����ɵ��κκ������
 // ���ߣ�`����   ************  E-mail��  free77@163.net************************
 // ****************************************************************************
 //  �����ں����ڲ���������࣬���µ�ʹ�� new �Ķ��巽���� *********************
 //  RunTimeLog * rtlog = new RunTimeLog;   Ȼ�������  delete rtlog;  *********
 //  Ҳ��ʹ�����·������壺 RunTimeLog  log;  �������ʽ���� ��������  *********
 // ****************************************************************************

 // �����ṩ�Ľӿ� ��Ҫ�� ::write(""); ����д����Ϣ�������ڴ��У�
 // �Լ� ::last(); �������������֣������� .log �ļ�д�뻺������
 // Ҳ����ֱ�ӵ��� ::WriteLog(); ���� .log �ļ�д�뻺������(����еĻ�)��
 // Ver 1.02 ������Ϣ��ʱ����鹦�ܣ�ʹ�� group ���Կ����Ƿ����
 // �������Զ�д���嵽���̴���...  �����廻��2048
#include "Stdafx.h"
#include <windows.h>
 #include "RunTimeLog.h"


   // ���캯������������
  RunTimeLog::RunTimeLog()  // �����������캯��
  {return;
    nobuff=false;  Lastshow=false;   dolog=false;
    active=false;  nosystime=false;  group = true;

    offset=NULL;    NumToWrite=NULL;
    SysTick=NULL;   FileHandle=NULL;
    lpBuff=NULL;    BuffSize = 2048; // ���� BuffSize �Ĵ�С

    bool   DelOld = false;        // �趨�Ƿ�Ҫ�ӵ�����־
    DWORD  OverWrite = true;     // �趨�Ƿ�Ҫ���Ǿ���־���������û�ж��Ļ�
    WORD   ToRecyclebin = false;  // �趨�Ƿ�Ѿ���־�ŵ�����վ

    OverWrite = OverWrite? CREATE_ALWAYS : CREATE_NEW; // һ���궨��
    ToRecyclebin = ToRecyclebin ? FOF_ALLOWUNDO | FOF_SILENT | FOF_NOCONFIRMATION : FOF_SILENT | FOF_NOCONFIRMATION;

 

    //��������õ��ڴ�������ռ䣬ָ�� lpBuff, ��СΪ BuffSize

    // lpBuff = (char*)HeapAlloc( GetProcessHeap(), HEAP_ZERO_MEMORY, BuffSize);
     lpBuff = new char[BuffSize];
    // lpBuff = Buffer;

    if ( lpBuff != NULL )
    {
     //��ȡ�ý����ļ������仯�� .log �ļ����������Ƿ��ظ��ˣ������͵�����վ
      GetModuleFileName( NULL,lpBuff,BuffSize);
      hlstrcpy(logfile, lpBuff, NULL);
      GetLongPathName( logfile, logfile, 512);  // ������� Win98 �е��øú��������ܻ�������ļ�����ʽ
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
      // Ȼ��Ӻ���ȥ����չ��
      for ( i=len; i>0; i-- )
      { if (logfile[i]=='.') { hlstrcat(logfile, ".log", i, 4); len = len + 3; break; } }

 


     
    // �����ж��ļ��Ƿ����
    Loop: // �������һ�δ����ļ��ɹ�ʱ�á�
    if (( GetFileAttributes( logfile ) == -1 )||(GetLastError()==ERROR_FILE_NOT_FOUND))
       ;// msg("�ļ�������");

    else if(DelOld)  // �ô���� true ��Ҫɾ��ԭ�ļ�

    {  // �����ļ��Ѿ������ˣ���Ҫ�ӵ�����վ����ȥ
    SHFILEOPSTRUCTA Fileop;
    LPSHFILEOPSTRUCT lpFileOp = &Fileop;
    memset( lpFileOp, NULL, sizeof(Fileop) );

 

    Fileop.fFlags = ToRecyclebin;
    Fileop.wFunc = FO_DELETE ;
    Fileop.pFrom = logfile;
    lpFileOp->hwnd = NULL;

    //*
    // ��������ʱ���ӵ��ļ�������ʽ�������� ������ָ��
    SHELLFILE * lpSHFileOperation = NULL;  // ����ָ�붨��
    HMODULE hlib = LoadLibrary( "shell32.dll" );
     if (hlib)
     {  
       lpSHFileOperation = (SHELLFILE *) GetProcAddress( hlib, "SHFileOperation" );
       if ( lpSHFileOperation )
       {
         /*if (*/ lpSHFileOperation(lpFileOp); //msg( "SHFileOperation ִ�й����г���������");
       }
       else msg("shell32.dll ��û���������");
       if (! FreeLibrary( hlib )) msg("�ͷ�DLLʧ��");
     }
    else msg("û�� shell32.dll"); //*/

    }  // ���������


    //������һ�����ļ���д�뿪ʼ��¼��ʱ��
     FileHandle = CreateFile( logfile, GENERIC_WRITE, FILE_SHARE_READ, NULL, OverWrite, FILE_ATTRIBUTE_NORMAL, NULL);
    if (FileHandle == INVALID_HANDLE_VALUE)
    {  // �����ܴ������ļ���ʱ���������ļ���׺
       if (hlstrcat( logfile, ".log", NULL, 4) < 125)  goto Loop;
       else { msg("CreateFile ִ�й����г��������⣬�������ԡ�" ); /*ExitProcess(0);*/}
    }
    else {  // �����Ч��ʱ���ִ��
      SetFilePointer(FileHandle, GetFileSize(FileHandle,NULL), NULL, FILE_BEGIN);  // ����ԭʼ��С

        WriteFile(FileHandle, "[Source Path & FileName:]  ", strlen("[Source Path & FileName:]  "), &NumToWrite, NULL);
        WriteFile(FileHandle, logfile, strlen(logfile), &NumToWrite, NULL);
        /*���ڷ���*/ WriteFile(FileHandle, "\x0d\x0a", strlen("\x0d\x0a"), &NumToWrite, NULL);

        wsprintf( lpBuff, "Record Start at  Date: %s  &  Time: %s .", GetDate(), GetTime() );

        SysTick = GetSysTick(&NumToWrite);  // ȡ��ϵͳʱ��
        WriteFile(FileHandle, SysTick, NumToWrite, &NumToWrite, NULL);

        WriteFile(FileHandle, lpBuff, strlen(lpBuff), &NumToWrite, NULL);

        /*���ڷ���*/ WriteFile(FileHandle, "\x0d\x0a", strlen("\x0d\x0a"), &NumToWrite, NULL);

      if (!SetEndOfFile(FileHandle)) MessageBox(NULL, "���ý�β���ɹ�", "??", MB_OK|MB_TOPMOST); // д��������β

      // CloseHandle( FileHandle );    FileHandle = INVALID_HANDLE_VALUE; // ��β�رվ��
       lpBuff[0] = NULL;
       } // ��ɾ����Чʱ��ִ��
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

    // д�������¼��ʱ��
     if (active)
     {
       WriteLog();
     if (lpBuff != NULL)
     {
     if (FileHandle == INVALID_HANDLE_VALUE) { msg("��� WriteLog �����г���������"); ExitProcess(0);}
     else 
     {  // �����Ч��ʱ���ִ��

     /*���ڷ���*/ WriteFile(FileHandle, "\x0d\x0a", strlen("\x0d\x0a"), &NumToWrite, NULL);
     SysTick = GetSysTick(&NumToWrite);  // ȡ��ϵͳʱ��
     WriteFile(FileHandle, SysTick, NumToWrite, &NumToWrite, NULL);

     wsprintf( lpBuff, "Record  End  at  Date: %s  &  Time: %s .", GetDate(), GetTime() );
     WriteFile(FileHandle, lpBuff, strlen(lpBuff), &NumToWrite, NULL);

     /*���ڷ���*/ WriteFile(FileHandle, "\x0d\x0a", strlen("\x0d\x0a"), &NumToWrite, NULL);

     if (!SetEndOfFile(FileHandle)) msg( "���ý�β���ɹ�"); // д��������β

     CloseHandle( FileHandle );    FileHandle = INVALID_HANDLE_VALUE; // ��β�رվ��
     lpBuff[0] = NULL;
      if (show) showResult(0);
     } // ��ɾ����Чʱ��ִ��
     } // End of if (lpBuff != NULL)
      active = false; // dolog = false;
     } // End of if (active)      
     return ;
   }
  // ******************************************************************
   RunTimeLog::~RunTimeLog()  // ��������
      {
    //  msg("������...");

      last(Lastshow);
      // msg("׼���ͷ��ڴ�");
      // HeapFree( GetProcessHeap(), NULL, lpBuff); lpBuff = NULL;
      dolog = false;  Sleep(100);
      delete lpBuff; lpBuff = NULL; dolog = false;

      return; 
      }  // End of ~RunTimeLog()
   // End of  ���캯������������

 //***************************************************************

   // ���� �� �ַ��� �����,����ֵ��һ���ַ�����ָ��
  // �����ڲ��ɹ���������಻������
  // ��һ����������Ҫת�������֣��ڶ���������ָ��ת������С����
  // �������������ַ������ѵ�һ���ַ���Ϊռλ��������ΪNULL�������Ĭ��

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
       // ���������ֵ� ascii ���ձ�
       static char num[11]={48,49,50,51,52,53,54,55,56,57,48};
       static DWORD base[11]=   // �����ǹ���һ��������
          { 0, 1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000 };
       static char array[16];  // ����װ���ַ�����
       // array[0]=0;  //�ض��ַ���
       DWORD index, i, st;
       index=0; st = 0;

       if (width>10) width=10; //  width ���ܴ��� 10

       // ���ݵ������������ı�ռλ����ֵ
       if (lpstr&&lstrlen(lpstr))
          num[10]=lpstr[0];
         else num[10]=num[0];

       // Ϊ�˷�ֹ����Ϊ������
       if (val==0)
          {
            for ( i=width; i>1; i-- )  // ���ռλ
               { array[index]=num[10];  index++; }
            array[index]=num[0]; array[index+1]=0; return array;
          } // End of if (val==0)

       // ���������Ŀ�ʼ
       for ( i=10; i>0; i--)  // ������ռλҪ���ѭ��
          {
           if (!st)
              {
               if (val>=base[i])
                  { st=1; i++; continue; }
                else if (!(i>width))  // ׼����дռλ��
                   { array[index]=num[10];  index++; }
              }
          else
             { // { array[index]=num[val/base[i]];  val = val % base[i];  index++; msg(array); }
              _asm {
                mov  eax,  val      // ���Ǳ�����
                mov  edx,  i      // ע�⣬�����±�һ��Ҫ�ǼĴ���
                sal  edx,  2      // ������λ���� i*4
                mov  ebx,  base[edx]
                xor  edx,  edx      // ������ǰһ��Ҫ��� edx
                div  ebx      // ������: ���� dx ������������ax ����������
                mov  val,  edx      // ��������
                add  eax,  30h      // ����Ϊ ASCii ��
                mov  ecx,  index
                mov  byte  ptr array[ecx], al    // ��������ַ����������
                add  index, 1       // �൱�� index ++
                 }  // End of _asm {
             }
          }  //End of for (DWORD i=...
       array[index]=0; // ���ض��ַ���
       return array;
     }  // */
   // ******************************************************************

  //***** �Զ��庯�� ***********************************
  DWORD  RunTimeLog::hlstrcpy(LPSTR  Str1, LPSTR Str2, DWORD Len )
     {
       DWORD ii = 0;
       if (Len == 0)   // ���û�и������ȣ����Զ��������
          { while(!(Str2[Len] == 0)) Len++; }
       if (! Len)        return 0;
       if (Str1==Str2)   return Len;
       if (Str1<Str2)
          { ii = 0;
            for ( ii; ii<Len; ii++ ) { Str1[ii] = Str2[ii]; } // �����ַ���
          }
       else{ ii = Len - 1;
             for ( ii; ii>0; ii-- )
                { Str1[ii] = Str2[ii]; }
                  Str1[0] = Str2[0];  // ����һ��ѭ��
          }
       Str1[Len] = 0;  // �������ַ�����β
       return Len;
     }
   //******************************************************************
  DWORD  RunTimeLog::hlstrcat(LPSTR  Str, LPSTR Plus, DWORD StrLen, DWORD PlusLen )
     {
       if (StrLen == 0)   // ���û�и������ȣ����Զ��������
       {   while(!(Str[StrLen] == 0)) StrLen++;   }
       if (PlusLen == 0)   // ���û�и������ȣ����Զ��������
       {   while(!(Plus[PlusLen] == 0)) PlusLen++;   }
       DWORD ii = 0;  // ���帴�� plus ����Ϊ 0
       for ( ii; ii < PlusLen; ii++ )  // �����ַ���
       {  Str[StrLen] = Plus[ii];   StrLen++;  }
          Str[StrLen] = 0;  // �������ַ�����β
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
  {    //��������С�
       static char tick[20]="";
       static DWORD dwPasTick=GetTickCount(), dwCurTick=GetTickCount();
       dwPasTick = dwCurTick;
       dwCurTick = GetTickCount();
       tick[0]=NULL;  // ���ԭ�����ַ���
       if ((dwPasTick != dwCurTick) && group ) lstrcat( tick, "\x0d\x0a" ); // ���ǰ��ʱ�䲻һ�£���������
        lstrcat( tick, "[" ); // Ԥ��������
       // lstrcat( tick, dwNum2Str(dwCurTick, NULL, NULL) );
        lstrcat( tick, dwNum2Str(dwCurTick) );
       *len = hlstrcat( tick, "] ", NULL, 2 );  // ���Ͻ�β
       return tick;
     }
   // ******************************************************************

   int RunTimeLog::write(LPSTR lpStr)  // ����Ϊһ�������ĺ���
   {   DWORD len,lenth,TickLen;
       LPSTR temp = NULL;
       len = strlen(lpStr);
       if (len&&(!nosystime)) { temp = GetSysTick(&NumToWrite);  TickLen = NumToWrite; }
       else { TickLen = 0; }
       lenth = len + TickLen + 2;


       if ( lenth > BuffSize )
       { // msg("����̫��,�޷�д�뻺��");
          if (FileHandle != INVALID_HANDLE_VALUE)
          { 
             WriteFile(FileHandle, temp, TickLen, &NumToWrite, NULL); // д��ϵͳʱ��
             WriteFile(FileHandle, lpStr, len, &NumToWrite, NULL);    // д���ַ�������
             /*���ڷ���*/ WriteFile(FileHandle, "\x0d\x0a", strlen("\x0d\x0a"), &NumToWrite, NULL);
             SetEndOfFile(FileHandle);
             return 1;
          } 
          return 0; // len ���� BuffSize �����ļ����������
       } // End of if ( len > BuffSize )

       if (  (offset + lenth) > BuffSize) WriteLog();

       if (active) // ����ǻ�Ծ�Ĳ��ṩд��
       {   while (!dolog);   dolog = false;

           if (TickLen) offset = hlstrcat( lpBuff, temp, offset, TickLen );     // дϵͳʱ��

           if (len) offset = hlstrcat( lpBuff, lpStr, offset, len );        // д���ַ���
           offset = hlstrcat( lpBuff, "\x0d\x0a", offset, NULL );  // ׷�ӷ���
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


       if (FileHandle == INVALID_HANDLE_VALUE) { msg("��� д�� LogFile �����г�������" ); return 0;}
       else
       {  // �����Ч��ʱ���ִ��
         WriteFile(FileHandle, lpBuff, offset, &NumToWrite, NULL);
         offset = 0; // д�����Ժ�ǵ�Ҫ�� 0
         lpBuff[0]=NULL; // lpBuff[1]=NULL;

         if (!SetEndOfFile(FileHandle)) msg( "���ý�β���ɹ�"); // д��������β

       } // ��ɾ����Чʱ��ִ��
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