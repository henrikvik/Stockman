#include <btBulletCollisionCommon.h>

namespace Logic
{
	enum ShapeType
	{
		ShapeTypePoint,
		ShapeTypeCube,			
		ShapeTypeSphere,
		ShapeTypePlane,
		ShapeTypeCylinder
	};

	class Shape
	{
	public:
		Shape(btVector3 position, btVector3 rotation)
		{
			m_shape = ShapeTypePoint;
			m_position = position;
			m_rotation = rotation;
		}

		void		setPos(btVector3 position)	{ m_position = position;	}
		void		setRot(btVector3 rotation)	{ m_rotation = rotation;	}
		btVector3	getPos() const				{ return m_position;		}
		btVector3	getRot() const				{ return m_rotation;		}

	protected:
		ShapeType	m_shape;
	private:
		btVector3	m_position;
		btVector3	m_rotation;
	};

	/* *
		Creates a 3D cube */
	class Cube : public Shape
	{
	public:
		Cube(btVector3 position, btVector3 rotation, btVector3 dimensions)
		: Shape(position, rotation) {
			m_dimensions = dimensions;
			m_shape = ShapeTypeCube;
		}

		void setDimensions(btVector3 dimensions) { m_dimensions = dimensions; }
		btVector3 getDimensions() const { return m_dimensions; }

	private:
		btVector3	m_dimensions;
	};

	/* *  
		Creates a 2D plane which is always infinite */
	class Plane : public Shape
	{
	public:
		Plane(btVector3 normal)
			: Shape({ 0, 0, 0 }, { 0, 0, 0 }) {
			m_shape = ShapeTypePlane;
			m_normal = normal;
		}

		void setNormal(btVector3 normal)	{ m_normal = normal;	}
		btVector3 getNormal() const			{ return m_normal;		}

	private:
		btVector3	m_normal;	//< Direction of the plane
	};

	/* *
		Creates a 3D sphere */
	class Sphere : public Shape
	{
	public:
		Sphere(btVector3 position, btVector3 rotation, float radius)
			: Shape(position, rotation) {
			m_radius = radius;
			m_shape = ShapeTypeSphere;
		}

		void setNormal(float radius)	{ m_radius = radius;	}
		float getNormal() const			{ return m_radius;		}

	private:
		float		m_radius;
	};

	/* *
		Creates a Line */

	/* *
		Creates a 3D Cylinder */
}