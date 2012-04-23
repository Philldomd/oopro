#ifndef CAMERA_H
#define CAMERA_H

#include "stdafx.h"

class Camera
{
public:
	//Constructor and Destructors
	Camera();
	~Camera();

	void	lookAt(const D3DXVECTOR3 &p_target);
	void	lookAt(const D3DXVECTOR3 &p_eye,
			 const D3DXVECTOR3 &p_target,
			 const D3DXVECTOR3 &p_up);
	void	perspective(float p_fovx, float p_aspect, float p_znear, float p_zfar);
	void	rotate(float p_pitch, float p_yaw);
	void	update(float p_gameTime);

	//Getter methods.

	const D3DXVECTOR3		&getPosition();
	const D3DXMATRIX		&getViewMatrix();
	const D3DXVECTOR3		&getLookAt();
	const D3DXMATRIX		&getProjectionMatrix();

private:
	static const float DEFAULT_FOVX;
	static const float DEFAULT_ZNEAR;
	static const float DEFAULT_ZFAR;
	static const D3DXVECTOR3 WORLD_XAXIS;
	static const D3DXVECTOR3 WORLD_YAXIS;
	static const D3DXVECTOR3 WORLD_ZAXIS;

	float m_longitudeDegrees; //Pitch
	float m_latitudeDegrees; //Yaw
	float m_fovx;
	float m_znear;
	float m_zfar;
	D3DXVECTOR3 m_eye;
	D3DXVECTOR3 m_target;
	D3DXVECTOR3 m_offset;
	D3DXVECTOR3 m_xAxis;
	D3DXVECTOR3 m_yAxis;
	D3DXVECTOR3 m_zAxis;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projMatrix;
	D3DXQUATERNION m_orientation;
};

#endif