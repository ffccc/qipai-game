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
</head>
<%
    If CxGame.GetRoleValue("103",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp" 
    End If  
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：游戏用户 - 银行记录</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <form id="form1" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    普通查询：
                </td>
                <td>
                    <input name="search" type="text" class="text" value="<%=CxGame.GetInfo(0,"search") %>" />
                    <select name="swhat">
                        <option value="1" selected="selected">按汇款人用户名</option>   
                        <option value="2"<% If CxGame.GetInfo(1,"swhat")=2 Then %> selected="selected" <%End If %>>按收款人用户名</option> 
                        <option value="3"<% If CxGame.GetInfo(1,"swhat")=3 Then %> selected="selected" <%End If %>>按汇款人游戏ID</option>
                        <option value="4"<% If CxGame.GetInfo(1,"swhat")=4 Then %> selected="selected" <%End If %>>按收款人游戏ID</option> 
                        <option value="5"<% If CxGame.GetInfo(1,"swhat")=5 Then %> selected="selected" <%End If %>>按游戏ID</option> 
                        <option value="6"<% If CxGame.GetInfo(1,"swhat")=6 Then %> selected="selected" <%End If %>>按房间ID</option> 
                    </select>
                    <select name="tradeType">
                        <option value="0" selected="selected">交易类别</option>   
                        <option value="1"<% If CxGame.GetInfo(1,"tradeType")=1 Then %> selected="selected" <%End If %>>存</option> 
                        <option value="2"<% If CxGame.GetInfo(1,"tradeType")=2 Then %> selected="selected" <%End If %>>取</option>
                        <option value="3"<% If CxGame.GetInfo(1,"tradeType")=3 Then %> selected="selected" <%End If %>>转</option>                         
                    </select>
                    <input type="submit" value="查询" class="btn wd1" />
                </td>
            </tr>
        </table>
       
    </form>
    <% 
        Call ConnectGame(QPRecordDB)
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
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Desc "
                End If
            Else 
                OrderStr = "CollectDate DESC"
            End If
            Dim field
            field = "RecordID,CollectDate,SourceUserID,TargetUserID,TradeType,SourceBank,SourceGold,TargetBank,TargetGold,SwapScore,Revenue,KindID,ServerID,IsGamePlaza"
            '查询条件
            
            If CxGame.GetInfo(0,"search")<>"" Then
                Select case Request("swhat")
                    case 1
                        queryCondition = " SourceUserID = (Select UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where Accounts='"&CxGame.GetInfo(0,"search")&"')"
                    case 2
                        queryCondition = " TargetUserID = (Select UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where Accounts='"&CxGame.GetInfo(0,"search")&"')"
                    case 3
                        queryCondition = " SourceUserID = (Select Top 1 UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where GameID='"&CxGame.GetInfo(1,"search")&"')"
                    case 4
                        queryCondition = " TargetUserID = (Select Top 1 UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where GameID='"&CxGame.GetInfo(1,"search")&"')"
                    case 5
                        queryCondition = " KindID = "&CxGame.GetInfo(0,"search")
                    case 6
                        queryCondition = " ServerID = "&CxGame.GetInfo(0,"search")
                End Select
            End If
            
            If queryCondition<>"" Then
                queryCondition = queryCondition & " AND "
            End If
            
            Select Case Request("tradeType")    
                Case "1"            
                    queryCondition = queryCondition & " TradeType=1 "
                Case "2"            
                    queryCondition = queryCondition & " TradeType=2 "
                Case "3"            
                    queryCondition = queryCondition & " TradeType=3 "
                Case "0"
                    queryCondition = queryCondition & " 1=1 "
            End Select
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize					'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="RecordID"					'主键
	            .Field=field	'字段
	            .Table="RecordInsure"			'表名
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
                    <th class="listTitle1"><a class="l1"  href="" onclick="GetOrder('CollectDate',this)">时间</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SourceUserID',this)">汇款人</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('TargetUserID',this)">收款人</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('TradeType',this)">交易类别</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SourceGold+SourceBank',this)">汇款人(银行/现金)</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('TargetGold+TargetBank',this)">收款人(银行/现金)</a></th>                  
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SwapScore',this)">交易金币</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('IsGamePlaza',this)">交易场所</a></th>
                    <th class="listTitle2">汇款人交易后(银行/现金)</th>
                    <th class="listTitle2">收款人交易后(银行/现金)</th> 
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Revenue',this)">服务费</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('KindID',this)">游戏</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('ServerID',this)">房间</a></th>
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
                    <td><%=rs(1,i) %></td>  
                    <td>
                        <a class="l" href="javascript:void(0)" onclick="javascript:openWindowOwn('AccountsInfo.asp?id=<%=rs(2,i) %>',<%=rs(2,i) %>,850,600)"><%=CxGame.GetUserNameByUserID(rs(2,i)) %></a>
                    </td>
                    <td>
                        <a class="l" href="javascript:void(0)" onclick="javascript:openWindowOwn('AccountsInfo.asp?id=<%=rs(3,i) %>',<%=rs(3,i) %>,850,600)"><%=CxGame.GetUserNameByUserID(rs(3,i)) %></a>
                    </td>
                    <td>
                        <% If rs(4,i) = 1 Then %>
                        存
                        <% Elseif rs(4,i) = 2 Then %>
                        取
                        <% Else %>
                        转
                        <% End If %>
                    </td>
                    <td><%=rs(5,i)&"/"&rs(6,i) %></td>
                    <td><%=rs(7,i)&"/"&rs(8,i) %></td>
                    <td><%=rs(9,i) %></td>
                    <% IF rs(13,i)=0 Then %>
                    <td>大厅</td>
                    <% Else %>
                    <td>网页</td>
                    <% End If %>
                    <td>
                        <% 
                            If rs(4,i)=1 Then
                                Response.Write CDbl(rs(5,i))+CDbl(rs(9,i))&"/"&CDbl(rs(6,i))-CDbl(rs(9,i))
                            Elseif rs(4,i) = 2 Then
                                Response.Write CDbl(rs(5,i))-CDbl(rs(9,i))&"/"&CDbl(rs(6,i))+CDbl(rs(9,i))
                            Else
                                Response.Write CDbl(rs(5,i))-CDbl(rs(9,i))-CDbl(rs(10,i))&"/"&CDbl(rs(6,i))
                            End If
                        %>
                    </td>
                    <td>
                        <% 
                            If rs(4,i)=3 Then
                                Response.Write CDbl(rs(7,i))+CDbl(rs(9,i))&"/"&CDbl(rs(8,i))
                            Else
                                response.Write "0/0"
                            End If
                        %>
                    </td>
                    <td><%=rs(10,i) %></td>
                    <td><%=CxGame.GetKindNameByKindID(rs(11,i)) %></td>
                    <td><%=CxGame.GetRoomNameByRoomID(rs(12,i)) %></td>
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
