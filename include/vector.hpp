#ifndef _CFG_PARSER_VECTOR_HPP_
#define _CFG_PARSER_VECTOR_HPP_

#include <array>


struct Vec2
{
	Vec2() noexcept
	{
	}

	Vec2(const float value) noexcept : x(value), y(value)
	{
	}
	
	Vec2(const float value_x, const float value_y) noexcept : x(value_x), y(value_y)
	{
	}

	Vec2(const std::array<float, 2u>& list) noexcept : x(list[0]), y(list[1])
	{
	}

	float x = 0.0f, y = 0.0f;
};

struct Vec3
{
	Vec3() noexcept
	{
	}

	Vec3(const float value) noexcept : x(value), y(value), z(value)
	{
	}

	Vec3(const float value_x, const float value_y, const float value_z) noexcept : x(value_x), y(value_y), z(value_z)
	{
	}

	Vec3(const std::array<float, 3u>& list) noexcept : x(list[0]), y(list[1]), z(list[2])
	{
	}

	float x = 0.0f, y = 0.0f, z = 0.0f;
};

struct Vec4
{
	Vec4() noexcept
	{
	}
	
	Vec4(const float value) noexcept : x(value), y(value), z(value), w(value)
	{
	}
	
	Vec4(const float value_x, const float value_y, const float value_z, const float value_w) noexcept : x(value_x), y(value_y), z(value_z), w(value_w)
	{
	}

	Vec4(const std::array<float, 4u>& list) noexcept : x(list[0]), y(list[1]), z(list[2]), w(list[3])
	{
	}

	float x = 0.0f, y = 0.0f, z = 0.0f, w = 0.0f;
};

struct Vec2d
{
	Vec2d() noexcept
	{
	}

	Vec2d(const double value) noexcept : x(value), y(value)
	{
	}

	Vec2d(const double value_x, const double value_y) noexcept : x(value_x), y(value_y)
	{
	}

	Vec2d(const std::array<double, 2u>& list) noexcept : x(list[0]), y(list[1])
	{
	}

	double x = 0.0, y = 0.0;
};

struct Vec3d
{
	Vec3d() noexcept
	{
	}

	Vec3d(const double value) noexcept : x(value), y(value), z(value)
	{
	}

	Vec3d(const double value_x, const double value_y, const double value_z) noexcept : x(value_x), y(value_y), z(value_z)
	{
	}

	Vec3d(const std::array<double, 3u>& list) noexcept
	{
		x = list[0];
		y = list[1];
		z = list[2];
	}

	double x = 0.0, y = 0.0, z = 0.0;
};

struct Vec4d
{
	Vec4d() noexcept
	{
	}

	Vec4d(const double value) noexcept : x(value), y(value), z(value), w(value)
	{
	}

	Vec4d(const double value_x, const double value_y, const double value_z, const double value_w) noexcept : x(value_x), y(value_y), z(value_z), w(value_w)
	{
	}

	Vec4d(const std::array<double, 4u>& list) noexcept : x(list[0]), y(list[1]), z(list[2]), w(list[3])
	{
	}

	double x = 0.0, y = 0.0, z = 0.0, w = 0.0;
};

struct Vec2b
{
	Vec2b() noexcept
	{
	}

	Vec2b(const bool value) noexcept : x(value), y(value)
	{
	}

	Vec2b(const bool value_x, const bool value_y) noexcept : x(value_x), y(value_y)
	{
	}

	Vec2b(const std::array<bool, 2u>& list) noexcept : x(list[0]), y(list[1])
	{
	}

	bool x = false, y = false;
};

struct Vec3b
{
	Vec3b() noexcept
	{
	}

	Vec3b(const bool value) noexcept : x(value), y(value), z(value)
	{
	}

	Vec3b(const bool value_x, const bool value_y, const bool value_z) noexcept : x(value_x), y(value_y), z(value_z)
	{
	}

	Vec3b(const std::array<bool, 3u>& list) noexcept : x(list[0]), y(list[1]), z(list[2])
	{
	}

	bool x = false, y = false, z = false;
};

struct Vec4b
{
	Vec4b() noexcept
	{
	}

	Vec4b(const bool value) noexcept : x(value), y(value), z(value), w(value)
	{
	}

	Vec4b(const bool value_x, const bool value_y, const bool value_z, const bool value_w) noexcept : x(value_x), y(value_y), z(value_z), w(value_w)
	{
	}

	Vec4b(const std::array<bool, 4u>& list) noexcept : x(list[0]), y(list[1]), z(list[2]), w(list[3])
	{
	}

	bool x = false, y = false, z = false, w = false;
};

struct Vec2i
{
	Vec2i() noexcept
	{
	}
	
	Vec2i(const int value) noexcept : x(value), y(value)
	{
	}
	
	Vec2i(const int value_x, const int value_y) noexcept : x(value_x), y(value_y)
	{
	}

	Vec2i(const std::array<bool, 2u>& list) noexcept : x(list[0]), y(list[1])
	{
	}

	int x = 0, y = 0;
};


struct Vec3i
{
	Vec3i() noexcept
	{
	}
	
	Vec3i(const int value) noexcept : x(value), y(value), z(value)
	{
	}
	
	Vec3i(const int value_x, const int value_y, const int value_z) noexcept : x(value_x), y(value_y), z(value_z)
	{
	}

	Vec3i(const std::array<bool, 3u>& list) noexcept : x(list[0]), y(list[1]), z(list[2])
	{
	}

	int x = 0, y = 0, z = 0;
};


struct Vec4i
{
	Vec4i() noexcept
	{
	}
	
	Vec4i(const int value) noexcept : x(value), y(value), z(value), w(value)
	{
	}
	
	Vec4i(const int value_x, const int value_y, const int value_z, const int value_w) noexcept : x(value_x), y(value_y), z(value_z), w(value_w)
	{
	}

	Vec4i(const std::array<int, 4u>& list) noexcept : x(list[0]), y(list[1]), z(list[2]), w(list[3])
	{
	}

	int x = 0, y = 0, z = 0, w = 0;
};


struct Vec2u
{
	Vec2u() noexcept
	{
	}
	
	Vec2u(const unsigned int value) noexcept : x(value), y(value)
	{
	}
	
	Vec2u(const unsigned int value_x, const unsigned int value_y) noexcept : x(value_x), y(value_y)
	{
	}

	Vec2u(const std::array<unsigned int, 2u>& list) noexcept : x(list[0]), y(list[1])
	{
	}

	unsigned int x = 0u, y = 0u;
};


struct Vec3u
{
	Vec3u() noexcept
	{
	}
	
	Vec3u(const unsigned int value) noexcept : x(value), y(value), z(value)
	{
	}

	Vec3u(const unsigned int value_x, const unsigned int value_y, const unsigned int value_z) noexcept : x(value_x), y(value_y), z(value_z)
	{
	}

	Vec3u(const std::array<unsigned int, 3u>& list) noexcept : x(list[0]), y(list[1]), z(list[2])
	{
	}

	unsigned int x = 0u, y = 0u, z = 0u;
};


struct Vec4u
{
	Vec4u() noexcept
	{
	}

	Vec4u(const unsigned int value) noexcept : x(value), y(value), z(value), w(value)
	{
	}

	Vec4u(const unsigned int value_x, const unsigned int value_y, const unsigned int value_z, const unsigned int value_w) noexcept : x(value_x), y(value_y), z(value_z), w(value_w)
	{
	}

	Vec4u(const std::array<unsigned int, 4u>& list) noexcept : x(list[0]), y(list[1]), z(list[2]), w(list[3])
	{
	}

	unsigned int x, y, z, w;
};

#endif
