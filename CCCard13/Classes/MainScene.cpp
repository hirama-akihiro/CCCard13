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
    auto card = CardSprite::create();
    card->setCard(getCard());
    card->setPosIndex(posIndex);
    addChild(card, ZORDER_SHOW_CARD);
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

bool CardSprite::init()
{
    if(!Sprite::init()){ return false;}
    return true;
}

void CardSprite::onEnter()
{
    Sprite::onEnter();
    
    // 画像の表示
    setTexture(getFileName(m_card.m_type));
    
    //　マークと数字の表示
    showNumber();
    
    // カードの位置とタグを指定
    float posX = CARD_1_POSX + CARD_DISTANCEX * m_posIndex.m_x;
    float posY = CARD_1_POSY + CARD_DISTANCEY * m_posIndex.m_y;
    setPosition(posX, posY);
    setTag(m_posIndex.m_x + m_posIndex.m_y * 5 + 1);
}

std::string CardSprite::getFileName(CardType cardType)
{
    // ファイル名の取得
    std::string filename;
    switch (cardType) {
        case Clubs:
            filename = "card_clubs.png";
            break;
        case Diamonds:
            filename = "card_diamonds.png";
            break;
        case Hearts:
            filename = "card_hearts.png";
            break;
        default:
            filename = "card_spades.png";
            break;
    }
    return filename;
}

void CardSprite::showNumber()
{
    // 表示する数字の取得
    std::string numberString;
    switch (m_card.m_number) {
        case 1:
            numberString = "A";
            break;
        case 11:
            numberString = "J";
            break;
        case 12:
            numberString = "Q";
            break;
        case 13:
            numberString = "K";
            break;
        default:
            numberString = StringUtils::format("%d",m_card.m_number);
            break;
    }
    
    // 表示する文字色の取得
    Color4B textColor;
    switch (m_card.m_type) {
        case Clubs:
        case Spades:
            textColor = Color4B::BLACK;
            break;
        default:
            textColor = Color4B::RED;
            break;
    }
    
    // ラベルの生成
    auto number = Label::createWithSystemFont(numberString, "Arial", 96);
    number->setPosition(Point(getContentSize() / 2));
    number->setTextColor(textColor);
    addChild(number);
}