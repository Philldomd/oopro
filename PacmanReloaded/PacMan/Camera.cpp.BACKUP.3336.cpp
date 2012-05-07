<<<<<<< HEAD
#include "Camera.h"

Camera::Camera(D3DXVECTOR3 initPos)
{
	mCameraPos	= initPos;
	mVelocity	= D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	mRight		= D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	mUp			= D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
	mLookAt		= D3DXVECTOR3( 0.0f, 0.0f, 1.0f );

	mPitch		= 0.0f;
	mYaw		= PI/2;
	mLookAtPos	= mCameraPos + mLookAt;

	D3DXMatrixIdentity(&mView);
	D3DXMatrixIdentity(&mProjection);

	MAXpitch	= PI/2 - 0.2f;
	MINpitch	= -PI/2  + 0.2f;

	mCameraSpeed = 200.0f;
	mFree = false;
	//mFrustum = BoundingFrustum();
}

Camera::~Camera(){}

void Camera::createProjectionMatrix( float pFOV, float pAspect,
	float pNearPlane, float pFarPlane )
{
	mFOV		= pFOV;
	mAspect		= pAspect;
	mNearPlane	= pNearPlane;
	mFarPlane	= pFarPlane;

	float yScale	= cotan( mFOV / 2 );
	float xScale	= yScale / mAspect;
	float planeDiff	= mFarPlane - mNearPlane;

	// Lens.
	mProjection._11 = xScale;
	mProjection._22 = yScale;
	mProjection._33 = mFarPlane / planeDiff;
	mProjection._43 = ( mNearPlane * mFarPlane ) / ( mNearPlane - mFarPlane );
	mProjection._34 = 1.0f;
	mProjection._44 = 0.0f;

	//mFrustum.setNearFar(mFOV, mAspect, mNearPlane, mFarPlane);
}

void Camera::updateViewMatrix()
{
	mUp = D3DXVECTOR3(0.0f,1.0f,0.0f);
	mLookAt = D3DXVECTOR3(0.0f,0.0f,1.0f);
	mRight = D3DXVECTOR3(1.0f,0.0f,0.0f);

	D3DXMATRIX R;

	D3DXMatrixRotationAxis(&R, &mRight, mPitch);

	D3DXVec3TransformNormal(&mUp, &mUp, &R);
	D3DXVec3TransformNormal(&mLookAt, &mLookAt, &R);

	D3DXMatrixRotationY(&R, mYaw);

	D3DXVec3TransformNormal(&mUp, &mUp, &R);
	D3DXVec3TransformNormal(&mRight, &mRight, &R);
	D3DXVec3TransformNormal(&mLookAt, &mLookAt, &R);

	// Update view matrix.
	mView._11 = mRight.x; mView._12 = mUp.x; mView._13 = mLookAt.x;
	mView._21 = mRight.y; mView._22 = mUp.y; mView._23 = mLookAt.y;
	mView._31 = mRight.z; mView._32 = mUp.z; mView._33 = mLookAt.z;

	mView._41 = - D3DXVec3Dot( &mCameraPos, &mRight );
	mView._42 = - D3DXVec3Dot( &mCameraPos, &mUp );
	mView._43 = - D3DXVec3Dot( &mCameraPos, &mLookAt );

	mView._14 = 0.0f;
	mView._24 = 0.0f;
	mView._34 = 0.0f;
	mView._44 = 1.0f;

	//D3DXMatrixLookAtLH(&mView, &mCameraPos, &mLookAtPos, &mUp);

	/*mFrustum.setPlanes( Vector(mCameraPos.x,	mCameraPos.y,	mCameraPos.z), 
		Vector(mLookAt.x,		mLookAt.y,		mLookAt.z), 
		Vector(mUp.x,			mUp.y,			mUp.z)
		);
	mFrustum.updatePosition(mCameraPos.x,	mCameraPos.y,	mCameraPos.z);*/
}

//void Camera::setViewMatrix(D3DXVECTOR3 pPos)
//{
//	mUp = D3DXVECTOR3(0.0f,1.0f,0.0f);
//	mLookAt = D3DXVECTOR3(0.0f,0.0f,1.0f);
//	mRight = D3DXVECTOR3(1.0f,0.0f,0.0f);
//
//	mCameraPos = pPos;
//
//	mPitch = 40*(PI/180);
//	mYaw = 270*(PI/180);
//
//	D3DXMATRIX R;
//
//	D3DXMatrixRotationAxis(&R, &mRight, mPitch);
//
//	D3DXVec3TransformNormal(&mUp, &mUp, &R);
//	D3DXVec3TransformNormal(&mLookAt, &mLookAt, &R);
//
//	D3DXMatrixRotationY(&R, mYaw);
//
//	D3DXVec3TransformNormal(&mUp, &mUp, &R);
//	D3DXVec3TransformNormal(&mRight, &mRight, &R);
//	D3DXVec3TransformNormal(&mLookAt, &mLookAt, &R);
//
//	// Update view matrix.
//	mView._11 = mRight.x; mView._12 = mUp.x; mView._13 = mLookAt.x;
//	mView._21 = mRight.y; mView._22 = mUp.y; mView._23 = mLookAt.y;
//	mView._31 = mRight.z; mView._32 = mUp.z; mView._33 = mLookAt.z;
//
//	mView._41 = - D3DXVec3Dot( &mCameraPos, &mRight );
//	mView._42 = - D3DXVec3Dot( &mCameraPos, &mUp );
//	mView._43 = - D3DXVec3Dot( &mCameraPos, &mLookAt );
//
//	mView._14 = 0.0f;
//	mView._24 = 0.0f;
//	mView._34 = 0.0f;
//	mView._44 = 1.0f;
//
//	mFrustum.setPlanes( Vector(mCameraPos.x,	mCameraPos.y,	mCameraPos.z), 
//		Vector(mLookAt.x,		mLookAt.y,		mLookAt.z), 
//		Vector(mUp.x,			mUp.y,			mUp.z)
//		);
//	mFrustum.updatePosition(mCameraPos.x,	mCameraPos.y,	mCameraPos.z);
//}

void Camera::walk( float f )
{
	f *= mCameraSpeed;
	D3DXVECTOR3		moveVector( mLookAt.x, 0.0f, mLookAt.z );
	D3DXVec3Normalize( &moveVector, &moveVector );
	moveVector *= f;
	mVelocity += moveVector;
}

void Camera::strafe( float f )
{
	f *= mCameraSpeed;
	mVelocity += mRight * f;
}

void Camera::updateCameraPos()
{
	// Move camera.
	mCameraPos += mVelocity;

	mVelocity = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );

	mLookAtPos = mCameraPos + mLookAt;
}

void Camera::setY( float f )
{
	mCameraPos.y += f ;
}

void Camera::updateYaw( float d )
{
	mYaw += d;

	if(mYaw >= 2 * PI)
		mYaw -= 2*PI;
	else if(mYaw <= 0.0f )
		mYaw += 2*PI;
}

void Camera::updatePitch( float d )
{
	if(mPitch < MAXpitch) // Can't pitch up more than MAXpitch.
		mPitch += d;
	else
		mPitch = MAXpitch;

	if(mPitch > MINpitch) // Can't pitch down more than MINpitch.
		mPitch += d;
	else
		mPitch = MINpitch;
}

float Camera::cotan( float d )
{
	return 1 / tan(d);
}

D3DXMATRIX Camera::getProjectionMatrix()
{
	return mProjection;
}

D3DXMATRIX Camera::getViewMatrix()
{
	return mView;
}

D3DXVECTOR3 Camera::getPosition()
{
	return mCameraPos;
}
bool Camera::isCameraFree()
{
	return mFree;
}
void Camera::setCameraFree(bool p)
{
	mFree = p;
}

void Camera::setCameraPos(D3DXVECTOR3 p_pos, D3DXVECTOR3 p_dir)
{
	mCameraPos = p_pos;

	mLookAtPos = mCameraPos + p_dir;
}
//
//D3DXVECTOR3 Camera::getLookAt()
//{
//	return mLookAt;
//}
//
//void Camera::setPosition(float x, float y, float z)
//{
//	mCameraPos = D3DXVECTOR3(x,y,z);
//}
//
//BoundingFrustum* Camera::getFrustum()
//{
//	return &mFrustum;
//}
//
//float Camera::getFarPlane()
//{
//	return mFarPlane;
//}
=======
#include "camera.h"
#include <math.h>
const float Camera::DEFAULT_FOVX = 80.f;
const float Camera::DEFAULT_ZFAR = 1000.0f;
const float Camera::DEFAULT_ZNEAR = 1.0f;

const D3DXVECTOR3 Camera::WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const D3DXVECTOR3 Camera::WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const D3DXVECTOR3 Camera::WORLD_ZAXIS(0.0f, 0.0f, 1.0f);


Camera::Camera()
{
	m_longitudeDegrees = 0.0f;
	m_latitudeDegrees = 0.0f;

	m_fovx = DEFAULT_FOVX;
	m_znear = DEFAULT_ZNEAR;
	m_zfar = DEFAULT_ZFAR;

	m_eye = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_offset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_xAxis = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_yAxis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_zAxis = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

	D3DXMatrixIdentity(&m_viewMatrix);
	D3DXMatrixIdentity(&m_projMatrix);
	D3DXQuaternionIdentity(&m_orientation);
}

Camera::~Camera(){};

void Camera::setTarget(const D3DXVECTOR3 &p_target)
{
	m_target = p_target;
}

void Camera::lookAt(const D3DXVECTOR3 &p_eye,
			 const D3DXVECTOR3 &p_target,
			 const D3DXVECTOR3 &p_up)
{
	m_eye = p_eye;
	m_target = p_target;

	//The offset vector is the vector from the taget to the eye pos.
	//This means that the negative z axis is the following.

	m_zAxis = p_target - p_eye;
	m_offset.x = -m_zAxis.x;
	m_offset.y = -m_zAxis.y;
	m_offset.z = -m_zAxis.z;
	D3DXVec3Normalize(&m_zAxis, &m_zAxis);

	D3DXVec3Cross(&m_xAxis, &p_up, &m_zAxis);
	D3DXVec3Normalize(&m_xAxis, &m_xAxis);
	
	D3DXVec3Cross(&m_yAxis, &m_zAxis, &m_xAxis);
	D3DXVec3Normalize(&m_yAxis, &m_yAxis);
	D3DXVec3Normalize(&m_xAxis, &m_xAxis);

	D3DXMatrixIdentity(&m_viewMatrix);

	//Building View Matrix

	m_viewMatrix(0,0) = m_xAxis.x;
	m_viewMatrix(1,0) = m_xAxis.y;
	m_viewMatrix(2,0) = m_xAxis.z;
	m_viewMatrix(3,0) = -D3DXVec3Dot(&m_xAxis, &m_eye);//kanske p_eye

	m_viewMatrix(0,1) = m_yAxis.x;
	m_viewMatrix(1,1) = m_yAxis.y;
	m_viewMatrix(2,1) = m_yAxis.z;
	m_viewMatrix(3,1) = -D3DXVec3Dot(&m_yAxis, &m_eye);//kanske p_eye

	m_viewMatrix(0,2) = m_zAxis.x;
	m_viewMatrix(1,2) = m_zAxis.y;
	m_viewMatrix(2,2) = m_zAxis.z;
	m_viewMatrix(3,2) = -D3DXVec3Dot(&m_zAxis, &m_eye);//kanske p_eye


	D3DXQuaternionRotationMatrix(&m_orientation, &m_viewMatrix);
}

void Camera::perspective(float p_fovx, float p_aspect, float p_znear, float p_zfar)
{
	float e = 1.0f / tanf((float)D3DXToRadian(p_fovx) / 2.0f);
	float aspectInv = 1.0f / p_aspect;
	float fovy = 2.0f * atanf(aspectInv / e);
	float xScale = 1.0f / tanf(0.5f * fovy);
	float yScale = xScale / aspectInv;

    m_projMatrix(0,0) = xScale;
    m_projMatrix(1,0) = 0.0f;
    m_projMatrix(2,0) = 0.0f;
    m_projMatrix(3,0) = 0.0f;

    m_projMatrix(0,1) = 0.0f;
    m_projMatrix(1,1) = yScale;
    m_projMatrix(2,1) = 0.0f;
    m_projMatrix(3,1) = 0.0f;

    m_projMatrix(0,2) = 0.0f;
    m_projMatrix(1,2) = 0.0f;
    m_projMatrix(2,2) = p_zfar / (p_zfar - p_znear);
    m_projMatrix(3,2) = -p_znear * p_zfar / (p_zfar - p_znear);

    m_projMatrix(0,3) = 0.0f;
    m_projMatrix(1,3) = 0.0f;
    m_projMatrix(2,3) = 1.0f;
    m_projMatrix(3,3) = 0.0f;

    m_fovx = p_fovx;
    m_znear = p_znear;
    m_zfar = p_zfar;

}

void Camera::rotate(float p_pitch, float p_yaw)
{
	// Longitude and Latitude degrees represents the maximum
	// number of degrees of rotation per second.

	m_longitudeDegrees = p_pitch;
	m_latitudeDegrees = p_yaw;
}

void Camera::update(float gameTime)
{
	float longitudeElapsed = m_longitudeDegrees * gameTime;
	float latitudeElapsed = m_latitudeDegrees * gameTime;

	D3DXQUATERNION rot;
	D3DXQUATERNION rotInverse;

	D3DXQuaternionRotationYawPitchRoll(&rot, (FLOAT)D3DXToRadian(longitudeElapsed),
		(FLOAT)D3DXToRadian(latitudeElapsed), 0.0f);
	D3DXQuaternionConjugate(&rotInverse, &rot);

	D3DXQUATERNION offsetVector(m_offset.x, m_offset.y, m_offset.z, 0.0f);
	D3DXQUATERNION result = rotInverse * offsetVector * rot;

	D3DXVECTOR3 transformedOffsetVector(result.x, result.y, result.z);
	D3DXVECTOR3 newCameraPosition = transformedOffsetVector + m_target;

	//Rebuild view matrix

	lookAt(newCameraPosition, m_target, WORLD_YAXIS);
}

//Get Functions

const D3DXVECTOR3 &Camera::getPosition()
{
	return m_eye;
}

const D3DXVECTOR3 &Camera::getLookAt()
{
	return m_zAxis;
}

D3DXMATRIX &Camera::getViewMatrix()
{
	return m_viewMatrix;
}

D3DXMATRIX &Camera::getProjectionMatrix()
{
	return m_projMatrix;
}

const D3DXVECTOR3 &Camera::getTarget()
{
	return m_target;
}
>>>>>>> philip
