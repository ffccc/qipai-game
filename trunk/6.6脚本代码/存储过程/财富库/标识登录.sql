
----------------------------------------------------------------------------------------------------
-- ���ӵ���ͳ�ƣ����ѽ�ң��û�����
----------------------------------------------------------------------------------------------------

----------------------------------------------------------------------------------------------------

USE QPTreasureDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GR_EfficacyUserID]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GR_EfficacyUserID]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

-- I D ��¼
CREATE PROC GSP_GR_EfficacyUserID
	@dwUserID INT,								-- �û� I D
	@strPassword NCHAR(32),						-- �û�����
	@strClientIP NVARCHAR(15),					-- ���ӵ�ַ
	@strMachineSerial NCHAR(32),				-- ������ʶ
	@wKindID INT,								-- ��Ϸ I D
	@wServerID INT								-- ���� I D
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
DECLARE @GroupID INT
DECLARE @UserRight INT
DECLARE @Gender TINYINT
DECLARE @Loveliness INT
DECLARE @MasterRight INT
DECLARE @MasterOrder INT
DECLARE @MemberOrder INT
DECLARE @MemberOverDate DATETIME
DECLARE @GroupName NVARCHAR(31)
DECLARE @CustomFaceVer TINYINT
DECLARE @strBankPassword NCHAR(32)

-- ���ֱ���
DECLARE @GameGold BIGINT
DECLARE @InsureScore BIGINT
DECLARE @Score BIGINT
DECLARE @WinCount INT
DECLARE @LostCount INT
DECLARE @DrawCount INT
DECLARE @FleeCount INT
DECLARE @Experience INT

-- ������Ϣ
DECLARE @PropCount INT

-- �������
DECLARE @ScoreLimit BIGINT

-- ��������
DECLARE @EnjoinLogon BIGINT
DECLARE @ErrorDescribe AS NVARCHAR(128)

-- ִ���߼�
BEGIN
	--
	SET @ScoreLimit=2147483647

	-- Ч���ַ
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineAddress(NOLOCK) WHERE AddrString=@strClientIP AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SELECT [ErrorDescribe]=N'��Ǹ��֪ͨ����ϵͳ��ֹ�������ڵ� IP ��ַ����Ϸ��¼Ȩ�ޣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 4
	END
	
	-- Ч�����
	SELECT @EnjoinLogon=EnjoinLogon FROM ConfineMachine(NOLOCK) WHERE MachineSerial=@strMachineSerial AND GETDATE()<EnjoinOverDate
	IF @EnjoinLogon IS NOT NULL AND @EnjoinLogon<>0
	BEGIN
		SELECT [ErrorDescribe]=N'��Ǹ��֪ͨ����ϵͳ��ֹ�����Ļ�������Ϸ��¼Ȩ�ޣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 7
	END
 
	-- ��ѯ�û�
	DECLARE @Nullity BIT
	DECLARE @StunDown BIT
	DECLARE @LogonPass AS NCHAR(32)
	DECLARE	@MachineSerial NCHAR(32)
	DECLARE @MoorMachine AS TINYINT
	SELECT @UserID=UserID, @GameID=GameID, @Accounts=Accounts, @UnderWrite=UnderWrite, @LogonPass=LogonPass, @FaceID=FaceID, 
		@Gender=Gender, @Nullity=Nullity, @StunDown=StunDown, @Experience=Experience, @UserRight=UserRight, @MasterRight=MasterRight,
		@MasterOrder=MasterOrder, @MemberOrder=MemberOrder,  @MemberOverDate=MemberOverDate, @MoorMachine=MoorMachine, @MachineSerial=MachineSerial, 
		@Loveliness=Loveliness,@CustomFaceVer=CustomFaceVer,@strBankPassword=InsurePass
	FROM QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo WHERE UserID=@dwUserID

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
		SELECT [ErrorDescribe]=N'�����ʺ�ʹ���˰�ȫ�رչ��ܣ����뵽���¿�ͨ����ܼ���ʹ�ã�'
		RETURN 2
	END	
	
	-- �̶�����
	IF @MoorMachine=1
	BEGIN
		IF @MachineSerial<>@strMachineSerial
		BEGIN
			SELECT [ErrorDescribe]=N'�����ʺ�ʹ�ù̶�������¼���ܣ�������ʹ�õĻ���������ָ���Ļ�����'
			RETURN 1
		END
	END

	-- �����ж�
	IF @LogonPass<>@strPassword AND @strClientIP<>N'0.0.0.0' AND @strPassword<>N''
	BEGIN
		SELECT [ErrorDescribe]=N'�����ʺŲ����ڻ������������������֤���ٴγ��ԣ�'
		RETURN 3
	END

	-- �̶�����
	IF @MoorMachine=2
	BEGIN
		SET @MoorMachine=1
		SELECT [ErrorDescribe]=N'�����ʺųɹ�ʹ���˹̶�������¼���ܣ�'
		UPDATE AccountsInfo SET MoorMachine=@MoorMachine, MachineSerial=@strMachineSerial WHERE UserID=@UserID
	END

	-- �������
	DELETE FROM GameScoreLocker WHERE UserID=@dwUserID AND KindID=@wKindID  AND ServerID=@wServerID 

	-- ��������
	DECLARE @LockKindID INT
	SELECT @LockKindID=KindID FROM GameScoreLocker WHERE UserID=@dwUserID
	IF @LockKindID IS NOT NULL
	BEGIN
		DECLARE @KindName NVARCHAR(31)
		SELECT @KindName=KindName FROM QPServerInfoDBLink.QPServerInfoDB.dbo.GameKindItem WHERE KindID=@LockKindID
		IF @KindName IS NULL SET @KindName=N'��ֵ'
		SELECT [ErrorDescribe]=N'���Ѿ���'+@KindName+N'��Ϸ�����ˣ�����ͬʱ�ڽ������Ϸ�����ˣ�'
		RETURN 4
	END
	INSERT GameScoreLocker (UserID,KindID,ServerID) VALUES (@dwUserID,@wKindID,@wServerID)

	-- ��Ϸ��Ϣ
	DECLARE @GameUserRight INT
	DECLARE @GameMasterRight INT
	DECLARE @GameMasterOrder INT
	SELECT @Score=Score,@InsureScore=InsureScore, @WinCount=WinCount, @LostCount=LostCount, @DrawCount=DrawCount,
		@DrawCount=DrawCount, @FleeCount=FleeCount, @GameUserRight=UserRight, @GameMasterRight=MasterRight, @GameMasterOrder=MasterOrder
	FROM GameScoreInfo WHERE UserID=@dwUserID

	-- ��Ϣ�ж�
	IF @Score IS NULL
	BEGIN
		-- ��������
		INSERT INTO GameScoreInfo (UserID, LastLogonIP, RegisterIP)	VALUES (@dwUserID,@strClientIP,@strClientIP)

		-- ��Ϸ��Ϣ
		SELECT @Score=Score, @InsureScore=InsureScore, @WinCount=WinCount, @LostCount=LostCount,@DrawCount=DrawCount, @DrawCount=DrawCount, 
			@FleeCount=FleeCount, @GameUserRight=UserRight, @GameMasterOrder=MasterOrder, @MasterRight=MasterRight
		FROM GameScoreInfo WHERE UserID=@dwUserID
	END

	-- �������
	IF @Score>=@ScoreLimit OR @InsureScore>=@ScoreLimit
	BEGIN
		-- �������
		DELETE FROM GameScoreLocker WHERE UserID=@dwUserID AND KindID=@wKindID  AND ServerID=@wServerID 

		SELECT [ErrorDescribe]=N'���Ľ����Ŀ�Ѿ�����ϵͳ���ޣ�����ϵ�ͻ����������˽���ϸ�����'
		RETURN 3
	END

	-- ������Ϣ
	UPDATE GameScoreInfo SET AllLogonTimes=AllLogonTimes+1, LastLogonDate=GETDATE(), LastLogonIP=@strClientIP WHERE UserID=@dwUserID

	-- ���
	SET @GameGold=@Score

	IF @InsureScore IS NULL SET @InsureScore=0

	-- ������Ϣ
	SET @GroupID=0
	SET @GroupName=''

	-- Ȩ�ޱ�־
	SET @UserRight=@UserRight|@GameUserRight
	SET @MasterRight=@MasterRight|@GameMasterRight

	-- Ȩ�޵ȼ�
	IF @MasterOrder<>0 OR @GameMasterOrder<>0
	BEGIN
		IF @GameMasterOrder>@MasterOrder SET @MasterOrder=@GameMasterOrder
	END
	ELSE SET @MasterRight=0

	-- �����¼
	INSERT RecordUserEnter (UserID, Score, KindID, ServerID, ClientIP) VALUES (@UserID, @Score, @wKindID, @wServerID, @strClientIP)

	-- ����ͳ��
	SELECT @PropCount=COUNT(UserID) FROM GameUserShelf WHERE UserID=@UserID AND UsedFlag=0 AND (KindID=@wKindID OR KindID=0)
	
	-- ��Ա�ȼ�
	IF GETDATE()>=@MemberOverDate 
	BEGIN 
		SET @MemberOrder=0

		-- ɾ�����ڻ�Ա���
		DELETE FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo WHERE UserID=@UserID
	END
	ELSE 
	BEGIN
		DECLARE @MemberCurDate DATETIME

		-- ��ǰ��Աʱ��
		SELECT @MemberCurDate=MIN(MemberOverDate) 
		FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo WHERE UserID=@UserID

		-- ɾ�����ڻ�Ա
		IF GETDATE()>=@MemberCurDate
		BEGIN
			-- ɾ����Ա���޹��ڵ����л�Ա���
			DELETE FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo WHERE UserID=@UserID AND MemberOverDate<=GETDATE()

			-- �л�����һ�����Ա���
			SELECT @MemberOrder=MAX(MemberOrder) 
			FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo WHERE UserID=@UserID
			IF @MemberOrder IS NOT NULL
			BEGIN
				UPDATE QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo 
				SET MemberOrder=@MemberOrder
				WHERE UserID=@UserID
			END
			ELSE SET @MemberOrder=0
		END
	END

	-- ������Ϣ
	UPDATE QPGameUserDBLink.QPGameUserDB.dbo.AccountsInfo SET MemberOrder=@MemberOrder WHERE UserID=@UserID

	-- ��Ա����
	SELECT @PropCount=@PropCount+COUNT(UserID) FROM QPGameUserDBLink.QPGameUserDB.dbo.MemberInfo WHERE UserID=@UserID

	-- ��¼ͳ��
	DECLARE @DateID INT
	SET @DateID=CAST(CAST(GETDATE() AS FLOAT) AS INT)
	UPDATE SystemStreamInfo SET LogonCount=LogonCount+1 WHERE DateID=@DateID AND KindID=@wKindID AND ServerID=@wServerID
	IF @@ROWCOUNT=0 INSERT SystemStreamInfo (DateID, KindID, ServerID, LogonCount) VALUES (@DateID, @wKindID, @wServerID, 1)

	-- �������
	SELECT @UserID AS UserID, @GameID AS GameID, @GroupID AS GroupID, @Accounts AS Accounts, @UnderWrite AS UnderWrite, @FaceID AS FaceID, 
		@Gender AS Gender, @GroupName AS GroupName, @MasterOrder AS MemberOrder, @UserRight AS UserRight, @MasterRight AS MasterRight, 
		@MasterOrder AS MasterOrder, @MemberOrder AS MemberOrder, @WinCount AS WinCount, @LostCount AS LostCount, @Loveliness AS Loveliness,
		@PropCount AS PropCount, @GameGold AS GameGold, @InsureScore AS InsureScore, @Loveliness AS Loveliness,	@CustomFaceVer AS CustomFaceVer,
		@DrawCount AS DrawCount, @FleeCount AS FleeCount, @Score AS Score, @Experience AS Experience, @ErrorDescribe AS ErrorDescribe,
		@strBankPassword AS BankPassword
END

RETURN 0

GO

----------------------------------------------------------------------------------------------------