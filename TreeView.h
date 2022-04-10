#include "UIWidget.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"
#include "ScrollView.h"
#include "Button.h"
//���Ϳؼ�

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
		int nBitmapId;      //λͼid
		bool bAllowExpand;  //�Ƿ�������չ
		bool bExpand;       //�Ƿ�չ��
		UIWidget *pWidget;
	};

	//���ڵ�
	struct node {
		node *next;
		node *child;
		TreeItem item;
	};

	//void AddBitmap(Sprite *sp);
	node *AllocTreeNode();
	node *AddTreeItem(node *parent,TreeItem item);//���treeitem


public:


	node *root;
	ScrollView *pScrollView;
};