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
			if (pWidget->OnMouseDown(x, y) == true)
				break;
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
		
		if (pWidget->OnMouseUp(x, y) == true)
			break;
		
		if (iter == GetWidgetList()->begin())
			break;
	}
	/*for (auto iter = GetWidgetList()->begin(); iter != GetWidgetList()->end(); iter++)
	{
		UIWidget *pWidget = *iter;
		pWidget->OnMouseUp(x, y);
	}*/
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



namespace CharEncoding {
	char* G2U(const char* gb2312)
	{
		int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len + 1];
		memset(wstr, 0, len + 1);
		MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
		len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
		char* str = new char[len + 1];
		memset(str, 0, len + 1);
		WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
		if (wstr) delete[] wstr;
		return str;
	}

	std::string UnicodeToANSI(const std::wstring& str)
	{
		char*  pElementText;
		int    iTextLen;
		// 宽字节转多字节
		iTextLen = WideCharToMultiByte(CP_ACP, 0,
			str.c_str(),
			-1,
			nullptr,
			0,
			nullptr,
			nullptr);

		pElementText = new char[iTextLen + 1];
		memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
		::WideCharToMultiByte(CP_ACP,
			0,
			str.c_str(),
			-1,
			pElementText,
			iTextLen,
			nullptr,
			nullptr);

		std::string strText;
		strText = pElementText;
		delete[] pElementText;
		return strText;
	}

	std::wstring AnsiToUNICODE(const std::string& str)
	{
		wchar_t*  pElementText;
		int    iTextLen;
		// 宽字节转多字节
		iTextLen = MultiByteToWideChar(CP_ACP, 0,
			str.c_str(),
			-1,
			nullptr,
			0);

		pElementText = new wchar_t[iTextLen + 1];
		memset((void*)pElementText, 0, sizeof(char) * (iTextLen + 1));
		::MultiByteToWideChar(CP_ACP,
			0,
			str.c_str(),
			-1,
			pElementText,
			iTextLen);

		std::wstring strText;
		strText = pElementText;
		delete[] pElementText;
		return strText;
	}

	std::string UnicodeToUTF8(LPCWSTR lpszWideStr)
	{
		int nLen = ::WideCharToMultiByte(CP_UTF8, 0, lpszWideStr, -1,
			nullptr, 0, nullptr, nullptr);

		char* buffer = new char[nLen + 1];
		::ZeroMemory(buffer, nLen + 1);

		::WideCharToMultiByte(CP_UTF8, 0, lpszWideStr, -1,
			buffer, nLen, nullptr, nullptr);

		std::string multStr = buffer;
		delete[] buffer;
		return multStr;
	}

	std::wstring Utf8ToUnicode(const std::string& str)
	{
		int nLen = ::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(),
			nullptr, 0);

		WCHAR* buffer = new WCHAR[nLen + 1];
		::ZeroMemory(buffer, sizeof(WCHAR)* (nLen + 1));

		::MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(),
			buffer, nLen);

		std::wstring wideStr = buffer;
		delete[] buffer;
		return wideStr;
	}
}