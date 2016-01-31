#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include "cocos2d.h"

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

class MainScene : public cocos2d::Layer
{
protected:
    std::vector<Card> m_cards;  // カード情報
    
    void initCards();   // カードを初期化する
    Card getCard(); // カードを取得する
    void createCard(PosIndex posIndex); //　カードを作成する
    void showInitCards();   // ゲーム開始時にカードを10枚表示する
    void initGame();   // ゲームを初期化する
    
public:
    // Mainシーンを作成する
    static cocos2d::Scene* createScene();

    // 初期化処理を行う
    virtual bool init();
    
    // create関数作成マクロ
    CREATE_FUNC(MainScene);
};

#endif // __MAIN_SCENE_H__
