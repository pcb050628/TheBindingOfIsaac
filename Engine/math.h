#pragma once

float Lerp(float a, float b, float t = 0.5f)
{
	return (1 - t) * a + t * b;
}

struct Vec2
{
	float x;
	float y;

	static Vec2 Zero() { return Vec2(0.f, 0.f); }
	static Vec2 One() { return Vec2(1.f, 1.f); }

	Vec2()
		: x(0.f)
		, y(0.f)
	{}

	Vec2(float value)
		: x(value)
		, y(value)
	{}

	Vec2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}

	float Length()
	{
		return sqrtf(x * x + y * y);
	}

	void Normalize()
	{
		float f = Length();

		x /= f;
		y /= f;
	}

	Vec2 operator + (const Vec2& _other)
	{
		return Vec2(x + _other.x, y + _other.y);
	}

	Vec2 operator - (const Vec2& _other)
	{
		return Vec2(x - _other.x, y - _other.y);
	}

	Vec2 operator * (const Vec2& _other)
	{
		return Vec2(x * _other.x, y * _other.y);
	}

	Vec2 operator / (const Vec2& _other)
	{
		return Vec2(x / _other.x, y / _other.y);
	}

	void operator += (const Vec2& _other)
	{
		x += _other.x;
		y += _other.y;
	}

	void operator -= (const Vec2& _other)
	{
		x -= _other.x;
		y -= _other.y;
	}

	void operator *= (const Vec2& _other)
	{
		x *= _other.x;
		y *= _other.y;
	}

	void operator /= (const Vec2& _other)
	{
		x /= _other.x;
		y /= _other.y;
	}
};