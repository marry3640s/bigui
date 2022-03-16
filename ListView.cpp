#include "ListView.h"
#include "ListHead.h"
#include "tools/gpu/GrContextFactory.h"
ListView::ListView()
{
	collist.clear();
	rowlist.clear();

	SetPosition(0, 0);
	SetSize(0, 0);
	memset(&ContentInfo, 0x00, sizeof(ContentInfo));
	vert_bar = new ScrollBar(Direction::Vertical);
	hori_bar = new ScrollBar(Direction::Horizontal);
	vert_bar->set_controller(this);
	hori_bar->set_controller(this);

	SetBackGroundColor(SkColorSetRGB(255, 255, 255));

	SetScrolloffsX(0);
	SetScrolloffsY(0);

	header= new ListHead(this);
	nAligment=ListAligment::LEFT;
	mecallback=0;
	sel_cellinfo.color=GetBackGroundColor();
	//nViewStyle|=
}

ListView::~ListView()
{
	DelAllCol();
	if(vert_bar!=NULL)
		delete vert_bar;
	if(hori_bar!=NULL)
		delete hori_bar;
	if(header!=NULL)
		delete header;
}

void ListView::AddCol(ColumnInfo info)
{
	collist.push_back(info);
}
void ListView::AddCol(char * name, int nWidth)
{
	ColumnInfo info;
	info.colname=name;
	info.nWidth=nWidth;
	info.nInitOrder=collist.size();
	info.nShowOrder=info.nInitOrder;
	collist.push_back(info);

	Button *but=new Button();
	but->SetText(name);
	titbutlist.push_back(but);
	ContentInfo.width+=nWidth;
}

void ListView::SetListViewRow(int nRow)
{
	if(rowlist.size()>nRow)
		return;
	int nAddRow=nRow-rowlist.size()+1;
	CellItem cell;
	memset(&cell,0x00,sizeof(CellItem));
	for (int k = 0; k < nAddRow; k++)
	{
		RowItem *rowit=new RowItem();
		rowit->nRowHeigth=25;
		rowlist.push_back(rowit);
		for (int j = 0; j < collist.size(); j++)
		{
			rowit->celllist.push_back(cell);
		}
	}
	ContentInfo.height+=nAddRow*25;
 //   printf("ContentInfo.height=%d\n", ContentInfo.height);
	UpdateScrollBarInfo();
}

void ListView::AddCellItem(UIWidget *pWidget,int nRow,int nCol)
{
	SetListViewRow(nRow);
	CellItemUpdate(pWidget,nRow,nCol);
}


void ListView::CellItemUpdate(UIWidget *pWidget,int nRow,int nCol)
{
	rowlist.at(nRow)->celllist.at(nCol).pWidget=pWidget;
	pWidget->SetPosition(0,0);
	pWidget->SetSize(collist.at(nCol).nWidth,rowlist.at(nRow)->nRowHeigth);
}


void ListView::CellItemUpdate(SkString text,int nRow,int nCol)
{
}

void ListView::DelRow(int nRow)
{
	//warring ,may be delete row failed
	if(rowlist.size()<=nRow)
		return;
	int nRowHei= rowlist[nRow]->nRowHeigth;

	for (int k = 0; k < rowlist[nRow]->celllist.size(); k++)
	{
		if(rowlist[nRow]->celllist[k].pWidget!=0)
			delete rowlist[nRow]->celllist[k].pWidget;
	}
	rowlist[nRow]->celllist.clear();
	delete rowlist[nRow];
	rowlist.erase(rowlist.begin()+nRow);
	
	ContentInfo.height-=nRowHei;
	UpdateScrollBarInfo();

}

void ListView::DelAllRow()
{
	/*for (int nRow = 0; nRow < rowlist.size(); nRow++)
	{
		printf("row size=%d\n",rowlist.size());
		DelRow(rowlist.size()-1);
	}
	return;*/
	for (int nRow = 0; nRow < rowlist.size(); nRow++)
	{
		for (int k = 0; k < rowlist[nRow]->celllist.size(); k++)
	    {
		    if(rowlist[nRow]->celllist[k].pWidget!=0)
			    delete rowlist[nRow]->celllist[k].pWidget;
	    }
	    rowlist[nRow]->celllist.clear();
		delete rowlist[nRow];
	}
	rowlist.clear();
	ContentInfo.height=0;
	UpdateScrollBarInfo();
}

void ListView::DelCol(int nCol)
{
	int nDelIndex=-1;
	for (int k = 0; k < collist.size(); k++)
	{
		if (collist[k].nShowOrder == nCol)
		{
			nDelIndex = k;
			break;
		}
	}
	if(nDelIndex==-1)
		return;

	for (int nRow = 0; nRow < rowlist.size(); nRow++)
	{
		if(rowlist[nRow]->celllist[nDelIndex].pWidget!=0)
			delete rowlist[nRow]->celllist[nDelIndex].pWidget;

		rowlist[nRow]->celllist.erase(rowlist[nRow]->celllist.begin()+nDelIndex);
	}
	int nColWidth=collist[nDelIndex].nWidth;
	collist.erase(collist.begin()+nDelIndex);
	ContentInfo.width-=nColWidth;
	UpdateScrollBarInfo();
}


void ListView::DelAllCol()
{
	DelAllRow();
	collist.clear();
	ContentInfo.height=0;
	ContentInfo.width=0;
	UpdateScrollBarInfo();
}

void ListView::Draw(SkCanvas* canvas) 
{
	
	
	ContentInfo.offs_y = GetScrolloffsY();
	SkScalar diff_y = ContentInfo.offs_y - ContentInfo.preoffs_y;
	ContentInfo.offs_x = GetScrolloffsX();
	SkScalar diff_x = ContentInfo.offs_x - ContentInfo.preoffs_x;
	/*SkScalar diff_y = ContentInfo.offs_y - ContentInfo.preoffs_y;
	SkScalar diff_x = ContentInfo.offs_x - ContentInfo.preoffs_x;*/

	ContentInfo.preoffs_y = ContentInfo.offs_y;
	ContentInfo.preoffs_x = ContentInfo.offs_x;
   // GrContext::
	//GrContext* context = canvas->getGrContext(

   
    auto context = canvas->recordingContext();
	SkImageInfo info = SkImageInfo::MakeN32(GetDisplayWidth(), GetDisplayHeigth(), kOpaque_SkAlphaType);
	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
    auto surfaceCanvas = gpuSurface->getCanvas();
	SkPaint paint;
	paint.setColor(GetBackGroundColor());
	surfaceCanvas->drawRect(SkRect{ 0,0,GetDisplayWidth(),GetDisplayHeigth() }, paint);
	int ins_y=0;
	int ins_x=0;
	fDrawTime=SkTime::GetMSecs();
	displaylist.clear();

	for (int row = 0; row< rowlist.size(); row++)
	{
		RowItem *currow=rowlist[row];
	    ins_y+=currow->nRowHeigth;
		if (ins_y >= (-ContentInfo.offs_y) && ins_y <= (-ContentInfo.offs_y) + GetDisplayHeigth()+currow->nRowHeigth)
		{
			ins_x=0;
			int nCol=0;
			for (auto iter =currow->celllist.begin(); iter != currow->celllist.end(); iter++,nCol++)
			{
				ins_x+=collist.at(nCol).nWidth;
				if (ins_x >= (-ContentInfo.offs_x) && ins_x <= (-ContentInfo.offs_x) + GetDisplayWidth()+collist.at(nCol).nWidth)
				{
					UIWidget *pChild = (*iter).pWidget;
					SkScalar top=(ins_y-(-ContentInfo.offs_y))-currow->nRowHeigth;
					SkScalar bottom=top+currow->nRowHeigth;
					SkScalar left=(ins_x-(-ContentInfo.offs_x))-collist.at(nCol).nWidth;
					SkScalar rigth=left+collist.at(nCol).nWidth;
					
					if (nViewStyle & LIST_STYLE_VERTGLINE)
					{
						SkPoint p1, p2;
						p1.set(ins_x + ContentInfo.offs_x, top);
						p2.set(ins_x + ContentInfo.offs_x, bottom);
						paint.setColor(SkColorSetRGB(30, 30, 30));
						surfaceCanvas->drawLine(p1, p2, paint);
						rigth-=1;
					}
					if (nViewStyle & LIST_STYLE_HORZGLINE)
					{
						SkPoint p1, p2;
						p1.set( ContentInfo.offs_x, bottom);
						p2.set(ins_x + ContentInfo.offs_x, bottom);
						paint.setColor(SkColorSetRGB(30, 30, 30));
						surfaceCanvas->drawLine(p1, p2, paint);
						if(row>0)
						    top+=1;
					
					}

					pChild->SetBound(left,top,rigth,bottom);
					displaylist.push_back(pChild);


					if (nViewStyle & LIST_STYLE_SIGNLESELCELL)
					{
						if (row == sel_cellinfo.nRow && nCol == sel_cellinfo.nCol)
							pChild->SetBackGroundColor(sel_cellinfo.color);
						else
							pChild->SetBackGroundColor(GetBackGroundColor());
					}
					else
					{
						if (row == sel_cellinfo.nRow)
							pChild->SetBackGroundColor(sel_cellinfo.color);
						else
							pChild->SetBackGroundColor(GetBackGroundColor());
					}
					pChild->Draw(surfaceCanvas);
				}
			}
		}
	}


	//for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	//{
	//    UIWidget *pChild = *iter;
	//    pChild->SetBound(pChild->GetBound().left()+diff_x, pChild->GetBound().top() + diff_y, pChild->GetWidth()+diff_x + pChild->GetBound().left(), pChild->GetBound().top() + pChild->GetHeight() + diff_y);
	//	if (pChild->GetBound().right() >= 0 && pChild->GetBound().left() <= GetDisplayWidth() && pChild->GetBound().bottom() >= 0 && pChild->GetBound().top() <= GetDisplayHeigth())
	//		displaylist.push_back(pChild);
	//}

	
	//for (auto iter = displaylist.begin(); iter != displaylist.end(); iter++)
	//{
	//	UIWidget *pChild = *iter;
	//	pChild->Draw(surfaceCanvas);
	//}

	if (nViewStyle & LIST_STYLE_HEAD)
	  header->Draw(canvas);
	UpdateScrollBarInfo();
	if (vert_bar != NULL)
		vert_bar->Draw(canvas);
	if (hori_bar != NULL)
		hori_bar->Draw(canvas);
	
	
	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
	canvas->drawImage(image, GetBound().left(), GetBound().top()+header->GetHeight());

	/*if (diff_y != 0 || diff_x != 0)
	   printf("draw need time=%f\n",SkTime::GetMSecs()-fDrawTime);*/
}
void ListView::OnMouseMove(int x, int y)
{

	if (vert_bar != NULL && vert_bar->IsVisible())
		vert_bar->OnMouseMove(x, y);

	if (hori_bar != NULL && hori_bar->IsVisible())
		hori_bar->OnMouseMove(x, y);

	header->OnMouseMove(x,y);
	SkPoint point=ScrollViewToChildPoint(x,y);
	for (auto iter = displaylist.begin(); iter != displaylist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		pChild->OnMouseMove(point.x(), point.y());
	}
}

void ListView::SetSelectedCellItemBackGround(SkColor color)
{
	sel_cellinfo.color=color;
}

CellSelectedInfo ListView::GetCellItemSelectedInfo(int x,int y)
{
	CellSelectedInfo selinfo;
	selinfo.color=sel_cellinfo.color;
	int ins_y=ContentInfo.offs_y;
	int ins_x=ContentInfo.offs_x;
	for (int k = 0; k < rowlist.size(); k++)
	{
		if (y >= ins_y && y <= ins_y + rowlist[k]->nRowHeigth)
		{
			selinfo.nRow=k;
			break;
		}
		ins_y+=rowlist[k]->nRowHeigth;
	}

	if(selinfo.nRow==-1)
		return selinfo;
	for (int k = 0; k < collist.size(); k++)
	{
		if (x >= ins_x && x <= ins_x + collist[k].nWidth)
		{
			selinfo.nCol = k;
				//printf("sel row=%d,col=%d\n",selinfo.nRow,selinfo.nCol);
			break;
		}
		ins_x += collist[k].nWidth;
	}

	selinfo.pWidget=rowlist[selinfo.nRow]->celllist[selinfo.nCol].pWidget;
	
	return selinfo;
}

bool ListView::OnMouseDown(int x, int y)
{
	
	if (vert_bar != NULL)
	{
		if (x >= vert_bar->GetBound().left() && x <= vert_bar->GetBound().right() && y >= vert_bar->GetBound().top() && y <= vert_bar->GetBound().bottom())
		  return  vert_bar->OnMouseDown(x, y);
	}

	if (hori_bar != NULL && hori_bar->IsVisible())
	{
		if (x >= hori_bar->GetBound().left() && x <= hori_bar->GetBound().right() && y >= hori_bar->GetBound().top() && y <= hori_bar->GetBound().bottom())
		   return  hori_bar->OnMouseDown(x, y);
	}
	header->OnMouseDown(x,y);

	SkPoint point=ScrollViewToChildPoint(x,y);

	sel_cellinfo=GetCellItemSelectedInfo(point.x(),point.y());
	for (auto iter = displaylist.begin(); iter != displaylist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		if (point.x() >= pChild->GetBound().left() && point.x() <= pChild->GetBound().right() && point.y() >= pChild->GetBound().top() && point.y() <= pChild->GetBound().bottom())
		{
			//printf("x=%d,y=%d,child_x=%d,child_y=%d,left=%f,right=%f,top=%f,bottom=%f\n", x, y, point.x(), point.y(), pChild->GetBound().left(), pChild->GetBound().right(), pChild->GetBound().top(), pChild->GetBound().bottom());
			return pChild->OnMouseDown(point.x(), point.y());
		}
		
	}
}
bool ListView::OnMouseUp(int x, int y)
{
	if (vert_bar != NULL && vert_bar->IsVisible())
	{
		vert_bar->OnMouseUp(x, y);
	}
	if (hori_bar != NULL && hori_bar->IsVisible())
	{
		hori_bar->OnMouseUp(x, y);
	}
	header->OnMouseUp(x,y);
	SkPoint point=ScrollViewToChildPoint(x,y);
	CellSelectedInfo temp=GetCellItemSelectedInfo(point.x(),point.y());
	if (mecallback != 0)
	{
		if (nViewStyle & LIST_STYLE_SIGNLESELCELL)
		{
			if (sel_cellinfo.nCol == temp.nCol && sel_cellinfo.nRow == temp.nRow)
				mecallback(temp,MouseEvent::MOUSE_LBUTTONPRESS);
			return false;
		}
		if (sel_cellinfo.nRow == temp.nRow)
		{
			mecallback(temp, MouseEvent::MOUSE_LBUTTONPRESS);
		}
	}
	return false;
}
void ListView::OnMouseWheel(float delta, uint32_t modifier)
{
	if(vert_bar==NULL || !vert_bar->IsVisible())
		return;
	ScrollToPosition(vert_bar,GetScrolloffsY()+delta*10);
}

void ListView::OnKey(skui::Key key, uint32_t modifiers)
{
    if (key == skui::Key::kDown && modifiers == 0)
	{
		//printf("on key modifiers=%d\n",modifiers);
		if (sel_cellinfo.nCol == -1)
		{
			sel_cellinfo.nCol=0;
			sel_cellinfo.nRow=0;
		}
		else
		{
			sel_cellinfo.nRow+=1;
			sel_cellinfo.nRow=std::min((int)rowlist.size()-1,sel_cellinfo.nRow);

			int ins_y=0;
			for (int k = 0; k <= sel_cellinfo.nRow; k++)
			{
				ins_y+=rowlist[k]->nRowHeigth;
			}
			ins_y=ins_y+ContentInfo.offs_y;
			if(ins_y>GetDisplayHeigth()-25)
				ScrollToPosition(vert_bar, GetScrolloffsY() - 25);

			/*if(sel_cellinfo.nRow*25<-GetScrolloffsY())
			   ScrollToPosition(vert_bar, GetScrolloffsY() - 25);*/
		}
	}
    if (key == skui::Key::kUp  && modifiers==0)
	{
 		 sel_cellinfo.nRow-=1;
		sel_cellinfo.nRow=std::max(0,sel_cellinfo.nRow);

		int ins_y=0;
		for (int k = 0; k <= sel_cellinfo.nRow; k++)
		{
			ins_y+=rowlist[k]->nRowHeigth;
		}
		ins_y=ins_y+ContentInfo.offs_y;
		printf("ins_y=%d\n",ins_y);
		if(ins_y<0+25)
		  ScrollToPosition(vert_bar,GetScrolloffsY()+25);
	}
    if (key == skui::Key::kLeft && modifiers == 0)
	{
		if (nViewStyle & LIST_STYLE_SIGNLESELCELL)
		{

			sel_cellinfo.nCol-=1;
			sel_cellinfo.nCol=std::max(0,sel_cellinfo.nCol);

			int ins_x=0;
		    for (int k = 0; k <= sel_cellinfo.nCol; k++)
		    {
			    ins_x+=collist[k].nWidth;
		    }
		    ins_x=ins_x+ContentInfo.offs_x;
		   // printf("ins_x=%d\n",ins_x);

			if(ins_x<collist[sel_cellinfo.nCol].nWidth)
			   ScrollToPosition(hori_bar, GetScrolloffsX()+collist[sel_cellinfo.nCol].nWidth);
			return;
		}
		ScrollToPosition(hori_bar,GetScrolloffsX()+50);
	}

	if (key == skui::Key::kRight && modifiers == 0)
	{
		if (nViewStyle & LIST_STYLE_SIGNLESELCELL)
		{
			if (sel_cellinfo.nCol == -1)
		    {
			    sel_cellinfo.nCol=0;
			    sel_cellinfo.nRow=0;
		    }
			else
			{
				sel_cellinfo.nCol+=1;
			    sel_cellinfo.nCol=std::min((int)collist.size()-1,sel_cellinfo.nCol);

				int ins_x=0;
		        for (int k = 0; k <= sel_cellinfo.nCol; k++)
		        {
			        ins_x+=collist[k].nWidth;
		        }
		        ins_x=ins_x+ContentInfo.offs_x;
		    //    printf("ins_x=%d\n",ins_x);
				if(ins_x>GetDisplayWidth()-collist[sel_cellinfo.nCol].nWidth)
				  ScrollToPosition(hori_bar, GetScrolloffsX() - collist[sel_cellinfo.nCol].nWidth);
			}
			return;
		}
		ScrollToPosition(hori_bar,GetScrolloffsX()-50);
	}
}

SkScalar ListView::GetDisplayWidth()
{
	SkScalar width=GetBound().width();
	if (ContentInfo.height > GetBound().height()-header->GetHeight() && vert_bar->IsVisible())
		width-=vert_bar->GetWidth();
	return width;
}
SkScalar ListView::GetDisplayHeigth() 
{
	SkScalar heigth=GetBound().height()-header->GetBound().height();
	if (ContentInfo.width > GetBound().width() && hori_bar->IsVisible())
		heigth-=hori_bar->GetHeight();
	return heigth;
}

void ListView::ScrollToPosition(ScrollBar* source, int position)
{
	if(source==NULL)
		return;
	if (source == vert_bar)
	{
		SkScalar pos_y=position;
		//printf("scroll before pos=%f\n",pos_y);
		pos_y=std::min((float)0,(float)pos_y);
		pos_y=std::max((float)(-(ContentInfo.height - GetDisplayHeigth())),(float)pos_y);
		//printf("scroll pos=%f\n",pos_y);
		SetScrolloffsY(pos_y);

	}

	if (source == hori_bar)
	{
		SkScalar pos_x=position;
		pos_x=std::min((float)0,(float)pos_x);
		pos_x=std::max((float)(-(ContentInfo.width - GetDisplayWidth())),(float)pos_x);
		SetScrolloffsX(pos_x);
	}
}

void ListView::SetSortCol(SortDescriptor desc)
{
	sortdescMap.insert(std::pair<int, SortDescriptor>(desc.nCol, desc));
}

void ListView::SetSortCol(int nCol)
{
	SortDescriptor desc;
	desc.nCol=nCol;
	SetSortCol(desc);
}

void ListView::ReadySort(int nCol)
{
	if(sortdescMap.find(nCol)==sortdescMap.end() || nCol<0)
		return;
	sortdescMap[nCol].ascending=!sortdescMap[nCol].ascending;
	QuiteSort(sortdescMap[nCol]);
	
}


void ListView::QuiteSort(SortDescriptor desc)
{

	sort(rowlist.begin(), rowlist.end(), [desc](RowItem *pRowx, RowItem *pRowy)
	{
		StaticText *x=(StaticText *)pRowx->celllist[desc.nCol].pWidget;
		StaticText *y=(StaticText *)pRowy->celllist[desc.nCol].pWidget;
		if(desc.ascending)
		     return atoi(x->GetText().c_str())>atoi(y->GetText().c_str());
		else
			 return atoi(x->GetText().c_str())<atoi(y->GetText().c_str());
	});
	//sort(rowlist.begin(), rowlist.end(), [](UIWidget *x, UIWidget *y) 
	//{
	//	return x->nShowOrder < y->nShowOrder;
	//});
}



void ListView::SetViewStyle(int nStyle)
{
	nViewStyle=nStyle;
	if (nViewStyle & LIST_STYLE_HEAD)
	{
		header->SetBound(GetBound().left(),GetBound().top(),GetDisplayWidth()+GetBound().left(),25+GetBound().top());
	}
	else
	{
		header->SetBound(0,0,0,0);
	}
}


SkPoint ListView::ScrollViewToChildPoint(int x, int y) 
{
	SkPoint point;
	point.set(x - GetBound().left(), y - GetBound().top()-header->GetHeight());
	return point;
}

void ListView::UpdateScrollBarInfo()
{
	if (ContentInfo.height > GetDisplayHeigth())
	{
		if (vert_bar->GetWidth() == 0)
		{
			vert_bar->SetPosition(GetBound().width()-BAR_VER_WIDTH+GetBound().left(),GetBound().top()+header->GetHeight());
			vert_bar->SetSize(BAR_VER_WIDTH,GetDisplayHeigth());
		}
		else
		{
			vert_bar->SetPosition(GetBound().width()-vert_bar->GetBound().width()+GetBound().left(),GetBound().top()+header->GetHeight());
			vert_bar->SetSize(vert_bar->GetBound().width(),GetDisplayHeigth());
		}

		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.height;
		barinfo.DisplaySize=vert_bar->GetHeight();
		barinfo.offset=ContentInfo.offs_y;
		SkScalar ContentMoveMax=barinfo.ContentSize-barinfo.DisplaySize;
		barinfo.offset=std::max(barinfo.offset,-ContentMoveMax);
		SetScrolloffsY(barinfo.offset);
		vert_bar->SetScrollBarInfo(barinfo);
	}
	else
	{
		vert_bar->SetBound(0,0,0,0);
		SetScrolloffsY(0);
	}

	if (ContentInfo.width > GetDisplayWidth())
	{
		if (hori_bar->GetWidth() == 0)
		{
			hori_bar->SetPosition(GetBound().left(), GetBound().bottom()-BAR_HORI_HEIGHT);
			hori_bar->SetSize(GetDisplayWidth(), BAR_HORI_HEIGHT);
		}
		else
		{
			hori_bar->SetPosition(GetBound().left(),GetBound().bottom()-hori_bar->GetBound().height());
			hori_bar->SetSize(GetDisplayWidth(), hori_bar->GetBound().height());
		}
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.width;
		barinfo.DisplaySize=hori_bar->GetWidth();
		barinfo.offset=ContentInfo.offs_x;

		SkScalar ContentMoveMax=barinfo.ContentSize-barinfo.DisplaySize;
		barinfo.offset=std::max(barinfo.offset,-ContentMoveMax);
		SetScrolloffsX(barinfo.offset);
		hori_bar->SetScrollBarInfo(barinfo);
	}
	else
	{
		hori_bar->SetBound(0,0,0,0);
		SetScrolloffsX(0);
	}
}

void ListView::SetCellItemMouseEvent(ListViewMouseEventCallBack fun)
{
	mecallback=fun;
}