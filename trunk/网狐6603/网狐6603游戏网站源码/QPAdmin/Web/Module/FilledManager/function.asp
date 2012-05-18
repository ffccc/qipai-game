<% 
    '生成会员卡
    Sub LivcardCreate(cardTypeID,cardPrice,count,cardGold,memberDays,noteInfo,cardPrefix,cardLength,pwdType,pwdLength,useRange,salesPerson,validDate)
        Dim i,datePrefix,realPrefix,cardRndNum,serialNo,serialPwd      
          
        '获取实卡类别信息
        Call ConnectGame(QPTreasureDB)
        Dim rs,sql
        Dim memberOrder,userRight,serviceRight
        Set rs=Server.CreateObject("Adodb.RecordSet")
        sql = "select * from GlobalLivcard where CardTypeID="&cardTypeID
        rs.Open sql,GameConn,1,3
        If rs.eof Then
            Call CloseGame()
            Exit Sub
        Else
            memberOrder = rs("MemberOrder")
            userRight = rs("UserRight")
            serviceRight = rs("ServiceRight")
        End If  
        Set rs = Nothing        
        
        '卡号前缀处理
        datePrefix = Year(Date())&Month(Date())&Day(Date())
        If cardPrefix="" Then
            realPrefix = datePrefix
        Else
            realPrefix = cardPrefix&datePrefix
        End If
        cardRndNum = cardLength-Len(realPrefix)
        
        '卡数据源
        Dim cardDic,isHas,cardStrList
        isHas = true
        Set cardDic = CreateObject("Scripting.Dictionary")   
        For i=1 To count
            serialNo = realPrefix&GetRandomNum(cardRndNum)
            serialPwd = GetRandomType(pwdType,pwdLength)
            do while isHas = true            
                If cardDic.Exists(serialNo) = false Then
                    cardDic.Add serialNo,serialPwd
                    cardStrList = cardStrList&serialNo&","&serialPwd&"/"
                    isHas = false
                Else
                    serialNo = realPrefix&GetRandomNum(cardRndNum)
                End If
            loop
            isHas = true
        Next          
        
        '生成记录处理
        Dim buildID  
        Dim CmdSP,adoRS 
        Set CmdSP = Server.CreateObject("Adodb.Command")
        CmdSP.CommandType = 4
		CmdSP.ActiveConnection = GameConn
		CmdSP.CommandText="WSP_PM_BuildStreamAdd"
        CmdSP(1) = session("AdminName")
        CmdSP(2) = cardTypeID
        CmdSP(3) = cardPrice
        CmdSP(4) = cardGold
        CmdSP(5) = count
        CmdSP(6) = Request.ServerVariables("REMOTE_ADDR")
        CmdSP(7) = noteInfo
        CmdSP(8) = ASCIIToByteArray(cardStrList)
        Set adoRS = CmdSP.Execute
        buildID =  adoRS.Fields(0).value
        Set adoRS = nothing
        Set CmdSP = nothing       
        
        '实卡入库
        Dim cardList,pwdList,k,vaDate
        cardList = cardDic.Keys
        pwdList = cardDic.Items
        If validDate="" Then
            vaDate = DateAdd("d",36000,Now())
        Else
            vaDate = DateAdd("d",validDate,Now())
        End If
        For k=0 To cardDic.Count-1
            GameConn.execute("WSP_PM_LivcardAdd '"&cardList(k)&"','"&md5(pwdList(k),32)&"',"&buildID&","&cardTypeID&","&cardPrice&","&cardGold&","&memberOrder&","&memberDays&","&userRight&","&serviceRight&","&useRange&",'"&salesPerson&"','"&vaDate&"'")
        Next
        Call CloseGame()
    End Sub
    
    '生成指定长度指定类型的随机数
    Function GetRandomType(pwdType,pwdLength)
        Dim strInfo,i,rValue
        Select case pwdType
            case 1
                strInfo = "123456789"
            case 2
                strInfo = "abcdefghijklmnopqrstuvwxyz"
            case 4
                strInfo = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            case 3
                strInfo = "123456789abcdefghijklmnopqrstuvwxyz"
            case 5
                strInfo = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            case 6
                strInfo = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
            case 7
                strInfo = "123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
            case else
                strInfo = "123456789"
        End Select 
        for i=1 To pwdLength
            rValue = rValue&Mid(strInfo,GetRand(1,Len(strInfo)),1)
        Next
        GetRandomType = rValue
    End Function
    
    '生成指定长度的数字随机数
    Function GetRandomNum(cardLength)
        Dim i,rValue
        for i=1 To cardLength
            rValue = rValue&GetRand(1,9)
        Next
        GetRandomNum = rValue
    End Function
    
    '生成一个随机数
    Function GetRand(min,max)
        Dim rValue
        Randomize
        rValue = Int((max - min + 1) * Rnd + min) 
        GetRand = rValue
    End Function
    
    Rem 将二进制转换成字符
    Function BinVal(bin)
        dim ret
        ret = 0
        for i = lenb(bin) to 1 step -1
        ret = ret *256 + ascb(midb(bin,i,1))
        next
        BinVal=ret
    End Function

    Rem 将字符转换成2进制数组的函数
    Function ASCIIToByteArray(sText)
        Dim objRS
        Dim lTemp
        Dim sTemp
        sTemp = ""
        For lTemp = 1 to LenB(sText)
        sTemp = sTemp & Right("00" & Hex(AscB(MidB(sText,lTemp,1))),2)
        Next
        
        Set objRS=Server.CreateObject("Adodb.RecordSet")
        objRS.Fields.Append "Temp",204,LenB(sText)+1
        objRS.Open
        objRS.AddNew
        objRS("Temp") = sTemp ' ADODB will convert here
        objRS.Update
        objRS.MoveFirst
        ASCIIToByteArray = objRS("Temp") ' A variant byte array is returned
        objRS.Close
        set objRS = Nothing
    End Function
    
    Rem 获取快钱支付方式
    Function GetKQPayType(payType)
        Dim Dic,rValue
        Set Dic = CreateObject("Scripting.Dictionary") 
        Dic.Add "00","显示快钱各支付方式列表"
        Dic.Add "10","显示银行卡支付方式"
        Dic.Add "11","显示电话银行支付方式"
        Dic.Add "12","显示快钱账户支付方式"
        Dic.Add "13","显示线下支付方式"
        Dic.Add "14","显示B2B 支付"
        If Dic.Exists(payType) Then
            rValue = Dic(payType)
        Else
            rValue = "--"
        End If
        Set Dic = Nothing
        GetKQPayType = rValue
    End Function
    
    Rem 获取快钱银行信息
    Function GetKQBankID(bankID)
        Dim Dic,rValue
        Set Dic = CreateObject("Scripting.Dictionary") 
        Dic.Add "ICBC","中国工商银行"
        Dic.Add "CMB","招商银行"
        Dic.Add "CCB","中国建设银行"
        Dic.Add "ABC","中国农业银行"
        Dic.Add "BOC_SH","中国银行(上海)"
        Dic.Add "BOC_GZ","中国银行(广州)"
        Dic.Add "SPDB","上海浦东发展银行"
        Dic.Add "BCOM","交通银行"
        Dic.Add "CMBC","中国民生银行"
        Dic.Add "SDB","深圳发展银行"
        Dic.Add "GDB","广东发展银行"
        Dic.Add "CITIC","中信银行"
        Dic.Add "HXB","华夏银行"
        Dic.Add "CIB","兴业银行"
        Dic.Add "GZRCC","广州市农村信用合作社"
        Dic.Add "GZCB","广州市商业银行"
        Dic.Add "SHRCC","上海农村商业银行"
        Dic.Add "CPSRB","中国邮政储蓄"
        Dic.Add "CEB","中国光大银行"
        If Dic.Exists(bankID) Then
            rValue = Dic(bankID)
        Else
            rValue = "--"
        End If
        Set Dic = Nothing
        GetKQBankID = rValue
    End Function
        
    Rem 获取快钱支付错误信息
    Function GetKQErrCode(errCode)
        Dim Dic,rValue
        Set Dic = CreateObject("Scripting.Dictionary") 
        Dic.Add "00000","未知错误"
        Dic.Add "10001","不支持的字符编码格式,系统支持的字符编码格式为1.UTF-8,2.GBK,3.GB2312"
        Dic.Add "10002","不支持的返回类型,系统支持的返回类型为1.页面返回,2.后台返回,3.同时支持页面和后台返回"
        Dic.Add "10003","页面返回地址和后台返回地址不能同时为空,请使用符合URL 规则的http 或者https 地址"
        Dic.Add "10004","页面返回地址和后台返回地址不能同时为空,请使用符合URL 规则的http 或者https 地址"
        Dic.Add "10005","不支持的网关接口版本号,目前系统支持的版本号为v2.0"
        Dic.Add "10006","商户号不存在"
        Dic.Add "10007","付款方用户名不正确"
        Dic.Add "10008","不支持的付款方联系方式,系统支持的联系方式为1.电子邮件,2.电话.当联系内容不为空时联系方式不能为空"
        Dic.Add "10009","付款方的联系内容不正确,请输入合法的联系地址"
        Dic.Add "10010","订单号不正确,系统只支持以字母,数字组合的订单号,最大长度不能超过30"
        Dic.Add "10011","订单金额不正确,请输入以分为单位的金额"
        Dic.Add "10012","订单提交时间不正确,请输入以yyyyMMddhhmmss 格式的时间字符串"
        Dic.Add "10013","商品名称不正确"
        Dic.Add "10014","商品数量不正确"
        Dic.Add "10015","商品ID 不正确"
        Dic.Add "10016","商品的描述不正确"
        Dic.Add "10017","扩展参数一不正确"
        Dic.Add "10018","扩展参数二不正确"
        Dic.Add "10019","指定的支付方式不正确"
        Dic.Add "10020","指定的支付服务代码不正确"
        Dic.Add "10021","指定的银行ID 不正确"
        Dic.Add "10022","不支持的语言类型,系统支持的语言为1.中文,2.英文"
        Dic.Add "10023","不支持的签名类型,系统支持的签名类型为1.MD5"
        Dic.Add "10024","商户未开通人民币网关"
        Dic.Add "10025","商户未开通国际卡人民币网关"
        Dic.Add "10026","商户未开通电话支付人民币网关"
        Dic.Add "10027","不正确的pid 值"
        Dic.Add "10028","不正确的国际卡支付参数,组合支付方式和支付方式必须为国际卡对应的参数"
        Dic.Add "10029","不正确的神州行支付参数,组合支付方式和支付方式必须为神州行支付对应的参数"
        Dic.Add "10030","不正确的代理商帐户代码"
        Dic.Add "10031","商户未开通代理网关"
        Dic.Add "10032","原始交易不存在"
        Dic.Add "10033","订单金额不正确,请输入以元为单位的金额,最多允许两位小数"
        Dic.Add "10034","手续费金额不正确,请输入以元为单位的金额,最多允许两位小数"
        Dic.Add "10035","手续费总额大于或等于订单金额"
        Dic.Add "10036","同一订单号禁止重复提交标志不正确"
        Dic.Add "10037","对不起，该订单不允许重复提交，请重新下订单提交"
        Dic.Add "20001","订单信息的签名内容不正确"
        Dic.Add "20002","商户账号已被冻结"
        Dic.Add "20003","商户交易金额已超过限制"
        Dic.Add "20004","商户制定的银行直连参数不正确"
        Dic.Add "20005","不能使用优惠券"
        Dic.Add "20006","商户账户不允许收款"
        Dic.Add "20007","账户已注销"
        Dic.Add "20008","订单金额小于支付手续费,不能支付"
        Dic.Add "20009","商户不允许银行直连,可能商户没有对网关定制或者定制中没有选择银行直连,请联系快钱客服"
        Dic.Add "20010","您通过快钱向此商户的支付金额超过支付限额,请联系快钱客服"
        Dic.Add "20011","您通过快钱向此商户的支付金额超过单笔订单的支付限额,请联系快钱客服"
        Dic.Add "20012","您通过快钱向此商户的支付金额超过单日的支付总限额,请联系快钱客服"
        Dic.Add "20013","您通过快钱向此商户的支付金额超过单月的支付总限额,请联系快钱客服"
        Dic.Add "30001","银行不可用"
        Dic.Add "30002","线下支付不可用"
        Dic.Add "30003","电话银行不可用"
        Dic.Add "30004","订单已支付成功,请勿重新支付"
        Dic.Add "30005","请填写您的电子邮箱"
        Dic.Add "50001","验证码不正确"
        Dic.Add "50002","用户不存在"
        Dic.Add "50003","用户被冻结"
        Dic.Add "50004","登录次数过多,用户已被锁定"
        Dic.Add "50005","付款人不能和收款人相同"
        Dic.Add "50006","你的操作已超时，请重新提交"
        Dic.Add "50007","账户不正确"
        Dic.Add "50008","密码不正确"
        Dic.Add "50009","余额不足"
        Dic.Add "50010","没有付款权限"
        Dic.Add "50011","不能向个人会员付款"
        Dic.Add "50012","复核错误"
        Dic.Add "50013","您的快钱盾已挂失,无法登录.您可以通过取消挂失或更换后再登录"
        Dic.Add "50014","您输入的快钱盾数据不正确"
        Dic.Add "50015","使用快钱盾遇到未知错误，请联系快钱客服"
        Dic.Add "50016","请使用您的默认用户名登录"
        Dic.Add "50017","复核重试次数超过3 次,您不能重新复核"
        Dic.Add "60001","货币种类不正确! 提示 1：人民币网关；3：预付费卡网关"
        Dic.Add "60002","定单号不能为空,不能超过50 位长度"
        Dic.Add "60003","定单号只能包含数字或字母以及中划线和下划线"
        Dic.Add "60004","该笔交易金额格式不正确,必须为整数或者小数"
        Dic.Add "60005","该笔交易金额格式不正确,必须为整数"
        Dic.Add "70001","您输入的用户不能使用此优惠券"
        Dic.Add "70002","您尝试使用优惠错误次数超过3 次,请与快钱客服联系"
        Dic.Add "70003","您验证优惠券的用户错误次数超过3 次,请与快钱客服联系"
        If Dic.Exists(errCode) Then
            rValue = Dic(errCode)
        Else
            rValue = "--"
        End If
        Set Dic = Nothing
        GetKQErrCode = rValue
    End Function
%>
