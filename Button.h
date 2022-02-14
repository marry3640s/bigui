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
	void OnMouseDown(int x, int y) override {};
	void OnMouseUp(int x, int y) override {};
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
		but_NormalStatu,
		but_PressedStatu,
		but_MouseStayStatu,
		but_DisabledStatu,
	};

	int GetButState()
	{
		return nButState;
	}

	SkString GetText()
	{
		return text;
	}
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;
	void OnMouseUp(int x, int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override {};
private:
	int nButState;
	SkString text;
};

class ButtonImage :public Button {
public:
	ButtonImage(const char *pNormalPath, const char *pPressedPath = 0, const char *pDisabledPath = 0);
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
