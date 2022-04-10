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

}

void NodeRec(TreeView::node *pNode, TreeView *pTree,int *pDepth,int *pWidth)
{
	TreeView::node *pp = pNode;
	for (;;)
	{
		if (pp->next == NULL)
			break;
		//printf("%s\n", pp->item.text);
		pp->item.pWidget->SetPosition((*pWidth) * 50+30, (*pDepth) * 25);
		pp->item.pWidget->SetSize(200, 25);
		pp->item.pWidget->SetVisible(true);
		(*pDepth)++;
		//pTree->pScrollView
		//pp->item.pWidget->SetPosition((*pWidth)*50, (*pDepth) * 25);
		
		if (pp->child != NULL)
		{
			if (pp->child->item.bAllowExpand == true && pp->child->item.bExpand == true)
			{
				(*pWidth)++;
				NodeRec(pp->child, pTree, pDepth,pWidth);
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
	int nDepth = 0, nWidth = 0;
	
	if(kk==0)
	  NodeRec(root, this,&nDepth,&nWidth);

	kk++;
	
	pScrollView->SetPosition(GetBound().left(), GetBound().top());
	pScrollView->SetSize(GetBound().width(), GetBound().height());
	if (kk == 1)
	pScrollView->SetContentSize(nWidth * 50 + 30+100, nDepth*25);
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

TreeView::node *TreeView::AllocTreeNode()
{
	int si = sizeof(node);
	node *pp = (node *)malloc(sizeof(node));
	pp->next = NULL;
	pp->child = NULL;
	return pp;
}

TreeView::node *TreeView::AddTreeItem(node *parent, TreeItem item)
{
	//¸ù½Úµã
	if (parent == NULL)
	{
		node *pp = root;
		for (;;)
		{
			if (pp->next == NULL)
			{
				node *bb = AllocTreeNode();
				pp->next = bb;
				pp->item.text = (char *)malloc(strlen(item.text)+1);
				memset(pp->item.text, 0x00, strlen(item.text) + 1);
				memcpy(pp->item.text, item.text, strlen(item.text));

				pp->item.ptag = (char *)malloc(strlen(item.ptag) + 1);
				memset(pp->item.ptag, 0x00, strlen(item.ptag) + 1);
				memcpy(pp->item.ptag, item.ptag, strlen(item.ptag));
				pp->item.bAllowExpand = item.bAllowExpand;
				pp->item.bExpand = item.bExpand;
			//	pp->item.text = item.text;
			
				pp->item.nBitmapId = item.nBitmapId;
				Button *but = new Button();
				but->SetText(pp->item.text);
				but->SetPosition(0, 0);
				but->SetSize(0, 0);
				pp->item.pWidget = but;
				but->SetVisible(false);
				//but->SetMouseEventCallBack()
				pScrollView->AddChild(but);
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
				//pp->item.text = item.text;
				pp->item.text = (char *)malloc(strlen(item.text)+1);
				memset(pp->item.text, 0x00, strlen(item.text) + 1);
				memcpy(pp->item.text, item.text, strlen(item.text));

				pp->item.ptag = (char *)malloc(strlen(item.ptag) + 1);
				memset(pp->item.ptag, 0x00, strlen(item.ptag) + 1);
				memcpy(pp->item.ptag, item.ptag, strlen(item.ptag));

				pp->item.nBitmapId = item.nBitmapId;
				pp->item.bAllowExpand = item.bAllowExpand;
				pp->item.bExpand = item.bExpand;

				Button *but = new Button();
				but->SetText(pp->item.text);
				but->SetPosition(0, 0);
				but->SetSize(0, 0);
				pp->item.pWidget = but;
				but->SetVisible(false);
				pScrollView->AddChild(but);
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

