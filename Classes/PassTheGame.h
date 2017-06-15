#ifndef __PASS_THE_GAME_H__
#define __PASS_THE_GAME_H__

#include "cocos2d.h"

class  PassTheGame : public cocos2d::Layer
{
public:
	//创建场景
	static cocos2d::Scene* createScene();
	//创建初始化init函数，当成功创建场景的时候返回true
	virtual bool init();
	//使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	CREATE_FUNC(PassTheGame);
};

#endif 
