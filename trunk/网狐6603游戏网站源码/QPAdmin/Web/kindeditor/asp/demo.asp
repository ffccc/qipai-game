﻿<%
Option Explicit

Dim htmlData

htmlData = Request.Form("content1")

Function htmlspecialchars(str)
	str = Replace(str, "&", "&amp;")
	str = Replace(str, "<", "&lt;")
	str = Replace(str, ">", "&gt;")
	str = Replace(str, """", "&quot;")
	htmlspecialchars = str
End Function
%>
<!doctype html>
<html>
<head>
	<meta charset="utf-8" />
	<title>KindEditor ASP</title>
	<script type="text/javascript" charset="utf-8" src="../kindeditor.js"></script>
	<script type="text/javascript">
		KE.show({
			id : 'content1',
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
	</script>
</head>
<body>
	<%=htmlData%>
	<form name="example" method="post" action="demo.asp">
		<textarea id="content1" name="content1" cols="100" rows="8" style="width:700px;height:200px;visibility:hidden;"><%=htmlspecialchars(htmlData)%></textarea>
		<br />
		<input type="submit" name="button" value="提交内容" /> (提交快捷键: Ctrl + Enter)
	</form>
</body>
</html>
