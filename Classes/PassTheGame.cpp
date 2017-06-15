#include "PassTheGame.h"
#include "MenuSwitch.h"
USING_NS_CC;
Scene* PassTheGame::createScene()
{
	// 创建一个场景对象
	auto scene = Scene::create();
	// 创建层对象
	auto layer = PassTheGame::create();
	// 将BeginScene层作为子节点添加到场景
	scene->addChild(layer);
	// 返回场景对象
	return scene;
}

bool PassTheGame::init()
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

							 //返回主菜单按钮
	auto* pBack = MenuItemImage::create("backtomenu1.png",
		"backtomenu2.png",
		this,
		menu_selector(MenuSwitch::BackToMenu));
	auto* Back = Menu::create(pBack, NULL);
	Back->setAnchorPoint(Vec2(0, 0));
	Back->setPosition(850, size.height / 2 - 70);
	Back->setScale(0.9f);
	addChild(Back);

	SimpleAudioEngine::getInstance()->playBackgroundMusic("Village.mp3", true);
	return true;
}