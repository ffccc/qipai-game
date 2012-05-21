
----------------------------------------------------------------------------------------------------

USE QPPlatformDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GS_LoadDataBaseInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GS_LoadDataBaseInfo]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- ������Ϣ
CREATE PROCEDURE GSP_GS_LoadDataBaseInfo
	@strDataBaseAddr NVARCHAR(15)				-- �б�����
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ������Ϣ
DECLARE @DBPort INT
DECLARE @DBAddr NVARCHAR(15)
DECLARE @DBName NVARCHAR(32)

-- ������Ϣ
DECLARE @DBUser NVARCHAR(512)
DECLARE @DBPassword NVARCHAR(512)

-- ִ���߼�
BEGIN

	-- ��ѯ��Ϣ
	SELECT @DBPort=DBPort, @DBAddr=DBAddr, @DBUser=DBUser, @DBPassword=DBPassword FROM DataBaseInfo(NOLOCK)
	WHERE DBAddr=@strDataBaseAddr
	
	-- �����ж�
	IF @@ROWCOUNT=0
	BEGIN
		SELECT N'���ݿ�������Ϣ�����ڣ����� QPPlatformDB ���ݿ�� DataBaseInfo ������' AS [ErrorDescribe]
		RETURN 1
	END
	
	-- �������
	SELECT @DBPort AS DBPort, @DBAddr AS DBAddr, @DBUser AS DBUser, @DBPassword AS DBPassword
	
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
