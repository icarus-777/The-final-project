#include "MapLayer.h"
bool MapLayer::init()
{
	//显示地图
	_map = TMXTiledMap::create("Map.tmx");
	// 把地图的锚点和位置都设置为原点，这样可以使地图的左下角与屏幕的左下角对齐
	//设置地图的锚点
	_map->setAnchorPoint(Vec2::ZERO);
	//设置地图位置
	_map->setPosition(0, 0);
	this->addChild(_map,0);
	// 获取障碍层，并设置障碍层不可见
	_collidable = _map->getLayer("collidable");
	_collidable->setVisible(false);
	_boxbottom = _map->getLayer("boxbottom");
	_layerNames.pushBack(_map->getLayer("boxtop"));
	_layerNames.pushBack(_map->getLayer("housetop"));
	return true;
}

bool MapLayer::boom(Vec2 _BubblePosition[])
{
	Sprite* Up;//被炸物的上半部分
	int _GID[4];//GID
	Sprite* emptySprite = Sprite::create();//空精灵
	emptySprite->setPosition(Vec2(-1, -1));
	Vector<Sprite*> _erasePopvector;//临时被摧毁建筑数组
	for (int i = 0; i < 4; i++)
	{
		//四个方向的爆炸，没有建筑就加一个空的精灵
		if (_boxbottom->getTileAt(_BubblePosition[i]) != nullptr)
		{
			_erasePopvector.pushBack(_boxbottom->getTileAt(_BubblePosition[i]));
		}
		else
		{
			_erasePopvector.pushBack(emptySprite);
		}

		_GID[i] = _boxbottom->getTileGIDAt(_BubblePosition[i]);
		//建筑要被设置为不可见
		if (_GID[i])
		{
			_erasePopvector.at(i)->setVisible(false);//下半部分
			Value properties = (_map->getPropertiesForGID(_GID[i]));
			ValueMap valuemap = properties.asValueMap();
			std::string value = valuemap.at("breakable").asString();
			if (value == "true")
			{
				Up = _layerNames.at(0)->getTileAt(_BubblePosition[i] + Vec2(0, -1));//上半部分
				Up->setVisible(false);
			}
		}
	}
	_erasePopvector.clear();
	return true;
}
bool MapLayer::judge_collide(Vec2 position)
{
	//使用tileGid函数获取TileMap坐标系里的GID，GID是“全局唯一标示”
	int tileGid = _collidable->getTileGIDAt(position);
	if (tileGid)
	{
		Value properties = (_map->getPropertiesForGID(tileGid));
		ValueMap valueMap = properties.asValueMap();
		//查找ValueMap，如果有可碰撞物体，直接返回
		std::string value = valueMap.at("collidable").asString();
		if (value.compare("true") == 0)
		{
			return 1;
		}
	}
	return false;
}



void MapLayer::create_BlockVector(Vector<Sprite*>& _breakableBlockVector)
{
	int breakableBlocks[13][17] = {
		//0  1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //0
		{ 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0 }, //1
		{ 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0 }, //2
		{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 }, //3
		{ 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0 }, //4
		{ 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 1, 1, 0 }, //5
		{ 0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0 }, //6
		{ 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0 }, //7
		{ 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0 }, //8
		{ 0, 1, 0, 1, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0 }, //9
		{ 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0 }, //10
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //11 
	};
	for (int i = 0; i < 13; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if (breakableBlocks[i][j] == 1)
			{
				auto test = Sprite::create();
				test->setAnchorPoint(Vec2(0.5, 0.5));
				test->setPosition(centerPositionForTileCoord(Vec2(j, i)));
				_breakableBlockVector.pushBack(test);
				this->addChild(test, 5);
			}
		}
	}
}
Vec2 MapLayer::centerPositionForTileCoord(const cocos2d::Vec2 &TileCoord) {
	Size mapSize = _map->getMapSize();//TileMap坐标的行数，列数
	Size tileSize = _map->getTileSize();//图块大小
	int x = TileCoord.x * tileSize.width + tileSize.width / 2;
	int y = (mapSize.height - TileCoord.y)*tileSize.height - tileSize.height / 2;
	return Vec2(x, y);
}
