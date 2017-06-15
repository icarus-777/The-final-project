#include "MapLayer.h"

bool MapLayer::init1(int a)
{
	_setMap = a;
	//显示地图
	if (_setMap == 1)
	{
		_map = TMXTiledMap::create("map1.tmx");
		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < 16; j++)
			{
					breakableBlocks[i][j] = breakableBlocks1[i][j];
			}
		}
	}
	else if (_setMap == 2)
	{
		_map = TMXTiledMap::create("map2.tmx");
		for (int i = 0; i < 12; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				breakableBlocks[i][j] = breakableBlocks2[i][j];
			}
		}
	}
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
	Vector<Action*> DestroyAction;//摧毁效果临时动作数组
	Vector<Sprite*> DestroySprite;
	for (int i = 0; i < 4; i++)
	{
		int m = _BubblePosition[i].x;
		int n = _BubblePosition[i].y;
		//四个方向的爆炸，没有建筑就加一个空的精灵
		if (_boxbottom->getTileAt(_BubblePosition[i]) != nullptr && breakableBlocks[n][m] == 1)
		{
			breakableBlocks[n][m] = 0;
			Sprite* destroySprite = Sprite::create();
			destroySprite->setPosition(centerPositionForTileCoord(_BubblePosition[i]));
			destroySprite->setAnchorPoint(Vec2(0.5, 0.25));
			DestroySprite.pushBack(destroySprite);//将精灵放入数组
			_map->addChild(destroySprite, 10);
			auto boomAnimation = Animation::create();
			for (unsigned i = 1; i <= 2; ++i)
			{
				boomAnimation->addSpriteFrameWithFile(StringUtils::format("Boom%d.png", i));
			}
			boomAnimation->setDelayPerUnit(1.0f / 8.0f);
			auto boomAnimate = Animate::create(boomAnimation);
			auto hideBoom = Hide::create();
			auto boomAction = Sequence::create(boomAnimate, hideBoom, NULL);
			DestroyAction.pushBack(boomAction);
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
	//摧毁效果
	while (DestroySprite.size()>0)
	{
		auto sprite = DestroySprite.at(0);
		auto action = DestroyAction.at(0);
		sprite->runAction(action);
		DestroySprite.eraseObject(sprite);
		DestroyAction.eraseObject(action);
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


void MapLayer::create_BlockVector(Vector<Sprite*>& _breakableBlockVector, Vector<SpriteGift*>& _giftVector)
{
	srand((int)time(NULL));
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			int m = (int)(100.0*rand() / RAND_MAX + 1);
			if (breakableBlocks[i][j] == 1)
			{
				Vec2 giftPosition = centerPositionForTileCoord(Vec2(j, i));
				auto test = Sprite::create();
				test->setAnchorPoint(Vec2(0.5, 0.5));
				test->setPosition(giftPosition);
				_breakableBlockVector.pushBack(test);
				this->addChild(test, 0);
				if (m <= 10)
				{//药水
					SpriteGift* Gift1 = SpriteGift::create("Gift1_1.png");
					Gift1->setAnchorPoint(Vec2(0.5, 0.5));
					Gift1->setPosition(giftPosition);
					_map->addChild(Gift1, 1);
					Gift1->setTag(GIFT1);
					_giftVector.pushBack(Gift1);
					auto animate = getAnimateByName("Gift1_", 0.1f, 3);
					auto repeatAnimate = RepeatForever::create(animate);
					Gift1->runAction(repeatAnimate);
				}
				else if (m >= 15 && m <= 25)
				{//鞋子
					SpriteGift* Gift2 = SpriteGift::create("Gift2_1.png");
					Gift2->setAnchorPoint(Vec2(0.5, 0.5));
					Gift2->setPosition(giftPosition);
					_map->addChild(Gift2, 1);
					Gift2->setTag(GIFT2);
					_giftVector.pushBack(Gift2);
					auto animate = getAnimateByName("Gift2_", 0.1f, 3);
					auto repeatAnimate = RepeatForever::create(animate);
					Gift2->runAction(repeatAnimate);
				}
				else if (m >= 30 && m <= 50)
				{//泡泡
					SpriteGift* Gift3 = SpriteGift::create("Gift3_1.png");
					Gift3->setAnchorPoint(Vec2(0.5, 0.5));
					Gift3->setPosition(giftPosition);
					_map->addChild(Gift3, 1);
					Gift3->setTag(GIFT3);
					_giftVector.pushBack(Gift3);
					auto animate = getAnimateByName("Gift3_", 0.1f, 3);
					auto repeatAnimate = RepeatForever::create(animate);
					Gift3->runAction(repeatAnimate);
				}
			}
		}
	}
}
Animate * MapLayer::getAnimateByName(std::string animName, float delay, int animNum)
{
	// 创建一个动画
	Animation* animation = Animation::create();
	// 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
	for (unsigned int i = 1; i <= animNum; i++) {
		std::string frameName = animName;

		//在动画帧名称后加上序号
		frameName.append(StringUtils::format("%d", i)).append(".png");
		animation->addSpriteFrameWithFile(frameName.c_str());
	}
	//设置动画延时
	animation->setDelayPerUnit(delay);

	//在播放完动画时恢复到初始帧
	animation->setRestoreOriginalFrame(true);
	Animate* animate = Animate::create(animation);

	return animate;
}
