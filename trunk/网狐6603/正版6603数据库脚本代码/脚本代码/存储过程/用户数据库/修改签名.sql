
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ModifyUnderWrite]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_ModifyUnderWrite]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- �޸�ǩ��
CREATE PROC GSP_GP_ModifyUnderWrite
	@dwUserID INT,								-- �û� I D	
	@strPassword NCHAR(32),						-- �û�����
	@strUnderWrite NVARCHAR(31),				-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��ѯ��Ϣ
	DECLARE @Password NCHAR(32)
	SELECT @Password=LogonPass FROM AccountsInfo WHERE UserID=@dwUserID

	-- �û��ж�
	IF @Password IS NULL
	BEGIN 
		SET @strErrorDescribe=N'�û���Ϣ�����ڣ�������ʧ�ܣ�'
		RETURN 1
	END

	-- �����ж�
	IF @Password<>@strPassword
	BEGIN 
		SET @strErrorDescribe=N'�������ṩ���ʺ����벻��ȷ������ǩ���޸�ʧ�ܣ�'
		RETURN 3
	END

	-- ����ǩ��
	UPDATE AccountsInfo SET UnderWrite=@strUnderWrite WHERE UserID=@dwUserID

	-- �ɹ��ж�
	IF @@ROWCOUNT=0
	BEGIN
		SET @strErrorDescribe=N'�޸ĸ���ǩ������ִ�д�������ϵ�ͻ��������ģ�'
		RETURN 4
	END

	RETURN 0

END

GO

----------------------------------------------------------------------------------------------------
