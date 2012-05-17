//CLog.cpp

#include "CLog.h"

CLog::CLog()
{
	bSigned = false;
	bTimeSigned = false;
	bConsel = false;
	m_iLine = 0;
	GetLocalTime( &time_start );
	strTime = new char[256];
	sprintf( strTime, "%d��%d��%d�գ�%dʱ%d��%d��", time_start.wYear, time_start.wMonth, time_start.wDay, time_start.wHour, time_start.wMinute, time_start.wSecond );
	char *fileName;
	m_fileName = new char[256];
	fileName = new char[256];

	//Ĭ�ϴ���Debug.log�ļ�
	sprintf( fileName, ".\\Debug.log" );
	ptr_fileList_head = new FILELIST;
	if( (ptr_fileList_head->file = fopen( fileName, "w" ) ) == NULL )
		MessageBox( 0, "���ļ�ʧ��!", 0, 0 );
	ptr_fileList_head->uId = 1;
	ptr_fileList_head->filename = new char[256];
	ptr_fileList_head->iEcount = 0;
	strcpy( ptr_fileList_head->filename, fileName );
	ptr_fileList_head->next = NULL;
	ptr_fileList = ptr_fileList_head;
	delete fileName;
}

CLog::~CLog()
{
	clear();
	if( m_fileName )
	{
		delete []m_fileName;
	}
	if( strTime )
	{
		delete []strTime;
	}
}

void CLog::clear()
{
	char *str = new char[256];
	//�ر��Ѵ��ļ�
	ptr_fileList = ptr_fileList_head;
	while( ptr_fileList->next != NULL )
	{
		DrawLine( 1, ptr_fileList->uId );
		sprintf( str, "��������	%d", ptr_fileList->iEcount );
		Report( str,ptr_fileList->uId ); 
		fclose( ptr_fileList->file );
		ptr_fileList = ptr_fileList->next;
	}
	DrawLine( 1, ptr_fileList->uId );
	sprintf( str, "��������	%d", ptr_fileList->iEcount );
	Report( str,ptr_fileList->uId );
	fclose( ptr_fileList->file );	
	//ɾ��ָ������
	ptr_fileList = ptr_fileList_head;
	while( ptr_fileList->next != NULL )
	{
		PTR_FILELIST ptr_tmp;
		ptr_tmp = ptr_fileList;
		delete []ptr_tmp->filename;
		ptr_fileList = ptr_fileList->next;
		delete ptr_tmp;
	}
	delete []ptr_fileList->filename;

	delete ptr_fileList;
}

void CLog::GetLogTime()
{
	GetLocalTime( &time_end );
	sprintf( strTime, 	"%d��%d��%d�գ�%dʱ%d��%d��", time_end.wYear, time_end.wMonth, time_end.wDay, time_end.wHour, time_end.wMinute, time_end.wSecond );
}
/*************************************************************
*				������: OpenNewFile(����)
*				����:	���ļ���û���򴴽�
*				����:	filename-�ļ���
						uId-Ҫ���ڵ��ļ���ID��
*************************************************************/
void CLog::OpenNewFile( char *filename, unsigned int ufileId )
{
	//char *str = new char[256];
	//strcpy( str,filename );
	//ָ���β
	ptr_fileList = ptr_fileList_head;
	while( ptr_fileList->next != NULL )
	{
		if( ptr_fileList->uId == ufileId )
		{
			MessageBox( 0, "�ļ�ID�Ѷ���", 0, 0 );
			return;
		}
		ptr_fileList = ptr_fileList->next;
	}
	ptr_fileList->next = new FILELIST;
	ptr_fileList->next->file = fopen( filename, "w" );
	ptr_fileList->next->filename = new char[256];
	strcpy( ptr_fileList->next->filename, filename );
	ptr_fileList->next->uId = ufileId;
	ptr_fileList->next->iEcount = 0;
	ptr_fileList->next->next = NULL;
	ptr_fileList = ptr_fileList->next;

	//delete []str;

}
/*************************************************************
*				������: DrawLine(����)
*				����:	��־
*				����:	style-��������
*************************************************************/
void CLog::DrawLine( int style, unsigned int fileId )
{
	ptr_fileList = ptr_fileList_head;
	while( ptr_fileList->uId != fileId && ptr_fileList->next != NULL )
	{
		ptr_fileList = ptr_fileList->next;
	}
	if( ptr_fileList->uId != fileId )
	{
		MessageBox( 0, "OpenFile,Error!", 0, 0 );
		return ;
	}
	FILE *file;
	file = ptr_fileList->file;
	char *fileName = ptr_fileList->filename;
	file = ptr_fileList->file;

#ifdef DEBUG
	{
		if( file )
		{
			fclose( file );
		}
		if( (file = fopen( fileName, "a"))==NULL )
		{
			MessageBox( 0, "���ļ�ʧ��!", 0, 0 );
		}
	}
#endif
	if( style == 1 )
	{
		fprintf( file , "*******************************************************************************\n" );
		if( bConsel )
		{
			printf( "********************************************************************************\n" );
		}
	}
	else if( style == 2 )
	{
		fprintf( file, "................................................................................\n" );
		if( bConsel )
		{
			printf( "................................................................................\n" );
		}
	}
	else if( style == 3 )
	{
		fprintf( file, "--------------------------------------------------------------------------------\n" );
		if( bConsel )
		{
			printf( "--------------------------------------------------------------------------------\n" );
		}
	}
	else
	{
		fprintf( file, "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n" );
		if( bConsel )
		{
			printf( "--------------------------------------------------------------------------------\n" );
		}
	}
}

/*************************************************************
*				������: Report(����)
*				����:	�������
*				����:	str-�����Ϣ
*						fileId-�ļ�ID
*************************************************************/
void CLog::Report( char *str, unsigned int fileId )
{
	//Ѱ���ļ�
	ptr_fileList = ptr_fileList_head;
	while( ptr_fileList->uId != fileId && ptr_fileList->next != NULL )
	{
		ptr_fileList = ptr_fileList->next;
	}
	if( ptr_fileList->uId != fileId )
	{
		MessageBox( 0, "OpenFile,Error!", 0, 0 );
		return ;
	}
	FILE *file;
	char *fileName = ptr_fileList->filename;
	file = ptr_fileList->file;
#ifdef DEBUG
	{
		if( file )
		{
			fclose( file );
		}
		if( (file = fopen( fileName, "a"))==NULL )
		{
			MessageBox( 0, "���ļ�ʧ��!", 0, 0 );
		}
	}
#endif
	//������ļ�

	if( bSigned && !bTimeSigned)
	{
		fprintf( file, "%s", str );
		fprintf( file, "  FILE��%s,LINE��%d\n",m_fileName, m_iLine);
		if( bConsel )
		{
			printf( "%s", str );
			printf( " FILE��%s,LINE��%d\n",m_fileName, m_iLine );
		}
	}
	else if( bTimeSigned && !bSigned )
	{
		GetLogTime();
		fprintf( file, "%s", str );
		fprintf( file, "  TIME��%s\n", strTime );
		if( bConsel )
		{
			printf( "%s", str );
			printf(  " TIME��%s\n", strTime  );
		}
	}
	else if( bSigned && bTimeSigned )
	{
		fprintf( file, "%s", str );
		fprintf( file, "  FILE��%s,LINE��%d",m_fileName, m_iLine);
		fprintf( file, "  TIME��%s\n", strTime );
		if( bConsel )
		{
			printf( "%s", str );
			printf( " FILE��%s,LINE��%d\n",m_fileName, m_iLine );
		}
	}
	else
	{
		fprintf( file, "%s\n", str );
		//consel����
		if( bConsel )
		{
			printf( "%s\n", str );
		}
	}
}
/*************************************************************
*				������: ReportError(����)
*				����:	�����쳣
*				����:	EInfo-�쳣��Ϣ
*						fileId-�ļ�ID
*************************************************************/
void CLog::ReportError( char *EInfo,unsigned int fileId )
{
	ptr_fileList = ptr_fileList_head;
	while( ptr_fileList->uId != fileId && ptr_fileList->next != NULL )
	{
		ptr_fileList = ptr_fileList->next;
	}
	if( ptr_fileList->uId != fileId )
	{
		MessageBox( 0, "OpenFile,Error!", 0, 0 );
		return ;
	}
	ptr_fileList->iEcount++;
	char *Error;
	Error = new char[256];
	strcpy( Error, "ERROR��" );
	strcat( Error, EInfo );
	Report( Error, fileId );
	delete []Error;
}

/*************************************************************
*				������: ReportNormal(����)
*				����:	��������
*				����:	NInfo-������Ϣ
*						fielID-�ļ�ID
*************************************************************/
void CLog::ReportNormal( char *NInfo,unsigned int fileId )
{
	char *Normal;
	Normal = new char[256];
	strcpy( Normal, "NORMAL��" );
	strcat( Normal, NInfo );
	Report( Normal, fileId );
	delete []Normal;
}
/*************************************************************
*				������: ReportVariable(����)
*				����:	�������
*				����:	VariableName-������
*						value-����ֵ
*						fileId-�ļ�ID
*************************************************************/
void CLog::ReportVariable( char *VariableName, char *value, unsigned int fileId )
{
	char *VariableInfo;
	VariableInfo = new char[100];
	strcpy( VariableInfo, "VARIABLE��" );
	strcat( VariableInfo, VariableName );
	sprintf( VariableInfo, "%s=%s", VariableInfo, value );
	Report( VariableInfo, fileId );
	delete []VariableInfo;
}

void CLog::ReportVariable( char *VariableName, int value, unsigned int fileId )
{
	char *VariableInfo;
	VariableInfo = new char[100];
	strcpy( VariableInfo, "VARIABLE��" );
	strcat( VariableInfo, VariableName );
	sprintf( VariableInfo, "%s=%d", VariableInfo, value );
	Report( VariableInfo, fileId );
	delete []VariableInfo;
}
 
void CLog::ReportVariable( char *VariableName, unsigned int value, unsigned int fileId )
{
	char *VariableInfo;
	VariableInfo = new char[100];
	strcpy( VariableInfo, "VARIABLE��" );
	strcat( VariableInfo, VariableName );
	sprintf( VariableInfo, "%s=%d", VariableInfo, value );
	Report( VariableInfo, fileId );
}

void CLog::ReportVariable( char *VariableName, double value, unsigned int fileId )
{
	char *VariableInfo;
	VariableInfo = new char[100];
	strcpy( VariableInfo, "VARIABLE��" );
	strcat( VariableInfo, VariableName );
	sprintf( VariableInfo, "%s=%l", VariableInfo, value );
	Report( VariableInfo, fileId );
	delete []VariableInfo;
}

void CLog::ReportVariable( char *VariableName, float value, unsigned int fileId )
{
	char *VariableInfo;
	VariableInfo = new char[100];
	strcpy( VariableInfo, "VARIABLE��" );
	strcat( VariableInfo, VariableName );
	sprintf( VariableInfo, "%s=%f", VariableInfo, value );
	Report( VariableInfo, fileId );
	delete []VariableInfo;
}

void CLog::ReportVariable( char *VariableName, std::string value, unsigned int fileId )
{
	char *VariableInfo;
	VariableInfo = new char[100];
	strcpy( VariableInfo, "VARIABLE��" );
	strcat( VariableInfo, VariableName );
	sprintf( VariableInfo, "%s=%s", VariableInfo, value.c_str() );
	Report( VariableInfo, fileId );
	delete []VariableInfo;
}