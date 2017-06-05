/**
* 内部含有Sprite
* 可以通过ForeverMove和StandMove进行移动
* 目前默认角色创建，以后可以添加角色选择功能
*/
#ifndef _PLAYER_
#define _PLAYER_

#include "cocos2d.h"

USING_NS_CC;

class Player :public Layer
{
public:
	virtual bool init();
	//精灵永远执行这个动作
	CC_SYNTHESIZE(Sprite*, player, Player);
	void ForeverMove(EventKeyboard::KeyCode code);
	void StandMove(EventKeyboard::KeyCode code);

	//通过动画名字得到相应的动画
	Animate* getAnimateByName(std::string animName, float delay, int animNum);
	Sprite* _player;  // 玩家精灵
	CREATE_FUNC(Player);
private:
	
	//向左、右、上、下的静态图片，当向对应方向移动时，用此贴图进行替换
	CCTexture2D *_player_texture_left;
	CCTexture2D *_player_texture_right;
	CCTexture2D *_player_texture_up;
	CCTexture2D *_player_texture_down;
};
#endif