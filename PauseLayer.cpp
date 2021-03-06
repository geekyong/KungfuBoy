﻿/*!
* \file PauseLayer.cpp
* \date 2015/04/08 15:30
*
* \author SuooL
* Contact: 1020935219@qq.com
*
* \brief 暂停界面
*
* \version 1.0
*
* TODO: long description
*
* \note
*/
#include "PauseLayer.h"
#include "GlobalDefine.h"
#include "GlobalData.h"
#include "GameLayer.h"
#include "StartLayer.h"
#include "cocos2d.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;
using namespace cocos2d::extension;

Scene* PauseLayer::createScene(RenderTexture* rt)
{
	Scene* scene = Scene::create();
	PauseLayer* layer = PauseLayer::create();

	auto sprite = Sprite::createWithTexture(rt->getSprite()->getTexture());
	sprite->setPosition(WINSIZE.width / 2, WINSIZE.height / 2);
	sprite->setFlippedY(true);
//	sprite->setColor(cocos2d::ccGRAY);
	scene->addChild(sprite, 0);

	scene->addChild(layer);

	return scene;
}

bool PauseLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto size = Director::getInstance()->getWinSize();

	// 背景黑色的图片
	spriteOn = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pauseBG1.png"));
	spriteDown = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("pauseBG2.png"));
	spriteOn->setPosition(size.width / 2, size.height + spriteOn->getContentSize().height / 2);
	spriteDown->setPosition(size.width / 2, - (spriteDown->getContentSize().height / 2));
	this->addChild(spriteOn); 
	this->addChild(spriteDown);

	/*
	继续游戏按钮：  X：630.5 Y349.0
	声音按键：X：190.0  Y294.0
	重置键按钮：X346.0 Y：294.0
	返回关卡界面键按钮：X890.0 Y：294.0
	下一关键按钮：X1053.0 Y：294.0
	*/
	// 继续游戏按钮
	auto resumeItem = MenuItemSprite::create(
		Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("playNormal.png")),
		Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("playSelected.png")),
		[&](Ref * ref){
		PLAYEFFECT;
		//弹出保存的场景
		auto moveOn = MoveBy::create(0.5f, Point(0, (spriteOn->getContentSize().height)));
		auto moveDown = MoveBy::create(0.5f, Point(0, -spriteDown->getContentSize().height));
		CallFunc* replace = CallFunc::create([](){Director::getInstance()->popScene();});
		Action* resume = Sequence::create(moveOn, replace, NULL);
		spriteDown->runAction(moveDown);
		spriteOn->runAction(resume);
		});
	resumeItem->setPosition(spriteOn->getContentSize().width / 2-5, 110.0f);

	// 返回游戏主界面
	auto startItem = MenuItemSprite::create(
		Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("backNormal.png")),
		Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("backSelected.png")),
		[](Ref * ref){
		PLAYEFFECT;
		//弹出保存的场景
		Director::getInstance()->replaceScene(StartLayer::createScene());});
	startItem->setPosition(890.0f, 180.0f);

	// 重新开始游戏
	auto againItem = MenuItemSprite::create(
		Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("againNormal.png")),
		Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("againSelected.png")),
		[](Ref * ref){
		PLAYEFFECT;
		//弹出保存的场景
		Director::getInstance()->replaceScene(GameLayer::createScene());});
	againItem->setPosition(346.0f, 180.0f);
	// 下一关
	auto nextItem = MenuItemSprite::create(
		Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("nextNormal.png")),
		Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("nextSelected.png")),
		[](Ref * ref){
		PLAYEFFECT;
		//弹出保存的场景
		if (m_iSelectGate++ < 3)
		{
			Director::getInstance()->replaceScene(GameLayer::createScene());
		}
		});
	nextItem->setPosition(1053.0f, 180.0f);

	// 声音设置按钮
	auto musicOn = MenuItemSprite::create(Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("musicOn.png")),
		Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("musicOff.png")));
	auto musicOff = MenuItemSprite::create(Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("musicOff.png")),
		Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("musicSelected.png")));
	auto musciToggle = MenuItemToggle::createWithTarget(this, menu_selector(PauseLayer::musicSet), musicOn, musicOff, NULL);
	musciToggle->setPosition(190.0f, 180.0f);

	auto menu = Menu::create(resumeItem, startItem, againItem, nextItem,musciToggle, NULL);
	menu->setPosition(Point::ZERO);
	spriteOn->addChild(menu);

	auto moveDown = MoveBy::create(0.6f, Point(0, - (spriteOn->getContentSize().height)));
	auto moveUp = MoveBy::create(0.6f, Point(0, spriteDown->getContentSize().height));
	spriteOn->runAction(moveDown);
	spriteDown->runAction(moveUp);

}

void PauseLayer::musicSet(Ref* pSender)
{
	PLAYEFFECT;
	auto musicTemp = (MenuItemToggle*)pSender;
	if (getBoolFromXML(SOUND_KEY))
	{
		aduioEngine->setEffectsVolume(getFloatFromXML(SOUNDVOL));
		aduioEngine->playEffect("");
	}
	if (musicTemp->getSelectedIndex() == 1)
	{
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	}
	else
	{
		aduioEngine->setBackgroundMusicVolume(getFloatFromXML(MUSICVOL));
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
} 