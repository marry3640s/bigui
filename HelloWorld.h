/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef HelloWorld_DEFINED
#define HelloWorld_DEFINED

//#include "sk_app/Application.h"
//#include "sk_app/Window.h"
#include "UIRoot.h"
#include "Action.h"
#include "Button.h"
#include "ListView.h"
#include "RollImage.h"
#include "ScrollView.h"
#include "Sprite.h"
#include "TextField.h"
#include "AtlasCache.h"
#include "TextBMFont.h"
#include "PlistCache.h"
#include "Menu.h"
#include "TabBar.h"
#include "SplitView.h"
#include "TreeView.h"
#include "tools/sk_app/Application.h"
#include "tools/sk_app/Window.h"

class SkCanvas;






const SkColor DEFAULT_CURSOR_COLOR = SK_ColorGRAY;
const SkScalar DEFAULT_CURSOR_WIDTH = 2;
class HelloWorld : public sk_app::Application, sk_app::Window::Layer, public UIRoot {
public:
    HelloWorld(int argc, char** argv, void* platformData);
    ~HelloWorld() override;

    void onIdle() override;
    // skui::InputState
    void onBackendCreated() override;
    void onPaint(SkSurface* canvas) override;
    bool onChar(SkUnichar c, skui::ModifierKey) override;
    bool onKey(skui::Key, skui::InputState, skui::ModifierKey) override;
     void onResize(int width, int height) override;
  
    /*virtual bool onMouse(int x, int y, skui::InputState, skui::ModifierKey) { return false; }
    virtual bool onMouseWheel(float delta, skui::ModifierKey) { return false; }*/
    bool onMouse(int x, int y, skui::InputState state, skui::ModifierKey) override;
	bool onRMouse(int x, int y, skui::InputState state, skui::ModifierKey) override;
    bool onMouseWheel(int x, int y, float delta, skui::ModifierKey) override;

    int onIMEMessage(unsigned int nMsg, unsigned int wparam, int lparam) override;

	
    void ClickCallback(UIWidget* pWidget, MouseEvent ev);
	void PopupMenuCallback(UIWidget* pWidget);
	void AddTabCallback(UIWidget* pWidget);
	void SplitDragCallback(UIWidget* pWidget,SkPoint po);
	void TreeItemCallback(UIWidget* pWidget, TreeView::node *pNode);
	

    void Init();



    void TestTextField();
    void TestListView();
    void TestScrollView();

private:
    void updateTitle();
	enum IMEInteraction { imeWindowed, imeInline } imeInteraction;
    sk_app::Window* fWindow;
    sk_app::Window::BackendType fBackendType;

    SkScalar fRotationAngle;
    sk_sp<SkImage> image;
    PlistCache* plist_gui;



	SkPaint fLinePaint;
	SkPaint fRectPaint;
	SkPoint fXY;
	SkSize fSize;
	bool fBlink;
	double fNextTime;
	TextField* pField;
	PopupMenu *popMenu;
	SplitView *pSplit;
	TabBar *pTab;

	
	
};

#endif
