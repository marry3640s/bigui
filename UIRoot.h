#pragma once
#include "UIWidget.h"
#include <vector>

#include "Action.h"
#include "GameTimer.h"
#include "windows.h"
//#include "sk_app/Window.h"
class Action;


//class sk_app;
class UIRoot:public UIShard{
public:
	//void AddWidget(UIWidget *pWidget,int nShowOrder=0);
	void DrawAllWidget(SkCanvas* canvas);
	void DrawWidget(SkCanvas* canvas,UIWidget *pWidget);

	void OnMouseDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseUp(int x,int y);

	void OnMouseWheel(float delta, uint32_t modifiers);
    void OnKey(skui::Key key, uint32_t modifiers);
	void OnChar(SkUnichar c, uint32_t modifiers);

	int OnIMEMsg(HWND hwnd,unsigned int iMessage, unsigned int wParam, int lParam);
private:
	//std::vector<UIWidget *> widgetlist;
};
#include <iostream>
//#include <cstdlib>
//#include <string>
//#include <sstream>

namespace CharEncoding {
	char* G2U(const char* gb2312);
	std::string UnicodeToANSI(const std::wstring& str);
	std::wstring AnsiToUNICODE(const std::string& str);
	std::string UnicodeToUTF8(LPCWSTR lpszWideStr);
	std::wstring Utf8ToUnicode(const std::string& str);
}