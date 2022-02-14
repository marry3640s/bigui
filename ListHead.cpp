#include "ListHead.h"
#define LEFT_PADDING 5
ListHead::ListHead(ListView *view)
{
	listview=view;
	SetPosition(0, 0);
	SetSize(0, 0);
	nMouseDownCol=-1;
}

ListHead::~ListHead()
{
}
#include "tools/gpu/GrContextFactory.h"
void ListHead::Draw(SkCanvas* canvas)
{
	//GrContext* context = canvas->getGrContext();
    auto context = canvas->recordingContext();
	SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
    auto surfaceCanvas = gpuSurface->getCanvas();

	SkPaint paint;
	paint.setColor(SkColorSetRGB(100, 100, 100));
	surfaceCanvas->drawRect(SkRect{ 0,0,GetWidth(),GetHeight() }, paint);
	std::vector<ColumnInfo> columns=listview->GetColList();
	SkScalar offset_x=listview->GetScrolloffsX();
	int ins_x=0;
	SkFont font;
	font.setSubpixel(true);
	font.setSize(16);

	
	for (int k = 0; k < columns.size(); k++)
	{
		SkRect bounds;
        font.measureText(columns[k].colname.c_str(),
                         columns[k].colname.size(),
                         SkTextEncoding::kUTF8,
                         &bounds);
		paint.setColor(SkColorSetRGB(0, 0, 0));
		SkScalar left_x=LEFT_PADDING;

		//SkScalar text_top=(GetHeight()-bounds.height())/2-bounds.top();
		int nDrawTextLength=columns[k].colname.size();

		if (bounds.width() > columns[k].nWidth-LEFT_PADDING)
			nDrawTextLength=(columns[k].nWidth-LEFT_PADDING)/(bounds.width()/columns[k].colname.size())-1;
		if (listview->GetListAligment() == ListView::ListAligment::LEFT)
		{
		}
		else if (listview->GetListAligment() == ListView::ListAligment::CENTER)
		{
			if (columns[k].nWidth > bounds.width())
				left_x=(columns[k].nWidth-bounds.width())/2;
		}
		else
		{
			if (columns[k].nWidth > bounds.width())
				left_x=columns[k].nWidth-bounds.width()-LEFT_PADDING;
		}
		nDrawTextLength=std::max(0,nDrawTextLength);
        surfaceCanvas->drawSimpleText(columns[k].colname.c_str(),
                                      nDrawTextLength,
                                      SkTextEncoding::kUTF8,
                                      offset_x + ins_x + left_x,
                                      15,
                                      font,
                                      paint);
		ins_x+=columns[k].nWidth;
		SkPoint p1,p2;
		p1.set(ins_x+offset_x,0);
		p2.set(ins_x+offset_x,GetHeight());
		paint.setColor(SkColorSetRGB(30, 30, 30));
		surfaceCanvas->drawLine(p1,p2,paint);
		
		
	}

	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
	canvas->drawImage(image, GetBound().left(), GetBound().top());

	return;
	

}


int ListHead::GetMousePosCol(int x, int y)
{
	if (x < GetBound().left() || x > GetBound().right() || y < GetBound().top() || y > GetBound().bottom())
		return -1;
	std::vector<ColumnInfo> columns=listview->GetColList();
	int ins_x=listview->GetScrolloffsX()+GetBound().left();
	for (int k = 0; k < columns.size(); k++)
	{
		if(x>=ins_x+LEFT_PADDING && x<=ins_x+columns[k].nWidth-LEFT_PADDING)
			return k;
		ins_x+=columns[k].nWidth;
	}
	return -1;
}

void ListHead::OnMouseMove(int x, int y)
{
	nMouseDownCol=GetMousePosCol(x,y);
}
void ListHead::OnMouseDown(int x, int y)
{
	nMouseDownCol=GetMousePosCol(x,y);
}
void ListHead::OnMouseUp(int x,int y)
{
	if(nMouseDownCol==GetMousePosCol(x,y) && nMouseDownCol>=0)
		listview->ReadySort(nMouseDownCol);

}
