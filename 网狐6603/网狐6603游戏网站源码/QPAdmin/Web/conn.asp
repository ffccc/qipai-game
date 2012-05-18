<%

Rem 数据库地址
Function GetDbAccPath(db_name)
    Dim accPath
    accPath=Server.Mappath("/data/") & "\"&db_name  '后台
    Set fileSys=Createobject("scripting.filesystemobject")

    IF fileSys.FileExists(accPath)=false Then 
     accPath=Server.mappath("../data/") & "\"&db_name  '代理系统
    End IF
    GetDbAccPath=accPath
End Function

Rem IP 数据库连接
Dim IPDbAccConn,ipDbname,ipDbAcc,ipDbAccConnStr
ipDbname="QQWry.mdb"
ipDbAcc=GetDbAccPath(ipDbname)

Set IPDbAccConn = Server.CreateObject("ADODB.Connection")
ipDbAccConnStr="Provider=Microsoft.Jet.OLEDB.4.0;Data Source=" &ipDbAcc
IPDbAccConn.Open ipDbAccConnStr

If Err Then
	err.Clear
	Set IPDbAccConn = Nothing
	Response.Write "IP地址数据库连接出错，请检查连接字串。"
	Response.End
End If

Rem 查询IP地址
Function GetCityFromIP(lIPStr)
Dim IPRs,IPStrArray,IpLong,IPStr
	If lIpStr="" Or IsNull(lIpStr) Then
		GetCityFromIP = "没有IP记录"
		Exit Function
	End If
	IPStrArray = Split(lIPStr, ".")
	If Ubound(IPStrArray)=3 Then
		IpLong = CLng(IPStrArray(0))*256*256*256+cint(IPStrArray(1))*256*256+cint(IPStrArray(2))*256+cint(IPStrArray(3))-1
		Set IpRs = IPDbAccConn.execute("select * from ip where StartIP<="&IpLong&" and EndIP>="&IpLong)
		If Not IpRs.Eof Then
			IpStr = IpRs("pos")&"  "&IpRs("Detail")
		Else
			IpStr = "未知地址"
		End If
		IpRs.Close
		Set IpRs = Nothing		    
	Else
		IpStr = "未知地址"
	End If
	'IF IsObject(IPDbAccConn) Then
     '   IPDbAccConn.Close
     '   Set IPDbAccConn=nothing
    'End IF 
GetCityFromIP = IpStr
End Function




%>
