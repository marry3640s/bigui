#pragma once


//#include "GrContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"
#include "Action.h"
#include "GameTimer.h"
#include "windows.h"
#include "..\\tools\\sk_app\\Window.h"

class Action;
class UIWidget;


enum MouseEvent{
	MOUSE_LBUTTONPRESS,
	MOUSE_RBUTTONPRESS,
	MOUSE_DOUBLECLICK,
	MOUSE_MOVE,
	MOUSE_LEAVE,
	MOUSE_WHEEL,
	MOUSE_NONE
};

typedef std::function<void(UIWidget *p,MouseEvent ev)> MouseCallBackFun;



extern  ActionManage *gActionManage;
extern  GameTimerManage *gTimerManage;
extern std::vector<UIWidget *> gWidgetList;
class UIShard {
public:
	UIShard();
	void RunAction(Action *type);
	void AddWidget(UIWidget *pWidget, int nShowOrder=0);
	void SetTimer(TimerCallBackFun fun, double fElapse);
	void KillTimer(TimerCallBackFun fun);
	void RemoveWidget(UIWidget *pWidget);
	ActionManage *GetActionManage()
	{
		return pActionManage;
	}
	GameTimerManage *GetTimerManage()
	{
		return pTimerManage;
	}
	std::vector<UIWidget *> * GetWidgetList()
	{
		return pWidgetList;
	}
private:
	ActionManage *pActionManage;
	GameTimerManage *pTimerManage;
	std::vector<UIWidget *> *pWidgetList;
};

class UIWidget :public UIShard{
public:
	UIWidget();
	virtual ~UIWidget() 
	{ 
		RemoveWidget(this); 
	};
	int nShowOrder;
	virtual void Draw(SkCanvas* canvas)=0;
	void SetPosition(SkScalar x, SkScalar y);
	void SetSize(SkScalar width, SkScalar height);
	void SetBound(SkScalar left, SkScalar top, SkScalar right, SkScalar bottom);

	void RunAction(Action *type);
	void StopAllAction();

//	void RunSequence(ActCallBackFun callback, ...);

	virtual void OnMouseDown(int x, int y)=0;
	virtual void OnMouseMove(int x, int y)=0;
	virtual void OnMouseUp(int x,int y)=0;
	virtual void OnMouseWheel(float delta, uint32_t modifier)=0;

	virtual void OnKey(skui::Key  key, uint32_t modifiers){};
	virtual void OnChar(SkUnichar c, uint32_t modifiers) {};

	virtual int  OnIMEMsg(HWND hwnd, unsigned int iMessage, unsigned int wParam, int lParam) { return true; };
	void SetMouseEventCallBack(MouseCallBackFun fu);
	SkScalar GetWidth()
	{
		return rect.width();
	}
	SkScalar GetHeight()
	{
		return rect.height();
	}

	inline SkRect GetBound()
	{
		return rect;
	}

	MouseCallBackFun GetMouseDownCallBack()
	{
		return callback_mouse;
	}

	void SetTag(int nSet)
	{
		nTag = nSet;
	}
	int GetTag()
	{
		return nTag;
	}
	bool IsVisible()
	{
		return bShowWindow;
	}
	void SetVisible(bool bVisible)
	{
		bShowWindow = bVisible;
	}

	SkScalar GetDegress()
	{
		return fDegress;
	}
	void SetDegress(SkScalar degress)
	{
		fDegress = degress;
	}

	SkScalar GetScrolloffsY()
	{
		return fScrolloffsY;
	}

	void SetScrolloffsY(SkScalar offs)
	{
		fScrolloffsY = offs;
	}

	SkScalar GetScrolloffsX()
	{
		return fScrolloffsX;
	}

	void SetScrolloffsX(SkScalar offs)
	{
		fScrolloffsX = offs;
	}


	void SetBackGroundColor(SkColor color)
	{
		background=color;
	}
	SkColor GetBackGroundColor()
	{
		return background;
	}
private:
	//SkPoint point;
	SkRect rect;
	MouseCallBackFun callback_mouse;
	int nTag;
	bool bShowWindow;
	SkScalar fDegress;

	SkScalar fScrolloffsY;
	SkScalar fScrolloffsX;
	SkColor background;
};