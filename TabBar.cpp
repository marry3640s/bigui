#include "TabBar.h"
#include "include/core/SkTextBlob.h"
#include "include/core/SkSurface.h"
TabBar::TabBar()
{
	nTabIdIndex = 0;
	nTabStyle = 0;
	SetBackGroundColor(SkColorSetARGB(0xFF, 41, 57, 85));

	font.setSubpixel(true);
	font.setSize(16);
	//font.setTypeface(SkTypeface::MakeFromName("simsun", SkFontStyle::Normal()));

	nSelTab = 0;
	//255,242,157 sel col
	//54,78,111
	//127,99,61 x selcol
	

	createLeftBut();
	createRightBut();

	nShowIndex = 0;
	AddTabFun = 0;
}

void TabBar::createLeftBut()
{
	auto surf = SkSurface::MakeRasterN32Premul(17, 17);
	SkPaint paint;
	paint.setColor(SkColorSetARGB(0xFF, 233, 231, 233));
	surf->getCanvas()->drawRect(SkRect{ 0,0,17,17 }, paint);
	SkPaint back;
	back.setColor(SkColorSetARGB(0xFF, 0, 0, 0));

	back.setAntiAlias(true); //ÎÞ¿¹¾â³Ý
	SkPath path;
	path.moveTo(4, 8);
	path.lineTo(11, 4);
	path.lineTo(11, 12);
	path.close();
	surf->getCanvas()->drawPath(path, back);
	sk_sp<SkImage> image(surf->makeImageSnapshot());

	left_but = new ButtonImage();
	left_but->SetNormalImage(&image);
	left_but->SetPressedImage(&image);

	left_but->SetPosition(0, 10);
	left_but->SetSize(17, 17);
	left_but->SetVisible(false);

	left_but->SetMouseEventCallBack(std::bind(&TabBar::LeftButCallback, this, std::placeholders::_1, std::placeholders::_2));
	this->AddWidget(left_but, 6);
}
void TabBar::createRightBut()
{
	auto surf = SkSurface::MakeRasterN32Premul(17, 17);
	SkPaint paint;
	paint.setColor(SkColorSetARGB(0xFF, 233, 231, 233));
	surf->getCanvas()->drawRect(SkRect{ 0,0,17,17 }, paint);
	SkPaint back;
	back.setColor(SkColorSetARGB(0xFF, 0, 0, 0));

	back.setAntiAlias(true); //ÎÞ¿¹¾â³Ý
	SkPath path;
	path.moveTo(11, 8);
	path.lineTo(4, 4);
	path.lineTo(4, 12);
	path.close();
	surf->getCanvas()->drawPath(path, back);
	sk_sp<SkImage> image(surf->makeImageSnapshot());

	right_but = new ButtonImage();
	right_but->SetNormalImage(&image);
	right_but->SetPressedImage(&image);

	right_but->SetPosition(0, 0);
	right_but->SetSize(17, 17);
	right_but->SetVisible(false);
	right_but->SetMouseEventCallBack(std::bind(&TabBar::RightButCallback, this, std::placeholders::_1, std::placeholders::_2));
	this->AddWidget(right_but, 6);
}

void TabBar::Draw(SkCanvas* canvas)
{
	if (IsVisible() == false)
		return;
	SkPaint paint;
	paint.setColor(GetBackGroundColor());
	canvas->drawRect(GetBound(), paint);
	int off_x = GetBound().left();
	int off_y = GetBound().top();
	SkPaint paint_text;

	int ins_y = 10+ GetBound().top();
	int ins_x = 2+GetBound().left();

	int nDisplayWidth = 0;
	bool bShowAllFlag = false;
	SkRect bounds;
	for (int k = 0; k < tablist.size(); k++)
	{
	//	sk_sp<SkTextBlob> blob = SkTextBlob::MakeFromString(tablist[k].but->GetText().c_str(), font);
		SkScalar font_width = font.measureText(tablist[k].but->GetText().c_str(), tablist[k].but->GetText().size(), SkTextEncoding::kUTF8, &bounds, &paint);
		SkScalar but_width = font_width + 35;
		nDisplayWidth += but_width;
	}
	if (nTabStyle & TabStyle::add_but)
	{
		nDisplayWidth += 8;
		nDisplayWidth += addbut_width_hei;
	}
	if (nDisplayWidth < GetBound().width())
	{
		nShowIndex = 0;
		bShowAllFlag = true;
	}
	
	for (int k = 0; k < tablist.size(); k++)
	{
		/*tablist[k].but->SetSize(0, 0);
		tablist[k].but->SetPosition(0, 0);*/
		tablist[k].but->SetVisible(false);
	}

	for (int k = nShowIndex; k < tablist.size(); k++)
	{
		//sk_sp<SkTextBlob> blob = SkTextBlob::MakeFromString(tablist[k].but->GetText().c_str() , font);
		SkScalar font_width = font.measureText(tablist[k].but->GetText().c_str(), tablist[k].but->GetText().size(), SkTextEncoding::kUTF8, &bounds, &paint);
		SkScalar but_width = font_width + 35;
		//SkScalar but_width = blob.get()->bounds().width() + 15;
		if (ins_x + but_width > GetBound().right() - addbut_width_hei - 8 - 40 && bShowAllFlag==false)
		{
			break;
		}
		tablist[k].but->SetSize(but_width, 25);
		tablist[k].but->SetPosition(ins_x, ins_y);
		tablist[k].but->SetVisible(true);
		ins_x += but_width;
		
	}
	

	if (nTabStyle & TabStyle::add_but)
	{
		add_but->SetPosition(ins_x + 8, ins_y);
	}
	if (nDisplayWidth > GetBound().width())
	{
		left_but->SetPosition(GetBound().right() - 38, 13 + GetBound().top());
		right_but->SetPosition(GetBound().right() - 20, 13 + GetBound().top());
		left_but->SetVisible(true);
		right_but->SetVisible(true);
	}
	else
	{
		left_but->SetVisible(false);
		right_but->SetVisible(false);
	}

}

void TabBar::OnMouseMove(int x, int y)
{
}

bool TabBar::OnMouseDown(int x, int y)
{
	return false;
}


bool TabBar::OnMouseUp(int x, int y)
{
	return false;
}

void TabBar::CloseButCallback(UIWidget* pWidget, int nTag)
{
	for (auto iter = tablist.begin(); iter != tablist.end();)
	{
		if (iter->but == pWidget)
		{
			delete pWidget;
			if (iter->pWidget != NULL)
				delete iter->pWidget;
			iter = tablist.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void TabBar::MouseDownCallback(UIWidget* pWidget)
{
	for (auto iter = tablist.begin(); iter != tablist.end();iter++)
	{
	
		if (iter->pWidget != NULL)
			iter->pWidget->SetVisible(false);
		if (iter->but->GetButStatu() != Button::but_DisabledStatu)
		{
			iter->but->SetButStatu(Button::but_NormalStatu);
			if (iter->but == pWidget)
			{
				iter->but->SetButStatu(Button::but_SelStatu);
				if (iter->pWidget != NULL)
				   iter->pWidget->SetVisible(true);
			}
		}
	}
}

void TabBar::LeftButCallback(UIWidget* pWidget, MouseEvent ev)
{
	//AddTab("new file...");
	nShowIndex--;
	nShowIndex = std::max(0, nShowIndex);
}

void TabBar::RightButCallback(UIWidget* pWidget, MouseEvent ev)
{
	//AddTab("new file...");
	int ins_x = 2 + GetBound().left();
	int k;
	for (k = nShowIndex; k < tablist.size(); k++)
	{
		sk_sp<SkTextBlob> blob = SkTextBlob::MakeFromString(tablist[k].but->GetText().c_str(), font);

		SkScalar but_width = blob.get()->bounds().width() + 15;
		if (ins_x + but_width > GetBound().right() - addbut_width_hei - 8 - 40)
		{
			nShowIndex++;
			break;
		}
		ins_x += but_width;

	}
	
}

void TabBar::AddButCallback(UIWidget* pWidget, MouseEvent ev)
{
	SkString str=SkStringPrintf("New File-%d", nTabIdIndex + 1);
	UIWidget *pSubTab=AddTab((char *)str.c_str());
	if (AddTabFun != 0)
		AddTabFun(pSubTab);
	
	for (int k = 0; k < tablist.size(); k++)
	{
		if (tablist[k].pWidget != NULL)
			tablist[k].pWidget->SetVisible(false);
	}
	MouseDownCallback(pSubTab);
}
UIWidget * TabBar::AddTab(char *pName)
{
	tabInfo info;
	nTabIdIndex++;

	ButtonHaveClose *but = new ButtonHaveClose();
	but->SetBackGroundColor(SkColorSetARGB(0xFF, 54, 78, 111));
	but->SetText(pName);
	but->SetPosition(0, 0);
	but->SetSize(0, 0);
	but->SetCloseButCallBack(std::bind(&TabBar::CloseButCallback, this, std::placeholders::_1, std::placeholders::_2));
	but->SetMouseDownCallBack(std::bind(&TabBar::MouseDownCallback, this, std::placeholders::_1));
	//but->SetTag()
	/*if (nTabIdIndex == 1)
		but->SetButStatu(Button::but_SelStatu);*/

	this->AddWidget(but,5);

	info.nTabId = nTabIdIndex;
	info.pWidget = NULL;
	info.but = but;

	tablist.push_back(info);
	return but;
}

void TabBar::SetTabWidget(UIWidget *pSubTab, UIWidget *pWidget)
{
	for (int k = 0; k < tablist.size(); k++)
	{
		if (pSubTab == tablist[k].but)
		{
			tablist[k].pWidget = pWidget;
		}
	}
}

void TabBar::SetTabStyle(int nStyle)
{
	nTabStyle = nStyle;
	if (nTabStyle & TabStyle::add_but)
	{
		auto surf = SkSurface::MakeRasterN32Premul(addbut_width_hei, addbut_width_hei);
		SkPaint paint;
		paint.setColor(GetBackGroundColor());
		surf->getCanvas()->drawRect(SkRect{ 0,0,addbut_width_hei,addbut_width_hei }, paint);
		SkPaint paint_add;
		paint_add.setStrokeWidth(2);
		paint_add.setColor(SkColorSetARGB(0xFF, 152, 106, 109));
		surf->getCanvas()->drawLine(12, 6, 12, 18, paint_add);
		surf->getCanvas()->drawLine(6, 12, 18, 12, paint_add);
		sk_sp<SkImage> image(surf->makeImageSnapshot());

		auto surf2 = SkSurface::MakeRasterN32Premul(addbut_width_hei, addbut_width_hei);
		SkPaint paint2;
		paint2.setColor(GetBackGroundColor());
		surf2->getCanvas()->drawRect(SkRect{ 0,0,addbut_width_hei,addbut_width_hei }, paint2);

		SkPaint pp;
		pp.setAntiAlias(true);
		pp.setColor(SkColorSetARGB(0xFF, 233, 233, 233));
		pp.setStrokeWidth(5);
		surf2->getCanvas()->drawCircle(addbut_width_hei /2, addbut_width_hei / 2, addbut_width_hei / 2, pp);
		//SkPaint paint_add;
		paint_add.setStrokeWidth(2);
		paint_add.setColor(SkColorSetARGB(0xFF, 102, 176, 129));
		surf2->getCanvas()->drawLine(12, 6, 12, 18, paint_add);
		surf2->getCanvas()->drawLine(6, 12, 18, 12, paint_add);
		sk_sp<SkImage> image2(surf2->makeImageSnapshot());

		add_but = new ButtonImage();
		add_but->SetNormalImage(&image);
		add_but->SetPressedImage(&image2);
		add_but->SetPosition(0, 0);
		add_but->SetSize(addbut_width_hei , addbut_width_hei );
		add_but->SetMouseEventCallBack(std::bind(&TabBar::AddButCallback, this, std::placeholders::_1, std::placeholders::_2));
		this->AddWidget(add_but, 5);
	}
}
