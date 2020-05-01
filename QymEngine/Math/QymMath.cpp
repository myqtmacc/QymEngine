#include "QymMath.h"

using namespace QymEngine;

vec3 QymIntersection::PlaneRayIntersection(const QymPlane & plane, const QymRay & ray) {
	vec3 origin(0.0f, 0.0f, 0.0f);
	float d = -vmath::dot(plane.m_vNormal, (plane.m_vPoint - origin));
	float rl = -vmath::dot(plane.m_vNormal, ray.m_vDirection);

	float t = d / rl;

	vec3 r = ray.GetPoint(t);
	return r;
}


int QymIntersection::PlaneSphereIntersection(const QymPlane & plane, const QymSphere & sphere) {
	float ds = vmath::dot(plane.m_vNormal, sphere.m_vCenter);
	float dp = vmath::dot(plane.m_vNormal, plane.m_vPoint);
	if ((ds - dp) >= sphere.m_fRadius) {
		return 1;
	}
	else if ((ds - dp) <= -sphere.m_fRadius) {
		return -1;
	}
	return 0;
}


int QymIntersection::PlaneAABBIntersection(const QymPlane & plane, const QymAABB & aabb) {
	float minD, maxD;
	const vec3 & n = plane.m_vNormal;
	const vec3 & min = aabb.m_vMin;
	const vec3 & max = aabb.m_vMax;
	if (n[0] > 0.0f) {
		minD = n[0] * min[0];
		maxD = n[0] * max[0];
	}
	else {
		minD = n[0] * max[0];
		maxD = n[0] * min[0];
	}

	if (n[1] > 0.0f) {
		minD += n[1] * min[1];
		maxD += n[1] * max[1];
	}
	else {
		minD = n[1] * max[1];
		maxD = n[1] * min[1];
	}

	if (n[2] > 0.0f) {
		minD = n[2] * min[2];
		maxD = n[2] * max[2];
	}
	else {
		minD = n[2] * max[2];
		maxD = n[2] * min[2];
	}

	float d = vmath::dot(plane.m_vNormal, plane.m_vPoint);
	if (minD >= d) {
		return 1;
	}
	if (maxD <= d) {
		return -1;
	}
	return 0;
}