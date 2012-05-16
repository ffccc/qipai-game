
----------------------------------------------------------------------------------------------------

USE QPServerInfoDB
GO

----------------------------------------------------------------------------------------------------

-- ɾ������
DELETE GameTypeItem
DELETE GameKindItem
DELETE GameNodeItem
GO

----------------------------------------------------------------------------------------------------

-- ��������
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 1, '�Ƹ���Ϸ',100, 0)
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 2, '������Ϸ',200, 0)
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 3, '������Ϸ',300, 0)
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 4, '�齫��Ϸ',400, 0)
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 5, '������Ϸ',500, 0)
INSERT GameTypeItem (TypeID, TypeName, SortID, Nullity) VALUES ( 6, '��Ƶ��Ϸ',600, 0)

----------------------------------------------------------------------------------------------------

-- �Ƹ���Ϸ
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 4, 1, '�ټ���', 'Baccarat.exe', 67078, 100, 0, 'QPTreasureDB')
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 10, 1, '21 ��', 'BlackJack.exe', 67078, 100, 0, 'QPTreasureDB')
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 25, 1, 'ҡ����', 'Liarsdice.exe', 67078, 100, 0, 'QPTreasureDB')
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 26, 1, '����', 'RedNine100.exe', 67078, 100, 0, 'QPTreasureDB')
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 28, 1, '����ţţ', 'Ox100.exe', 67078, 100, 0, 'QPTreasureDB')
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 29, 1, '�ٱ����', 'ShowHand100.exe', 67078, 100, 0, 'QPTreasureDB')

-- ������Ϸ
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 111, 2, 'ǧ��˫��', 'ShuangKouQB.exe', 67078, 100, 0, 'QPShuangKouQBDB')

-- ������Ϸ
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 202, 3, '�Ĺ�����', 'FourEnsign.exe', 67078, 100, 0, 'QPFourEnsignDB')

-- �齫��Ϸ
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 305, 4, '�㽭�齫', 'SparrowZJ.exe', 67078, 100, 0, 'QPSparrowZJDB')

-- ������Ϸ
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 400, 5, '������', 'LLShow.exe', 67078, 100, 0, 'QPLLShowDB')

----��Ƶ��
INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 522, 6, '��񶷵���', 'LandCrazyVD.exe', 67078, 100, 0, 'QPLandCrazyDB')

INSERT GameKindItem (KindID, TypeID, KindName, ProcessName, MaxVersion, SortID, Nullity, DataBaseName) VALUES ( 2, 1, 'ͬ��˳����ʽ���ţ�', 'HKFiveCard.exe', 67078, 0, 0, 'QPTreasureDB')

----------------------------------------------------------------------------------------------------

GO