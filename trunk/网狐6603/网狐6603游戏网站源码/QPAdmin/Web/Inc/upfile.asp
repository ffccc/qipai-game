
<!--#include FILE="upload_5xsoft.inc"-->

<%@ CODEPAGE=65001%>
<%Response.CodePage=65001%>
<%Response.Charset="UTF-8"%>
<%
    Dim fileurl,rs,sql
    Set upload = New upload_5xsoft 
    f= Server.MapPath("../../Upload/gamehelp")&"\\"
    Set file = upload.file("file1")  
    n = right(file.FileName,4)
    s=replace(now(),":","")
    s=replace(s,"-","")
    s=replace(s," ","")
    s=replace(s,"/","")
    fileurl=f&s&n
    Call ConnectGame("QPNativeWebDB")
    Set rs=Server.CreateObject("Adodb.RecordSet")
    Response.Write  CxGame.GetInfo(0,"KindID")
    Response.Write Request("KindName")
    Response.Write fileurl
    Response.Write Request("HelpIntro")
    Response.Write Request("content1")
    Response.Write Request("content2")
    sql="Insert into GameRulesInfo(KindID,KindName,ImgRuleUrl,HelpIntro,HelpRule,HelpGrade) values ('"&upload.form("KindID")&"','"&upload.form("KindName")&"','"&fileurl&"','"&upload.form("HelpIntro")&"','"&upload.form("content1")&"','"&upload.form("content2")&"')"
    rs.Open sql,GameConn,1,3
    Call CxGame.ShowInfo2("保存成功！","RoleList.asp",1200)
    file.SaveAs f&s&n  
    session("image")=s&n
%> 