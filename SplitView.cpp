#include "SplitView.h"



SplitView::SplitView()
{
	SetBackGroundColor(SkColorSetRGB(52, 72, 105));
	bMouseDragged = false;
	
}
void SplitView::Draw(SkCanvas* canvas)
{
	SkPaint paint;
	paint.setColor(GetBackGroundColor());
	canvas->drawRect(GetBound(), paint);
	nDirectionType = Direction::Horizontal;
	if (GetBound().height() > GetBound().width())
		nDirectionType = Direction::Vertical;
	if (GetMouseDragged())
	{
		SkPaint drag_paint;
		drag_paint.setColor(SkColorSetRGB(212,212,212));
		if (nDirectionType == Direction::Vertical)
		{
			canvas->drawRect(SkRect{ (SkScalar)mouse_offset,GetBound().top(), mouse_offset + GetBound().width(), GetBound().bottom() }, drag_paint);
		}
	}
}


void SplitView::OnMouseMove(int x, int y)
{
	if (IsVisible() == false)
		return ;

	if (x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom() || GetMouseDragged())
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
	}
	
	if (GetMouseDragged())
	{
		if (GetBound().height() > GetBound().width())
			nDirectionType = Direction::Vertical;
		int nMousePos = y;
		if (nDirectionType == Direction::Vertical)
			nMousePos = x;
		mouse_offset = nMousePos;
	}
	
}

bool SplitView::OnMouseDown(int x, int y)
{
	if (IsVisible() == false)
		return false;
	if (GetBound().height() > GetBound().width())
		nDirectionType = Direction::Vertical;
	int nMousePos = y;
	if (nDirectionType == Direction::Vertical)
		nMousePos = x;
	if (GetMouseDragged() == false)
	{
		mouse_offset = nMousePos ;
		SetCursor(LoadCursor(NULL, IDC_SIZEWE));
		
		//printf("mouse_offset=%d\n",x);
		SetMouseDragged(true);
	}
	return false;
}


bool SplitView::OnMouseUp(int x, int y)
{
	if (IsVisible() == false)
		return false;
	if (GetMouseDragged())
	{
		SetCursor(LoadCursor(NULL, IDC_CROSS));
		funcc(this, SkPoint{ (SkScalar)x, (SkScalar)y });
	}
	SetMouseDragged(false);
	
	return false;
}


