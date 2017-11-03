#ifndef OBJECT_H
#define OBJECT_H


namespace Logic
{
	class Object
	{
	public:
        Object() {};
        virtual ~Object() {};
        Object(const Object &other) = delete;
        Object* operator=(const Object &other) = delete;
    };
}


#endif // !OBJECT_H
