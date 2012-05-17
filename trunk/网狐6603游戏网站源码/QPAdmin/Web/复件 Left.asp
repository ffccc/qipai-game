<!--#include file="CommonFun.asp"-->
<!--#include file="conn.asp"-->
<!--#include file="GameConn.asp"-->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <link href="Css/layout.css" rel="stylesheet" type="text/css" />
    <style type="text/css">
        body {background-image: url(images/sideBg.gif); background-repeat:repeat-y;}
    </style>
    <script type="text/javascript">
        function showHide(obj) {
            var oStyle = obj.style;
            var imgId = obj.id.replace("M","S");
            oStyle.display == "none" ? oStyle.display = "block" : oStyle.display = "none";
            oStyle.display == "none" ? document.getElementById(imgId).src = "images/arrBig1.gif" : document.getElementById(imgId).src = "images/arrBig.gif";
        }
        
        function GetUrl(obj,url)
        {
            window.open(url,"frm_main_content");
            var trList = document.getElementsByTagName("tr");
            for(var i=0;i<trList.length;i++)
            {
                if(trList[i].className=="linkBg")
                {
                    trList[i].className = "s";
                }
            }            
            obj.className="linkBg";
        }
    </script>
</head>
<body>
<%
    Dim rs,sql,n
    Call ConnectGame(QPPlatformManagerDB)
    set rs=server.createobject("adodb.recordset")
    sql="select distinct  bm.moduleid,bm.title,bm.orderno from Base_RolePermission br  ,Base_Users bu,Base_Module bm,Base_ModulePermission bmp  where br.roleid=bu.roleid   and bm.moduleid=bmp.parentid and br.moduleid=bmp.moduleid and bu.username='"&session("AdminName")&"' order by bm.orderno"
    rs.open sql,GameConn,1,3

    If IsNull(rs) Then
       Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
    Else 
    n=0 
    do until rs.eof           
%>
<table width="95%" border="0" cellpadding="0" cellspacing="0">
<tbody>
    <tr>
		<td class="hui f14 bold pd32 hand" height="30" onclick="JavaScript:showHide(M_<%=n+1%>);">
		    <img src="images/arrBig.gif" width="11" height="11" id="S_<%=n+1%>" /> <%=rs("title") %>
		</td>
	</tr>
	<tr>
	    <td id="M_<%=n+1%>">
	        <table width="93%" border="0" align="right" cellpadding="0" cellspacing="0" class="hui">
	        <tbody>
                <%
                    Dim rs1,sql1
                    Call ConnectGame(QPPlatformManagerDB)
                    set rs1=server.createobject("adodb.recordset")
                    sql1="select distinct bm.moduleid,bm.title,bm.orderno,bm.link from Base_RolePermission br  ,Base_Users bu,Base_Module bm,Base_ModulePermission bmp  where br.roleid=bu.roleid   and bm.moduleid=bmp.moduleid and br.moduleid=bm.moduleid and bm.Nullity='false' and bm.IsMenu='true' and bu.username='"&session("AdminName")&"' and bm.parentid="&rs("moduleid")&" order by bm.orderno"
                    rs1.open sql1,GameConn,1,3
                    do until rs1.eof  
                %>
                <tr class="s" onclick="GetUrl(this,'<%=rs1("link") %>')">
				    <td width="313" height="25" align="right"></td>
				    <td width="67" height="25"><img src="images/arrSmall.gif" width="8" height="7" /></td>
				    <td width="725" height="25" align="left"><%=rs1("title") %></td>
			    </tr>
                <% 
                    rs1.movenext
                    loop
                %>
	         </tbody>
	        </table>
	    </td>
	</tr>
</tbody>
</table>
 <% 
     n=n+1
     rs.movenext
     loop
     End If
 %>
</body>
</html>
