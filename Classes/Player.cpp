#include"Player.h"

bool Player::init1(int a)
{
	_setPlayer = a;
	if (_setPlayer == 1)
	{
		_player = Sprite::create("player_down_1.png");
		_player->setAnchorPoint(_player->getAnchorPoint() + Vec2(0, -0.5));
		_player_texture_left = CCTextureCache::sharedTextureCache()->addImage("player_left_1.png");
		_player_texture_right = CCTextureCache::sharedTextureCache()->addImage("player_right_1.png");
		_player_texture_up = CCTextureCache::sharedTextureCache()->addImage("player_up_1.png");
		_player_texture_down = CCTextureCache::sharedTextureCache()->addImage("player_down_1.png");
		this->addChild(_player);
	}
	else if (_setPlayer == 2)
	{
		_player = Sprite::create("player2_down_1.png");
		_player->setAnchorPoint(_player->getAnchorPoint() + Vec2(0, -0.5));
		_player_texture_left = CCTextureCache::sharedTextureCache()->addImage("player2_left_1.png");
		_player_texture_right = CCTextureCache::sharedTextureCache()->addImage("player2_right_1.png");
		_player_texture_up = CCTextureCache::sharedTextureCache()->addImage("player2_up_1.png");
		_player_texture_down = CCTextureCache::sharedTextureCache()->addImage("player2_down_1.png");
		this->addChild(_player);
	}
	
	return true;
}
void Player::ForeverMove(int code) {

	Animate* animate;
	std::string Name = "player";
	if(_setPlayer == 2)
		Name = "player2";
	_player->stopAllActions();
	switch (code) {
	case keyBoard_left:
		animate = getAnimateByName(Name + "_left_", 0.1f, 6);
		break;
	case keyBoard_right:
		animate = getAnimateByName(Name + "_right_", 0.1f, 6);
		break;
	case keyBoard_up:
		animate = getAnimateByName(Name + "_up_", 0.05f, 6);
		break;
	case keyBoard_down:
		animate = getAnimateByName(Name + "_down_", 0.1f, 6);
		break;
	}
	//创建一个动作 重复执行动画
	auto repeatAnimate = RepeatForever::create(animate);
	_player->runAction(repeatAnimate);
}
void Player::StandMove(int code)
{
	_player->stopAllActions();
	if (keyBoard_left == code) {
		_player->setTexture(_player_texture_left);
	}
	else if (keyBoard_right == code) {
		_player->setTexture(_player_texture_right);
	}
	else if (keyBoard_up == code) {
		_player->setTexture(_player_texture_up);
	}
	else if (keyBoard_down == code) {
		_player->setTexture(_player_texture_down);
	}
}
Animate * Player::getAnimateByName(std::string animName, float delay, int animNum)
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

Animate * Player::die()
{
	auto animation = Animation::create();
	if (_setPlayer == 1)
	{
		for (int i = 1; i <= 11; i++) {
				animation->addSpriteFrameWithFileName(StringUtils::format("player_die_%d.png", i));
			}
	}
	else if (_setPlayer == 2)
	{
		for (int i = 1; i <= 11; i++) {
			animation->addSpriteFrameWithFileName(StringUtils::format("player2_die_%d.png", i));
		}
	}
	// 设置动画播放的属性，3秒/15帧
	animation->setDelayPerUnit(1.0f / 3.0f);
	// 让精灵对象在动画执行完后恢复到最初状态
	animation->setRestoreOriginalFrame(false);
	auto action = Animate::create(animation);
	return action;
}

void Player::setdie()
{
	isalive = false;
}

bool Player::getisalive()
{
	return isalive;
}




