<%
Class CommonObj
    Private m_lOnBits(30)
    Private m_l2Power(30)

    Private Sub Class_Initialize  
        m_lOnBits(0) = CLng(1)
        m_lOnBits(1) = CLng(3)
        m_lOnBits(2) = CLng(7)
        m_lOnBits(3) = CLng(15)
        m_lOnBits(4) = CLng(31)
        m_lOnBits(5) = CLng(63)
        m_lOnBits(6) = CLng(127)
        m_lOnBits(7) = CLng(255)
        m_lOnBits(8) = CLng(511)
        m_lOnBits(9) = CLng(1023)
        m_lOnBits(10) = CLng(2047)
        m_lOnBits(11) = CLng(4095)
        m_lOnBits(12) = CLng(8191)
        m_lOnBits(13) = CLng(16383)
        m_lOnBits(14) = CLng(32767)
        m_lOnBits(15) = CLng(65535)
        m_lOnBits(16) = CLng(131071)
        m_lOnBits(17) = CLng(262143)
        m_lOnBits(18) = CLng(524287)
        m_lOnBits(19) = CLng(1048575)
        m_lOnBits(20) = CLng(2097151)
        m_lOnBits(21) = CLng(4194303)
        m_lOnBits(22) = CLng(8388607)
        m_lOnBits(23) = CLng(16777215)
        m_lOnBits(24) = CLng(33554431)
        m_lOnBits(25) = CLng(67108863)
        m_lOnBits(26) = CLng(134217727)
        m_lOnBits(27) = CLng(268435455)
        m_lOnBits(28) = CLng(536870911)
        m_lOnBits(29) = CLng(1073741823)
        m_lOnBits(30) = CLng(2147483647)
        
        m_l2Power(0) = CLng(1)
        m_l2Power(1) = CLng(2)
        m_l2Power(2) = CLng(4)
        m_l2Power(3) = CLng(8)
        m_l2Power(4) = CLng(16)
        m_l2Power(5) = CLng(32)
        m_l2Power(6) = CLng(64)
        m_l2Power(7) = CLng(128)
        m_l2Power(8) = CLng(256)
        m_l2Power(9) = CLng(512)
        m_l2Power(10) = CLng(1024)
        m_l2Power(11) = CLng(2048)
        m_l2Power(12) = CLng(4096)
        m_l2Power(13) = CLng(8192)
        m_l2Power(14) = CLng(16384)
        m_l2Power(15) = CLng(32768)
        m_l2Power(16) = CLng(65536)
        m_l2Power(17) = CLng(131072)
        m_l2Power(18) = CLng(262144)
        m_l2Power(19) = CLng(524288)
        m_l2Power(20) = CLng(1048576)
        m_l2Power(21) = CLng(2097152)
        m_l2Power(22) = CLng(4194304)
        m_l2Power(23) = CLng(8388608)
        m_l2Power(24) = CLng(16777216)
        m_l2Power(25) = CLng(33554432)
        m_l2Power(26) = CLng(67108864)
        m_l2Power(27) = CLng(134217728)
        m_l2Power(28) = CLng(268435456)
        m_l2Power(29) = CLng(536870912)
        m_l2Power(30) = CLng(1073741824)  
    End Sub 

    Private Sub Class_Terminate()
	End Sub 
	
	Rem 左移
	 Function LShift(lValue, iShiftBits)
        If iShiftBits = 0 Then
            LShift = lValue
            Exit Function
        ElseIf iShiftBits = 31 Then
            If lValue And 1 Then
                LShift = &H80000000
            Else
                LShift = 0
            End If
            Exit Function
        ElseIf iShiftBits < 0 Or iShiftBits > 31 Then
            Err.Raise 6
        End If

        If (lValue And m_l2Power(31 - iShiftBits)) Then
            LShift = ((lValue And m_lOnBits(31 - (iShiftBits + 1))) * m_l2Power(iShiftBits)) Or &H80000000
        Else
            LShift = ((lValue And m_lOnBits(31 - iShiftBits)) * m_l2Power(iShiftBits))
        End If
    End Function

    Rem 右移
    Private Function RShift(lValue, iShiftBits)
        If iShiftBits = 0 Then
            RShift = lValue
            Exit Function
        ElseIf iShiftBits = 31 Then
            If lValue And &H80000000 Then
                RShift = 1
            Else
                RShift = 0
            End If
            Exit Function
        ElseIf iShiftBits < 0 Or iShiftBits > 31 Then
            Err.Raise 6
        End If
        
        RShift = (lValue And &H7FFFFFFE) \ m_l2Power(iShiftBits)

        If (lValue And &H80000000) Then
            RShift = (RShift Or (&H40000000 \ m_l2Power(iShiftBits - 1)))
        End If	
    End Function
	
	Rem 把给定的日期格式化为距现在的模糊时间段，比如 1 分钟前
	Function FormatDateSpan(dateSpan)
	    Dim days,hours,minute
	    days = DateDiff("d",dateSpan,Now())
	    hours = DateDiff("h",dateSpan,Now())
	    minute = DateDiff("n",dateSpan,Now())
	    If days>=365 Then
	        days = days\365&" 年前"
	    Elseif days>=30 Then
	        days = days\30&" 月前"
	    Elseif days>7 and days\7<=4 Then
	        days = days\7&" 周前"
	    Elseif days>=1 Then
	        days = days&" 天前"
	    Elseif days<1 and hours>=1 Then
	        days = hours&" 小时前"
	    Elseif hours<1 and minute>=1 Then
	        days = minute&" 分钟前"
	    Else
	        days = "1 分钟前"
	    End If
	    FormatDateSpan = days
	End Function
	
	Rem 计算版本号
	Function CalVersion(version)
	    Dim rValue
	    rValue = RShift(version,24)&"."
	    rValue = rValue&RShift(LShift(RShift(version,16),24),24)&"."
	    rValue = rValue&RShift(LShift(RShift(version,8),24),24)&"."
	    rValue = rValue&RShift(LShift(version,24),24)
	    CalVersion = rValue
	End Function
	
	Rem 还原版本号
	Function CalVersion2(version)
	    Dim rValueArr,rValue
	    rValueArr = Split(version,".")
	    rValue = LShift(rValueArr(0),24) Or LShift(rValueArr(1),16) Or LShift(rValueArr(2),8) Or rValueArr(3)
	    CalVersion2 = rValue
    End Function
    
    Rem 三目运算
    Function IIf(Condition, ValueIfTrue, ValueIfFalse)   
        If Condition Then   
            IIf = ValueIfTrue   
        Else   
            IIf = ValueIfFalse   
        End if  
    End Function 
    
    Rem 获取每个月的实际天数
    Function GetDayCountOfMonth(yearStr,monthStr)
        Dim str,array,n,newMonth
        str = "31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31"
        array = Split(str,",")
        If Left(monthStr,1) = "0" Then
            newMonth = Mid(monthStr,2,1)
        Else
            newMonth = monthStr
        End If
        n = array(monthStr-1)
        If newMonth=2 And  (yearStr Mod 4=0 And (yearStr Mod 100<>0 Or yearStr Mod 400 =0)) Then
            n =n+1
        End If
        GetDayCountOfMonth = n
    End Function

End Class
   
Rem 建立类对象
Dim Common
Set Common=New CommonObj
%>