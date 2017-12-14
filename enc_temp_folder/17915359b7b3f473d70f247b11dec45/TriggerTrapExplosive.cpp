#include <AI\Trigger\TriggerTrapExplosive.h>
#include <Graphics\include\Particles\ParticleSystem.h>
#include <Player\Player.h>
#include <AI\Enemy.h>
using namespace Logic;

TriggerTrapExplosive::TriggerTrapExplosive(btRigidBody* body) : 
    Trigger(Resources::Models::Jump_Pad, body, { 1.0f, 1.0f, 1.0f },
            TriggerType::TRAP_EXPLOSIVE, 0.f, false) {
    getLightRenderInfo().color = DirectX::Colors::BlanchedAlmond;
}

void TriggerTrapExplosive::onCollision(PhysicsObject &obj, btVector3 contactPoint, float dmgMultiplier)
{
    if (getIsActive()) {
        if (Player *p = dynamic_cast<Player*> (&obj)) {
            p->takeDamage(1);
            Graphics::FXSystem->addEffect("DamageBoom", getPosition());
        }
        else if (Enemy *e = dynamic_cast<Enemy*> (&obj)) {
            e->damage(5000);
            Graphics::FXSystem->addEffect("DamageBoom", getPosition());
        }
        setIsActive(false);
        setShouldRemove(true);
    }
}