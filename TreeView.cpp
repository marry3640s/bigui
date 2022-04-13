#include "TreeView.h"





TreeView::TreeView()
{
	SetBackGroundColor(SkColorSetRGB(52, 72, 105));
	root = NULL;
	if (root == NULL)
	{
		root=AllocTreeNode();
	}
	pScrollView = new ScrollView();
	nBitmapIndex = 0;
	func = 0;
	

}

void NodeRec(TreeView::node *pNode, TreeView *pTree,int *pDepth,int *pWidth,int *pMaxWidth)
{
	TreeView::node *pp = pNode;
	for (;;)
	{
		if (pp->next == NULL)
			break;
		//printf("%s\n", pp->item.text);
		/*pp->item.pWidget->SetPosition((*pWidth) * 30+10, (*pDepth) * 25);
		pp->item.pWidget->SetSize(200, 25);
		pp->item.pWidget->SetVisible(true);*/
		//目录
		SkScalar off_y = pTree->pScrollView->GetScrolloffsY();
		SkScalar off_x = pTree->pScrollView->GetScrolloffsX();
		int xoff = 0;
		if (pp->item.bAllowExpand == true && pp->item.bExpand == false)
		{
			pp->item.pWidget[0]->SetPosition((*pWidth) * 30 + 10+ off_x,  (*pDepth) * 22+ off_y);
			pp->item.pWidget[0]->SetSize(16, 16);
			pp->item.pWidget[0]->SetVisible(true);

			pp->item.pWidget[2]->SetPosition((*pWidth) * 30 + 10+18 + off_x, (*pDepth) * 22 + off_y);
			pp->item.pWidget[2]->SetSize(16, 16);
			pp->item.pWidget[2]->SetVisible(true);
			xoff = 36;
		}
		else if (pp->item.bAllowExpand == true && pp->item.bExpand == true)
		{
			pp->item.pWidget[1]->SetPosition((*pWidth) * 30 + 10 + off_x, (*pDepth) * 22 + off_y);
			pp->item.pWidget[1]->SetSize(16, 16);
			pp->item.pWidget[1]->SetVisible(true);

			pp->item.pWidget[3]->SetPosition((*pWidth) * 30 + 10 + 18 + off_x, (*pDepth) * 22 + off_y);
			pp->item.pWidget[3]->SetSize(16, 16);
			pp->item.pWidget[3]->SetVisible(true);
			xoff = 36;
		}
		//文件
		else
		{
			pp->item.pWidget[4]->SetPosition((*pWidth) * 30 + 10 + off_x, (*pDepth) * 22 + off_y);
			pp->item.pWidget[4]->SetSize(16, 16);
			pp->item.pWidget[4]->SetVisible(true);
			xoff = 18;
		}
		pp->item.pWidget[5]->SetPosition((*pWidth) * 30 + 10+xoff + off_x, (*pDepth) * 22+2 + off_y);
		pp->item.pWidget[5]->SetSize(200, 22);
		pp->item.pWidget[5]->SetVisible(true);
		(*pDepth)++;
		//pTree->pScrollView
		//pp->item.pWidget->SetPosition((*pWidth)*50, (*pDepth) * 25);
		
		if (pp->child != NULL)
		{
			if (pp->item.bAllowExpand == true && pp->item.bExpand == true)
			{
				(*pWidth)++;
				*pMaxWidth = std::max((*pWidth), 0);
				NodeRec(pp->child, pTree, pDepth,pWidth, pMaxWidth);
				(*pWidth)--;
				
			}
		}
		
		pp = pp->next;
	}
}
int kk = 0;
void TreeView::Draw(SkCanvas* canvas)
{
	
	/*SkPaint paint;
	paint.setColor(GetBackGroundColor());*/
	int nDepth = 0, nWidth = 0,nMaxWidth=0;
	
	if (kk == 0)
	{
		NodeRec(root, this, &nDepth, &nWidth,&nMaxWidth);
		pScrollView->SetContentSize(nMaxWidth * 50 + 30 + 100, nDepth * 22);
	}

	kk++;
	
	pScrollView->SetPosition(GetBound().left(), GetBound().top());
	pScrollView->SetSize(GetBound().width(), GetBound().height());
	//if (kk == 1)
	
	pScrollView->Draw(canvas);
}

void TreeView::OnMouseMove(int x, int y)
{
	if (IsVisible() == false)
		return;
	pScrollView->OnMouseMove(x, y);
}

bool TreeView::OnMouseDown(int x, int y)
{
	if (IsVisible() == false)
		return false;
	pScrollView->OnMouseDown(x, y);
	return false;
}


bool TreeView::OnMouseUp(int x, int y)
{
	if (IsVisible() == false)
		return false;
	pScrollView->OnMouseUp(x, y);
	return false;
}

void TreeView::OnMouseWheel(float delta, uint32_t modifier)
{
	if (IsVisible() == false)
		return ;
	pScrollView->OnMouseWheel(delta, modifier);
}


int TreeView::AddBitmap(char *pBitmapPath)
{
	int nRet = nBitmapIndex;
	//Sprite *sp = new Sprite(pBitmapPath);
	bitmapinfo.push_back(pBitmapPath);
	nBitmapIndex++;
	return nRet;
}

TreeView::node *TreeView::AllocTreeNode()
{
	int si = sizeof(node);
	node *pp = (node *)malloc(sizeof(node));
	pp->next = NULL;
	pp->child = NULL;
	return pp;
}

void NodeSetVisible(TreeView::node *pNode, TreeView *pTree)
{
	TreeView::node *pp = pNode;
	for (;;)
	{
		if (pp->next == NULL)
			break;
		/*if (pp->item.bAllowExpand == true && pp->item.bExpand == false)
		{
			
			pp->item.pWidget[0]->SetVisible(false);
			pp->item.pWidget[2]->SetVisible(false);
		
		}
		else if (pp->item.bAllowExpand == true && pp->item.bExpand == true)
		{
			pp->item.pWidget[1]->SetVisible(false);
			pp->item.pWidget[3]->SetVisible(false);
			
		}*/
		if (pp->item.bAllowExpand == true)
		{
			pp->item.pWidget[0]->SetVisible(false);
			pp->item.pWidget[2]->SetVisible(false);
			pp->item.pWidget[1]->SetVisible(false);
			pp->item.pWidget[3]->SetVisible(false);
		}

		else
		   pp->item.pWidget[4]->SetVisible(false);
		pp->item.pWidget[5]->SetVisible(false);
		if (pp->child != NULL)
		{
			if (pp->item.bAllowExpand == true && pp->item.bExpand == true)
			{
				NodeSetVisible(pp->child, pTree);
			}
		}
		pp = pp->next;
	}
}
void TreeView::TreeNodeButCallback(UIWidget* pWidget, MouseEvent ev)
{
	ButtonImage *but = (ButtonImage *)pWidget;
	node *pp=(node *)but->GetParam();
	int nDepth = 0, nWidth = 0,  nMaxWidth = 0;
	if (pp->item.bAllowExpand == true && pp->item.bExpand == true)
	{
		NodeSetVisible(pp, this);
		pp->item.pWidget[1]->SetVisible(false);
		pp->item.bExpand = false;

		NodeRec(root, this, &nDepth, &nWidth,&nMaxWidth);
		pScrollView->SetContentSize(nMaxWidth * 50 + 30 + 100, nDepth * 22);
	}
	else if (pp->item.bAllowExpand == true && pp->item.bExpand == false)
	{
		pp->item.pWidget[0]->SetVisible(false);
		pp->item.bExpand = true;

		NodeRec(root, this, &nDepth, &nWidth,&nMaxWidth);
		pScrollView->SetContentSize(nMaxWidth * 50 + 30 + 100, nDepth * 22);
	}

}
void TreeView::SubNodeCallback(UIWidget* pWidget, MouseEvent ev)
{
	Button *but = (Button *)pWidget;
	node *pp = (node *)but->GetParam();
	if (GetTreeItemCallback() != NULL)
	{
		GetTreeItemCallback()(pWidget,pp);
	}
	
}
void TreeView::AddItemToView(node *pp ,TreeItem item)
{
	//node *bb = AllocTreeNode();

	pp->item.text = (char *)malloc(strlen(item.text) + 1);
	memset(pp->item.text, 0x00, strlen(item.text) + 1);
	memcpy(pp->item.text, item.text, strlen(item.text));

	pp->item.ptag = (char *)malloc(strlen(item.ptag) + 1);
	memset(pp->item.ptag, 0x00, strlen(item.ptag) + 1);
	memcpy(pp->item.ptag, item.ptag, strlen(item.ptag));
	pp->item.bAllowExpand = item.bAllowExpand;
	pp->item.bExpand = item.bExpand;

//	pp->item.nBitmapId = item.nBitmapId;
	Button *but = new Button();
	but->SetText(item.text);
	pp->item.pWidget[5] = but;
	but->SetVisible(false);
	but->SetParam(pp);
	but->SetMouseEventCallBack(std::bind(&TreeView::SubNodeCallback, this, std::placeholders::_1, std::placeholders::_2));
	pScrollView->AddChild(but);

	Sprite *sp = new Sprite(bitmapinfo[item.nBitmapId[0]].c_str());
	sp->SetVisible(false);
	pScrollView->AddChild(sp);

	Sprite *sp1 = new Sprite(bitmapinfo[item.nBitmapId[1]].c_str());
	sp1->SetVisible(false);
	pScrollView->AddChild(sp1);

	if (item.bAllowExpand == true)
	{
		ButtonImage *pButForward = new ButtonImage("C:\\skia\\example\\icon\\forward.png");
		ButtonImage *pButExpland = new ButtonImage("C:\\skia\\example\\icon\\expland.png");
		pButForward->SetVisible(false);
		pButExpland->SetVisible(false);
		pScrollView->AddChild(pButForward);
		pScrollView->AddChild(pButExpland);

		pButForward->SetParam(pp);
		pButExpland->SetParam(pp);

		pButForward->SetMouseEventCallBack(std::bind(&TreeView::TreeNodeButCallback, this, std::placeholders::_1, std::placeholders::_2));
		pButExpland->SetMouseEventCallBack(std::bind(&TreeView::TreeNodeButCallback, this, std::placeholders::_1, std::placeholders::_2));
		pp->item.pWidget[0] = pButForward;
		pp->item.pWidget[1] = pButExpland;



		pp->item.pWidget[2] = sp;
		pp->item.pWidget[3] = sp1;
		
	}
	else
	{
		pp->item.pWidget[4] = sp;
	}

	
}
TreeView::node *TreeView::AddTreeItem(node *parent, TreeItem item)
{
	//根节点
	if (parent == NULL)
	{
		node *pp = root;
		for (;;)
		{
			if (pp->next == NULL)
			{
				node *bb = AllocTreeNode();
				pp->next = bb;
				AddItemToView(pp, item);
				break;
			}
			pp = pp->next;
		
		}
		return pp;
	}
	else
	{
		node *pp = parent->child;
		int k = 0;
		if (pp == NULL)
		{
			pp = AllocTreeNode();
			parent->child = pp;
			
		}
		for (;;)
		{

			if (pp->next == NULL)
			{
				node *bb = AllocTreeNode();
				pp->next = bb;
				AddItemToView(pp, item);
				break;
			}
			pp = pp->next;
			k++;
		/*	if (pp == NULL)
			{
				pp = (node *)malloc(sizeof(node));
				pp->next = 0;
				pp->child = 0;
				pp->bExpand = false;
				pp->item.text = item.text;
				pp->item.nBitmapId = item.nBitmapId;
				if(k==0)
				   parent->child = pp;
				break;
			}
			pp = pp->next;
			k++;*/
		}
		return pp;
	}
}

