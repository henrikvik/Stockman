#ifndef OBJECT_H
#define OBJECT_H

#include <Misc/NonCopyable.h>

namespace Logic
{
	class Object : public NonCopyable
	{
	public:
        Object() {};
        virtual ~Object() {};
        virtual void render() const = 0;
    };
}


#endif // !OBJECT_H
