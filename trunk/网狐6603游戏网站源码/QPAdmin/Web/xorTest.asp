<!--#include file="md5.asp"-->

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" >
<head>
    <title>Untitled Page</title>
</head>
<body>

<%


Dim srcData
Dim dstData

srcData="sa"

  Dim  o    
  Set o = Server.CreateObject("Game.CWHEncryptNet")   
  dstData=o.XorEncrypt(srcData)
  Response.Write dstData
  
  Response.Write "<br />"
  
  
  Response.Write o.XorCrevasse(dstData)
  
  Set o=Nothing 

%>

<% 
    Dim str
    str = "ÎÞÁÄµÄÈËenshiaskyaya"
    response.Write "<br>"&md5(str,32)
%>

</body>
</html>
