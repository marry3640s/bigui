#pragma once
//#include "GrContext.h"
#include "include/core/SkTime.h"
#include "vector"
#include "UIRoot.h"
#include "ScrollView.h"
#include "Sprite.h"
#include "GameTimer.h"
#include "HelloWorld.h"
#define MAX_COLUMN 5
#define MAX_ROLL_COUNT 2000
struct RollConfig {
	int nRow;             
	int nColumn;          
	int nGoodsHeight;     
	int nGoodsWidth;      
	int nGoodsSpace;      
	int nRollGoodsCount;  
	
};


struct lotpays {
	unsigned char goods;   
	int  nPays;            
	int  nDepth;           
	long long llPoint;     
	int  nStay;            
	int  nStayCount;      
						  
};

#define MAX_LOTPAYS 3

struct StayGoodsInfo {
	int            nLotpayCount;
	lotpays        lotpayslist[MAX_LOTPAYS];
};


struct RollData {
	char RollGoods[MAX_COLUMN][MAX_ROLL_COUNT];
	int  nSpeed[MAX_COLUMN]; 
};


class RollImage : public UIShard {
public:
	RollImage::RollImage(struct RollConfig con);
	void SetGoodsPath(char *pFilePath);
	//void SetRollConfig(struct RollConfig con);
	void SetRollData(struct RollData data, StayGoodsInfo info);

	void ShowStayGoods(StayGoodsInfo stay);

	void Stay();
	int nStayIndex;
	ScrollView *view[MAX_COLUMN];
	void Rolling(float ft);

	
private:
	RollConfig conf;
	StayGoodsInfo stayinfo;
	char pszGoodsPath[260];

	Sprite  *spgoods[5][3];
	RollConfig config;
	RollData   curRollData;
};


