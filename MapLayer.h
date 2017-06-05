/**
* 内部有TMXTiledMap，可以通过getMap调用
* 目前只支持默认地图创建，可以添加地图选择功能
* 通过boom函数实现泡泡爆炸与地图的交互
* judge_collide函数判断特定位置是否是障碍物
* create_BlockVector用来创建障碍物数组
*/
#ifndef _MAP_LAYER_
#define _MAP_LAYER

#include "cocos2d.h"

USING_NS_CC;


class MapLayer :public Layer
{
public:
	virtual bool init();
	TMXTiledMap* getMap() { return _map; }
	bool boom(Vec2 _BubblePosition[]);
	bool judge_collide(Vec2 position);
	//求方格的中心坐标  
	Vec2 centerPositionForTileCoord(const cocos2d::Vec2 &TileCoord);
	//在breakable的block上放一个精灵 这样就可以发生碰撞
	void create_BlockVector(Vector<Sprite*> &_breakableBlockVector);
	CREATE_FUNC(MapLayer);
private:
	TMXTiledMap* _map;  // 地图
	TMXLayer* _collidable;  // 障碍层
	TMXLayer* _boxbottom;//底层
	Vector<TMXLayer*> _layerNames;//层名字
};
#endif
