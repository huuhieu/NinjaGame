#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;

class HelloWorld : public cocos2d::CCLayerColor
{
public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    HelloWorld();
    ~HelloWorld();
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(HelloWorld);
  
    void addTarget();
    
    void spriteMoveFinished(CCNode* psender);
    void gameLogic(float dt);
    void update(float dt);
  void ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
  
protected:
    cocos2d::CCArray* mTargets;
    cocos2d::CCArray* mProjectiles;
    int nProjectilesDestroyed;
};

#endif // __HELLOWORLD_SCENE_H__
