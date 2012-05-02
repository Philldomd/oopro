#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"

class Camera
{
public:
	Camera(D3DXVECTOR3 initPos);
	~Camera();

	void				createProjectionMatrix( float pFOV, float pAspect,
												float pNearPlane, float pFarPlane );
	void				updateCameraPos();
	void				updateViewMatrix();
	//void				setViewMatrix(D3DXVECTOR3 pPos);
	void				setY( float f );
	//void				setPosition(float x, float y, float z);
	void				walk( float f );
	void				strafe( float f );
	void				updateYaw( float d );
	void				updatePitch( float d );
	D3DXVECTOR3			getPosition();
	//D3DXVECTOR3			getLookAt();
	D3DXMATRIX			getProjectionMatrix();
	D3DXMATRIX			getViewMatrix();
	bool				isCameraFree();
	void				setCameraFree(bool p);
	void				setCameraPos(D3DXVECTOR3 p_pos, D3DXVECTOR3 p_dir);
	//float				getFarPlane();

private:
	D3DXVECTOR3		mCameraPos;
	D3DXVECTOR3		mUp;
	D3DXVECTOR3		mLookAtPos;
	D3DXVECTOR3		mVelocity;
	D3DXVECTOR3		mLookAt;
	D3DXVECTOR3		mRight;

	D3DXMATRIX		mView;
	D3DXMATRIX		mProjection;

	float			mFOV;
	float			mAspect;
	float			mNearPlane;
	float			mFarPlane;

	float			mPitch;
	float			MAXpitch;
	float			MINpitch;
	float			mYaw;
	float			mCameraSpeed;
	bool			mFree;

	float			cotan(float i);
};

#endif CAMERA_H