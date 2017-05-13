#ifndef __MENU_SWITCH_H__
#define __MENU_SWITCH_H__

#include "cocos2d.h"

class MenuSwitch : public cocos2d::Layer
{
public:
	//开始游戏菜单
	void StartGame(cocos2d::Object* pSender);
	//结束游戏菜单
	void ExitGame(cocos2d::Ref* pSender);
	//返回主菜单
	void BackToMenu(cocos2d::Ref* pSender);
	//游戏设置
	void GameSet(cocos2d::Ref* pSender);
};

#endif 