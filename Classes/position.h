#ifndef __POSITION_H__
#define __POSITION_H__

#include "cocos2d.h"
USING_NS_CC;

Vec2 centerPositionForTileCoord(const cocos2d::Vec2 &TileCoord);
Vec2 tileCoordForPosition(Vec2 position);
#endif
