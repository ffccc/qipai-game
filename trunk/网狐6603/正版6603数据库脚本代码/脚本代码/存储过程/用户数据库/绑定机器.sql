
----------------------------------------------------------------------------------------------------

USE QPAccountsDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_MoorMachine]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_MoorMachine]
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_UnMoorMachine]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_UnMoorMachine]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- �󶨻���
CREATE PROC GSP_GP_MoorMachine
	@dwUserID INT,								-- �û� I D	
	@strPassword NCHAR(32),						-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strMachineSerial NVARCHAR(32),				-- ������ʶ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��ѯ��Ϣ
	DECLARE @Password NCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	DECLARE	@MachineSerial NCHAR(32)
	SELECT @Password=InsurePass, @MoorMachine=MoorMachine, @MachineSerial=LastLogonMachine FROM AccountsInfo WHERE UserID=@dwUserID

	-- �û��ж�
	IF @Password IS NULL
	BEGIN 
		SET @strErrorDescribe=N'�û���Ϣ�����ڣ�������ʧ�ܣ�'
		RETURN 1
	END

	-- ���ж�
	IF @MoorMachine=1 AND @MachineSerial<>@strMachineSerial
	BEGIN
		SET @strErrorDescribe=N'�����ʺ��Ѿ��������������ˣ�����������ܽ��б����󶨲�����'
		RETURN 2
	END

	-- �����ж�
	IF @Password<>@strPassword
	BEGIN 
		SET @strErrorDescribe=N'������������������벻��ȷ��������ʧ�ܣ�'
		RETURN 3
	END

	-- �󶨲���
	UPDATE AccountsInfo SET MoorMachine=1, LastLogonMachine=@strMachineSerial WHERE UserID=@dwUserID

	-- �ɹ��ж�
	IF @@ROWCOUNT=0
	BEGIN 
		SET @strErrorDescribe=N'�ʺŰ󶨲���ִ�д�������ϵ�ͻ��������ģ�'
		RETURN 4
	END

	-- �����Ϣ
	SET @strErrorDescribe=N'�����ʺ���˻����󶨳ɹ��ˣ�����Ҫ��������ڱ��������У�'
	
	RETURN 0

END

GO

----------------------------------------------------------------------------------------------------

-- �����
CREATE PROC GSP_GP_UnMoorMachine
	@dwUserID INT,								-- �û� I D	
	@strPassword NCHAR(32),						-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strMachineSerial NVARCHAR(32),				-- ������ʶ
	@strErrorDescribe NVARCHAR(127) OUTPUT		-- �����Ϣ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ִ���߼�
BEGIN

	-- ��ѯ��Ϣ
	DECLARE @Password NCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	DECLARE	@MachineSerial NCHAR(32)
	SELECT @Password=InsurePass, @MoorMachine=MoorMachine, @MachineSerial=LastLogonMachine FROM AccountsInfo WHERE UserID=@dwUserID

	-- �û��ж�
	IF @MachineSerial IS NULL
	BEGIN 
		SET @strErrorDescribe=N'�û���Ϣ�����ڣ�������ʧ�ܣ�'
		RETURN 1
	END

	-- �����ж�
	IF @MoorMachine=1 AND @MachineSerial<>@strMachineSerial
	BEGIN
		SET @strErrorDescribe=N'�����ʺ����������������˰󶨣����������ʧ�ܣ�'
		RETURN 5
	END

	-- �����ж�
	IF @Password<>@strPassword
	BEGIN
		SET @strErrorDescribe=N'������������������벻��ȷ�����������ʧ�ܣ�'
		RETURN 3
	END

	-- �������
	UPDATE AccountsInfo SET MoorMachine=0 WHERE UserID=@dwUserID

	-- �ɹ��ж�
	IF @@ROWCOUNT=0
	BEGIN 
		SET @strErrorDescribe=N'�ʺŽ���󶨲���ִ�д�������ϵ�ͻ��������ģ�'
		RETURN 4
	END

	-- �����Ϣ
	SET @strErrorDescribe=N'�����ʺ����������󶨳ɹ��ˣ�'
	
	RETURN 0

END

GO

----------------------------------------------------------------------------------------------------
