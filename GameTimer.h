#pragma once
//#include "GrContext.h"
#include "include/core/SkTime.h"
#include <map>
#include <vector>
#include <iostream>
class UIShard;
typedef void(UIShard::*TimerCallBackFun)(void);
#define timer_sel(sel)  static_cast<TimerCallBackFun>(&sel)

class GameTimer
{
public:
	GameTimer();
	void SetTimer(UIShard *target, TimerCallBackFun fun, double ftime);
	void KillTimer(TimerCallBackFun fun);
	void update();

	TimerCallBackFun GetCallBackFun()
	{
		return callfun;
	}
	bool GetTimerEnable()
	{
		return bEnable;
	}
private:
	double fLastStamp;
	double fElapse;
	TimerCallBackFun callfun;
	UIShard *pTarget;
	bool bEnable;
};
class GameTimerManage
{
public:
	GameTimerManage();
	void UpdateAllGameTimer();
	void SetTimer(UIShard *target, TimerCallBackFun fun, double fElapse);
	void KillTimer(TimerCallBackFun fun);

	bool IsTimerRuning(TimerCallBackFun fun);
private:
	//std::map<void *, GameTimer *> timerlist;
	std::vector<GameTimer *> timerlist;
	std::vector<GameTimer *> templist;
	//std::map<char *, GameTimer *> timerlist;
};
