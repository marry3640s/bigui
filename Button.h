#pragma once

#include "UIWidget.h"
//#include "GrContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"

//SkPoint

class StaticText :public UIWidget {
public:
	StaticText(char *pText);
	~StaticText();
	SkString GetText() {
		return text;
	}
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override {};
	bool OnMouseDown(int x, int y) override { return false; };
	bool OnMouseUp(int x, int y) override { return false; };
	void OnMouseWheel(float delta, uint32_t modifier) override {};

	void SetText(const char *pText);
private:
	SkString text;

};


class Button :public UIWidget {
public:
	Button();
	~Button() {};
	void SetEnable(bool bEnable);
	void SetText(const char *pText);
	enum {
		but_NormalStatu=0,    //常规
		but_PressedStatu,     //按下
		but_MouseStayStatu,   //鼠标停留
		but_DisabledStatu,    //禁用
		but_SelStatu,         //选用
		but_SetAndStayStatu   //选用并停留
	};

	int GetButStatu()
	{
		return nButStatu;
	}

	

	void SetButStatu(int nStatu)
	{
		nButStatu = nStatu;
	}

	SkString GetText()
	{
		return text;
	}
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	bool OnMouseDown(int x, int y) override;
	bool OnMouseUp(int x, int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override {};

private:
	int nButStatu;
	SkString text;
	
	
};


typedef std::function<void(UIWidget *p,int nTag)> CloseButCallBack;
typedef std::function<void(UIWidget *p)> MouseDownCallBack;
class ButtonHaveClose :public Button
{
public:
	ButtonHaveClose();
	~ButtonHaveClose() {};
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	bool OnMouseDown(int x, int y) override;
	bool OnMouseUp(int x, int y) override;
	enum {
		closebut_none =0,
		closebut_stay , //关闭按钮停留
	};

	bool MouseInCloseButArea(int x, int y);//鼠标在关闭按钮区域
	void SetCloseButCallBack(CloseButCallBack fun)
	{
		closeFunc = fun;
	}
	void SetMouseDownCallBack(MouseDownCallBack fun)
	{
		downFunc = fun;
	}
private:
	SkFont  fonts_text[6];  //对应按钮的五种状态字体
	SkPaint fonts_back[6]; //对应按钮的五种状态的字体背景色

	SkPaint stay_back;
	SkPaint sel_back;
	int nButCloseStatu;

	CloseButCallBack closeFunc;
	MouseDownCallBack downFunc;
};

class ButtonImage :public Button {
public:
	ButtonImage(const char *pNormalPath=0, const char *pPressedPath = 0, const char *pDisabledPath = 0);
	~ButtonImage() {};
	void SetButtonImageNormal(const char *pImagePath);
	void SetButtonImagePressed(const char *pImagePath);
	void SetButtonImageDisabled(const char *pImagePath);

	void SetNormalImage(sk_sp<SkImage> *pSpriteImage);
	void SetPressedImage(sk_sp<SkImage> *pSpriteImage);

	void Draw(SkCanvas* canvas) override;

private:
	sk_sp<SkImage> NormalImage;
	sk_sp<SkImage> PressedImage;
	sk_sp<SkImage> DisabledImage;

};
