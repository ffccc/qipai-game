<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>无标题页</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>
    <script>
         function GetAllUsers()
        {
            document.myformList.action = "?action=alluser"
            document.myformList.submit();
        }
    </script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：游戏用户 - 魅力管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
     <script type="text/javascript">        
        function QueryInfo(opType)
        {
            var startDate = document.form1.StartDate.value;
            var endDate = document.form1.EndDate.value;
            if(opType==1)
            {
                if(!isDate(startDate)||!isDate(endDate))
                {
                    alert("日期格式不正确！");
                    return
                }
            }
            if(compareDate(startDate,endDate)==false)
		    {
		        
		        alert("开始时间不可大于结束时间！")
		        return false;
		    }
            document.form1.QueryType.value = opType
            document.form1.submit();
        }
    </script>
    <form name="form1" action="" method="post">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    日期查询：
                </td>
                <td>
                    <% If CxGame.GetInfo(0,"StartDate")<>"" Then %>
                    <input name="StartDate" type="text" class="text" style="width:70px;" value="<%=CxGame.GetInfo(0,"StartDate") %>" maxlength="10" /><img src="../../Images/btn_calendar.gif" onclick="calendar(StartDate)" style="cursor:pointer;vertical-align:middle"/>                    
                    <% Else %>
                    <input name="StartDate" type="text" class="text" style="width:70px;" value="<%=Year(Date())&"-"&Month(Date())&"-1" %>" maxlength="10" /><img src="../../Images/btn_calendar.gif" onclick="calendar(StartDate)" style="cursor:pointer;vertical-align:middle"/>  
                    <% End If %>
                    至
                    <% If CxGame.GetInfo(0,"EndDate")<>"" Then %>
                    <input name="EndDate" type="text" class="text" style="width:70px;" value="<%=CxGame.GetInfo(0,"EndDate") %>" maxlength="10" /><img src="../../Images/btn_calendar.gif" onclick="calendar(EndDate)" style="cursor:pointer;vertical-align:middle"/>                    
                    <% Else%>
                    <input name="EndDate" type="text" class="text" style="width:70px;" value="<%=Date() %>"  maxlength="10"/><img src="../../Images/btn_calendar.gif" onclick="calendar(EndDate)" style="cursor:pointer;vertical-align:middle"/>                    
                    <% End If %>
                    <input type="button" value="查询" class="btn wd1" onclick="QueryInfo('1')" />
                    <input type="hidden" name="QueryType" />                 
                </td>
            </tr>
        </table>        
    </form> 
    <form name="form2" action="?" method="post">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    用户查询：
                </td>
                <td>
                    <input name="search" type="text" class="text"  value="<%=CxGame.GetInfo(0,"search") %>"/>
                     <select name="swhat">
                        <option value="1" selected="selected">按用户帐号</option>   
                        <option value="2"<% If CxGame.GetInfo(1,"swhat")=2 Then %> selected="selected" <%End if %>>按游戏ID</option>                    
                    </select>
                    <input type="submit" value="查询" class="btn wd1" />    
                    <input type="button" value="所有用户" class="btn wd2" onclick="GetAllUsers()" /> 
                </td>
            </tr>
        </table>      
    </form>
    <% 
        Call ConnectGame(QPRecordDB)
        Call Main()
        Call CloseGame()
        
        Sub Main()
            Dim rs,nav,Page,i
            Dim lCount, queryCondition, OrderStr
            Dim startDate,endDate
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
            If CxGame.GetInfo(0,"QueryType")<>"" Then
                Select case CxGame.GetInfo(0,"QueryType")
                    case 1
                        startDate = CxGame.GetInfo(0,"StartDate")&" 00:00:00"
                        endDate = CxGame.GetInfo(0,"EndDate")&" 23:59:59"
                  
                End Select
                queryCondition = "(CollectDate between '"&startDate&"' And '"&endDate&"')"
            End If
            If CxGame.GetInfo(0,"search")<>"" Then
                queryCondition = "UserID = (Select UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where Accounts='"&Trim(Request("search"))&"')"   
            End If
            If Trim(Request("search"))<>"" Then
                Select case Request("swhat")
                    case 1
                        queryCondition = "UserID = (Select UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where Accounts='"&Trim(Request("search"))&"')"
                    case 2
                        queryCondition = "UserID = (Select Top 1 UserID From QPAccountsDBLink.QPAccountsDB.dbo.AccountsInfo Where GameID='"&CxGame.SqlCheckNum(Trim(Request("search")))&"')"
                   
                End Select
            End If   
            If Request("action")="alluser" Then
                queryCondition = " 1=1"
            End If               
            Dim fields
            fields = "RecordID,UserID,KindID,ServerID,CurInsureScore,CurPresent,ConvertPresent,ConvertRate,IsGamePlaza,ClientIP,CollectDate"
            
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
	            .Field=fields	'字段
	            
	            .Table="RecordConvertPresent"			'表名
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
    <form name="myformList" method="post" action="">
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box Tmg7">
                <tr align="center" class="bold">
                    <th class="listTitle1"><a class="l1"  href="" onclick="GetOrder('RecordID',this)">记录标识</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('UserID',this)">用户</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('KindID',this)">游戏</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ServerID',this)">房间</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CurInsureScore',this)">兑换前银行金币</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CurInsureScore',this)">兑换后银行金币</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CurPresent',this)">兑换前魅力点</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ConvertPresent',this)">兑换点数</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ConvertRate',this)">兑换比例</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('IsGamePlaza',this)">兑换场所</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ClientIP',this)">兑换地址</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('CollectDate',this)">兑换日期</a></th>
                </tr>
                <% 
                    Dim className
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='left'><br>没有任何信息!<br><br></td></tr>")
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
                    <td><%=rs(0,i) %></td>
                    <td><%=CxGame.GetUserNameByUserID(rs(1,i)) %></td>
                    <td><%=CxGame.GetKindNameByKindID(rs(2,i)) %></td>
                    <td><%=CxGame.GetRoomNameByRoomID(rs(3,i)) %></td>
                    <td><%=rs(4,i) %></td>
                    <td><%=CLng(rs(4,i))+ CLng(rs(6,i))*CLng(rs(7,i)) %></td>
                    <td><%=rs(5,i) %></td>
                    <td><%=rs(6,i) %></td>
                    <td><%=rs(7,i) %></td>
                    <% IF rs(8,i)=0 Then %>
                    <td>大厅</td>
                    <% Else %>
                    <td>网页</td>
                    <% End If %>
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
