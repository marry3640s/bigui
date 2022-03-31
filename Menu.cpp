#include "Menu.h"

PopupMenu::PopupMenu()
{
	SetBackGroundColor(SkColorSetRGB(218, 218, 218));
	font.setSubpixel(true);
	font.setSize(18);
	font.setTypeface(SkTypeface::MakeFromName("simsun", SkFontStyle::Normal()));
	font_paint.setColor(SkColorSetRGB(0, 0, 0));

	text_height = 25;
	sep_height = 12;
}
void PopupMenu::Draw(SkCanvas* canvas)
{
	SkPaint paint;
	paint.setColor(GetBackGroundColor());

	int width = 0,height = 0;
	SkRect bounds;
	for (int k = 0; k < item.size(); k++)
	{
		if (item[k].sty == ItemStyle::text)
		{
			int fontwidth = font.measureText(item[k].name.data(), item[k].name.size(), SkTextEncoding::kUTF8, &bounds, &font_paint);
			width=std::max(width, fontwidth);
			height += text_height;
		}
		else if (item[k].sty == ItemStyle::separator)
			height += sep_height;
	}
	width += 50;
	height += text_height;

	SetBound((SkScalar)mouse_x, (SkScalar)mouse_y, (SkScalar)width + mouse_x, (SkScalar)height + mouse_y);
	canvas->drawRect(SkRect{ (SkScalar)mouse_x,(SkScalar)mouse_y,(SkScalar)width +mouse_x,(SkScalar)height +mouse_y }, paint);
	
	int off_y = 20,off_x=20;
	int ins_y = off_y +mouse_y;
	int ins_x = mouse_x+ off_x;

	for (int k = 0; k < item.size(); k++)
	{
		if (item[k].sty == ItemStyle::text)
		{
			if (item[k].nStatu == item_MouseStayStatu || item[k].nStatu == item_PressedStatu)
			{
				SkPaint pp;
				pp.setColor(SkColorSetRGB(100, 100, 100));
				
				canvas->drawRect(SkRect{ (SkScalar)mouse_x,(SkScalar)ins_y-off_y,(SkScalar)width + mouse_x,(SkScalar)ins_y - off_y+ text_height }, pp);
			}
			canvas->drawSimpleText(item[k].name.data(), item[k].name.size(), SkTextEncoding::kUTF8, ins_x, ins_y, font, font_paint);
			ins_y += text_height;
		}
		else if (item[k].sty == ItemStyle::separator)
		{
			paint.setColor(SkColorSetRGB(128, 128, 128));
			canvas->drawRect(SkRect{ (SkScalar)ins_x,(SkScalar)ins_y+8- off_y,(SkScalar)width+ins_x-off_x ,(SkScalar)ins_y+10- off_y }, paint);
			ins_y += sep_height;
			
		}
	}
}

PopupMenu::menuitem *PopupMenu::GetMenuItem(int x, int y)
{
	int scr_y = y-mouse_y ;
	int scr_x = x-mouse_x;
	int ins_y = 0;
	for (int k = 0; k < item.size(); k++)
	{
		if (item[k].sty == ItemStyle::text)
			ins_y += text_height;
		else if (item[k].sty == ItemStyle::separator)
			ins_y += sep_height;
		if ( ins_y>scr_y)
			return &item[k];
	}

	return 0;

}
void PopupMenu::OnMouseMove(int x, int y)
{
	if (IsVisible() == false)
		return;
	if (!(x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom()))
	{
		for (int k = 0; k < item.size(); k++)
		{
			if ( item[k].sty != ItemStyle::separator)
				item[k].nStatu = item_NormalStatu;
		}
		return;
	}
	menuitem *ptr = GetMenuItem(x, y);
	if (ptr != 0)
	{
		if (ptr->bEnable == false)
			return;
		if (ptr->nStatu != item_PressedStatu)
			ptr->nStatu = item_MouseStayStatu;
		for (int k = 0; k < item.size(); k++)
		{
			if (ptr != &item[k] && item[k].sty != ItemStyle::separator)
			{
				item[k].nStatu = item_NormalStatu;
			}
		}
	}
}

bool PopupMenu::OnMouseDown(int x, int y)
{
	if (IsVisible() == false)
		return false;
	if (!(x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom()))
		return false;
	menuitem *item = GetMenuItem(x, y);
	if (item != 0)
	{
		if (item->bEnable == false)
			return false;
	    item->nStatu = item_PressedStatu;
	}
	return false;
}

void PopupMenu::OnMouseRDown(int x, int y)
{
	SetVisible(true);
	mouse_x = x;
	mouse_y = y;
}

bool PopupMenu::OnMouseUp(int x, int y)
{

	if (IsVisible() == false)
		return false;
	if (!(x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom()))
		return false;
	menuitem *ptr = GetMenuItem(x, y);
	if (ptr != 0)
	{
		if (ptr->bEnable == false)
			return false;
		if (ptr->nStatu == item_PressedStatu)
		{
			if(ptr->func!=0)
			   ptr->func(this);
		}
		
	}

	SetVisible(false);
	return false;
}


void PopupMenu::AddMenuItem(char *pName , MenuItemCallBack callback)
{
	menuitem sub;
	sub.sty = ItemStyle::text;
	sub.name = pName;
	sub.func = callback;
	sub.bEnable = true;
	sub.nStatu = item_NormalStatu;
	item.push_back(sub);
}

void PopupMenu::AppendSeparator()
{
	menuitem sub;
	sub.sty = ItemStyle::separator;
	sub.name = "";
	sub.func = 0;
	sub.bEnable = false;
	sub.nStatu = item_DisabledStatu;
	item.push_back(sub);
}