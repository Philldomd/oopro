//-----------------------------------------------------------------------------
// Copyright (c) 2006-2008 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include <cmath>
#include "third_person_camera.h"

const float ThirdPersonCamera::DEFAULT_FOVX = 80.0f;
const float ThirdPersonCamera::DEFAULT_ZFAR = 1000.0f;
const float ThirdPersonCamera::DEFAULT_ZNEAR = 1.0f;

const D3DXVECTOR3 ThirdPersonCamera::WORLD_XAXIS(1.0f, 0.0f, 0.0f);
const D3DXVECTOR3 ThirdPersonCamera::WORLD_YAXIS(0.0f, 1.0f, 0.0f);
const D3DXVECTOR3 ThirdPersonCamera::WORLD_ZAXIS(0.0f, 0.0f, 1.0f);

ThirdPersonCamera::ThirdPersonCamera()
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

ThirdPersonCamera::~ThirdPersonCamera()
{
}

void ThirdPersonCamera::lookAt(const D3DXVECTOR3 &target)
{
    m_target = target;
}

void ThirdPersonCamera::lookAt(const D3DXVECTOR3 &eye, const D3DXVECTOR3 &target, const D3DXVECTOR3 &up)
{
    m_eye = eye;
    m_target = target;

    // The offset vector is the vector from the target 'at' position to the
    // 'eye' position. This happens to also be the local negative z axis of the
    // camera. Notice that the offset vector is always relative to the 'at'
    // position.

    m_zAxis = target - eye;
    m_offset.x = -m_zAxis.x;
    m_offset.y = -m_zAxis.y;
    m_offset.z = -m_zAxis.z;
    D3DXVec3Normalize(&m_zAxis, &m_zAxis);

    D3DXVec3Cross(&m_xAxis, &up, &m_zAxis);
    D3DXVec3Normalize(&m_xAxis, &m_xAxis);

    D3DXVec3Cross(&m_yAxis, &m_zAxis, &m_xAxis);
    D3DXVec3Normalize(&m_yAxis, &m_yAxis);
    D3DXVec3Normalize(&m_xAxis, &m_xAxis);

    D3DXMatrixIdentity(&m_viewMatrix);

    m_viewMatrix(0,0) = m_xAxis.x;
    m_viewMatrix(1,0) = m_xAxis.y;
    m_viewMatrix(2,0) = m_xAxis.z;
    m_viewMatrix(3,0) = -D3DXVec3Dot(&m_xAxis, &eye);

    m_viewMatrix(0,1) = m_yAxis.x;
    m_viewMatrix(1,1) = m_yAxis.y;
    m_viewMatrix(2,1) = m_yAxis.z;
    m_viewMatrix(3,1) = -D3DXVec3Dot(&m_yAxis, &eye);

    m_viewMatrix(0,2) = m_zAxis.x;
    m_viewMatrix(1,2) = m_zAxis.y;
    m_viewMatrix(2,2) = m_zAxis.z;
    m_viewMatrix(3,2) = -D3DXVec3Dot(&m_zAxis, &eye);

    D3DXQuaternionRotationMatrix(&m_orientation, &m_viewMatrix);
}

void ThirdPersonCamera::perspective(float fovx, float aspect, float znear, float zfar)
{
    float e = 1.0f / tanf(D3DXToRadian(fovx) / 2.0f);
    float aspectInv = 1.0f / aspect;
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
    m_projMatrix(2,2) = zfar / (zfar - znear);
    m_projMatrix(3,2) = -znear * zfar / (zfar - znear);

    m_projMatrix(0,3) = 0.0f;
    m_projMatrix(1,3) = 0.0f;
    m_projMatrix(2,3) = 1.0f;
    m_projMatrix(3,3) = 0.0f;

    m_fovx = fovx;
    m_znear = znear;
    m_zfar = zfar;
}

void ThirdPersonCamera::rotate(float longitudeDegrees, float latitudeDegrees)
{
    // Both 'longitudeDegrees' and 'latitudeDegrees' represents the maximum
    // number of degrees of rotation per second.

    m_latitudeDegrees = latitudeDegrees;
    m_longitudeDegrees = longitudeDegrees;
}

void ThirdPersonCamera::update(float elapsedTimeSec)
{
    // This method must be called once per frame to rebuild the view matrix.
    // The most important part of this update() method is the camera's offset
    // vector. Everything depends on it. The offset vector describes the
    // camera's position relative to the camera's current look at position.
    // The offset vector is always relative to the current look at position.
    // Adding the offset vector to the current look at position will give us
    // the correct camera eye position. So applying rotations to the camera
    // really means rotating the offset vector. What we are basically doing
    // is orbiting the eye position about the look at position.

    // Determine how many degrees of rotation to apply based on current time.

    float latitudeElapsed = m_latitudeDegrees * elapsedTimeSec;
    float longitudeElapsed = m_longitudeDegrees * elapsedTimeSec;

    // Rotate the offset vector based on the current camera rotation.
    // We use the quaternion triple product here to rotate the offset vector.

    D3DXQUATERNION rot;
    D3DXQUATERNION rotInv;
    
    D3DXQuaternionRotationYawPitchRoll(&rot, D3DXToRadian(longitudeElapsed),
        D3DXToRadian(latitudeElapsed), 0.0f);
    D3DXQuaternionConjugate(&rotInv, &rot);

    D3DXQUATERNION offsetVector(m_offset.x, m_offset.y, m_offset.z, 0.0f);
    D3DXQUATERNION result = rotInv * offsetVector * rot;

    // Once the offset vector has been rotated into its new orientation we
    // use the transformed offset vector to calculate the new camera 'eye'
    // position based on the camera's current target 'at' position. We do this
    // to ensure that the camera is always at the required distance from the
    // target 'at' position.

    D3DXVECTOR3 transformedOffsetVector(result.x, result.y, result.z);
    D3DXVECTOR3 newCameraPosition = transformedOffsetVector + m_target;

    // Rebuild the view matrix.

    lookAt(newCameraPosition, m_target, WORLD_YAXIS);
}