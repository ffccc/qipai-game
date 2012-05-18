<%
'***************************************
'权限检查
'***************************************
Dim AdminSet,AdminName,AdminPassword
AdminName=Replace(Trim(Session("AdminName")),"'","")
AdminPassword=Replace(Trim(Session("AdminPassword")),"'","")

IF AdminName="" OR adminpassword="" Then
	Response.Redirect "Login.asp"
End IF


%>