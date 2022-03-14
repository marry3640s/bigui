#include "UIRoot.h"
#include <algorithm>
using namespace std;

//void UIRoot::AddWidget(UIWidget *pWidget, int nShowOrder)
//{
//	pWidget->nShowOrder = nShowOrder;
//	widgetlist.push_back(pWidget);
//	
//}

void UIRoot::DrawAllWidget(SkCanvas* canvas)
{
	
	sort(GetWidgetList()->begin(), GetWidgetList()->end(), [](UIWidget *x, UIWidget *y) {return x->nShowOrder < y->nShowOrder; });

	for (auto iter = GetWidgetList()->begin(); iter != GetWidgetList()->end(); iter++)
	{
		DrawWidget(canvas,*iter);
	}
	GetTimerManage()->UpdateAllGameTimer();
	GetActionManage()->UpdateAllAction();

}

void UIRoot::DrawWidget(SkCanvas* canvas,UIWidget *pWidget)
{
	if(pWidget->IsVisible())
	   pWidget->Draw(canvas);
}


void UIRoot::OnMouseDown(int x, int y)
{
	if (GetWidgetList()->size() == 0)
		return;
	for (auto iter = GetWidgetList()->end()-1; ;iter--)
	{
		UIWidget *pWidget = *iter;
		if (x >= pWidget->GetBound().left() && x <= pWidget->GetBound().right() && y >= pWidget->GetBound().top() && y <= pWidget->GetBound().bottom() && pWidget->IsVisible())
		{
			pWidget->OnMouseDown(x, y);
			//return;
		}
		if (iter == GetWidgetList()->begin())
			break;
	}
}


void UIRoot::OnMouseUp(int x, int y)
{
	if (GetWidgetList()->size() == 0)
		return;
	for (auto iter = GetWidgetList()->end()-1; ;iter--)
	{
		UIWidget *pWidget = *iter;
		//if (x >= pWidget->GetBound().left() && x <= pWidget->GetBound().right() && y >= pWidget->GetBound().top() && y <= pWidget->GetBound().bottom() && pWidget->IsVisible())
		//{
			pWidget->OnMouseUp(x, y);
			//return;
		//}
		if (iter == GetWidgetList()->begin())
			break;
	}
}


void  UIRoot::OnMouseMove(int x, int y)
{
	if (GetWidgetList()->size() == 0)
		return;
	for (auto iter = GetWidgetList()->end() - 1; ; iter--)
	{
		UIWidget *pWidget = *iter;
		pWidget->OnMouseMove(x, y);
	/*	if (x >= pWidget->GetBound().left() && x <= pWidget->GetBound().right() && y >= pWidget->GetBound().top() && y <= pWidget->GetBound().bottom())
			return;*/
		if (iter == GetWidgetList()->begin())
			break;
	}
}

void  UIRoot::OnMouseWheel(float delta, uint32_t modifier)
{
	if (GetWidgetList()->size() == 0)
		return;
	for (auto iter = GetWidgetList()->end() - 1; ; iter--)
	{
		UIWidget *pWidget = *iter;
		pWidget->OnMouseWheel(delta, modifier);
		/*if (x >= pWidget->GetBound().left() && x <= pWidget->GetBound().right() && y >= pWidget->GetBound().top() && y <= pWidget->GetBound().bottom())
			return;*/
		if (iter == GetWidgetList()->begin())
			break;
	}
}

void UIRoot::OnKey(skui::Key key, uint32_t modifiers)
{
	if ( GetWidgetList()->size() == 0)
		return;
	for (auto iter = GetWidgetList()->end() - 1; ; iter--)
	{
		UIWidget *pWidget = *iter;
		pWidget->OnKey(key, modifiers);
		/*if (x >= pWidget->GetBound().left() && x <= pWidget->GetBound().right() && y >= pWidget->GetBound().top() && y <= pWidget->GetBound().bottom())
			return;*/
		if (iter == GetWidgetList()->begin())
			break;
	}
}

void UIRoot::OnChar(SkUnichar c, uint32_t modifiers)
{
	if (GetWidgetList()->size() == 0)
		return;
	for (auto iter = GetWidgetList()->end() - 1; ; iter--)
	{
		UIWidget *pWidget = *iter;
		pWidget->OnChar(c, modifiers);
		if (iter == GetWidgetList()->begin())
			break;
	}
}
int UIRoot::OnIMEMsg(HWND hwnd, unsigned int iMessage, unsigned int wParam, int lParam)
{
	if (GetWidgetList()->size() == 0)
		return false;
	for (auto iter = GetWidgetList()->end() - 1; ; iter--)
	{
		UIWidget *pWidget = *iter;
		pWidget->OnIMEMsg(hwnd, iMessage,wParam,lParam);
		if (iter == GetWidgetList()->begin())
			break;
	}
	return true;
}