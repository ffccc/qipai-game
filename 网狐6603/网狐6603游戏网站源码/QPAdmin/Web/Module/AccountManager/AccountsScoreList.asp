<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>
    <script>
         function GetAllUsers()
        {
            document.myformList.action = "?action=alluser"
            document.myformList.submit();
        }
    </script>
</head>
<%
    If CxGame.GetRoleValue("102",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：游戏用户 - 积分管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <form name="form1" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    普通查询：
                </td>
                <td>
                    <input name="search" type="text" class="text" value="<%=CxGame.GetInfo(0,"search") %>" />
                    <select name="swhat">
                        <option value="1" selected="selected">按用户帐号</option>   
                        <option value="2"<% If CxGame.GetInfo(1,"swhat")=2 Then %> selected="selected" <%End if %>>按游戏ID</option> 
                        <option value="3"<% If CxGame.GetInfo(1,"swhat")=3 Then %> selected="selected" <%End if %>>按用户积分大于等于</option>
                        <option value="4"<% If CxGame.GetInfo(1,"swhat")=4 Then %> selected="selected" <%End if %>>按用户积分小于等于</option>                       
                    </select>
                    <select name="kindID" style="width:100px">
                    <% 
                        Dim ArrayList,i
                        ArrayList = CxGame.GetScoreKindList
                        For i=0 To Ubound(ArrayList) 
                        If Not IsEmpty(ArrayList(i,0)) Then
                    %>
                    <option value="<%=ArrayList(i,0) %>"<%If CxGame.GetInfo(1,"kindID")= ArrayList(i,0) Then %> selected="selected" <%End If %>><%=ArrayList(i,1) %></option>           
                    <% 
                        End If
                        Next  
                        Set ArrayList = nothing
                    %>         
                    </select>                       
                    <input type="submit" value="查询" class="btn wd1" />
                    <input type="button" value="所有用户" class="btn wd2" onclick="GetAllUsers()" /> 
                </td>
            </tr>
        </table>
      
    </form>
    <% 
        Dim kindID,dname
        kindID = CxGame.GetInfo(1,"kindID")
        dname="QPGameScoreDB"
        
        IF kindID<=0 Then
            dname="QPGameScoreDB"
        Else
            dname = CxGame.GetKindDBNameByKindID(kindID)
            If IsNull(dname) Or dname="" Then
                dname="QPGameScoreDB"
            End If
        End If  
        
        Call ConnectGame(dname)
        Select case lcase(Request("action"))            
            case else
            Call Main()
        End Select
        Call CloseGame()  
        
        Sub Main()            
            Dim rs,nav,Page,i
            Dim lCount, queryCondition, OrderStr
             If Request.QueryString("orders")<>""And Request.QueryString("OrderType")<>"" Then
                If Request.QueryString("OrderType")<>"-102" Then
                    OrderStr=Request.QueryString("orders")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Request.QueryString("orders")&" Desc "
                End If
            Else 
                OrderStr = "LastLogonDate DESC"
            End If
            
            Dim field
            field = "UserID,Score,WinCount,LostCount,DrawCount,FleeCount,AllLogonTimes,PlayTimeCount,OnLineTimeCount,LastLogonDate,LastLogonIP"
            '查询条件
            
            If Trim(Request("search"))<>"" Then
                Select case Request("swhat")
                    case 1
                        queryCondition = "UserID = (Select UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where Accounts='"&Trim(Request("search"))&"')"
                    case 2
                        queryCondition = "UserID = (Select Top 1 UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where GameID='"&CxGame.SqlCheckNum(Trim(Request("search")))&"')"
                    case 3
                        queryCondition = "Score>="&CxGame.SqlCheckNum(Trim(Request("search")))
                    case 4
                        queryCondition = "Score<="&CxGame.SqlCheckNum(Trim(Request("search")))
                End Select
            End If   
            If Request("action")="alluser" Then
                queryCondition = " 1=1"
            End If       
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize					'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="UserID"					'主键
	            .Field=field	'字段
	            .Table="GameScoreInfo"			'表名
	            .Condition=queryCondition		'条件,不需要where
	            .OrderBy=OrderStr				'排序,不需要order by,需要asc或者desc
	            .RecordCount = 0				'总记录数，可以外部赋值，0不保存（适合搜索），-1存为session，-2存为cookies，-3存为applacation

	            .NumericJump = 5                '数字上下页个数，可选参数，默认为3，负数为跳转个数，0为显示所有
	            .Template = "总记录数：{$RecordCount} 总页数：{$PageCount} 每页记录数：{$PageSize} 当前页数：{$PageIndex} {$FirstPage} {$PreviousPage} {$NumericPage} {$NextPage} {$LastPage} {$InputPage} {$SelectPage}" '整体模板，可选参数，有默认值
	            .FirstPage = "首页"             '可选参数，有默认值
	            .PreviousPage = "上一页"        '可选参数，有默认值
	            .NextPage = "下一页"            '可选参数，有默认值
	            .LastPage = "尾页"              '可选参数，有默认值
	            .NumericPage = " {$PageNum} "   '数字分页部分模板，可选参数，有默认值
            End With
            
            rs = Page.ResultSet()       '记录集
            lCount = Page.RowCount()    '可选，输出总记录数
            nav = Page.Nav()            '分页样式
            
            Page = Null
            Set Page = Nothing
            '==============================================================================================================
    %>
    <form name="myformList" method="post" action=''>
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box Tmg7">
                <tr class="bold">
                    <th class="listTitle1"><a class="l1" href="" onclick="GetOrder('UserID',this)">用户帐号(游戏ＩＤ)</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Score',this)">积分</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('WinCount',this)">赢局</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('LostCount',this)">输局</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('DrawCount',this)">和局</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('FleeCount',this)">逃局</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('AllLogonTimes',this)">登录次数</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('PlayTimeCount',this)">游戏时长(时)</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('OnLineTimeCount',this)">在线时长(时)</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('LastLogonDate',this)">最后登录时间</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('LastLogonIP',this)">最后登录地址</a></th>
                </tr>
                <% 
                    Dim className
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else
                    
                    For i=0 To Ubound(rs,2)
                    If i Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If 
                %>
                <tr align="center" class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                    <td>
                        <a class="l" href="javascript:void(0)" onclick="javascript:openWindowOwn('AccountsScoreInfo.asp?id=<%=rs(0,i) %>&KindID=<%=CxGame.GetInfo(1,"kindID") %>',<%=rs(0,i) %>,850,600)"><%=CxGame.GetUserNameByUserID(rs(0,i)) %>(<%=CxGame.GetUserInfo(rs(0,i))(0,0) %>)</a>
                    </td>
                    <td><%=FormatNumber(rs(1,i),2,-1) %></td>           
                    <td><%=rs(2,i) %></td>       
                    <td><%=rs(3,i) %></td>       
                    <td><%=rs(4,i) %></td>       
                    <td><%=rs(5,i) %></td>       
                    <td><%=rs(6,i) %></td>            
                    <td><%=FormatNumber((rs(7,i) / 3600),,-1,,0) %></td>       
                    <td><%=FormatNumber((rs(8,i) / 3600),,-1,,0) %></td>       
                    <td><%=rs(9,i) %></td>   
                    <td><%=rs(10,i) %></td>        
                </tr>
                <% 
                    Next
                    End If
                %>
            </table>
        </div>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="page" align="right"><%Response.Write nav%></td>
            </tr>
        </table>  
    </form>
    <% 
        End Sub
    %>
</body>
</html>
