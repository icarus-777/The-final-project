#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
#include"Player.h"
#include"MapLayer.h"
USING_NS_CC;

enum Type
{
	PLAYER = 1,
	POP = 2
};
class GameScene : public cocos2d::Layer
{
public:
	//创建Scene场景
	static cocos2d::Scene* createScene();
	//init函数，完成初始化操作
	virtual bool init();
	// 将屏幕坐标转换为TileMap坐标
	Vec2 tileCoordForPosition(Vec2 position);
   //判断键盘是否按住
	bool isKeyPressed(EventKeyboard::KeyCode code);
	//求方格的中心坐标  
	Vec2 centerPositionForTileCoord(const cocos2d::Vec2 &TileCoord);
	//键盘按下后的事情
	void keyPressedDuration(EventKeyboard::KeyCode code);
	//更新位置
	void UpdatePosition(float delta);
	/**
	*以player脚底中心为锚点，以身体中心为原点
	*建立八个检测点，每次传入需要使用的点
	*进行检测，如果下一步要碰到障碍物，则停止并后退1像素（防止卡住）
	*/
	void rightMove(int Move,int flag1,int flag2,int flag3);
	// 回调更新函数，该函数每一帧都会调用
	void update(float delta);
	//使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
	CREATE_FUNC(GameScene);
	//判断是否会碰到障碍物
	bool collide(Vec2 position,int type);
private:

	//键盘按键记录
	std::map<cocos2d::EventKeyboard::KeyCode, bool> keys;
	MapLayer * map;  // 地图
	Player* _player;  // 玩家精灵
	Vector<Sprite* > _popVector;//泡泡层
	Vector<Sprite*> _breakableBlockVector;//可爆炸物数组
	int power = 2;//泡泡威力
	bool on_pop=false;
	//记录上次行动方向，左右上下
	bool moveRecord[4] = { false,false,false,false };
	double move[4][2] = { { -2,0 },{ 2,0 },{ 0,2 },{ 0,-2 } };
};

#endif
