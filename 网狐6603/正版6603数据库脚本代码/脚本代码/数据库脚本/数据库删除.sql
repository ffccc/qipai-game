
USE master
GO

IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'QPAccountsDB')
DROP DATABASE [QPAccountsDB]
GO

IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'QPExerciseDB')
DROP DATABASE [QPExerciseDB]
GO

IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'QPGameMatchDB')
DROP DATABASE [QPGameMatchDB]
GO

IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'QPGameScoreDB')
DROP DATABASE [QPGameScoreDB]
GO

IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'QPPlatformDB')
DROP DATABASE [QPPlatformDB]
GO

IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'QPRecordDB')
DROP DATABASE [QPRecordDB]
GO

IF EXISTS (SELECT name FROM master.dbo.sysdatabases WHERE name = N'QPTreasureDB')
DROP DATABASE [QPTreasureDB]
GO


