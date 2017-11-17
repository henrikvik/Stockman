#include <Physics/FuncContactResult.h>
using namespace Logic;

FunContactResult::FunContactResult(std::function<bool(btBroadphaseProxy* proxy)> needsCollision,
    std::function <btScalar(btManifoldPoint& cp, const btCollisionObjectWrapper* colObj0, int partId0, int index0,
    const btCollisionObjectWrapper* colObj1, int partId1, int index1)> singleRes) {
    m_needsCollision = needsCollision;
    m_singleRes = singleRes;
}

bool FunContactResult::needsCollision(btBroadphaseProxy* proxy) const {
    return m_needsCollision(proxy);
}

btScalar FunContactResult::addSingleResult(btManifoldPoint& cp,
    const btCollisionObjectWrapper* colObj0, int partId0, int index0,
    const btCollisionObjectWrapper* colObj1, int partId1, int index1)
{
    return m_singleRes(cp, colObj0, partId0, index0, colObj1, partId1, index1);
}