/**************************************************************
 *
 * ___GameOverScene.cpp___
 * ___Cocos2dSimpleGame___
 *
 * Create by ___DuongHuuHieu___
 *
 * Copyright Vinova Pte. Ltd. All right reserved.
 *************************************************************
 */

#include "GameOverScene.h"
#include "HelloWorldScene.h"

using namespace cocos2d;

CCScene* GameOverLayer::scene()
{
  // 'scene' is an autorelease object
  CCScene *scene = CCScene::create();
  
  // 'layer' is an autorelease object
  GameOverLayer *layer = GameOverLayer::create();
  
  // add layer as a child to scene
  scene->addChild(layer);
  
  // return the scene
  return scene;
}

CCScene* GameOverScene::scene()
{
  // 'scene' is an autorelease object
  CCScene *scene = CCScene::create();
  
  // 'layer' is an autorelease object
  GameOverScene *layer = GameOverScene::create();
  
  // add layer as a child to scene
  scene->addChild(layer);
  
  // return the scene
  return scene;
}

bool GameOverScene::init()
{
  if (CCScene::init())
  {
    this->_layer = GameOverLayer::create();
    this->_layer->retain();
    this->addChild(_layer);
    
    return true;
  }
  else
  {
    return false;
  }
}

GameOverScene::~GameOverScene()
{
  if (_layer) {
    _layer->release();
    _layer = NULL;
  }
}

bool GameOverLayer::init()
{
  if (CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
  {
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    this->_label = CCLabelTTF::create("", "Artial",  32);
    _label->retain();
    _label->setColor(ccc3(0, 0, 0));
    _label->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(_label);
    this->runAction( CCSequence::create(
                                        CCDelayTime::create(3),
                                        CCCallFunc::create(this,
                                                           callfunc_selector(GameOverLayer::gameOverDone)),
                                        NULL));
    return true;
  }
  else
  {
    return false;
  }
}

void GameOverLayer::gameOverDone()
{
  CCDirector::sharedDirector()->replaceScene(HelloWorld::scene());
}

GameOverLayer::~GameOverLayer()
{
  if (_label)
  {
    _label->release();
    _label = NULL;
  }
}
