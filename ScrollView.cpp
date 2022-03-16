#include "ScrollView.h"
//#include "windows.h"
ScrollView::ScrollView()
{
	SetPosition(0, 0);
	SetSize(0, 0);

	memset(&ContentInfo, 0x00, sizeof(ContentInfo));
	SetBackGroundColor(SkColorSetRGB(255, 255, 255));
	vert_bar = new ScrollBar(Direction::Vertical);
	hori_bar = new ScrollBar(Direction::Horizontal);
	vert_bar->set_controller(this);
	hori_bar->set_controller(this);
	llDrawTick=0;
	
}

//void ScrollView::Draw(SkCanvas* canvas)
//{
//	GrContext* context = canvas->getGrContext();
//	SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
//	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
//    auto surfaceCanvas = gpuSurface->getCanvas();
//	SkScalar x = 0, y = 0;
//
//	/*ContentInfo.offs += 0.1;
//	if (ContentInfo.offs >= ContentInfo.height - GetHeight())
//	{
//		ContentInfo.offs = 0;
//	}*/
//	y = ContentInfo.offs;
//	for (auto iter = imagelist.begin(); iter != imagelist.end(); iter++)
//	{
//		surfaceCanvas->drawImage((*iter).get(), x, y, 0);
//		y += (*iter)->height();
//	}
//	SkFont font;
//	font.setSubpixel(true);
//	font.setSize(16);
//	SkPaint paint;
//	surfaceCanvas->drawSimpleText("very goods", 10, kUTF8_SkTextEncoding, x, y,font, paint);
//	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
//	canvas->drawImage(image, GetBound().left(), GetBound().top());
//}
#include "tools/gpu/GrContextFactory.h"
void ScrollView::Draw(SkCanvas* canvas)
{
	/*ScrollContentInfo offset;
	memset(&offset, 0x00, sizeof(offset));
	for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		if (nDirectionType == Direction::Vertical)
		{
	
			pChild->SetRect(pChild->GetBound().left(), offset.height+ ContentInfo.offs, pChild->GetWidth() + pChild->GetBound().left(), offset.height + pChild->GetHeight()+ ContentInfo.offs);
			offset.height += pChild->GetHeight();
			offset.width = GetWidth();
		}
		else if (nDirectionType == Direction::Horizontal)
		{
			pChild->SetRect(offset.width+ ContentInfo.offs, pChild->GetBound().top(), pChild->GetWidth() + offset.width+ ContentInfo.offs, pChild->GetBound().top() + pChild->GetHeight());
			offset.width += pChild->GetWidth();
			offset.height = GetHeight();

		}
	}


	ContentInfo.height = offset.height;
	ContentInfo.width = offset.width;*/

	//test 
	/*if (nDirectionType == Direction::Vertical)
	{
		ContentInfo.offs += 0.01;
		if (ContentInfo.offs >= ContentInfo.height - GetHeight())
		{
			ContentInfo.offs = 0;
		}
	}
	else if (nDirectionType == Direction::Horizontal)
	{
		ContentInfo.offs += 0.1;
		if (ContentInfo.offs >= ContentInfo.width - GetWidth())
		{
			ContentInfo.offs = 0;
		}
	}*/


	/*if(GetTickCount64()-llLastDraw<32)
		return;*/
	ContentInfo.offs_y = GetScrolloffsY();
	SkScalar diff_y = ContentInfo.offs_y - ContentInfo.preoffs_y;
	ContentInfo.offs_x = GetScrolloffsX();
	SkScalar diff_x = ContentInfo.offs_x - ContentInfo.preoffs_x;
	//long long llInitStamp=GetTickCount64();

	fDrawTime=SkTime::GetMSecs();
	if (diff_y != 0 || diff_x != 0 || llDrawTick%20==0)
	{
		
		displaylist.clear();
		for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
		{
		    UIWidget *pChild = *iter;
		    pChild->SetBound(pChild->GetBound().left()+diff_x, pChild->GetBound().top() + diff_y, pChild->GetWidth()+diff_x + pChild->GetBound().left(), pChild->GetBound().top() + pChild->GetHeight() + diff_y);

			if (pChild->GetBound().right() >= 0 && pChild->GetBound().left() <= GetDisplayWidth() && pChild->GetBound().bottom() >= 0 && pChild->GetBound().top() <= GetDisplayHeigth())
			{
				displaylist.push_back(pChild);
			}
		}
	}

	/*if (diff != 0)
	{
		printf("diff=%f\n", diff);
	}*/

	ContentInfo.preoffs_y = ContentInfo.offs_y;
	ContentInfo.preoffs_x = ContentInfo.offs_x;
   
    auto context = canvas->recordingContext();
	SkImageInfo info = SkImageInfo::MakeN32(GetDisplayWidth(), GetDisplayHeigth(), kOpaque_SkAlphaType);
	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
    auto surfaceCanvas = gpuSurface->getCanvas();
	SkPaint paint;
	paint.setColor(GetBackGroundColor());
	surfaceCanvas->drawRect(SkRect{ 0,0,GetWidth(),GetHeight() }, paint);

	for (auto iter = displaylist.begin(); iter != displaylist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		pChild->Draw(surfaceCanvas);
	}
	UpdateScrollBarInfo();
	if (vert_bar != NULL)
		vert_bar->Draw(canvas);
	if (hori_bar != NULL)
		hori_bar->Draw(canvas);
	/*if (vert_bar != NULL)
	{
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.height;
		barinfo.DisplaySize=vert_bar->GetHeight();
		barinfo.offset=ContentInfo.offs_y;
		vert_bar->SetScrollBarInfo(barinfo);
		vert_bar->Draw(surfaceCanvas);
	}

	if (hori_bar != NULL)
	{
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.width;
		barinfo.DisplaySize=hori_bar->GetWidth();
		barinfo.offset=ContentInfo.offs_x;
		hori_bar->SetScrollBarInfo(barinfo);
		hori_bar->Draw(surfaceCanvas);
	}*/
	
	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
	canvas->drawImage(image, GetBound().left(), GetBound().top());
	/*if (diff_y != 0 || diff_x != 0)
	   printf("draw need time=%f\n",SkTime::GetMSecs()-fDrawTime);*/
	llDrawTick++;

	//llLastDraw=GetTickCount64();
}

void ScrollView::OnMouseMove(int x, int y)
{

	if (vert_bar != NULL && vert_bar->IsVisible())
		vert_bar->OnMouseMove(x, y);

	if (hori_bar != NULL && hori_bar->IsVisible())
		hori_bar->OnMouseMove(x, y);
	SkPoint point=ScrollViewToChildPoint(x,y);
	/*if (point.x() >= vert_bar->GetBound().left() && point.x() <= vert_bar->GetBound().right() && point.y() >= vert_bar->GetBound().top() && point.y() <= vert_bar->GetBound().bottom())
		return;
	if (point.x() >= hori_bar->GetBound().left() && point.x() <= hori_bar->GetBound().right() && point.y() >= hori_bar->GetBound().top() && point.y() <= hori_bar->GetBound().bottom())
		return ;

	if(point.x()>GetWidth() || point.y()>GetHeight())
		return;*/

	for (auto iter = displaylist.begin(); iter != displaylist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		pChild->OnMouseMove(point.x(), point.y());
	}

	
}

bool  ScrollView::OnMouseDown(int x, int y)
{
    if (vert_bar != NULL && vert_bar->IsVisible())
	{
		if (x >= vert_bar->GetBound().left() && x <= vert_bar->GetBound().right() && y >= vert_bar->GetBound().top() && y <= vert_bar->GetBound().bottom())
		  return  vert_bar->OnMouseDown(x, y);
	}

	if (hori_bar != NULL && hori_bar->IsVisible())
	{
		if (x >= hori_bar->GetBound().left() && x <= hori_bar->GetBound().right() && y >= hori_bar->GetBound().top() && y <= hori_bar->GetBound().bottom())
		   return  hori_bar->OnMouseDown(x, y);
	}
	SkPoint point=ScrollViewToChildPoint(x,y);
	for (auto iter = displaylist.begin(); iter != displaylist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		if (point.x() >= pChild->GetBound().left() && point.x() <= pChild->GetBound().right() && point.y() >= pChild->GetBound().top() && point.y() <= pChild->GetBound().bottom())
		{
			//printf("x=%d,y=%d,child_x=%d,child_y=%d,left=%f,right=%f,top=%f,bottom=%f\n", x, y, point.x(), point.y(), pChild->GetBound().left(), pChild->GetBound().right(), pChild->GetBound().top(), pChild->GetBound().bottom());
			return pChild->OnMouseDown(point.x(), point.y());
		}
		
	}
	return false;

	
}


bool ScrollView::OnMouseUp(int x, int y)
{
	if (vert_bar != NULL && vert_bar->IsVisible())
	{
		vert_bar->OnMouseUp(x, y);
	}
	if (hori_bar != NULL && hori_bar->IsVisible())
	{
		hori_bar->OnMouseUp(x, y);
	}
	return false;
}

void ScrollView::OnMouseWheel(float delta, uint32_t modifier)
{
	//printf("OnMouseWheel delta=%f,modifier=%d\n",delta,modifier);
	if(vert_bar==NULL || !vert_bar->IsVisible())
		return;
	ScrollToPosition(vert_bar,GetScrolloffsY()+delta*10);
}


void ScrollView::ScrollToPosition(ScrollBar* source, int position)
{
	if(source==NULL)
		return;
	if (source == vert_bar)
	{
		SkScalar pos_y=position;
		//printf("scroll before pos=%f\n",pos_y);
		pos_y=std::min((float)0,(float)pos_y);
		pos_y=std::max((float)(-(ContentInfo.height - GetDisplayHeigth())),(float)pos_y);
	//	printf("scroll pos=%f\n",pos_y);
		SetScrolloffsY(pos_y);

	}

	if (source == hori_bar)
	{
		SkScalar pos_x=position;
		pos_x=std::min((float)0,(float)pos_x);
		pos_x=std::max((float)(-(ContentInfo.width - GetDisplayWidth())),(float)pos_x);
		SetScrolloffsX(pos_x);
	}
}

//void ScrollView::SetDirection(Direction nType)
//{
//	nDirectionType = nType;
//}
//
//void ScrollView::JumpTop()
//{
//	if (nDirectionType == Direction::Vertical)
//		ContentInfo.offs_y = 0;
//	this->RunAction(new ScrollTo(0, 0, ContentInfo.offs_y));
//}
//void ScrollView::JumpBottom()
//{
//	if (nDirectionType == Direction::Vertical)
//		ContentInfo.offs_y= -(ContentInfo.height - GetHeight());
//	this->RunAction(new ScrollTo(0, 0, ContentInfo.offs_y));
//	/*if (nDirectionType == Direction::Vertical)
//		ContentInfo.offs = -(ContentInfo.height - GetHeight());
//	SetScrollOffs(ContentInfo.offs);*/
//}
//
//void ScrollView::JumpLeft()
//{
//}
//void ScrollView::JumpRight()
//{
//}

void ScrollView::InitOffset()
{
	ContentInfo.offs_y = 0;
	ContentInfo.preoffs_y = 0;
	ContentInfo.offs_x = 0;
	ContentInfo.preoffs_x = 0;
	SetScrolloffsX(0);
	SetScrolloffsY(0);
}

void ScrollView::RemoveAllChildWidget()
{

	for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		delete pChild;
	}
	childlist.clear();
	InitOffset();

}

void ScrollView::AddChild(UIWidget *pWidget)
{
	childlist.push_back(pWidget);
}

SkScalar ScrollView::GetDisplayWidth()
{
	SkScalar width=GetBound().width();
	if (ContentInfo.height > GetBound().height() && vert_bar->IsVisible())
		width-=vert_bar->GetWidth();
	return width;
}
SkScalar ScrollView::GetDisplayHeigth() 
{
	SkScalar heigth=GetBound().height();
	if (ContentInfo.width > GetBound().width() && hori_bar->IsVisible())
		heigth-=hori_bar->GetHeight();
	return heigth;
}

void ScrollView::SetContentSize(SkScalar width, SkScalar height)
{
	ContentInfo.height = height;
	ContentInfo.width = width;
	UpdateScrollBarInfo();
}



void ScrollView::UpdateScrollBarInfo()
{
	if (ContentInfo.height > GetDisplayHeigth())
	{
		if (vert_bar->GetWidth() == 0)
		{
			vert_bar->SetPosition(GetBound().width()-BAR_VER_WIDTH+GetBound().left(),GetBound().top());
			vert_bar->SetSize(BAR_VER_WIDTH,GetDisplayHeigth());
		}
		else
		{
			vert_bar->SetPosition(GetBound().width()-vert_bar->GetBound().width()+GetBound().left(),GetBound().top());
			vert_bar->SetSize(vert_bar->GetBound().width(),GetDisplayHeigth());
		}

		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.height;
		barinfo.DisplaySize=vert_bar->GetHeight();
		barinfo.offset=ContentInfo.offs_y;
		SkScalar ContentMoveMax=barinfo.ContentSize-barinfo.DisplaySize;
		barinfo.offset=std::max(barinfo.offset,-ContentMoveMax);
		SetScrolloffsY(barinfo.offset);
		vert_bar->SetScrollBarInfo(barinfo);
	}
	else
	{
		vert_bar->SetBound(0,0,0,0);
		SetScrolloffsY(0);
	}

	if (ContentInfo.width > GetDisplayWidth())
	{
		if (hori_bar->GetWidth() == 0)
		{
			hori_bar->SetPosition(GetBound().left(), GetBound().bottom()-BAR_HORI_HEIGHT);
			hori_bar->SetSize(GetDisplayWidth(), BAR_HORI_HEIGHT);
		}
		else
		{
			hori_bar->SetPosition(GetBound().left(),GetBound().bottom()-hori_bar->GetBound().height());
			hori_bar->SetSize(GetDisplayWidth(), hori_bar->GetBound().height());
		}
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.width;
		barinfo.DisplaySize=hori_bar->GetWidth();
		barinfo.offset=ContentInfo.offs_x;

		SkScalar ContentMoveMax=barinfo.ContentSize-barinfo.DisplaySize;
		barinfo.offset=std::max(barinfo.offset,-ContentMoveMax);
		SetScrolloffsX(barinfo.offset);
		hori_bar->SetScrollBarInfo(barinfo);
	}
	else
	{
		hori_bar->SetBound(0,0,0,0);
		SetScrolloffsX(0);
	}
}



void ScrollView::SetHorizontalScrollBar(ScrollBar* horiz_sb) 
{
   horiz_sb->SetVisible(vert_bar->IsVisible());
   delete vert_bar;
   horiz_sb->set_controller(this);
   vert_bar = horiz_sb;
}

void ScrollView::SetVerticalScrollBar(ScrollBar* vert_sb) 
{
   vert_sb->SetVisible(hori_bar->IsVisible());
   delete hori_bar;
   vert_sb->set_controller(this);
   hori_bar = vert_sb;
}




//void  ScrollView::AddChild(char *pImagePath)
//{
//
//	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
//	if (blob == NULL) return;
//	sk_sp<SkImage> image = SkImage::MakeFromEncoded(blob);
//	imagelist.push_back(image);
//
//	if (nDirectionType == Direction::Vertical)
//	{
//		ContentInfo.height += image->height();
//		ContentInfo.width = GetWidth();
//	}
//	else if (nDirectionType == Direction::Horizontal)
//	{
//		ContentInfo.width += image->width();
//		ContentInfo.height = GetHeight();
//	}
//
//	//sk_sp<SkSurface> surface =  SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info, 0, &props) : SkSurface::MakeRaster(info, &props);
//
//
//}

//void draw(SkCanvas* canvas) {
//	SkPaint paint;
//	paint.setTextSize(32);
//	GrContext* context = canvas->getGrContext();
//	if (!context) {
//		canvas->drawString("GPU only!", 20, 40, paint);
//		return;
//	}
//	SkImageInfo info = SkImageInfo::MakeN32(256, 64, kOpaque_SkAlphaType);
//	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
//	auto surfaceCanvas = gpuSurface->getCanvas();
//	surfaceCanvas->clear(SK_ColorWHITE);
//	surfaceCanvas->drawString("GPU rocks!", 20, 40, paint);
//	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
//	canvas->drawImage(image, 0, 0);
//}