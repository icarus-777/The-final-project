#include "PauseScene.h"
#include "GameScene.h"
#include "MenuSwitch.h"
USING_NS_CC;
Scene* PauseScene::createScene()
{
	// 创建一个场景对象
	auto scene = Scene::create();
	// 创建层对象
	auto layer = PauseScene::create();
	// 将BeginScene层作为子节点添加到场景
	scene->addChild(layer);
	// 返回场景对象
	return scene;
}

bool PauseScene::init()
{
	// 调用父类的init方法
	if (!Layer::init())
	{
		return false;
	}
	// 获得设备可见视图大小
	Size size = Director::getInstance()->getVisibleSize();
	//获得坐标原点
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//游戏初始界面
	auto* background = Sprite::create("BeginScene.png");
	background->setPosition(size.width / 2, size.height / 2);  //设置位置
	background->setScale(1.5f);//大小
	addChild(background, 0); //添加到场景

							 //重新开始游戏按钮
	auto* pStart = MenuItemImage::create("button1.1.png",
		"button2.2.png",
		this,
		menu_selector(MenuSwitch::StartGame));
	auto* Start = Menu::create(pStart, NULL);
	Start->setAnchorPoint(Vec2(0, 0));
	Start->setPosition(850, size.height / 5);
	Start->setScale(0.9f);
	addChild(Start);
	//结束游戏按钮
	auto* pExit = MenuItemImage::create("button1.2.png",
		"button2.3.png",
		this,
		menu_selector(MenuSwitch::ExitGame));
	auto* Exit = Menu::create(pExit, NULL);
	Exit->setAnchorPoint(Vec2(0, 0));
	Exit->setPosition(850, size.height * 2 / 5);
	Exit->setScale(0.9f);
	addChild(Exit);

	//游戏设置按钮
	auto* pSet = MenuItemImage::create("button3.1.png",
		"button3.2.png",
		this,
		menu_selector(MenuSwitch::GameSet));
	auto* Set = Menu::create(pSet, NULL);
	Set->setAnchorPoint(Vec2(0, 0));
	Set->setPosition(850, size.height * 3 / 5);
	Set->setScale(0.9f);
	addChild(Set);

	//继续游戏
	auto* pGoOn = MenuItemImage::create("继续.png",
		"继续.png",
		this,
		menu_selector(MenuSwitch::Continue));
	auto* GoOn = Menu::create(pGoOn, NULL);
	GoOn->setAnchorPoint(Vec2(0, 0));
	GoOn->setPosition(850, size.height * 4 / 5);
	GoOn->setScale(0.9f);
	addChild(GoOn);
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Village.mp3", true);
	return true;
}