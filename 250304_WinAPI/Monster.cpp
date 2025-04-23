#include "Monster.h"
// #include "Game.h"
// #include "Map.h"
#include "Level.h"
#include "TurnManager.h"
#include "PathFinder.h"
#include "CombatSyetem.h"
#include "MonsterAI.h"

Monster::Monster(FPOINT pos, float speed, int hp, int attDmg, int defense)
{
    position = pos;
    this->speed = speed;
    this->hp = hp;
    this->maxHp = hp;
    this->attackDmg = attDmg;
    this->defense = defense;
    isMoving = false;
    isActive = true;

    type = EntityType::MONSTER;
    curState = EntityState::IDLE;

    targetPos = { position.x + TILE_SIZE, position.y + TILE_SIZE };
    // targetPos = pos

    // 에너지 test
    actionCost = 6.f;
    energyPerTurn = 10.0f;

    // AI
    monsterAi = new MonsterAI;
}

Monster::~Monster()
{
}

void Monster::Act(Level* level)
{
    // AI는 curState 상태와 도착지를 정한다
    monsterAi->Act(curState, level, this);     

    switch(curState)
    {
    case EntityState::IDLE: // 뭐할까
        // 가만히
        return;
    case EntityState::MOVE: // 움직일겨(Hunting, Wanering)-상태변환
        Move(level);
        return;
    case EntityState::ATTACK:   // 공격할겨
        Attack(level);
        return;
    case EntityState::DEAD:     // 죽을겨
        // 애니메이션 끝나면 actor 목록에서 지워야함
        return;
    }
}

void Monster::Attack(Level* level)
{
    if (target)
    {
        CombatSyetem::GetInstance()->ProcessAttack(this, target);
       // SetRandomTargetPos();
       curState = EntityState::IDLE;
    }
}

//void Monster::ActIdle(Level* level)
//{
//    // Idle일 때는 타겟 잡아서 타겟이 있으면 어택모드, 없으면 무브모드 될테야~~
//
//
//    target = level->GetActorAt(targetPos);  // target pos에 타겟이 있다면(actor가 있다면)-actor pos 얻어옴
//    if (target)
//    {
//        if (target->GetType() == EntityType::PLAYER)
//        {
//            curState = EntityState::ATTACK;
//            return;
//        }
//    }
//    curState = EntityState::MOVE;
//}

void Monster::Move(Level* level)
{
    // 이동 순서 체크용
    // Sleep(100);
    
    auto map = level->GetMap(targetPos.x, targetPos.y);
    
    if ((map && !map->CanGo()) || level->GetActorAt(targetPos))
    {
        //SetRandomTargetPos();
        curState = EntityState::IDLE;
        return;
    }
     if (!level->GetMap(targetPos.x, targetPos.x)->CanGo()) return;

    if (map->visible)
    {
        FPOINT delta = targetPos - position;

        float deltaTime = TimerManager::GetInstance()->GetDeltaTime();
        delta.Normalize();

        position.x += speed * deltaTime * delta.x;
        position.y += speed * deltaTime * delta.y;

        delta = targetPos - position;

        // 매직넘버로,,, -> 변수로 dir 저장해두고 쓰면 Dot Product
        if (delta.Length() <= 10.f)
        {
            position = targetPos;
            // 테스트용이라 도착지 정하는건 수정해야함
            //isMoving = false;
            //SetRandomTargetPos();
            curState = EntityState::IDLE;
        }
    }
    else
    {
        position = targetPos;
        //SetRandomTargetPos();
        curState = EntityState::IDLE;
    }

}

//void Monster::SetRandomTargetPos()
//{
//    // 랜덤으로 시드 생성
//    random_device rd;
//
//    // 고성능 엔진을 시드(rd())로 초기화
//    mt19937_64 eng(rd());
//
//    // 범위 설정
//    uniform_int_distribution<int> dist(0, 3);
//    FPOINT dir[] = { {-TILE_SIZE, 0}, {TILE_SIZE, 0}, {0, TILE_SIZE}, {0, -TILE_SIZE} };
//
//    targetPos = position + dir[dist(eng)];
//}
