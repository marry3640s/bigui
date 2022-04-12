/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

 //#include "HelloWorld.h"
#include "example/HelloWorld.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"
#include "include/core/SkSurface.h"
#include "include/core/SkTime.h"
#include "include/effects/SkDashPathEffect.h"
#include "include/effects/SkGradientShader.h"
#include "example/xml/tinyxml2.h"
//#include "windows.h"
#include "tools/sk_app/win/Window_win.h"
using namespace CharEncoding;
#include <imm.h>
#ifdef _MSC_VER
#pragma comment(lib, "imm32")
#endif
//HWND hwnd;
RollImage* roll;
using namespace sk_app;
using namespace tinyxml2;
TreeView *pTree;
ActionManage* gActionManage;
GameTimerManage* gTimerManage;
std::vector<UIWidget*> gWidgetList;
Application* Application::Create(int argc, char** argv, void* platformData) {
	gActionManage = new ActionManage();
	gTimerManage = new GameTimerManage();
	gWidgetList.clear();
	return new HelloWorld(argc, argv, platformData);

	// std::max();
}
HWND hwnd;
int nBitmapIds[5];
ListView* view;
SkPoint rightpoint[24] = { {490, 40},   {613, 40},   {736, 40},   {859, 40},   {982, 40},
						  {1105, 40},  {1105, 114}, {1105, 188}, {1105, 262}, {1105, 336},
						  {1105, 410}, {1105, 483}, {982, 483},  {859, 483},  {736, 483},
						  {613, 483},  {490, 483},  {367, 483},  {367, 410},  {367, 336},
						  {367, 262},  {367, 188},  {367, 114},  {367, 40} };
long long llLastStamp;










void HelloWorld::TestScrollView() {
	char pszTest[32][32] = { "very goods",
							"hello world",
							"miss",
							"SogouWBIpunt",
							"skscalar",
							"button",
							"client",
							"press",
							"oleacc",
							"winine.dll" };
	ScrollView* sview = new ScrollView();
	char pszPath[256];

	int nLine = 5000;
	for (int j = 0; j < nLine; j++) {
		for (int k = 0; k < 3; k++) {
			StaticText* text;
			if (k == 0) {
				sprintf_s(pszPath, 256, "%d", j + 1);
				text = new StaticText(pszPath);
			}
			else
				text = new StaticText(pszTest[k]);

			text->SetTag(j * 3 + k);
			// but->SetText(pszTest[k]);
			text->SetSize(100, 25);
			text->SetPosition(100 * k, j * 25);

			sview->AddChild(text);
		}
		// but->SetUiEventCallBack(std::bind(&HelloWorld::ClickCallback, this,
		// std::placeholders::_1));
	}

	// sview->SetDirection(ScrollView::Direction::Horizontal);
	sview->SetPosition(100, 200);
	sview->SetSize(580, 580);
	sview->SetContentSize(1024, nLine * 25);
	// sview->JumpBottom();
	this->AddWidget(sview);
}

void HelloWorld::TestListView() {
	Button* but = new Button();
	but->SetText("test listview");
	but->SetPosition(100, 50);
	but->SetSize(120, 25);
	this->AddWidget(but);
	but->SetMouseEventCallBack(std::bind(
		&HelloWorld::ClickCallback, this, std::placeholders::_1, std::placeholders::_2));

	char pszTest[32][32] = { "very goods",
							"hello world",
							"miss",
							"SogouWBIpunt",
							"skscalar",
							"button",
							"client",
							"press",
							"oleacc",
							"winine.dll" };

	char pszTitel[32][32] = { "num",
							 "probetcd_helloworld",
							 "memory",
							 "pid",
							 "cpu",
							 "username",
							 "statues",
							 "spec",
							 "odbc",
							 "myserver" };
	view = new ListView();
	for (int k = 0; k < 11; k++) view->AddCol(pszTitel[k % 11], 100);
	char pszPath[256];

	int nLine = 2000000;//671088-671125
	for (int j = 0; j < nLine; j++) {


		for (int k = 0; k < 11; k++) {
			StaticText* text;
			if (k == 0) {
				sprintf_s(pszPath, 256, "%d", j + 1);
				text = new StaticText(pszPath);

			}
			else
				text = new StaticText(pszTest[k % 11]);

			view->AddCellItem(text, j, k);
		}
		/*but->SetUiEventCallBack(std::bind(&HelloWorld::ClickCallback, this,
		 std::placeholders::_1));*/
	}

	// sview->SetDirection(ScrollView::Direction::Horizontal);
	view->SetPosition(100, 100);
	view->SetSize(500, 500);
	view->SetViewStyle(15);
	view->SetSelectedCellItemBackGround(SkColorSetRGB(85, 150, 150));
	view->SetSortCol(0);
	// sview->JumpBottom();
	this->AddWidget(view);
}
char* G2U2(const char* gb2312)
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

void HelloWorld::SplitDragCallback(UIWidget* pWidget, SkPoint po)
{
	int aa;
	aa = 5;
	pSplit->SetPosition((SkScalar)po.x(), 37);
}
void HelloWorld::AddTabCallback(UIWidget* pSubTab)
{
	/*int a;
	a = 5;*/
	TextField *pTextField = new TextField();
	pTextField->SetPosition(0, 37);
	pTextField->SetSize(fWindow->width()-200, fWindow->height() - 37);
	pTextField->SetTextFieldStyle(TextField::TextFieldStyle::multi_line | TextField::TextFieldStyle::show_linenum);
	this->AddWidget(pTextField);
	pTab->SetTabWidget(pSubTab, pTextField);

	
}


#pragma comment (lib, "User32.lib")

DWORD ListAllFileInDirectory(LPSTR szPath,TreeView::node *pNode)
{
	char szFilePath[MAX_PATH];
	WIN32_FIND_DATA FindFileData;
	HANDLE hListFile;
	char szFullPath[MAX_PATH];

	// 构造代表子目录和文件夹路径的字符串，使用通配符“*”
	lstrcpy(szFilePath, szPath);
	lstrcat(szFilePath, "\\*");

	// 查找第一个文件目录，获得查找句柄
	hListFile = FindFirstFile(szFilePath, &FindFileData);
	if (hListFile == INVALID_HANDLE_VALUE)
	{
		printf("错误： %d", GetLastError());
		return 1;
	}
	else
	{
		do {
			// 过滤"."和".."，不需要遍历
			if (lstrcmp(FindFileData.cFileName, TEXT(".")) == 0 ||
				lstrcmp(FindFileData.cFileName, TEXT("..")) == 0)
			{
				continue;
			}

			// 构造成全路径
			//wsprintf(szFullPath, "%s\\%s", szPath, FindFileData.cFileName);
			lstrcpy(szFullPath, szPath);
			lstrcat(szFullPath, "\\");
			lstrcat(szFullPath, FindFileData.cFileName);

			//dwTotalFileNum++;

			// 打印
		//	printf("\n%s\t", szFullPath);

			//AddTreeItem()
			// 如果是目录，则递归调用，列举下级目录
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
			//	printf("<DIR>");
				TreeView::TreeItem item;
				item.text = FindFileData.cFileName;
				item.bAllowExpand = true;
				item.bExpand = false;
				item.ptag = szFullPath;
				item.nBitmapId[0] = nBitmapIds[0];
				item.nBitmapId[1] = nBitmapIds[1];
				//item.text = szFullPath;
			

				TreeView::node *pp = pTree->AddTreeItem(pNode, item);
				ListAllFileInDirectory(szFullPath,pp);
			}
			else
			{
				TreeView::TreeItem item;
				item.text = FindFileData.cFileName;
				item.bAllowExpand = false;
				item.bExpand = false;
				item.ptag = szFullPath;
	
				item.nBitmapId[0] = nBitmapIds[2];
				item.nBitmapId[1] = nBitmapIds[2];
				pTree->AddTreeItem(pNode, item);
			}
		} while (FindNextFile(hListFile, &FindFileData));
	}
	return 0;
}
void HelloWorld::TestTextField() {

	//imeInteraction = imeWindowed;

	pTree = new TreeView();
	pTree->SetPosition(fWindow->width() - 194, 37);
	pTree->SetSize(194, fWindow->height() - 37);
	nBitmapIds[0]=pTree->AddBitmap("C:\\skia\\example\\icon\\floder.png");
	nBitmapIds[1] = pTree->AddBitmap("C:\\skia\\example\\icon\\openfloder.png");
	nBitmapIds[2] = pTree->AddBitmap("C:\\skia\\example\\icon\\files.png");
	//nBitmapIds[3] = pTree->AddBitmap("C:\\skia\\example\\icon\\forward.png");
	//nBitmapIds[4] = pTree->AddBitmap("C:\\skia\\example\\icon\\expland.png");
	this->AddWidget(pTree);
	TreeView::TreeItem item;
	char pszPath[] = "C:\\bighouse\\BestUI";
	item.text = pszPath;
	item.bAllowExpand = true;
	item.bExpand = true;
	item.ptag = pszPath;
	item.nBitmapId[0] = nBitmapIds[0];
	item.nBitmapId[1] = nBitmapIds[1];
	TreeView::node *pNode=pTree->AddTreeItem(0, item);
    ListAllFileInDirectory(pszPath, pNode);
	

	if (pSplit == NULL)
	{
		pSplit = new SplitView();
		pSplit->SetSize(6, fWindow->height() - 37);
		pSplit->SetPosition(fWindow->width() - 200, 37);
		pSplit->SetDragCallBack(std::bind(&HelloWorld::SplitDragCallback, this, std::placeholders::_1, std::placeholders::_2));
		this->AddWidget(pSplit, 10);
	}
	pTab = new TabBar();
	
	
	//this->AddWidget(pField);

	popMenu = new PopupMenu();
	popMenu->AddMenuItem("cut", 0);
	popMenu->AddMenuItem("copy", 0);
	popMenu->AddMenuItem("paste", 0);
	popMenu->AppendSeparator();
	popMenu->AddMenuItem("select all", std::bind(&HelloWorld::PopupMenuCallback, this, std::placeholders::_1));
	popMenu->AddMenuItem(G2U("打开目录"), std::bind(&HelloWorld::PopupMenuCallback, this, std::placeholders::_1));
	this->AddWidget(popMenu);
	popMenu->SetVisible(false);

	pTab->SetPosition(0, 0);
	pTab->SetSize(fWindow->width(), 37);
	pTab->SetTabStyle((int)(TabStyle::item_have_close | TabStyle::add_but));
	pTab->SetAddTabCallBack(std::bind(&HelloWorld::AddTabCallback, this, std::placeholders::_1));
	//int nTabId=pTab->AddTab("bbc");
	//int nTabId2 = pTab->AddTab("Button.cpp");
	//int nTabId3 = pTab->AddTab("HelloWorld.cpp");
	//pTab->SetTabWidget(nTabId,pField);
	this->AddWidget(pTab);


	return;

	FILE *fp;
	char str[8192];

	/* 打开用于读取的文件 */
	//fp = fopen("C:\\bighouse\\44.txt", "r");
	//fp = fopen("C:\\bighouse\\55.txt", "r");
	//fp = fopen("C:\\bighouse\\77.txt", "r");
	fp = fopen("C:\\bighouse\\老虎证券利率分析.txt", "r");
	//fp = fopen("C:\\bighouse\\22.txt", "r");
	//fp = fopen("C:\\bighouse\\66.txt", "r");
	if (fp == NULL) {
		return;
	}
	int k = 0;
	//644-650
	while(fgets(str, 8192, fp) != NULL /*&&k<60000*/) {
		/* 向标准输出 stdout 写入内容 */
		//puts(str);
		char *pText=G2U2(str);
		textline info;
		info.nHeight = TEXT_HEIGHT;
		info.txtbuf = pText;
		pField->insertline(info);
		delete pText;
		k++;
	}
	/*textline info;
	info.nHeight = TEXT_HEIGHT;
	info.txtbuf = "";
	pField->insertline(info);*/
	fclose(fp);
}

void HelloWorld::Init() {
	this->KillTimer(timer_sel(HelloWorld::Init));
	/*TcpClient *pClient = new TcpClient(this);
	pClient->Connect("192.168.0.226", 13056);*/
	// InitUi();
}

void HelloWorld::ClickCallback(UIWidget* pWidget, MouseEvent ev) {
}

void HelloWorld::PopupMenuCallback(UIWidget* pWidget)
{
	int a;
	a = 5;
}
HelloWorld::HelloWorld(int argc, char** argv, void* platformData)
	: fBackendType(Window::kNativeGL_BackendType), fRotationAngle(0) {
	SkGraphics::Init();
	fLinePaint.setColor(SK_ColorGRAY);
	fLinePaint.setAntiAlias(true);

	fRectPaint.setColor(DEFAULT_CURSOR_COLOR);
	fRectPaint.setStyle(SkPaint::kStroke_Style);
	fRectPaint.setStrokeWidth(2);
	fRectPaint.setAntiAlias(true);
	 fNextTime = -DBL_MAX;
	fXY = SkPoint::Make(0, 0);
	fSize = SkSize::Make(0, 0);
	fBlink = true;
	fWindow = Window::CreateNativeWindow(platformData);
	fWindow->setRequestedDisplayParams(DisplayParams());
	Window_win *ww = (Window_win*)fWindow;
	pField = NULL;
	pSplit = NULL;
	hwnd = ww->GetWindowHwnd();
	/*SetCapture(ww->GetWindowHwnd());*/
	/*Sequence sq = Sequence(0, [&]() {
		fBlink = !fBlink;
	}, new DelayTime(1.0), 0);

	Repeat re = Repeat(p, &se, 3, 0);
	sq.RunSequence();*/
	// Window_win* pwin = (Window_win*)fWindow;
	// hwnd=pwin->GetWindowHwnd();
	// register callbacks
	fWindow->pushLayer(this);

	fWindow->attach(fBackendType);

	
	// InitUi();
	 // TestScrollView();
	 // TestListView();
	TestTextField();

	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);

	return;

	// this->SetTimer(std::bind(&HelloWorld::TestTimer, this), 10);
	// TimerCallBackFun p2 = static_cast<TimerCallBackFun>(&HelloWorld::TestTimer);
	/*this->SetTimer(timer_sel(HelloWorld::TestTimer), 2);
	this->SetTimer(timer_sel(HelloWorld::TestTimer2), 5);*/

	// TestListView();

	/*Button *but = new Button();
	but->SetText(pszTest[0]);
	but->SetSize(80, 17);
	but->SetPosition(100, 100);
	this->AddWidget(but);
	but->SetUiEventCallBack(std::bind(&HelloWorld::ClickCallback, this, std::placeholders::_1));*/

	/*char pszPath[256];
	sprintf_s(pszPath, 256, "G:\\0.png");
	Sprite *p = new Sprite(pszPath);
	this->AddWidget(p);
	p->SetPosition(300, 300);

	p->SetAnchorPoint(SkPoint::Make(0.8, 0.8));
	RotateTo *ro = new RotateTo(2, 360);
	p->RunAction(ro);*/

	// Repeat re = Repeat(p, ro, 3, 0);
	/*MoveTo *to = new MoveTo(2, 800, 500);
	p->RunAction(to);*/
	// p->RunAction(new DelayTime(10.2, [&](void){
	//	  printf("kkkkk\n");
	//	//  this->KillTimer(timer_sel(HelloWorld::TestTimer));
	//   }
	//));

	// this->RunAction(new DelayTime(18, [&](void) {
	//	printf("jjjjj\n");
	//	this->KillTimer(timer_sel(HelloWorld::TestTimer2));
	// }
	//));

	// new Sequence(p, de, pBlink, 0);

	// Blink *pBlink = new Blink(2, 2);
	// DelayTime *de = new DelayTime(2);

	// Sequence se =  Sequence(p, [&](void) {
	//	printf("kkkkk222222\n");
	//	this->KillTimer(timer_sel(HelloWorld::TestTimer));
	//},pBlink,de,0);

	// Sequence se=Sequence(p,0,ro, de, pBlink,0);
	//   Repeat re = Repeat(p, &se, 3, 0);
	//	se.RunSequence();

	// p->RunAction(pBlink);

	//
	// p->RunAction(de);
	// p->SetOpacity(0.2);
	// p->SetScaleY(3);
}

HelloWorld::~HelloWorld() {
	fWindow->detach();
	delete fWindow;
}

void HelloWorld::updateTitle() {
	if (!fWindow || fWindow->sampleCount() <= 1) {
		return;
	}

	SkString title("Hello World ");
	title.append(Window::kRaster_BackendType == fBackendType ? "Raster" : "OpenGL");
	fWindow->setTitle(title.c_str());
}

void HelloWorld::onBackendCreated() {
	if (fWindow == 0) return;
	this->updateTitle();
	fWindow->show();
	fWindow->inval();
}
int nPaintTick = 0;
long long llInitStamp;
void HelloWorld::onPaint(SkSurface* surface) {
	auto canvas = surface->getCanvas();
	canvas->clear(SK_ColorWHITE);

	//if (fBlink) {
	//	canvas->drawRect(SkRect::MakeXYWH(fXY.fX+100, fXY.fY+100, DEFAULT_CURSOR_WIDTH, 20/*fSize.fHeight*/), fRectPaint);
	//}
	//else {
	//	//canvas->drawLine(fXY + xy, fXY + xy + SkPoint::Make(1, fSize.fHeight), fLinePaint);
	//}
	//return;

	DrawAllWidget(canvas);
	if (nPaintTick == 0) {
		llInitStamp = GetTickCount64();
	}
	if (nPaintTick % 60 == 0 && nPaintTick != 0) {
		long long lstamp = (GetTickCount64() - llInitStamp);
		//printf("time stamp=%lld,每秒能处理%d帧\n", lstamp, 60 * 1000 / lstamp);
		llInitStamp = GetTickCount64();
		// long long nPaintTick=GetTickCount64();
	}
	nPaintTick++;
	return;
}

void HelloWorld::onIdle() {
	double now = SkTime::GetNSecs();
	if (now >= fNextTime) {
		constexpr double kHalfPeriodNanoSeconds = 0.5 * 1e9;
		fNextTime = now + kHalfPeriodNanoSeconds;
		fBlink = !fBlink;
		fWindow->inval();
	}
	// Just re-paint continously
	fWindow->inval();
}

bool HelloWorld::onKey(skui::Key key, skui::InputState state, skui::ModifierKey modifiers) {
	if (state != skui::InputState::kDown) {
		return false;
	}
	OnKey(key, (uint32_t)modifiers);
	return true;
}

bool HelloWorld::onChar(SkUnichar c, skui::ModifierKey modifiers) {
	OnChar(c, (uint32_t)modifiers);
	return true;
}

// bool HelloWorld::onChar(SkUnichar c, skui::ModifierKey modifiers) {
//	if (' ' == c) {
//		fBackendType = Window::kRaster_BackendType == fBackendType ? Window::kNativeGL_BackendType
//			: Window::kRaster_BackendType;
//		fWindow->detach();
//		fWindow->attach(fBackendType);
//	}
//	return true;
//}

bool  HelloWorld::onRMouse(int x, int y, skui::InputState state, skui::ModifierKey modifiers)
{
	if (skui::InputState::kDown == state) {
		/*int a;
		a = 5;*/
		popMenu->OnMouseRDown(x, y);
	}
	else if (skui::InputState::kUp == state)
	{
	}
	return true;
}
bool HelloWorld::onMouse(int x, int y, skui::InputState state, skui::ModifierKey modifiers) {
	if (skui::InputState::kDown == state) {
		SetCapture(hwnd);
		OnMouseDown(x, y);
		
	}
	else if (skui::InputState::kMove == state) {
		OnMouseMove(x, y);
	}
	else if (skui::InputState::kUp == state) {
		OnMouseUp(x, y);
		ReleaseCapture();
		popMenu->SetVisible(false);
	}
	return true;
}

bool HelloWorld::onMouseWheel(float delta, skui::ModifierKey modifiers) {
	OnMouseWheel(delta, (uint32_t)modifiers);
	return true;
}

void HelloWorld::onResize(int width, int height)
{
	/*if (pField != 0)
	{
		pField->SetPosition(0, 0);
		pField->SetSize(width, height);
	}*/
}

int HelloWorld::onIMEMessage(unsigned int iMessage, unsigned int wParam, int lParam)
{
	OnIMEMsg(hwnd, iMessage, wParam, lParam);
	return ::DefWindowProc(hwnd, iMessage, wParam, lParam);
	
	
}