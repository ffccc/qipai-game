@echo off

TITLE �������ݿ⡾Ver6.6_Spreader�� �����ű�������... [�ڼ�����ر�]


md D:\���ݿ�

echo	sdasfsd

pause



Rem �������ݿ�����
set rootPath=���ݿ�ű�\
osql -E -i "%rootPath%���ݿ�ɾ��.sql"
osql -E -i "%rootPath%1_1_�û���ű�.sql"
osql -E -i "%rootPath%1_2_ƽ̨��ű�.sql"
osql -E -i "%rootPath%1_3_�Ƹ���ű�.sql"
osql -E -i "%rootPath%1_4_���ֿ�ű�.sql"
osql -E -i "%rootPath%1_5_��ϰ��ű�.sql"
osql -E -i "%rootPath%1_6_������ű�.sql"

echo 1.1
pause
osql -E -i "%rootPath%2_1_�û���ű�.sql"
osql -E -i "%rootPath%2_2_ƽ̨��ű�.sql"
osql -E -i "%rootPath%2_3_�Ƹ���ű�.sql"
osql -E -i "%rootPath%2_4_���ֱ�ű�.sql"
osql -E -i "%rootPath%2_5_��ϰ��ű�.sql"
osql -E -i "%rootPath%2_6_������ű�.sql"

echo 1.2
pause

Rem �����ӷ�����������
set rootPath=���ݽű�\
osql -E -i "%rootPath%������Ϣ.sql"
osql -E -i "%rootPath%��Ϸ�б�.sql"
osql -E -i "%rootPath%�Ƹ�����.sql"
osql -E -i "%rootPath%ƽ̨����.sql"
osql -E -i "%rootPath%�û�����.sql"
osql -E -i "%rootPath%�ʻ�����.sql"

echo 2.1
pause

Rem �洢����
set rootPath=�洢����\�û���\
osql -E  -i "%rootPath%��ʶ��¼.sql"
osql -E  -i "%rootPath%�ʺŵ�¼.sql"
osql -E  -i "%rootPath%ע���ʺ�.sql"
osql -E  -i "%rootPath%�û�Ȩ��.sql"
osql -E  -i "%rootPath%�����ʺ�.sql"
osql -E  -i "%rootPath%�Զ���ͷ��.sql"
echo 4444
pause
set rootPath=�洢����\ƽ̨��\
osql -E  -i "%rootPath%��������.sql"
osql -E  -i "%rootPath%��������.sql"
osql -E  -i "%rootPath%������Ϣ.sql"
osql -E  -i "%rootPath%���ؽڵ�.sql"
echo 2.2
pause

set rootPath=�洢����\�Ƹ���\
osql -E  -i "%rootPath%��ʶ��¼.sql"
osql -E  -i "%rootPath%���ػ�����.sql"
osql -E  -i "%rootPath%�뿪����.sql"
osql -E  -i "%rootPath%��Ϸд��.sql"
osql -E  -i "%rootPath%�û�Ȩ��.sql"
osql -E  -i "%rootPath%�������.sql"
osql -E  -i "%rootPath%�����һ�.sql"
osql -E  -i "%rootPath%�ʻ�����.sql"
osql -E  -i "%rootPath%���м�¼.sql"

echo 3.1
pause
set rootPath=�洢����\���ֿ�\
osql -E  -i "%rootPath%��ʶ��¼.sql"
osql -E  -i "%rootPath%���ػ�����.sql"
osql -E  -i "%rootPath%�뿪����.sql"
osql -E  -i "%rootPath%��Ϸд��.sql"
osql -E  -i "%rootPath%�û�Ȩ��.sql"
osql -E  -i "%rootPath%�������.sql"
osql -E  -i "%rootPath%�����һ�.sql"
osql -E  -i "%rootPath%�ʻ�����.sql"
osql -E  -i "%rootPath%���м�¼.sql"
echo 3.2
pause
set rootPath=�洢����\��ϰ��\
osql -E  -i "%rootPath%��ʶ��¼.sql"
osql -E  -i "%rootPath%���ػ�����.sql"
osql -E  -i "%rootPath%�뿪����.sql"
osql -E  -i "%rootPath%��Ϸд��.sql"
osql -E  -i "%rootPath%�û�Ȩ��.sql"
osql -E  -i "%rootPath%�������.sql"
osql -E  -i "%rootPath%�����һ�.sql"
osql -E  -i "%rootPath%�ʻ�����.sql"
osql -E  -i "%rootPath%���м�¼.sql"
echo 3.3
pause

set rootPath=�洢����\������\
osql -E  -i "%rootPath%��ʶ��¼.sql"
osql -E  -i "%rootPath%���ػ�����.sql"
osql -E  -i "%rootPath%�뿪����.sql"
osql -E  -i "%rootPath%��Ϸд��.sql"
osql -E  -i "%rootPath%�û�Ȩ��.sql"
osql -E  -i "%rootPath%�������.sql"
osql -E  -i "%rootPath%�����һ�.sql"
osql -E  -i "%rootPath%�ʻ�����.sql"
osql -E  -i "%rootPath%���м�¼.sql"
echo 3.4
pause

CLS
@echo off
@echo ����������ϷID ����
set rootPath=���ݽű�\
osql -E  -i "%rootPath%��ʶ����.sql"

COLOR 0A
CLS
@echo off
cls
echo ------------------------------
echo.
echo	��Ҫ���ݿ⽨����ɣ�������Լ�ƽ̨�Ļ�����Ϸִ�� 
echo.
echo.
echo	��Ȩ���У� �����������Ƽ����޹�˾
echo ------------------------------

pause


