#ifndef __POP_H__
#define __POP_H__

#include "cocos2d.h"
#include "MapLayer.h"
USING_NS_CC;
class Pop : public cocos2d::Sprite
{
private:
	int leftWaterLength;
	int rightWaterLength;
	int upWaterLength;
	int downWaterLength;
	Vec2 _BubblePosition[4];
public:
	static Pop* create(const std::string& filename)
	{
		Pop *sprite = new Pop();
		if (sprite && sprite->initWithFile(filename))
		{
			sprite->autorelease();
			return sprite;
		}
		CC_SAFE_DELETE(sprite);
		return nullptr;
	}
	Animate* centerBoom1();
	Sequence* centerBoom2();
<<<<<<< HEAD
	void destroy(int& power, Pop*& popx, Vector<Sprite*>& _breakableBlockVector, MapLayer*& map, Vector<Sprite*>&_pop);
=======
	void destroy(int& power, Pop*& popx, Vector<Sprite*>& _breakableBlockVector, MapLayer*& map,Vector<Sprite*>&_pop);
>>>>>>> origin/master
	bool collide(Vec2 position, int type, Vector<Sprite*>& _breakableBlockVector, MapLayer*& map);
	Vec2* setBubblePosition(int& power, Vec2 popPosition);
};

#endif 