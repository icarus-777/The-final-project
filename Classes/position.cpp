#include "position.h"

Vec2 centerPositionForTileCoord(const cocos2d::Vec2 &TileCoord) 
{
	int x = TileCoord.x * 40 + 40 / 2;
	int y = (12 - TileCoord.y) * 40 - 40 / 2;
	return Vec2(x, y);
}
Vec2 tileCoordForPosition(Vec2 position)
{// CC_CONTENT_SCALE_FACTOR Retina返回2，否则返回1
 // 玩家位置的x除以地图的宽，得到的是地图横向的第几个格子（tile）
 // 地图宽计算：26[格子] * 64[图块的宽] = 1680[地图宽]
 // 假如精灵在的x坐标是640，则精灵所在地图的格子计算：640[精灵位置] / 64[图块的宽] = 10 [格子]
	int x = (int)(position.x / (40 / CC_CONTENT_SCALE_FACTOR()));
	// 玩家位置的y除以地图的高，得到的是地图纵向第几个格子（tile），
	// 但是因为cocos2d-x的y轴（左下角）和TileMap的y轴（左上角）轴相反，所以使用地图的高度减去玩家位置的y
	float pointHeight = 40 / CC_CONTENT_SCALE_FACTOR();
	int y = (int)((12 * pointHeight - position.y) / pointHeight);
	return Vec2(x, y);
}