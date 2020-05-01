#pragma once

#include <cassert>
#include "vmath.h"

namespace QymEngine {
	using namespace vmath;

	class Ratio2D {
	public:
		float X;
		float Y;

		Ratio2D() :X(0), Y(0){};
		Ratio2D(float x, float y) :X(x), Y(y) {};
		Ratio2D(const Ratio2D &obj) :X(obj.X), Y(obj.Y){};

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

		Size2D() :X(0), Y(0){};
		Size2D(size_t x, size_t y) :X(x), Y(y) {};

		Size2D(const Size2D &obj) :X(obj.X), Y(obj.Y){};

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

		RectF() :Location(0, 0), Size(0, 0){};
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

		Rect() :Location(0, 0), Size(0, 0){};
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
		QymRay() : m_vStart(vec3(0.0f, 0.0f, 0.0f)), m_vDirection(vec3(0.0f, 1.0f, 0.0f)) {}
		QymRay(vec3 _start, vec3 _dir) : m_vStart(_start), m_vDirection(_dir) {}

		vec3 GetPoint(float t) const { return m_vStart + m_vDirection * t; }

	private:
		vec3 m_vStart;
		vec3 m_vDirection;
	};

	//平面的数学表示，点+法向量形式
	class QymPlane
	{
		friend class QymIntersection;
	public:
		QymPlane() : m_vNormal(vec3(0.0f, 1.0f, 0.0f)), m_vPoint(vec3(0.0f, 0.0f, 0.0f)) {}
		QymPlane(vec3 _point, vec3 _normal) : m_vNormal(_normal), m_vPoint(_point) {}

		//vec3 GetIntersectionPoint(QymRay ray);
		vec3 GetNormal() const { return m_vNormal; }
		vec3 GetPoint() const { return m_vPoint; }

	private:
		vec3 m_vNormal;
		vec3 m_vPoint;
	};

	//包围球
	class QymSphere
	{
		friend class QymIntersection;
	public:
		QymSphere() : m_vCenter(vec3(0.0f, 0.0f, 0.0f)), m_fRadius(1.0f) {}
		QymSphere(vec3 _center, float _radius) : m_vCenter(_center), m_fRadius(_radius) {}

		vec3 GetCenter() const { return m_vCenter; }
		float GetRadius() const { return m_fRadius; }

	private:
		vec3 m_vCenter;
		float m_fRadius;
	};

	//轴对齐包围盒
	class QymAABB
	{
		friend class QymIntersection;
	public:
		QymAABB() = default;
		QymAABB(vec3 _min, vec3 _max) : m_vMin(_min), m_vMax(_max) {}
		QymAABB(float _minX, float _maxX, float _minY, float _maxY, float _minZ, float _maxZ) :
			m_vMin(_minX, _minY, _minZ),
			m_vMax(_maxX, _maxY, _maxZ)
		{}

		vec3 GetMin() const { return m_vMin; }
		float GetMinX() const { return m_vMin[0]; }
		float GetMinY() const { return m_vMin[1]; }
		float GetMinZ() const { return m_vMin[2]; }
		vec3 GetMax() const { return m_vMax; }
		float GetMaxX() const { return m_vMax[0]; }
		float GetMaxY() const { return m_vMax[1]; }
		float GetMaxZ() const { return m_vMax[2]; }

	private:
		vec3 m_vMin;
		vec3 m_vMax;
	};

	//方向包围盒
	class QymOBB
	{
		typedef vec3 PPP;
		friend class QymIntersection;
	public:
		QymOBB() = default;
		QymOBB(const vec3 (& pts)[8])
		{
			for (int i = 0; i < 8; i++)
			{
				this->m_vPts[i] = pts[i];
			}
		}

		void GetPts(vec3 (& pts)[8]) {
			for (int i = 0; i < 8; i++)
			{
				pts[i] = this->m_vPts[i];
			}
		}

		vec3 & operator[](int i)
		{
			assert(i >= 0 && i < 8);
			return this->m_vPts[i];
		}

	private:
		vec3 m_vPts[8];
	};

	class QymIntersection
	{
	public:
		//返回交点
		static vec3 PlaneRayIntersection(const QymPlane & plane, const QymRay & ray);

		//正面返回1，背面返回-1，相交返回0
		static int PlaneSphereIntersection(const QymPlane & plane, const QymSphere & sphere);

		//正面返回1，背面返回-1，相交返回0
		static int PlaneAABBIntersection(const QymPlane & plane, const QymAABB & aabb);
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
