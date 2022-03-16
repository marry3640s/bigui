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
	void AppendSeparator();//添加分割条
	

	
private:

	enum ItemStyle  //菜单项样式
	{
		text,            //文字
		separator,       //分割条
		bitmap_text      //图片和文字

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
		MenuItemCallBack func; //菜单项点击后的回调
		ItemStyle sty;
		bool bEnable;          //是否可用
		int  nStatu;           //状态

	};
	std::vector<menuitem> item;
	int mouse_x;
	int mouse_y;
	SkFont font;
	SkPaint font_paint;

	int text_height;
	int sep_height;
public:
	menuitem *GetMenuItem(int x, int y);//通过坐标获取当前的ITEM

};