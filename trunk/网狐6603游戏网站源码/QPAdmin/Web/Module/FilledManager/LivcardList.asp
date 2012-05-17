<!--#include file="../../CommonFun.asp"-->
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
    <script type="text/javascript" src="../../Js/Check.js"></script>
    <script type="text/javascript" src="../../Js/Calendar.js"></script>
     <script type="text/javascript" src="../../Js/Sort.js"></script>   
</head>
<%
    If CxGame.GetRoleValue("200",trim(session("AdminName")))<"1" Then
    response.redirect "/Index.asp"   
    End If
%>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：充值系统 - 实卡管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
        <tr>
            <td height="28">
                <ul>
                    <li class="tab1">会员卡管理</li>
	                <li class="tab2" onclick="openWindowSelf('LivcardCreate.asp')">会员卡生成</li>
	                <li class="tab2" onclick="openWindowSelf('LivcardStat.asp')">库存统计</li>
	                <li class="tab2" onclick="openWindowSelf('LivcardGlobal.asp')">类型管理</li>
                </ul>
            </td>
        </tr>
    </table> 
    <script type="text/javascript">
        function CheckForm1()
        {
            var startDate = document.form1.StartDate.value;
            var endDate = document.form1.EndDate.value;
            if(!isDate(startDate)||!isDate(endDate))
            {
                alert("日期格式不正确！");
                return false;
            }
            if(compareDate(startDate,endDate)==false)
		    {
		        
		        alert("开始时间不可大于结束时间！")
		         return
		    }
        }
        
        function CheckForm2()
        {
            var info = document.form2.search.value.trim();
            if(info=="")
            {
                alert("卡号不能为空！");
                return
            }
            else
            {
                openWindow("LivcardInfo.asp?SerialID="+info,700,500);
            }
        }
    </script>
    <form name="form1" id="form1" action="" onsubmit="return CheckForm1()">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="titleQueBg">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    批次查询：
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
                    <input type="submit" value="查询" class="btn wd1" />                    
                </td>
            </tr>
        </table> 
   </form>
   <form name="form2" action="" >
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="titleQueBg Tmg7">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    卡号查询：
                </td>
                <td style="width: 220px">
                    <input name="search" id="search" type="text" class="text"  value="<%=CxGame.GetInfo(0,"search") %>"/>
                    <input type="button" value="查询" class="btn wd1" onclick="CheckForm2()" />                    
                </td>
                <td class="listTdLeft" style="width: 80px">
                    销售商查询：
                </td>
                 <td>
                    <input name="what" id="what" type="text" class="text"  value="<%=CxGame.GetInfo(0,"what") %>"/>
                    <input type="submit" value="查询" class="btn wd1" onclick="CheckForm3()" />                    
                </td>
            </tr>
        </table>      
    </form> 
    <% 
        Call ConnectGame(QPTreasureDB)
        Select case Request("action")
            case "download"
                Call DownLoadCard()
            case "getcardinfo"
                Call GetCardInfo()
            case else
                Call Main()
        End Select        
        Call CloseGame()
        
        '导出实卡
        Sub DownLoadCard()
            '更新次数
            GameConn.Execute("update LivcardBuildStream set DownLoadCount=DownLoadCount+1 Where BuildID="&Request("id"))
            
            Dim rs,sql,arrayList,i,cardInfo,cardTypeID,cardCount
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from LivcardBuildStream where BuildID="&Request("id")
            rs.Open sql,GameConn,1,3
            cardTypeID = rs("CardTypeID")
            cardCount = rs("BuildCount")
            arrayList = Split(rs("BuildCardPacket"),"/")
            Set rs = nothing
            cardInfo = "第 "&Request("id")&" 批次 ("&CxGame.GetCardNameByCardID(cardTypeID)&") "&cardCount&" 张"&vbNewLine
            cardInfo = cardInfo&"===================================================="&vbNewLine
            cardInfo = cardInfo&"卡号,卡密"&vbNewLine
            For i=0 To UBound(arrayList)-1 
                cardInfo = cardInfo& arrayList(i)&vbNewLine
            Next
            '导出
            Dim fileName
            fileName = Year(Date())&Month(Date())&Day(Date())&Request("id")&".txt"
            Response.Clear()
            Response.ContentType = "application/text"
            Response.AddHeader "Content-Disposition", "attachment;filename=" & fileName
            Response.Write(cardInfo)
            Response.End()
        End Sub
        
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
                OrderStr = "BuildDate DESC"
            End If
            queryCondition = "BuildDate between '"&Year(Date())&"-"&Month(Date())&"-1"&" 00:00:00"&"' And '"&Date()&" 23:59:59"&"'"
            '查询条件     
            If CxGame.GetInfo(0,"StartDate")<>"" Then
                startDate = CxGame.GetInfo(0,"StartDate")&" 00:00:00"
                endDate = CxGame.GetInfo(0,"EndDate")&" 23:59:59"
                queryCondition = "BuildDate between '"&startDate&"' And '"&endDate&"'"
            End If   
            
            If CxGame.GetInfo(0,"what")<>"" Then
                queryCondition = "BuildID in (select BuildID from LivcardAssociator where SalesPerson ='"&CxGame.GetInfo(0,"what")&"')"
            End If    
            
            Dim field
            field = "BuildID,BuildDate,AdminName,CardTypeID,BuildCount,CardPrice,BuildCount*CardPrice As PriceCount,CardGold,BuildAddr,DownLoadCount,NoteInfo"
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize					'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="BuildID"					'主键
	            .Field=field	'字段
	            .Table="LivcardBuildStream"			'表名
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
    <script type="text/javascript">
        function DownloadCard(buildID)
        {
            if(!confirm("确定要执行选定的操作吗？"))
            {
                return;
            }
            else
            {
                document.myformList.action = "?action=download&id="+buildID;
                document.myformList.submit();
            }
        }
    </script>
    <form name="myformList" method="post" action=''>
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box Tmg7">
                <tr align="center" class="bold">
                    <th class="listTitle1"><a class="l1"  href="" onclick="GetOrder('BuildID',this)">生产批次</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('BuildDate',this)">生成日期</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('AdminName',this)">管理员</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('BuildID',this)">销售商</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardTypeID',this)">实卡名称</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('BuildCount',this)">实卡数量</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardPrice',this)">实卡价格</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('PriceCount',this)">总金额</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardGold',this)">赠送金币</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('BuildAddr',this)">地址</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('DownLoadCount',this)">导出次数</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('NoteInfo',this)">备注</a></th>   
                    <th class="listTitle">管理</th>                 
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
                <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='default';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                    <td><%=rs(0,i) %></td>  
                    <td><%=rs(1,i) %></td> 
                    <td><%=rs(2,i) %></td> 
                    <td><%=CxGame.GetSalesPersonByBuildID(rs(0,i))%></td>
                    <td><%=CxGame.GetCardNameByCardID(rs(3,i)) %></td> 
                    <td><%=rs(4,i) %></td> 
                    <td><%=FormatNumber(rs(5,i)) %></td> 
                    <td><%=FormatNumber(rs(6,i)) %></td> 
                    <td><%=FormatNumber(rs(7,i)) %></td> 
                    <td><%=rs(8,i) %></td> 
                    <td><%=rs(9,i) %></td> 
                    <td><%=rs(10,i) %></td> 
                    <td>
                        <a href="javascript:void(0)" onclick="DownloadCard(<%=rs(0,i) %>)" class="l">实卡导出</a>
                        <a href="javascript:void(0)" onclick="openWindowOwn('LivcardListByBuild.asp?id=<%=rs(0,i) %>',<%=rs(0,i) %>,800,580)" class="l">实卡信息</a>
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
