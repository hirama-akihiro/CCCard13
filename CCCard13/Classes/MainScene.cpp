#include "MainScene.h"

#define CARD_NUM 13 // 1種類あたりのカード枚数
#define CARD_TYPE_NUM 4 // カードの種類

#define CARD_1_POSX 200 // 1番のカード位置(x)
#define CARD_1_POSY 320 // 1番のカード位置(y)
#define CARD_DISTANCEX 140 // カード間の距離(x方向)
#define CARD_DISTANCEY 200 // カード間の距離(y方向)

#define ZORDER_SHOW_CARD 1 // 表示しているカードのZオーダー

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
    
    // ゲームを初期化する
    initGame();
    
    return true;
}

void MainScene::initCards()
{
    // ゲームカードのクリア
    m_cards.clear();
    
    // 4種類 x 13枚分のループ
    for(int type = 0; type < CARD_TYPE_NUM;++type){
        for(int number = 1;number <= CARD_NUM;++number){
            // カード情報の作成
            Card card;
            card.m_number = number;
            card.m_type = (CardType)type;
            
            // カードを追加する
            m_cards.push_back(card);
        }
    }
}

Card MainScene::getCard()
{
    std::random_device rd;
    std::mt19937 rand = std::mt19937(rd());
    
    // インデックスをランダムに取得する
    int index = std::uniform_int_distribution<int>(0, (int) m_cards.size() - 1)(rand);
    
    // カードの情報を取得する
    Card card = m_cards.at(index);
    
    // 一時カードから削除する
    m_cards.erase(m_cards.begin() + index);
    
    return card;
}

void MainScene::createCard(PosIndex posIndex)
{
    // 新しいカードを作成する
    Sprite* card = Sprite::create("card_spades.png");
    card->setPosition(CARD_1_POSX + CARD_DISTANCEX * posIndex.m_x,
                      CARD_1_POSY + CARD_DISTANCEY * posIndex.m_y);
    addChild(card,ZORDER_SHOW_CARD);
}

void MainScene::showInitCards()
{
    for(int tag = 1; tag <= 10; ++tag){
        auto card = getChildByTag(tag);
        if(card){
            // カードが残っている場合は削除する
            card->removeFromParent();
        }
    }
    
    // 5列 x 2行分のループ
    for(int x = 0; x < 5; ++x){
        for(int y = 0; y < 2; ++y){
            PosIndex posIndex;
            posIndex.m_x = x;
            posIndex.m_y = y;
            
            // カードの生成
            createCard(posIndex);
        }
    }
}

void MainScene::initGame()
{
    // カードを初期化する
    initCards();
    
    // カードを表示する
    showInitCards();
}