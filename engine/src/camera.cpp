#include "camera.h"

namespace engine
{
	Camera::Camera(GLuint uboBP) : uboBP(uboBP)
	{
		glGenBuffers(1, &uboId);
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		{
			glBufferData(GL_UNIFORM_BUFFER, sizeof(Matrix4) * 2, 0, GL_STREAM_DRAW);
			glBindBufferBase(GL_UNIFORM_BUFFER, uboBP, uboId);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	Camera::~Camera() { glDeleteBuffers(1, &uboId); }

	GLuint Camera::getUboBP() const { return uboBP; }

	Matrix4 Camera::getProjectionMatrix() const { return projectionMatrix; }
	void Camera::setOrtho(float l, float r, float b, float t, float n, float f)
	{
		projectionMatrix = Matrix4::CreateOrthographicProjection(l, r, b, t, n, f);
	}
	void Camera::setPerspective(float fov, float aspect, float n, float f)
	{
		projectionMatrix = Matrix4::CreatePerspectiveProjection(fov, aspect, n, f);
	}

	Matrix4 Camera::getViewMatrix() const { return viewMatrix; }
	void Camera::lookAt(Vector3 eye, Vector3 center)
	{
		Vector3 up = Vector3::up();
		Matrix4 matrix = Matrix4::CreateLookAt(eye, center, up);

		Vector3 view = center - eye;
		pitch = atan(view.y / sqrt(view.x * view.x + view.z * view.z));
		yaw = atan2(view.z, view.x);
		position = eye;
	}

	float Camera::getPitch() { return pitch; }
	float Camera::getYaw() { return yaw; }
	Vector3 Camera::getPosition() { return position; }

	void Camera::update(float elapsedSecs, Vector2 cursorDiff)
	{
		Engine& engine = Engine::getInstance();

		// handle mouse
		pitch += -cursorDiff.y * CAMERA_MOUSE_SENSITIVITY;
		yaw += cursorDiff.x * CAMERA_MOUSE_SENSITIVITY;

		if (pitch > MAX_TILT) pitch = MAX_TILT;
		if (pitch < -MAX_TILT) pitch = -MAX_TILT;

		// handle keyboard
		bool forwardPressed = engine.getKey(GLFW_KEY_W) == GLFW_PRESS;
		bool backwardsPressed = engine.getKey(GLFW_KEY_S) == GLFW_PRESS;

		bool leftPressed = engine.getKey(GLFW_KEY_A) == GLFW_PRESS;
		bool rightPressed = engine.getKey(GLFW_KEY_D) == GLFW_PRESS;

		bool ascendPressed = engine.getKey(GLFW_KEY_SPACE) == GLFW_PRESS;
		bool descendPressed = engine.getKey(GLFW_KEY_C) == GLFW_PRESS;

		Vector3 cameraVelocity = Vector3(0, 0, 0);
		if (forwardPressed && !backwardsPressed) cameraVelocity.z = CAMERA_SPEED;
		if (!forwardPressed && backwardsPressed) cameraVelocity.z = -CAMERA_SPEED;

		if (leftPressed && !rightPressed) cameraVelocity.x = -CAMERA_SPEED;
		if (!leftPressed && rightPressed) cameraVelocity.x = CAMERA_SPEED;

		if (ascendPressed && !descendPressed) cameraVelocity.y = CAMERA_SPEED;
		if (!ascendPressed && descendPressed) cameraVelocity.y = -CAMERA_SPEED;

		// calculate view matrix
		Vector3 front = Vector3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));
		Vector3 right = front.cross(Vector3::up()).normalized();
		Vector3 up = right.cross(front).normalized();

		Vector3 frontWithoutY = front;
		frontWithoutY.y = 0;
		frontWithoutY.normalize();

		Vector3 rightWithoutY = right;
		rightWithoutY.y = 0;
		rightWithoutY.normalize();

		position += cameraVelocity.x * CAMERA_SPEED * elapsedSecs * rightWithoutY;
		position += cameraVelocity.y * CAMERA_SPEED * elapsedSecs * Vector3::up();
		position += cameraVelocity.z * CAMERA_SPEED * elapsedSecs * frontWithoutY;

		viewMatrix = Matrix4(
			right.x, right.y, right.z, -right.dot(position),
			up.x, up.y, up.z, -up.dot(position),
			-front.x, -front.y, -front.z, front.dot(position),
			0, 0, 0, 1
		);

		// write to uniform buffer object
		glBindBuffer(GL_UNIFORM_BUFFER, uboId);
		{
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Matrix4), &viewMatrix);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Matrix4), sizeof(Matrix4), &projectionMatrix);
		}
	}
}
