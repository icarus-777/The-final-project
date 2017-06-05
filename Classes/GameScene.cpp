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
	auto keyBoardListener = EventListenerKeyboard::create();
	//当键被按下，map中这个键的值被设为true
	keyBoardListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {
		keys[code] = true;
		if (code != EventKeyboard::KeyCode::KEY_SPACE) {
			_player->stopAllActions();
			_player->ForeverMove(code);
		}
	};
	//当键被松开，map中这个键的值被设为false
	keyBoardListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event) {
		keys[code] = false;
		_player->stopAllActions();
		auto
			leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW,
			rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW,
			upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW,
			downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW,
			space = EventKeyboard::KeyCode::KEY_SPACE;
		if (isKeyPressed(leftArrow)) {
			_player->ForeverMove(leftArrow);
		}
		else if (isKeyPressed(rightArrow))
		{
			_player->ForeverMove(rightArrow);
		}
		else if (isKeyPressed(upArrow))
		{
			_player->ForeverMove(upArrow);
		}
		else if (isKeyPressed(downArrow))
		{
			_player->ForeverMove(downArrow);
		}
		else {
			_player->StandMove(code);
		}
	};
	// 响应键盘事件函数
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
	// 游戏主循环,每帧都调用的函数
	this->scheduleUpdate();
	return true;
}



//判断键盘是否按住
bool GameScene::isKeyPressed(EventKeyboard::KeyCode code) {
	return keys[code];
}
//更新玩家位置
void GameScene::UpdatePosition(float delta) {
	//四个方向键
	auto leftArrow = EventKeyboard::KeyCode::KEY_LEFT_ARROW,
		rightArrow = EventKeyboard::KeyCode::KEY_RIGHT_ARROW,
		upArrow = EventKeyboard::KeyCode::KEY_UP_ARROW,
		downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW,
	space = EventKeyboard::KeyCode::KEY_SPACE;

	int arrow = isKeyPressed(leftArrow) + isKeyPressed(rightArrow) + isKeyPressed(upArrow) + isKeyPressed(downArrow);

	//同时按下方向键和空格键，也可放下一个水泡
	if ((isKeyPressed(space) && arrow)||(isKeyPressed(space))) {
		keyPressedDuration(space);
		keys[space] = false;
	}

	//按下方向键，玩家移动
	if (isKeyPressed(leftArrow)) {
		keyPressedDuration(leftArrow);
	}
	else if (isKeyPressed(rightArrow)) {
		keyPressedDuration(rightArrow);
	}
	else if (isKeyPressed(upArrow)) {
		keyPressedDuration(upArrow);
	}
	else if (isKeyPressed(downArrow)) {
		keyPressedDuration(downArrow);
	}
}


//每帧调用这个函数，更新画面
void GameScene::update(float delta) {
	//更新玩家位置
	UpdatePosition(delta);
	//判断是否碰到道具
	giftcollide(delta);
}


//键盘按下后的事情
void GameScene::keyPressedDuration(EventKeyboard::KeyCode code) {
	int Move=4;
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		Move=0;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		Move = 1;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		Move = 2;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		Move = 3;
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		if (_popVector.size() < popMax)
		{
			//玩家位置
			Vec2 _tilePlayer = tileCoordForPosition(_player->getPosition());
			Vec2 _popPosition = centerPositionForTileCoord(_tilePlayer);

			SpriteFrameCache::getInstance()->addSpriteFramesWithFile("popo.plist");

			//放置一个泡泡在格子的中心
			Sprite* pop = Sprite::createWithSpriteFrameName("pop_1.png");
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

				// ②使用精灵帧缓存中的精灵创建一个动画，并设置属性
				auto animation = Animation::create();
				// 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
				for (int i = 1; i <= 3; i++) {
					std::string szName = StringUtils::format("pop_%d.png", i);
					// 将单张图片添加为精灵帧（即动画帧）
					animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
				}
				// 设置动画播放的属性，3秒/15帧
				animation->setDelayPerUnit(1.0f / 3.0f);
				// 让精灵对象在动画执行完后恢复到最初状态
				animation->setRestoreOriginalFrame(false);
				animation->setLoops(-1);//表示循环播放
				auto animate = Animate::create(animation);
				pop->runAction(animate);
			});
			auto callFunc2 = CallFunc::create([=] {
				// ②使用精灵帧缓存中的精灵创建一个动画，并设置属性
				auto animation = Animation::create();
				// 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
				for (int i = 4; i <= 7; i++) {
					std::string szName = StringUtils::format("pop_%d.png", i);
					// 将单张图片添加为精灵帧（即动画帧）
					animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
				}
				// 设置动画播放的属性，3秒/15帧
				animation->setDelayPerUnit(1.0f / 12.0f);
				// 让精灵对象在动画执行完后恢复到最初状态
				animation->setRestoreOriginalFrame(false);
				animation->setLoops(1);//表示循环播放
				auto animate = Animate::create(animation);
				auto hide = Hide::create();//放完之后隐藏
				auto action = Sequence::create(animate, hide, NULL);
				auto popx = _popVector.at(0);//获得第一个泡泡
				Vec2 popPosition = popx->getPosition();
				popx->runAction(action);
				//临时动作数组
				Vector<Action*> leftAction;
				Vector<Action*> rightAction;
				Vector<Action*>	upAction;
				Vector<Action*> downAction;
				//临时精灵数组
				Vector<Sprite*> leftSprite;
				Vector<Sprite*> rightSprite;
				Vector<Sprite*> upSprite;
				Vector<Sprite*> downSprite;
				//水柱长度
				int leftWaterLength = power,
					rightWaterLength = power,
					upWaterLength = power,
					downWaterLength = power;

				//判断水柱长度是多少
				for (int i = 0; i < leftWaterLength; i++)
				{
					if (collide(Vec2(tileCoordForPosition(popx->getPosition()).x - i - 1, tileCoordForPosition(popx->getPosition()).y), POP))//缺一个函数
					{
						leftWaterLength = i;
						break;
					}
				}

				for (int i = 0; i < rightWaterLength; i++)
					if (collide(Vec2(tileCoordForPosition(popx->getPosition()).x + i +1, tileCoordForPosition(popx->getPosition()).y), POP))
					{
						rightWaterLength = i;
						break;
					}

				for (int i = 0; i < upWaterLength; i++)
					if (collide(Vec2(tileCoordForPosition(popx->getPosition()).x, tileCoordForPosition(popx->getPosition()).y - i - 1), POP))
					{
						upWaterLength = i;
						break;
					}

				for (int i = 0; i < downWaterLength; i++)
					if (collide(Vec2(tileCoordForPosition(popx->getPosition()).x, tileCoordForPosition(popx->getPosition()).y + i + 1), POP))
					{
						downWaterLength = i;
						break;
					}
				//水柱动画效果
				if (rightWaterLength != 0)
				{
					for (int i = 1; i <= rightWaterLength; i++)
					{
						auto sprite = Sprite::create();
						sprite->setPosition(Vec2(popx->getPositionX() + 40 * i, popx->getPositionY()));
						rightSprite.pushBack(sprite);//将精灵放入数组
						map->getMap()->addChild(sprite, 10);
						if (rightWaterLength != i)//水柱中间的动画
						{
							auto animation = Animation::create();
							for (int j = 1; j <= 4; ++j)
								animation->addSpriteFrameWithFileName(StringUtils::format("Explosion_right_%d.png", j));
							animation->setDelayPerUnit(0.1f);
							auto animate = Animate::create(animation);
							auto hide = Hide::create();
							auto delay = DelayTime::create(0.f);
							auto action = Sequence::create(delay, animate, hide, NULL);
							rightAction.pushBack(action);
						}
						else {//水柱尾部的动画
							auto animation = Animation::create();
							for (int j = 1; j <= 4; j++)
								animation->addSpriteFrameWithFileName(StringUtils::format("Explosion_right_0%d.png", j));
							animation->setDelayPerUnit(0.1f);
							auto animate = Animate::create(animation);
							auto hide = Hide::create();
							auto delay = DelayTime::create(0.f);
							auto action = Sequence::create(delay, animate, hide, NULL);
							rightAction.pushBack(action);
						}
					}
				}

				if (leftWaterLength != 0)
				{
					for (int i = 1; i <= leftWaterLength; i++)
					{
						auto sprite = Sprite::create();
						sprite->setPosition(Vec2(popx->getPositionX() - 40 * i, popx->getPositionY()));
						leftSprite.pushBack(sprite);//将精灵放入数组
						map->getMap()->addChild(sprite, 10);
						if (leftWaterLength != i)//水柱中间的动画
						{
							auto animation = Animation::create();
							for (int j = 1; j <= 4; j++)
								animation->addSpriteFrameWithFileName(StringUtils::format("Explosion_left_%d.png", j));
							animation->setDelayPerUnit(0.1f);
							auto animate = Animate::create(animation);
							auto hide = Hide::create();
							auto delay = DelayTime::create(0.f);
							auto action = Sequence::create(delay, animate, hide, NULL);
							leftAction.pushBack(action);
						}
						else {//水柱尾部的动画
							auto animation = Animation::create();
							for (int j = 1; j <= 4; j++)
								animation->addSpriteFrameWithFileName(StringUtils::format("Explosion_left_0%d.png", j));
							animation->setDelayPerUnit(0.1f);
							auto animate = Animate::create(animation);
							auto hide = Hide::create();
							auto delay = DelayTime::create(0.f);
							auto action = Sequence::create(delay, animate, hide, NULL);
							leftAction.pushBack(action);
						}
					}
				}
				if (upWaterLength != 0)
				{
					for (int i = 1; i <= upWaterLength; i++)
					{
						auto sprite = Sprite::create();
						sprite->setPosition(Vec2(popx->getPositionX(), popx->getPositionY() + 40 * i));
						upSprite.pushBack(sprite);//将精灵放入数组
						map->getMap()->addChild(sprite, 10);
						if (upWaterLength != i)//水柱中间的动画
						{
							auto animation = Animation::create();
							for (int j = 1; j <= 4; j++)
								animation->addSpriteFrameWithFileName(StringUtils::format("Explosion_up_%d.png", j));
							animation->setDelayPerUnit(0.1f);
							auto animate = Animate::create(animation);
							auto hide = Hide::create();
							auto delay = DelayTime::create(0.f);
							auto action = Sequence::create(delay, animate, hide, NULL);
							upAction.pushBack(action);
						}
						else {//水柱尾部的动画
							auto animation = Animation::create();
							for (int j = 1; j <= 4; j++)
								animation->addSpriteFrameWithFileName(StringUtils::format("Explosion_up_0%d.png", j));
							animation->setDelayPerUnit(0.1f);
							auto animate = Animate::create(animation);
							auto hide = Hide::create();
							auto delay = DelayTime::create(0.f);
							auto action = Sequence::create(delay, animate, hide, NULL);
							upAction.pushBack(action);
						}
					}
				}
				if (downWaterLength != 0)
				{
					for (int i = 1; i <= downWaterLength; i++)
					{
						auto sprite = Sprite::create();
						sprite->setPosition(Vec2(popx->getPositionX(), popx->getPositionY() - 40 * i));
						downSprite.pushBack(sprite);//将精灵放入数组
						map->getMap()->addChild(sprite, 10);
						if (downWaterLength != i)//水柱中间的动画
						{
							auto animation = Animation::create();
							for (int j = 1; j <= 4; j++)
								animation->addSpriteFrameWithFileName(StringUtils::format("Explosion_down_%d.png", j));
							animation->setDelayPerUnit(0.1f);
							auto animate = Animate::create(animation);
							auto hide = Hide::create();
							auto delay = DelayTime::create(0.f);
							auto action = Sequence::create(delay, animate, hide, NULL);
							downAction.pushBack(action);
						}
						else {//水柱尾部的动画
							auto animation = Animation::create();
							for (int j = 1; j <= 4; j++)
								animation->addSpriteFrameWithFileName(StringUtils::format("Explosion_down_0%d.png", j));
							animation->setDelayPerUnit(0.1f);
							auto animate = Animate::create(animation);
							auto hide = Hide::create();
							auto delay = DelayTime::create(0.f);
							auto action = Sequence::create(delay, animate, hide, NULL);
							downAction.pushBack(action);
						}
					}
				}

				//执行所有动画并从数组中删除
				while (rightWaterLength != 0 && rightSprite.size()>0)
				{
					auto sprite = rightSprite.at(0);
					auto action = rightAction.at(0);
					sprite->runAction(action);
					rightSprite.eraseObject(sprite);
					rightAction.eraseObject(action);
				}

				while (leftWaterLength != 0 && leftSprite.size()>0)
				{
					auto sprite = leftSprite.at(0);
					auto action = leftAction.at(0);
					sprite->runAction(action);
					leftSprite.eraseObject(sprite);
						leftAction.eraseObject(action);
				}

				while (upWaterLength != 0 && upSprite.size()>0)
				{
					auto sprite = upSprite.at(0);
					auto action = upAction.at(0);
					sprite->runAction(action);
					upSprite.eraseObject(sprite);
					upAction.eraseObject(action);
				}

				while (downWaterLength != 0 && downSprite.size()>0)
				{
					auto sprite = downSprite.at(0);
					auto action = downAction.at(0);
					sprite->runAction(action);
					downSprite.eraseObject(sprite);
					downAction.eraseObject(action);
				}
				//泡泡位置
				Vec2 popPositionForTileMap = tileCoordForPosition(popPosition);

				Vec2 _BubblePosition[4];
				//四个爆炸位置
				if (leftWaterLength == power)
				{
					_BubblePosition[0] = popPositionForTileMap - Vec2(leftWaterLength, 0);
				}
				else
				{
					_BubblePosition[0] = popPositionForTileMap - Vec2(leftWaterLength + 1, 0);
				}
				if (rightWaterLength == power)
				{
					_BubblePosition[1] = popPositionForTileMap + Vec2(rightWaterLength, 0);
				}
				else
				{
					_BubblePosition[1] = popPositionForTileMap + Vec2(rightWaterLength + 1, 0);
				}
				if (upWaterLength == power)
				{
					_BubblePosition[2] = popPositionForTileMap - Vec2(0, upWaterLength);
				}
				else
				{
					_BubblePosition[2] = popPositionForTileMap - Vec2(0, upWaterLength + 1);
				}
				if (downWaterLength == power)
				{
					_BubblePosition[3] = popPositionForTileMap + Vec2(0, downWaterLength);
				}
				else
				{
					_BubblePosition[3] = popPositionForTileMap + Vec2(0, downWaterLength + 1);
				}
				map->boom(_BubblePosition);
			});
			auto callFunc3 = CallFunc::create([=] {
				//泡泡爆炸时，获取地图上现存的第一个被放置的泡泡并删除它
				auto pop = _popVector.at(0);
				map->removeChild(pop);
				_popVector.eraseObject(pop);
			});
			//创建一个序列动作，放置泡泡->泡泡加入泡泡vector->泡泡爆炸
			auto action = Sequence::create(callFunc1, _delayDelete, callFunc2, e, callFunc3, NULL);
			pop->runAction(action);
		}
		
		break;
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
Vec2 GameScene::centerPositionForTileCoord(const cocos2d::Vec2 &TileCoord) {
	Size mapSize = (map->getMap())->getMapSize();//TileMap坐标的行数，列数
	Size tileSize = (map->getMap())->getTileSize();//图块大小
	int x = TileCoord.x * tileSize.width + tileSize.width / 2;
	int y = (mapSize.height - TileCoord.y)*tileSize.height - tileSize.height / 2;
	return Vec2(x, y);
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
			map->getMap()->removeChild(erase);
			_giftVector.eraseObject(erase);
		}
	}
}


// 将屏幕坐标转换为TileMap坐标，暂时没用
Vec2 GameScene::tileCoordForPosition(Vec2 position)
{// CC_CONTENT_SCALE_FACTOR Retina返回2，否则返回1
 // 玩家位置的x除以地图的宽，得到的是地图横向的第几个格子（tile）
 // 地图宽计算：26[格子] * 64[图块的宽] = 1680[地图宽]
 // 假如精灵在的x坐标是640，则精灵所在地图的格子计算：640[精灵位置] / 64[图块的宽] = 10 [格子]
	int x = (int)(position.x / ((map->getMap())->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	// 玩家位置的y除以地图的高，得到的是地图纵向第几个格子（tile），
	// 但是因为cocos2d-x的y轴（左下角）和TileMap的y轴（左上角）轴相反，所以使用地图的高度减去玩家位置的y
	float pointHeight = (map->getMap())->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int)(((map->getMap())->getMapSize().height * pointHeight - position.y) / pointHeight);
	return Vec2(x,y);
}

