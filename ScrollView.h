#pragma once

#include "UIWidget.h"
//#include "GrContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"

#include "include/core/SkSurface.h"
#include <vector>
#include "UIRoot.h"
#include "ScrollBar.h"


struct ScrollContentInfo {
	int width;
    int height;
    int offs_y;
    int preoffs_y;
    int offs_x;
    int preoffs_x;
};

class ScrollView :public UIWidget,public ScrollBarController {
public:

	
	ScrollView();
	~ScrollView() {};
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	bool OnMouseDown(int x, int y) override;
	bool OnMouseUp(int x,int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override;

	SkPoint ScrollViewToChildPoint(int x, int y) 
	{
		SkPoint point;
		point.set(x - GetBound().left(), y - GetBound().top());
		return point;
	}

//	void AddChild(char *pImagePath);
	void AddChild(UIWidget *pWidget);

	/*void JumpTop();
	void JumpBottom();
	void JumpLeft();
	void JumpRight();*/

	void SetContentSize(SkScalar width, SkScalar height);
	void UpdateScrollBarInfo();

	//void SetDirection(Direction nType);
	
	void RemoveAllChildWidget();

	void InitOffset();

	void ScrollToPosition(ScrollBar* source, int position);

	SkScalar GetDisplayWidth();
	SkScalar GetDisplayHeigth() ;

	// Customize the scrollbar design. ScrollView takes the ownership of the
    // specified ScrollBar. |horiz_sb| and |vert_sb| cannot be NULL.
    void SetHorizontalScrollBar(ScrollBar* horiz_sb);
    void SetVerticalScrollBar(ScrollBar* vert_sb);

   // Returns the horizontal/vertical scrollbar. This may return NULL.
    ScrollBar* GetHorizontalScrollBar() const { return hori_bar; }
    ScrollBar* GetVerticalScrollBar() const { return vert_bar; }

private:
	std::vector<sk_sp<SkImage>> imagelist;
	/*SkScalar offs;
	SkScalar hei;*/
	//Direction nDirectionType;
	ScrollContentInfo ContentInfo;

	std::vector<UIWidget *> childlist;

	std::vector<UIWidget *> displaylist;

	ScrollBar *vert_bar;
	ScrollBar *hori_bar;

	double fDrawTime;
	long long llDrawTick;
};