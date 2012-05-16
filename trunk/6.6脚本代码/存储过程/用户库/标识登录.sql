
----------------------------------------------------------------------------------------------------

USE QPGameUserDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_EfficacyGameID]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_EfficacyGameID]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- I D ��¼
CREATE PROC GSP_GP_EfficacyGameID
	@dwGameID INT,								-- ��Ϸ I D
	@strPassword NCHAR(32),						-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strMachineSerial NCHAR(32)					-- ������ʶ
WITH ENCRYPTION AS

-- ��������
SET NOCOUNT ON

-- ������Ϣ
DECLARE @UserID INT
DECLARE @FaceID INT
DECLARE @Accounts NVARCHAR(31)
DECLARE @UnderWrite NVARCHAR(63)

-- ��չ��Ϣ
DECLARE @GameID INT
DECLARE @Gender TINYINT
DECLARE @Experience INT
DECLARE @Loveliness INT
DECLARE @MemberOrder INT
DECLARE @MemberOverDate DATETIME
DECLARE @CustomFaceVer TINYINT

-- ��������
DECLARE @EnjoinLogon AS INT
DECLARE @ErrorDescribe AS NVARCHAR(128)

-- ִ���߼�
BEGIN
	-- Ч���ַ
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SELECT [ErrorDescribe]=N'��Ǹ��֪ͨ����ϵͳ��ֹ�������ڵ� IP ��ַ�ĵ�¼���ܣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 4
	END
	
	-- Ч�����
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineSerial AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SELECT [ErrorDescribe]=N'��Ǹ��֪ͨ����ϵͳ��ֹ�����Ļ����ĵ�¼���ܣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 7
	END
 
	-- ��ѯ�û�
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE @LogonPass AS NCHAR(32)
	DECLARE	@MachineSerial NCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	SELECT @UserID=UserID, @GameID=GameID, @Accounts=Accounts, @UnderWrite=UnderWrite, @LogonPass=LogonPass, @FaceID=FaceID,
		@Gender=Gender, @Nullity=Nullity, @StunDown=StunDown, @Experience=Experience, @MemberOrder=MemberOrder, @MemberOverDate=MemberOverDate, 
		@MoorMachine=MoorMachine, @MachineSerial=MachineSerial, @Loveliness=Loveliness,@CustomFaceVer=CustomFaceVer
	FROM AccountsInfo(NOLOCK) WHERE GameID=@dwGameID

	-- ��ѯ�û�
	IF @UserID IS NULL
	BEGIN
		SELECT [ErrorDescribe]=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 1
	END	

	-- �ʺŽ�ֹ
	IF @Nullity<>0
	BEGIN
		SELECT [ErrorDescribe]=N'�����ʺ���ʱ���ڶ���״̬������ϵ�ͻ����������˽���ϸ�����'
		RETURN 2
	END	

	-- �ʺŹر�
	IF @StunDown<>0
	BEGIN
		SELECT [ErrorDescribe]=N'�����ʺ�ʹ���˰�ȫ�رչ��ܣ��������¿�ͨ����ܼ���ʹ�ã�'
		RETURN 2
	END	
	
	-- �̶�����
	IF @MoorMachine=1
	BEGIN
		IF @MachineSerial<>@strMachineSerial
		BEGIN
			SELECT [ErrorDescribe]=N'�����ʺ�ʹ�ù̶�������½���ܣ�������ʹ�õĻ���������ָ���Ļ�����'
			RETURN 1
		END
	END

	-- �����ж�
	IF @LogonPass<>@strPassword
	BEGIN
		SELECT [ErrorDescribe]=N'�����ʺŲ����ڻ������������������֤���ٴγ��Ե�¼��'
		RETURN 3
	END

	-- �̶�����
	IF @MoorMachine=2
	BEGIN
		SET @MoorMachine=1
		SET @ErrorDescribe=N'�����ʺųɹ�ʹ���˹̶�������½���ܣ�'
		UPDATE AccountsInfo SET MoorMachine=@MoorMachine, MachineSerial=@strMachineSerial WHERE UserID=@UserID
	END

	-- ��Ա�ȼ�
	IF GETDATE()>=@MemberOverDate 
	BEGIN 
		SET @MemberOrder=0

		-- ɾ�����ڻ�Ա���
		DELETE FROM MemberInfo WHERE UserID=@UserID
	END
	ELSE 
	BEGIN
		DECLARE @MemberCurDate DATETIME

		-- ��ǰ��Աʱ��
		SELECT @MemberCurDate=MIN(MemberOverDate) FROM MemberInfo WHERE UserID=@UserID

		-- ɾ�����ڻ�Ա
		IF GETDATE()>=@MemberCurDate
		BEGIN
			-- ɾ����Ա���޹��ڵ����л�Ա���
			DELETE FROM MemberInfo WHERE UserID=@UserID AND MemberOverDate<=GETDATE()

			-- �л�����һ�����Ա���
			SELECT @MemberOrder=MAX(MemberOrder) FROM MemberInfo WHERE UserID=@UserID
			IF @MemberOrder IS NOT NULL
			BEGIN
				UPDATE AccountsInfo SET MemberOrder=@MemberOrder WHERE UserID=@UserID
			END
			ELSE SET @MemberOrder=0
		END
	END

	-- ������Ϣ
	UPDATE AccountsInfo SET MemberOrder=@MemberOrder, GameLogonTimes=GameLogonTimes+1,LastLogonDate=GETDATE(),
		LastLogonIP=@strClientIP,MachineSerial=@strMachineSerial WHERE UserID=@UserID

	-- ��¼��־
	DECLARE @DateID INT
	SET @DateID=CAST(CAST(GETDATE() AS FLOAT) AS INT)
	UPDATE SystemStreamInfo SET GameLogonSuccess=GameLogonSuccess+1 WHERE DateID=@DateID
	IF @@ROWCOUNT=0 INSERT SystemStreamInfo (DateID, GameLogonSuccess) VALUES (@DateID, 1)

	-- �������
	SELECT @UserID AS UserID, @GameID AS GameID, @Accounts AS Accounts, @UnderWrite AS UnderWrite, @FaceID AS FaceID, 
		@Gender AS Gender, @Experience AS Experience, @MemberOrder AS MemberOrder, @MemberOverDate AS MemberOverDate,
		@ErrorDescribe AS ErrorDescribe, @Loveliness AS Loveliness,@CustomFaceVer AS CustomFaceVer

END

RETURN 0

GO

----------------------------------------------------------------------------------------------------
