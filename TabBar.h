#include "UIWidget.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"
#include "Button.h"


#define TABITEM_MAX_WIDTH  100  //tab�����
#define TAB_HEIGHT 20
enum TabStyle{
	none = 0,
	item_have_close = 1,//�Ӱ�ť�йر�
	add_but=2,          //����Ӱ�ť
};

#define addbut_width_hei 24
class TabBar :public UIWidget {
public:
	TabBar();
	~TabBar() {};
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	bool OnMouseDown(int x, int y) override;
	bool OnMouseUp(int x, int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override {};

	int AddTab(char *pName);
	void SetTabStyle(int nStyle); //������ʽ
	void SetTabWidget(int nTabId, UIWidget *pWidget);

	void CloseButCallback(UIWidget* pWidget, int nTag);
	void MouseDownCallback(UIWidget* pWidget);
	void AddButCallback(UIWidget* pWidget, MouseEvent ev);
	void LeftButCallback(UIWidget* pWidget, MouseEvent ev);
	void RightButCallback(UIWidget* pWidget, MouseEvent ev);
	void createLeftBut();
	void createRightBut();

private:
	struct tabInfo
	{
		//std::string  name;
		int          nTabId;
		ButtonHaveClose *but;
		UIWidget     *pWidget;
	};

	std::vector<tabInfo> tablist; 
	int nTabIdIndex;//tabId���ۼӵķ�ʽ

	SkPoint mouse_pt;

	int nTabStyle;//��ʽ
	int nSelTab;//��ǰѡ���tab
	SkFont font;
	ButtonImage *add_but;
	ButtonImage *left_but, *right_but;
	int nShowIndex;
	//void OnMouseRDown(int x, int y);
};