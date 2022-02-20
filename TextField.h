#pragma once

#include <deque>
#include <stack>
//#include "GrContext.h"
#include "ScrollView.h"
//#include "SkCanvas.h"

struct TextDesc {
    SkColor color;
    SkUnichar text;
};

// struct

struct TextPoint {
    int x = 0;
    int y = 0;
};


struct SelTextInfo
{
	TextPoint init;
	TextPoint end;
};


struct textline {
	std::string  txtbuf;
	int nHeight;
};

//字体信息
struct FontInfoEach
{
	int nNextIndex;
	SkScalar total_width;
	SkScalar ch_width;
};

enum UndoState {
	None,
	Del,
	Inster
};
struct undoInfo {
	TextPoint ins_begin;
	TextPoint ins_end;
	std::string text;
	UndoState state; //删除
};

#define TEXT_HEIGHT 16
class TextField : public UIWidget , public ScrollBarController {
public:
    TextField();
	~TextField();

    void Draw(SkCanvas* canvas) override;
    void OnMouseMove(int x, int y) override;
    void OnMouseDown(int x, int y) override;
    void OnMouseUp(int x, int y) override;
    void OnMouseWheel(float delta, uint32_t modifier) override;
    void OnKey(skui::Key key, uint32_t modifiers) override;
    void OnChar(SkUnichar c, uint32_t modifiers) override;



    void DrawCursor(SkCanvas* canvas);
	SkScalar GetCursorX();//获取当前光标的X位置
	SkScalar GetCursorY();//获取当前光标的Y位置

    int GetMouseXCharOffset(int x);

	int DrawSelRect(SkCanvas* canvas,int nLine);

	FontInfoEach GetFontWidthForLineIndex(int nIndex);

	void insertChar(SkUnichar c);
	void insertline(textline);
	void TextSelDel();
	void PushCurUndeSelDel();

	void ResetCurUndo();
	void CheckUndo();

   // int FindSuitX(std::wstring text, int nCmpX);
   
	SkScalar GetDisplayWidth();
	SkScalar GetDisplayHeigth();
	void SetContentSize(SkScalar width, SkScalar height);
	void ScrollToPosition(ScrollBar* source, int position);
	void UpdateScrollBarInfo();
    SkPoint ScrollViewToChildPoint(int x, int y) {
        SkPoint point;
        point.set(x - GetBound().left(), y - GetBound().top());
        return point;
    }
	void SetMouseDragged(bool bValue) {
		bMouseDragged = bValue;
	}

	bool GetMouseDragged()
	{
		return bMouseDragged;
	}

private:
    // std::deque<TextDesc> textlist;
    ScrollBar* vert_bar;
    ScrollBar* hori_bar;
	ScrollContentInfo ContentInfo;
    SkColor text_color;

	SelTextInfo selinfo;
	
	undoInfo cur_undo;
	std::stack<undoInfo> undo;
	
    std::deque<textline> line;
    // std::wstring textlist;
    TextPoint inspos;//当前输入文本的位置

    int nLastX;

	char chinachar[3];
	int  unindex;

    double fCurPosBlinkTime;
	bool bBlink;
	SkFont font;
	double fNextTime;

	bool bMouseDragged;
	bool bSelFlag;
    // SkString
};

// DrawStringRectWithFlag