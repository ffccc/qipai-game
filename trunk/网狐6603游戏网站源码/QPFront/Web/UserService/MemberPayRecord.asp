<!--#include file="../Inc/CommonFun.asp" -->
<!--#include file="../Inc/Conn.asp" -->
<!--#include file="../Inc/Cls_Page.asp" -->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
    <meta name="Description" content="<%=CxGame.WebTitle %>" />
    <meta name="Keywords" content="<%=CxGame.WebTitle %>" />
    <meta name="rating" content="general" />
    <meta name="robots" content="all" />
    <title><%=CxGame.WebTitle %></title>
    <link rel="stylesheet" type="text/css" href="../css/layout.css" />
    <link rel="stylesheet" type="text/css" href="../css/global.css" />
    <link rel="stylesheet" type="text/css" href="../Css/user_layout.css" />
    <link rel="stylesheet" type="text/css" href="../Css/game_layout.css" />   
    <script src="../JS/jquery_last.js" type="text/javascript"></script>   
    <script src="../JS/formValidatorRegex.js" type="text/javascript"></script>   
    <script src="../JS/Calendar.js" type="text/javascript"></script>   
    
    <script type="text/javascript">
        function QueryInfo()
        {            
            var startDate = document.form1.StartDate.value;
            var endDate = document.form1.EndDate.value;
            if(!isDate(startDate)||!isDate(endDate))
            {
                alert("日期格式不正确！");
                return false;
            }
        }
    </script>
</head>
<body style="background:none;">
<%
    CxGame.IsCheckLogin
%>
<div class="mainRight3">
  <div class="userRigthBody">
    <div class="userRigthTitle">首页 - 充值中心 - 充值记录</div>
    <div class="userRigthBg">
      <div class="title f14 bold lan">充值记录</div><br />
      <form name="form1" action="" method="get" onsubmit="return QueryInfo()">
	  	<table width="100%" border="0" cellspacing="0" cellpadding="0">
          <tr>
            <td align="right" style="width:60px;">日期查询：</td>
            <td align="left">
                <% If CxGame.GetInfo(0,"StartDate")<>"" Then %>
                <input name="StartDate" type="text" class="text" style="width:70px;" value="<%=CxGame.GetInfo(0,"StartDate") %>" maxlength="10" /><img src="../../Images/btn_calendar.gif" onclick="calendar(StartDate)" style="cursor:pointer;vertical-align:text-bottom"/>                    
                <% Else %>
                <input name="StartDate" type="text" class="text" style="width:70px;" value="<%=Year(Date())&"-"&Month(Date())&"-1" %>" maxlength="10" /><img src="../../Images/btn_calendar.gif" onclick="calendar(StartDate)" style="cursor:pointer;vertical-align:text-bottom"/>  
                <% End If %>
                至
                <% If CxGame.GetInfo(0,"EndDate")<>"" Then %>
                <input name="EndDate" type="text" class="text" style="width:70px;" value="<%=CxGame.GetInfo(0,"EndDate") %>" maxlength="10" /><img src="../../Images/btn_calendar.gif" onclick="calendar(EndDate)" style="cursor:pointer;vertical-align:text-bottom"/>                    
                <% Else%>
                <input name="EndDate" type="text" class="text" style="width:70px;" value="<%=Date() %>"  maxlength="10"/><img src="../../Images/btn_calendar.gif" onclick="calendar(EndDate)" style="cursor:pointer;vertical-align:text-bottom"/>                    
                <% End If %>
                <select name="shareType" style="width:80px;">
                <option value="0">全部服务</option>
                <%
                    Dim ShareList,j 
                    ShareList = CxGame.GetShareInfoList
                    For j=0 To Ubound(ShareList)       
                %>
                <option value="<%=ShareList(j,0) %>" <%If CxGame.GetInfo(1,"shareType") = ShareList(j,0) Then %> selected="selected" <%End If %>><%=ShareList(j,1) %></option>
                <% 
                    Next  
                    Set ShareList = nothing
                %>
                </select>
                <input type="submit" value="查询" class="bnt" />
                <input type="hidden" name="QueryType" />     
            </td>
          </tr>
        </table>
        </form>
        <% 
            Call ConnectGame(QPTreasureDB)
            Call Main()
            Call CloseGame()
            
            Sub Main()
                Dim rs,nav,Page,i
                Dim lCount, queryCondition, OrderStr
                Dim startDate,endDate
                OrderStr = "ApplyDate DESC"                           
                queryCondition = "DetailID=0"
                '查询条件
                startDate = CxGame.GetInfo(0,"StartDate")&" 00:00:00"
                endDate = CxGame.GetInfo(0,"EndDate")&" 23:59:59"
                
                If CxGame.GetInfo(0,"shareType")<>"" Then
                    Select case CxGame.GetInfo(1,"SelectType")
                        case 0
                            queryCondition = "UserID="&Session("UserID")&" And (ApplyDate between '"&startDate&"' And '"&endDate&"')"
                        case 1
                            queryCondition = "UserID="&Session("UserID")&" And ShareID = "&CxGame.GetInfo(0,"shareType")&" And (ApplyDate between '"&startDate&"' And '"&endDate&"')"
                        case 2
                            queryCondition = "UserID="&Session("UserID")&" And ShareID = "&CxGame.GetInfo(0,"shareType")&" And (ApplyDate between '"&startDate&"' And '"&endDate&"')"
                        case 3
                            queryCondition = "UserID="&Session("UserID")&" And ShareID = "&CxGame.GetInfo(0,"shareType")&" And (ApplyDate between '"&startDate&"' And '"&endDate&"')"                                    
                    End Select
                End If    
                
                Dim fields
                fields = "DetailID,ApplyDate,ShareID,SerialID,OrderID,CardTypeID,CardPrice,CardGold,OrderAmount,PayAmount"    
                
                '==============================================================================================================
                '执行存储过程
                        
                Set Page = new Cls_Page      '创建对象
                Set Page.Conn = GameConn     '得到数据库连接对象        
                With Page
                    .PageSize = 20					'每页记录条数
                    .PageParm = "page"				'页参数
                    '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
                    .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
                    .Pkey="DetailID"					'主键
                    .Field=fields	'字段
    	            
                    .Table="ShareDetialInfo"		'表名
                    .Condition=queryCondition		'条件,不需要where
                    .OrderBy=OrderStr				'排序,不需要order by,需要asc或者desc
                    .RecordCount = 0				'总记录数，可以外部赋值，0不保存（适合搜索），-1存为session，-2存为cookies，-3存为applacation

                    .NumericJump = 1                '数字上下页个数，可选参数，默认为3，负数为跳转个数，0为显示所有
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
  	    <table width="100%" border="0" cellpadding="0" cellspacing="1" bgcolor="#BAD7E5" class="leftTop">
          <tr align="center" class="bold">
            <td class="leftTop trBg">充值日期</td>
            <td class="leftTop trBg">充值类型</td>
            <td class="leftTop trBg">充值卡号</td>
            <td class="leftTop trBg">订单号</td>
            <td class="leftTop trBg">卡类型</td>
            <td class="leftTop trBg">卡价格</td>  
            <td class="leftTop trBg">赠送金币</td>       
            <td class="leftTop trBg">订单金额</td>                        
            <td class="leftTop trBg">支付金额</td>                        
          </tr>   
          <% 
            Dim className
            If IsNull(rs) Then
                Response.Write("<tr><td bgcolor='#FFFFFF' colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
            Else
            
            For i=0 To Ubound(rs,2)
            If i Mod 2 = 0 Then
                className="trBg1"        
            Else
                className="trBg2"      
            End If 
        %>
        <tr align="center" class="<%=className %>">
            <td><%=rs(1,i) %></td>  
            <td><%=CxGame.GetShareNameByShareID(rs(2,i)) %></td>  
            <td><%=rs(3,i) %></td>  
            <td><%=rs(4,i) %></td>  
            <td><%=CxGame.GetCardNameByCardID(rs(5,i)) %></td>  
            <td><%=rs(6,i) %></td>  
            <td><%=rs(7,i) %></td>  
            <td>
            <%If rs(8,i)="0" Then %>
            &nbsp;
            <%Else %>
            <%=rs(8,i) %>
            <%End If %>
            </td>  
            <td><%=rs(9,i) %></td>  
        </tr>
        <% 
            Next
            End If
        %>    
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="page2" align="right"><%Response.Write nav%></td>
            </tr>
        </table>  
        </form>
        <% 
            End Sub
        %>
      <div class="clear"></div>
    </div>
    <div class="userRightBottom">
      <div class="clear"></div>
    </div>
    <div class="clear"></div>
  </div>
  <div class="clear"></div>
</div>
</body>
</html>