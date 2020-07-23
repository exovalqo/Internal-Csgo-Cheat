#pragma once

#include "Vectors.h"
#define VEC_T_NAN FLOAT32_NAN

class QAngleByValue;
class VectorByValue;
class TableVector;
struct cplane_t;
// plane_t structure
// !!! if this is changed, it must be changed in asm code too !!!
// FIXME: does the asm code even exist anymore?
// FIXME: this should move to a different file
class QAngle
{
public:
	// Members
	float x, y, z;

	// Construction/destruction
	QAngle(void);
	QAngle(float X, float Y, float Z);
	//	QAngle(RadianEuler const &angles);	// evil auto type promotion!!!

		// Allow pass-by-value
	operator QAngleByValue& () { return *((QAngleByValue*)(this)); }
	operator const QAngleByValue& () const { return *((const QAngleByValue*)(this)); }

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	void Random(float minVal, float maxVal);

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// equality
	bool operator==(const QAngle& v) const;
	bool operator!=(const QAngle& v) const;

	// arithmetic operations
	QAngle& operator+=(const QAngle& v);
	QAngle& operator-=(const QAngle& v);
	QAngle& operator*=(float s);
	QAngle& operator/=(float s);

	// Get the Vector3's magnitude.
	float	Length() const;
	float	LengthSqr() const;

	// negate the QAngle components
	//void	Negate(); 

	// No assignment operators either...
	QAngle& operator=(const QAngle& src);

#ifndef Vector3_NO_SLOW_OPERATIONS
	// copy constructors

	// arithmetic operations
	QAngle	operator-(void) const;

	QAngle	operator+(const QAngle& v) const;
	QAngle	operator-(const QAngle& v) const;
	QAngle	operator*(float fl) const;
	QAngle	operator/(float fl) const;
#else

private:
	// No copy constructors allowed if we're in optimal mode
	QAngle(const QAngle& vOther);

#endif
};

FORCEINLINE void NetworkVarConstruct(QAngle& q) { q.x = q.y = q.z = 0.0f; }

//-----------------------------------------------------------------------------
// Allows us to specifically pass the Vector3 by value when we need to
//-----------------------------------------------------------------------------
class QAngleByValue : public QAngle
{
public:
	// Construction/destruction:
	QAngleByValue(void) : QAngle() {}
	QAngleByValue(float X, float Y, float Z) : QAngle(X, Y, Z) {}
	QAngleByValue(const QAngleByValue& vOther) { *this = vOther; }
};


inline void Vector3Add(const QAngle& a, const QAngle& b, QAngle& result)
{
	result.x = a.x + b.x;
	result.y = a.y + b.y;
	result.z = a.z + b.z;
}

inline void Vector3MA(const QAngle& start, float scale, const QAngle& direction, QAngle& dest)
{
	dest.x = start.x + scale * direction.x;
	dest.y = start.y + scale * direction.y;
	dest.z = start.z + scale * direction.z;
}


//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------
inline QAngle::QAngle(void)
{
#ifdef _DEBUG
	// Initialize to NAN to catch errors
	x = y = z = VEC_T_NAN;
#endif
}

inline QAngle::QAngle(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
}


//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------
inline void QAngle::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
}

inline void QAngle::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	z = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
}

inline QAngle RandomAngle(float minVal, float maxVal)
{
	Vector3 random;
	random.Random(minVal, maxVal);
	QAngle ret(random.x, random.y, random.z);
	return ret;
}




//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------
inline QAngle& QAngle::operator=(const QAngle& vOther)
{
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}


//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
inline float& QAngle::operator[](int i)
{
	return ((float*)this)[i];
}

inline float QAngle::operator[](int i) const
{
	return ((float*)this)[i];
}


//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* QAngle::Base()
{
	return (float*)this;
}

inline float const* QAngle::Base() const
{
	return (float const*)this;
}


//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------
inline bool QAngle::IsValid() const
{
	return IsFinite(x) && IsFinite(y) && IsFinite(z);
}

//-----------------------------------------------------------------------------
// Invalidate
//-----------------------------------------------------------------------------

inline void QAngle::Invalidate()
{
	//#ifdef _DEBUG
	//#ifdef Vector3_PARANOIA
	x = y = z = VEC_T_NAN;
	//#endif
	//#endif
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------
inline bool QAngle::operator==(const QAngle& src) const
{
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool QAngle::operator!=(const QAngle& src) const
{
	return (src.x != x) || (src.y != y) || (src.z != z);
}

//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------
inline void VectorCopy(const QAngle& src, QAngle& dst)
{
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}


//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------
inline QAngle& QAngle::operator+=(const QAngle& v)
{
	x += v.x; y += v.y; z += v.z;
	return *this;
}

inline QAngle& QAngle::operator-=(const QAngle& v)
{
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

inline QAngle& QAngle::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	return *this;
}

inline QAngle& QAngle::operator/=(float fl)
{
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	return *this;
}


//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline float QAngle::Length() const
{
	return (float)FastSqrt(LengthSqr());
}


inline float QAngle::LengthSqr() const
{
	return x * x + y * y + z * z;
}



inline QAngle QAngle::operator+(const QAngle& v) const
{
	QAngle res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

inline QAngle QAngle::operator-(const QAngle& v) const
{
	QAngle res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline QAngle QAngle::operator*(float fl) const
{
	QAngle res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline QAngle QAngle::operator/(float fl) const
{
	QAngle res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline QAngle operator*(float fl, const QAngle& v)
{
	QAngle ret(v * fl);
	return ret;
}


class TableVector
{
public:
	float x, y, z;

	operator Vector3& () { return *((Vector3*)(this)); }
	operator const Vector3& () const { return *((const Vector3*)(this)); }

	// array access...
	inline float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	inline float operator[](int i) const
	{
		return ((float*)this)[i];
	}
};

typedef int SideType;

// Used to represent sides of things like planes.
#define	SIDE_FRONT	0
#define	SIDE_BACK	1
#define	SIDE_ON		2

#define VP_EPSILON	0.01f


class VPlane
{
public:
	VPlane();
	VPlane(const Vector3& vNormal, float dist);

	void		Init(const Vector3& vNormal, float dist);

	// Return the distance from the point to the plane.
	float		DistTo(const Vector3& vVec) const;

	// Copy.
	VPlane& operator=(const VPlane& thePlane);

	// Returns SIDE_ON, SIDE_FRONT, or SIDE_BACK.
	// The epsilon for SIDE_ON can be passed in.
	SideType	GetPointSide(const Vector3& vPoint, float sideEpsilon = VP_EPSILON) const;

	// Returns SIDE_FRONT or SIDE_BACK.
	SideType	GetPointSideExact(const Vector3& vPoint) const;

	// Classify the box with respect to the plane.
	// Returns SIDE_ON, SIDE_FRONT, or SIDE_BACK
	SideType	BoxOnPlaneSide(const Vector3& vMin, const Vector3& vMax) const;

#ifndef VECTOR_NO_SLOW_OPERATIONS
	// Flip the plane.
	VPlane		Flip();

	// Get a point on the plane (normal*dist).
	Vector3		GetPointOnPlane() const;

	// Snap the specified point to the plane (along the plane's normal).
	Vector3		SnapPointToPlane(const Vector3& vPoint) const;
#endif

public:
	Vector3		m_Normal;
	float		m_Dist;

#ifdef VECTOR_NO_SLOW_OPERATIONS
private:
	// No copy constructors allowed if we're in optimal mode
	VPlane(const VPlane& vOther);
#endif
};


//-----------------------------------------------------------------------------
// Inlines.
//-----------------------------------------------------------------------------
inline VPlane::VPlane()
{
}

inline VPlane::VPlane(const Vector3& vNormal, float dist)
{
	m_Normal = vNormal;
	m_Dist = dist;
}

inline void	VPlane::Init(const Vector3& vNormal, float dist)
{
	m_Normal = vNormal;
	m_Dist = dist;
}

inline float VPlane::DistTo(const Vector3& vVec) const
{
	return vVec.Dot(m_Normal) - m_Dist;
}

inline VPlane& VPlane::operator=(const VPlane& thePlane)
{
	m_Normal = thePlane.m_Normal;
	m_Dist = thePlane.m_Dist;
	return *this;
}

#ifndef VECTOR_NO_SLOW_OPERATIONS

inline VPlane VPlane::Flip()
{
	return VPlane(-m_Normal, -m_Dist);
}

inline Vector3 VPlane::GetPointOnPlane() const
{
	return m_Normal * m_Dist;
}

inline Vector3 VPlane::SnapPointToPlane(const Vector3& vPoint) const
{
	return vPoint - m_Normal * DistTo(vPoint);
}

#endif

inline SideType VPlane::GetPointSide(const Vector3& vPoint, float sideEpsilon) const
{
	float fDist;

	fDist = DistTo(vPoint);
	if (fDist >= sideEpsilon)
		return SIDE_FRONT;
	else if (fDist <= -sideEpsilon)
		return SIDE_BACK;
	else
		return SIDE_ON;
}

inline SideType VPlane::GetPointSideExact(const Vector3& vPoint) const
{
	return DistTo(vPoint) > 0.0f ? SIDE_FRONT : SIDE_BACK;
}


// BUGBUG: This should either simply use the implementation in mathlib or cease to exist.
// mathlib implementation is much more efficient.  Check to see that VPlane isn't used in
// performance critical code.
inline SideType VPlane::BoxOnPlaneSide(const Vector3& vMin, const Vector3& vMax) const
{
	int i, firstSide, side;
	TableVector vPoints[8] =
	{
		{ vMin.x, vMin.y, vMin.z },
		{ vMin.x, vMin.y, vMax.z },
		{ vMin.x, vMax.y, vMax.z },
		{ vMin.x, vMax.y, vMin.z },

		{ vMax.x, vMin.y, vMin.z },
		{ vMax.x, vMin.y, vMax.z },
		{ vMax.x, vMax.y, vMax.z },
		{ vMax.x, vMax.y, vMin.z },
	};

	firstSide = GetPointSideExact(vPoints[0]);
	for (i = 1; i < 8; i++)
	{
		side = GetPointSideExact(vPoints[i]);

		// Does the box cross the plane?
		if (side != firstSide)
			return SIDE_ON;
	}

	// Ok, they're all on the same side, return that.
	return firstSide;
}



class VMatrix
{
public:

	VMatrix();
	VMatrix(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
		);

	// Creates a matrix where the X axis = forward
	// the Y axis = left, and the Z axis = up
	VMatrix(const Vector3& forward, const Vector3& left, const Vector3& up);
	VMatrix(const Vector3& forward, const Vector3& left, const Vector3& up, const Vector3& translation);

	// Construct from a 3x4 matrix
	VMatrix(const matrix3x4_t& matrix3x4);

	// Set the values in the matrix.
	void		Init(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33
		);


	// Initialize from a 3x4
	void		Init(const matrix3x4_t& matrix3x4);

	// array access
	inline float* operator[](int i)
	{
		return m[i];
	}

	inline const float* operator[](int i) const
	{
		return m[i];
	}

	// Get a pointer to m[0][0]
	inline float* Base()
	{
		return &m[0][0];
	}

	inline const float* Base() const
	{
		return &m[0][0];
	}

	void		SetLeft(const Vector3& vLeft);
	void		SetUp(const Vector3& vUp);
	void		SetForward(const Vector3& vForward);

	void		GetBasisVector3s(Vector3& vForward, Vector3& vLeft, Vector3& vUp) const;
	void		SetBasisVector3s(const Vector3& vForward, const Vector3& vLeft, const Vector3& vUp);

	// Get/set the translation.
	Vector3& GetTranslation(Vector3& vTrans) const;
	void		SetTranslation(const Vector3& vTrans);

	void		PreTranslate(const Vector3& vTrans);
	void		PostTranslate(const Vector3& vTrans);

	const matrix3x4_t& As3x4() const;
	void		CopyFrom3x4(const matrix3x4_t& m3x4);
	void		Set3x4(matrix3x4_t& matrix3x4) const;

	bool		operator==(const VMatrix& src) const;
	bool		operator!=(const VMatrix& src) const { return !(*this == src); }

#ifndef Vector3_NO_SLOW_OPERATIONS
	// Access the basis Vector3s.
	Vector3		GetLeft() const;
	Vector3		GetUp() const;
	Vector3		GetForward() const;
	Vector3		GetTranslation() const;
#endif


	// Matrix->Vector3 operations.
public:
	// Multiply by a 3D Vector3 (same as operator*).
	void		V3Mul(const Vector3& vIn, Vector3& vOut) const;

	// Multiply by a 4D Vector3.
	void		V4Mul(const Vector4D& vIn, Vector4D& vOut) const;

#ifndef Vector3_NO_SLOW_OPERATIONS
	// Applies the rotation (ignores translation in the matrix). (This just calls VMul3x3).
	Vector3		ApplyRotation(const Vector3& vVec) const;

	// Multiply by a Vector3 (divides by w, assumes input w is 1).
	Vector3		operator*(const Vector3& vVec) const;

	// Multiply by the upper 3x3 part of the matrix (ie: only apply rotation).
	Vector3		VMul3x3(const Vector3& vVec) const;

	// Apply the inverse (transposed) rotation (only works on pure rotation matrix)
	Vector3		VMul3x3Transpose(const Vector3& vVec) const;

	// Multiply by the upper 3 rows.
	Vector3		VMul4x3(const Vector3& vVec) const;

	// Apply the inverse (transposed) transformation (only works on pure rotation/translation)
	Vector3		VMul4x3Transpose(const Vector3& vVec) const;
#endif


	// Matrix->plane operations.
public:
	// Transform the plane. The matrix can only contain translation and rotation.
	void		TransformPlane(const VPlane& inPlane, VPlane& outPlane) const;

#ifndef Vector3_NO_SLOW_OPERATIONS
	// Just calls TransformPlane and returns the result.
	VPlane		operator*(const VPlane& thePlane) const;
#endif

	// Matrix->matrix operations.
public:

	VMatrix& operator=(const VMatrix& mOther);

	// Multiply two matrices (out = this * vm).
	void		MatrixMul(const VMatrix& vm, VMatrix& out) const;

	// Add two matrices.
	const VMatrix& operator+=(const VMatrix& other);

#ifndef Vector3_NO_SLOW_OPERATIONS
	// Just calls MatrixMul and returns the result.	
	VMatrix		operator*(const VMatrix& mOther) const;

	// Add/Subtract two matrices.
	VMatrix		operator+(const VMatrix& other) const;
	VMatrix		operator-(const VMatrix& other) const;

	// Negation.
	VMatrix		operator-() const;

	// Return inverse matrix. Be careful because the results are undefined 
	// if the matrix doesn't have an inverse (ie: InverseGeneral returns false).
	VMatrix		operator~() const;
#endif

	// Matrix operations.
public:
	// Set to identity.
	void		Identity();

	bool		IsIdentity() const;

	// Setup a matrix for origin and angles.
	void		SetupMatrixOrgAngles(const Vector3& origin, const QAngle& vAngles);

	// Setup a matrix for angles and no translation.
	void		SetupMatrixAngles(const QAngle& vAngles);

	// General inverse. This may fail so check the return!
	bool		InverseGeneral(VMatrix& vInverse) const;

	// Does a fast inverse, assuming the matrix only contains translation and rotation.
	void		InverseTR(VMatrix& mRet) const;

	// Usually used for debug checks. Returns true if the upper 3x3 contains
	// unit Vector3s and they are all orthogonal.
	bool		IsRotationMatrix() const;

#ifndef Vector3_NO_SLOW_OPERATIONS
	// This calls the other InverseTR and returns the result.
	VMatrix		InverseTR() const;

	// Get the scale of the matrix's basis Vector3s.
	Vector3		GetScale() const;

	// (Fast) multiply by a scaling matrix setup from vScale.
	VMatrix		Scale(const Vector3& vScale);

	// Normalize the basis Vector3s.
	VMatrix		NormalizeBasisVector3s() const;

	// Transpose.
	VMatrix		Transpose() const;

	// Transpose upper-left 3x3.
	VMatrix		Transpose3x3() const;
#endif

public:
	// The matrix.
	float		m[4][4];
};



//-----------------------------------------------------------------------------
// Helper functions.
//-----------------------------------------------------------------------------

#ifndef Vector3_NO_SLOW_OPERATIONS

// Setup an identity matrix.
VMatrix		SetupMatrixIdentity();

// Setup as a scaling matrix.
VMatrix		SetupMatrixScale(const Vector3& vScale);

// Setup a translation matrix.
VMatrix		SetupMatrixTranslation(const Vector3& vTranslation);

// Setup a matrix to reflect around the plane.
VMatrix		SetupMatrixReflection(const VPlane& thePlane);

// Setup a matrix to project from vOrigin onto thePlane.
VMatrix		SetupMatrixProjection(const Vector3& vOrigin, const VPlane& thePlane);

// Setup a matrix to rotate the specified amount around the specified axis.
VMatrix		SetupMatrixAxisRot(const Vector3& vAxis, float fDegrees);

// Setup a matrix from euler angles. Just sets identity and calls MatrixAngles.
VMatrix		SetupMatrixAngles(const QAngle& vAngles);

// Setup a matrix for origin and angles.
VMatrix		SetupMatrixOrgAngles(const Vector3& origin, const QAngle& vAngles);

#endif

#define VMatToString(mat)	(static_cast<const char *>(CFmtStr("[ (%f, %f, %f), (%f, %f, %f), (%f, %f, %f), (%f, %f, %f) ]", mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3], mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3], mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3], mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3] ))) // ** Note: this generates a temporary, don't hold reference!

//-----------------------------------------------------------------------------
// Returns the point at the intersection on the 3 planes.
// Returns false if it can't be solved (2 or more planes are parallel).
//-----------------------------------------------------------------------------
bool PlaneIntersection(const VPlane& vp1, const VPlane& vp2, const VPlane& vp3, Vector3& vOut);


//-----------------------------------------------------------------------------
// These methods are faster. Use them if you want faster code
//-----------------------------------------------------------------------------
void MatrixSetIdentity(VMatrix& dst);
void MatrixTranspose(const VMatrix& src, VMatrix& dst);
void MatrixCopy(const VMatrix& src, VMatrix& dst);
void MatrixMultiply(const VMatrix& src1, const VMatrix& src2, VMatrix& dst);

// Accessors
void MatrixGetColumn(const VMatrix& src, int nCol, Vector3* pColumn);
void MatrixSetColumn(VMatrix& src, int nCol, const Vector3& column);
void MatrixGetRow(const VMatrix& src, int nCol, Vector3* pColumn);
void MatrixSetRow(VMatrix& src, int nCol, const Vector3& column);

// Vector3DMultiply treats src2 as if it's a direction Vector3
void Vector3DMultiply(const VMatrix& src1, const Vector3& src2, Vector3& dst);

// Vector3DMultiplyPosition treats src2 as if it's a point (adds the translation)
inline void Vector3DMultiplyPosition(const VMatrix& src1, const VectorByValue src2, Vector3& dst);

// Vector3DMultiplyPositionProjective treats src2 as if it's a point 
// and does the perspective divide at the end
void Vector3DMultiplyPositionProjective(const VMatrix& src1, const Vector3& src2, Vector3& dst);

// Vector3DMultiplyPosition treats src2 as if it's a direction 
// and does the perspective divide at the end
// NOTE: src1 had better be an inverse transpose to use this correctly
void Vector3DMultiplyProjective(const VMatrix& src1, const Vector3& src2, Vector3& dst);

void Vecto4DMultiply(const VMatrix& src1, const Vector4D& src2, Vector4D& dst);

// Same as Vecto4DMultiply except that src2 has an implicit W of 1
void Vecto4DMultiplyPosition(const VMatrix& src1, const Vector3& src2, Vector4D& dst);

// Multiplies the Vector3 by the transpose of the matrix
void Vector3DMultiplyTranspose(const VMatrix& src1, const Vector3& src2, Vector3& dst);
void Vecto4DMultiplyTranspose(const VMatrix& src1, const Vector4D& src2, Vector4D& dst);


// Transform a plane that has an axis-aligned normal
void MatrixTransformAxisAlignedPlane(const VMatrix& src, int nDim, float flSign, float flDist, cplane_t& outPlane);

void MatrixBuildTranslation(VMatrix& dst, float x, float y, float z);
void MatrixBuildTranslation(VMatrix& dst, const Vector3& translation);

inline void MatrixTranslate(VMatrix& dst, const Vector3& translation)
{
	VMatrix matTranslation, temp;
	MatrixBuildTranslation(matTranslation, translation);
	MatrixMultiply(dst, matTranslation, temp);
	dst = temp;
}


void MatrixBuildRotationAboutAxis(VMatrix& dst, const Vector3& vAxisOfRot, float angleDegrees);
void MatrixBuildRotateZ(VMatrix& dst, float angleDegrees);

inline void MatrixRotate(VMatrix& dst, const Vector3& vAxisOfRot, float angleDegrees)
{
	VMatrix rotation, temp;
	MatrixBuildRotationAboutAxis(rotation, vAxisOfRot, angleDegrees);
	MatrixMultiply(dst, rotation, temp);
	dst = temp;
}

// Builds a rotation matrix that rotates one direction Vector3 into another
void MatrixBuildRotation(VMatrix& dst, const Vector3& initialDirection, const Vector3& finalDirection);

// Builds a scale matrix
void MatrixBuildScale(VMatrix& dst, float x, float y, float z);
void MatrixBuildScale(VMatrix& dst, const Vector3& scale);

// Build a perspective matrix.
// zNear and zFar are assumed to be positive.
// You end up looking down positive Z, X is to the right, Y is up.
// X range: [0..1]
// Y range: [0..1]
// Z range: [0..1]
void MatrixBuildPerspective(VMatrix& dst, float fovX, float fovY, float zNear, float zFar);

//-----------------------------------------------------------------------------
// Given a projection matrix, take the extremes of the space in transformed into world space and
// get a bounding box.
//-----------------------------------------------------------------------------
void CalculateAABBFromProjectionMatrix(const VMatrix& worldToVolume, Vector3* pMins, Vector3* pMaxs);

//-----------------------------------------------------------------------------
// Given a projection matrix, take the extremes of the space in transformed into world space and
// get a bounding sphere.
//-----------------------------------------------------------------------------
void CalculateSphereFromProjectionMatrix(const VMatrix& worldToVolume, Vector3* pCenter, float* pflRadius);

//-----------------------------------------------------------------------------
// Given an inverse projection matrix, take the extremes of the space in transformed into world space and
// get a bounding box.
//-----------------------------------------------------------------------------
void CalculateAABBFromProjectionMatrixInverse(const VMatrix& volumeToWorld, Vector3* pMins, Vector3* pMaxs);

//-----------------------------------------------------------------------------
// Given an inverse projection matrix, take the extremes of the space in transformed into world space and
// get a bounding sphere.
//-----------------------------------------------------------------------------
void CalculateSphereFromProjectionMatrixInverse(const VMatrix& volumeToWorld, Vector3* pCenter, float* pflRadius);

//-----------------------------------------------------------------------------
// Calculate frustum planes given a clip->world space transform.
//-----------------------------------------------------------------------------
//void FrustumPlanesFromMatrix(const VMatrix& clipToWorld, Frustum_t& frustum);

//-----------------------------------------------------------------------------
// Setup a matrix from euler angles. 
//-----------------------------------------------------------------------------
void MatrixFromAngles(const QAngle& vAngles, VMatrix& dst);

//-----------------------------------------------------------------------------
// Creates euler angles from a matrix 
//-----------------------------------------------------------------------------
void MatrixToAngles(const VMatrix& src, QAngle& vAngles);

//-----------------------------------------------------------------------------
// Does a fast inverse, assuming the matrix only contains translation and rotation.
//-----------------------------------------------------------------------------
void MatrixInverseTR(const VMatrix& src, VMatrix& dst);

//-----------------------------------------------------------------------------
// Inverts any matrix at all
//-----------------------------------------------------------------------------
bool MatrixInverseGeneral(const VMatrix& src, VMatrix& dst);

//-----------------------------------------------------------------------------
// Computes the inverse transpose
//-----------------------------------------------------------------------------
void MatrixInverseTranspose(const VMatrix& src, VMatrix& dst);



//-----------------------------------------------------------------------------
// VMatrix inlines.
//-----------------------------------------------------------------------------
inline VMatrix::VMatrix()
{
}

inline VMatrix::VMatrix(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	Init(
		m00, m01, m02, m03,
		m10, m11, m12, m13,
		m20, m21, m22, m23,
		m30, m31, m32, m33
		);
}


inline VMatrix::VMatrix(const matrix3x4_t& matrix3x4)
{
	Init(matrix3x4);
}


//-----------------------------------------------------------------------------
// Creates a matrix where the X axis = forward
// the Y axis = left, and the Z axis = up
//-----------------------------------------------------------------------------
inline VMatrix::VMatrix(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis)
{
	Init(
		xAxis.x, yAxis.x, zAxis.x, 0.0f,
		xAxis.y, yAxis.y, zAxis.y, 0.0f,
		xAxis.z, yAxis.z, zAxis.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}

inline VMatrix::VMatrix(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis, const Vector3& translation)
{
	Init(
		xAxis.x, yAxis.x, zAxis.x, translation.x,
		xAxis.y, yAxis.y, zAxis.y, translation.y,
		xAxis.z, yAxis.z, zAxis.z, translation.z,
		0.0f, 0.0f, 0.0f, 1.0f
		);
}


inline void VMatrix::Init(
	float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33
	)
{
	m[0][0] = m00;
	m[0][1] = m01;
	m[0][2] = m02;
	m[0][3] = m03;

	m[1][0] = m10;
	m[1][1] = m11;
	m[1][2] = m12;
	m[1][3] = m13;

	m[2][0] = m20;
	m[2][1] = m21;
	m[2][2] = m22;
	m[2][3] = m23;

	m[3][0] = m30;
	m[3][1] = m31;
	m[3][2] = m32;
	m[3][3] = m33;
}


//-----------------------------------------------------------------------------
// Initialize from a 3x4
//-----------------------------------------------------------------------------
inline void VMatrix::Init(const matrix3x4_t& matrix3x4)
{
	memcpy(m, matrix3x4.Base(), sizeof(matrix3x4_t));

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}


//-----------------------------------------------------------------------------
// Methods related to the basis Vector3s of the matrix
//-----------------------------------------------------------------------------

#ifndef Vector3_NO_SLOW_OPERATIONS

inline Vector3 VMatrix::GetForward() const
{
	return Vector3(m[0][0], m[1][0], m[2][0]);
}

inline Vector3 VMatrix::GetLeft() const
{
	return Vector3(m[0][1], m[1][1], m[2][1]);
}

inline Vector3 VMatrix::GetUp() const
{
	return Vector3(m[0][2], m[1][2], m[2][2]);
}

#endif

inline void VMatrix::SetForward(const Vector3& vForward)
{
	m[0][0] = vForward.x;
	m[1][0] = vForward.y;
	m[2][0] = vForward.z;
}

inline void VMatrix::SetLeft(const Vector3& vLeft)
{
	m[0][1] = vLeft.x;
	m[1][1] = vLeft.y;
	m[2][1] = vLeft.z;
}

inline void VMatrix::SetUp(const Vector3& vUp)
{
	m[0][2] = vUp.x;
	m[1][2] = vUp.y;
	m[2][2] = vUp.z;
}

inline void VMatrix::GetBasisVector3s(Vector3& vForward, Vector3& vLeft, Vector3& vUp) const
{
	vForward.Init(m[0][0], m[1][0], m[2][0]);
	vLeft.Init(m[0][1], m[1][1], m[2][1]);
	vUp.Init(m[0][2], m[1][2], m[2][2]);
}

inline void VMatrix::SetBasisVector3s(const Vector3& vForward, const Vector3& vLeft, const Vector3& vUp)
{
	SetForward(vForward);
	SetLeft(vLeft);
	SetUp(vUp);
}


//-----------------------------------------------------------------------------
// Methods related to the translation component of the matrix
//-----------------------------------------------------------------------------
#ifndef Vector3_NO_SLOW_OPERATIONS

inline Vector3 VMatrix::GetTranslation() const
{
	return Vector3(m[0][3], m[1][3], m[2][3]);
}

#endif

inline Vector3& VMatrix::GetTranslation(Vector3& vTrans) const
{
	vTrans.x = m[0][3];
	vTrans.y = m[1][3];
	vTrans.z = m[2][3];
	return vTrans;
}

inline void VMatrix::SetTranslation(const Vector3& vTrans)
{
	m[0][3] = vTrans.x;
	m[1][3] = vTrans.y;
	m[2][3] = vTrans.z;
}


//-----------------------------------------------------------------------------
// appply translation to this matrix in the input space
//-----------------------------------------------------------------------------
inline void VMatrix::PreTranslate(const Vector3& vTrans)
{
	Vector3 tmp;
	Vector3DMultiplyPosition(*this, vTrans, tmp);
	m[0][3] = tmp.x;
	m[1][3] = tmp.y;
	m[2][3] = tmp.z;
}


//-----------------------------------------------------------------------------
// appply translation to this matrix in the output space
//-----------------------------------------------------------------------------
inline void VMatrix::PostTranslate(const Vector3& vTrans)
{
	m[0][3] += vTrans.x;
	m[1][3] += vTrans.y;
	m[2][3] += vTrans.z;
}

inline const matrix3x4_t& VMatrix::As3x4() const
{
	return *((const matrix3x4_t*)this);
}

inline void VMatrix::CopyFrom3x4(const matrix3x4_t& m3x4)
{
	memcpy(m, m3x4.Base(), sizeof(matrix3x4_t));
	m[3][0] = m[3][1] = m[3][2] = 0;
	m[3][3] = 1;
}

inline void	VMatrix::Set3x4(matrix3x4_t& matrix3x4) const
{
	memcpy(matrix3x4.Base(), m, sizeof(matrix3x4_t));
}


//-----------------------------------------------------------------------------
// Matrix math operations
//-----------------------------------------------------------------------------
inline const VMatrix& VMatrix::operator+=(const VMatrix& other)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m[i][j] += other.m[i][j];
		}
	}

	return *this;
}


#ifndef Vector3_NO_SLOW_OPERATIONS

inline VMatrix VMatrix::operator+(const VMatrix& other) const
{
	VMatrix ret;
	for (int i = 0; i < 16; i++)
	{
		((float*)ret.m)[i] = ((float*)m)[i] + ((float*)other.m)[i];
	}
	return ret;
}

inline VMatrix VMatrix::operator-(const VMatrix& other) const
{
	VMatrix ret;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			ret.m[i][j] = m[i][j] - other.m[i][j];
		}
	}

	return ret;
}

inline VMatrix VMatrix::operator-() const
{
	VMatrix ret;
	for (int i = 0; i < 16; i++)
	{
		((float*)ret.m)[i] = ((float*)m)[i];
	}
	return ret;
}

#endif // Vector3_NO_SLOW_OPERATIONS


//-----------------------------------------------------------------------------
// Vector3 transformation
//-----------------------------------------------------------------------------

#ifndef Vector3_NO_SLOW_OPERATIONS

inline Vector3 VMatrix::operator*(const Vector3& vVec) const
{
	Vector3 vRet;
	vRet.x = m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z + m[0][3];
	vRet.y = m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z + m[1][3];
	vRet.z = m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z + m[2][3];

	return vRet;
}

inline Vector3 VMatrix::VMul4x3(const Vector3& vVec) const
{
	Vector3 vResult;
	Vector3DMultiplyPosition(*this, vVec, vResult);
	return vResult;
}


inline Vector3 VMatrix::VMul4x3Transpose(const Vector3& vVec) const
{
	Vector3 tmp = vVec;
	tmp.x -= m[0][3];
	tmp.y -= m[1][3];
	tmp.z -= m[2][3];

	return Vector3(
		m[0][0] * tmp.x + m[1][0] * tmp.y + m[2][0] * tmp.z,
		m[0][1] * tmp.x + m[1][1] * tmp.y + m[2][1] * tmp.z,
		m[0][2] * tmp.x + m[1][2] * tmp.y + m[2][2] * tmp.z
		);
}

inline Vector3 VMatrix::VMul3x3(const Vector3& vVec) const
{
	return Vector3(
		m[0][0] * vVec.x + m[0][1] * vVec.y + m[0][2] * vVec.z,
		m[1][0] * vVec.x + m[1][1] * vVec.y + m[1][2] * vVec.z,
		m[2][0] * vVec.x + m[2][1] * vVec.y + m[2][2] * vVec.z
		);
}

inline Vector3 VMatrix::VMul3x3Transpose(const Vector3& vVec) const
{
	return Vector3(
		m[0][0] * vVec.x + m[1][0] * vVec.y + m[2][0] * vVec.z,
		m[0][1] * vVec.x + m[1][1] * vVec.y + m[2][1] * vVec.z,
		m[0][2] * vVec.x + m[1][2] * vVec.y + m[2][2] * vVec.z
		);
}

#endif // Vector3_NO_SLOW_OPERATIONS


inline void VMatrix::V3Mul(const Vector3& vIn, Vector3& vOut) const
{
	float rw;

	rw = 1.0f / (m[3][0] * vIn.x + m[3][1] * vIn.y + m[3][2] * vIn.z + m[3][3]);
	vOut.x = (m[0][0] * vIn.x + m[0][1] * vIn.y + m[0][2] * vIn.z + m[0][3]) * rw;
	vOut.y = (m[1][0] * vIn.x + m[1][1] * vIn.y + m[1][2] * vIn.z + m[1][3]) * rw;
	vOut.z = (m[2][0] * vIn.x + m[2][1] * vIn.y + m[2][2] * vIn.z + m[2][3]) * rw;
}

inline void VMatrix::V4Mul(const Vector4D& vIn, Vector4D& vOut) const
{
	vOut[0] = m[0][0] * vIn[0] + m[0][1] * vIn[1] + m[0][2] * vIn[2] + m[0][3] * vIn[3];
	vOut[1] = m[1][0] * vIn[0] + m[1][1] * vIn[1] + m[1][2] * vIn[2] + m[1][3] * vIn[3];
	vOut[2] = m[2][0] * vIn[0] + m[2][1] * vIn[1] + m[2][2] * vIn[2] + m[2][3] * vIn[3];
	vOut[3] = m[3][0] * vIn[0] + m[3][1] * vIn[1] + m[3][2] * vIn[2] + m[3][3] * vIn[3];
}


//-----------------------------------------------------------------------------
// Plane transformation
//-----------------------------------------------------------------------------
inline void	VMatrix::TransformPlane(const VPlane& inPlane, VPlane& outPlane) const
{
	Vector3 vTrans;
	Vector3DMultiply(*this, inPlane.m_Normal, outPlane.m_Normal);
	outPlane.m_Dist = inPlane.m_Dist * DotProduct(outPlane.m_Normal, outPlane.m_Normal);
	outPlane.m_Dist += DotProduct(outPlane.m_Normal, GetTranslation(vTrans));
}


//-----------------------------------------------------------------------------
// Other random stuff
//-----------------------------------------------------------------------------
inline void VMatrix::Identity()
{
	MatrixSetIdentity(*this);
}


inline bool VMatrix::IsIdentity() const
{
	return
		m[0][0] == 1.0f && m[0][1] == 0.0f && m[0][2] == 0.0f && m[0][3] == 0.0f &&
		m[1][0] == 0.0f && m[1][1] == 1.0f && m[1][2] == 0.0f && m[1][3] == 0.0f &&
		m[2][0] == 0.0f && m[2][1] == 0.0f && m[2][2] == 1.0f && m[2][3] == 0.0f &&
		m[3][0] == 0.0f && m[3][1] == 0.0f && m[3][2] == 0.0f && m[3][3] == 1.0f;
}

#ifndef Vector3_NO_SLOW_OPERATIONS

inline Vector3 VMatrix::ApplyRotation(const Vector3& vVec) const
{
	return VMul3x3(vVec);
}

inline VMatrix VMatrix::operator~() const
{
	VMatrix mRet;
	InverseGeneral(mRet);
	return mRet;
}

#endif


//-----------------------------------------------------------------------------
// Accessors
//-----------------------------------------------------------------------------
inline void MatrixGetColumn(const VMatrix& src, int nCol, Vector3* pColumn)
{

	pColumn->x = src[0][nCol];
	pColumn->y = src[1][nCol];
	pColumn->z = src[2][nCol];
}

inline void MatrixSetColumn(VMatrix& src, int nCol, const Vector3& column)
{

	src.m[0][nCol] = column.x;
	src.m[1][nCol] = column.y;
	src.m[2][nCol] = column.z;
}

inline void MatrixGetRow(const VMatrix& src, int nRow, Vector3* pRow)
{
	*pRow = *(Vector3*)src[nRow];
}

inline void MatrixSetRow(VMatrix& dst, int nRow, const Vector3& row)
{
	*(Vector3*)dst[nRow] = row;
}


//-----------------------------------------------------------------------------
// Vector3DMultiplyPosition treats src2 as if it's a point (adds the translation)
//-----------------------------------------------------------------------------
// NJS: src2 is passed in as a full Vector3 rather than a reference to prevent the need
// for 2 branches and a potential copy in the body.  (ie, handling the case when the src2
// reference is the same as the dst reference ).
inline void Vector3DMultiplyPosition(const VMatrix& src1, const VectorByValue src2, Vector3& dst)
{
	dst[0] = src1[0][0] * src2.x + src1[0][1] * src2.y + src1[0][2] * src2.z + src1[0][3];
	dst[1] = src1[1][0] * src2.x + src1[1][1] * src2.y + src1[1][2] * src2.z + src1[1][3];
	dst[2] = src1[2][0] * src2.x + src1[2][1] * src2.y + src1[2][2] * src2.z + src1[2][3];
}




//-----------------------------------------------------------------------------
// Matrix equality test
//-----------------------------------------------------------------------------
inline bool MatricesAreEqual(const VMatrix& src1, const VMatrix& src2, float flTolerance)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (fabs(src1[i][j] - src2[i][j]) > flTolerance)
				return false;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void MatrixBuildOrtho(VMatrix& dst, double left, double top, double right, double bottom, double zNear, double zFar);
void MatrixBuildPerspectiveX(VMatrix& dst, double flFovX, double flAspect, double flZNear, double flZFar);
void MatrixBuildPerspectiveOffCenterX(VMatrix& dst, double flFovX, double flAspect, double flZNear, double flZFar, double bottom, double top, double left, double right);
void MatrixBuildPerspectiveZRange(VMatrix& dst, double flZNear, double flZFar);

inline void MatrixOrtho(VMatrix& dst, double left, double top, double right, double bottom, double zNear, double zFar)
{
	VMatrix mat;
	MatrixBuildOrtho(mat, left, top, right, bottom, zNear, zFar);

	VMatrix temp;
	MatrixMultiply(dst, mat, temp);
	dst = temp;
}

inline void MatrixPerspectiveX(VMatrix& dst, double flFovX, double flAspect, double flZNear, double flZFar)
{
	VMatrix mat;
	MatrixBuildPerspectiveX(mat, flFovX, flAspect, flZNear, flZFar);

	VMatrix temp;
	MatrixMultiply(dst, mat, temp);
	dst = temp;
}

inline void MatrixPerspectiveOffCenterX(VMatrix& dst, double flFovX, double flAspect, double flZNear, double flZFar, double bottom, double top, double left, double right)
{
	VMatrix mat;
	MatrixBuildPerspectiveOffCenterX(mat, flFovX, flAspect, flZNear, flZFar, bottom, top, left, right);

	VMatrix temp;
	MatrixMultiply(dst, mat, temp);
	dst = temp;
}
