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