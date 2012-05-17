<!--#include file="../../CommonFun.asp"-->
<!--#include file="../../GameConn.asp"-->
<!--#include file="../../Cls_Page.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
    <title></title>
    <link href="../../Css/layout.css" rel="stylesheet" type="text/css" />
    <script type="text/javascript" src="../../Js/common.js"></script>
</head>
<body>
    <!-- 头部菜单 Start -->
    <table width="100%" border="0" cellpadding="0" cellspacing="0" class="title">
        <tr>
            <td width="19" height="25" valign="top"  class="Lpd10"><div class="arr"></div></td>
            <td width="1232" height="25" valign="top" align="left">你当前位置：管理中心 - 新闻管理</td>
        </tr>
    </table>
    <!-- 头部菜单 End -->  
    <% 
        Call ConnectGame(QPNativeWebDB)
        Select case lcase(Request("action"))
            case "newsinfo"
            Call GetNewsInfo()
            case "save"
            Call SaveNewsInfo()
            case else
            Call Main()
        End Select
        Call CloseGame()  
        
        Sub SaveNewsInfo()
            Dim rs,sql
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from News where NewsID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.eof Then
                rs.addnew
                rs("UserID") = session("AdminID")
                rs("IssueIP") = Request.ServerVariables("REMOTE_ADDR")
            End If
            rs("Subject") = CxGame.GetInfo(0,"in_Subject")
            rs("OnTop") = CxGame.GetInfo(1,"in_OnTop")
            rs("OnTopAll") = CxGame.GetInfo(1,"in_OnTopAll")
            rs("IsElite") = CxGame.GetInfo(1,"in_IsElite")
            rs("IsHot") = CxGame.GetInfo(1,"in_IsHot")
            rs("IsLock") = CxGame.GetInfo(1,"in_IsLock")
            rs("IsLinks") = CxGame.GetInfo(1,"in_IsLinks")
            If CxGame.GetInfo(0,"in_LinkUrl")="http://" Then
                rs("LinkUrl") = ""
            Else
                rs("LinkUrl") = CxGame.GetInfo(0,"in_LinkUrl")
            End If
            rs("Body") = CxGame.GetInfo(0,"content1")
            rs("ClassID") = CxGame.GetInfo(1,"in_ClassID")
            rs("LastModifyIP") = Request.ServerVariables("REMOTE_ADDR")
            rs("LastModifyDate") = Now()
            rs.update
            If Request("id")<>"" Then
                Call CxGame.ShowInfo2("修改成功！","NewsList.asp?page="&Request("page")&"",1200)
            Else
                Call CxGame.ShowInfo2("新增成功！","NewsList.asp",1200)
            End If
            rs.close()
            Set rs = Nothing    
        End Sub
        
        '删除
        Sub Delete(lID)
            Dim ID
            ID = Trim(lID)
            GameConn.execute "Delete From News Where NewsID="&lID
        End Sub
        
        Sub Main()
            Dim cidArray, lLoop
            cidArray = Split(Request("cid"),",")
            For lLoop=0 To UBound(cidArray)
                Call Delete(cidArray(lLoop))
            Next            
            
            Dim rs,nav,Page,i
            Dim lCount, queryCondition, OrderStr
            OrderStr = "IssueDate DESC"            
            
            Dim fields        
            fields = "NewsID,Subject,ClassID,IsLinks,IsLock,OnTopAll,OnTop,IsElite,IsHot,UserID,IssueDate"     
            
            '==============================================================================================================
            '执行存储过程
                    
            Set Page = new Cls_Page      '创建对象
            Set Page.Conn = GameConn     '得到数据库连接对象        
            With Page
                .PageSize = 20				'每页记录条数
                .PageParm = "page"				'页参数
                '.PageIndex = 10				'当前页，可选参数，一般是生成静态时需要
	            .Database = "mssql"				'数据库类型,AC为access,MSSQL为sqlserver2000存储过程版,MYSQL为mysql,PGSQL为PostGreSql
	            .Pkey="NewsID"					'主键
	            .Field=fields	'字段
	            .Table="News"			'表名
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
	                <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=newsinfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />     
                    <input type="hidden" name="Action" value="DeleteAll" />                
                </td>
            </tr>
        </table> 
        <div id="content">
            <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="box" id="list">
                <tr align="center" class="bold">
                    <td class="listTitle1" style="width:38px;"><input type="checkbox" name="chkAll" onclick="SelectAll(this.checked);" /></td>
                    <td class="listTitle2">标题</td>
                    <td class="listTitle2">类别</td>
                    <td class="listTitle2">引用</td>
                    <td class="listTitle2">发布</td>
                    <td class="listTitle2">总置顶</td>
                    <td class="listTitle2">置顶</td>
                    <td class="listTitle2">推荐</td>
                    <td class="listTitle2">热点</td>
                    <td class="listTitle2">作者</td>
                    <td class="listTitle2">时间</td>
                </tr>
                <% 
                    Dim NewsID,className
                    If IsNull(rs) Then
                        Response.Write("<tr class='tdbg'><td colspan='100' align='center'><br>没有任何信息!<br><br></td></tr>")
                    Else
                    
                    For i=0 To Ubound(rs,2)
                    NewsID = rs(0,i)     
                    If i Mod 2 = 0 Then
                        className="list"        
                    Else
                        className="listBg"      
                    End If 
                %>
                <tr align="center" class="<%=className %>" onmouseover="currentcolor=this.style.backgroundColor;this.style.backgroundColor='#caebfc';this.style.cursor='pointer';"
                    onmouseout="this.style.backgroundColor=currentcolor">
                <td><input name='cid' type='checkbox' value='<%=NewsID%>'/></td>     
                <td align="left" onclick="Redirect('?action=newsinfo&id=<%=NewsID %>&page=<%=Request("page") %>')"><%=rs(1,i) %></td>           
                <td onclick="Redirect('?action=newsinfo&id=<%=NewsID %>&page=<%=Request("page") %>')">
                    <% If rs(2,i)=1 Then %>
                    新闻
                    <% Else %>
                    公告
                    <% End If %>
                </td> 
                <td onclick="Redirect('?action=newsinfo&id=<%=NewsID %>&page=<%=Request("page") %>')">
                    <%=CxGame.GetStatusInfo(rs(3,i)) %>
                </td> 
                <td onclick="Redirect('?action=newsinfo&id=<%=NewsID %>&page=<%=Request("page") %>')">
                    <%=CxGame.GetStatusInfo(rs(4,i)) %>
                </td> 
                <td onclick="Redirect('?action=newsinfo&id=<%=NewsID %>&page=<%=Request("page") %>')">
                    <%=CxGame.GetStatusInfo(rs(5,i)) %>
                </td> 
                <td onclick="Redirect('?action=newsinfo&id=<%=NewsID %>&page=<%=Request("page") %>')">
                    <%=CxGame.GetStatusInfo(rs(6,i)) %>
                </td> 
                <td onclick="Redirect('?action=newsinfo&id=<%=NewsID %>&page=<%=Request("page") %>')">
                    <%=CxGame.GetStatusInfo(rs(7,i)) %>
                </td> 
                <td onclick="Redirect('?action=newsinfo&id=<%=NewsID %>&page=<%=Request("page") %>')">
                    <%=CxGame.GetStatusInfo(rs(8,i)) %>
                </td> 
                <td onclick="Redirect('?action=newsinfo&id=<%=NewsID %>&page=<%=Request("page") %>')">
                    <%=GetQPAdminName(rs(9,i)) %>
                </td> 
                <td onclick="Redirect('?action=newsinfo&id=<%=NewsID %>&page=<%=Request("page") %>')">
                    <%=rs(10,i) %>
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
                <td class="listTitleBg"><span>选择：</span>&nbsp;<a class="l1" href="javascript:SelectAll(true);">全部</a>&nbsp;-&nbsp;<a class="l1" href="javascript:SelectAll(false);">无</a></td>
                <td class="page" align="right"><%Response.Write nav%></td>
            </tr>
        </table>        
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" id="OpList">
            <tr>
                <td height="39" class="titleOpBg">
	                <input type="button" value="新建" class="btn wd1" onclick="Redirect('?action=newsinfo')" />
                    <input class="btnLine" type="button" />
                    <input type="submit" value="删除" class="btn wd1" />                  
                </td>
            </tr>
        </table> 
    </form>
    <% 
        End Sub
        
        Sub GetNewsInfo()
            Dim rs,sql,titleInfo
            Dim NewsID,Subject,OnTop,OnTopAll,IsElite,IsHot,IsLock,IsLinks,LinkUrl,Body,ClassID
            Set rs=Server.CreateObject("Adodb.RecordSet")
            sql = "select * from dbo.News where NewsID='"&Request("id")&"'"
            rs.Open sql,GameConn,1,3
            If rs.Bof And rs.Eof Then
                titleInfo = "新增新闻"     
                IsLock = 1         
                LinkUrl="http://"  
            Else
                titleInfo = "修改新闻"  
                Subject = rs("Subject")
                OnTop = rs("OnTop")
                OnTopAll = rs("OnTopAll")    
                IsElite = rs("IsElite")
                IsHot = rs("IsHot")
                IsLock = rs("IsLock")
                IsLinks = rs("IsLinks")
                LinkUrl = rs("LinkUrl")
                Body = rs("Body")
                ClassID = rs("ClassID")
            End If   
    %>
    <script type="text/javascript" src="../../kindeditor/kindeditor.js"></script>
    <script type="text/javascript">
		KE.show({
			id : 'content1',
			urlType : 'absolute',
			imageUploadJson : '../../asp/upload_json.asp?type=',
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
		    var subject = document.myFormInfo.in_Subject.value.trim()
		    var content1 = document.myFormInfo.content1.value.trim();
		    if(subject=="")
		    {
		        alert("标题不能为空！");
		        return false;
		    }
		    if(content1=="")
		    {
		        alert("内容不能为空！");
		        return false;
		    }
		}
	</script>
    <form name="myFormInfo" method="post" action='?action=save&id=<%=Request("id") %>&page=<%=Request("page") %>' onsubmit="return CheckFormInfo();">
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('NewsList.asp')" />                           
                    <input class="btnLine" type="button" />  
                    <input type="submit" value="保存" class="btn wd1" />  
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0" class="listBg2">
            <tr>
                <td height="35" colspan="2" class="f14 bold Lpd10 Rpd10"><div class="hg3  pd7"><%=titleInfo %></div></td>
            </tr>
            <tr>
                <td class="listTdLeft">标题：</td>
                <td>
                    <select name="in_ClassID" style="width:80px;">
                    <option value="1" <%If ClassID=1 Then %> selected="selected" <% End If %>>新闻</option>
                    <option value="2" <%If ClassID=2 Then %> selected="selected" <% End If %>>公告</option>
                    </select>  
                    <input name="in_Subject" type="text" class="text" style="width:500px;" value="<%=Subject %>"/>                                    
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">属性：</td>
                <td>
                    <input name="in_OnTop" id="in_OnTop" type="checkbox" value="1" <%If OnTop=1 Then %> checked="checked" <% End If %> /><label for="in_OnTop">置顶新闻</label>
                    <input name="in_OnTopAll" id="in_OnTopAll" type="checkbox" value="1" <%If OnTopAll=1 Then %> checked="checked" <% End If %> /><label for="in_OnTopAll">总置顶新闻</label>    
                    <input name="in_IsElite" id="in_IsElite" type="checkbox"value="1" <%If IsElite=1 Then %> checked="checked" <% End If %> /><label for="in_IsElite">推荐新闻</label>         
                    <input name="in_IsHot" id="in_IsHot" type="checkbox" value="1" <%If IsHot=1 Then %> checked="checked" <% End If %> /><label for="in_IsHot">热门新闻</label>              
                    <input name="in_IsLock" id="in_IsLock" type="checkbox" value="1" <%If IsLock=1 Then %> checked="checked" <% End If %> /><label for="in_IsLock">立即发布</label>                                                                               
                </td>
            </tr>
            <tr>
                <td class="listTdLeft">引用：</td>
                <td>                    
                    <input name="in_LinkUrl" type="text" class="text" style="width:585px;" <% If LinkUrl="" Then %> value="http://" <% Else %> value="<%=LinkUrl %>" <% End If %>/>     
                    <input name="in_IsLinks" id="in_IsLinks" type="checkbox" value="1" <%If IsLinks=1 Then %> checked="checked" <% End If %> /><label for="in_IsLinks">链接地址</label>                              
                </td>
            </tr>
            <tr>
                <td class="listTdLeft" valign="top">内容：</td>
                <td>                    
                    <textarea id="content1" name="content1" cols="100" rows="8" style="width:700px;height:350px;visibility:hidden;">
                    <%=Body %>
                    </textarea>                        
                </td>
            </tr>
        </table>
        <table width="100%" border="0" align="center" cellpadding="0" cellspacing="0">
            <tr>
                <td class="titleOpBg Lpd10">
                    <input type="button" value="返回" class="btn wd1" onclick="Redirect('NewsList.asp')" />                           
                    <input class="btnLine" type="button" />  
                    <input type="submit" value="保存" class="btn wd1" />  
                </td>
            </tr>
        </table>
    </form>
    <%
        rs.Close()
        Set rs=nothing
        End Sub
    %>
</body>
</html>
