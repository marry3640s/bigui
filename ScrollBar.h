#include "UIWidget.h"
//#include "GrContext.h"
//#include "ScrollView.h"

enum Direction {
	Vertical,
	Horizontal
};

#define BAR_VER_WIDTH    10
#define BAR_HORI_HEIGHT  10

class ScrollBar;

class  ScrollBarController {
 public:
  virtual void ScrollToPosition(ScrollBar* source, int position) = 0;
  //virtual int GetScrollIncrement(ScrollBar* source,
  //                               bool is_page,
  //                               bool is_positive) = 0;
};


struct ScrollBarInfo{
	SkScalar ContentSize;
	SkScalar DisplaySize;
	SkScalar offset;
};
class ScrollBar:public UIWidget
{
public:
	enum ScrollAmount {
    SCROLL_NONE = 0,
    SCROLL_START,
    SCROLL_END,
    SCROLL_PREV_LINE,
    SCROLL_NEXT_LINE,
    SCROLL_PREV_PAGE,
    SCROLL_NEXT_PAGE,
   };

	enum ThumbStatu{
		NormalStatu,
		MouseStayStatu,
		MousePressedStatu,
		DisabledStatu,
	};

	ScrollBar(Direction dir);
	~ScrollBar();
	void SetScrollBarInfo(ScrollBarInfo info);

	//void SetOffset(SkScalar off)
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override ;
	bool OnMouseDown(int x, int y) override ;
	bool OnMouseUp(int x,int y) override;

	void OnMouseWheel(float delta, uint32_t modifier) override {};

	SkScalar GetThumbSize();
	SkScalar GetThumbPosition();

	void set_controller(ScrollBarController* controller) {
       scrollbar_controller = controller;
    }
	ScrollBarController* GetScrollBarController()  { 
		return scrollbar_controller; 
	}

	void SetMouseDragged(bool bValue){
		bMouseDragged=bValue;
	}

	bool GetMouseDragged()
	{
		return bMouseDragged;
	}
private:
	ScrollBarInfo barinfo;
	Direction nDirectionType;
	SkRect thumbrect;

	ThumbStatu thumbst;
	int mouse_offset;
	//int offset_reset;
	bool bMouseDragged;
    ScrollBarController* scrollbar_controller;

};