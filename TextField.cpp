#include "TextField.h"
#include "windows.h"
#include "include/core/SkTextBlob.h"
#include "tools/gpu/GrContextFactory.h"
using namespace CharEncoding;
TextField::TextField() 
{
    SetBackGroundColor(SkColorSetRGB(255, 255, 255));
    text_color = SkColorSetRGB(0, 0, 0);
	memset(&ContentInfo, 0x00, sizeof(ContentInfo));
	vert_bar = new ScrollBar(Direction::Vertical);
    hori_bar = new ScrollBar(Direction::Horizontal);
    vert_bar->set_controller(this);
    hori_bar->set_controller(this);

	nTextFieldStyle = TextFieldStyle::multi_line;
    
    unindex = 0;
	fNextTime = -DBL_MAX;
	
    fCurPosBlinkTime = SkTime::GetMSecs();

	
	font.setSubpixel(true);
	font.setSize(18);
	font.setTypeface(SkTypeface::MakeFromName("simsun", SkFontStyle::Normal()));
	
	memset(&inspos, 0x00, sizeof(inspos));
	memset(chinachar, 0x00, sizeof(chinachar));
	bSelFlag = false;
	ResetCurUndo();

	nShowNumWidth = 0;
	SetMouseDragged(false);

	SkPaint paint;
	SkRect bounds;
	SkString str = SkStringPrintf("历史市场数据AHKKMN");
	font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds, &paint);
	nLineHeight = bounds.height()+1;
	SetContentSize(GetWidth(), line.size() * nLineHeight);
	
	/*if (line.size() == 0)
	{
		textline info;
		info.nHeight = nLineHeight;
		line.push_back(info);
	}*/
}
TextField::~TextField() 
{
}

void TextField::insertline(textline info)
{
	//info.nHeight = nLineHeight;
	if (info.txtbuf.data()[info.txtbuf.size() - 1] == '\n')
	{
		info.txtbuf.erase(info.txtbuf.size()-1, 1);
		//info.txtbuf.data()[info.txtbuf.size() - 1] = 0x00;
	}
	line.push_back(info);
}

int TextField::DrawSelRect(SkCanvas* surfaceCanvas,int nLine)
{
	SkRect rect;
	TextPoint first, sec;
	if (bSelFlag == false)
		return false;
	memcpy(&first, &selinfo.init, sizeof(TextPoint));
	memcpy(&sec, &selinfo.end, sizeof(TextPoint));
	/*if (selinfo.end.y > selinfo.init.y)
	{
		memcpy(&first, &selinfo.init, sizeof(TextPoint));
		memcpy(&sec, &selinfo.end, sizeof(TextPoint));
	}
	else if (selinfo.end.y < selinfo.init.y)
	{
		memcpy(&first, &selinfo.end, sizeof(TextPoint));
		memcpy(&sec, &selinfo.init, sizeof(TextPoint));
	}
	else if (selinfo.end.y == selinfo.init.y && selinfo.end.x > selinfo.init.x)
	{
		memcpy(&first, &selinfo.init, sizeof(TextPoint));
		memcpy(&sec, &selinfo.end, sizeof(TextPoint));
	}
	else if (selinfo.end.y == selinfo.init.y && selinfo.end.x == selinfo.init.x)
	{
		memcpy(&first, &selinfo.end, sizeof(TextPoint));
		memcpy(&sec, &selinfo.init, sizeof(TextPoint));
	}
	else if (selinfo.end.y == selinfo.init.y && selinfo.end.x < selinfo.init.x)
	{
		memcpy(&first, &selinfo.end, sizeof(TextPoint));
		memcpy(&sec, &selinfo.init, sizeof(TextPoint));
	}
	else
	{
		printf("gggg\n");
	}*/
	int ins_y =  nLine* nLineHeight+ ContentInfo.offs_y;
	int ins_x = ContentInfo.offs_x;
	//printf("ins_x=%d\n", ins_x);
	SkPaint sel;
	sel.setColor(SkColorSetRGB(153, 201, 239));
	/*SkPaint sel;
	sel.setColor(SK_ColorYELLOW);
	surfaceCanvas->drawRect(SkRect{ 8, 0, 16, nLineHeight }, sel);*/
	char *text = line[nLine].txtbuf.data();
	SkPaint paint;
	SkRect bounds;
	SkScalar nextwidth = 0;

	if (first.y == nLine)
	{
		
		if (first.y == sec.y)
		{
			SkScalar width = font.measureText(text, first.x, SkTextEncoding::kUTF8, &bounds, &paint);
			nextwidth = font.measureText(text, sec.x, SkTextEncoding::kUTF8, &bounds, &paint);
			surfaceCanvas->drawRect(SkRect{ width+ ins_x, (SkScalar)ins_y, nextwidth+ ins_x, (SkScalar)ins_y + nLineHeight }, sel);
		}
		////向上滚动
		else if (first.y > sec.y)
		{
			SkScalar width = font.measureText(text, first.x, SkTextEncoding::kUTF8, &bounds, &paint);
			nextwidth = font.measureText(text, first.x , SkTextEncoding::kUTF8, &bounds, &paint);
			surfaceCanvas->drawRect(SkRect{ (SkScalar)(0 + ins_x), (SkScalar)ins_y, nextwidth + ins_x, (SkScalar)ins_y + nLineHeight }, sel);
		}
		////向下滚动
		else if (first.y < sec.y)
		{
			SkScalar width = font.measureText(text, first.x, SkTextEncoding::kUTF8, &bounds, &paint);
			nextwidth = font.measureText(text, strlen(text), SkTextEncoding::kUTF8, &bounds, &paint);
			surfaceCanvas->drawRect(SkRect{ width + ins_x, (SkScalar)ins_y, nextwidth + ins_x, (SkScalar)ins_y + nLineHeight }, sel);
		}
	}
	else if ((nLine > first.y && nLine < sec.y) || (nLine>sec.y && nLine<first.y))
	{
		nextwidth = font.measureText(text, strlen(text), SkTextEncoding::kUTF8, &bounds, &paint);
		surfaceCanvas->drawRect(SkRect{ (SkScalar)(0 + ins_x), (SkScalar)ins_y, nextwidth + ins_x, (SkScalar)ins_y + nLineHeight }, sel);
	}
	else if (sec.y==nLine)
	{
		////向上滚动
		if (first.y > sec.y)
		{
			SkScalar width = font.measureText(text, sec.x, SkTextEncoding::kUTF8, &bounds, &paint);
			nextwidth = font.measureText(text, strlen(text), SkTextEncoding::kUTF8, &bounds, &paint);
			//SkScalar width = font.measureText(text, sec.x, SkTextEncoding::kUTF8, &bounds, &paint);;
			surfaceCanvas->drawRect(SkRect{ width + ins_x, (SkScalar)ins_y, nextwidth + ins_x, (SkScalar)ins_y + nLineHeight }, sel);
		}
		////向下滚动
		else if (first.y < sec.y)
		{
			SkScalar width = font.measureText(text, sec.x, SkTextEncoding::kUTF8, &bounds, &paint);;
			surfaceCanvas->drawRect(SkRect{ (SkScalar)(0 + ins_x), (SkScalar)ins_y, width + ins_x, (SkScalar)ins_y + nLineHeight }, sel);
		}
	}


	

	return true;
}

void TextField::Draw(SkCanvas* canvas)
{
	if (IsVisible() == false)
		return;
	ContentInfo.offs_y = GetScrolloffsY();
	ContentInfo.offs_x = GetScrolloffsX();
	int nIndex = (-ContentInfo.offs_y) / nLineHeight;
	SkPaint paint;
	SkCanvas *surfaceCanvas_num;
	sk_sp<SkSurface> gpuSurface_num;
	SkRect bounds;
	if (nTextFieldStyle & TextFieldStyle::show_linenum)
	{
		
		//SkScalar fontwidth = 0;
		//计算显示行数的宽度
		int nMaxNum = nIndex + GetDisplayHeigth() / nLineHeight + 1;
		SkString str = SkStringPrintf("%d", nMaxNum);
		font.measureText(str.c_str(), str.size(), SkTextEncoding::kUTF8, &bounds, &paint);
		nShowNumWidth = bounds.width() + 10;

		auto context_num = canvas->recordingContext();
		SkImageInfo info_num = SkImageInfo::MakeN32((SkScalar)nShowNumWidth, GetHeight(), kOpaque_SkAlphaType);
		gpuSurface_num=SkSurface::MakeRenderTarget(context_num, SkBudgeted::kNo, info_num);
		surfaceCanvas_num = gpuSurface_num->getCanvas();
		paint.setColor(GetBackGroundColor());
		surfaceCanvas_num->drawRect(SkRect{ 0, 0, (SkScalar)nShowNumWidth, GetHeight() }, paint);
	}

	int ins_y = nLineHeight - (-ContentInfo.offs_y) % nLineHeight;
	int ins_x = ContentInfo.offs_x ;

	auto context = canvas->recordingContext();
	SkImageInfo info = SkImageInfo::MakeN32(GetWidth()- nShowNumWidth, GetHeight(), kOpaque_SkAlphaType);
	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
	auto surfaceCanvas = gpuSurface->getCanvas();
	
	paint.setColor(GetBackGroundColor());
	surfaceCanvas->drawRect(SkRect{ 0, 0, GetWidth() - nShowNumWidth, GetHeight() }, paint);

	SkScalar fontwidth = 0;
	SkScalar fDisplayWidth = 0;
	for (int k = nIndex; k < line.size(); k++)
	{
		//if (GetMouseDragged())

		DrawSelRect(surfaceCanvas, k);
		//paint.setColor(0xDE000000);
		paint.setColor(text_color);

		if (nTextFieldStyle & TextFieldStyle::show_linenum)
		{
			SkPaint paint_num;
			paint_num.setColor(SkColorSetARGB(0xFF, 45, 145, 175));
			SkString str = SkStringPrintf("%d", 1 + k);
			surfaceCanvas_num->drawSimpleText(str.c_str(), str.size(), SkTextEncoding::kUTF8,
				9, ins_y, font, paint_num);
		}
		//char *text = G2U(line[k].txtbuf.data());
		char *text = line[k].txtbuf.data();
		//if (text != 0)
		{
			surfaceCanvas->drawSimpleText(text, strlen(text), SkTextEncoding::kUTF8,
				ins_x, ins_y, font, paint);
			fontwidth = font.measureText(text, strlen(text), SkTextEncoding::kUTF8, &bounds, &paint);
			fDisplayWidth = std::max(fDisplayWidth, fontwidth);
		}
		//delete text;
		ins_y += nLineHeight;
		SetContentSize(fDisplayWidth + 32, line.size() * nLineHeight +/*(int)GetDisplayHeigth()% */nLineHeight);
		if (ins_y > GetDisplayHeigth())
			break;
	}

	DrawCursor(surfaceCanvas);

	

	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
	canvas->drawImage(image, GetBound().left()+ nShowNumWidth, GetBound().top());
	if (nTextFieldStyle & TextFieldStyle::show_linenum)
	{
		sk_sp<SkImage> image2(gpuSurface_num->makeImageSnapshot());
		canvas->drawImage(image2, GetBound().left(), GetBound().top());
	}

	UpdateScrollBarInfo();
	if (vert_bar != NULL)
		vert_bar->Draw(canvas);
	if (hori_bar != NULL)
		hori_bar->Draw(canvas);
	
}


//获取当前光标的X位置
SkScalar TextField::GetCursorX()
{
	if (line.size() == 0)
		return 0;
	SkScalar width;
	SkPaint paint;
	SkRect bounds;
	char *text = line[inspos.y].txtbuf.data();
	//if (text[inspos.x - 1] == ' ')
	//{
	//	//measureText过滤了空格的处理，所以我要格外处理空格的问题
	//	std::string temp = text;
	//	temp[inspos.x - 1] = 'i';
	//	font.measureText(temp.data(), inspos.x, SkTextEncoding::kUTF8, &bounds, &paint);
	//}
	//else
	//{
	//
	//	ret =font.measureText(text, inspos.x, SkTextEncoding::kUTF8, &bounds, &paint);
	//}
	//delete text;
	width = font.measureText(text, inspos.x, SkTextEncoding::kUTF8, &bounds, &paint);
	return width + GetScrolloffsX();
		
}

//获取当前光标的Y位置
SkScalar TextField::GetCursorY()
{
	/*SkScalar height = 0;
	for (int k = 0; k < line.size(); k++)
	{
		height += line[k].nHeight;
	}
	return height-20;*/
	return inspos.y * nLineHeight+ GetScrolloffsY();
}

void TextField::DrawCursor(SkCanvas* canvas)
{
	SkPaint CursorPaint;
	CursorPaint.setColor(SK_ColorBLACK);
	//CursorPaint.setStyle(SkPaint::kStroke_Style);
	//CursorPaint.setStrokeWidth(1);
	CursorPaint.setAntiAlias(true);

	double now = SkTime::GetNSecs();
	if (now >= fNextTime) {
		constexpr double kHalfPeriodNanoSeconds = 0.5 * 1e9;
		fNextTime = now + kHalfPeriodNanoSeconds;
		bBlink = !bBlink;
		
	}
	if (bBlink) {
	 	canvas->drawRect(SkRect::MakeXYWH(GetCursorX(), GetCursorY()+1, 1, nLineHeight+1), CursorPaint);
	 }
}





void TextField::OnMouseMove(int x, int y) 
{
    if (vert_bar != NULL && vert_bar->IsVisible())
		vert_bar->OnMouseMove(x, y);
    if (hori_bar != NULL && hori_bar->IsVisible()) 
		hori_bar->OnMouseMove(x, y);

	if (GetMouseDragged())
	{
		if (line.size() == 0)
			return;
		//selinfo.init
		bSelFlag = true;
		SkPoint point = ScrollViewToChildPoint(x, y);
		selinfo.end.y=  (point.y() + (-GetScrolloffsY())) / nLineHeight;
		selinfo.end.y = std::max(0, selinfo.end.y);
		selinfo.end.y = std::min(selinfo.end.y, (int)line.size()-1);
		selinfo.end.x = GetMouseXCharOffset(point.x());
		inspos.x = selinfo.end.x;
		inspos.y = selinfo.end.y;
		printf("init:%d,%d end:%d,%d,x=%d,y=%d\n",selinfo.init.x,selinfo.init.y,selinfo.end.x,selinfo.end.y,x,y);

		if ((inspos.y+2)*nLineHeight + GetScrolloffsY() > GetDisplayHeigth())
		{
			ScrollToPosition(vert_bar, -(-GetScrolloffsY() + nLineHeight*3));
		}
		else if (inspos.y*nLineHeight + GetScrolloffsY()<0)
		{
			ScrollToPosition(vert_bar, -(-GetScrolloffsY() - nLineHeight * 3));
		}

		if (selinfo.end.y == selinfo.init.y)
		{
			printf("curX=%f,%f\n", GetCursorX(), GetScrolloffsX());
			if (GetCursorX()-30 < 0  && GetScrolloffsX()<0)
			{
				ScrollToPosition(hori_bar, -(-GetScrolloffsX() - 50));
			}
			if (GetCursorX() + 50 >= GetDisplayWidth())
			{
				ScrollToPosition(hori_bar, -(-GetScrolloffsX() + 50));
			}
		}

		

	}
}


FontInfoEach TextField::GetFontWidthForLineIndex(int nIndex)
{
	FontInfoEach info;
	memset(&info, 0x00, sizeof(FontInfoEach));
	SkPaint paint;
	SkRect bounds;
	std::string temp;
	int ret;
	int k = nIndex;
	info.nNextIndex = k;
	if (k > line[inspos.y].txtbuf.size())
		return info;
	for (;;)
	{
		info.nNextIndex++;
		temp.push_back(line[inspos.y].txtbuf.data()[k - 1]);
		ret = font.measureText(line[inspos.y].txtbuf.data(), k, SkTextEncoding::kUTF8, &bounds, &paint);
		if (ret > 0 || k >= line[inspos.y].txtbuf.size())
			break;
		k++;
	}
	//info.nNextIndex = k;
	info.total_width = ret/*bounds.width()*/;
	info.ch_width=font.measureText(temp.data(), temp.size(), SkTextEncoding::kUTF8, &bounds, &paint);
	//info.ch_width= bounds.width();
	return info;
}

int TextField::GetMouseXCharOffset(int point_x)
{
	int k = 1;
	int inx = 0;
	FontInfoEach info;
	SkScalar pp = point_x - GetScrolloffsX();
	int nOffset=0;
	for (;;)
	{
		info = GetFontWidthForLineIndex(k);
		if (pp < info.total_width || k >= line[inspos.y].txtbuf.size())
		{
			inx = info.nNextIndex - 1;
			break;
		}
		k = info.nNextIndex;
	}
	if (pp > info.total_width - info.ch_width / 2) //偏移向右，指向下一个
		nOffset = inx;
	else
		nOffset = inx - (info.nNextIndex - k);

	nOffset = std::min(nOffset, (int)line[inspos.y].txtbuf.size());
	return nOffset;
}

bool TextField::OnMouseDown(int x, int y)
{
	if (IsVisible() == false)
		return false;
	if (vert_bar != NULL && vert_bar->IsVisible())
	{
        if (x >= vert_bar->GetBound().left() && x <= vert_bar->GetBound().right() && y >= vert_bar->GetBound().top() && y <= vert_bar->GetBound().bottom())
            return vert_bar->OnMouseDown(x, y);
    }

    if (hori_bar != NULL && hori_bar->IsVisible())
	{
        if (x >= hori_bar->GetBound().left() && x <= hori_bar->GetBound().right() && y >= hori_bar->GetBound().top() && y <= hori_bar->GetBound().bottom())
            return hori_bar->OnMouseDown(x, y);
    }

    SkPoint point = ScrollViewToChildPoint(x, y);

    if (point.y() > line.size() * nLineHeight) return false;

	//int nYIndex = (-GetScrolloffsY()) / nLineHeight;
    int nIndex = (point.y()+ (-GetScrolloffsY())) / nLineHeight ;
	if (nIndex >= line.size())
		return false;
	
   
    SkPaint paint;
    SkRect bounds;
	inspos.y = nIndex;
	printf("%d,%f\n", inspos.y+1, GetScrolloffsY());
	//const char* fLocale = "en";
	//ShapeResult result = Shape(line[inspos.y].txtbuf.data(), line[inspos.y].txtbuf.size(),font, fLocale, GetDisplayWidth());
	inspos.x = GetMouseXCharOffset(point.x());
	
	//if(bSelFlag==false)
	bSelFlag = false;
	if (GetMouseDragged() == false)
	{
		selinfo.init.x = inspos.x;
		selinfo.init.y = inspos.y;
	}
	SetMouseDragged(true);

	/*if (!(cur_undo.ins_begin.x == inspos.x || cur_undo.ins_begin.y == inspos.y))
	{
		undo.push(cur_undo);
		ResetCurUndo();
	}*/


	return false;
	
  
}

bool TextField::OnMouseUp(int x, int y)
{
	if (IsVisible() == false)
		return false;
	/*if (GetMouseDragged() == true)
	{
		inspos.x=selinfo.end.x;
		inspos.y = selinfo.end.y;
	}*/
	SetMouseDragged(false);
   /* if (x < GetBound().left() || x > GetBound().right() || y < GetBound().top() ||y > GetBound().bottom())
        return ;*/
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

void TextField::OnMouseWheel(float delta, uint32_t modifier) 
{
	if (IsVisible() == false)
		return;
    if (vert_bar == NULL || !vert_bar->IsVisible()) 
		return;
    ScrollToPosition(vert_bar, GetScrolloffsY() + delta * 20);
}
void TextField::PushCurUndeSelDel()
{
	TextPoint first, sec;
	if (selinfo.end.y > selinfo.init.y)
	{
		memcpy(&first, &selinfo.init, sizeof(TextPoint));
		memcpy(&sec, &selinfo.end, sizeof(TextPoint));
	}
	else if (selinfo.end.y < selinfo.init.y)
	{
		memcpy(&first, &selinfo.end, sizeof(TextPoint));
		memcpy(&sec, &selinfo.init, sizeof(TextPoint));
	}
	else if (selinfo.end.y == selinfo.init.y && selinfo.end.x > selinfo.init.x)
	{
		memcpy(&first, &selinfo.init, sizeof(TextPoint));
		memcpy(&sec, &selinfo.end, sizeof(TextPoint));
	}
	else if (selinfo.end.y == selinfo.init.y && selinfo.end.x == selinfo.init.x)
	{
		memcpy(&first, &selinfo.end, sizeof(TextPoint));
		memcpy(&sec, &selinfo.init, sizeof(TextPoint));
	}
	else if (selinfo.end.y == selinfo.init.y && selinfo.end.x < selinfo.init.x)
	{
		memcpy(&first, &selinfo.end, sizeof(TextPoint));
		memcpy(&sec, &selinfo.init, sizeof(TextPoint));
	}

	std::string cptext;
	char *pText;
	int nIdx = 0;
	for (int k = first.y; k <= sec.y; k++)
	{
		if (k == first.y)
		{

			//line[k].txtbuf.erase(first.x, line[k].txtbuf.size() - first.x);
			if (first.y != sec.y)
			{
				cptext.insert(cptext.size(), line[k].txtbuf.data() + first.x, line[k].txtbuf.size() - first.x);
				cptext.push_back(0x0a);
			}
			else
			{
				cptext.insert(cptext.size(), line[k].txtbuf.data() + first.x, sec.x - first.x);
			}
			 

		}
		else if (k == sec.y)
		{

			cptext.insert(cptext.size(), line[k].txtbuf.data(), sec.x);
			/*line[k - nIdx].txtbuf.erase(0, sec.x);

			int temp = line[first.y].txtbuf.size();
			line[first.y].txtbuf.insert(temp, line[k - nIdx].txtbuf.data(), line[k - nIdx].txtbuf.size());
			line[k - nIdx].txtbuf.erase();
			line.erase(line.begin() + k - nIdx);*/
		}
		else
		{
			cptext.insert(cptext.size(), line[k].txtbuf.data(), line[k].txtbuf.size());
			cptext.push_back(0x0a);
			//删除整行
			/*line[k - nIdx].txtbuf.erase();
			line.erase(line.begin() + k - nIdx);
			nIdx++;*/
		}
		//std::reverse(cptext.begin(), cptext.end());
		//cur_undo.text.insert(0,cptext.data(),cptext.size());
	
		//cptext.clear();
	}
	std::reverse(cptext.begin(), cptext.end());
	cur_undo.text = cptext;
	cur_undo.ins_end.x = first.x;
	cur_undo.ins_end.y = first.y;
}
void TextField::TextSelDel()
{
	TextPoint first, sec;
	if (selinfo.end.y > selinfo.init.y)
	{
		memcpy(&first, &selinfo.init, sizeof(TextPoint));
		memcpy(&sec, &selinfo.end, sizeof(TextPoint));
	}
	else if (selinfo.end.y < selinfo.init.y)
	{
		memcpy(&first, &selinfo.end, sizeof(TextPoint));
		memcpy(&sec, &selinfo.init, sizeof(TextPoint));
	}
	else if (selinfo.end.y == selinfo.init.y && selinfo.end.x > selinfo.init.x)
	{
		memcpy(&first, &selinfo.init, sizeof(TextPoint));
		memcpy(&sec, &selinfo.end, sizeof(TextPoint));
	}
	else if (selinfo.end.y == selinfo.init.y && selinfo.end.x == selinfo.init.x)
	{
		memcpy(&first, &selinfo.end, sizeof(TextPoint));
		memcpy(&sec, &selinfo.init, sizeof(TextPoint));
	}
	else if (selinfo.end.y == selinfo.init.y && selinfo.end.x < selinfo.init.x)
	{
		memcpy(&first, &selinfo.end, sizeof(TextPoint));
		memcpy(&sec, &selinfo.init, sizeof(TextPoint));
	}

	std::string cptext;
	char *pText;
	int nIdx = 0;
	for (int k = first.y; k <= sec.y; k++)
	{
		if (k == first.y)
		{
			if(first.y!=sec.y)
			   line[k].txtbuf.erase(first.x, line[k].txtbuf.size()-first.x);
			else
				line[k].txtbuf.erase(first.x, sec.x - first.x);
		
		}
		else if (k == sec.y)
		{
			line[k-nIdx].txtbuf.erase(0, sec.x);

			int temp = line[first.y].txtbuf.size();
			line[first.y].txtbuf.insert(temp, line[k - nIdx].txtbuf.data(), line[k - nIdx].txtbuf.size());
			line[k - nIdx].txtbuf.erase();
			line.erase(line.begin() + k - nIdx);
		}
		else
		{
			//删除整行
			line[k - nIdx].txtbuf.erase();
			line.erase(line.begin() + k - nIdx);
			nIdx++;
		}
	}
	inspos.x = first.x;
	inspos.y = first.y;
	bSelFlag = false;
}
void TextField::OnKey(skui::Key key, uint32_t modifiers) {
	if (IsVisible() == false)
		return;
	skui::ModifierKey modi = (skui::ModifierKey)modifiers;
	using sknonstd::Any;
	skui::ModifierKey ctrlAltCmd = modi & (skui::ModifierKey::kControl |
		skui::ModifierKey::kOption |
		skui::ModifierKey::kCommand);
	bool shift = Any(modi & (skui::ModifierKey::kShift));
	if (!Any(ctrlAltCmd)) {
		/*if (key == skui::Key::kLeft || key == skui::Key::kRight || key == skui::Key::kUp ||
			key == skui::Key::kDown || key == skui::Key::kHome || key == skui::Key::kEnd)
		{

		}*/
		if (key == skui::Key::kDown)
		{
			inspos.y++;
			inspos.y = std::min(inspos.y,(int)line.size()-1);
			inspos.x = line[inspos.y].txtbuf.size();
			if ( (inspos.y-1)*nLineHeight > -GetScrolloffsY())
			{
				ScrollToPosition(vert_bar, -(-GetScrolloffsY() + nLineHeight));
			}
		}
		else if (key == skui::Key::kUp)
		{
			inspos.y--;
			inspos.y = std::max(0, inspos.y);
			inspos.x =  line[inspos.y].txtbuf.size();
			if(-GetScrolloffsY() > inspos.y*nLineHeight)
			{
				ScrollToPosition(vert_bar, -(-GetScrolloffsY() - nLineHeight));
			}
		}
		else if (key == skui::Key::kLeft)
		{
			if (inspos.y == 0 && inspos.x == 0) return;
			if (inspos.x == 0)
			{
				inspos.y--;
				inspos.x = line[inspos.y].txtbuf.size();
				if (-GetScrolloffsY() > inspos.y*nLineHeight)
				{
					ScrollToPosition(vert_bar, -(-GetScrolloffsY() - nLineHeight));
				}
			}
			else
			{
				inspos.x--;
				SkPaint paint;
				SkRect bounds;
				int ret;
				if (inspos.x > 0)
				{
					for (;;)
					{

						ret = font.measureText(line[inspos.y].txtbuf.data(), inspos.x, SkTextEncoding::kUTF8, &bounds, &paint);
						if (ret > 0 || inspos.x==0)
							break;
						inspos.x--;
					}
				}
				if (GetCursorX() < 0)
				{
					ScrollToPosition(hori_bar, -(-GetScrolloffsX() - 16));
				}
			}
			printf("inspos.x=%d,modifiers=%d\n", inspos.x, modifiers);
		}
		else if (key == skui::Key::kRight)
		{
			
			if (inspos.x >= line[inspos.y].txtbuf.size() && inspos.y < line.size() - 1)
			{
				inspos.y++;
				inspos.x = 0;
				if ((inspos.y - 1)*nLineHeight > -GetScrolloffsY())
				{
					ScrollToPosition(vert_bar, -(-GetScrolloffsY() + nLineHeight));
				}
				// curpos.y = std::min((int)line.size() - 1, curpos.y);
			}
			else
			{
				inspos.x++;
				inspos.x = std::min(inspos.x, (int)line[inspos.y].txtbuf.size());
				SkPaint paint;
				SkRect bounds;
				int ret;
				
				for (;;)
				{
					ret = font.measureText(line[inspos.y].txtbuf.data(), inspos.x, SkTextEncoding::kUTF8, &bounds, &paint);
					if (ret > 0 || inspos.x == line[inspos.y].txtbuf.size())
						break;
					inspos.x++;
					inspos.x = std::min(inspos.x, (int)line[inspos.y].txtbuf.size());
				}

				if (GetCursorX() + 20 >= GetDisplayWidth())
				{
					ScrollToPosition(hori_bar, -(-GetScrolloffsX() + 16));
				}
				
			}
		}

		else if (key == skui::Key::kBack)
		{
			if (!(cur_undo.ins_end.x == inspos.x && cur_undo.ins_end.y == inspos.y) && cur_undo.state != UndoState::None)
			{
				undo.push(cur_undo);
				ResetCurUndo();
			}
			if (cur_undo.text.size() == 0)
			{
				cur_undo.ins_begin.x = inspos.x;
				cur_undo.ins_begin.y = inspos.y;
			}
			cur_undo.state = UndoState::Del;
			
			if (bSelFlag == true)
			{
				PushCurUndeSelDel();
				TextSelDel();
			}

			else if (inspos.x == 0)
			{
				if (inspos.y > 0)
				{
					int temp = line[inspos.y - 1].txtbuf.size();
					line[inspos.y - 1].txtbuf.insert(temp, line[inspos.y].txtbuf.data(), line[inspos.y].txtbuf.size());

					line[inspos.y].txtbuf.erase();
					line.erase(line.begin() + inspos.y);
					inspos.y--;
					inspos.x = temp;
					SetContentSize(GetWidth() - 10, line.size() * nLineHeight + (int)GetDisplayHeigth() % nLineHeight);
					if (-GetScrolloffsY() > inspos.y*nLineHeight)
					{
						ScrollToPosition(vert_bar, -(-GetScrolloffsY() - nLineHeight));
					}
					cur_undo.text.push_back(0x0a);//
				}
				cur_undo.ins_end.x = inspos.x;
				cur_undo.ins_end.y = inspos.y;
				return;
			}
			else
			{
				cur_undo.text.push_back(line[inspos.y].txtbuf[inspos.x - 1]);
				line[inspos.y].txtbuf.erase(inspos.x - 1, 1);
				inspos.x--;
				SkPaint paint;
				SkRect bounds;
				int ret;
				if (inspos.x > 0)
				{
					for (;;)
					{

						ret = font.measureText(line[inspos.y].txtbuf.data(), inspos.x, SkTextEncoding::kUTF8, &bounds, &paint);
						if (ret > 0 || inspos.x == 0)
							break;
						cur_undo.text.push_back(line[inspos.y].txtbuf[inspos.x - 1]);
						line[inspos.y].txtbuf.erase(inspos.x - 1, 1);
						inspos.x--;
					}
				}
				cur_undo.ins_end.x = inspos.x;
				cur_undo.ins_end.y = inspos.y;
			}
		
			
		}
	}
    
    
}

void TextField::ResetCurUndo()
{
	cur_undo.text.clear();
	cur_undo.state = UndoState::None;
	memset(&cur_undo.ins_begin, 0x00, sizeof(TextPoint));
	memset(&cur_undo.ins_end, 0x00, sizeof(TextPoint));
}

void TextField::CheckUndo()
{
	if (cur_undo.text.size() > 0)
	{
		undo.push(cur_undo);
		ResetCurUndo();
	}
}
//插入文字
void TextField::insertChar(SkUnichar c)
{
	if (c == (SkUnichar)skui::Key::kBack)
		return;
	if (!(cur_undo.ins_end.x == inspos.x && cur_undo.ins_end.y == inspos.y) && cur_undo.state!= UndoState::None)
	{
		undo.push(cur_undo);
		ResetCurUndo();
	}
	if (cur_undo.text.size() == 0)
	{
		//cur_undo.state = UndoState::Inster;
		cur_undo.ins_begin.x = inspos.x;
		cur_undo.ins_begin.y = inspos.y;
	}
	

	if (line.size() == 0)
	{
		textline info;
		info.nHeight = nLineHeight;
		line.push_back(info);
	}
	if (c == 0x0d)//回车键
	{
		//不在行的尾部按回车，要将一行分割成二行
		if (inspos.x < line[inspos.y].txtbuf.size())
		{
			textline info;
			info.nHeight = nLineHeight;
			/*char *pText = line[inspos.y].txtbuf.data() + line[inspos.y].txtbuf.size() - inspos.x;
			for (int k = 0; k < line[inspos.y].txtbuf.size() - inspos.x; k++)
			{
				info.txtbuf.push_back(pText[k]);
			}*/
			info.txtbuf = line[inspos.y].txtbuf.substr(inspos.x, line[inspos.y].txtbuf.size() - inspos.x);
			line[inspos.y].txtbuf.resize(inspos.x);
			line.insert(line.begin() + inspos.y + 1, info);
		}
		else
		{
			//在当前位置插入新的一行
			textline info;
			info.nHeight = nLineHeight;
			line.insert(line.begin() + inspos.y+1, info);

		}
		inspos.y++;
		inspos.x = 0;
		SetContentSize(GetWidth()-10, line.size() * nLineHeight+/*(int)GetDisplayHeigth()%*/ nLineHeight);
		if((inspos.y+1) * nLineHeight>= GetDisplayHeigth())
		  ScrollToPosition(vert_bar, -(-GetScrolloffsY()+ nLineHeight));
	}
	//中文字符 
	else if (c > 0x80)
	{
		chinachar[unindex] = (char)c;
	    unindex++;

	    if (unindex == 2) 
		{
	            //std::wstring wtxt = string2wstring(unchar, "Chinese");
	            //// ptext->push_back(wtxt[0]);
	            //ptext->insert(curpos.x, &wtxt[0], 1);
		
		   //一个中文字符占3个utf8字节
			char *pUtf=G2U(chinachar);
			//line[inspos.y].txtbuf.insert()
			//str.
			for (int k = 0; k < strlen(pUtf); k++)
			{
				line[inspos.y].txtbuf.insert(inspos.x, 1, pUtf[k]);
				inspos.x++;
			}
			if (GetCursorX() + 20 >= GetDisplayWidth())
			{
				ScrollToPosition(hori_bar, -(-GetScrolloffsX() + 60));
			}
			/*line[inspos.y].txtbuf.insert(inspos.x, 1, chinachar[0]);
			line[inspos.y].txtbuf.insert(inspos.x+1, 1, chinachar[1]);*/
	
			//inspos.x+=3;//一个中文字符占3个utf8字节
	     }
	     unindex = unindex % 2;


	 } 
	else
	{
	
		line[inspos.y].txtbuf.insert(inspos.x, 1, c);
		printf("%s\n", line[inspos.y].txtbuf.data());
		inspos.x++;
		//SetContentSize(GetWidth() - 10, line.size() * nLineHeight + (int)GetDisplayHeigth() % nLineHeight);
		if (GetCursorX()+20 >= GetDisplayWidth())
		{
			ScrollToPosition(hori_bar, -(-GetScrolloffsX() + 60));
		}
	}
	cur_undo.state = UndoState::Inster;
	cur_undo.text.push_back(c);
	cur_undo.ins_end.x = inspos.x;
	cur_undo.ins_end.y = inspos.y;
	
}

void  TextField::insertString(char *pBuf)
{
	std::string text;
	int nAddLine = 0;
	int nBufLen = strlen(pBuf);
	std::string temp;
	for (int k = 0; k < nBufLen; k++)
	{
		if (pBuf[k] == 0x0d || pBuf[k] == 0x0a)
		{
			if (nAddLine == 0)
			{
				if (line.size() == 0 || k == 0)
				{
					textline info;
					info.nHeight = nLineHeight;
					line.push_back(info);
				}
				line[inspos.y].txtbuf.insert(inspos.x, text.data(), text.size());
				temp = line[inspos.y].txtbuf.data() + text.size() + inspos.x;
				//line[inspos.y].txtbuf.resize(text.size() + inspos.x);
				line[inspos.y].txtbuf.erase(text.size() + inspos.x);
				inspos.x += text.size();
			}
			else
			{

				textline info;
				info.nHeight = nLineHeight;

				info.txtbuf = text;
				line.insert(line.begin() + inspos.y, info);
				//line.push_back(info);

				inspos.x = text.size();
			}

			text.clear();
			if (pBuf[k] == 0x0d /*&& pBuf[k + 1] == 0x0a*/)
				k++;
			if (k >= nBufLen)
				break;
			nAddLine++;
			inspos.y++;
		}
		if (pBuf[k] != 0x0d && pBuf[k] != 0x0a)
			text.insert(text.size(), 1, pBuf[k]);
	}
	if (text.size() > 0 && nAddLine == 0)
	{
		line[inspos.y].txtbuf.insert(inspos.x, text.data(), text.size());
		inspos.x += text.size();
	}
	else if (text.size() > 0 && nAddLine != 0 || temp.size() > 0)
	{
		text += temp;
		textline info;
		info.nHeight = nLineHeight;
		info.txtbuf = text;
		line.insert(line.begin() + inspos.y, info);
		inspos.x = text.size() - temp.size();
	}
	else if (text.size() == 0 && nAddLine != 0 && temp.size() == 0)
	{
		textline info;
		info.nHeight = nLineHeight;
		info.txtbuf = text;
		line.insert(line.begin() + inspos.y, info);
		inspos.x = text.size();
		//inspos.y++;
	}
}

void TextField::OnChar(SkUnichar c, uint32_t modifiers)
{
    /*CANDIDATEFORM exclude_rectangle = {0, CFS_EXCLUDE, {500, 500},
      {200, 100,500,200}};
        ImmSetCandidateWindow(imm_context, &exclude_rectangle);*/

   //ctrl-v   modifiers=16,c=22
	if (IsVisible() == false)
		return;
	using sknonstd::Any;
	skui::ModifierKey modi = (skui::ModifierKey)modifiers;

	modi &= ~skui::ModifierKey::kFirstPress;
	if (!Any(modi & (skui::ModifierKey::kControl |
		skui::ModifierKey::kOption |
		skui::ModifierKey::kCommand))) {
		if (/*((unsigned)c < 0x7F && */(unsigned)c >= 0x20 || c == 0x000A || c==0x000D || c==0x0009 ) {
			
			insertChar(c);
		}
		else if (c == 26)  //ctrl-z undo
		{
			if (undo.size() == 0 || cur_undo.state!= UndoState::None)
			{
				if (cur_undo.text.size() > 0)
				{
					undo.push(cur_undo);
					ResetCurUndo();
				}
			}
			if (undo.size() > 0)
			{
				undoInfo info=undo.top();
				if (info.state == UndoState::Inster)
				{
					selinfo.init.x = info.ins_begin.x;
					selinfo.init.y = info.ins_begin.y;
					selinfo.end.x = info.ins_end.x;
					selinfo.end.y = info.ins_end.y;
					TextSelDel();
				}
				else if (info.state == UndoState::Del)
				{
					std::reverse(info.text.begin(), info.text.end());
					char *pBuf = info.text.data();
					inspos.x = info.ins_end.x;
					inspos.y = info.ins_end.y;
					insertString(pBuf);
				}
				undo.pop();
			}
		}
		else if (c == 01) //ctrl-a  全选
		{
			if (line.size() > 0)
			{
				selinfo.init.x = 0;
				selinfo.init.y = 0;
				selinfo.end.x = line[line.size() - 1].txtbuf.size();
				selinfo.end.y = line.size()-1;
				bSelFlag = true;
				ScrollToPosition(vert_bar, -(ContentInfo.height - GetDisplayHeigth()));//滚动到尾部
				inspos.x = selinfo.end.x;
				inspos.y = selinfo.end.y;
			}

		}
		else if (c == 03) //ctrl-c 复制
		{
			if (bSelFlag == true)
			{
				TextPoint first, sec;
				if (selinfo.end.y > selinfo.init.y)
				{
					memcpy(&first, &selinfo.init, sizeof(TextPoint));
					memcpy(&sec, &selinfo.end, sizeof(TextPoint));
				}
				else if (selinfo.end.y < selinfo.init.y)
				{
					memcpy(&first, &selinfo.end, sizeof(TextPoint));
					memcpy(&sec, &selinfo.init, sizeof(TextPoint));
				}
				else if (selinfo.end.y == selinfo.init.y && selinfo.end.x > selinfo.init.x)
				{
					memcpy(&first, &selinfo.init, sizeof(TextPoint));
					memcpy(&sec, &selinfo.end, sizeof(TextPoint));
				}
				else if (selinfo.end.y == selinfo.init.y && selinfo.end.x == selinfo.init.x)
				{
					memcpy(&first, &selinfo.end, sizeof(TextPoint));
					memcpy(&sec, &selinfo.init, sizeof(TextPoint));
				}
				else if (selinfo.end.y == selinfo.init.y && selinfo.end.x < selinfo.init.x)
				{
					memcpy(&first, &selinfo.end, sizeof(TextPoint));
					memcpy(&sec, &selinfo.init, sizeof(TextPoint));
				}
				std::string cptext;
				char *pText;

				/*
				cur_undo.ins_begin.x = first.x;
				cur_undo.ins_begin.y = first.y;

				cur_undo.state = UndoState::Inster;
			
				cur_undo.text.push_back(c);*/

				for (int k = first.y; k <= sec.y; k++)
				{
					
					//将utf8转为ansi
					/*std::wstring un = Utf8ToUnicode(line[k].txtbuf.data());
					std::string an = UnicodeToANSI(un);
					pText = an.data();*/
					if (k == first.y)
					{
						std::string tt = line[k].txtbuf.data()+first.x;
						std::wstring un = Utf8ToUnicode(tt);
						std::string an = UnicodeToANSI(un);
						pText = an.data();
						cptext.insert(0, pText, strlen(pText));
					//	cur_undo.text.insert(0, pText, strlen(pText));
					}
					else if (k == sec.y)
					{
						std::string tt = line[k].txtbuf.data() ;
						tt.resize(sec.x);
						std::wstring un = Utf8ToUnicode(tt);
						std::string an = UnicodeToANSI(un);
						pText = an.data();
						cptext.insert(cptext.size(), pText, strlen(pText));
					}
					else
					{
						std::wstring un = Utf8ToUnicode(line[k].txtbuf.data());
						std::string an = UnicodeToANSI(un);
						pText = an.data();
						cptext.insert(cptext.size(), pText, strlen(pText));
					}
					if(first.y!=sec.y && k!=sec.y)
					   cptext.insert(cptext.size(),1, 0x0a);
				}
			/*	cur_undo.ins_end.x = sec.x;
				cur_undo.ins_end.y = sec.y;*/
				if (!OpenClipboard(NULL))
					return;
				EmptyClipboard();

				// Allocate a global memory object for the text.
				HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, cptext.size()+1);
				if (hglbCopy == NULL)
				{
					CloseClipboard();
					return;
				}

				// Lock the handle and copy the text to the buffer.
				unsigned char *lpucharCopy = (unsigned char *)GlobalLock(hglbCopy);
				memcpy(lpucharCopy, cptext.data(), cptext.size());
				lpucharCopy[cptext.size()] = 0;    // null character

				GlobalUnlock(hglbCopy);

				// Place the handle on the clipboard.
				SetClipboardData(CF_TEXT, hglbCopy);
				CloseClipboard();
			}
		}
		else if (c == 0x16)//ctrl-v
		{
			if (OpenClipboard(NULL))//打开剪贴板  
			{
				if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
				{
					HANDLE hClip;
					char* pBuf;

					//读取数据  
					hClip = GetClipboardData(CF_TEXT);
					char *pData = (char*)GlobalLock(hClip);
					pBuf = G2U(pData);
					
					std::string text;
					int nAddLine=0;
					int nBufLen = strlen(pBuf);

					if(cur_undo.state != UndoState::None)
			        {
			            undo.push(cur_undo);
			            ResetCurUndo();
			        }

					cur_undo.ins_begin.x = inspos.x;
					cur_undo.ins_begin.y = inspos.y;

					cur_undo.state = UndoState::Inster;

					
					cur_undo.text.insert(0,pBuf,strlen(pBuf));

					insertString(pBuf);
						
					
					cur_undo.ins_end.x = inspos.x;
					cur_undo.ins_end.y = inspos.y;
					delete pBuf;

					GlobalUnlock(hClip); 
					CloseClipboard();
				}
			}
		}
	}

}


SkScalar TextField::GetDisplayWidth()
{
	SkScalar width = GetBound().width();
	if (ContentInfo.height > GetBound().height() && vert_bar->IsVisible())
		width -= vert_bar->GetWidth();
	return width;
}
SkScalar TextField::GetDisplayHeigth()
{
	SkScalar heigth = GetBound().height();
	if (ContentInfo.width > GetBound().width()- nShowNumWidth && hori_bar->IsVisible())
		heigth -= hori_bar->GetHeight();
	return heigth;
}

void TextField::SetContentSize(SkScalar width, SkScalar height)
{
	ContentInfo.height = height;
	ContentInfo.width = width;
	UpdateScrollBarInfo();
}


void TextField::UpdateScrollBarInfo()
{
	if (ContentInfo.height > GetDisplayHeigth())
	{
		if (vert_bar->GetWidth() == 0)
		{
			vert_bar->SetPosition(GetBound().width() - BAR_VER_WIDTH + GetBound().left(), GetBound().top());
			vert_bar->SetSize(BAR_VER_WIDTH, GetDisplayHeigth());
		}
		else
		{
			vert_bar->SetPosition(GetBound().width() - vert_bar->GetBound().width() + GetBound().left(), GetBound().top());
			vert_bar->SetSize(vert_bar->GetBound().width(), GetDisplayHeigth());
		}

		ScrollBarInfo barinfo;
		barinfo.ContentSize = ContentInfo.height;
		barinfo.DisplaySize = vert_bar->GetHeight();
		barinfo.offset = ContentInfo.offs_y;
		SkScalar ContentMoveMax = barinfo.ContentSize - barinfo.DisplaySize;
		barinfo.offset = std::max(barinfo.offset, -ContentMoveMax);
		SetScrolloffsY(barinfo.offset);
		vert_bar->SetScrollBarInfo(barinfo);
	}
	else
	{
		vert_bar->SetBound(0, 0, 0, 0);
		SetScrolloffsY(0);
	}

	if (ContentInfo.width > GetDisplayWidth())
	{
		if (hori_bar->GetWidth() == 0)
		{
			hori_bar->SetPosition(GetBound().left(), GetBound().bottom() - BAR_HORI_HEIGHT);
			hori_bar->SetSize(GetDisplayWidth(), BAR_HORI_HEIGHT);
		}
		else
		{
			hori_bar->SetPosition(GetBound().left(), GetBound().bottom() - hori_bar->GetBound().height());
			hori_bar->SetSize(GetDisplayWidth(), hori_bar->GetBound().height());
		}
		ScrollBarInfo barinfo;
		barinfo.ContentSize = ContentInfo.width;
		barinfo.DisplaySize = hori_bar->GetWidth();
		barinfo.offset = ContentInfo.offs_x;

		SkScalar ContentMoveMax = barinfo.ContentSize - barinfo.DisplaySize;
		barinfo.offset = std::max(barinfo.offset, -ContentMoveMax);
		SetScrolloffsX(barinfo.offset);
		hori_bar->SetScrollBarInfo(barinfo);
	}
	else
	{
		hori_bar->SetBound(0, 0, 0, 0);
		SetScrolloffsX(0);
	}
}

void TextField::ScrollToPosition(ScrollBar* source, int position)
{
	if (source == NULL)
		return;
	if (source == vert_bar)
	{
		SkScalar pos_y = position;
		//printf("scroll before pos=%f\n",pos_y);
		pos_y = std::min((float)0, (float)pos_y);
		pos_y = std::max((float)(-(ContentInfo.height - GetDisplayHeigth())), (float)pos_y);
		//printf("scroll pos=%f\n",pos_y);
		SetScrolloffsY(pos_y);

	}

	if (source == hori_bar)
	{
		SkScalar pos_x = position;
		pos_x = std::min((float)0, (float)pos_x);
		pos_x = std::max((float)(-(ContentInfo.width - GetDisplayWidth())), (float)pos_x);
		SetScrolloffsX(pos_x);
	}
}


int  TextField::OnIMEMsg(HWND hwnd, unsigned int iMessage, unsigned int wParam, int lParam)
{
	if (IsVisible() == false)
		return true;
	if (iMessage == WM_IME_COMPOSITION)
	{
		//中文输入的时候，就重新设置输入法的光标位置。
		HIMC hImc = ::ImmGetContext(hwnd);
		if (hImc) {

			COMPOSITIONFORM cfs;
			cfs.dwStyle = CFS_POINT;
			cfs.ptCurrentPos.x = GetCursorX()+1+ GetBound().left();
			cfs.ptCurrentPos.y = GetCursorY()+ /*nLineHeight+5*/+GetBound().top();
			::ImmSetCompositionWindow(hImc, &cfs);
			::ImmReleaseContext(hwnd, hImc);
		}
	}
	return true;
}