#ifndef D3D_FOUNT_HEAD_FILE
#define D3D_FOUNT_HEAD_FILE

#include "D3DDevice.h"
#include "D3DSprite.h"
#include "GameEngineHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��������
struct tagD3DFontItem
{
	//�ַ���Ϣ
	UINT							uChar;								//�������
	CD3DSprite						D3DSprite;							//���徫��

	//���λ��
	CSize							SizeCell;							//��Ԫ��С
	CSize							SizeFont;							//�����С
	CPoint							PointOrigin;						//ԭ��λ��
};

//���Ͷ���
typedef CWHArray<tagD3DFontItem *> CD3DFontItemPtrArray;
typedef CMap<UINT,UINT,tagD3DFontItem *,tagD3DFontItem *> CD3DFontItemPtrMap;

//////////////////////////////////////////////////////////////////////////////////

//�������
class GAME_ENGINE_CLASS CD3DFont
{
	//���ñ���
protected:
	UINT							m_uGrayLevel;						//�Ҷȵȼ�
	UINT							m_uMaxBufferCount;					//������Ŀ

	//�������
protected:
	CFont							m_Font;								//�������
	TEXTMETRIC						m_TextMetric;						//��������

	//�ں˶���
protected:
	CD3DFontItemPtrMap				m_FontItemPtrMap;					//��������
	CD3DFontItemPtrArray			m_FontItemPtrActive;				//��������

	//��������
public:
	//���캯��
	CD3DFont();
	//��������
	virtual ~CD3DFont();

	//������Ŀ
public:
	//������Ŀ
	UINT GetMaxBufferCount() { return m_uMaxBufferCount; }
	//������Ŀ
	VOID SetMaxBufferCount(UINT uMaxBufferCount) { m_uMaxBufferCount=uMaxBufferCount; }

	//��������
public:
	//ɾ������
	bool DeleteFont();
	//��������
	bool CreateFont(LOGFONT & LogFont, UINT uGrayLevel);
	//��������
	bool CreateFont(INT nPointSize, LPCTSTR lpszFaceName, UINT uGrayLevel);

	//�������
public:
	//�������
	bool DrawText(CD3DDevice * pD3DDevice, LPCTSTR pszString, CRect rcDraw, UINT uFormat, D3DCOLOR D3DColor);
	//�������
	bool DrawText(CD3DDevice * pD3DDevice, LPCTSTR pszString, INT nXPos, INT nYPos, UINT uFormat, D3DCOLOR D3DColor);

	//�������
private:
	//��������
	tagD3DFontItem * ActiveFontItem(CD3DDevice * pD3DDevice, UINT uChar);
	//����λ��
	CSize CalePostion(CD3DDevice * pD3DDevice, LPCTSTR pszString, UINT uFormat, INT nAreaWidth);
};

//////////////////////////////////////////////////////////////////////////////////

#endif