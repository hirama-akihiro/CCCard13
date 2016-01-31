#include "TitleScene.h"
#include "MainScene.h"

USING_NS_CC;

Scene* TitleScene::createScene()
{
    // シーンはautoreleaseオブジェクト
    auto scene = Scene::create();
    
    // レイヤーはautoreleaseオブジェクト
    auto layer = TitleScene::create();
    
    // レイヤーをシーンに追加
    scene->addChild(layer);
    
    // シーンを返す
    return scene;
}

// インスタンスの初期化はinitメソッドで行う
bool TitleScene::init()
{
    // 基底クラスの初期化
    if ( !Layer::init() ) { return false; }
    
    Sprite* spriteBackGround = Sprite::create("background.png");
    Size winSize = Director::getInstance()->getWinSize();
    spriteBackGround->setPosition(winSize.width * 0.5, winSize.height * 0.5);
    spriteBackGround->setScale(2.0f);
    addChild(spriteBackGround);
    
    // タイトル文字のラベルを生成
    Label* labelTitle = Label::create();
    // タイトルラベルのフォントを設定(引数:フォント名)
    labelTitle->setSystemFontName("fonts/Marker Felt.ttf");
    // タイトルラベルのフォントサイズを設定
    labelTitle->setSystemFontSize(120);
    // タイトルラベルで表示する文字を設定
    labelTitle->setString("CCCard13");
    // タイトルラベルの文字色を設定(デフォルト値:COLOR3B(255, 255, 255))
    labelTitle->setColor(Color3B::BLACK);
    // タイトルラベルの表示位置を設定
    Vec2 titlePos = Vec2(winSize.width * 0.5, winSize.height * 0.6);
    labelTitle->setPosition(titlePos);
    // タイトルラベルをレイヤーに追加
    addChild(labelTitle);
    
    // メッセージ文字のラベルを生成
    Label* labelMessage = Label::createWithSystemFont("Touch Screen", "fonts/Marker Felt.ttf", 60);
    // メッセージラベルの文字色を設定(デフォルト値:COLOR3B(255, 255, 255))
    labelMessage->setColor(Color3B::BLACK);
    // メッセージラベルの表示位置を設定
    Vec2 messagePos = Vec2(winSize.width * 0.5, winSize.height * 0.3);
    labelMessage->setPosition(messagePos);
    // メッセージラベルをレイヤーに追加
    addChild(labelMessage);
    
    // タッチイベントリスナーを生成
    auto listener = EventListenerTouchOneByOne::create();
    // イベントリスナーにタッチイベントを設定
    listener->onTouchBegan = CC_CALLBACK_2(TitleScene::onTouchBegan, this);
    // イベントリスナーをディスパッチャーに設定
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    return true;
}

// タッチ開始時に一度だけ呼ばれるメソッド
bool TitleScene::onTouchBegan(Touch *touch, Event *unused_event)
{
    // 遷移先のシーンを生成
    auto nextScene = MainScene::createScene();
    // シーンの遷移を行う
    Director::getInstance()->replaceScene(TransitionPageTurn::create(2.0f, nextScene, false));
    
    return true;
}

