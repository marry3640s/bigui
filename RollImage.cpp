#include "RollImage.h"


RollImage::RollImage(struct RollConfig con)
{

	memset(pszGoodsPath, 0x00, sizeof(pszGoodsPath));
	memcpy(&config, &con, sizeof(RollConfig));
	SkScalar width, height;
	width = config.nGoodsWidth + 5 * 2;
	height = config.nGoodsHeight * config.nRollGoodsCount ;
	for (int k = 0; k < con.nColumn; k++)
	{
		view[k]= new ScrollView();
		view[k]->SetPosition(150 * k + 250, 150);
		view[k]->SetSize(width, config.nGoodsHeight *3+15);
		view[k]->SetContentSize(width, height);
		this->AddWidget(view[k]);
		view[k]->SetBackGroundColor(SkColorSetRGB(200, 200, 200));

	
	}
}

void RollImage::Rolling(float ft)
{
	
}

void RollImage::SetGoodsPath(char *pGoodsPath)
{
	if (pGoodsPath == 0)
		return;
	strcpy(pszGoodsPath, pGoodsPath);

}




void RollImage::Stay()
{
	int nIndex = nStayIndex;
	nIndex = nIndex%stayinfo.nLotpayCount;
	for (int j = 0; j < stayinfo.lotpayslist[nIndex].nDepth; j++)
	{
		for (int m = 0; m < 3; m++)
		{
			if (spgoods[j][m]->GetTag() == stayinfo.lotpayslist[nIndex].goods)
			{
				Blink * blink = new Blink(2.7f, 3);
				spgoods[j][m]->RunAction(blink);
				

			}
		}
	}
	
	nStayIndex++;

	this->KillTimer(timer_sel(RollImage::Stay));
	

	Sequence sq = Sequence(0, [&](){
		this->SetTimer(timer_sel(RollImage::Stay), 2.7);
	}, new DelayTime(0.3), 0);
	sq.RunSequence();

}



void RollImage::ShowStayGoods(StayGoodsInfo stay)
{
	nStayIndex = 0;
	
	if (stay.nLotpayCount >= 1)
	{
		//schedule(schedule_selector(RollImage::Stay), 0.0);
		this->SetTimer(timer_sel(RollImage::Stay), 0);
	}
	
}


void RollImage::SetRollData(struct RollData roData, StayGoodsInfo info)
{
	/*auto schedule = Director::getInstance()->getScheduler();
	if (schedule->isScheduled(schedule_selector(RollImage::Stay), this))
	{
		unschedule(schedule_selector(RollImage::Stay));
	}*/
	
	this->KillTimer(timer_sel(RollImage::Stay));
	memcpy(&stayinfo, &info, sizeof(StayGoodsInfo));
	memcpy(&curRollData, &roData, sizeof(RollData));

	for (int k = 0; k < config.nColumn; k++)
	{
		//view[k]->removeAllChildren();
		view[k]->RemoveAllChildWidget();
	}
	char pszBuf[256];

	for (int k = 0; k < config.nColumn; k++)
	{
		for (int j = 0; j < config.nRollGoodsCount; j++)
		{

			//sprintf(pszBuf, "%s/bg.png", pszGoodsPath);
			//Sprite* bg = new Sprite(pszBuf);
			////bg->runAction()
			//bg->SetPosition(65, j * 145 + 70);
			//view[k]->AddChild(bg);

			//if (roData.RollGoods[k][j] >= 6)
			//{
			//	sprintf(pszBuf, "%s/goodsbg/bg%d.png", pszGoodsPath, roData.RollGoods[k][j]);
			//	Sprite* sp = new Sprite(pszBuf);
			//	sp->SetPosition(65, j * 145 + 70);
			//	view[k]->AddChild(sp/*,100*/);
			//}

			sprintf(pszBuf, "%s/goods%d.png", pszGoodsPath, roData.RollGoods[k][j]);
			Sprite* sp1 = new Sprite(pszBuf);
			sp1->SetPosition(0, j * 120 /*+ 20*/);
			view[k]->AddChild(sp1/*,999*/);

			sp1->SetTag(roData.RollGoods[k][j]);
			if (j == config.nRollGoodsCount - 1)
				spgoods[k][0] = sp1;
			if (j == config.nRollGoodsCount - 2)
				spgoods[k][1] = sp1;
			if (j == config.nRollGoodsCount - 3)
				spgoods[k][2] = sp1;
			
		}
		//view[k]->jumpToBottom();
		//view[k]->scrollToTop(0.8f+k*0.28f, false);

		/*view[k]->JumpTop();
		ScrollTo *sc = new ScrollTo(1.0f + k * 0.2f,0, -(config.nGoodsHeight * 17));
		view[k]->RunAction(sc);*/
		//view[k]->scrollToBottom(1.0f + k*0.2f, false);
	}

	if (stayinfo.nLotpayCount > 0)
	{
		/*this->runAction(Sequence::create(DelayTime::create(2.5f), CallFunc::create([&]() {
			ShowStayGoods(stayinfo);
		}), NULL));*/

		Sequence sq = Sequence(0, [&]() {
			//ShowStayGoods(stayinfo);
		}, new DelayTime(2.5), 0);
		sq.RunSequence();
	}
	

	
}

