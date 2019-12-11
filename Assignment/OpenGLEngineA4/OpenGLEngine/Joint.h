#pragma once
#include "ECSConfig.h"

namespace Reality
{
	struct Joint
	{
		Joint(Vector3 _joint1 = Vector3(0, 0, 0), Vector3 _joint2 = Vector3(1, 1, 1), Vector3 _joint3 = Vector3(0.0f, 0.0f, 0.0f), Vector3 _joint4 = Vector3(0.0f, 0.0f, 0.0f), int _numJoints = 0, Vector3 _eulerAngles = Vector3(0.0f, 0.0f, 0.0f)) :
			joint0(_joint1), joint1(_joint2), joint2(_joint3), joint3(_joint4), numJoints(_numJoints)
		{
			joint[0] = joint0;
			joint[1] = joint1;
			joint[2] = joint2;
			joint[3] = joint3;
			for (int i = 0; i < numJoints; i++) {
				TransformationMatrix[i] = glm::translate(glm::mat4(1.0f), joint[i]);
			}
		}
	public:
		Vector3 joint0;
		Vector3 joint1;
		Vector3 joint2;
		Vector3 joint3;
		Vector3 joint[4];
		Mat4 TransformationMatrix[4];
		Quaternion orientation;
		int numJoints;
	};
}