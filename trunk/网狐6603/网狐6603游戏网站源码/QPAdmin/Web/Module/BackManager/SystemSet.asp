<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../Conn.asp"-->
<!--#include file="../../GameConn.asp"-->
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <title>无标题页</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>   
</head>
<%
    If CxGame.GetRoleValue("501",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：后台系统 - 系统设置</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
    
        Call ConnectGame(QPPlatformManagerDB)
        Select case lcase(Request("action"))
            case "save"
            Call Save()
            case else
            Call Main()
        End Select
        
        Sub Save()
            Dim rs,sql,SiteName,PageSize,CopyRight,Revenue,GameScore
            Call ConnectGame(QPPlatformManagerDB)
            If CxGame.GetInfo(0,"in_SiteName")="" Then
                SiteName = "网狐棋牌平台管理后台"
            Else
                SiteName = CxGame.GetInfo(0,"in_SiteName")
            End If
            
            If CxGame.GetInfo(0,"in_CopyRight")="" Then
                CopyRight = "CopyRight 2010 深圳市网狐科技有限公司"
            Else
                CopyRight = CxGame.GetInfo(0,"in_CopyRight")
            End If
            
            If CxGame.GetInfo(0,"in_Revenue")="" Then
                Revenue = "0.00"
            Else
                Revenue = CxGame.GetInfo(0,"in_Revenue")
            End If
            
            If CxGame.GetInfo(0,"in_GameScore")="" Then
                GameScore = "0"
            Else
                GameScore = CxGame.GetInfo(0,"in_GameScore")
            End If
            
            PageSize = CxGame.GetInfo(0,"in_PageSize")
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql="select top 1 * from QPAdminSiteInfo "
            rs.open sql,GameConn,1,3
            If rs.eof then 
                rs.addnew      
            End If  
            rs("SiteName") = SiteName
            rs("CopyRight") = CopyRight
            rs("PageSize") = PageSize
            rs.update
            Call CxGame.ShowInfo2("修改成功！","SystemSet.asp",1200)
        End Sub 
        
        Sub Main()
            Dim rs,sql
            Dim SiteName,PageSize,CopyRight,Revenue,GameScore
            set rs=server.createobject("adodb.recordset")    
            sql="select top 1 *  from  QPAdminSiteInfo"     
            rs.open sql,GameConn,1,3
            if  rs.eof  then  
                SiteName="网狐棋牌平台管理后台"
                PageSize=30
                CopyRight = "CopyRight 2010 深圳市网狐科技有限公司"
            Else
                SiteName = rs(1)    
                PageSize = rs(2)
                CopyRight = rs(3)
            End If         
            
            
    %>
    <form name="myFormInfo2" method="post" action='?action=save' onsubmit="return CheckFormInfo2()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="submit" value="保存更改" class="btn wd2" />  
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">显示</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">在列表中：</td>
                <td>
                    每页显示
                    <select name="in_PageSize" style="width:100px">
                        <option value="30" <%If PageSize=30 Then %> selected="selected" <%End If %>>30(推荐)</option>
                        <option value="50" <%If PageSize=50 Then %> selected="selected" <%End If %>>50</option>
                        <option value="100" <%If PageSize=100 Then %> selected="selected" <%End If %>>100</option>
                    </select> 
                    条数据
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">管理后台标题</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">标题：</td>
                <td>
                    <input name="in_SiteName" type="text" class="text" style="width:300px;" value="<%=SiteName %>" />  
                    &nbsp;&nbsp;<span class="hui6">为空时为默认值</span>                       
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">版权</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">名称：</td>
                <td>
                    <input name="in_CopyRight" type="text" class="text" style="width:300px;" value="<%=CopyRight %>" /> 
                    &nbsp;&nbsp;<span class="hui6">为空时为默认值</span>                        
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="submit" value="保存更改" class="btn wd2" />  
                </td>
            </tr>
        </table>
        <% 
            rs.Close()
            Set rs=nothing
            End Sub
        %>
    </form>
</body>
</html>
