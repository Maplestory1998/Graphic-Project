#pragma once

#include <glm/glm/glm.hpp>
#include "camera.h"

void MoveCamera(SCamera& in, SCamera::Camera_Movement direction)
{
	switch (direction)
	{
	case SCamera::FORWARD:
		in.Position += in.MovementSpeed * in.Front;
		break;
	case SCamera::BACKWARD:
		in.Position -= in.MovementSpeed * in.Front;
		break;
	case SCamera::LEFT:
		in.Position -= in.MovementSpeed * in.Right;
		break;
	case SCamera::RIGHT:
		in.Position += in.MovementSpeed * in.Right;
		break;
	default:
		break;
	}

	return;
}

void OrientCamera(SCamera& in, float xoffset, float yoffset)
{
	in.Yaw += xoffset * in.MouseSensitivity;
	in.Pitch += yoffset * in.MouseSensitivity;

	if (in.Pitch > 89.0f)	in.Pitch = 89.0f;
	if (in.Pitch < -89.0f)	in.Pitch = -89.0f;		

	glm::vec3 target;
	target.x = cos(glm::radians(in.Yaw)) * cos(glm::radians(in.Pitch)) ;
	target.y = sin(glm::radians(in.Pitch)) ;
	target.z = sin(glm::radians(in.Yaw)) * cos(glm::radians(in.Pitch)) ;

	in.Front = glm::normalize(target);
	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));
}