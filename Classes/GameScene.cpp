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
	map->setScale(1.6f);
    addChild(map, 0);

	// 获取障碍层，并设置障碍层不可见
	_collidable = map->getLayer("collidable");
	_collidable->setVisible(false);

	// ①获取精灵帧缓存的单例对象，并读取animation.plist文件将精灵帧纹理添加到精灵帧缓存当中
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("player.plist");
	//一个玩家
	_player = Sprite::create("player_down_1.png");
	// 设置位置并添加为地图的子节点
	_player->setPosition(50, visibleSize.height / 2 );
	//遮挡效果实现将所有需要被玩家遮挡的物体的zOrder值都设置成<2，所以玩家的zOrder设计成2，就可以实现遮挡效果了。											  
	map->addChild(_player, 2);
	// 创建键盘事件监听器
	auto keyBoardListener = EventListenerKeyboard::create();
	//当键被按下，map中这个键的值被设为true
	keyBoardListener->onKeyPressed = [=](EventKeyboard::KeyCode code, Event* event) {
		keys[code] = true;
	};
	//当键被松开，map中这个键的值被设为false
	keyBoardListener->onKeyReleased = [=](EventKeyboard::KeyCode code, Event* event) {
		keys[code] = false;
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
		downArrow = EventKeyboard::KeyCode::KEY_DOWN_ARROW;
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
	default:
		break;
	}
	if (keys[EventKeyboard::KeyCode::KEY_LEFT_ARROW] == true) {
		// ②使用精灵帧缓存中的精灵创建一个动画，并设置属性
		auto animation = Animation::create();
		// 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
		for (int i = 1; i <= 6; i++) {
			std::string szName = StringUtils::format("player_left_%d.png", i);
			// 将单张图片添加为精灵帧（即动画帧）
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
		}
		// 设置动画播放的属性，3秒/15帧
		animation->setDelayPerUnit(1.0f / 6.0f);

		// ③用动画帧缓冲AnimationCache缓存所有动画和动画帧
		AnimationCache::getInstance()->addAnimation(animation, "walk");
		// ⑥从动画帧缓冲AnimationCache中获取之前缓存的动画
		auto walk = AnimationCache::getInstance()->getAnimation("walk");
		// ⑦创建动画动作
		auto animate = Animate::create(walk);
		// ⑧精灵执行动作
		_player->runAction(animate);
		auto moveTo = MoveTo::create(0.06, _player->getPosition() + Vec2(offsetX, offsetY));
		_player->runAction(moveTo);
	}
	else if (keys[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] == true) {
		// ②使用精灵帧缓存中的精灵创建一个动画，并设置属性
		auto animation = Animation::create();
		// 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
		for (int i = 1; i <= 6; i++) {
			std::string szName = StringUtils::format("player_right_%d.png", i);
			// 将单张图片添加为精灵帧（即动画帧）
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
		}
		// 设置动画播放的属性，3秒/15帧
		animation->setDelayPerUnit(1.0f / 6.0f);

		// ③用动画帧缓冲AnimationCache缓存所有动画和动画帧
		AnimationCache::getInstance()->addAnimation(animation, "walk");
		// ⑥从动画帧缓冲AnimationCache中获取之前缓存的动画
		auto walk = AnimationCache::getInstance()->getAnimation("walk");
		// ⑦创建动画动作
		auto animate = Animate::create(walk);
		// ⑧精灵执行动作
		_player->runAction(animate);
		auto moveTo = MoveTo::create(0.06, _player->getPosition() + Vec2(offsetX, offsetY));
		_player->runAction(moveTo);
	}
	else if (keys[EventKeyboard::KeyCode::KEY_UP_ARROW] == true) {
		// ②使用精灵帧缓存中的精灵创建一个动画，并设置属性
		auto animation = Animation::create();
		// 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
		for (int i = 1; i <= 6; i++) {
			std::string szName = StringUtils::format("player_up_%d.png", i);
			// 将单张图片添加为精灵帧（即动画帧）
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
		}
		// 设置动画播放的属性，3秒/15帧
		animation->setDelayPerUnit(1.0f / 6.0f);

		// ③用动画帧缓冲AnimationCache缓存所有动画和动画帧
		AnimationCache::getInstance()->addAnimation(animation, "walk");
		// ⑥从动画帧缓冲AnimationCache中获取之前缓存的动画
		auto walk = AnimationCache::getInstance()->getAnimation("walk");
		// ⑦创建动画动作
		auto animate = Animate::create(walk);
		// ⑧精灵执行动作
		_player->runAction(animate);
		auto moveTo = MoveTo::create(0.06, _player->getPosition() + Vec2(offsetX, offsetY));
		_player->runAction(moveTo);
	}
	else if (keys[EventKeyboard::KeyCode::KEY_DOWN_ARROW] == true) {
		// ②使用精灵帧缓存中的精灵创建一个动画，并设置属性
		auto animation = Animation::create();
		// 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
		for (int i = 1; i <= 6; i++) {
			std::string szName = StringUtils::format("player_down_%d.png", i);
			// 将单张图片添加为精灵帧（即动画帧）
			animation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(szName));
		}
		// 设置动画播放的属性，3秒/15帧
		animation->setDelayPerUnit(1.0f / 6.0f);

		// ③用动画帧缓冲AnimationCache缓存所有动画和动画帧
		AnimationCache::getInstance()->addAnimation(animation, "walk");
		// ⑥从动画帧缓冲AnimationCache中获取之前缓存的动画
		auto walk = AnimationCache::getInstance()->getAnimation("walk");
		// ⑦创建动画动作
		auto animate = Animate::create(walk);
		// ⑧精灵执行动作
		_player->runAction(animate);
		auto moveTo = MoveTo::create(0.06, _player->getPosition() + Vec2(offsetX, offsetY));
		_player->runAction(moveTo);
	}
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

