

#include "Button.h"

//#include "GrContext.h"
//#include "SkCanvas.h"
//#include "SkFont.h"
//#include "SkGradientShader.h"
//#include "SkGraphics.h"
//#include "SkImage.h"


ButtonImage::ButtonImage(const char *pNormalPath, const char *pPressedPath, const char *pDisabledPath)
{
	if (pNormalPath == NULL)
		return;
	SetButtonImageNormal(pNormalPath);
	SetButtonImagePressed(pPressedPath == 0 ? pNormalPath : pPressedPath);
	SetButtonImageDisabled(pDisabledPath == 0 ? pNormalPath : pDisabledPath);

}

void ButtonImage::SetButtonImageNormal(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	NormalImage = SkImage::MakeFromEncoded(blob);
	SetSize(NormalImage->width(), NormalImage->height());
}

void ButtonImage::SetButtonImagePressed(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	PressedImage = SkImage::MakeFromEncoded(blob);
}

void ButtonImage::SetButtonImageDisabled(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	DisabledImage = SkImage::MakeFromEncoded(blob);
}


void ButtonImage::SetNormalImage(sk_sp<SkImage> *pSpriteImage)
{
	NormalImage = *pSpriteImage;
	SetSize(NormalImage->width(), NormalImage->height());
}
void ButtonImage::SetPressedImage(sk_sp<SkImage> *pSpriteImage)
{
	PressedImage = *pSpriteImage;
}


void ButtonImage::Draw(SkCanvas* canvas)
{
	if (IsVisible() == false)
		return;
	if (GetBound().right() == 0 || GetBound().bottom() == 0)
	{
		SetSize(NormalImage->width(), NormalImage->height());
		SetSize(PressedImage->width(), PressedImage->height());
		SetSize(DisabledImage->width(), DisabledImage->height());
	}
	if (GetButStatu() == but_NormalStatu)
        canvas->drawImageRect(NormalImage.get(), GetBound(), SkSamplingOptions(), 0);
	else if (GetButStatu() == but_MouseStayStatu || GetButStatu() == but_PressedStatu)
        canvas->drawImageRect(PressedImage.get(), GetBound(), SkSamplingOptions(), 0);
	else if (GetButStatu() == but_DisabledStatu)
        canvas->drawImageRect(DisabledImage.get(), GetBound(), SkSamplingOptions(), 0);
}




Button::Button()
{
	SetButStatu(but_NormalStatu);
	SetPosition(0, 0);
	SetSize(0, 0);
	
}

void Button::SetText(const char *pText)
{
	text = pText;
}

void Button::SetEnable(bool bEnable)
{
	if (bEnable == false)
		SetButStatu(but_DisabledStatu);
	else
		SetButStatu(but_NormalStatu);
}



void Button::Draw(SkCanvas* canvas)
{
	if (IsVisible() == false)
		return;
	
	
	SkPaint paint;

	SkRect bounds;
	SkFont font;
	font.setSubpixel(true);
	font.setSize(16);
    font.measureText(text.c_str(), text.size(), SkTextEncoding::kUTF8, &bounds);
	if (GetButStatu() == but_NormalStatu)
	{
		canvas->drawSimpleText(text.c_str(), text.size(), SkTextEncoding::kUTF8, GetBound().left(), GetBound().top() - bounds.top(), font, paint);

	}
	if (GetButStatu() == but_MouseStayStatu || GetButStatu() == but_PressedStatu)
	{
		//paint.setColor(SK_ColorDKGRAY);
		paint.setColor(SK_ColorRED);
		canvas->drawSimpleText(text.c_str(), text.size(), SkTextEncoding::kUTF8, GetBound().left(), GetBound().top() - bounds.top(), font, paint);
	}
}

bool Button::OnMouseDown(int x, int y)
{
	if (GetButStatu() == but_DisabledStatu)
		return false;
	SetButStatu(but_PressedStatu);
	return false;
	/*if (GetMouseDownCallBack() != NULL)
		GetMouseDownCallBack()(this);*/
}


void Button::OnMouseMove(int x, int y)
{
	if (GetButStatu() == but_DisabledStatu)
		return;
	if (x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom())
	{
		if (GetButStatu() != but_PressedStatu)
			SetButStatu(but_MouseStayStatu);
	}
	else
		SetButStatu(but_NormalStatu);
}

bool Button::OnMouseUp(int x, int y)
{
	if (GetButStatu() == but_DisabledStatu)
		return false;
	if (x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom())
	{
		if (GetButStatu() != but_PressedStatu)
			SetButStatu(but_MouseStayStatu);
		if (GetMouseDownCallBack() != NULL && GetButStatu() == but_PressedStatu)
		{
			GetMouseDownCallBack()(this, MOUSE_LBUTTONPRESS);
			return true;
		}
	}
	else
		SetButStatu(but_NormalStatu);
	return false;

}

ButtonHaveClose::ButtonHaveClose()
{
	nButCloseStatu = closebut_none;
	for (int k = but_NormalStatu; k <= but_SetAndStayStatu; k++)
	{
		fonts_back[k].setColor(SkColorSetARGB(0xFF, 255, 255,255));
		fonts_text[k].setSubpixel(true);
		fonts_text[k].setSize(16);
	}
	fonts_back[but_SelStatu].setColor(SkColorSetARGB(0xFF, 0, 0, 0));
	fonts_back[but_SetAndStayStatu].setColor(SkColorSetARGB(0xFF, 0, 0, 0));

	stay_back.setColor(SkColorSetARGB(0xFF, 91, 113, 153));
	sel_back.setColor(SkColorSetARGB(0xFF, 255, 242, 157));
}

bool ButtonHaveClose::MouseInCloseButArea(int x, int y)
{
	if (x >= GetBound().right() - 15 && x <= GetBound().right() - 2 && y >= GetBound().top() + 3 && y <= GetBound().bottom() - 3)
	{
		return true;
	}
	return false;
}

void ButtonHaveClose::Draw(SkCanvas* canvas)
{
	if (IsVisible() == false)
		return;
	SkPaint paint;
	paint.setColor(GetBackGroundColor());
	canvas->drawRect(GetBound(), paint);
	SkRect bounds;
	fonts_text[GetButStatu()].measureText(GetText().c_str(), GetText().size(), SkTextEncoding::kUTF8, &bounds);

	//draw close but
	if (GetButStatu() == but_SelStatu || GetButStatu()== but_SetAndStayStatu)
	{
		canvas->drawRect(GetBound(), sel_back);
	}
	else if (GetButStatu() == but_MouseStayStatu)
	{
		canvas->drawRect(GetBound(), stay_back);
	}
	canvas->drawSimpleText(GetText().c_str(), GetText().size(), SkTextEncoding::kUTF8, GetBound().left()+1, GetBound().top() - bounds.top()+5, fonts_text[GetButStatu()], fonts_back[GetButStatu()]);
	if (nButCloseStatu == closebut_stay)
	{
		SkPaint aa;
		aa.setColor(SkColorSetARGB(0xFF, 255, 252, 244));
		canvas->drawRect({ GetBound().right() - 17,GetBound().top() + 4,GetBound().right() - 3,GetBound().bottom() - 4 }, aa);
	}

	if (GetButStatu() == but_SelStatu || GetButStatu() == but_SetAndStayStatu)
	{
		SkPaint closeSel_back;			
		closeSel_back.setColor(SkColorSetARGB(0xFF, 127, 99, 61));
		SkFont fontx;
		fontx.setSize(13);
		
		canvas->drawSimpleText("X", 1, SkTextEncoding::kUTF8, GetBound().right() - 15, GetBound().top() - bounds.top() + 5, fontx, closeSel_back);
	}
	else if (GetButStatu() == but_MouseStayStatu)
	{
		SkPaint closeSel_back;
		if(nButCloseStatu == closebut_stay)
			closeSel_back.setColor(SkColorSetARGB(0xFF, 0, 0, 0));
		else
		    closeSel_back.setColor(SkColorSetARGB(0xFF, 255, 255, 255));
		SkFont fontx;
		fontx.setSize(13);
		canvas->drawSimpleText("X", 1, SkTextEncoding::kUTF8, GetBound().right() - 15, GetBound().top() - bounds.top() + 5, fontx, closeSel_back);
	}
}
bool ButtonHaveClose::OnMouseDown(int x, int y)
{

	if (GetButStatu() == but_DisabledStatu)
		return false;
	if (x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom())
	{
		if (!MouseInCloseButArea(x, y))
		{
			//´¥·¢¶¯×÷
			downFunc(this);
			return true;
		}
	}
	return false;
}

void ButtonHaveClose::OnMouseMove(int x, int y)
{
	if (GetButStatu() == but_DisabledStatu)
		return;
	if (x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom())
	{
		if (GetButStatu() != but_PressedStatu)
		{
			if (GetButStatu() != but_SelStatu && GetButStatu() != but_SetAndStayStatu)
				SetButStatu(but_MouseStayStatu);
			else
				SetButStatu(but_SetAndStayStatu);
			if (MouseInCloseButArea(x, y))
			{
				nButCloseStatu = closebut_stay;
			}
			else
				nButCloseStatu = closebut_none;
		}
	}
	else
	{
		if(GetButStatu()!=but_SelStatu && GetButStatu() != but_SetAndStayStatu)
		  SetButStatu(but_NormalStatu);
		if(GetButStatu()== but_SetAndStayStatu)
			SetButStatu(but_SelStatu);
		nButCloseStatu = closebut_none;
	}
}

bool ButtonHaveClose::OnMouseUp(int x, int y)
{
	if (GetButStatu() == but_DisabledStatu)
		return false;
	if (x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom())
	{
		if (MouseInCloseButArea(x, y) && nButCloseStatu == closebut_stay)
		{
			closeFunc(this, GetTag());
			return true;
		}
	}
	return false;
}
StaticText::StaticText(char *pText)
{
	text = pText;
	SetPosition(0, 0);
	SetSize(0, 0);
}
StaticText::~StaticText()
{

}

void StaticText::SetText(const char *pText)
{
	text = pText;
}

void StaticText::Draw(SkCanvas* canvas)
{
	if (IsVisible() == false)
		return;
	SkFont font;
	font.setSubpixel(true);
	font.setSize(20);
	SkPaint paint;
	SkRect bounds;
    font.measureText(text.c_str(), text.size(), SkTextEncoding::kUTF8, &bounds);

	//SkPaint paint2;
	//paint2.setColor(GetBackGroundColor());
	//canvas->drawRect(GetBound(), paint2);
	//paint.setColor(SkColorSetRGB(38, 70, 10));

	int nDrawTextLength = text.size();
	if (bounds.width() > GetWidth())
		nDrawTextLength = (GetWidth()) / (bounds.width() / text.size()) - 1;
	nDrawTextLength = std::max(0, nDrawTextLength);
	canvas->drawSimpleText(text.c_str(), nDrawTextLength, SkTextEncoding::kUTF8, GetBound().left(), GetBound().top() - bounds.top(), font, paint);
}
