#include "GameScene.h"

USING_NS_CC;
Scene* GameScene::createScene()
{
	// 创建一个场景对象
	auto scene = Scene::create();
	// 创建层对象
	auto layer = GameScene::create();
	// 将层作为子节点添加到场景
	scene->addChild(layer);
	// 返回场景对象
	return scene;
}

bool GameScene::init()
{

	if (!Layer::init())
	{
		return false;
	}
	// 获得从窗口大小
	Size visibleSize = Director::getInstance()->getVisibleSize();

	//显示地图
	  map = MapLayer::create();
	  map->setScale(1.5f);
    addChild(map, 0);
<<<<<<< HEAD
=======

>>>>>>> origin/master
	//暂停按钮
	auto* pBreak = MenuItemImage::create("pause 1.png",
		"pause 2.png",
		this,
		menu_selector(MenuSwitch::Break));
	auto* Break = Menu::create(pBreak, NULL);
<<<<<<< HEAD
	Break->setAnchorPoint(Vec2(0, 0));
	Break->setPosition(850, 40);
	Break->setScale(0.9f);
	addChild(Break);
=======
	Break->setAnchorPoint(Vec2(0, 1.2));
	Break->setPosition(visibleSize.width, 0);
	Break->setScale(0.9f);
	addChild(Break);

>>>>>>> origin/master
	//一个玩家
	auto objects = (map->getMap())->getObjectGroup("Object");
	ValueMap mapPlayer = objects->getObject("player");
	int x = mapPlayer.at("x").asInt();
	int y = mapPlayer.at("y").asInt();
	_player =Player::create();
	// 设置位置并添加为地图的子节点
	_player->setPosition(x, y);
	_player->setAnchorPoint(_player->getAnchorPoint() + Vec2(0, -0.5));
	map->getMap()->addChild(_player, 4);

	map->create_BlockVector(_breakableBlockVector, _giftVector);
	// 创建键盘事件监听器
	key = KeyBoard::create();
	key->startKB();
	this->addChild(key);

	// 游戏主循环,每帧都调用的函数
	this->scheduleUpdate();
	return true;
}

//每帧调用这个函数，更新画面
void GameScene::update(float delta) {
	//更新玩家位置
	keyPressedDuration(delta);
	//判断是否碰到道具
	giftcollide(delta);
}

//键盘按下后的事情
void GameScene::keyPressedDuration(float delta) {
	int Move=4;
	if (key->keyBoardDirection < 4&& key->keyBoardDirection>=0)
	{
		if (key->keyBoardDirection != record)
		{
			_player->ForeverMove(key->keyBoardDirection);
			record = key->keyBoardDirection;
		}
		Move = key->keyBoardDirection;
	}
	else if (key->keyBoardDirection == 4)
	{
		_player->StandMove(record);
		record = -1;
	}
	else if(key->keyBoardDirection ==keyBoard_space)
	{
		if (_popVector.size() < popMax)
		{
			//玩家位置
			Vec2 _tilePlayer = tileCoordForPosition(_player->getPosition());
			Vec2 _popPosition = centerPositionForTileCoord(_tilePlayer);
			//放置一个泡泡在格子的中心
			Pop* pop = Pop::create("pop_1.png");
			pop->setAnchorPoint(Vec2(0.5, 0.5));
			pop->setPosition(_popPosition);
			pop->setScale(0.9f);
			_popVector.pushBack(pop);
			//设置泡泡的zorder
			map->getMap()->addChild(pop, 1);
			//从泡泡被放入vector到泡泡爆炸的延时
			DelayTime * _delayDelete = DelayTime::create(3.0f);
			DelayTime *e = DelayTime::create(0.5f);
			auto callFunc1 = CallFunc::create([=] {
				pop->runAction(pop->centerBoom1());
			});
			auto callFunc2 = CallFunc::create([=] {
				auto popx = _popVector.at(0);//获得第一个泡泡
				Vec2 popPosition = popx->getPosition();
				popx->runAction(pop->centerBoom2());
				popx->destroy(power,popx, _breakableBlockVector,map);
				SimpleAudioEngine::getInstance()->playEffect("explode.wav");
				map->boom(popx->setBubblePosition(power, popPosition));
			});
			auto callFunc3 = CallFunc::create([=] {
				//泡泡爆炸时，获取地图上现存的第一个被放置的泡泡并删除它
				auto pop = _popVector.at(0);
				map->getMap()->removeChild(pop);
				_popVector.eraseObject(pop);
			});
			//创建一个序列动作，放置泡泡->泡泡加入泡泡vector->泡泡爆炸
			auto action = Sequence::create(callFunc1, _delayDelete, callFunc2, e, callFunc3, NULL);
			pop->runAction(action);
		}
		key->keyBoardDirection = keyBoard_stand;
	}
	//开始判断是否撞墙
	if (Move != 4)
	{
		if (Move == 0)
			rightMove(Move, 2, 2, 6);
		if (Move == 1)
			rightMove(Move, 3, 3, 7);
		if (Move == 2)
			rightMove(Move, 1, 4, 5);
		if (Move == 3)
			rightMove(Move, 0, 6, 7);
	}
}

void GameScene::rightMove(int Move,int flag1,int flag2,int flag3)
{
	move[0][0] = -(2 + addSpeed);
	move[1][0] = 2 + addSpeed;
	move[2][1] = 2 + addSpeed;
	move[3][1] = -(2 + addSpeed);
	auto destination = _player->getPosition() + Vec2(move[Move][0], move[Move][1]);
	auto moveTo = MoveTo::create(0.01, destination);
	auto tmp=tileCoordForPosition(destination);     //测试使用
	auto delt=15;
	//判断是否在泡泡上
	auto now= tileCoordForPosition(_player->getPosition());
	if (!_popVector.empty()&&now != tileCoordForPosition(_popVector.at(_popVector.size() - 1)->getPosition()))
		on_pop = false;
	else
		on_pop = true;
	Vec2 tileCoord[8];//0 下,1 上, 2 左, 3 右 ,4 左上, 5 右上, 6 左下, 7 右下
	tileCoord[0] = tileCoordForPosition(destination + Vec2(0, -3));
	tileCoord[1] = tileCoordForPosition(destination + Vec2(0, 2*delt));
	tileCoord[2] = tileCoordForPosition(destination + Vec2(-delt, delt));
	tileCoord[3] = tileCoordForPosition(destination + Vec2(delt, delt));
	tileCoord[4] = tileCoordForPosition(destination + Vec2(-delt, 2*delt));
	tileCoord[5] = tileCoordForPosition(destination + Vec2(delt, 2*delt));
	tileCoord[6] = tileCoordForPosition(destination + Vec2(-delt, -3)); 
	tileCoord[7] = tileCoordForPosition(destination + Vec2(delt, -3));
	if (!collide(tileCoord[flag1], PLAYER) && !collide(tileCoord[flag2], PLAYER) && !collide(tileCoord[flag3], PLAYER))
	{
		for (int i = 0; i < 4; i++)
		{
			moveRecord[i] = false;
		}
		_player->runAction(moveTo);
	}
	else
	{
		if (!moveRecord[Move])
		{
			auto destination = _player->getPosition() + Vec2(-move[Move][0], -move[Move][1]);
			auto moveTo = MoveTo::create(0.01, destination);
			_player->runAction(moveTo);
		}
		for (int i = 0; i < 4; i++)
		{
			moveRecord[i] = false;
		}
		moveRecord[Move] = true;
	}
}

bool GameScene::collide(Vec2 position,int type)
{
	//泡泡也是障碍物
	int pop_size;
	if (on_pop)
		pop_size = _popVector.size() - 1;
	else
		pop_size = _popVector.size();
	for (int i = 0; i < pop_size; i++) {
		Vec2 popPosition = _popVector.at(i)->getPosition();
		Vec2 popPositionForTileMap = tileCoordForPosition(popPosition);
		if (position == popPositionForTileMap && type == PLAYER) {
			return true;
		}
	}
	//检测是否可炸
	for (int i = 0; i < _breakableBlockVector.size(); i++)
	{

		Vec2 popPosition = _breakableBlockVector.at(i)->getPosition();
		Sprite* erase = _breakableBlockVector.at(i);
		Vec2 popPositionForTileMap = tileCoordForPosition(popPosition);
		if (position == popPositionForTileMap)
		{
			if (type == POP)
				_breakableBlockVector.eraseObject(erase);
			return true;
		}
	}
	//使用tileGid函数获取TileMap坐标系里的GID，GID是“全局唯一标示”
	if (map->judge_collide(position))
		return 1;
	return 0;
}
void GameScene::giftcollide(float delta)
{
	Vec2 playerposition = _player->getPosition();
	Vec2 playerPositionForTileMap = tileCoordForPosition(playerposition);
	for (int i = 0; i < _giftVector.size(); i++)
	{
		
		Vec2 giftPosition = _giftVector.at(i)->getPosition();
		Vec2 giftPositionForTileMap = tileCoordForPosition(giftPosition);
		SpriteGift* erase = _giftVector.at(i);
		if (playerPositionForTileMap == giftPositionForTileMap)
		{
			if (erase->getTag() == GIFT1)
			{
				power++;
			}
			else if (erase->getTag() == GIFT2)
			{
				addSpeed = 0.2 + addSpeed;
			}
			else if (erase->getTag() == GIFT3)
			{
				popMax++;
			}
			SimpleAudioEngine::getInstance()->playEffect("get.wav");
			map->getMap()->removeChild(erase);
			_giftVector.eraseObject(erase);
		}
	}
}



