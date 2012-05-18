<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../Conn.asp"-->
<!--#include file="../../md5.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
     <script type="text/javascript" src="../../Js/Sort.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：后台系统 - 推广明细</td>
        </tr>
    </table>
     <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 ">
        <tr>
            <td height="28">
                <ul>
	                <li class="tab2" onclick="openWindowSelf('SpreadSet.asp')" >推广设置</li>
                    <li class="tab1" >推广明细</li>
	                <li class="tab2" onclick="openWindowSelf('FinanceInfo.asp')">财务明细</li>
                </ul>
            </td>
        </tr>
    </table>  
    <form id="form1" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    用户查询：
                </td>
                <td>
                    <input name="search" type="text" class="text" value="<%=CxGame.GetInfo(0,"search") %>" />
                    <select name="swhat">
                        <option value="1" selected="selected">按用户帐号</option>   
                        <option value="2"<% If CxGame.GetInfo(1,"swhat")=2 Then %> selected="selected" <%End if %>>按游戏ID</option>                     
                    </select>  
                    <input type="submit" value="查询" class="btn wd1" />
                </td>
            </tr>
        </table>
        
    </form>
    <!-- 头部菜单 End -->      
    <%  
        Call ConnectGame(QPAccountsDB)
        Call Main()
        Sub Main() 
            Dim nav,Page,i
            Dim lCount, queryCondition
            Dim rs,sql,id,className,rowNum,OrderStr
            If Request.QueryString("orders")<>""And Request.QueryString("OrderType")<>"" Then
                If Request.QueryString("OrderType")<>"-102" Then
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Replace(Request.QueryString("orders")," ","+")&" Desc "
                End If
            Else 
                OrderStr = "RegisterDate Asc"
            End If
            If Trim(Request("search"))<>"" Then
                Select case Request("swhat")
                    case 1
                        queryCondition = "SpreaderID<>0 and SpreaderID = (Select UserID From AccountsInfo Where Accounts='"&Trim(Request("search"))&"')"
                    case 2
                        queryCondition = "SpreaderID<>0 and SpreaderID = (Select Top 1 UserID From AccountsInfo Where GameID='"&CxGame.SqlCheckNum(Trim(Request("search")))&"')"  
                End Select
            Else
                queryCondition=" 1<>1 "    
            End If
            Dim fields        
            fields = "UserID,Accounts,gameid, RegisterDate"     
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = 20				'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="UserID"					'主键
	            .Field=fields	'字段
	            .Table="AccountsInfo "			'表名
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
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class=" box Tmg7">
                <tr align="center" class="bold">
                    <th class="listTitle1"><a class="l1"  href="" onclick="GetOrder('Accounts',this)">帐号</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('GameID',this)">游戏ID</a></th>
                    <th class="listTitle2">贡献金币</th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('RegisterDate',this)">推荐时间</a></th>
                    <th class="listTitle">&nbsp;</th>
                </tr>
                <% 
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else  
                    For i=0 To Ubound(rs,2)               
                    rowNum = rowNum+1
                    If rowNum Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If 
                %>
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">          
                    <td><%=rs(1,i) %></td>    
                    <td><%=rs(2,i) %></td>    
                    <td style="text-align:right"><%=FormatNumber(CxGame.GetRecordSpreadByUserID(rs(0,i)),2,-1) %></td>    
                    <td><%=rs(3,i) %></td>   
                    <td><a class="l" href="javascript:void(0)" onclick="openWindowOwn('SpreadInfo.asp?userid=<%=rs(0,i) %>',<%=rs(0,i) %>,850,600)">贡献明细</a></td>
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
