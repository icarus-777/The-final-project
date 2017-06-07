//
//  PreLoadScene.h
//  AirfightGame
//
//  Created by Jason on 14-5-8.
//
//

#ifndef __AirfightGame__PreLoadScene__
#define __AirfightGame__PreLoadScene__

#include <iostream>
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;
using namespace CocosDenshion;

// 3. 预加载类：完成预加载背景音乐、音效、精灵表单等准备工作
class PreLoad : public cocos2d::Layer
{
protected:
    // 用来保存游戏需要加载的资源总数
    int _sourceCount;
    // 进度条
    ProgressTimer* _progress;
    // 进度条更新的次数
    float _progressInterval;
public:
    /**
     * 静态函数，创建Scene场景
     * 注意：在Cocos2d-x中没有Cocos2d-iPhone中的“id”,建议返回类实例的指针
     */
    static cocos2d::Scene* createScene();
    /**
     * init函数，完成初始化操作。
     * 注意：init函数在Cocos2d-x中返回bool值，而不是返回Cocos2d-iPhone中的“id”
     */
    virtual bool init();
    
    virtual void onEnterTransitionDidFinish();
    
    // 加载背景音乐
    void loadMusic(ValueVector musicFiles);
    // 加载游戏音效
    void loadEffect(ValueVector effectFiles);
    // 加载精灵表单
    void loadSpriteSheets(ValueVector spriteSheets);
    // 更新游戏进度条
    void progressUpdate();
    
    /**
     * 使用CREATE_FUNC宏创建当前类的对象，返回的对象将会由自动释放池管理内存的释放
     */
    CREATE_FUNC(PreLoad);
};

#endif /* defined(__AirfightGame__PreLoadScene__) */
