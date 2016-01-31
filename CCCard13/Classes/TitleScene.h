#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "cocos2d.h"

class TitleScene : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);
    
    CREATE_FUNC(TitleScene);
};

#endif // __TITLE_SCENE_H__
