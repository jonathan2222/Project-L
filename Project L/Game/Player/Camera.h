#ifndef CAMERA_H
#define CAMERA_H

#include "../../Maths/Maths.h"

class Camera
{
public:
	// A zoom of 1.0f means that the camera has a side length of 0.5 units.
	Camera(const Vec3& position = Vec3(), float zoom = 1.0f, float ratio = 1.0f);
	~Camera() = default;

	void move(const Vec3& translation);

	void setZoom(float zoom, float ratio = 1.0f);
	void setPosition(const Vec3& position);

	Vec3 getPosition() const;
	float getZoom() const;

	Mat4 getMatrix() const;
	Mat4 getView() const;
	Mat4 getProj() const;

private:
	Mat4 view;
	Mat4 proj;
	float zoom;
};

#endif
