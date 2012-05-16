
----------------------------------------------------------------------------------------------------

USE QPServerInfoDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_LoadDataBaseInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_LoadDataBaseInfo]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- ������Ϣ
CREATE PROCEDURE GSP_LoadDataBaseInfo
	@strDBAddr NVARCHAR(15)						-- ���ݿ��ַ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ��������
DECLARE @DBPort INT
DECLARE @DBAddr NVARCHAR(15)
DECLARE @DBUser NVARCHAR(512)
DECLARE @DBPassword NVARCHAR(512)
DECLARE @ErrorDescribe NVARCHAR(128)

-- ִ���߼�
BEGIN

	-- ��ѯ��Ϣ
	SELECT @DBAddr=DBAddr, @DBPort=DBPort, @DBUser=DBUser, @DBPassword=DBPassword 
		FROM DataBaseInfo(NOLOCK) WHERE DBAddr=@strDBAddr
	
	-- �����ж�
	IF @DBAddr IS NULL
	BEGIN
		SET @ErrorDescribe=N'���ݿ� [ '+@strDBAddr+N' ] ������Ϣ�����ڣ����� QPServerInfoDB ���ݿ�� SQLDBConnectInfo ������'
		SELECT [ErrorDescribe]=@ErrorDescribe
		RETURN 1
	END
	
	-- �������
	SELECT @DBAddr AS DBAddr, @DBPort AS DBPort, @DBUser AS DBUser, @DBPassword AS DBPassword
	
END

RETURN 0

----------------------------------------------------------------------------------------------------

GO
