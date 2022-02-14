#pragma once
#include "UIWidget.h"
//#include "GrContext.h"
#include "include/core/SkCanvas.h"
#include "ScrollView.h"
#include "ListView.h"
class ListHead:public UIWidget{
public:
	ListHead(ListView *view);
	~ListHead();
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;
	void OnMouseUp(int x,int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override {};
	int GetMousePosCol(int x, int y);
	SkScalar GetListHeadHeight()
	{
		return GetBound().height();
	}
private:
	ListView *listview;
	int nMouseDownCol;
};