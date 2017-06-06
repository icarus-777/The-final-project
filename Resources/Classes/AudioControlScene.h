#ifndef __AUDIO_CONTROL_SCENE__
#define __AUDIO_CONTROL_SCENE__


#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "BeginScene.h"
using namespace CocosDenshion;

// AudioControl类用来设置背景音乐和声音特效
class AudioControl : public cocos2d::Layer
{
public:
	//静态函数，创建Scene场景
	static cocos2d::Scene* createScene();

// init函数，完成初始化操作。
	virtual bool init();

	//使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	
	CREATE_FUNC(AudioControl);
};
#endif