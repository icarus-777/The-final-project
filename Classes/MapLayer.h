/**
* 内部有TMXTiledMap，可以通过getMap调用
* 目前只支持默认地图创建，可以添加地图选择功能
* 通过boom函数实现泡泡爆炸与地图的交互
* judge_collide函数判断特定位置是否是障碍物
* create_BlockVector用来创建障碍物数组
*/
#ifndef _MAP_LAYER_
#define _MAP_LAYER_

#include "cocos2d.h"
#include "SpriteGift.h"
#include "position.h"
USING_NS_CC;


enum Type
{
	PLAYER = 1,
	POP = 2,
	GIFT1 = 3,
	GIFT2 = 4,
	GIFT3 = 5
};

class MapLayer :public Layer
{
public:
	virtual bool init1(int a);
	TMXTiledMap* getMap() { return _map; }
	bool boom(Vec2 _BubblePosition[]);
	bool judge_collide(Vec2 position);
	//在breakable的block上放一个精灵 这样就可以发生碰撞
	void create_BlockVector(Vector<Sprite*> &_breakableBlockVector, Vector<SpriteGift*>& _giftVector);
	Animate * getAnimateByName(std::string animName, float delay, int animNum);
	CREATE_FUNC(MapLayer);
private:
	int _setMap = 1;
	TMXTiledMap* _map;  // 地图
	TMXLayer* _collidable;  // 障碍层
	TMXLayer* _boxbottom;//底层
	Vector<TMXLayer*> _layerNames;//层名字
	int breakableBlocks1[12][16] = {
		//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //0
		{ 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0 }, //1
		{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 0 }, //2
		{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }, //3
		{ 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0 }, //4
		{ 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0 }, //5
		{ 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0 }, //6
		{ 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 }, //7
		{ 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0 }, //8
		{ 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0 }, //9
		{ 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0 }, //10
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //11 
	};
	int breakableBlocks2[12][16] = {
		//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //0
		{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, //1
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //2
		{ 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0 }, //3
		{ 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0 }, //4
		{ 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0 }, //5
		{ 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0 }, //6
		{ 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0 }, //7
		{ 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0 }, //8
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //9
		{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 }, //10
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //11 
	};
	int breakableBlocks[12][16] = { 0 };
};
#endif
