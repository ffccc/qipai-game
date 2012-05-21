USE [master]
GO
/****** 对象:  Database [QPPlatformDB]    脚本日期: 12/08/2008 11:31:04 ******/
CREATE DATABASE [QPPlatformDB] ON  PRIMARY 
( NAME = N'QPPlatformDB', FILENAME = N'D:\数据库\QPPlatformDB.mdf' , SIZE = 3072KB , MAXSIZE = UNLIMITED, FILEGROWTH = 1024KB )
 LOG ON 
( NAME = N'QPPlatformDB_log', FILENAME = N'D:\数据库\QPPlatformDB_log.LDF' , SIZE = 1024KB , MAXSIZE = 2048GB , FILEGROWTH = 10%)
 COLLATE Chinese_PRC_CI_AS
GO
IF (1 = FULLTEXTSERVICEPROPERTY('IsFullTextInstalled'))
begin
EXEC [QPPlatformDB].[dbo].[sp_fulltext_database] @action = 'disable'
end
GO
ALTER DATABASE [QPPlatformDB] SET ANSI_NULL_DEFAULT OFF 
GO
ALTER DATABASE [QPPlatformDB] SET ANSI_NULLS OFF 
GO
ALTER DATABASE [QPPlatformDB] SET ANSI_PADDING OFF 
GO
ALTER DATABASE [QPPlatformDB] SET ANSI_WARNINGS OFF 
GO
ALTER DATABASE [QPPlatformDB] SET ARITHABORT OFF 
GO
ALTER DATABASE [QPPlatformDB] SET AUTO_CLOSE OFF 
GO
ALTER DATABASE [QPPlatformDB] SET AUTO_CREATE_STATISTICS ON 
GO
ALTER DATABASE [QPPlatformDB] SET AUTO_SHRINK OFF 
GO
ALTER DATABASE [QPPlatformDB] SET AUTO_UPDATE_STATISTICS ON 
GO
ALTER DATABASE [QPPlatformDB] SET CURSOR_CLOSE_ON_COMMIT OFF 
GO
ALTER DATABASE [QPPlatformDB] SET CURSOR_DEFAULT  GLOBAL 
GO
ALTER DATABASE [QPPlatformDB] SET CONCAT_NULL_YIELDS_NULL OFF 
GO
ALTER DATABASE [QPPlatformDB] SET NUMERIC_ROUNDABORT OFF 
GO
ALTER DATABASE [QPPlatformDB] SET QUOTED_IDENTIFIER OFF 
GO
ALTER DATABASE [QPPlatformDB] SET RECURSIVE_TRIGGERS OFF 
GO
ALTER DATABASE [QPPlatformDB] SET  ENABLE_BROKER 
GO
ALTER DATABASE [QPPlatformDB] SET AUTO_UPDATE_STATISTICS_ASYNC OFF 
GO
ALTER DATABASE [QPPlatformDB] SET DATE_CORRELATION_OPTIMIZATION OFF 
GO
ALTER DATABASE [QPPlatformDB] SET TRUSTWORTHY OFF 
GO
ALTER DATABASE [QPPlatformDB] SET ALLOW_SNAPSHOT_ISOLATION OFF 
GO
ALTER DATABASE [QPPlatformDB] SET PARAMETERIZATION SIMPLE 
GO
ALTER DATABASE [QPPlatformDB] SET  READ_WRITE 
GO
ALTER DATABASE [QPPlatformDB] SET RECOVERY FULL 
GO
ALTER DATABASE [QPPlatformDB] SET  MULTI_USER 
GO
ALTER DATABASE [QPPlatformDB] SET PAGE_VERIFY CHECKSUM  
GO
ALTER DATABASE [QPPlatformDB] SET DB_CHAINING OFF 

if ( ((@@microsoftversion / power(2, 24) = 8) and (@@microsoftversion & 0xffff >= 760)) or 
		(@@microsoftversion / power(2, 24) >= 9) )begin 
	exec dbo.sp_dboption @dbname =  N'QPPlatformDB', @optname = 'db chaining', @optvalue = 'OFF'
 end