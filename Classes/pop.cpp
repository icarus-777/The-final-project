#include "pop.h"


Animate* Pop::centerBoom1()
{
	// ②使用精灵帧缓存中的精灵创建一个动画，并设置属性
	auto animation = Animation::create();
	// 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
	for (int i = 1; i <= 3; i++) {
		animation->addSpriteFrameWithFileName(StringUtils::format("pop_%d.png", i));
	}
	// 设置动画播放的属性，3秒/15帧
	animation->setDelayPerUnit(1.0f / 3.0f);
	// 让精灵对象在动画执行完后恢复到最初状态
	animation->setRestoreOriginalFrame(false);
	animation->setLoops(-1);//表示循环播放
	auto action = Animate::create(animation);
	return action;
}
Sequence* Pop::centerBoom2()
{
	// ②使用精灵帧缓存中的精灵创建一个动画，并设置属性
	auto animation = Animation::create();
	// 循环从精灵帧缓存中获取与图片名称相对应的精灵帧组成动画
	for (int i = 4; i <= 7; i++) {
		animation->addSpriteFrameWithFileName(StringUtils::format("pop_%d.png", i));
	}
	// 设置动画播放的属性，3秒/15帧
	animation->setDelayPerUnit(1.0f / 12.0f);
	// 让精灵对象在动画执行完后恢复到最初状态
	animation->setRestoreOriginalFrame(false);
	animation->setLoops(1);//表示循环播放
	auto animate = Animate::create(animation);
	auto hide = Hide::create();//放完之后隐藏
	auto action = Sequence::create(animate, hide, NULL);
	return action;
}

void Pop::destroy(int& power, Pop*& popx, Vector<Sprite*>& _breakableBlockVector, MapLayer*& map)
{
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
	leftWaterLength = power,
	rightWaterLength = power,
	upWaterLength = power,
	downWaterLength = power;

	//判断水柱长度是多少
	for (int i = 0; i < leftWaterLength; i++)
	{
		if (collide(Vec2(tileCoordForPosition(popx->getPosition()).x - i - 1, tileCoordForPosition(popx->getPosition()).y), POP,  _breakableBlockVector,map))//缺一个函数
		{
			leftWaterLength = i;
			break;
		}
	}

	for (int i = 0; i < rightWaterLength; i++)
		if (collide(Vec2(tileCoordForPosition(popx->getPosition()).x + i + 1, tileCoordForPosition(popx->getPosition()).y), POP, _breakableBlockVector, map))
		{
			rightWaterLength = i;
			break;
		}

	for (int i = 0; i < upWaterLength; i++)
		if (collide(Vec2(tileCoordForPosition(popx->getPosition()).x, tileCoordForPosition(popx->getPosition()).y - i - 1), POP, _breakableBlockVector, map))
		{
			upWaterLength = i;
			break;
		}

	for (int i = 0; i < downWaterLength; i++)
		if (collide(Vec2(tileCoordForPosition(popx->getPosition()).x, tileCoordForPosition(popx->getPosition()).y + i + 1), POP, _breakableBlockVector, map))
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
}
bool Pop::collide(Vec2 position, int type, Vector<Sprite*>& _breakableBlockVector, MapLayer*& map)
{
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

Vec2* Pop::setBubblePosition(int & power, Vec2 popPosition)
{
	Vec2 popPositionForTileMap = tileCoordForPosition(popPosition);
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
	return _BubblePosition;
}
