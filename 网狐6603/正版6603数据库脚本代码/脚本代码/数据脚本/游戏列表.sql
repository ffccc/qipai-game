


----------------------------------------------------------------------------------------------------

USE QPPlatformDB
GO

----------------------------------------------------------------------------------------------------

-- 删除数据
DELETE GameTypeItem
DELETE GameKindItem
DELETE GameNodeItem
GO

----------------------------------------------------------------------------------------------------

-- 类型数据
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 1,0,100,'财富游戏',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 2,0,200,'百人游戏',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 3,0,300,'扑克游戏',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 4,0,400,'麻将游戏',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 5,0,500,'棋类游戏',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 6,0,600,'休闲游戏',0)
INSERT [GameTypeItem] ([TypeID],[JoinID],[SortID],[TypeName],[Nullity]) VALUES ( 7,0,700,'地方游戏',0)


----------------------------------------------------------------------------------------------------

INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 1,1,1,0,100,'按钮梭哈','ShowHandAN.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 2,2,1,0,100,'按钮五张','HKFiveCardAN.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 3,3,1,0,100,'德州扑克','DZShowHand.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 4,4,1,0,100,'二人牛牛','OxEx.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 5,5,1,0,100,'牛牛','Ox.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 6,6,1,0,100,'诈金花','ZaJinHua.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 7,7,1,0,100,'十三张','Thirteen.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 8,8,1,0,100,'二八杠','28Gang.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 9,9,1,0,100,'推筒子','TuiTongZi.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 10,10,1,0,100,'疯狂斗地主','LandCrazy.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 11,11,1,0,100,'扯旋','CheXuan.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 12,12,1,0,100,'红九','RedNine.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 13,13,1,0,100,'摇骰子','Liarsdice.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 14,14,1,0,100,'牌九','PaiJiu.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 15,15,1,0,100,'十点半','TenHalfPoint.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 16,16,1,0,100,'21 点','BlackJack.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 17,17,1,0,100,'填大坑','TianDaKeng.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 18,18,1,0,100,'四张','FourCard.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 19,19,1,0,100,'二人梭哈','ShowHandANEx.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 22,22,1,0,100,'二人数字五张','HKFiveCardSZEX.EXE',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 100,100,2,0,100,'百家乐','Baccarat.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 101,101,2,0,100,'百人单双','DanShuangBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 102,102,2,0,100,'百人梭哈','ShowHandBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 103,103,2,0,100,'十二生肖','ZodiacBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 104,104,2,0,100,'百人牛牛','OxBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 105,105,2,0,100,'两张','RedNineBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 106,106,2,0,100,'百人牌九','PaiJiuBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 107,107,2,0,100,'百人龙虎斗','LongHuDouBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 108,108,2,0,100,'碰碰车','BumperCarBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 109,109,2,0,100,'百人小九','NineXiaoBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 110,110,2,0,100,'百人憋十','BieShiBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 111,111,2,0,100,'百人骰子','DiceBattle.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 200,200,3,0,100,'斗地主','Land.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 201,201,3,0,100,'两副斗地主','Land2.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 202,202,3,0,100,'两人斗地主','LandEx.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 203,203,3,0,100,'两幅升级','UpGrade2.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 204,204,3,0,100,'三幅升级','UpGrade3.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 205,205,3,0,100,'四幅升级','UpGrade4.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 206,206,3,0,100,'广西升级','UpGradeGX.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 207,207,3,0,100,'广西标分','BiaoFen.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 208,208,3,0,100,'二人双扣','ShuangKouEx.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 209,209,3,0,100,'双扣','ShuangKou.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 210,210,3,0,100,'千变双扣','ShuangKouQB.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 211,211,3,0,100,'百变双扣','ShuangKouBB.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 212,212,3,0,100,'关牌','GuanPaiBG.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 213,213,3,0,100,'关牌','GuanPai.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 214,214,3,0,100,'锄大地','ChuDaDi.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 215,215,3,0,100,'拱猪','GongZhu.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 216,216,3,0,100,'挖坑','WaKeng.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 217,217,3,0,100,'510K','510K.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 218,218,3,0,100,'红五','HongWu.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 219,219,3,0,100,'红五两副','HongWu2.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 220,220,3,0,100,'红五三副','HongWu3.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 221,221,3,0,100,'三打一','SanDaYi.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 222,222,3,0,100,'宿迁掼蛋','GuanDanSQ.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 223,223,3,0,100,'淮安掼蛋','GuanDanHA.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 224,224,3,0,100,'湖南三打哈','SanDaHaHN.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 225,225,3,0,100,'红十','RedTen.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 300,300,4,0,100,'大众麻将','SparrowDZ.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 301,301,4,0,100,'国标麻将','SparrowGB.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 302,302,4,0,100,'血战麻将','SparrowXZ.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 303,303,4,0,100,'长沙麻将','SparrowCS.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 304,304,4,0,100,'浙江麻将','SparrowZJ.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 305,305,4,0,100,'长春麻将','SparrowCC.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 306,306,4,0,100,'二人麻将','SparrowER.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 307,307,4,0,100,'温州麻将','SparrowWZ.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 308,308,4,0,100,'温州二人麻将','SparrowWZEX.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 309,309,4,0,100,'宁海麻将','SparrowNH.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 400,400,5,0,100,'中国象棋','ChinaChessVD.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 401,401,5,0,100,'五子棋','GoBang.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 402,402,5,0,100,'四国军棋','FourEnsign.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 403,403,5,0,100,'围棋','Rock.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 404,404,5,0,100,'轮盘','Round.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 500,500,6,0,100,'连连看','LLShow.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 501,501,6,0,100,'飞行棋','Plane.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 600,600,7,0,100,'义乌牛公','OxYW.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 601,601,7,0,100,'湖南跑得快','RunFastHN.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 602,602,7,0,100,'跑胡子','PaoHuZi.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 603,603,7,0,100,'东北红十','RedTenDB.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 604,604,7,0,100,'辽源红十','RedTenLY.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 605,605,7,0,100,'丹东红十','RedTenDD.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 606,606,7,0,100,'奉化双扣','ShuangKouFH.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 607,607,7,0,100,'宁波斗地主','LandLB.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 608,608,7,0,100,'丹东五狼腿','WuLangTui.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 609,609,7,0,100,'辽源刨幺','PaoYao.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 610,610,7,0,100,'三七十','SanQiShi.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 611,611,7,0,100,'打纵','DaZong.exe',0)
INSERT [GameKindItem] ([KindID],[GameID],[TypeID],[JoinID],[SortID],[KindName],[ProcessName],[Nullity]) VALUES ( 612,612,7,0,100,'南京跑得快','RunFastNJ.exe',0)
