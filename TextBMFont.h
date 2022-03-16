#pragma once
#include "Sprite.h"

struct TextBMInfo
{
	int nCharId;
	SkScalar x;
	SkScalar y;
	SkScalar width;
	SkScalar height;
	SkScalar xoffset;
	SkScalar yoffset;
	SkScalar xadvance;
	Sprite *sptext;
};

class TextBMFont :public UIWidget {
public:
	TextBMFont(char *pFntFile);
	void Draw(SkCanvas* canvas);
	void SetText(char *pText);

	void OnMouseMove(int x, int y) override {};
	bool OnMouseDown(int x, int y) override { return false; };
	bool OnMouseUp(int x, int y) override { return false; };
	void OnMouseWheel(float delta, uint32_t modifier) override {};

	void SetScale(SkScalar scale);
	void SetScale(SkScalar sx, SkScalar sy);
	Sprite * GetSpriteByCharId(int nCharId);
	std::string  GetText()
	{
		return text;
	}
private:
	Sprite *texture_sp;
	std::map<int, TextBMInfo> charMap;
	std::string text;
};
