#pragma once
#include "UIWidget.h"
//#include "GrContext.h"
#include "include/core/SkCanvas.h"
#include "ScrollView.h"
#include "button.h"
//#include "ListHead.h"

enum CellTypes {
  TEXT_ONLY = 0,
  ICON_AND_TEXT,
};


struct CellItem {
	UIWidget *pWidget;
	/*UIWidget *pWidget;
	CellTypes type;*/
};


struct ColumnInfo {
	int nInitOrder;
	int nShowOrder;
	int nWidth;
	SkString colname;
};

#define LIST_STYLE_HEAD        1
#define LIST_STYLE_VERTGLINE   2   
#define LIST_STYLE_HORZGLINE   4  
#define LIST_STYLE_SIGNLESELCELL  8
//#define LIST_STYLE_FULLLINE    8


class RowItem {
public:
//private:
	std::vector<CellItem> celllist;
	int nRowHeigth;
};

struct SortDescriptor
{
	int nCol=-1;
	bool ascending=false;
};

struct CellSelectedInfo 
{
	UIWidget *pWidget=0;
	int nRow=-1;
	int nCol=-1;
	SkColor color;
};

typedef std::function<void(CellSelectedInfo selcell,MouseEvent ev)> ListViewMouseEventCallBack;

class ListHead;
class ListView :public UIWidget ,public ScrollBarController 
{
public:

	enum ListAligment
	{
		LEFT,
		CENTER,
		RIGHT
	};
	ListView();
	~ListView();
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	bool OnMouseDown(int x, int y) override;
	bool OnMouseUp(int x,int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override;
    void OnKey(skui::Key key, uint32_t modifiers) override;
	void AddCol(ColumnInfo info);
	void AddCol(char *name,int nWidth);

	//void AddCellItem(SkString text);
	//void AddCellItem(UIWidget *pWidget);

	void AddCellItem(UIWidget *pWidget,int nRow,int nCol);

	void CellItemUpdate(UIWidget *pWidget,int nRow,int nCol);
	void CellItemUpdate(SkString text,int nRow,int nCol);

	CellSelectedInfo GetCellItemSelectedInfo(int x,int y);


	void DelRow(int nRow);
	void DelAllRow();

	void DelCol(int nCol);
	void DelAllCol();

	void SetListViewRow(int nRow);

	void ScrollToPosition(ScrollBar* source, int position);

	//void SetContentSize(SkScalar width, SkScalar height);
	void UpdateScrollBarInfo();

	void SetViewStyle(int nStyle);

	int GetViewStyle()
	{
		return nViewStyle;
	}
	ListAligment GetListAligment()
	{
		return nAligment;
	}

	SkScalar GetDisplayWidth();
	SkScalar GetDisplayHeigth();

	void QuiteSort(SortDescriptor desc);
	void SetSortCol(SortDescriptor desc);
	void SetSortCol(int nCol);
	void ReadySort(int nCol);

	SkPoint ScrollViewToChildPoint(int x, int y);


	void SetCellItemMouseEvent(ListViewMouseEventCallBack fun);

	void SetSelectedCellItemBackGround(SkColor color);

	std::vector<ColumnInfo> GetColList()
	{
		return collist;
	}
	//void AddColumn(ColumnInfo col);
private:
	ScrollContentInfo ContentInfo;
	ScrollBar *vert_bar;
	ScrollBar *hori_bar;

	ListHead *header;
	std::vector<UIWidget *> displaylist;

	std::vector<RowItem *> rowlist;
	std::vector<ColumnInfo> collist;

	double fDrawTime;

	std::vector<Button *> titbutlist;
	std::map<int,SortDescriptor> sortdescMap;
	int nViewStyle;
	ListAligment nAligment;
	CellSelectedInfo  sel_cellinfo;
	ListViewMouseEventCallBack mecallback;
};