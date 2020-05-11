#pragma once

#include <cassert>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_LEFT_HANDED
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace QymEngine {

	namespace Math {

		/* 
		Different graphics backend use different specification for math calulation, such as origin of coordinate system, NDC depth range and use of row space ot column space
		but for the game play side, we dont want to care about the implementation details between graphics backend. So we unify the math specifications for game play side here:
		1. use left hand coordinate system.
		2. depth range from 0 to 1
		3. use column space for vectors and matrix, so M * V instead of V * M
		before upload math data to platform specific shader, we need to do some translation, take OpenGL for example:
		though OpenGL use right hand by default, what we need to do is just to output correct NDC in vertex shader, so upload matrix in left hand is OK
		OpenGL need vertex shader output z from -1 to 1, so we need to modify projection matrix before uploading to map from [0, 1] to [-1, 1]
		*/

		constexpr double M_PI = glm::pi<double>();

		typedef glm::vec2 Vector2f;
		typedef glm::vec3 Vector3f;
		typedef glm::vec4 Vector4f;
		typedef glm::mat3 Matrix3x3f;
		typedef glm::mat4 Matrix4x4f;

		inline Vector4f operator*(const Matrix4x4f & _mat4, const Vector3f & _vec3)
		{
			return _mat4 * Vector4f(_vec3, 1.0f);
		}

		template<typename T>
		T Identity()
		{
			return glm::identity<T>();
		}
		template<typename M>
		typename std::enable_if < std::is_same<Matrix3x3f, M>::value
			|| std::is_same<Matrix4x4f, M>::value, M >::type Transpose(const M & _mat)
		{
			return glm::transpose(_mat);
		}
		template<typename M>
		typename std::enable_if < std::is_same<Matrix3x3f, M>::value
			|| std::is_same<Matrix4x4f, M>::value, M >::type Inverse(const M & _mat)
		{
			return glm::inverse(_mat);
		}
		template<typename T>
		const typename T::value_type* ToPtr(const T & _t)
		{
			return reinterpret_cast<const typename T::value_type*>(&_t);
		}

		inline Matrix4x4f LookAt(Vector3f const& eye, Vector3f const& center, Vector3f const& up)
		{
			return glm::lookAt(eye, center, up);
		}
		inline Matrix4x4f Perspective(float fovy, float aspect, float zNear, float zFar)
		{
			return glm::perspective(glm::radians(fovy), aspect, zNear, zFar);
		}
		inline Matrix4x4f Ortho(float left, float right, float bottom, float top, float zNear, float zFar)
		{
			return glm::ortho(left, right, bottom, top, zNear, zFar);
		}

		inline Matrix4x4f Translate(const Vector3f & _v)
		{
			return glm::translate(glm::identity<Matrix4x4f>(), _v);
		}
		inline Matrix4x4f Rotate(float angle, const Vector3f & axis)
		{
			return glm::rotate(glm::identity<Matrix4x4f>(), glm::radians(angle), axis);
		}
		inline Matrix4x4f Scale(const Vector3f & _v)
		{
			return glm::scale(glm::identity<Matrix4x4f>(), _v);
		}
		inline Matrix4x4f Scale(float _s)
		{
			return glm::scale(glm::identity<Matrix4x4f>(), Vector3f(_s));
		}

		class Ratio2D {
		public:
			float X;
			float Y;

			Ratio2D() :X(0), Y(0) {};
			Ratio2D(float x, float y) :X(x), Y(y) {};
			Ratio2D(const Ratio2D &obj) :X(obj.X), Y(obj.Y) {};

			Ratio2D operator + (const Ratio2D &other) const {
				return Ratio2D(X + other.X, Y + other.Y);
			};
			Ratio2D operator - (const Ratio2D &other) const {
				return Ratio2D(X - other.X, Y - other.Y);
			};

			Ratio2D operator * (const Ratio2D &other) const {
				return Ratio2D(X * other.X, Y * other.Y);
			};

			bool operator == (const Ratio2D &other) const {
				return (X == other.X) && (Y == other.Y);
			}
			bool operator != (const Ratio2D &other) const {
				return (X != other.X) || (Y != other.Y);
			}
			bool IsNatual() const
			{
				return X >= 0 && Y >= 0;
			}
		};

		class Size2D {
		public:
			size_t X;
			size_t Y;

			Size2D() :X(0), Y(0) {};
			Size2D(size_t x, size_t y) :X(x), Y(y) {};

			Size2D(const Size2D &obj) :X(obj.X), Y(obj.Y) {};

			Size2D operator + (const Size2D &other) const {
				return Size2D(X + other.X, Y + other.Y);
			};
			Size2D operator - (const Size2D &other) const {
				return Size2D(X - other.X, Y - other.Y);
			};

			bool operator == (const Size2D &other) const {
				return (X == other.X) && (Y == other.Y);
			}
			bool operator != (const Size2D &other) const {
				return (X != other.X) || (Y != other.Y);
			}

			static Size2D Zero;
		};

		class RectF
		{
		public:
			Ratio2D Location;
			Ratio2D Size;

			RectF() :Location(0, 0), Size(0, 0) {};
			RectF(Ratio2D location, Ratio2D size) :Location(location), Size(size) {};

			bool operator == (const RectF &other) const {
				return (Location == other.Location) && (Size == other.Size);
			}
			bool operator != (const RectF &other) const {
				return (Location != other.Location) || (Size != other.Size);
			}
		};

		class Rect
		{
		public:
			Size2D Location;
			Size2D Size;

			Rect() :Location(0, 0), Size(0, 0) {};
			Rect(Size2D location, Size2D size) :Location(location), Size(size) {};

			Rect operator + (const Rect &other) const
			{
				return Rect(Location + other.Location, Size + other.Size);
			};
			Rect operator - (const Rect &other) const
			{
				return Rect(Location - other.Location, Size - other.Size);
			};

			bool operator == (const Rect &other) const {
				return (Location == other.Location) && (Size == other.Size);
			}
			bool operator != (const Rect &other) const {
				return (Location != other.Location) || (Size != other.Size);
			}
		};


		Size2D operator * (const Size2D & size, const Ratio2D &other);
		Ratio2D operator / (const Size2D & size, const Size2D &other);
		Rect operator * (const RectF & rectF, const Size2D &size);
		RectF operator / (const Rect & rect, const Size2D &size);

		//射线的数学表示，参数形式
		class QymRay
		{
			friend class QymIntersection;
		public:
			QymRay() : m_vStart(Vector3f(0.0f, 0.0f, 0.0f)), m_vDirection(Vector3f(0.0f, 1.0f, 0.0f)) {}
			QymRay(Vector3f _start, Vector3f _dir) : m_vStart(_start), m_vDirection(_dir) {}

			Vector3f GetPoint(float t) const { return m_vStart + m_vDirection * t; }

		private:
			Vector3f m_vStart;
			Vector3f m_vDirection;
		};

		//平面的数学表示，点+法向量形式
		class QymPlane
		{
			friend class QymIntersection;
		public:
			QymPlane() : m_vNormal(Vector3f(0.0f, 1.0f, 0.0f)), m_vPoint(Vector3f(0.0f, 0.0f, 0.0f)) {}
			QymPlane(Vector3f _point, Vector3f _normal) : m_vNormal(_normal), m_vPoint(_point) {}

			//Vector3f GetIntersectionPoint(QymRay ray);
			Vector3f GetNormal() const { return m_vNormal; }
			Vector3f GetPoint() const { return m_vPoint; }

		private:
			Vector3f m_vNormal;
			Vector3f m_vPoint;
		};

		//包围球
		class QymSphere
		{
			friend class QymIntersection;
		public:
			QymSphere() : m_vCenter(Vector3f(0.0f, 0.0f, 0.0f)), m_fRadius(1.0f) {}
			QymSphere(Vector3f _center, float _radius) : m_vCenter(_center), m_fRadius(_radius) {}

			Vector3f GetCenter() const { return m_vCenter; }
			float GetRadius() const { return m_fRadius; }

		private:
			Vector3f m_vCenter;
			float m_fRadius;
		};

		//轴对齐包围盒
		class QymAABB
		{
			friend class QymIntersection;
		public:
			QymAABB() = default;
			QymAABB(Vector3f _min, Vector3f _max) : m_vMin(_min), m_vMax(_max) {}
			QymAABB(float _minX, float _maxX, float _minY, float _maxY, float _minZ, float _maxZ) :
				m_vMin(_minX, _minY, _minZ),
				m_vMax(_maxX, _maxY, _maxZ)
			{}

			Vector3f GetMin() const { return m_vMin; }
			float GetMinX() const { return m_vMin[0]; }
			float GetMinY() const { return m_vMin[1]; }
			float GetMinZ() const { return m_vMin[2]; }
			Vector3f GetMax() const { return m_vMax; }
			float GetMaxX() const { return m_vMax[0]; }
			float GetMaxY() const { return m_vMax[1]; }
			float GetMaxZ() const { return m_vMax[2]; }

			void GetPts(Vector3f(&pts)[8]) const {
				pts[0] = { GetMinX(), GetMinY(), GetMinZ() };
				pts[1] = { GetMinX(), GetMinY(), GetMaxZ() };
				pts[2] = { GetMinX(), GetMaxY(), GetMinZ() };
				pts[3] = { GetMinX(), GetMaxY(), GetMaxZ() };
				pts[4] = { GetMaxX(), GetMinY(), GetMinZ() };
				pts[5] = { GetMaxX(), GetMinY(), GetMaxZ() };
				pts[6] = { GetMaxX(), GetMaxY(), GetMinZ() };
				pts[7] = { GetMaxX(), GetMaxY(), GetMaxZ() };
			}

			void GetPts(Vector4f(&pts)[8]) const {
				pts[0] = { GetMinX(), GetMinY(), GetMinZ(), 1.0f };
				pts[1] = { GetMinX(), GetMinY(), GetMaxZ(), 1.0f };
				pts[2] = { GetMinX(), GetMaxY(), GetMinZ(), 1.0f };
				pts[3] = { GetMinX(), GetMaxY(), GetMaxZ(), 1.0f };
				pts[4] = { GetMaxX(), GetMinY(), GetMinZ(), 1.0f };
				pts[5] = { GetMaxX(), GetMinY(), GetMaxZ(), 1.0f };
				pts[6] = { GetMaxX(), GetMaxY(), GetMinZ(), 1.0f };
				pts[7] = { GetMaxX(), GetMaxY(), GetMaxZ(), 1.0f };
			}

		private:
			Vector3f m_vMin;
			Vector3f m_vMax;
		};

		//方向包围盒
		class QymOBB
		{
			typedef Vector3f PPP;
			friend class QymIntersection;
		public:
			QymOBB() = default;
			QymOBB(const Vector3f(&pts)[8])
			{
				for (int i = 0; i < 8; i++)
				{
					this->m_vPts[i] = pts[i];
				}
			}

			void GetPts(Vector3f(&pts)[8]) const {
				for (int i = 0; i < 8; i++)
				{
					pts[i] = this->m_vPts[i];
				}
			}

			void GetPts(Vector4f(&pts)[8]) const {
				for (int i = 0; i < 8; i++)
				{
					pts[i] = Vector4f(this->m_vPts[i], 1.0f);
				}
			}

			Vector3f & operator[](int i)
			{
				assert(i >= 0 && i < 8);
				return this->m_vPts[i];
			}

		private:
			Vector3f m_vPts[8];
		};

		class QymIntersection
		{
		public:
			//返回交点
			static Vector3f PlaneRayIntersection(const QymPlane & plane, const QymRay & ray);

			//正面返回1，背面返回-1，相交返回0
			static int PlaneSphereIntersection(const QymPlane & plane, const QymSphere & sphere);

			//正面返回1，背面返回-1，相交返回0
			static int PlaneAABBIntersection(const QymPlane & plane, const QymAABB & aabb);

			template<typename BB>
			static bool FrustrumIntersection(const Matrix4x4f & vpm, const BB &bb);
		};

		inline float A2R(float angle) {
			return angle * 0.0174532925f;
		}

		inline float R2A(float radian) {
			return radian * 57.295779513082f;
		}

		inline float myClamp(float f, float min = 0.0f, float max = 1.0f) {
			if (f < min)
				f = min;
			else if (f > max)
				f = max;

			return f;
		}
	}
}
