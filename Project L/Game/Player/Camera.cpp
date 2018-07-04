#include "Camera.h"

#include "../../Maths/MathsTransform.h"

#define Z_NEAR	0.001f
#define Z_FAR	100.0f

Camera::Camera(const Vec3 & position, float zoom, float ratio)
{
	const float halfZoom = zoom / 2.0f;
	this->zoom = zoom;
	this->proj = MathsTransform::orthographic(-halfZoom* ratio, halfZoom*ratio, halfZoom, -halfZoom, Z_NEAR, Z_FAR);
	this->view = MathsTransform::translate(-position.x, -position.y, -position.z);
}

void Camera::move(const Vec3 & translation)
{
	this->view[3][0] -= translation.x;
	this->view[3][1] -= translation.y;
	this->view[3][2] -= translation.z;
}

void Camera::setZoom(float zoom, float ratio)
{
	const float halfZoom = zoom / 2.0f;
	this->zoom = zoom;
	this->proj = MathsTransform::orthographic(this->proj, -halfZoom* ratio, halfZoom*ratio, halfZoom, -halfZoom, Z_NEAR, Z_FAR);
}

void Camera::setPosition(const Vec3 & position)
{
	this->view[3][0] = -position.x;
	this->view[3][1] = -position.y;
	this->view[3][2] = -position.z;
}

Vec3 Camera::getPosition() const
{
	return { -this->view[3][0], -this->view[3][1], -this->view[3][2] };
}

float Camera::getZoom() const
{
	return this->zoom;
}

Mat4 Camera::getMatrix() const
{
	return this->proj*this->view;
}

Mat4 Camera::getView() const
{
	return this->view;
}

Mat4 Camera::getProj() const
{
	return this->proj;
}
