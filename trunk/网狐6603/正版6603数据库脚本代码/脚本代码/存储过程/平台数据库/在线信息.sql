
----------------------------------------------------------------------------------------------------

USE QPPlatformDB
GO

IF EXISTS (SELECT * FROM DBO.SYSOBJECTS WHERE ID = OBJECT_ID(N'[dbo].[GSP_GP_OnLineCountInfo]') and OBJECTPROPERTY(ID, N'IsProcedure') = 1)
DROP PROCEDURE [dbo].[GSP_GP_OnLineCountInfo]
GO

SET QUOTED_IDENTIFIER ON 
GO

SET ANSI_NULLS ON 
GO

----------------------------------------------------------------------------------------------------

--������Ϣ
CREATE  PROCEDURE dbo.GSP_GP_OnLineCountInfo
	@strMachineID NVARCHAR(32),					-- ������ʶ
	@strMachineServer NVARCHAR(32),				-- ��������
	@dwOnLineCountSum INT,						-- ��������
	@strOnLineCountKind NVARCHAR(2048)			-- ������Ϣ
WITH ENCRYPTION AS

--��������
SET NOCOUNT ON

--������Ϣ
INSERT OnLineStreamInfo (MachineID, MachineServer, OnLineCountSum, OnLineCountKind)
VALUES (@strMachineID, @strMachineServer, @dwOnLineCountSum, @strOnLineCountKind)

RETURN 0

GO

----------------------------------------------------------------------------------------------------
