#include "UIWidget.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"

//typedef std::function<void(void)> MenuItemCallBack;
typedef std::function<void(UIWidget *p)> MenuItemCallBack;
class PopupMenu:public UIWidget {
public:
	PopupMenu();
	~PopupMenu() {};
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	bool OnMouseDown(int x, int y) override;
	bool OnMouseUp(int x, int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override {};

	void OnMouseRDown(int x, int y);
	void AddMenuItem(char *pName,MenuItemCallBack callback);
	void AppendSeparator();//��ӷָ���
	

	
private:

	enum ItemStyle  //�˵�����ʽ
	{
		text,            //����
		separator,       //�ָ���
		bitmap_text      //ͼƬ������

	};
	enum {
		item_NormalStatu,
		item_PressedStatu,
		item_MouseStayStatu,
		item_DisabledStatu,
	};
	struct menuitem
	{
		std::string name;
		MenuItemCallBack func; //�˵�������Ļص�
		ItemStyle sty;
		bool bEnable;          //�Ƿ����
		int  nStatu;           //״̬

	};
	std::vector<menuitem> item;
	int mouse_x;
	int mouse_y;
	SkFont font;
	SkPaint font_paint;

	int text_height;
	int sep_height;
public:
	menuitem *GetMenuItem(int x, int y);//ͨ�������ȡ��ǰ��ITEM

};