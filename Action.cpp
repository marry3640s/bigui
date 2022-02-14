#include "Action.h"
#include "UIWidget.h"

#include "UIRoot.h"

#include "Sprite.h"

Animation::Animation()
{
	SetRunTime(1.0f);
	nLoopsCount = 1;
	nShowIndex = 0;
	fDelayPerUnit = 0.1;
}
Animation::~Animation()
{
	for (int k = 0; k < splist.size(); k++)
		delete splist[k];
	splist.clear();
}
void Animation::StartAction()
{
	fLastStamp = SkTime::GetMSecs();
	SetRunTime(splist.size()*fDelayPerUnit*nLoopsCount);
	ShowSprite(nShowIndex);
}
void Animation::StopAction()
{
	int a;
	a = 5;
	/*for (int k = 0; k < splist.size(); k++)
		delete splist[k];
	splist.clear();*/

}
void Animation::ShowSprite(int nIndex)
{
	/*printf("show index=%d\n", nIndex);*/
	for (int k = 0; k < splist.size(); k++)
	{
		splist[k]->SetPosition(GetWidget()->GetBound().left(), GetWidget()->GetBound().top());
		if (k == nIndex)
			splist[k]->SetVisible(true);
		else
			splist[k]->SetVisible(false);
	}
}
void Animation::update()
{
	if (ActionIsStart() == false)
		return;
	if (SkTime::GetMSecs() - fLastStamp >= fDelayPerUnit*1000)
	{
		nShowIndex++;
		nShowIndex = nShowIndex % splist.size();
		ShowSprite(nShowIndex);
		
	
		fLastStamp = SkTime::GetMSecs();
	}
}

void Animation::SetDelayPerUnit(double ft)
{
	fDelayPerUnit = ft;
}
void Animation::AddSprite(Sprite *sp)
{
	splist.push_back(sp);
	sp->SetVisible(false);
}
void Animation::SetLoops(int nLoops)
{
	nLoopsCount = nLoops;
	if (nLoops == -1)
		nLoopsCount = 10000000;
	SetRunTime(splist.size()*fDelayPerUnit*nLoopsCount);
}




BezierTo::BezierTo(double runtime, BezierConfig con)
{
	SetRunTime(runtime);
	memcpy(&config, &con, sizeof(BezierConfig));
}

Point2D BezierTo::PointOnCubicBezier(Point2D* cp, float t)
{
	float   ax, bx, cx;
	float   ay, by, cy;
	float   tSquared, tCubed;
	Point2D result;

	cx = 3.0 * (cp[1].x - cp[0].x);
	bx = 3.0 * (cp[2].x - cp[1].x) - cx;
	ax = cp[3].x - cp[0].x - cx - bx;

	cy = 3.0 * (cp[1].y - cp[0].y);
	by = 3.0 * (cp[2].y - cp[1].y) - cy;
	ay = cp[3].y - cp[0].y - cy - by;


	tSquared = t * t;
	tCubed = tSquared * t;

	result.x = (ax * tCubed) + (bx * tSquared) + (cx * t) + cp[0].x;
	result.y = (ay * tCubed) + (by * tSquared) + (cy * t) + cp[0].y;

	return result;
}


void  BezierTo::ComputeBezier(Point2D* cp, int numberOfPoints, Point2D* curve)
{
	float   dt;
	int    i;
	dt = 1.0 / (numberOfPoints - 1);
	for (i = 0; i < numberOfPoints; i++)
		curve[i] = PointOnCubicBezier(cp, i*dt);
}

void BezierTo::StartAction()
{
	pointlist =(Point2D *) malloc(config.nNumPoints * sizeof(Point2D));
	Point2D cp[4];
	memcpy(cp, &config, sizeof(Point2D) * 4);
	ComputeBezier(cp, config.nNumPoints, pointlist);
	fDelayPerUnit = GetRunTime()/config.nNumPoints;
	fLastStamp = SkTime::GetMSecs();
	nNodeIndex = 0;
}

void BezierTo::StopAction()
{
	//printf("nNodeIndex=%d", nNodeIndex);
	GetWidget()->SetPosition(pointlist[config.nNumPoints-1].x, pointlist[config.nNumPoints - 1].y);
	free(pointlist);
}

void BezierTo::update()
{
	if (ActionIsStart() == false)
		return;
	double curstamp = SkTime::GetMSecs();
	if (SkTime::GetMSecs() - fLastStamp >= fDelayPerUnit)
	{
		nNodeIndex++;
		nNodeIndex = nNodeIndex % config.nNumPoints;
		
		GetWidget()->SetPosition(pointlist[nNodeIndex].x, pointlist[nNodeIndex].y);
	//	printf("nNodeIndex=%d,mov x=%f,y=%f\n", nNodeIndex,pointlist[nNodeIndex].x, pointlist[nNodeIndex].y);
		fLastStamp = SkTime::GetMSecs();
	}
}

Blink::Blink(double runtime, int nBlink)
{
	nBlinkTick=nBlink;
	SetRunTime(runtime);
	bVisble = false;
}

void Blink::StartAction()
{
	fLastStamp = SkTime::GetMSecs();

}


void Blink::StopAction()
{
	GetWidget()->SetVisible(true);
}

void Blink::update()
{
	double fEachBlink = GetRunTime() / nBlinkTick/2;
	if (ActionIsStart() == false)
		return;
	if (SkTime::GetMSecs() - fLastStamp >= fEachBlink)
	{
		GetWidget()->SetVisible(bVisble);
		bVisble ^= 1;
		fLastStamp = SkTime::GetMSecs();
	}
}

ScrollTo::ScrollTo(double runtime, SkScalar x, SkScalar y)
{
	SetRunTime(runtime);
	move_x = x;
	move_y = y;
}

void ScrollTo::StartAction()
{
	init_y = GetWidget()->GetScrolloffsY();
	range_y = init_y - move_y;

	init_x = GetWidget()->GetScrolloffsX();
	range_x = init_x - move_x;
}
void ScrollTo::StopAction()
{
	GetWidget()->SetScrolloffsY(move_y);
}
void ScrollTo::update()
{
	if (ActionIsStart() == false)
		return;
	double curstamp = SkTime::GetMSecs();
	double rate = (curstamp - GetInitStamp()) / GetRunTime();

	SkScalar offs_y = init_y - range_y * rate;
	GetWidget()->SetScrolloffsY(offs_y);

	SkScalar offs_x = init_x - range_x * rate;
	GetWidget()->SetScrolloffsX(offs_x);
	//GetWidget()->SetScrollOffs(move_y-rate)
	
}

MoveTo::MoveTo(double runtime, SkScalar x, SkScalar y)
{
	SetRunTime(runtime);
	move_x = x;
	move_y = y;
	
}

void MoveTo::StartAction()
{
	init_x = GetWidget()->GetBound().left();
	init_y = GetWidget()->GetBound().top();
	/*range_x = fabs(move_x - GetWidget()->GetBound().left());
	range_y = fabs(move_y - GetWidget()->GetBound().top());*/
	range_x = move_x - GetWidget()->GetBound().left();
	range_y = move_y - GetWidget()->GetBound().top();
}
void MoveTo::StopAction()
{
	GetWidget()->SetPosition(move_x, move_y);
}

void MoveTo::update()
{
	if (ActionIsStart() == false)
		return;
	double curstamp = SkTime::GetMSecs();
	double rate=(curstamp - GetInitStamp())/ GetRunTime();
	SkScalar x, y;
	x = rate * range_x+init_x;
	y = rate * range_y+init_y;
	GetWidget()->SetPosition(x, y);
}


RotateTo::RotateTo(double runtime, SkScalar degrees)
{
	SetRunTime(runtime);
	fDegress=degrees;
}

void RotateTo::StartAction()
{
}

void RotateTo::StopAction()
{
	GetWidget()->SetDegress(fDegress);
}

void RotateTo::update()
{
	if (ActionIsStart() == false)
		return;
	double curstamp = SkTime::GetMSecs();
	double rate = (curstamp - GetInitStamp()) / GetRunTime();
	GetWidget()->SetDegress(rate*fDegress);
}

DelayTime::DelayTime(double runtime, ActCallBackFun callback)
{
	SetRunTime(runtime);
	fun = callback;
}

void DelayTime::update()
{
	ActionIsStart();
}


void DelayTime::StopAction()
{
	if (fun != 0)
		fun();
}


Repeat::Repeat(UIWidget *pUi, Action *act, int nRep, ActCallBackFun callback)
{
	pActionManage = gActionManage;
	double fDelayTime = 0;
	 
	Action *pAct = act;
	for (int k = 0; k < nRep; k++)
	{
		if (k != 0)
		{
			pAct = pAct->clone();
		}
		pActionManage->AddAction(pAct, pUi, fDelayTime);
		
		fDelayTime += pAct->GetRunTime();
	}
	if (callback != NULL)
	{
		pActionManage->AddAction(new DelayTime(fDelayTime / 1000, callback), pUi, 0);
	}
}

Repeat::Repeat(UIWidget *pUi, Sequence *seq, int nRep, ActCallBackFun callback)
{
	double fDelayTime = 0;
	for (int k = 0; k < nRep; k++)
	{
		fDelayTime+=seq->RunSequence();
	}
	if (callback != NULL)
	{
		pActionManage->AddAction(new DelayTime(fDelayTime / 1000, callback), pUi, 0);
	}
}

Sequence::Sequence(UIWidget *pUi, ActCallBackFun callback, ...)
{
	pActionManage = gActionManage;
	fun = callback;
	pWidget = pUi;
	va_list params;
	va_start(params, callback);

	for (;;)
	{
		Action *pAct = va_arg(params, Action *);
		if (pAct == NULL)
			break;
		list.push_back(pAct);
		
	}
	va_end(params);
	nRef = 0;
	fDelayTotal = 0;

	
}

double Sequence::RunSequence()
{
	double fDelayTime = 0;

	if (nRef > 0)
	{
		for (auto iter = list.begin(); iter != list.end(); iter++)
		{
			Action *pAct = (*iter)->clone();
			(*iter) = pAct;
		}
	}
	for (auto iter = list.begin(); iter != list.end();iter++)
	{
		Action *pAct = (*iter);
		pActionManage->AddAction(pAct, pWidget, fDelayTime+ fDelayTotal);
		fDelayTime += pAct->GetRunTime();
	}

	if (fun != NULL)
	{
		pActionManage->AddAction(new DelayTime((fDelayTime + fDelayTotal) / 1000, fun), pWidget, 0);
	}
	nRef++;
	fDelayTotal += fDelayTime;
	return fDelayTime;
}

Sequence::~Sequence()
{
	list.clear();
}




Action::Action()
{
	pWidget = NULL;
	bStartFlag = false;
}

bool Action::ActionIsStart()
{
	bool bRet = false;
	double curstamp=SkTime::GetMSecs();
	if (curstamp < initstamp)
		st = Ready;
	else if (curstamp >= initstamp && curstamp <= endstamp)
	{
		st = Start;
		bRet = true;
		if (bStartFlag == false)
		{
			StartAction();
			bStartFlag = true;
		}
	}
	else
	{
		st = Stop;
		StopAction();
	}

	return bRet;


}



void Action::ReadyAction(double fDelayTime)
{
	initstamp = SkTime::GetMSecs() + fDelayTime;
	endstamp = initstamp + runstamp;
//	printf("%f	%f\n", initstamp, endstamp);
}


ActionManage::ActionManage()
{

}

void ActionManage::UpdateAllAction()
{
	for (auto iter = actionlist.begin(); iter != actionlist.end();)
	{
		if ((*iter)->GetActionState() == Action::ActState::Stop)
		{
			delete *iter;
			iter=actionlist.erase(iter);
			
		}
		else
		{
			(*iter)->update();
			iter++;
		}
		
	}
}

void  ActionManage::StopAllAction(UIWidget *pWidget)
{
	for (auto iter = actionlist.begin(); iter != actionlist.end();)
	{
		if ((*iter)->GetWidget() == pWidget)
		{
			delete *iter;
			iter = actionlist.erase(iter);

		}
		else
		{
			iter++;
		}
	}
}

void ActionManage::AddAction(Action *act, UIWidget *pWidget, double fDelayTime)
{
	actionlist.push_back(act);
	act->SetWidget(pWidget);
	act->ReadyAction(fDelayTime);
}

