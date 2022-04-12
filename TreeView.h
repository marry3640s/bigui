#include "UIWidget.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/effects/SkGradientShader.h"
#include "include/core/SkGraphics.h"
#include "include/core/SkImage.h"
#include "ScrollView.h"
#include "Button.h"
#include "Sprite.h"
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
		int  nBitmapId[2];          //λͼid
		bool bAllowExpand;          //�Ƿ�������չ
		bool bExpand;               //�Ƿ�չ��
		UIWidget *pWidget[6];       //0��ǰ��1չ����2Ŀ¼��3չ��Ŀ¼��4.�ļ�������ͼ�꣬5.���ƻ��ļ���
	};

	//���ڵ�
	struct node {
		node *next;
		node *child;
		TreeItem item;
	};

	int AddBitmap(char *pBitmapPath);
	node *AllocTreeNode();
	node *AddTreeItem(node *parent,TreeItem item);//���treeitem

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