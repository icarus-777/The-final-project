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
	  map = TMXTiledMap::create("cnm.tmx");
	  // 把地图的锚点和位置都设置为原点，这样可以使地图的左下角与屏幕的左下角对齐
	//设置地图的锚点
	map->setAnchorPoint(Vec2::ZERO);
	//设置地图位置
	map->setPosition(0, 0);
	map->setScale(1.5f);
    addChild(map, 0);
	//一个玩家
	_player = Sprite::create("player_down_1.png");
	//设置玩家锚点
	_player->setAnchorPoint(Vec2(0.5,0.3));
	// 设置位置并添加为地图的子节点
	_player->setPosition(40, 40 );
	_player->setScale(0.8f);
	//遮挡效果实现将所有需要被玩家遮挡的物体的zOrder值都设置成<2，所以玩家的zOrder设计成2，就可以实现遮挡效果了。											  
	map->addChild(_player, 2);
	//将四个方向的移动图加入缓存
	_player_texture_left = CCTextureCache::sharedTextureCache()->addImage("player_left_1.png");
	_player_texture_right = CCTextureCache::sharedTextureCache()->addImage("player_right_1.png");
	_player_texture_up = CCTextureCache::sharedTextureCache()->addImage("player_up_1.png");
	_player_texture_down = CCTextureCache::sharedTextureCache()->addImage("player_down_1.png");
	// 创建键盘事件监听器
	auto keyBoardListener = EventListenerKeyboard::create();
	//当键被按下，map中这个键的值被设为true
	keyBoardListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {
		keys[code] = true;
		if (code != EventKeyboard::KeyCode::KEY_SPACE) {
			_player->stopAllActions();
			this->ForeverMove(code);
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
			this->ForeverMove(leftArrow);
		}
		else if (isKeyPressed(rightArrow)) {
			this->ForeverMove(rightArrow);
		}
		else if (isKeyPressed(upArrow)) {
			this->ForeverMove(upArrow);
		}
		else if (isKeyPressed(downArrow)) {
			this->ForeverMove(downArrow);
		}
		else {
			if ((EventKeyboard::KeyCode::KEY_LEFT_ARROW == code)) {
				_player->setTexture(_player_texture_left);
			}
			else if (EventKeyboard::KeyCode::KEY_RIGHT_ARROW == code) {
				_player->setTexture(_player_texture_right);
			}
			else if (EventKeyboard::KeyCode::KEY_UP_ARROW == code) {
				_player->setTexture(_player_texture_up);
			}
			else if (EventKeyboard::KeyCode::KEY_DOWN_ARROW == code) {
				_player->setTexture(_player_texture_down);
			}
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
	if ((isKeyPressed(space) && arrow)) {
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
	else if (isKeyPressed(space)) {
		keyPressedDuration(space);
	}
}


//每帧调用这个函数，更新画面
void GameScene::update(float delta) {
	//更新玩家位置
	UpdatePosition(delta);
}


//键盘按下后的事情
void GameScene::keyPressedDuration(EventKeyboard::KeyCode code) {
	int offsetX = 0, offsetY = 0;
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		offsetX -= 2;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		offsetX += 2;
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		offsetY += 2;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		offsetY -= 2;
		break;
	case EventKeyboard::KeyCode::KEY_SPACE: 
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
		map->addChild(pop, 1);
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
			animation->setDelayPerUnit(1.0f / 8.0f);
			// 让精灵对象在动画执行完后恢复到最初状态
			animation->setRestoreOriginalFrame(false);
			animation->setLoops(1);//表示循环播放
			auto animate = Animate::create(animation);
			pop->runAction(animate);

		});
		auto callFunc3 = CallFunc::create([=] {
			//泡泡爆炸时，获取地图上现存的第一个被放置的泡泡并删除它
			auto pop = _popVector.at(0);
			map->removeChild(pop);
			_popVector.eraseObject(pop);
		});
		//创建一个序列动作，放置泡泡->泡泡加入泡泡vector->泡泡爆炸
		auto action = Sequence::create(callFunc1, _delayDelete, callFunc2, e,callFunc3,NULL);
		pop->runAction(action);
		break;
	}
		auto moveTo = MoveTo::create(0.06, _player->getPosition() + Vec2(offsetX, offsetY));
		_player->runAction(moveTo);
}
// 将屏幕坐标转换为TileMap坐标，暂时没用
Vec2 GameScene::tileCoordForPosition(Vec2 position)
{// CC_CONTENT_SCALE_FACTOR Retina返回2，否则返回1
 // 玩家位置的x除以地图的宽，得到的是地图横向的第几个格子（tile）
 // 地图宽计算：26[格子] * 64[图块的宽] = 1680[地图宽]
 // 假如精灵在的x坐标是640，则精灵所在地图的格子计算：640[精灵位置] / 64[图块的宽] = 10 [格子]
	int x = (int)(position.x / (map->getTileSize().width / CC_CONTENT_SCALE_FACTOR()));
	// 玩家位置的y除以地图的高，得到的是地图纵向第几个格子（tile），
	// 但是因为cocos2d-x的y轴（左下角）和TileMap的y轴（左上角）轴相反，所以使用地图的高度减去玩家位置的y
	float pointHeight = map->getTileSize().height / CC_CONTENT_SCALE_FACTOR();
	int y = (int)((map->getMapSize().height * pointHeight - position.y) / pointHeight);
	return Vec2(x,y);
}
//方格的中心坐标
Vec2 GameScene::centerPositionForTileCoord(const cocos2d::Vec2 &TileCoord) {
	Size mapSize = map->getMapSize();//TileMap坐标的行数，列数
	Size tileSize =map->getTileSize();//图块大小
	int x = TileCoord.x * tileSize.width + tileSize.width / 2;
	int y = (mapSize.height - TileCoord.y)*tileSize.height - tileSize.height / 2;
	return Vec2(x, y);
}
void GameScene::ForeverMove(EventKeyboard::KeyCode code) {

	Animate* animate;
	std::string Name = "player";

	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		animate = getAnimateByName(Name + "_left_", 0.1f, 6);
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		animate = getAnimateByName(Name + "_right_", 0.1f, 6);
		break;
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		animate = getAnimateByName(Name + "_up_", 0.05f, 6);
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		animate = getAnimateByName(Name + "_down_", 0.1f, 6);
		break;
	}
	//创建一个动作 重复执行动画
	auto repeatAnimate = RepeatForever::create(animate);
	_player->runAction(repeatAnimate);
}
//通过动画名字得到相应的动画
Animate * GameScene::getAnimateByName(std::string animName, float delay, int animNum)
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
