#include "Select.h"
#include "MenuSwitch.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
Scene* Select::createScene()
{
	// 创建一个场景对象，该对象将会由自动释放池管理内存的释放
	auto scene = Scene::create();

	// 创建层对象，该对象将会由自动释放池管理内存的释放
	auto layer = Select::create();

	scene->addChild(layer);

	// 返回场景对象
	return scene;
}

//  "init" 函数中初始化实例
bool Select::init()
{
	// 调用父类的init函数
	if (!Layer::init())
	{
		return false;
	}

	// 获得设备支持的可见OpenGL视图大小（即窗口大小）。
	Size visibleSize = Director::getInstance()->getVisibleSize();
	// 获得可见OpenGL视图的起源点（默认x=0，y=0）。
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto* background = Sprite::create("background3.png");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);  //设置位置
	background->setScale(1.5f);//大小
	addChild(background, 0); //添加到场景
	Sprite* tag = Sprite::create("Map1.png");
	tag->setAnchorPoint(Vec2(0.5, 0.5));
	tag->setPosition(600, 450);
	tag->setScale(0.9f);
	_mapVector.pushBack(tag);
	addChild(tag, 2);
	Sprite* tag2 = Sprite::create("123.png");
	tag2->setAnchorPoint(Vec2(0.5, 0.5));
	tag2->setPosition(325, 230);
	tag2->setScale(0.9f);
	_tagVector.pushBack(tag2);
	addChild(tag2, 2);
	auto map1_button = Button::create("xiaoqu1.png");
	map1_button->setPosition(Vec2(visibleSize.width / 4.5, visibleSize.height*0.7));
	map1_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			while (_mapVector.size()>0)
			{
				auto sprite = _mapVector.at(0);
				removeChild(sprite);
				_mapVector.eraseObject(sprite);
			}
			Sprite* tag = Sprite::create("Map1.png");
			tag->setAnchorPoint(Vec2(0.5, 0.5));
			tag->setPosition(600, 450);
			tag->setScale(0.9f);
			_mapVector.pushBack(tag);
			addChild(tag, 2);
			_selectMap = 1;
		}
	});
	this->addChild(map1_button, 1);

	auto map2_button = Button::create("xiaoqu2.png");
	map2_button->setPosition(Vec2(visibleSize.width / 4.5, visibleSize.height*0.6));
	map2_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			while (_mapVector.size()>0)
			{
				auto sprite = _mapVector.at(0);
				removeChild(sprite);
				_mapVector.eraseObject(sprite);
			}
			Sprite* tag = Sprite::create("Map2.png");
			tag->setAnchorPoint(Vec2(0.5, 0.5));
			tag->setPosition(600, 450);
			tag->setScale(0.9f);
			_mapVector.pushBack(tag);
			addChild(tag, 2);
			_selectMap = 2;
		}
	});
	this->addChild(map2_button, 1);

	auto player1_button = Button::create("player_1.png");
	player1_button->setPosition(Vec2(visibleSize.width / 3, visibleSize.height*0.2));
	player1_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			while (_tagVector.size()>0)
			{
				auto sprite = _tagVector.at(0);
				removeChild(sprite);
				_tagVector.eraseObject(sprite);
			}
			Sprite* tag = Sprite::create("123.png");
			tag->setAnchorPoint(Vec2(0.5, 0.5));
			tag->setPosition(325,230);
			tag->setScale(0.9f);
			_tagVector.pushBack(tag);
			addChild(tag, 2);
			_selectPlayer = 1;
		}
	});
	this->addChild(player1_button, 1);

	auto player2_button = Button::create("player_2.png");
	player2_button->setPosition(Vec2(visibleSize.width / 1.55, visibleSize.height*0.2));
	player2_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			while (_tagVector.size()>0)
			{
				auto sprite = _tagVector.at(0);
				removeChild(sprite);
				_tagVector.eraseObject(sprite);
			}
			Sprite* tag = Sprite::create("123.png");
			tag->setAnchorPoint(Vec2(0.5, 0.5));
			tag->setPosition(620, 230);
			tag->setScale(0.9f);
			_tagVector.pushBack(tag);
			addChild(tag, 2);
			_selectPlayer = 2;
		}
	});
	this->addChild(player2_button, 1);

	auto start_button = Button::create("start.png");
	start_button->setPosition(Vec2(visibleSize.width / 2, visibleSize.height*0.2));
	start_button->addTouchEventListener([=](Ref* pSender, Widget::TouchEventType type) {
		if (type == Widget::TouchEventType::ENDED) {
			auto transition = TransitionSlideInR::create(1.0, GameScene::createScene(_selectPlayer, _selectMap));
			Director::getInstance()->pushScene(transition);
		}
	});
	this->addChild(start_button, 1);
	return true;
}
