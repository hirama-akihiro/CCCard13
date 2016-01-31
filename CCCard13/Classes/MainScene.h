#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

class MainScene : public cocos2d::Layer
{
public:
    // Mainシーンを作成する
    static cocos2d::Scene* createScene();

    // 初期化処理を行う
    virtual bool init();
    
    // create関数作成マクロ
    CREATE_FUNC(MainScene);
};

#endif // __MAIN_SCENE_H__
