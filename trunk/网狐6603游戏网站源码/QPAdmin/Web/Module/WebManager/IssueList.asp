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
    
</head>

	
<body>
   
     <!-- 头部菜单 Start -->
     
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：管理中心 - 问题管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->  
     <%
     
	    Call ConnectGame("QPNativeWebDB")
        Select case lcase(Request("action"))
            case "save"
            Call Save()
            case "newinfo"
            Call NewInfo()
            case "delete"
            Call DeleteInfo()
            case else
            Call Main()
        End Select
        
        '删除
        Sub DeleteInfo(lID)
            Dim ID
            ID = Trim(lID)
            GameConn.execute "Delete From GameIssueInfo Where IssueID="&ID
	        Call CxGame.ShowInfo2("删除成功！","IssueList.asp",1200)
        End Sub
       
        '禁用还原
        Sub Nullity(lID,typeValue)
            Dim ID
            ID = lID
            GameConn.execute "Update GameIssueInfo Set Nullity="&typeValue&" Where IssueID="&lID
        End Sub
        
        '新增更新
	    Sub Save()
	        If Request("IssueID")<>"" Then
	            GameConn.execute "update GameIssueInfo set IssueTitle='"&Request("IssueTitle")&"',IssueContent='"&Request("content1")&"',Nullity='"&Request("Nullity")&"',ModifyDate='"&Now()&"' Where IssueID="&Request("IssueID")
	            Call CxGame.ShowInfo2("更新成功！","IssueList.asp",1200)
	        Else 
	            GameConn.execute "insert into  GameIssueInfo (IssueTitle,IssueContent,Nullity,CollectDate) values ('"&Request("IssueTitle")&"','"&Request("content1")&"','"&Request("Nullity")&"','"&Now()&"')"   
	            Call CxGame.ShowInfo2("新增成功！","IssueList.asp",1200)
	        End If
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop,acType
            cidArray = Split(Request("cid"),",")
            acType = Request("action")
            For lLoop=0 To UBound(cidArray)
                Select case acType
                    case "del"
                        Call DeleteInfo(cidArray(lLoop))
                    case "jinyong"
                        Call Nullity(cidArray(lLoop),1)
                    case "huanyuan"
                        Call Nullity(cidArray(lLoop),0)
                End Select
            Next            
            
            If Request("orders")<>""And Request("OrderType")<>"" Then
                If Request("OrderType")<>"-102" Then
                    OrderStr=Replace(Request("orders")," ","+")&" Asc "
                End If
                If Request.QueryString("OrderType")<>"-101" Then
                    OrderStr=Replace(Request("orders")," ","+")&" Desc "
                End If
            Else 
                OrderStr = "CollectDate Asc"
            End If   
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = 20				'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="IssueID"					'主键
	            .Field="*"	'字段
	            .Table="GameIssueInfo"			'表名
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
       function Operate(opType)
        {
            var opVal = document.form1.in_optype
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
                    {
                        if(cids[i].name == "cid")
                            cid+=cids[i].value+",";
                    }
                }       
                if(cid=="")
                {
                    showError("未选中任何数据");
                    return;
                }
                //操作处理
                if(opType=="del")
                    opVal.value = "del";
                if(opType=="jinyong")
                    opVal.value = "jinyong";
                if(opType=="huanyuan")
                    opVal.value = "huanyuan";
            }
            document.form1.action = "?action="+opVal.value;
            document.form1.submit();
        }
    </script>
    <form id="form1" name="form1" method="post" action="?action=orderinfo">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td height="39" class="titleOpBg">
                    <input type="button" value="新增" class="btn wd1" onclick="Redirect('IssueList.asp?action=newinfo')" />
                    <input class="btnLine" type="button" />
                    <input type="button" value="删除" class="btn wd1" onclick="Operate('del')" />
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
                    <th class="listTitle1"><a class="l1"  href="" onclick="GetOrder('IssueID',this)">问题标识</a></th>
                    <th class="listTitle2" ><a class="l1"  href="" onclick="GetOrder('IssueTitle',this)">问题标题</a></th>
                    <th class="listTitle2" ><a class="l1"  href="" onclick="GetOrder('Nullity',this)">禁用状态</a></th>
                    <th class="listTitle2" ><a class="l1"  href="" onclick="GetOrder('CollectDate',this)">新增日期</a></th>
                    <th class="listTitle"><a class="l1"  href="" onclick="GetOrder('ModifyDate',this)">修改日期</a></th>
                </tr>
                <% 
                    Dim rs,sql,id,className,rowNum,OrderStr
                   
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
                 <tr class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor"> 
                    <td><input name='cid' type='checkbox' value='<%=rs(0,i)%>'/></td>   
                    <td onclick="Redirect('IssueList.asp?action=newinfo&IssueID=<%=rs(0,i) %>')"><%=rs(0,i) %></td>    
                    <td onclick="Redirect('IssueList.asp?action=newinfo&IssueID=<%=rs(0,i) %>')"><%=rs(1,i) %></td>    
                  <td>
                        <% If rs(3,i)=0 Then%>
                        否                        
                        <% Else %>
                        <span style="color:red">是</span>
                        <% End If %>
                    </td>   
                    <td onclick="Redirect('IssueList.asp?action=newinfo&IssueID=<%=rs(0,i) %>')"><%=rs(4,i) %></td> 
                    <td onclick="Redirect('IssueList.asp?action=newinfo&IssueID=<%=rs(0,i) %>')"><%=rs(5,i) %></td>
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
                   <input type="button" value="新增" class="btn wd1" onclick="Redirect('IssueList.asp?action=newinfo')" />
                   <input class="btnLine" type="button" />
                   <input type="button" value="删除" class="btn wd1" onclick="Operate('del')" />
                    <input type="button" value="禁用" class="btn wd1" onclick="Operate('jinyong')"/>
                    <input type="button" value="还原" class="btn wd1" onclick="Operate('huanyuan')"/>
                </td>
            </tr>
        </table>        
    </form>
    <%
        End Sub
        
        Sub NewInfo()
            Dim rs,sql,IssueTitle,IssueContent,Nullity,IssueID
            IssueID=Request("IssueID")
            If(Request("IssueID")<>"") Then
                Set rs=server.createobject("adodb.recordset")
                sql="select * from GameIssueInfo where IssueID="&Request("IssueID")
                rs.open sql,GameConn,1,3
                IssueTitle=rs("IssueTitle")
                IssueContent=rs("IssueContent")
                Nullity=rs("Nullity")
            Else
                IssueTitle=""
                IssueContent=""
                Nullity=""
            End If
    %> 
    <script type="text/javascript" src="../../kindeditor/kindeditor.js"></script>
    <script type="text/javascript">
		KE.show({
			id : 'content1',
			urlType : 'absolute',
			imageUploadJson : '../../asp/upload_json.asp',
			fileManagerJson : '../../asp/file_manager_json.asp',
			allowFileManager : true,
			afterCreate : function(id) {
				KE.event.ctrl(document, 13, function() {
					KE.util.setData(id);
					document.forms['example'].submit();
				});
				KE.event.ctrl(KE.g[id].iframeDoc, 13, function() {
					KE.util.setData(id);
					document.forms['example'].submit();
				});
			}
		});
		
		function CheckFormInfo()
		{
		   
		    var IssueTitle = document.form1.IssueTitle.value.trim()
		    var content1 = document.form1.content1.value.trim();
		    if(IssueTitle=="")
		    {
		        alert("问题标题不能为空！");
		        return false;
		    }
		    if(content1=="")
		    {
		        alert("问题内容不能为空！");
		        return false;
		    }
		}
	</script>
    <form name="form1" method="post" action="?action=save&IssueID=<%=IssueID %>"   onsubmit="return CheckFormInfo();">
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" >
             <tr>
                 <td height="39" class="titleOpBg">
                     <input type="submit" value="保存" class="btn wd1" /><input type="hidden" name="in_IssueID" value="<%=IssueID%>"/>
                     <input class="btnLine" type="button" />
                     <input type="button" value="返回" class="btn wd1" onclick="Redirect('IssueList.asp')" />
                 </td>
             </tr>
         </table>  
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7">问题管理</div></td>
            </tr>
            <tr>
                <td class="listTdLeft">问题标题：</td>
                <td>
                    <input name="IssueTitle" id="IssueTitle" type="text" class="text wd4" style="width:694px" value="<%=IssueTitle %>" />
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">禁用状态：</td>
                <td>
                    <%
                        If Nullity="0" or Nullity="" Then
                    %>
                    <input name="Nullity" type="radio"  value="0"  checked="checked" />否
                    <input name="Nullity"  type="radio" value="1" />是
                    <%
                        Else 
                    %>
                    <input name="Nullity" type="radio" value="0" />否
                    <input name="Nullity"  type="radio" value="1" checked="checked" />是
                    <%
                        End If
                    %>
                </td>
            </tr>
             <tr>
                <td class="listTdLeft">问题内容：</td>
                <td>
                    <textarea id="content1"  name="content1" cols="100" rows="8" style="width:700px;height:350px;visibility:hidden;"><%=IssueContent %>
                    </textarea> 
                </td>
            </tr>
             <%
                End Sub
              %>
         </table>
         <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" id="OpList">
             <tr>
                 <td height="39" class="titleOpBg">
                     <input type="submit" value="保存" class="btn wd1" />
                     <input class="btnLine" type="button" />
                     <input type="button" value="返回" class="btn wd1" onclick="Redirect('IssueList.asp')" />
                 </td>
             </tr>
         </table>
     </form> 
    
</body>
</html>
