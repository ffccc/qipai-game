<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../function.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->
<!--#include file="../../conn.asp"-->

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
     <script type="text/javascript" src="../../Js/Sort.js"></script>
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
            <td width="1232" height="25" valign="top" align="left">你当前位置：游戏用户 - 泡点设置</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->
    <% 
        Call ConnectGame("QPPlatformManagerDB")
        Select case lcase(Request("action"))            
            case else
            Call Main()
        End Select
        Call CloseGame()  
        
        '删除操作
        Sub Delete(lID)
            Dim ID
            ID = lID
            GameConn.execute "delete GrantInfo where SiteID='"&ID&"'"            
        End Sub
        
        Sub Main() 
            Dim cidArray, lLoop
            cidArray = Split(Request("cid"),",")
            For lLoop=0 To UBound(cidArray)
                Call Delete(cidArray(lLoop))
            Next           
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
                OrderStr = "SiteID Asc"
            End If
            
            Dim field
            field = "*"
            '查询条件
            
           
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = GetPageSize					'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="SiteID"					'主键
	            .Field=field	'字段
	            .Table="GrantInfo"			'表名
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
        function CheckFormList()
        {
            if(!confirm("确定要执行选定的操作吗？"))
            {
                return false;
            }
            else
            {
                var cid="";
                var cids = GelTags("input");                
                for(var i=0;i<cids.length;i++)
                {
                    if(cids[i].checked)
                    {
                        if(cids[i].name == "cid")
                            cid+=cids[i].value+",";
                    }
                }       
                if(cid=="")
                {
                    showError("未选中任何数据");
                    return false;
                }
            }
        }
    </script>
    <form name="myformList" method="post" action='' onsubmit="return CheckFormList();">
        <% If Request("Action") = "DeleteAll" Then %>
        <script type="text/javascript">
            showInfo("删除成功")
        </script>
        <% End If %>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="新建" class="btn wd1" onclick="Redirect('GrantManager.asp')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />
                    <input type="hidden" name="Action" value="DeleteAll" />         
                </td>
            </tr>
        </table>
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box">
                <tr class="bold">
                    <th class="listTitle1" width='38' align='center'><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></th>
                    <th class="listTitle1"><a class="l1" href="" onclick="GetOrder('SiteID',this)">记录标识</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('GrantRoom',this)">游戏房间</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('GrantStartDate',this)">赠送开始时间</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('GrantEndDate',this)">赠送结束时间</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('GrantObjet',this)">赠送对象</a></th>                    
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('MaxGrant',this)">封顶总值</a></th>
                    <th class="listTitle2"><a class="l1"  href="" onclick="GetOrder('DayMaxGrant',this)">单日封顶值</a></th>
                    <th class="listTitle">操作</th>
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
                    <td><input name='cid' type='checkbox' value='<%=rs(0,i)%>'/></td>  
                    <td>
                        <%=rs(0,i) %>
                    </td>
                    <td>
                    <% If rs(1,i)="-2" Then %>
                    积分房间
                    <% ElseIf rs(1,i)="-1" Then %>
                    金币房间
                    <% Else %>
                    <%=CxGame.GetRoomNameByRoomID(Trim(CInt(rs(1,i)))) %>
                    <% End If %>
                    </td>           
                    <td><%=rs(2,i) %></td>       
                    <td><%=rs(3,i) %></td>       
                    <td>
                    <% 
                        Dim strObj
                        If InStr(rs(4,i),"0")>0 Then
                            IF strObj<>"" Then
                                strObj=strObj&",全体用户"
                            Else
                                strObj="全体用户"
                            End If
                        End IF
                        If InStr(rs(4,i),"1")>0 Then
                            IF strObj<>"" Then
                                strObj=strObj&",蓝钻会员"
                            Else
                                strObj="蓝钻会员"
                            End If
                        End IF
                        If InStr(rs(4,i),"2")>0 Then
                            IF strObj<>"" Then
                                strObj=strObj&",红钻会员"
                            Else
                                strObj="红钻会员"
                            End If
                        End IF
                        If InStr(rs(4,i),"3")>0 Then
                            IF strObj<>"" Then
                                strObj=strObj&",黄钻会员"
                            Else
                                GrantObjet="黄钻会员"
                            End If
                        End IF
                        If InStr(rs(4,i),"4")>0 Then
                            IF strObj<>"" Then
                                strObj=strObj&",紫钻会员"
                            Else
                                strObj="紫钻会员"
                            End If
                        End IF
                         If InStr(rs(4,i),"5")>0 Then
                            IF strObj<>"" Then
                                strObj=strObj&",普通用户"
                            Else
                                strObj="紫钻会员"
                            End If
                        End IF
                            
                    %>
                    <%=strObj %>
                    <% strObj="" %>
                    </td>       
                    <td><%=rs(5,i) %></td>       
                    <td><%=rs(6,i) %></td>            
                    <td><a class="l" href="GrantManager.asp?siteid=<%=rs(0,i) %>">基本设置</a> <a class="l" href="GrantTimeCountsInfo.asp?siteid=<%=rs(0,i) %>">时段配置</a></td>
                </tr>
                <% 
                    Next
                    End If
                %>
            </table>
        </div>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="listTitleBg"><span>选择：</span>&nbsp;<a class="l1" href="javascript:SelectAll(true);">全部</a>&nbsp;-&nbsp;<a class="l1" href="javascript:SelectAll(false);">无</a></td>
                <td class="page" align="right"><%Response.Write nav%></td>
            </tr>
        </table> 
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" id="OpList">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="新建" class="btn wd1" onclick="Redirect('GrantManager.asp')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />
                    <input type="hidden" name="Action" value="DeleteAll" />         
                </td>
            </tr>
        </table> 
    </form>
    <% 
        End Sub
    %>
</body>
</html>