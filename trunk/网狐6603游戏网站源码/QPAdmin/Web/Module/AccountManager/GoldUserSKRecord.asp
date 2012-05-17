<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../conn.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title>“<%=CxGame.GetUserNameByUserID(Request("id")) %>”充值记录</title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
    <script type="text/javascript" src="../../Js/comm.js"></script>
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
    <script type="text/javascript" src="../../Js/Sort.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：充值记录</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->   
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab1">实卡充值</li>
	                <li class="tab2" onclick="openWindowSelf('GoldUserOLRecord.asp?id=<%=Request("id") %>')">网上充值</li>
                </ul>
            </td>
        </tr>
    </table>    
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
    <form name="form1" action="" method="get">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="titleQueBg">
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
                    <select name="cardType" style="width:100px;"> 
                    <option value="0">全部卡片</option>
                    <% 
                        Dim ArrayList,i
                        ArrayList = CxGame.GetCardTypeList     
                        For i=0 To Ubound(ArrayList)  
                    %>
                    <option value="<%=ArrayList(i,0) %>" <%If CxGame.GetInfo(1,"cardType") = ArrayList(i,0) Then %> selected="selected" <%End If %>><%=ArrayList(i,1) %></option>
                    <% 
                        Next  
                        Set ArrayList = nothing
                    %>
                    </select>           
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
    </form>
    <% 
        Call ConnectGame(QPTreasureDB)
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
                OrderStr = "ApplyDate DESC"
            End If
            queryCondition = " DetailID=0"
            Dim field
            field = "DetailID,UserID,ApplyDate,Accounts,GameID,SerialID,CardTypeID,CardPrice,CardGold,BeforeGold,IPAddress,OperUserID"
            '查询条件             
            
            Dim startDate,endDate
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
                queryCondition = " UserID="&Request("id")&" And (ApplyDate between '"&startDate&"' And '"&endDate&"')"
            End If
            
            Select Case Request("cardType")      
                case 0
                    queryCondition = queryCondition&""
                case else
                    queryCondition = queryCondition&" And CardTypeID="&Request("cardType")  
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
	            .Pkey="DetailID"					'主键
	            .Field=field	'字段
	            .Table="ShareDetialInfo"			'表名
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
                    <th class="listTitle1"><a class="l1"  href="" onclick="GetOrder('ApplyDate',this)">充值时间</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('Accounts',this)">用户名</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('GameID',this)">游戏ID</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SerialID',this)">充值卡号</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardTypeID',this)">实卡名称</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardPrice',this)">实卡价格</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardGold',this)">赠送金币</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('BeforeGold',this)">用户金币</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('IPAddress',this)">充值地址</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('OperUserID',this)">操作用户</a></th>
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
                    <td><%=rs(2,i) %></td>  
                    <td><%=rs(3,i) %></td>  
                    <td><%=rs(4,i) %></td>  
                    <td><a href="javascript:void(0)" onclick="javascript:openWindow('../FilledManager/LivcardInfo.asp?SerialID=<%=rs(5,i) %>',700,500)" class="l"><%=rs(5,i) %></a></td>
                    <td><%=CxGame.GetCardNameByCardID(rs(6,i)) %></td>
                    <td><%=FormatNumber(rs(7,i)) %></td>
                    <td><%=FormatNumber(rs(8,i)) %></td>
                    <td><%=FormatNumber(rs(9,i)) %></td>
                    <td><%=rs(10,i) %></td>
                    <td>
                        <%If rs(11,i) <> rs(1,i) Then%>
                        <%=CxGame.GetUserNameByUserID(rs(11,i)) %>
                        <%End If %>
                    </td>
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
