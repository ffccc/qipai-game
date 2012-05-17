<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../Conn.asp"-->
<!--#include file="../../GameConn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <title>无标题页</title>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
</head>
<%
    If CxGame.GetRoleValue("308",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：后台系统 - 泡点设置</td>
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
            Dim rs,sql,GrantRoom,GrantStartDate,GrantEndDate,GrantObjet,MaxGrant,DayMaxGrant
            Call ConnectGame(QPPlatformManagerDB)
            If Request("in_GrantRoom")<>"" Then
                GrantRoom=Request("in_GrantRoom")
            End If
            
            If Request("GrantStartDate")<>"" Then
                GrantStartDate=Request("GrantStartDate")
            Else
                GrantStartDate=Year(Date())&"-"&Month(Date())&"-"&Day(Date())
            End If
            
            If Request("GrantEndDate")<>"" Then
                GrantEndDate=Request("GrantEndDate")
            Else
                GrantEndDate=Year(Date())&"-"&Month(Date())&"-"&Day(Date())
            End If
            
            If Request("in_GrantObjet1")<>"" Then
                IF GrantObjet<>"" Then
                    GrantObjet=GrantObjet&","&Request("in_GrantObjet1")
                Else
                    GrantObjet=Request("in_GrantObjet1")
                End If
            End IF
            If Request("in_GrantObjet2")<>"" Then
                IF GrantObjet<>"" Then
                    GrantObjet=GrantObjet&","&Request("in_GrantObjet2")
                Else
                    GrantObjet=Request("in_GrantObjet2")
                End If
            End IF
           
            If Request("in_GrantObjet3")<>"" Then
                IF GrantObjet<>"" Then
                    GrantObjet=GrantObjet&","&Request("in_GrantObjet3")
                Else
                    GrantObjet=Request("in_GrantObjet3")
                End If
            End IF
            
            If Request("in_GrantObjet4")<>"" Then
                IF GrantObjet<>"" Then
                    GrantObjet=GrantObjet&","&Request("in_GrantObjet4")
                Else
                    GrantObjet=Request("in_GrantObjet4")
                End If
            End IF
            
            If Request("in_GrantObjet5")<>"" Then
                IF GrantObjet<>"" Then
                    GrantObjet=GrantObjet&","&Request("in_GrantObjet5")
                Else
                    GrantObjet=Request("in_GrantObjet5")
                End If
            End IF
            
            If Request("in_GrantObjet6")<>"" Then
                IF GrantObjet<>"" Then
                    GrantObjet=GrantObjet&","&Request("in_GrantObjet6")
                Else
                    GrantObjet=Request("in_GrantObjet6")
                End If
            End IF
            
            If Request("in_MaxGrant")<>"" Then
                MaxGrant=Request("in_MaxGrant")
            End If
            
            If Request("in_DayMaxGrant")<>"" Then
                DayMaxGrant=Request("in_DayMaxGrant")
            End If
                  
            PageSize = CxGame.GetInfo(0,"in_PageSize")
            Set rs=Server.CreateObject("Adodb.RecordSet")
            If Request("siteid")<>"" Then
                sql="select  * from GrantInfo where SiteID="&Request("siteid")
            Else
                sql="select  * from GrantInfo where 1<>1 "
            End If
            rs.open sql,GameConn,1,3
            If rs.eof then 
                rs.addnew      
            End If  
            rs("GrantRoom") = GrantRoom
            rs("GrantStartDate") = GrantStartDate
            rs("GrantEndDate") = GrantEndDate
            rs("GrantObjet") = GrantObjet
            rs("MaxGrant") = MaxGrant
            rs("DayMaxGrant") = DayMaxGrant
            rs.update
            Call CxGame.ShowInfo2("修改成功！","GrantList.asp",1200)
        End Sub 
        
        Sub Main()
            Dim rs,sql
            Dim GrantRoom,GrantStartDate,GrantEndDate,GrantObjet,MaxGrant,DayMaxGrant
            Set rs=server.createobject("adodb.recordset")    
            If Request("siteid")<>"" Then
                sql="select  * from GrantInfo where SiteID="&Request("siteid")
            Else
                sql="select  * from GrantInfo where 1<>1 "
            End If
            rs.open sql,GameConn,1,3
            If  rs.eof  then  
                GrantRoom=0
                GrantStartDate = Year(Date())&"-"&Month(Date())&"-"&Day(Date())
                GrantEndDate = Year(Date())&"-"&Month(Date())&"-"&Day(Date())
                GrantObjet = 0
                MaxGrant = 0
                DayMaxGrant=0
            Else
                GrantRoom=rs(1)
                GrantStartDate = rs(2)
                GrantEndDate = rs(3)
                GrantObjet = rs(4)
                MaxGrant = rs(5)
                DayMaxGrant = rs(6)
            End If   
    %>
    <script>
        function CheckFormInfo()
        {
            var maxGrant = document.myFormInfo2.in_MaxGrant.value;
            var dayMaxGrant = document.myFormInfo2.in_DayMaxGrant.value;
            if(IsPositiveInt(maxGrant)==false)
            {
                alert("封顶总数目非数值型字符！");
                return false;
            }
            if(IsPositiveInt(dayMaxGrant)==false)
            {
                alert("单日封顶数目非数值型字符！");
                return false;
            }
            if(compareDate(document.form1.GrantStartDate.value.trim(),document.form1.GrantEndDate.value.trim())==false)
		    {
		        
		        alert("开始时间不可大于结束时间！")
		        return
		    }
        }
    </script>
    <form name="myFormInfo2" method="post" action='?action=save' onsubmit="return CheckFormInfo()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GrantList.asp')" />
                    <input type="submit" value="保存" class="btn wd1" />  
                </td>
            </tr> 
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">赠送对象</div><input type=hidden name="siteid" value="<%=Request("siteid") %>" /></td>
            </tr>
            <tr>
                <td class="listTdLeft">对象：</td>
                <td>
                   <%
                        If InStr(GrantObjet,"0")>0 Then
                   %>
                   <input name="in_GrantObjet1" type="checkbox" checked="checked" value="0" />所有用户
                   <%
                        Else
                   %>
                    <input name="in_GrantObjet1" type="checkbox" value="0" />全体用户
                   <%     
                        End IF
                        If InStr(GrantObjet,"1")>0 Then
                   %>
                   <input name="in_GrantObjet2" type="checkbox" checked="checked" value="1"/>蓝钻会员
                   <%
                        Else 
                   %>
                   <input name="in_GrantObjet2" type="checkbox" value="1"/>蓝钻会员 
                   <%
                        End If
                        If InStr(GrantObjet,"2")>0 Then
                   %>
                   <input name="in_GrantObjet3" type="checkbox" checked="checked" value="2"/>红钻会员
                   <%
                        Else 
                   %>
                   <input name="in_GrantObjet3" type="checkbox" value="2"/>红钻会员 
                   <%
                        End If
                        If InStr(GrantObjet,"3")>0 Then
                   %>
                   <input name="in_GrantObjet4" type="checkbox" checked="checked" value="3"/>黄钻会员
                   <%
                        Else 
                   %>
                   <input name="in_GrantObjet4" type="checkbox" value="3"/>黄钻会员 
                   <%
                        End If
                        If InStr(GrantObjet,"4")>0 Then
                   %>
                   <input name="in_GrantObjet5" type="checkbox" checked="checked" value="4"/>紫钻会员
                   <%
                        Else 
                   %>
                   <input name="in_GrantObjet5" type="checkbox" value="4"/>紫钻会员 
                   <%
                        End If
                        If InStr(GrantObjet,"5")>0 Then
                   %>
                   <input name="in_GrantObjet6" type="checkbox" checked="checked" value="5"/>普通用户
                   <%
                        Else 
                   %>
                   <input name="in_GrantObjet6" type="checkbox" value="5"/>普通用户 
                   <%
                        End If
                   %>
                        
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">赠送房间</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">房间：</td>
                <td>
                    <select name="in_GrantRoom"  style="width:120px;">
                    <option value="-2"<% If GrantRoom= "-2" Then %> selected="selected" <% End If %> >积分房间</option>
                    <option value="-1"<% If GrantRoom= "-1" Then %> selected="selected" <% End If %> >金币房间</option>
                    <% 
                        Dim ArrayKind,i
                        ArrayKind = CxGame.GetRoomList1()
                        For i=0 To Ubound(ArrayKind)                                                         
                    %>
                    <option value="<%=ArrayKind(i,0) %>"<% If GrantRoom= ArrayKind(i,0) Then %> selected="selected" <% End If %> ><%=ArrayKind(i,1) %></option>
                    <%                      
                        Next  
                        Set ArrayKind = nothing
                    %>
                    </select>
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">赠送时间</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">时间：</td>
                <td>
                   <input name="GrantStartDate" type="text" class="text" style="width:100px;" maxlength="10" value="<%=GrantStartDate %>"  /><img src="../../Images/btn_calendar.gif" onclick="calendar(GrantStartDate)" style="cursor:pointer;vertical-align:middle"/> 
	                    至
	               <input name="GrantEndDate" type="text" class="text" style="width:100px;" maxlength="10" value="<%=GrantEndDate %>"  /><img src="../../Images/btn_calendar.gif" onclick="calendar(GrantEndDate)" style="cursor:pointer;vertical-align:middle"/>                  
                </td>
            </tr>
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">赠送封顶</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">总数目：</td>
                <td>
                    <input name="in_MaxGrant" type="text" class="text" style="width:300px;" value="<%=MaxGrant %>" />                
                </td>
            </tr>
             <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7"></div></td>
            </tr>
            <tr>
                <td class="listTdLeft">单日数目：</td>
                <td>
                    <input name="in_DayMaxGrant" type="text" class="text" style="width:300px;" value="<%=DayMaxGrant %>" />                
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10"> 
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('GrantList.asp')" />
                    <input type="submit" value="保存" class="btn wd1" />  
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