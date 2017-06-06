#include"KeyBoard.h"

bool KeyBoard::init()
{
	keyBoardDirection = -1;
	mulPressed = 0;
	return true;
}

void KeyBoard::startKB()
{
	auto keyBoardListener = EventListenerKeyboard::create();
	keyBoardListener->onKeyPressed = CC_CALLBACK_2(KeyBoard::onKeyPressed, this);
	keyBoardListener->onKeyReleased = CC_CALLBACK_2(KeyBoard::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyBoardListener, this);
}

void KeyBoard::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	++mulPressed;
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_UP_ARROW:
		keyBoardDirection = keyBoard_up;
		break;
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		keyBoardDirection = keyBoard_left;
		break;
	case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		keyBoardDirection = keyBoard_down;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		keyBoardDirection = keyBoard_right;
		break;
	case EventKeyboard::KeyCode::KEY_SPACE:
		keyBoardDirection = keyBoard_space;
		break;
	default:
		keyBoardDirection = keyBoard_stand;
		break;
	}
}

void KeyBoard::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	--mulPressed;
	if (!mulPressed)
	{
		keyBoardDirection = keyBoard_stand;
	}
}

void KeyBoard::stopKB()
{
	_eventDispatcher->removeEventListenersForTarget(this);
}