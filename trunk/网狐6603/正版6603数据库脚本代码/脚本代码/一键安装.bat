@echo off

TITLE �������ݿ⡾Ver6.6_Spreader�� �����ű�������... [�ڼ�����ر�]

md D:\���ݿ�

Rem �������ݿ�����
set rootPath=���ݿ�ű�\
osql -E -i "%rootPath%���ݿ�ɾ��.sql"
osql -E -i "%rootPath%1_1_�û���ű�.sql"
osql -E -i "%rootPath%1_2_ƽ̨��ű�.sql"
osql -E -i "%rootPath%1_3_��ҿ�ű�.sql"
osql -E -i "%rootPath%1_4_��¼��ű�.sql"
osql -E -i "%rootPath%1_5_���ֿ�ű�.sql"

osql -E -i "%rootPath%2_1_�û���ű�.sql"
osql -E -i "%rootPath%2_2_ƽ̨��ű�.sql"
osql -E -i "%rootPath%2_3_��ҿ�ű�.sql"
osql -E -i "%rootPath%2_4_��¼��ű�.sql"
osql -E -i "%rootPath%2_5_���ֿ�ű�.sql"


Rem �����ӷ�����������
set rootPath=���ݽű�\
osql -E -i "%rootPath%��Ϸ�б�.sql"
osql -E -i "%rootPath%�Ƹ�����.sql"
osql -E -i "%rootPath%ƽ̨����.sql"
osql -E -i "%rootPath%�û�����.sql"
osql -E -i "%rootPath%��¼����.sql"

Rem �洢����
set rootPath=�洢����\�û����ݿ�\
osql -E  -i "%rootPath%�󶨻���.sql"
osql -E  -i "%rootPath%��ʶ��¼.sql"
osql -E  -i "%rootPath%�޸�����.sql"
osql -E  -i "%rootPath%�޸�ǩ��.sql"
osql -E  -i "%rootPath%�û�����.sql"
osql -E  -i "%rootPath%�ʺŵ�¼.sql"
osql -E  -i "%rootPath%ע���ʺ�.sql"
osql -E  -i "%rootPath%�Զ�ͷ��.sql"

set rootPath=�洢����\ƽ̨���ݿ�\
osql -E  -i "%rootPath%�������.sql"
osql -E  -i "%rootPath%���ؽڵ�.sql"
osql -E  -i "%rootPath%��������.sql"
osql -E  -i "%rootPath%����ҳ��.sql"
osql -E  -i "%rootPath%��������.sql"
osql -E  -i "%rootPath%������Ϣ.sql"
osql -E  -i "%rootPath%ģ�����.sql"
osql -E  -i "%rootPath%������Ϣ.sql"


set rootPath=�洢����\������ݿ�\
osql -E  -i "%rootPath%��ʶ��¼.sql"
osql -E  -i "%rootPath%���ص���.sql"
osql -E  -i "%rootPath%�뿪����.sql"
osql -E  -i "%rootPath%�б�����.sql"
osql -E  -i "%rootPath%���з���.sql"
osql -E  -i "%rootPath%��Ϸд��.sql"

set rootPath=�洢����\�������ݿ�\
osql -E  -i "%rootPath%��ʶ��¼.sql"
osql -E  -i "%rootPath%���ص���.sql"
osql -E  -i "%rootPath%�뿪����.sql"
osql -E  -i "%rootPath%�б�����.sql"
osql -E  -i "%rootPath%��Ϸд��.sql"


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


