#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"
#include "extensions/cocos-ext.h"

// カードの種類
enum CardType{
    Clubs = 0,
    Diamonds = 1,
    Hearts = 2,
    Spades = 3
};

// カード
struct Card{
    int m_number;
    CardType m_type;
};

// カードを表示する位置
struct PosIndex{
    int m_x;
    int m_y;
};

class CardSprite : public cocos2d::Sprite
{
protected:
    std::string getFileName(CardType cardType); // 表示画像ファイル名取得
    void showNumber(); // カードのマークと数字を表示
public:
    virtual bool init(); // 初期化処理
    void onEnter() override; // 表示前処理
    CREATE_FUNC(CardSprite); // create関数作成マクロ
    CC_SYNTHESIZE(Card, m_card, Card); // カード情報
    CC_SYNTHESIZE(PosIndex, m_posIndex, PosIndex); // 表示位置
    
    void moveBackToInitPos(); // 元の位置に移動する
    void moveToTrash(); // カードを捨てる
    void moveToInitPos(); // 最初の位置に移動する
};

class MainScene : public cocos2d::Layer
{
protected:
    std::vector<Card> m_cards;  // カード情報
    CardSprite* m_firstCard;    // 最初にタップされたカード
    float m_timer; // 経過時間
    
    void initCards();   // カードを初期化する
    Card getCard(); // カードを取得する
    void createCard(PosIndex posIndex); //　カードを作成する
    void showInitCards();   // ゲーム開始時にカードを10枚表示する
    void initGame();   // ゲームを初期化する
    CardSprite* getTouchCard(cocos2d::Touch* touch); // タップされたカードを取得
    void showButton(); // ボタンを表示する
    void initTrash(); // ゴミカードを初期化する
    void onTapButton(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent); // ボタンがタップされた時に呼ばれる
    void showBackCards(); // カードの山を表示する
    void showTimerLabel(); // 経過時間を表示
public:
    // Mainシーンを作成する
    static cocos2d::Scene* createScene();

    // 初期化処理を行う
    virtual bool init();
    // 毎フレーム呼ばれる関数
    virtual void update(float dt) override;
    
    // create関数作成マクロ
    CREATE_FUNC(MainScene);
    
    // タップイベント
    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* unused_event);
    virtual void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* unused_event);
};

#endif // __MAIN_SCENE_H__
