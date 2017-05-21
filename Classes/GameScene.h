#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	//创建Scene场景
	static cocos2d::Scene* createScene();
	//init函数，完成初始化操作
	virtual bool init();
	// 将屏幕坐标转换为TileMap坐标，暂时没用
	Vec2 tileCoordForPosition(Vec2 position);
	//求方格的中心坐标  
	Vec2 centerPositionForTileCoord(const cocos2d::Vec2 &TileCoord);
   //判断键盘是否按住
	bool isKeyPressed(EventKeyboard::KeyCode code);
	//键盘按下后的事情
	void keyPressedDuration(EventKeyboard::KeyCode code);
  //精灵永远执行这个动作
	void ForeverMove(EventKeyboard::KeyCode code);
	//通过动画名字得到相应的动画
	Animate* getAnimateByName(std::string animName, float delay, int animNum);
	//更新位置
	void UpdatePosition(float delta);
	// 回调更新函数，该函数每一帧都会调用
	void update(float delta);
	//使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	CREATE_FUNC(GameScene);
private:
	//键盘按键记录
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	TMXTiledMap* map;  // 地图
	Sprite* _player;  // 玩家精灵
	TMXLayer* _collidable;  // 障碍层
	Vector<Sprite* > _popVector;
	//向左、右、上、下的静态图片，当向对应方向移动时，用此贴图进行替换
	CCTexture2D *_player_texture_left;
	CCTexture2D *_player_texture_right;
	CCTexture2D *_player_texture_up;
	CCTexture2D *_player_texture_down;
};

#endif
