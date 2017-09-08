#ifndef OBJECT_H
#define OBJECT_H


namespace Logic
{
	class Object
	{
	public:
		Object();
		Object(const Object& other) = delete;
		Object* operator=(const Object& other) = delete;
		~Object();

	private:
	};
}


#endif // !OBJECT_H
