#include "MainScene.h"

USING_NS_CC;

Scene* MainScene::createScene()
{
    // シーンを生成する
    auto scene = Scene::create();
    
    // MainSceneクラスのレイヤーを生成する
    auto layer = MainScene::create();

    // シーンに対してレイヤーを追加する
    scene->addChild(layer);

    // シーンを返す
    return scene;
}

bool MainScene::init()
{
    // レイヤーの初期化
    if(!Layer::init()) { return false; }
    
    return true;
}