#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "GameOverScene.h"   

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
  CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic(
                                                                        "background-music-aac.wav", true);
  //////////////////////////////
  // 1. super init first
  if ( !CCLayerColor::initWithColor(ccc4(255, 255, 255, 255)))
  {
      return false;
  }

  //Initialize arrays
  mTargets = new CCArray;
  mProjectiles = new CCArray;
  
  /////////////////////////////
  // 2. add a menu item with "X" image, which is clicked to quit the program
  //    you may modify it.

  // add a "close" icon to exit the progress. it's an autorelease object
  CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback) );
  pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

  // create menu, it's an autorelease object
  CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
  pMenu->setPosition( CCPointZero );
  this->addChild(pMenu, 1);
  
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  CCSprite* player = CCSprite::create("Player.png",
                                      CCRectMake(0, 0, 27, 40));
  player->setPosition( ccp(player->getContentSize().width/2,
                           winSize.height/2));
  this->addChild(player);
  this->schedule(schedule_selector(HelloWorld::gameLogic), 1.0);
  this->schedule(schedule_selector(HelloWorld::update));
  this->setTouchEnabled(true);
  return true;
}

void HelloWorld::addTarget()
{
  CCSprite* target = CCSprite::create("Target.png",
                                      CCRectMake(0, 0, 27, 40));
  //Add target to array and set tag to 1
  target->setTag(1);
  mTargets->addObject(target);
  
  //Determine where to spawn the target along the Y axis
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  int minY = target->getContentSize().height/2;
  int maxY = winSize.height - target->getContentSize().height/2;
  int rangeY = maxY - minY;
  //srand( TimGetTicks() );
  int actualY = ( rand() % rangeY ) + minY;
  
  // Create the target slightly off-screen along the right edge,
  // and along a random position along the Y axis as calculated
  target->setPosition(
                      ccp(winSize.width + (target->getContentSize().width/2),
                          actualY) );
  this->addChild(target);
  
  // Determine speed of the target
  int minDuration = (int)2.0;
  int maxDuration = (int)4.0;
  int rangeDuration = maxDuration - minDuration;
  
  // srand( TimGetTicks() );
  int actualDuration = ( rand() % rangeDuration )
  + minDuration;
  
  // Create the actions
  CCFiniteTimeAction* actionMove =
  CCMoveTo::create( (float)actualDuration,
                   ccp(0 - target->getContentSize().width/2,
                       actualY) );
  CCFiniteTimeAction* actionMoveDone =
  CCCallFuncN::create( this,
                      callfuncN_selector(HelloWorld::spriteMoveFinished));
  target->runAction(CCSequence::create(actionMove,
                                       actionMoveDone, NULL) );
}

void HelloWorld::spriteMoveFinished(CCNode* psender)
{
  CCSprite* sprite = (CCSprite*)psender;
  this->removeChild(sprite, true);
  
  if (sprite->getTag() == 1)
  {
    mTargets->removeObject(sprite);
    GameOverScene* gameOverScene = GameOverScene::create();
    gameOverScene->getLayer()->getLabel()->setString("You Lose Like A Bitch");
    CCDirector::sharedDirector()->replaceScene(gameOverScene);
  }
  else if (sprite->getTag() == 2)
  {
    mProjectiles->removeObject(sprite);
  }
}

void HelloWorld::gameLogic(float dt)
{
  this->addTarget();
}

void HelloWorld::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
  //music when fire
  CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.wav", true);
  // Choose one of the touches to work with
  CCTouch* touch = (CCTouch*)( pTouches->anyObject() );
  CCPoint location = touch->getLocationInView();
  location = CCDirector::sharedDirector()->convertToGL(location);
  
  // Set up initial location of projectile
  CCSize winSize = CCDirector::sharedDirector()->getWinSize();
  CCSprite *projectile = CCSprite::create("Projectile.png",
                                          CCRectMake(0, 0, 20, 20));
  //Add bullet to array and set tag to 2
  projectile->setTag(2);
  mProjectiles->addObject(projectile);
  projectile->setPosition( ccp(20, winSize.height/2) );
  
  // Determinie offset of location to projectile
  int offX = location.x - projectile->getPosition().x;
  int offY = location.y - projectile->getPosition().y;
  
  // Bail out if we are shooting down or backwards
  if (offX <= 0) return;
  
  // Ok to add now - we've double checked position
  this->addChild(projectile);
  
  // Determine where we wish to shoot the projectile to
  int realX = winSize.width
  + (projectile->getContentSize().width/2);
  float ratio = (float)offY / (float)offX;
  int realY = (realX * ratio) + projectile->getPosition().y;
  CCPoint realDest = ccp(realX, realY);
  
  // Determine the length of how far we're shooting
  int offRealX = realX - projectile->getPosition().x;
  int offRealY = realY - projectile->getPosition().y;
  float length = sqrtf((offRealX * offRealX)
                       + (offRealY*offRealY));
  float velocity = 480/1; // 480pixels/1sec
  float realMoveDuration = length/velocity;
  // Move projectile to actual endpoint
  projectile->runAction(CCSequence::create(CCMoveTo::create(realMoveDuration,
                                                            realDest),
                                           CCCallFuncN::create(this,
                                                               callfuncN_selector(HelloWorld::spriteMoveFinished)),
                                           NULL));
                                                            
}

//Destroy func
HelloWorld::~HelloWorld()
{
  if(mTargets)
  {
    mTargets->release();
    mTargets = NULL;
  }
  
  if (mProjectiles)
  {
    mProjectiles->release();
    mProjectiles = NULL;
  }
}

//Create func
HelloWorld::HelloWorld()
:mTargets(NULL)
,mProjectiles(NULL)
{
  nProjectilesDestroyed = 0;
}

void HelloWorld::update(float dt)
{
  CCArray* projectilesToDelete = new CCArray;
  CCArray* targetsToDelete = new CCArray;
  CCObject* it = NULL;
  CCObject* jt = NULL;
  
  CCARRAY_FOREACH(mProjectiles, it)
  {
    CCSprite* projectile = dynamic_cast<CCSprite*>(it);
    CCRect projectileRect = CCRectMake(
                                       projectile->getPosition().x -
                                       (projectile->getContentSize().width/2),
                                       projectile->getPosition().y -
                                       (projectile->getContentSize().height/2),
                                       projectile->getPosition().y -
                                       (projectile->getContentSize().height/2),
                                       projectile->getContentSize().height);
    CCARRAY_FOREACH(mTargets, jt)
    {
      CCSprite *target = dynamic_cast<CCSprite*>(jt);
      CCRect targetRect = CCRectMake(
                                     target->getPosition().x -
                                     (target->getContentSize().width/2),
                                     target->getPosition().y -
                                     (target->getContentSize().height/2),
                                     target->getContentSize().width,
                                     target->getContentSize().height);
      if (projectileRect.intersectsRect(targetRect))
      {
        targetsToDelete->addObject(target);
        projectilesToDelete->addObject(projectile);
      }
    }
  }
  
  CCARRAY_FOREACH(targetsToDelete, jt)
  {
    CCSprite* target = dynamic_cast<CCSprite*>(jt);
    mTargets->removeObject(target);
    this->removeChild(target, true);
    nProjectilesDestroyed++;
    
    if (nProjectilesDestroyed >= 5)
    {
      GameOverScene* gameOverScene = GameOverScene::create();
      gameOverScene->getLayer()->getLabel()->setString("You Win Like A Boss!");
      CCDirector::sharedDirector()->replaceScene(gameOverScene);
    }
  }
  
  CCARRAY_FOREACH(projectilesToDelete, it)
  {
    CCSprite* projectile = dynamic_cast<CCSprite*>(it);
    mProjectiles->removeObject(projectile);
    this->removeChild(projectile, true);
  }
  
  projectilesToDelete->release();
  targetsToDelete->release();
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
