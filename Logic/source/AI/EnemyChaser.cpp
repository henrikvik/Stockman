#include <AI\EnemyChaser.h>
using namespace Logic;

const float EnemyChaser::MAX_HP = 3;
const float EnemyChaser::BASE_DAMAGE = 5;
const float EnemyChaser::MOVE_SPEED = 18;

EnemyChaser::EnemyChaser(btRigidBody* body)
    : Enemy(Graphics::ModelID::CUBE, body, { 1, 1, 1 }, MAX_HP, BASE_DAMAGE, MOVE_SPEED, 0, 0)
{
    setBehavior(MELEE);
}

EnemyChaser::~EnemyChaser()
{
}