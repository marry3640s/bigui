
#include "ScrollBar.h"

ScrollBar::ScrollBar(Direction dir)
{
	nDirectionType=dir;
	thumbst=ThumbStatu::NormalStatu;
	SetMouseDragged(false);
	SetPosition(0, 0);
	SetSize(0,0);
}

ScrollBar::~ScrollBar()
{

}
void ScrollBar::SetScrollBarInfo(ScrollBarInfo info)
{
	memcpy(&barinfo,&info,sizeof(ScrollBarInfo));
}


SkScalar ScrollBar::GetThumbSize()
{
	return std::max((float)10,barinfo.DisplaySize*barinfo.DisplaySize/barinfo.ContentSize);
}

SkScalar ScrollBar::GetThumbPosition()
{
	SkScalar ContentMoveMax=barinfo.ContentSize-barinfo.DisplaySize;
	SkScalar ThumbMoveMax=(barinfo.DisplaySize-GetThumbSize());
	SkScalar ContentOffset=(-barinfo.offset);
	return  ContentOffset*ThumbMoveMax/ContentMoveMax;
}

void ScrollBar::Draw(SkCanvas* canvas)
{
	if(GetWidth()==0 || GetHeight()==0 || !IsVisible())
		return;
	SkPaint paint;
	paint.setColor(SkColorSetRGB(241,241,241));
	canvas->drawRect(GetBound(),paint);

	paint.setColor(SkColorSetRGB(193,193,193));

	if(thumbst==ThumbStatu::MouseStayStatu)
		paint.setColor(SkColorSetRGB(153,153,153));
	else if(thumbst==ThumbStatu::MousePressedStatu)
		paint.setColor(SkColorSetRGB(123,123,123));
	SkScalar ThumbPos=GetThumbPosition();


	//printf("thumbpos=%f,barinfo.offset=%f\n",ThumbPos,barinfo.offset);
	if(nDirectionType==Direction::Vertical)
        thumbrect.setLTRB(GetBound().left(),
                      GetBound().top() + ThumbPos,
                      GetWidth() + GetBound().left(),
                      GetThumbSize() + GetBound().top() + ThumbPos);
	else if(nDirectionType==Direction::Horizontal)
        thumbrect.setLTRB(GetBound().left() + ThumbPos,
                          GetBound().top(),
                          GetBound().left() + ThumbPos + GetThumbSize(),
                          GetBound().top() + GetBound().height());
	canvas->drawRect(thumbrect,paint);
	
}

void ScrollBar::OnMouseMove(int x, int y)
{
	if (IsVisible() == false)
		return;
	if (x >= thumbrect.left() && x <= thumbrect.right() && y >= thumbrect.top() && y <= thumbrect.bottom())
	{
		thumbst=ThumbStatu::MouseStayStatu;
	}
	else
	{
		thumbst = ThumbStatu::NormalStatu;
	}

	if (GetMouseDragged())
	{
		if (nDirectionType == Direction::Vertical)
		{
			if (x >= thumbrect.left() - 80 && x <= thumbrect.right() + 80 && y >= thumbrect.top()-80 && y <= thumbrect.bottom()+80)
				thumbst = ThumbStatu::MousePressedStatu;
			else
				return;
		}
		else if (nDirectionType == Direction::Horizontal)
		{
			if (x >= thumbrect.left()-50&& x <= thumbrect.right()+50  && y >= thumbrect.top()-50 && y <= thumbrect.bottom()+50)
				thumbst = ThumbStatu::MousePressedStatu;
			else
				return;
		}
	}

	if (GetMouseDragged())
	{
		int nMousePos=x;
	    if(nDirectionType==Direction::Vertical)
		    nMousePos=y;
		thumbst=ThumbStatu::MousePressedStatu;
		int thumb_move =  mouse_offset-nMousePos;
		//printf("thumb_y2=%d\n",nMousePos);
	    GetScrollBarController()->ScrollToPosition(this,thumb_move*barinfo.ContentSize/barinfo.DisplaySize);

		//barinfo.offset=thumb_move*barinfo.ContentSize/barinfo.DisplaySize;
	
	}
}

void ScrollBar::OnMouseDown(int x, int y)
{
	if (IsVisible() == false)
		return;
	int nMousePos=x;
	if(nDirectionType==Direction::Vertical)
		nMousePos=y;
	if (GetMouseDragged()==false)
	{
		mouse_offset=nMousePos+(-GetThumbPosition());
		//printf("mouse_offset=%d\n",y);
		SetMouseDragged(true);
	}
	
}

void ScrollBar::OnMouseUp(int x, int y)
{
	if (IsVisible() == false)
		return;
	//printf("mouse up=%d\n",y);

	if (GetMouseDragged())
	{
		/*int nMousePos=x;
	    if(nDirectionType==Direction::Vertical)
		    nMousePos=y;
		thumbst=ThumbStatu::MousePressedStatu;
		int thumb_move =  mouse_offset-nMousePos;
	    GetScrollBarController()->ScrollToPosition(this,thumb_move*barinfo.ContentSize/barinfo.DisplaySize);*/

		printf("SetMouseDragged(false)\n");
	
	}
	SetMouseDragged(false);
	
}