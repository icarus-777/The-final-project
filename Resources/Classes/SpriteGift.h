#ifndef __SPRITE_GIFT_H__
#define __SPRITE_GIFT_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;

class SpriteGift : public cocos2d::Sprite
{
private:
	int _tag;  // 哪种道具
public:
	// 静态的create函数
	static SpriteGift* create(const std::string& filename)
	{
		SpriteGift *sprite = new SpriteGift();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	void setTag(int tag) { _tag = tag; }
	int getTag() { return _tag; }
};

#endif
