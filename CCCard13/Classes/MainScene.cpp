#include "MainScene.h"

#define CARD_NUM 13 // 1種類あたりのカード枚数
#define CARD_TYPE_NUM 4 // カードの種類

#define CARD_1_POSX 200 // 1番のカード位置(x)
#define CARD_1_POSY 320 // 1番のカード位置(y)
#define CARD_DISTANCEX 140 // カード間の距離(x方向)
#define CARD_DISTANCEY 200 // カード間の距離(y方向)

#define BUTTON_POSX 340 // ボタンの位置(x)
#define BUTTON_POSY 120 // ボタンの位置(y)

#define TIMER_LABEL_POSX 550 // ラベルの位置(x)
#define TIMER_LABEL_POSY 120 // ラベルの位置(y)

#define ZORDER_SHOW_CARD 1 // 表示しているカードのZオーダー
#define ZORDER_MOVING_CARD 2 // 移動しているカードのZオーダー

#define TAG_TRUSH_CARD 11 // 捨てられたカードのタグ
#define TAG_BACK_CARD 12 // カードの山のタグ
#define TAG_TIMER_LABEL 13 // 時間ラベルのタグ

#define MOVING_TIME 0.3 // カードのアニメーションの実行

USING_NS_CC;
USING_NS_CC_EXT;

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
    
    // シングルタップイベントの取得
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(_swallowsTouches); // 優先度の高いリスナーだけ実行
    
    // イベント関数の割り当て
    listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(MainScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(MainScene::onTouchEnded, this);
    listener->onTouchCancelled = CC_CALLBACK_2(MainScene::onTouchCancelled, this);
    
    // イベントを追加する
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    // ゲームを初期化する
    initGame();
    
    return true;
}

void MainScene::update(float dt){
    // 経過時間の計算
    m_timer += dt;
    
    auto timerLabel = (Label*)getChildByTag(TAG_TIMER_LABEL);
    if(timerLabel){
        timerLabel->setString(StringUtils::format("%0.2fs",m_timer));
    }
    
    // ゲームの終了判定
    bool finish = true;
    for(int tag = 1; tag <= 10; ++tag){
        auto card = getChildByTag(tag);
        if(card){
            // 場にカードがある
            finish = false;
            break;
        }
    }
    
    if(finish){
        // ゲーム終了
        unscheduleUpdate();
    }
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
    float posX = CARD_1_POSX;
    float posY = CARD_1_POSY - CARD_DISTANCEY;
    
    // 新しいカードを作成する
    auto card = CardSprite::create();
    card->setCard(getCard());
    card->setPosition(posX, posY);
    card->setPosIndex(posIndex);
    card->moveToInitPos();
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
    // 裏を向いているカードを表示する
    showBackCards();
    
    // 時間を表示する
    showTimerLabel();
    
    // ボタンを表示する
    showButton();
}

CardSprite* MainScene::getTouchCard(cocos2d::Touch *touch)
{
    for(int tag = 1; tag <= 10; tag++){
        // 表示されているカードを取得
        auto card = (CardSprite*)getChildByTag(tag);
        if(card &&
           card != m_firstCard &&
           card->getBoundingBox().containsPoint(touch->getLocation())){
            // タップされたカードの場合は、そのカードを返す
            return card;
        }
    }
    return nullptr;
}

bool MainScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    // タップされたカードを取得する
    m_firstCard = getTouchCard(touch);
    if(m_firstCard){
        // 場に出ているカードが選択された場合
        // Zオーダーを変更する
        m_firstCard->setLocalZOrder(ZORDER_MOVING_CARD);
        return true;
    }
    return false;
}

void MainScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    // スワイプしているカードの位置を変更
    m_firstCard->setPosition(m_firstCard->getPosition() + touch->getDelta());
}

void MainScene::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    bool success = false;
    
    // タップしたカードの取得
    auto secondSprite = getTouchCard(touch);
    if(secondSprite){
        // 2枚選択
        if(m_firstCard->getCard().m_number + secondSprite->getCard().m_number == 13){
            // 2枚のカードを足して13になる
            success = true;
        }
    }else{
        if(m_firstCard->getCard().m_number == 13){
            // 1枚のカードで13になる
            success = true;
        }
    }
    
    if(success){
        // 新しいカードを配置する
        if((int)m_cards.size() > 0){
            createCard(m_firstCard->getPosIndex());
        }
        
        // カードを捨てる
        m_firstCard->moveToTrash();
        
        if(secondSprite){
            // もう1枚の新しいカードを配置する
            if((int)m_cards.size() > 0){
                createCard(secondSprite->getPosIndex());
            }
            
            // カードを捨てる
            secondSprite->moveToTrash();
        }
        
        if((int)m_cards.size() <= 0){
            // カードの山を削除する
            removeChildByTag(TAG_BACK_CARD);
        }
    }else{
        // タップしているカードを元の位置に戻す
        m_firstCard->moveBackToInitPos();
        m_firstCard->setLocalZOrder(ZORDER_SHOW_CARD);
    }
    
    // タップしているカードの指定を外す
    m_firstCard = nullptr;
}

void MainScene::onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *unused_event)
{
    // タップ終了と同じ処理を行う
    onTouchEnded(touch, unused_event);
}

void MainScene::showButton()
{
    // ボタンを作成する
    auto button = ControlButton::create(Scale9Sprite::create("button.png"));
    
    // 画像を引き延ばさない設定
    button->setAdjustBackgroundImage(false);
    
    // ボタンの位置設定
    button->setPosition(BUTTON_POSX, BUTTON_POSY);
    
    // ボタンをタップした時に呼び出す関数の設定
    button->addTargetWithActionForControlEvents(this,
                                                cccontrol_selector(MainScene::onTapButton),
                                                Control::EventType::TOUCH_UP_INSIDE);
    
    // ボタンに表示する文字
    button->setTitleForState("Start", Control::State::NORMAL);
    
    // 画面に追加する
    addChild(button);
}

void MainScene::initTrash()
{
    while (true) {
        // ゴミカードが無くなるまでループ
        auto card = getChildByTag(TAG_TRUSH_CARD);
        if(card){
            // ゴミカードが見つかったら削除する
            card->removeFromParent();
        }else{
            break;
        }
    }
}

void MainScene::showBackCards()
{
    auto backCards = getChildByTag(TAG_BACK_CARD);
    if(!backCards){
        // 表示されていない場合
        float posX = CARD_1_POSX;
        float posY = CARD_1_POSY - CARD_DISTANCEY;
        
        // カードの山を表示する
        backCards = Sprite::create("card_back.png");
        backCards->setPosition(posX, posY);
        backCards->setTag(TAG_BACK_CARD);
        addChild(backCards);
    }
}

void MainScene::showTimerLabel()
{
    m_timer = 0;
    
    auto timerLabel = (Label*)getChildByTag(TAG_TIMER_LABEL);
    if(!timerLabel){
        // 時間ラベルを表示する
        timerLabel = Label::createWithSystemFont("", "Arial", 48);
        timerLabel->setPosition(TIMER_LABEL_POSX, TIMER_LABEL_POSY);
        timerLabel->setTag(TAG_TIMER_LABEL);
        addChild(timerLabel);
    }
    timerLabel->setString((StringUtils::format("%0.2fs", m_timer)));
}

void MainScene::onTapButton(cocos2d::Ref *sender, cocos2d::extension::Control::EventType controlEvent)
{
    // update関数の呼び出しを停止
    unscheduleUpdate();
    
    // カードを初期化する
    initCards();
    
    // カードを表示する
    showInitCards();
    
    // カードの山を表示する
    showBackCards();
    
    // ゴミ箱を初期化する
    initTrash();
    
    // 時間を表示する
    showTimerLabel();
    
    // update関数の呼び出しを開始
    scheduleUpdate();
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
    setTexture("card_back.png");
    
    // カードのタグを設定
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

void CardSprite::moveBackToInitPos()
{
    // 移動アニメーションの作成
    float posX = CARD_1_POSX + CARD_DISTANCEX * m_posIndex.m_x;
    float posY = CARD_1_POSY + CARD_DISTANCEY * m_posIndex.m_y;
    auto moveTo = MoveTo::create(MOVING_TIME, Point(posX, posY));
    
    // アニメーションの実行
    runAction(moveTo);
}

void CardSprite::moveToTrash()
{
    // 移動アニメーションの作成
    float posX = CARD_1_POSX + CARD_DISTANCEX * 4;
    float posY = CARD_1_POSY - CARD_DISTANCEY;
    auto moveTo = MoveTo::create(MOVING_TIME, Point(posX, posY));
    
    // アニメーション後に呼び出す関数の作成
    auto func = CallFunc::create([&](){
        setTag(TAG_TRUSH_CARD);
    });
    
    // アクションの直列結合
    auto seq = Sequence::create(moveTo, func, nullptr);
    
    // アニメーションの実行
    runAction(seq);
}

void CardSprite::moveToInitPos()
{
    // カードの位置とタグを指定
    float posX = CARD_1_POSX + CARD_DISTANCEX * m_posIndex.m_x;
    float posY = CARD_1_POSY + CARD_DISTANCEY * m_posIndex.m_y;
    auto moveTo = MoveTo::create(MOVING_TIME, Point(posX, posY));
    
    // カードの回転アニメーションの作成
    auto scale1 = ScaleTo::create(MOVING_TIME / 2, 0, 1);
    auto func1 = CallFunc::create([&](){
        // 画像の表示
        setTexture(getFileName(m_card.m_type));
        // 数字の表示
        showNumber();
    });
    auto scale2 = ScaleTo::create(MOVING_TIME / 2, 1, 1);
    
    // アクションの直列結合
    auto seq1 = Sequence::create(scale1, func1, scale2, nullptr);
    
    // アクションの並列結合
    auto spawn = Spawn::create(moveTo, seq1, nullptr);
    
    // アニメーションの実行
    runAction(spawn);
}