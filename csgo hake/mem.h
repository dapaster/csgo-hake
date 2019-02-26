#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <math.h>

#define PI 3.14159265f

class mem
{
public:
	static HANDLE handle;

	static unsigned int GetModuleBaseAddress(DWORD pid, const char* module);

	template <typename T>
	static T read(void* address)
	{
		T var;
		if (!ReadProcessMemory(handle, address, &var, sizeof(T), 0));
			//std::cout << "Read failed";

		return var;
	}
	
	static void read(void* address, int size, unsigned char* out)
	{
		if (!ReadProcessMemory(handle, address, out, size, 0));
			//std::cout << "Read failed";
	}

	template <typename T>
	static void write(void* address, T value)
	{
		if (!WriteProcessMemory(handle, address, &value, sizeof(T), 0));
			//std::cout << "Write failed";
	}

	template <typename T>
	static void write(void* address, int size, T* in)
	{
		if (!WriteProcessMemory(handle, address, in, size, 0));
			//std::cout << "Write failed";
	}
};

struct angle
{
	float yaw;
	float pitch;
};

struct vec3
{
	float x, y, z;

	const static int size = sizeof(float) * 3;

	static vec3 parse(unsigned char* raw, int separator = sizeof(float))
	{
		vec3 vec;

		unsigned char tmp[sizeof(float)];

		for (int i = 0; i < 3; i++)
		{

			for (int v = 0; v < 4; v++)
			{
				tmp[v] = *(raw + i * separator + v);
			}

			float a;
			memcpy(&a, tmp, sizeof(float));

			switch (i)
			{
			case 0:
				vec.x = a;
				break;
			case 1:
				vec.y = a;
				break;
			case 2:
				vec.z = a;
				break;
			
			}

		}

		return vec;
	}

	vec3 operator - (vec3 a)
	{
		vec3 v;

		v.x = x - a.x;
		v.y = y - a.y;
		v.z = z - a.z;

		return v;
	}

	vec3 operator + (vec3 a)
	{
		vec3 v;

		v.x = x + a.x;
		v.y = y + a.y;
		v.z = z + a.z;

		return v;
	}

	vec3 operator * (float a)
	{
		vec3 v;

		v.x = x * a;
		v.y = y * a;
		v.z = z * a;

		return v;
	}

	static angle calcAngle(vec3 resVec)
	{
		angle a;

		a.yaw = (atan2f(resVec.y, resVec.x) * 180 / PI);

		float straightDist = sqrtf(pow(resVec.x, 2) + pow(resVec.y, 2));
		a.pitch = -(atan2f(resVec.z, straightDist) * 180 / PI);


		return a;
	}

	void print()
	{
		printf("x: %f, y: %f, z: %f\n", x, y, z);
	}

};
