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
    
     <script type="text/javascript" src="../../Js/Sort.js"></script>   
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">目前操作功能：实卡列表</td>
        </tr>
    </table>
    <form id="form1" action="">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="titleQueBg Tmg7">
            <tr>
                <td class="listTdLeft" style="width: 80px">
                    快速查询：
                </td>
                <td>
                    <a href="?id=<%=Request("id") %>&used=1" class="l">已使用</a> | 
                    <a href="?id=<%=Request("id") %>&used=2" class="l">未使用</a> |
                    <a href="?id=<%=Request("id") %>&used=3" class="l">已禁用</a> |
                    <a href="?id=<%=Request("id") %>&used=4" class="l">未禁用</a> |
                    <a href="?id=<%=Request("id") %>&used=0" class="l">全部</a>
                </td>
            </tr>
        </table>
    </form>
    <!-- 头部菜单 End -->
    <%
        Call ConnectGame(QPTreasureDB)
        Call Main()
        Call CloseGame()
        
        '禁用还原
        Sub Nullity(lID,typeValue)
            Dim ID
            ID = lID
            GameConn.execute "Update LivcardAssociator Set Nullity="&typeValue&" Where CardID="&lID
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop,acType
            cidArray = Split(Request("cid"),",")
            acType = Request("action")
            For lLoop=0 To UBound(cidArray)
                Select case acType
                    case "jinyong"
                        Call Nullity(cidArray(lLoop),1)
                    case "huanyuan"
                        Call Nullity(cidArray(lLoop),0)
                End Select
            Next          
            
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
                OrderStr = "CardID ASC"
            End If
            queryCondition = "BuildID="&Request("id")   
            If CxGame.GetInfo(1,"used")<>"" Then
                Select case CxGame.GetInfo(1,"used")
                    case 0
                    case 1
                        queryCondition = queryCondition&" And ApplyDate is not null"
                    case 2
                        queryCondition = queryCondition&" And ApplyDate is null"
                    case 3
                        queryCondition = queryCondition&" And Nullity = 1"
                    case 4
                        queryCondition = queryCondition&" And Nullity = 0"
                End Select
            End If        
            
            Dim field
            field = "CardID,SerialID,CardTypeID,CardPrice,CardGold,MemberOrder,MemberDays,ValidDate,ApplyDate,Nullity,UseRange,SalesPerson"
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize					'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="CardID"					'主键
	            .Field=field	'字段
	            .Table="LivcardAssociator"			'表名
	            .Condition=queryCondition		'条件,不需要where
	            .OrderBy=OrderStr				'排序,不需要order by,需要asc或者desc
	            .RecordCount = 0				'总记录数，可以外部赋值，0不保存（适合搜索），-1存为session，-2存为cookies，-3存为applacation

	            .NumericJump = 2                '数字上下页个数，可选参数，默认为3，负数为跳转个数，0为显示所有
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
        function Operate(opType)
        {
            var opVal = document.myformList.in_optype
            if(!confirm("确定要执行选定的操作吗？"))
            {
                return;
            }
            else
            {
                var cid="";
                var cids = GelTags("input");                
                for(var i=0;i<cids.length;i++)
                {
                    if(cids[i].checked)
                        cid+=cids[i].value+",";
                }
                if(cid=="")
                {
                    alert("未选中任何数据");
                    return;
                }
                //操作处理
                if(opType=="jinyong")
                    opVal.value = "jinyong";
                if(opType=="huanyuan")
                    opVal.value = "huanyuan";
            }
            document.myformList.action = "?id=<%=Request("id") %>&action="+opVal.value;
            document.myformList.submit();
        }
    </script>
    <form name="myformList" method="post" action=''>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="Tmg7">
            <tr>
                <td height="39" class="titleOpBg">
	                <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />
                    <input class="btnLine" type="button" />   
                    <input type="button" value="禁用" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input type="button" value="还原" class="btn wd1" onclick="Operate('huanyuan')"/>  
                    <input name="in_optype" type="hidden" />    
                </td>
            </tr>
        </table>
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr align="center" class="bold">
                    <th class="listTitle1" width='38' align='center'><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SerialID',this)">卡号</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardTypeID',this)">实卡名称</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardPrice',this)">实卡价格</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('CardGold',this)">赠送金币</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MemberOrder',this)">会员等级</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MemberDays',this)">会员天数</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ValidDate',this)">有效期限</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('ApplyDate',this)">充值日期</a></th>    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('UseRange',this)">使用范围</a></th>          
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('SalesPerson',this)">销售商</a></th>                           
                    <th class="listTitle"><a  class="l1" href="" onclick="GetOrder('Nullity',this)">禁用状态</a></th>                                
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
                    <td><input name='cid' type='checkbox' value='<%=rs(0,i)%>'/></td>     
                    <td><a href="javascript:void(0)" onclick="javascript:openWindow('LivcardInfo.asp?SerialID=<%=rs(1,i) %>',700,500)" class="l"><%=rs(1,i) %></a></td>
                    <td><%=CxGame.GetCardNameByCardID(rs(2,i)) %></td> 
                    <td><%=FormatNumber(rs(3,i)) %></td> 
                    <td><%=FormatNumber(rs(4,i)) %></td> 
                    <td><%=CxGame.GetMemberOrderName(rs(5,i)) %></td> 
                    <td><%=rs(6,i) %></td> 
                    <td><%=rs(7,i) %></td> 
                    <td><%=rs(8,i) %></td> 
                    <td><%=CxGame.GetCardUseRange(rs(10,i)) %></td>               
                    <td><%=rs(11,i) %></td>                   
                    <td><%=CxGame.GetNullityInfo(rs(9,i)) %></td>                                  
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
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" id="OpList">
            <tr>
                <td height="39" class="titleOpBg">
	                <input type="button" value="关闭" class="btn wd1" onclick="window.close()" />
                    <input class="btnLine" type="button" />   
                    <input type="button" value="禁用" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input type="button" value="还原" class="btn wd1" onclick="Operate('huanyuan')"/>  
                    <input name="in_optype" type="hidden" />    
                </td>
            </tr>
        </table>
    </form>
    <% 
        End Sub
    %>
</body>
</html>
