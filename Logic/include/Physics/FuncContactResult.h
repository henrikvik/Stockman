#include <btBulletCollisionCommon.h>
#include <functional>

namespace Logic
{
    class FunContactResult : public btCollisionWorld::ContactResultCallback {
    private:
        std::function<bool(btBroadphaseProxy* proxy)> m_needsCollision;
        std::function <btScalar(btManifoldPoint& cp,
            const btCollisionObjectWrapper* colObj0, int partId0, int index0,
            const btCollisionObjectWrapper* colObj1, int partId1, int index1)>
            m_singleRes;
    public:
        FunContactResult(
            std::function<bool(btBroadphaseProxy* proxy)> needsCollision,
            std::function <btScalar(btManifoldPoint& cp,
                const btCollisionObjectWrapper* colObj0, int partId0, int index0,
                const btCollisionObjectWrapper* colObj1, int partId1, int index1)> singleRes
        );

        virtual bool needsCollision(btBroadphaseProxy* proxy) const;

        //! Called with each contact for your own processing (e.g. test if contacts fall in within sensor parameters)
        virtual btScalar addSingleResult(btManifoldPoint& cp,
            const btCollisionObjectWrapper* colObj0, int partId0, int index0,
            const btCollisionObjectWrapper* colObj1, int partId1, int index1);
    };
};