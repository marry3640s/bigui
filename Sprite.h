#pragma once
#include "UIWidget.h"
//#include "GrContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"
#include "src/core/SkAutoPixmapStorage.h"

//SkPoint


class Sprite :public UIWidget {
public:
	Sprite(const char *pImagePath);
	Sprite(sk_sp<SkImage> *pSpriteImage);
	~Sprite();

	void Draw(SkCanvas* canvas);
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;
	void OnMouseUp(int x, int y) override {};
	void OnMouseWheel(float delta, uint32_t modifier) override {};

	void SetScale(SkScalar scale);
	void SetScale(SkScalar sx, SkScalar sy);
	void SetScaleX(SkScalar sx);
	void SetScaleY(SkScalar sy);

	void SetAnchorPoint(SkPoint po);

	void SetOpacity(SkScalar op)
	{
		opacity = op;
	}

	Sprite *readPixels(const SkRect& subset,bool bRotated=false);

	sk_sp<SkImage> GetSkImage()
	{
		return SpriteImage;
	}
private:
	sk_sp<SkImage> SpriteImage;

	SkScalar opacity;
	SkPoint  anchor;
	SkCanvas *sp_canvas;
};
