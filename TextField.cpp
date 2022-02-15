#include "TextField.h"
#include "windows.h"
#include "include/core/SkTextBlob.h"
#include "tools/gpu/GrContextFactory.h"

char* G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}

TextField::TextField() 
{
    SetBackGroundColor(SkColorSetRGB(230, 230, 230));
    text_color = SkColorSetRGB(0, 0, 0);
	memset(&ContentInfo, 0x00, sizeof(ContentInfo));
	vert_bar = new ScrollBar(Direction::Vertical);
    hori_bar = new ScrollBar(Direction::Horizontal);
    vert_bar->set_controller(this);
    hori_bar->set_controller(this);

    //imm_context = ::ImmGetContext(hwnd);
    //CANDIDATEFORM exclude_rectangle = {0, CFS_EXCLUDE, {500, 500}, {200, 100, 500, 200}};
    //ImmSetCandidateWindow(imm_context, &exclude_rectangle);
    unindex = 0;
	fNextTime = -DBL_MAX;
	SetContentSize(GetWidth(), line.size() * TEXT_HEIGHT);
    fCurPosBlinkTime = SkTime::GetMSecs();

	
	font.setSubpixel(true);
	font.setSize(18);
	font.setTypeface(SkTypeface::MakeFromName("simsun", SkFontStyle::Normal()));
	memset(&inspos, 0x00, sizeof(inspos));
	memset(chinachar, 0x00, sizeof(chinachar));
	bSelFlag = false;
	/*if (line.size() == 0)
	{
		textline info;
		info.nHeight = TEXT_HEIGHT;
		line.push_back(info);
	}*/
}
TextField::~TextField() 
{
}

void TextField::insertline(textline info)
{
	//info.nHeight = TEXT_HEIGHT;
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
	int ins_y =  nLine* TEXT_HEIGHT+ ContentInfo.offs_y;
	int ins_x = ContentInfo.offs_x;
	//printf("ins_x=%d\n", ins_x);
	SkPaint sel;
	sel.setColor(SK_ColorYELLOW);
	/*SkPaint sel;
	sel.setColor(SK_ColorYELLOW);
	surfaceCanvas->drawRect(SkRect{ 8, 0, 16, TEXT_HEIGHT }, sel);*/
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
			surfaceCanvas->drawRect(SkRect{ width+ ins_x, (SkScalar)ins_y, nextwidth+ ins_x, (SkScalar)ins_y + TEXT_HEIGHT }, sel);
		}
		////向上滚动
		else if (first.y > sec.y)
		{
			SkScalar width = font.measureText(text, first.x, SkTextEncoding::kUTF8, &bounds, &paint);
			nextwidth = font.measureText(text, first.x , SkTextEncoding::kUTF8, &bounds, &paint);
			surfaceCanvas->drawRect(SkRect{ (SkScalar)(0 + ins_x), (SkScalar)ins_y, nextwidth + ins_x, (SkScalar)ins_y + TEXT_HEIGHT }, sel);
		}
		////向下滚动
		else if (first.y < sec.y)
		{
			SkScalar width = font.measureText(text, first.x, SkTextEncoding::kUTF8, &bounds, &paint);
			nextwidth = font.measureText(text, strlen(text), SkTextEncoding::kUTF8, &bounds, &paint);
			surfaceCanvas->drawRect(SkRect{ width + ins_x, (SkScalar)ins_y, nextwidth + ins_x, (SkScalar)ins_y + TEXT_HEIGHT }, sel);
		}
	}
	else if ((nLine > first.y && nLine < sec.y) || (nLine>sec.y && nLine<first.y))
	{
		nextwidth = font.measureText(text, strlen(text), SkTextEncoding::kUTF8, &bounds, &paint);
		surfaceCanvas->drawRect(SkRect{ (SkScalar)(0 + ins_x), (SkScalar)ins_y, nextwidth + ins_x, (SkScalar)ins_y + TEXT_HEIGHT }, sel);
	}
	else if (sec.y==nLine)
	{
		////向上滚动
		if (first.y > sec.y)
		{
			SkScalar width = font.measureText(text, sec.x, SkTextEncoding::kUTF8, &bounds, &paint);
			nextwidth = font.measureText(text, strlen(text), SkTextEncoding::kUTF8, &bounds, &paint);
			//SkScalar width = font.measureText(text, sec.x, SkTextEncoding::kUTF8, &bounds, &paint);;
			surfaceCanvas->drawRect(SkRect{ width + ins_x, (SkScalar)ins_y, nextwidth + ins_x, (SkScalar)ins_y + TEXT_HEIGHT }, sel);
		}
		////向下滚动
		else if (first.y < sec.y)
		{
			SkScalar width = font.measureText(text, sec.x, SkTextEncoding::kUTF8, &bounds, &paint);;
			surfaceCanvas->drawRect(SkRect{ (SkScalar)(0 + ins_x), (SkScalar)ins_y, width + ins_x, (SkScalar)ins_y + TEXT_HEIGHT }, sel);
		}
	}


	

	return true;
}

void TextField::Draw(SkCanvas* canvas) 
{
	ContentInfo.offs_y = GetScrolloffsY();
	SkScalar diff_y = ContentInfo.offs_y - ContentInfo.preoffs_y;
	ContentInfo.offs_x = GetScrolloffsX();
	SkScalar diff_x = ContentInfo.offs_x - ContentInfo.preoffs_x;
   // sk_sp<GrContext> context = GrContext::MakeGL(interface);
   // GrContext* context = canvas->getGrContext();
    auto context = canvas->recordingContext();
    SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
    auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
    auto surfaceCanvas = gpuSurface->getCanvas();
    SkPaint paint;
    paint.setColor(GetBackGroundColor());
    surfaceCanvas->drawRect(SkRect{0, 0, GetWidth(), GetHeight()}, paint);

   
	SkPaint sel;
	sel.setColor(SK_ColorYELLOW);
	//surfaceCanvas->drawRect(SkRect{ -28, 0,-20, TEXT_HEIGHT }, sel);
	int nIndex = (-ContentInfo.offs_y) / TEXT_HEIGHT;

	int ins_y = TEXT_HEIGHT- (-ContentInfo.offs_y)% TEXT_HEIGHT;
	int ins_x = ContentInfo.offs_x;
	
	///测试显示中文代码
	/*SkString str = SkStringPrintf("Test text. 一二三四五六七八九十");
	const char *text = str.c_str();
	paint.setColor(SkColorSetRGB(0, 0, 0));
	surfaceCanvas->drawSimpleText(text, strlen(text), SkTextEncoding::kUTF8,
		ins_x, ins_y, font, paint);*/
	SkRect bounds;
	SkScalar fontwidth = 0;
	SkScalar fDisplayWidth = 0;
	for (int k = nIndex; k < line.size(); k++)
	{
		//if (GetMouseDragged())
		DrawSelRect(surfaceCanvas, k);
		paint.setColor(SkColorSetRGB(0, 0, 0));
		//char *text = G2U(line[k].txtbuf.data());
		char *text = line[k].txtbuf.data();
		//if (text != 0)
		{
			surfaceCanvas->drawSimpleText(text, strlen(text), SkTextEncoding::kUTF8,
				ins_x, ins_y, font, paint);
			fontwidth =font.measureText(text, strlen(text), SkTextEncoding::kUTF8, &bounds, &paint);
			fDisplayWidth = std::max(fDisplayWidth, fontwidth);
		}
		//delete text;
		ins_y += line[k].nHeight;
		if (ins_y > GetDisplayHeigth())
			break;
	}
	DrawCursor(surfaceCanvas);
	SetContentSize(fDisplayWidth+32, line.size() * TEXT_HEIGHT+/*(int)GetDisplayHeigth()% */TEXT_HEIGHT);
	/*int ins_y = 20;
	int ins_x = 0;
    for (int k = 0; k < line.size(); k++) 
	{
        paint.setColor(SkColorSetRGB(0, 0, 0));

        std::wstring* ptext = GetLineText(k);
        surfaceCanvas->drawSimpleText(ptext->c_str(), ptext->size() * 2, kUTF16_SkTextEncoding,ins_x, ins_y, font, paint);
        ins_y += line[k].nHeight;
    }*/

  //  DrawCurPosBlink(surfaceCanvas);
    /*Sequence sq = Sequence(0, [&]() {
        DrawCurPosBlink(surfaceCanvas);
    }, new DelayTime(1.0), 0);
    sq.RunSequence();*/

    paint.setColor(GetBackGroundColor());
    sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
    canvas->drawImage(image, GetBound().left(), GetBound().top());

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
	return inspos.y * TEXT_HEIGHT+ GetScrolloffsY();
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
	 	canvas->drawRect(SkRect::MakeXYWH(GetCursorX(), GetCursorY()+1, 1, TEXT_HEIGHT+1), CursorPaint);
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

		//selinfo.init
		bSelFlag = true;
		SkPoint point = ScrollViewToChildPoint(x, y);
		selinfo.end.y=  (point.y() + (-GetScrolloffsY())) / TEXT_HEIGHT;
		selinfo.end.y = std::max(0, selinfo.end.y);
		selinfo.end.y = std::min(selinfo.end.y, (int)line.size()-1);
		selinfo.end.x = GetMouseXCharOffset(point.x());
		inspos.x = selinfo.end.x;
		inspos.y = selinfo.end.y;
		printf("init:%d,%d end:%d,%d,x=%d,y=%d\n",selinfo.init.x,selinfo.init.y,selinfo.end.x,selinfo.end.y,x,y);

		if ((inspos.y+2)*TEXT_HEIGHT + GetScrolloffsY() > GetDisplayHeigth())
		{
			ScrollToPosition(vert_bar, -(-GetScrolloffsY() + TEXT_HEIGHT*3));
		}
		else if (inspos.y*TEXT_HEIGHT + GetScrolloffsY()<0)
		{
			ScrollToPosition(vert_bar, -(-GetScrolloffsY() - TEXT_HEIGHT * 3));
		}

		if (selinfo.end.y == selinfo.init.y)
		{
			printf("curX=%f,%f\n", GetCursorX(), GetScrolloffsX());
			if (GetCursorX()-30 < 0  )
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

void TextField::OnMouseDown(int x, int y) 
{

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

    if (point.y() > line.size() * TEXT_HEIGHT) return;

	//int nYIndex = (-GetScrolloffsY()) / TEXT_HEIGHT;
    int nIndex = (point.y()+ (-GetScrolloffsY())) / TEXT_HEIGHT ;
	if (nIndex >= line.size())
		return;
	
   
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
	
	
  
}

void TextField::OnMouseUp(int x, int y) 
{
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
	
}

void TextField::OnMouseWheel(float delta, uint32_t modifier) 
{
    if (vert_bar == NULL || !vert_bar->IsVisible()) 
		return;
    ScrollToPosition(vert_bar, GetScrolloffsY() + delta * 20);
}

void TextField::OnKey(skui::Key key, uint32_t modifiers) {
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
			if ( (inspos.y-1)*TEXT_HEIGHT > -GetScrolloffsY())
			{
				ScrollToPosition(vert_bar, -(-GetScrolloffsY() + TEXT_HEIGHT));
			}
		}
		else if (key == skui::Key::kUp)
		{
			inspos.y--;
			inspos.y = std::max(0, inspos.y);
			inspos.x =  line[inspos.y].txtbuf.size();
			if(-GetScrolloffsY() > inspos.y*TEXT_HEIGHT)
			{
				ScrollToPosition(vert_bar, -(-GetScrolloffsY() - TEXT_HEIGHT));
			}
		}
		else if (key == skui::Key::kLeft)
		{
			if (inspos.y == 0 && inspos.x == 0) return;
			if (inspos.x == 0)
			{
				inspos.y--;
				inspos.x = line[inspos.y].txtbuf.size();
				if (-GetScrolloffsY() > inspos.y*TEXT_HEIGHT)
				{
					ScrollToPosition(vert_bar, -(-GetScrolloffsY() - TEXT_HEIGHT));
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
				if ((inspos.y - 1)*TEXT_HEIGHT > -GetScrolloffsY())
				{
					ScrollToPosition(vert_bar, -(-GetScrolloffsY() + TEXT_HEIGHT));
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

			if (inspos.x == 0)
			{
				if (inspos.y > 0)
				{
					int temp = line[inspos.y - 1].txtbuf.size();
					line[inspos.y - 1].txtbuf.insert(temp, line[inspos.y].txtbuf.data(), line[inspos.y].txtbuf.size());

					line[inspos.y].txtbuf.erase();
					line.erase(line.begin() + inspos.y);
					inspos.y--;
					inspos.x = temp;
					SetContentSize(GetWidth() - 10, line.size() * TEXT_HEIGHT + (int)GetDisplayHeigth() % TEXT_HEIGHT);
					if (-GetScrolloffsY() > inspos.y*TEXT_HEIGHT)
					{
						ScrollToPosition(vert_bar, -(-GetScrolloffsY() - TEXT_HEIGHT));
					}
				}
				return;
			}
			else
			{
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
						line[inspos.y].txtbuf.erase(inspos.x - 1, 1);
						inspos.x--;
					}
				}
			}
			
		}
	}
    
    
}



//插入文字
void TextField::insertChar(SkUnichar c)
{
	if (c == (SkUnichar)skui::Key::kBack)
		return;
	if (line.size() == 0)
	{
		textline info;
		info.nHeight = TEXT_HEIGHT;
		line.push_back(info);
	}
	if (c == 0x0d)//回车键
	{
		//不在行的尾部按回车，要将一行分割成二行
		if (inspos.x < line[inspos.y].txtbuf.size())
		{
			textline info;
			info.nHeight = TEXT_HEIGHT;
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
			info.nHeight = TEXT_HEIGHT;
			line.insert(line.begin() + inspos.y+1, info);

		}
		inspos.y++;
		inspos.x = 0;
		SetContentSize(GetWidth()-10, line.size() * TEXT_HEIGHT+/*(int)GetDisplayHeigth()%*/ TEXT_HEIGHT);
		if((inspos.y+1) * TEXT_HEIGHT>= GetDisplayHeigth())
		  ScrollToPosition(vert_bar, -(-GetScrolloffsY()+ TEXT_HEIGHT));
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
		//SetContentSize(GetWidth() - 10, line.size() * TEXT_HEIGHT + (int)GetDisplayHeigth() % TEXT_HEIGHT);
		if (GetCursorX()+20 >= GetDisplayWidth())
		{
			ScrollToPosition(hori_bar, -(-GetScrolloffsX() + 60));
		}
	}
}


void TextField::OnChar(SkUnichar c, uint32_t modifiers)
{
    /*CANDIDATEFORM exclude_rectangle = {0, CFS_EXCLUDE, {500, 500},
      {200, 100,500,200}};
        ImmSetCandidateWindow(imm_context, &exclude_rectangle);*/

   //ctrl-v   modifiers=16,c=22

	using sknonstd::Any;
	skui::ModifierKey modi = (skui::ModifierKey)modifiers;

	modi &= ~skui::ModifierKey::kFirstPress;
	if (!Any(modi & (skui::ModifierKey::kControl |
		skui::ModifierKey::kOption |
		skui::ModifierKey::kCommand))) {
		if (/*((unsigned)c < 0x7F && */(unsigned)c >= 0x20 || c == 0x000A || c==0x000D || c==0x0009 ) {
			
			insertChar(c);
		}
		else if (c == 03) //ctrl-c
		{

		}
		else if (c == 16)//ctrl-v
		{

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
	if (ContentInfo.width > GetBound().width() && hori_bar->IsVisible())
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
