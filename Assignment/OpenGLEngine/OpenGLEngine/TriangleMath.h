#pragma once
#include "ECSConfig.h"

namespace Reality {
	class TriangleMath {
	public:
		static float DOT(Vector3 a, Vector3 b) {
			float temp = (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
			return temp;
		}
		static Vector3 CROSS(Vector3 a, Vector3 b) {
			Vector3 temp = Vector3((a.y*b.z) - (a.z*b.y), (a.z*b.x) - (a.x*b.z), (a.x*b.y) - (a.y*b.x));
			return temp;
		}
		static bool BARY(Vector3 a) {
			if (a.x <= 1 && a.x >= 0) {
				if (a.y <= 1 && a.y >= 0) {
					if (a.z <= 1 && a.z >= 0) {
						return true;
					}
				}
			}
			return false;
		}
		static Vector3 SDIVISION(Vector3 a) {
			Vector3 temp;
			temp.x = a.x / a.length();
			temp.y = a.y / a.length();
			temp.z = a.z / a.length();
			return temp;
		}
	};
}
