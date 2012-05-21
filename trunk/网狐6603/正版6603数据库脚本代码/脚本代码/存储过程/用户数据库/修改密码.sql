
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ModifyLogonPassword]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_ModifyLogonPassword]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_ModifyInsurePassword]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_ModifyInsurePassword]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- �޸�����
CREATE PROC GSP_GP_ModifyLogonPassword
	@dwUserID INT,								-- �û� I D
	@strSrcPassword NCHAR(32),					-- �û�����
	@strNewPassword NCHAR(32),					-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��������
	DECLARE @UserID INT
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE @LogonPass AS NCHAR(32)

	-- ��ѯ�û�
	SELECT @UserID=UserID, @LogonPass=LogonPass, @Nullity=Nullity, @StunDown=StunDown
	FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID

	-- ��ѯ�û�
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��ԣ�'
		RETURN 1
	END	

	-- �ʺŽ�ֹ
	IF @Nullity<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	

	-- �ʺŹر�
	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ�ʹ���˰�ȫ�رչ��ܣ��������¿�ͨ����ܼ���ʹ�ã�'
		RETURN 2
	END	
	
	-- �����ж�
	IF @LogonPass<>@strSrcPassword
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��ԣ�'
		RETURN 3
	END

	-- �����ж�
	IF @strNewPassword=N''
	BEGIN
		SET @strErrorDescribe=N'���ʺ�����Ϊ�գ���������������Ϊ�գ����֤���ٴγ��ԣ�'
		RETURN 3
	END

	-- �޸�����
	UPDATE AccountsInfo SET LogonPass=@strNewPassword WHERE UserID=@dwUserID

	-- ������Ϣ
	IF @@ERROR=0 
	BEGIN
		SET @strErrorDescribe=N'�ʺ������޸ĳɹ������μ��������ʺ����룡'
	END

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------

-- �޸�����
CREATE PROC GSP_GP_ModifyInsurePassword
	@dwUserID INT,								-- �û� I D
	@strSrcPassword NCHAR(32),					-- �û�����
	@strNewPassword NCHAR(32),					-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��������
	DECLARE @UserID INT
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE @InsurePass AS NCHAR(32)

	-- ��ѯ�û�
	SELECT @UserID=UserID, @InsurePass=InsurePass, @Nullity=Nullity, @StunDown=StunDown
	FROM AccountsInfo(NOLOCK) WHERE UserID=@dwUserID

	-- ��ѯ�û�
	IF @UserID IS NULL
	BEGIN
		SET @strErrorDescribe=N'�����ʺŲ����ڻ������������������֤���ٴγ��ԣ�'
		RETURN 1
	END	

	-- �ʺŽ�ֹ
	IF @Nullity<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	

	-- �ʺŹر�
	IF @StunDown<>0
	BEGIN
		SET @strErrorDescribe=N'�����ʺ�ʹ���˰�ȫ�رչ��ܣ��������¿�ͨ����ܼ���ʹ�ã�'
		RETURN 2
	END	
	
	-- �����ж�
	IF @InsurePass IS NOT NULL AND @InsurePass<>@strSrcPassword
	BEGIN
		SET @strErrorDescribe=N'�������������������֤���ٴγ��ԣ�'
		RETURN 3
	END

	-- �����ж�
	IF @strNewPassword=N''
	BEGIN
		SET @strErrorDescribe=N'���ʺ�����Ϊ�գ���������������Ϊ�գ����֤���ٴγ��ԣ�'
		RETURN 3
	END

	-- �޸�����
	UPDATE AccountsInfo SET InsurePass=@strNewPassword WHERE UserID=@dwUserID

	-- ������Ϣ
	IF @@ERROR=0 
	BEGIN
		SET @strErrorDescribe=N'���չ������޸ĳɹ������μ������±��չ����룡'
	END

	RETURN 0

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
