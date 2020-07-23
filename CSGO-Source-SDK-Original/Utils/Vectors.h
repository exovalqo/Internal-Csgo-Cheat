#pragma once
#include <math.h>
#define ALIGN16				__declspec(align(16))
#define FORCEINLINE			__forceinline
#define VALVE_RAND_MAX		0x7fff
#define FastSqrt(x)			::sqrtf(x)
#define FLOAT32_NAN_BITS    (unsigned long)0x7FC00000	// not a number!
#define FLOAT32_NAN			BitsToFloat(FLOAT32_NAN_BITS);
#define FLT_MAX				3.402823466e+38F       
#define float_NAN			FLOAT32_NAN

inline float BitsToFloat(unsigned long i)
{
	return *reinterpret_cast<float*>(&i);
}

#pragma region Vector2D
class Vector2D
{
public:
	// Members
	float x, y;

	// Construction/destruction
	Vector2D();
	Vector2D(float X, float Y);
	Vector2D(const float* pFloat);

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f);

	// Got any nasty NAN's?
	bool IsValid() const;

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// Initialization methods
	void Random(float minVal, float maxVal);

	// equality
	bool operator==(const Vector2D& v) const;
	bool operator!=(const Vector2D& v) const;

	// arithmetic operations
	Vector2D& operator+=(const Vector2D& v);
	Vector2D& operator-=(const Vector2D& v);
	Vector2D& operator*=(const Vector2D& v);
	Vector2D& operator*=(float s);
	Vector2D& operator/=(const Vector2D& v);
	Vector2D& operator/=(float s);

	// negate the Vector2D components
	void	Negate();

	// Get the Vector2D's magnitude.
	float	Length() const;

	// Get the Vector2D's magnitude squared.
	float	LengthSqr(void) const;

	// return true if this vector is (0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance);
	}

	// Normalize in place and return the old length.
	float	NormalizeInPlace();

	// Compare length.
	bool	IsLengthGreaterThan(float val) const;
	bool	IsLengthLessThan(float val) const;

	// Get the distance from this Vector2D to the other one.
	float	DistTo(const Vector2D& vOther) const;

	// Get the distance from this Vector2D to the other one squared.
	float	DistToSqr(const Vector2D& vOther) const;

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual Vector2D equation (because it's done per-component
	// rather than per-Vector2D).
	void	MulAdd(const Vector2D& a, const Vector2D& b, float scalar);

	// Dot product.
	float	Dot(const Vector2D& vOther) const;

	// assignment
	Vector2D& operator=(const Vector2D& vOther);

	// copy constructors

	// arithmetic operations
	Vector2D	operator-(void) const;

	Vector2D	operator+(const Vector2D& v) const;
	Vector2D	operator-(const Vector2D& v) const;
	Vector2D	operator*(const Vector2D& v) const;
	Vector2D	operator/(const Vector2D& v) const;
	Vector2D	operator*(float fl) const;
	Vector2D	operator/(float fl) const;

	// Cross product between two vectors.
	Vector2D	Cross(const Vector2D& vOther) const;

	// Returns a Vector2D with the min or max in X, Y, and Z.
	Vector2D	Min(const Vector2D& vOther) const;
	Vector2D	Max(const Vector2D& vOther) const;


private:
	// No copy constructors allowed if we're in optimal mode
	Vector2D(const Vector2D& vOther);

};



//-----------------------------------------------------------------------------

const Vector2D vec2_origin(0, 0);
const Vector2D vec2_invalid(FLT_MAX, FLT_MAX);

//-----------------------------------------------------------------------------
// Vector2D related operations
//-----------------------------------------------------------------------------

// Vector2D clear
void Vector2DClear(Vector2D& a);

// Copy
void Vector2DCopy(const Vector2D& src, Vector2D& dst);

// Vector2D arithmetic
void Vector2DAdd(const Vector2D& a, const Vector2D& b, Vector2D& result);
void Vector2DSubtract(const Vector2D& a, const Vector2D& b, Vector2D& result);
void Vector2DMultiply(const Vector2D& a, float b, Vector2D& result);
void Vector2DMultiply(const Vector2D& a, const Vector2D& b, Vector2D& result);
void Vector2DDivide(const Vector2D& a, float b, Vector2D& result);
void Vector2DDivide(const Vector2D& a, const Vector2D& b, Vector2D& result);
void Vector2DMA(const Vector2D& start, float s, const Vector2D& dir, Vector2D& result);

// Store the min or max of each of x, y, and z into the result.
void Vector2DMin(const Vector2D& a, const Vector2D& b, Vector2D& result);
void Vector2DMax(const Vector2D& a, const Vector2D& b, Vector2D& result);

#define Vector2DExpand( v ) (v).x, (v).y

// Normalization
float Vector2DNormalize(Vector2D& v);

// Length
float Vector2DLength(const Vector2D& v);

// Dot Product
float DotProduct2D(const Vector2D& a, const Vector2D& b);

// Linearly interpolate between two vectors
void Vector2DLerp(const Vector2D& src1, const Vector2D& src2, float t, Vector2D& dest);


//-----------------------------------------------------------------------------
//
// Inlined Vector2D methods
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------

inline Vector2D::Vector2D()
{

}
inline Vector2D::Vector2D(float X, float Y)
{
	x = X; y = Y;
}

inline Vector2D::Vector2D(const float* pFloat)
{
	x = pFloat[0]; y = pFloat[1];
}


//-----------------------------------------------------------------------------
// copy constructor
//-----------------------------------------------------------------------------

inline Vector2D::Vector2D(const Vector2D& vOther)
{
	x = vOther.x; y = vOther.y;
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

inline void Vector2D::Init(float ix, float iy)
{
	x = ix; y = iy;
}

inline void Vector2D::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
}

inline void Vector2DClear(Vector2D& a)
{
	a.x = a.y = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

inline Vector2D& Vector2D::operator=(const Vector2D& vOther)
{
	x = vOther.x; y = vOther.y;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

inline float& Vector2D::operator[](int i)
{
	return ((float*)this)[i];
}

inline float Vector2D::operator[](int i) const
{
	return ((float*)this)[i];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

inline float* Vector2D::Base()
{
	return (float*)this;
}

inline float const* Vector2D::Base() const
{
	return (float const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

inline unsigned long& FloatBits(float& f)
{
	return *reinterpret_cast<unsigned long*>(&f);
}

inline bool IsFinite(float f)
{
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
}

inline bool Vector2D::IsValid() const
{
	return IsFinite(x) && IsFinite(y);
}


//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

inline bool Vector2D::operator==(const Vector2D& src) const
{

	return (src.x == x) && (src.y == y);
}

inline bool Vector2D::operator!=(const Vector2D& src) const
{
	return (src.x != x) || (src.y != y);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

inline void Vector2DCopy(const Vector2D& src, Vector2D& dst)
{
	dst.x = src.x;
	dst.y = src.y;
}

inline void	Vector2D::CopyToArray(float* rgfl) const
{

	rgfl[0] = x; rgfl[1] = y;
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------

inline void Vector2D::Negate()
{
	x = -x; y = -y;
}

inline Vector2D& Vector2D::operator+=(const Vector2D& v)
{
	x += v.x; y += v.y;
	return *this;
}

inline Vector2D& Vector2D::operator-=(const Vector2D& v)
{
	x -= v.x; y -= v.y;
	return *this;
}

inline Vector2D& Vector2D::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	return *this;
}

inline Vector2D& Vector2D::operator*=(const Vector2D& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

inline Vector2D& Vector2D::operator/=(float fl)
{

	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;

	return *this;
}

inline Vector2D& Vector2D::operator/=(const Vector2D& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

inline void Vector2DAdd(const Vector2D& a, const Vector2D& b, Vector2D& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
}

inline void Vector2DSubtract(const Vector2D& a, const Vector2D& b, Vector2D& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
}

inline void Vector2DMultiply(const Vector2D& a, float b, Vector2D& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
}

inline void Vector2DMultiply(const Vector2D& a, const Vector2D& b, Vector2D& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
}


inline void Vector2DDivide(const Vector2D& a, float b, Vector2D& c)
{
	float oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
}

inline void Vector2DDivide(const Vector2D& a, const Vector2D& b, Vector2D& c)
{

	c.x = a.x / b.x;
	c.y = a.y / b.y;
}

inline void Vector2DMA(const Vector2D& start, float s, const Vector2D& dir, Vector2D& result)
{
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
}

// FIXME: Remove
// For backwards compatability
inline void	Vector2D::MulAdd(const Vector2D& a, const Vector2D& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
}

inline void Vector2DLerp(const Vector2D& src1, const Vector2D& src2, float t, Vector2D& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
inline float DotProduct2D(const Vector2D& a, const Vector2D& b)
{
	return(a.x * b.x + a.y * b.y);
}

// for backwards compatability
inline float Vector2D::Dot(const Vector2D& vOther) const
{
	return DotProduct2D(*this, vOther);
}


//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline float Vector2DLength(const Vector2D& v)
{
	return (float)FastSqrt(v.x * v.x + v.y * v.y);
}

inline float Vector2D::LengthSqr(void) const
{
	return (x * x + y * y);
}

inline float Vector2D::NormalizeInPlace()
{
	return Vector2DNormalize(*this);
}

inline bool Vector2D::IsLengthGreaterThan(float val) const
{
	return LengthSqr() > val * val;
}

inline bool Vector2D::IsLengthLessThan(float val) const
{
	return LengthSqr() < val * val;
}

inline float Vector2D::Length(void) const
{
	return Vector2DLength(*this);
}


inline void Vector2DMin(const Vector2D& a, const Vector2D& b, Vector2D& result)
{
	result.x = (a.x < b.x) ? a.x : b.x;
	result.y = (a.y < b.y) ? a.y : b.y;
}


inline void Vector2DMax(const Vector2D& a, const Vector2D& b, Vector2D& result)
{
	result.x = (a.x > b.x) ? a.x : b.x;
	result.y = (a.y > b.y) ? a.y : b.y;
}


//-----------------------------------------------------------------------------
// Normalization
//-----------------------------------------------------------------------------
inline float Vector2DNormalize(Vector2D& v)
{
	float l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f;
	}
	return l;
}


//-----------------------------------------------------------------------------
// Get the distance from this Vector2D to the other one 
//-----------------------------------------------------------------------------
inline float Vector2D::DistTo(const Vector2D& vOther) const
{
	Vector2D delta;
	Vector2DSubtract(*this, vOther, delta);
	return delta.Length();
}

inline float Vector2D::DistToSqr(const Vector2D& vOther) const
{
	Vector2D delta;
	Vector2DSubtract(*this, vOther, delta);
	return delta.LengthSqr();
}


//-----------------------------------------------------------------------------
// Computes the closest point to vecTarget no farther than flMaxDist from vecStart
//-----------------------------------------------------------------------------
inline void ComputeClosestPoint2D(const Vector2D& vecStart, float flMaxDist, const Vector2D& vecTarget, Vector2D* pResult)
{
	Vector2D vecDelta;
	Vector2DSubtract(vecTarget, vecStart, vecDelta);
	float flDistSqr = vecDelta.LengthSqr();
	if (flDistSqr <= flMaxDist * flMaxDist)
	{
		*pResult = vecTarget;
	}
	else
	{
		vecDelta /= FastSqrt(flDistSqr);
		Vector2DMA(vecStart, flMaxDist, vecDelta, *pResult);
	}
}



//-----------------------------------------------------------------------------
//
// Slow methods
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Returns a Vector2D with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------

inline Vector2D Vector2D::Min(const Vector2D& vOther) const
{
	return Vector2D(x < vOther.x ? x : vOther.x,
		y < vOther.y ? y : vOther.y);
}

inline Vector2D Vector2D::Max(const Vector2D& vOther) const
{
	return Vector2D(x > vOther.x ? x : vOther.x,
		y > vOther.y ? y : vOther.y);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

inline Vector2D Vector2D::operator-(void) const
{
	return Vector2D(-x, -y);
}

inline Vector2D Vector2D::operator+(const Vector2D& v) const
{
	Vector2D res;
	Vector2DAdd(*this, v, res);
	return res;
}

inline Vector2D Vector2D::operator-(const Vector2D& v) const
{
	Vector2D res;
	Vector2DSubtract(*this, v, res);
	return res;
}

inline Vector2D Vector2D::operator*(float fl) const
{
	Vector2D res;
	Vector2DMultiply(*this, fl, res);
	return res;
}

inline Vector2D Vector2D::operator*(const Vector2D& v) const
{
	Vector2D res;
	Vector2DMultiply(*this, v, res);
	return res;
}

inline Vector2D Vector2D::operator/(float fl) const
{
	Vector2D res;
	Vector2DDivide(*this, fl, res);
	return res;
}

inline Vector2D Vector2D::operator/(const Vector2D& v) const
{
	Vector2D res;
	Vector2DDivide(*this, v, res);
	return res;
}

inline Vector2D operator*(float fl, const Vector2D& v)
{
	return v * fl;
}
#pragma endregion

#pragma region Vector3
class VectorByValue;
class Vector3
{
public:
	float x, y, z;
	Vector3();
	Vector3(float X, float Y, float Z);
	explicit Vector3(float XYZ); ///< broadcast initialize

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	// TODO (Ilya): Should there be an init that takes a single float for consistency?

   // Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// Cast to Vector2D...
	Vector2D& AsVector2D();
	const Vector2D& AsVector2D() const;

	// Initialization methods
	void Random(float minVal, float maxVal);
	inline void Zero(); ///< zero out a vector

	// equality
	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;

	// arithmetic operations
	FORCEINLINE Vector3& operator+=(const Vector3& v);
	FORCEINLINE Vector3& operator-=(const Vector3& v);
	FORCEINLINE Vector3& operator*=(const Vector3& v);
	FORCEINLINE Vector3& operator*=(float s);
	FORCEINLINE Vector3& operator/=(const Vector3& v);
	FORCEINLINE Vector3& operator/=(float s);
	FORCEINLINE Vector3& operator+=(float fl); ///< broadcast add
	FORCEINLINE Vector3& operator-=(float fl); ///< broadcast sub			

// negate the vector components
	void	Negate();

	// Get the vector's magnitude.
	inline float	Length() const;

	// Get the vector's magnitude squared.
	FORCEINLINE float LengthSqr(void) const
	{
		return (x * x + y * y + z * z);
	}

	// return true if this vector is (0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance&&
			z > -tolerance && z < tolerance);
	}

	float	NormalizeInPlace();
	Vector3	Normalized() const;
	bool	IsLengthGreaterThan(float val) const;
	bool	IsLengthLessThan(float val) const;

	// check if a vector is within the box defined by two other vectors
	FORCEINLINE bool WithinAABox(Vector3 const& boxmin, Vector3 const& boxmax);

	// Get the distance from this Vector3 to the other one.
	float	DistTo(const Vector3& vOther) const;

	// Get the distance from this Vector3 to the other one squared.
	// NJS: note, VC wasn't inlining it correctly in several deeply nested inlines due to being an 'out of line' inline.  
	// may be able to tidy this up after switching to VC7
	FORCEINLINE float DistToSqr(const Vector3& vOther) const
	{
		Vector3 delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual Vector3 equation (because it's done per-component
	// rather than per-Vector3).
	void	MulAdd(const Vector3& a, const Vector3& b, float scalar);

	// Dot product.
	float	Dot(const Vector3& vOther) const;

	// assignment
	Vector3& operator=(const Vector3& vOther);

	// 2d
	float	Length2D(void) const;
	float	Length2DSqr(void) const;

	//operator VectorByValue& () { return *((Vector3ByValue*)(this)); }
	//operator const VectorByValue& () const { return *((const Vector3ByValue*)(this)); }


	// arithmetic operations
	Vector3	operator-(void) const;

	Vector3	operator+(const Vector3& v) const;
	Vector3	operator-(const Vector3& v) const;
	Vector3	operator*(const Vector3& v) const;
	Vector3	operator/(const Vector3& v) const;
	Vector3	operator*(float fl) const;
	Vector3	operator/(float fl) const;

	// Cross product between two vectors.
	Vector3	Cross(const Vector3& vOther) const;

	// Returns a vector with the min or max in X, Y, and Z.
	Vector3	Min(const Vector3& vOther) const;
	Vector3	Max(const Vector3& vOther) const;

	operator VectorByValue& () { return *((VectorByValue*)(this)); }
	operator const VectorByValue& () const { return *((const VectorByValue*)(this)); }
};

//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------

inline Vector3::Vector3()
{

}
inline Vector3::Vector3(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
}

inline Vector3::Vector3(float XYZ)
{
	x = y = z = XYZ;
}

//inline Vector::Vector(const float *pFloat)					
//{
//	Assert( pFloat );
//	x = pFloat[0]; y = pFloat[1]; z = pFloat[2];	
//	CHECK_VALID(*this);
//} 


//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

inline void Vector3::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
}

inline void Vector3::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	z = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
}

// This should really be a single opcode on the PowerPC (move r0 onto the vec reg)
inline void Vector3::Zero()
{
	x = y = z = 0.0f;
}

inline void Vector3Clear(Vector3& a)
{
	a.x = a.y = a.z = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

inline Vector3& Vector3::operator=(const Vector3& vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}


//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
inline float& Vector3::operator[](int i)
{
	return ((float*)this)[i];
}

inline float Vector3::operator[](int i) const
{
	return ((float*)this)[i];
}


//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* Vector3::Base()
{
	return (float*)this;
}

inline float const* Vector3::Base() const
{
	return (float const*)this;
}

//-----------------------------------------------------------------------------
// Cast to Vector2D...
//-----------------------------------------------------------------------------

inline Vector2D& Vector3::AsVector2D()
{
	return *(Vector2D*)this;
}

inline const Vector2D& Vector3::AsVector2D() const
{
	return *(const Vector2D*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

inline bool Vector3::IsValid() const
{
	return IsFinite(x) && IsFinite(y) && IsFinite(z);
}

//-----------------------------------------------------------------------------
// Invalidate
//-----------------------------------------------------------------------------

inline void Vector3::Invalidate()
{

	x = y = z = float_NAN;

}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

inline bool Vector3::operator==(const Vector3& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool Vector3::operator!=(const Vector3& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

FORCEINLINE void Vector3Copy(const Vector3& src, Vector3& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}

inline void	Vector3::CopyToArray(float* rgfl) const
{
	rgfl[0] = x, rgfl[1] = y, rgfl[2] = z;
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------
// #pragma message("TODO: these should be SSE")

inline void Vector3::Negate()
{
	x = -x; y = -y; z = -z;
}

FORCEINLINE Vector3& Vector3::operator+=(const Vector3& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

FORCEINLINE Vector3& Vector3::operator-=(const Vector3& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

FORCEINLINE  Vector3& Vector3::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

FORCEINLINE  Vector3& Vector3::operator*=(const Vector3& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

// this ought to be an opcode.
FORCEINLINE Vector3& Vector3::operator+=(float fl)
{
	x += fl;
	y += fl;
	z += fl;
	return *this;
}

FORCEINLINE Vector3& Vector3::operator-=(float fl)
{
	x -= fl;
	y -= fl;
	z -= fl;
	return *this;
}
//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

inline Vector3 Vector3::operator-(void) const
{
	return Vector3(-x, -y, -z);
}

inline Vector3 Vector3::operator+(const Vector3& v) const
{
	Vector3 res;
	res.x = this->x + v.x;
	res.y = this->y + v.y;
	res.z = this->z + v.z;
	return res;
}

inline Vector3 Vector3::operator-(const Vector3& v) const
{
	Vector3 res;
	res.x = this->x - v.x;
	res.y = this->y - v.y;
	res.z = this->z - v.z;
	return res;
}

inline Vector3 Vector3::operator*(float fl) const
{
	Vector3 res;
	res.x = this->x * fl;
	res.y = this->y * fl;
	res.z = this->z * fl;
	return res;
}

inline Vector3 Vector3::operator*(const Vector3& v) const
{
	Vector3 res;
	res.x = this->x * v.x;
	res.y = this->y * v.y;
	res.z = this->z * v.z;
	return res;
}

inline Vector3 Vector3::operator/(float fl) const
{
	Vector3 res;
	res.x = this->x / fl;
	res.y = this->y / fl;
	res.z = this->z / fl;
	return res;
}

inline Vector3 Vector3::operator/(const Vector3& v) const
{
	Vector3 res;
	res.x = this->x / v.x;
	res.y = this->y / v.y;
	res.z = this->z / v.z;
	return res;
}

inline Vector3 operator*(float fl, const Vector3& v)
{
	return v * fl;
}

//-----------------------------------------------------------------------------
// cross product
//-----------------------------------------------------------------------------

inline Vector3 Vector3::Cross(const Vector3& vOther) const
{
	Vector3 res;
	res.x = this->y * vOther.z - this->z * vOther.y;
	res.y = this->z * vOther.x - this->x * vOther.z;
	res.z = this->x * vOther.y - this->y * vOther.x;
	return res;
}


FORCEINLINE  Vector3& Vector3::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}

FORCEINLINE  Vector3& Vector3::operator/=(const Vector3& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}
#pragma endregion
FORCEINLINE float DotProduct(const Vector3& a, const Vector3& b)
{
	return(a.x * b.x + a.y * b.y + a.z * b.z);
}

class ALIGN16 VectorAligned : public Vector3
{
public:
	inline VectorAligned(void) {};
	inline VectorAligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}
private:
	VectorAligned(const VectorAligned& vOther);
	VectorAligned(const Vector3& vOther);

	/*explicit VectorAligned(const Vector3& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}
	*/
public:
	inline VectorAligned& operator=(const Vector3& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

	float w;	// this space is used anyway
};
struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}

	float* operator[](int i) { return m_flMatVal[i]; }
	const float* operator[](int i) const { return m_flMatVal[i]; }
	float* Base() { return &m_flMatVal[0][0]; }
	const float* Base() const { return &m_flMatVal[0][0]; }

	float m_flMatVal[3][4];
};

//========= Copyright Valve Corporation, All rights reserved. ============//
//
// Purpose: 
//
// $NoKeywords: $
//
//=============================================================================//
class VectorByValue : public Vector3
{
public:
	// Construction/destruction:
	VectorByValue(void) : Vector3() {}
	VectorByValue(float X, float Y, float Z) : Vector3(X, Y, Z) {}
	VectorByValue(const VectorByValue& vOther) { *this = vOther; }
};
//=========================================================
// 4D Vector4D
//=========================================================
#include <xmmintrin.h>
class Vector4D
{
public:
	// Members
	float x, y, z, w;

	// Construction/destruction
	Vector4D(void);
	Vector4D(float X, float Y, float Z, float W);
	Vector4D(const float* pFloat);

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f);

	// Got any nasty NAN's?
	bool IsValid() const;

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	inline float* Base();
	inline float const* Base() const;

	// Cast to Vector and Vector2D...
	Vector3& AsVector3D();
	Vector3 const& AsVector3D() const;

	Vector2D& AsVector2D();
	Vector2D const& AsVector2D() const;

	// Initialization methods
	void Random(float minVal, float maxVal);

	// equality
	bool operator==(const Vector4D& v) const;
	bool operator!=(const Vector4D& v) const;

	// arithmetic operations
	Vector4D& operator+=(const Vector4D& v);
	Vector4D& operator-=(const Vector4D& v);
	Vector4D& operator*=(const Vector4D& v);
	Vector4D& operator*=(float s);
	Vector4D& operator/=(const Vector4D& v);
	Vector4D& operator/=(float s);

	// negate the Vector4D components
	void	Negate();

	// Get the Vector4D's magnitude.
	float	Length() const;

	// Get the Vector4D's magnitude squared.
	float	LengthSqr(void) const;

	// return true if this vector is (0,0,0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance&&
			z > -tolerance && z < tolerance&&
			w > -tolerance && w < tolerance);
	}

	// Get the distance from this Vector4D to the other one.
	float	DistTo(const Vector4D& vOther) const;

	// Get the distance from this Vector4D to the other one squared.
	float	DistToSqr(const Vector4D& vOther) const;

	// Copy
	void	CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual Vector4D equation (because it's done per-component
	// rather than per-Vector4D).
	void	MulAdd(Vector4D const& a, Vector4D const& b, float scalar);

	// Dot product.
	float	Dot(Vector4D const& vOther) const;

	// No copy constructors allowed if we're in optimal mode
#ifdef VECTOR_NO_SLOW_OPERATIONS
private:
#else
public:
#endif
	Vector4D(Vector4D const& vOther);

	// No assignment operators either...
	Vector4D& operator=(Vector4D const& src);
};

const Vector4D vec4_origin(0.0f, 0.0f, 0.0f, 0.0f);
const Vector4D vec4_invalid(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX);

//-----------------------------------------------------------------------------
// SSE optimized routines
//-----------------------------------------------------------------------------

class ALIGN16 Vector4DAligned : public Vector4D
{
public:
	Vector4DAligned(void) {}
	Vector4DAligned(float X, float Y, float Z, float W);

	inline void Set(float X, float Y, float Z, float W);
	inline void InitZero(void);

	inline __m128& AsM128() { return *(__m128*) & x; }
	inline const __m128& AsM128() const { return *(const __m128*) & x; }


};

//-----------------------------------------------------------------------------
// Vector4D related operations
//-----------------------------------------------------------------------------

// Vector4D clear
void Vector4DClear(Vector4D& a);

// Copy
void Vector4DCopy(Vector4D const& src, Vector4D& dst);

// Vector4D arithmetic
void Vector4DAdd(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DSubtract(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DMultiply(Vector4D const& a, float b, Vector4D& result);
void Vector4DMultiply(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DDivide(Vector4D const& a, float b, Vector4D& result);
void Vector4DDivide(Vector4D const& a, Vector4D const& b, Vector4D& result);
void Vector4DMA(Vector4D const& start, float s, Vector4D const& dir, Vector4D& result);

// Vector4DAligned arithmetic
void Vector4DMultiplyAligned(Vector4DAligned const& a, float b, Vector4DAligned& result);


#define Vector4DExpand( v ) (v).x, (v).y, (v).z, (v).w

// Normalization
float Vector4DNormalize(Vector4D& v);

// Length
float Vector4DLength(Vector4D const& v);

// Dot Product
float DotProduct4D(Vector4D const& a, Vector4D const& b);

// Linearly interpolate between two vectors
void Vector4DLerp(Vector4D const& src1, Vector4D const& src2, float t, Vector4D& dest);


//-----------------------------------------------------------------------------
//
// Inlined Vector4D methods
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------

inline Vector4D::Vector4D(void)
{
#ifdef _DEBUG
	// Initialize to NAN to catch errors
	x = y = z = w = float_NAN;
#endif
}

inline Vector4D::Vector4D(float X, float Y, float Z, float W)
{
	x = X; y = Y; z = Z; w = W;
}

inline Vector4D::Vector4D(const float* pFloat)
{
	x = pFloat[0]; y = pFloat[1]; z = pFloat[2]; w = pFloat[3];
}


//-----------------------------------------------------------------------------
// copy constructor
//-----------------------------------------------------------------------------

inline Vector4D::Vector4D(const Vector4D& vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z; w = vOther.w;
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

inline void Vector4D::Init(float ix, float iy, float iz, float iw)
{
	x = ix; y = iy; z = iz;	w = iw;

}

inline void Vector4D::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	z = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	w = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
}

inline void Vector4DClear(Vector4D& a)
{
	a.x = a.y = a.z = a.w = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

inline Vector4D& Vector4D::operator=(const Vector4D& vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z; w = vOther.w;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

inline float& Vector4D::operator[](int i)
{
	return ((float*)this)[i];
}

inline float Vector4D::operator[](int i) const
{
	return ((float*)this)[i];
}

//-----------------------------------------------------------------------------
// Cast to Vector and Vector2D...
//-----------------------------------------------------------------------------

inline Vector3& Vector4D::AsVector3D()
{
	return *(Vector3*)this;
}

inline Vector3 const& Vector4D::AsVector3D() const
{
	return *(Vector3 const*)this;
}

inline Vector2D& Vector4D::AsVector2D()
{
	return *(Vector2D*)this;
}

inline Vector2D const& Vector4D::AsVector2D() const
{
	return *(Vector2D const*)this;
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

inline float* Vector4D::Base()
{
	return (float*)this;
}

inline float const* Vector4D::Base() const
{
	return (float const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

inline bool Vector4D::IsValid() const
{
	return IsFinite(x) && IsFinite(y) && IsFinite(z) && IsFinite(w);
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

inline bool Vector4D::operator==(Vector4D const& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z) && (src.w == w);
}

inline bool Vector4D::operator!=(Vector4D const& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z) || (src.w != w);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

inline void Vector4DCopy(Vector4D const& src, Vector4D& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
	dst.w = src.w;
}

inline void	Vector4D::CopyToArray(float* rgfl) const
{
	rgfl[0] = x; rgfl[1] = y; rgfl[2] = z; rgfl[3] = w;
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------

inline void Vector4D::Negate()
{
	x = -x; y = -y; z = -z; w = -w;
}

inline Vector4D& Vector4D::operator+=(const Vector4D& v)
{
	x += v.x; y += v.y; z += v.z; w += v.w;
	return *this;
}

inline Vector4D& Vector4D::operator-=(const Vector4D& v)
{
	x -= v.x; y -= v.y; z -= v.z; w -= v.w;
	return *this;
}

inline Vector4D& Vector4D::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	w *= fl;
	return *this;
}

inline Vector4D& Vector4D::operator*=(Vector4D const& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

inline Vector4D& Vector4D::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	w *= oofl;
	return *this;
}

inline Vector4D& Vector4D::operator/=(Vector4D const& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

inline void Vector4DAdd(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	c.w = a.w + b.w;
}

inline void Vector4DSubtract(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	c.w = a.w - b.w;
}

inline void Vector4DMultiply(Vector4D const& a, float b, Vector4D& c)
{
	c.x = a.x * b;
	c.y = a.y * b;
	c.z = a.z * b;
	c.w = a.w * b;
}

inline void Vector4DMultiply(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
	c.w = a.w * b.w;
}

inline void Vector4DDivide(Vector4D const& a, float b, Vector4D& c)
{
	float oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
	c.z = a.z * oob;
	c.w = a.w * oob;
}

inline void Vector4DDivide(Vector4D const& a, Vector4D const& b, Vector4D& c)
{
	c.x = a.x / b.x;
	c.y = a.y / b.y;
	c.z = a.z / b.z;
	c.w = a.w / b.w;
}

inline void Vector4DMA(Vector4D const& start, float s, Vector4D const& dir, Vector4D& result)
{
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
	result.z = start.z + s * dir.z;
	result.w = start.w + s * dir.w;
}

// FIXME: Remove
// For backwards compatability
inline void	Vector4D::MulAdd(Vector4D const& a, Vector4D const& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
	w = a.w + b.w * scalar;
}

inline void Vector4DLerp(const Vector4D& src1, const Vector4D& src2, float t, Vector4D& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
	dest[2] = src1[2] + (src2[2] - src1[2]) * t;
	dest[3] = src1[3] + (src2[3] - src1[3]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------

inline float DotProduct4D(const Vector4D& a, const Vector4D& b)
{
	return(a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w);
}

// for backwards compatability
inline float Vector4D::Dot(Vector4D const& vOther) const
{
	return DotProduct4D(*this, vOther);
}


//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------

inline float Vector4DLength(Vector4D const& v)
{
	return (float)FastSqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

inline float Vector4D::LengthSqr(void) const
{
	return (x * x + y * y + z * z + w * w);
}

inline float Vector4D::Length(void) const
{
	return Vector4DLength(*this);
}


//-----------------------------------------------------------------------------
// Normalization
//-----------------------------------------------------------------------------

// FIXME: Can't use until we're un-macroed in mathlib.h
inline float Vector4DNormalize(Vector4D& v)
{
	float l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = v.z = v.w = 0.0f;
	}
	return l;
}

//-----------------------------------------------------------------------------
// Get the distance from this Vector4D to the other one 
//-----------------------------------------------------------------------------

inline float Vector4D::DistTo(const Vector4D& vOther) const
{
	Vector4D delta;
	Vector4DSubtract(*this, vOther, delta);
	return delta.Length();
}

inline float Vector4D::DistToSqr(const Vector4D& vOther) const
{
	Vector4D delta;
	Vector4DSubtract(*this, vOther, delta);
	return delta.LengthSqr();
}


//-----------------------------------------------------------------------------
// Vector4DAligned routines
//-----------------------------------------------------------------------------

inline Vector4DAligned::Vector4DAligned(float X, float Y, float Z, float W)
{
	x = X; y = Y; z = Z; w = W;
}

inline void Vector4DAligned::Set(float X, float Y, float Z, float W)
{
	x = X; y = Y; z = Z; w = W;
}

inline void Vector4DAligned::InitZero(void)
{
#if !defined( _X360 )
	this->AsM128() = _mm_set1_ps(0.0f);
#else
	this->AsM128() = __vspltisw(0);
#endif
}

inline void Vector4DMultiplyAligned(Vector4DAligned const& a, Vector4DAligned const& b, Vector4DAligned& c)
{
#if !defined( _X360 )
	c.x = a.x * b.x;
	c.y = a.y * b.y;
	c.z = a.z * b.z;
	c.w = a.w * b.w;
#else
	c.AsM128() = __vmulfp(a.AsM128(), b.AsM128());
#endif
}

inline void Vector4DWeightMAD(float w, Vector4DAligned const& vInA, Vector4DAligned& vOutA, Vector4DAligned const& vInB, Vector4DAligned& vOutB)
{

#if !defined( _X360 )
	vOutA.x += vInA.x * w;
	vOutA.y += vInA.y * w;
	vOutA.z += vInA.z * w;
	vOutA.w += vInA.w * w;

	vOutB.x += vInB.x * w;
	vOutB.y += vInB.y * w;
	vOutB.z += vInB.z * w;
	vOutB.w += vInB.w * w;
#else
	__vector4 temp;

	temp = __lvlx(&w, 0);
	temp = __vspltw(temp, 0);

	vOutA.AsM128() = __vmaddfp(vInA.AsM128(), temp, vOutA.AsM128());
	vOutB.AsM128() = __vmaddfp(vInB.AsM128(), temp, vOutB.AsM128());
#endif
}

inline void Vector4DWeightMADSSE(float w, Vector4DAligned const& vInA, Vector4DAligned& vOutA, Vector4DAligned const& vInB, Vector4DAligned& vOutB)
{

#if !defined( _X360 )
	// Replicate scalar float out to 4 components
	__m128 packed = _mm_set1_ps(w);

	// 4D SSE Vector MAD
	vOutA.AsM128() = _mm_add_ps(vOutA.AsM128(), _mm_mul_ps(vInA.AsM128(), packed));
	vOutB.AsM128() = _mm_add_ps(vOutB.AsM128(), _mm_mul_ps(vInB.AsM128(), packed));
#else
	__vector4 temp;

	temp = __lvlx(&w, 0);
	temp = __vspltw(temp, 0);

	vOutA.AsM128() = __vmaddfp(vInA.AsM128(), temp, vOutA.AsM128());
	vOutB.AsM128() = __vmaddfp(vInB.AsM128(), temp, vOutB.AsM128());
#endif
}


