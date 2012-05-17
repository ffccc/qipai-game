<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>“<%=CxGame.GetUserNameByUserID(Request("id")) %>”银行记录</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script> 
    <script type="text/javascript" src="../../Js/common.js"></script> 
     <script type="text/javascript" src="../../Js/Sort.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：银行记录</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <form name="form1" action="" method="get">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 titleQueBg">
            <tr>
                <td class="listTdLeft" style="width:80px;">
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
                    <input type="button" value="今天" class="btn wd1" onclick="QueryInfo('2')"/>
                    <input type="button" value="昨天" class="btn wd1" onclick="QueryInfo('3')"/>
                    <input type="button" value="本星期" class="btn wd6" onclick="QueryInfo('4')"/>
                    <input type="button" value="上星期" class="btn wd6" onclick="QueryInfo('5')"/>
                    <input type="hidden" name="QueryType" />     
                    <input type="hidden" name="id" value="<%=request("id") %>" />  
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7 titleQueBg">
            <tr>
                <td class="listTdLeft" style="width:80px;">
                    快速查询：
                </td>
                <td>
                    <a href="?id=<%=request("id") %>&tradeType=1" class="l">存</a> | 
                    <a href="?id=<%=request("id") %>&tradeType=2" class="l">取</a> |
                    <a href="?id=<%=request("id") %>&tradeType=3" class="l">转</a> |
                    <a href="?id=<%=request("id") %>&tradeType=0" class="l">全部</a>
                </td>
            </tr>
        </table>
    </form>    
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
            document.form1.QueryType.value = opType
            document.form1.submit();
        }
    </script>
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
            queryCondition = "SourceUserID="&Request("id")&" And RecordID=0"
            '查询条件
            If CxGame.GetInfo(0,"QueryType")<>"" Then
                Select case CxGame.GetInfo(0,"QueryType")
                    case 1
                        startDate = CxGame.GetInfo(0,"StartDate")&" 00:00:00"
                        endDate = CxGame.GetInfo(0,"EndDate")&" 23:59:59"
                    case 2
                        startDate = Date()&" 00:00:00"
                        endDate = Date()&" 23:59:59"
                    case 3
                        startDate = DateAdd("d",-1,Date())&" 00:00:00"
                        endDate = DateAdd("d",-1,Date())&" 23:59:59"
                    case 4
                        startDate = DateAdd("d",1-Weekday(Date()),Date())&" 00:00:00"
                        endDate = DateAdd("d",6,startDate)&" 23:59:59"
                    case 5
                        startDate = DateAdd("d",1-7-Weekday(Date()),Date())&" 00:00:00"
                        endDate = DateAdd("d",6,startDate)&" 23:59:59"
                End Select
                queryCondition = "SourceUserID="&Request("id")&" And (CollectDate between '"&startDate&"' And '"&endDate&"')"
            End If
            
            Select Case Request("tradeType")    
                Case "1"            
                    queryCondition = "SourceUserID="&Request("id")&" And TradeType=1"
                Case "2"            
                    queryCondition = "SourceUserID="&Request("id")&" And TradeType=2"
                Case "3"            
                    queryCondition = "SourceUserID="&Request("id")&" And TradeType=3"
                Case "0"
                    queryCondition = "SourceUserID="&Request("id")
            End Select            
            
            Dim field
            field = "RecordID,CollectDate,SourceUserID,TargetUserID,TradeType,SourceBank,SourceGold,TargetBank,TargetGold,SwapScore,Revenue,ClientIP,KindID,ServerID"
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = 1					'每页记录条数
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
                <tr align="center" class="bold">
                    <th class="listTitle1"><a class="l1"  href="" onclick="GetOrder('CollectDate',this)">时间</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SourceUserID',this)">汇款人</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('TargetUserID',this)">收款人</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('TradeType',this)">交易类别</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SourceBank',this)">汇款人银行</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SourceGold',this)">汇款人金币</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('TargetBank',this)">收款人银行</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('TargetGold',this)">收款人金币</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SwapScore',this)">交易金币</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Revenue',this)">服务费</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ClientIP',this)">操作地址</a></th>
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
                    <td><%=CxGame.GetUserNameByUserID(rs(2,i)) %></td>  
                    <td><%=CxGame.GetUserNameByUserID(rs(3,i)) %></td>  
                    <td>
                        <% If rs(4,i) = 1 Then %>
                        存
                        <% Elseif rs(4,i) = 2 Then %>
                        取
                        <% Else %>
                        转
                        <% End If %>
                    </td>
                    <td><%=rs(5,i) %></td>
                    <td><%=rs(6,i) %></td>
                    <td><%=rs(7,i) %></td>
                    <td><%=rs(8,i) %></td>
                    <td><%=rs(9,i) %></td>
                    <td><%=rs(10,i) %></td>
                    <td><%=rs(11,i) %></td>
                    <td><%=CxGame.GetKindNameByKindID(rs(12,i)) %></td>
                    <td><%=CxGame.GetRoomNameByRoomID(rs(13,i)) %></td>
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
