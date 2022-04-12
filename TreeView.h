#include "UIWidget.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"
#include "ScrollView.h"
#include "Button.h"
#include "Sprite.h"
//树型控件

class TreeView :public UIWidget {
public:
	TreeView();
	~TreeView() {};
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	bool OnMouseDown(int x, int y) override;
	bool OnMouseUp(int x, int y) override;
	void OnMouseWheel(float delta, uint32_t modifier);



	struct TreeItem {
		char *text;
		char *ptag;
		int  nBitmapId[2];          //位图id
		bool bAllowExpand;          //是否允许扩展
		bool bExpand;               //是否展开
		UIWidget *pWidget[6];       //0向前，1展开，2目录，3展开目录，4.文件或其他图标，5.名称或文件名
	};

	//树节点
	struct node {
		node *next;
		node *child;
		TreeItem item;
	};

	int AddBitmap(char *pBitmapPath);
	node *AllocTreeNode();
	node *AddTreeItem(node *parent,TreeItem item);//添加treeitem

	void AddItemToView(node *pp,TreeItem item);
	void TreeNodeButCallback(UIWidget* pWidget, MouseEvent ev);
	/*struct BitmapInfo {
		int nId;
		std::string path;
	};*/

	std::vector<std::string> bitmapinfo;


public:


	node *root;
	ScrollView *pScrollView;
	int nBitmapIndex;
	//ButtonImage *pButForward;
//	ButtonImage *pButExpland;
};