#include "GameTimer.h"

GameTimer::GameTimer()
{
	bEnable = false;
}

void GameTimer::update()
{
	double curstamp = SkTime::GetMSecs();
	if (curstamp >= fLastStamp + fElapse)
	{
		fLastStamp = curstamp;
		(pTarget->*callfun)();
	}
}

void GameTimer::SetTimer(UIShard *target, TimerCallBackFun fun, double ftime)
{
	callfun = fun;
	fElapse = ftime * 1000;
	fLastStamp = SkTime::GetMSecs();
	pTarget = target;
	bEnable = true;
}

void GameTimer::KillTimer(TimerCallBackFun fun)
{
	//callfun = NULL;
	fElapse = 1000000 * 1000;
	bEnable = false;
}

GameTimerManage::GameTimerManage()
{

}

//size_t getAddress(TimerCallBackFun function) {
//	typedef void (fnType)(void);
//	fnType * fnPointer = function.target<fnType *>();
//	return (size_t)fnPointer;
//}

void GameTimerManage::SetTimer(UIShard *target, TimerCallBackFun fun, double fElapse)
{
	/*void *pAddr = &fun;
	if (timerlist.find(pAddr) == timerlist.end())
	{
		GameTimer *pTimer = new GameTimer();
		pTimer->SetTimer(target, fun, fElapse);
		timerlist.insert(std::pair<void *, GameTimer*>(pAddr, pTimer));
	}*/
	//if (timerlist.find(*(target->*fun)) != timerlist.end())
	//{

	//}
	if (IsTimerRuning(fun) == false)
	{
		GameTimer *pTimer = new GameTimer();
		pTimer->SetTimer(target, fun, fElapse);
		templist.push_back(pTimer);
	}
	
	/*GameTimer *pTimer = new GameTimer();
	pTimer->SetTimer(target, fun, fElapse);
	if (IsTimerRuning(fun) == false)
	{
		timerlist.push_back(pTimer);
	}*/

	//char *pAddress = (char *)fun;
	/*auto it = timerlist.find(target->*fun);
	if (it == timerlist.end())
	{
		GameTimer *pTimer = new GameTimer();
		pTimer->SetTimer(target,fun, fElapse);
		timerlist.insert(std::pair<TimerCallBackFun, GameTimer*>(fun, pTimer));
	}*/
}

void GameTimerManage::UpdateAllGameTimer()
{
	/*std::map<void *, GameTimer *>::iterator iter, itTemp;
	for (iter = timerlist.begin(); iter != timerlist.end();)
	{
		itTemp = iter;
		itTemp++;
	
		if (iter->second->GetTimerEnable() == false)
		{
			TimerCallBackFun fun = iter->second->GetCallBackFun();
			void *pAddr = &fun;
			timerlist.erase(pAddr);
		
		}
		iter = itTemp;
	}
	
	for (iter = timerlist.begin(); iter != timerlist.end();iter++)
	{
		iter->second->update();

	}*/

	timerlist.insert(timerlist.end(), templist.begin(), templist.end());
	templist.clear();
	if (timerlist.size() == 0)
		return;

	for (auto iter = timerlist.begin(); iter != timerlist.end();)
	{
		if ((*iter)->GetTimerEnable() == false)
		{
			delete *iter;
			iter = timerlist.erase(iter);
		}
		else
			iter++;
	}
	for (auto iter = timerlist.begin(); iter != timerlist.end();iter++)
	{
		(*iter)->update();
	}
}


bool GameTimerManage::IsTimerRuning(TimerCallBackFun fun)
{
	/*void *pAddr = &fun;
	auto iter = timerlist.find(pAddr);
	if (iter != timerlist.end())
	{
		return iter->second->GetTimerEnable();
	}
	return false;*/
	for (auto iter = timerlist.begin(); iter != timerlist.end();iter++)
	{
		if ((*iter)->GetCallBackFun() == fun)
			return (*iter)->GetTimerEnable();
		
	}
	return false;
}

void GameTimerManage::KillTimer(TimerCallBackFun fun)
{
	/*void *pAddr = &fun;
	auto iter = timerlist.find(pAddr);
	if (iter != timerlist.end())
	{
		iter->second->KillTimer(fun);
	}*/
	for (auto iter = timerlist.begin(); iter != timerlist.end(); iter++)
	{
		if ((*iter)->GetCallBackFun() == fun)
		{
			(*iter)->KillTimer(fun);
			break;
		}
	}
}
