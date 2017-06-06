#include "MenuSwitch.h"
#include "GameScene.h"
#include "BeginScene.h"
#include "AudioControlScene.h"
USING_NS_CC;
void MenuSwitch::StartGame(cocos2d::Object* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("lay.wav");
	auto transition = TransitionSlideInL::create(0.5, GameScene::createScene());///创建场景转换
	Director::getInstance()->replaceScene(transition);//转到游戏界面
}

void MenuSwitch::ExitGame(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("lay.wav");
	Director::getInstance()->end();//结束游戏
}
void MenuSwitch::BackToMenu(cocos2d::Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("lay.wav");
	auto transition = TransitionSlideInL::create(0.5, BeginScene::createScene());///创建场景转换
	Director::getInstance()->replaceScene(transition);
}
void MenuSwitch::GameSet(cocos2d::Ref* pSender)
{
	SimpleAudioEngine::getInstance()->playEffect("lay.wav");
	auto transition = TransitionSlideInL::create(0.5, AudioControl::createScene());///创建场景转换
	Director::getInstance()->replaceScene(transition);
}