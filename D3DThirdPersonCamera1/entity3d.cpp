#include "entity3d.h"

Entity3D::Entity3D()
{
    D3DXMatrixIdentity(&m_worldMatrix);
    D3DXQuaternionIdentity(&m_orientation);
    D3DXQuaternionIdentity(&m_rotation);
    
    m_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
    m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
    m_forward = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
    
    m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_eulerOrient = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    m_eulerRotate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    
    m_constrainedToWorldYAxis = false;
}

Entity3D::~Entity3D()
{
}

void Entity3D::constrainToWorldYAxis(bool constrain)
{
    // Constraining rotations to the world Y axis means that all heading
    // changes are applied to the world Y axis rather than the entity's
    // local Y axis.

    m_constrainedToWorldYAxis = constrain;
}

void Entity3D::orient(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    // orient() changes the direction the entity is facing. This directly
    // affects the orientation of the entity's right, up, and forward vectors.
    // orient() is usually called in response to the user's input if the entity
    // is able to be moved by the user.

    m_eulerOrient.x += pitchDegrees;
    m_eulerOrient.y += headingDegrees;
    m_eulerOrient.z += rollDegrees;

    if (m_eulerOrient.x > 360.0f)
        m_eulerOrient.x -= 360.0f;

    if (m_eulerOrient.x < -360.0f)
        m_eulerOrient.x += 360.0f;

    if (m_eulerOrient.y > 360.0f)
        m_eulerOrient.y -= 360.0f;

    if (m_eulerOrient.y < -360.0f)
        m_eulerOrient.y += 360.0f;

    if (m_eulerOrient.z > 360.0f)
        m_eulerOrient.z -= 360.0f;

    if (m_eulerOrient.z < -360.0f)
        m_eulerOrient.z += 360.0f;
}

void Entity3D::rotate(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    // rotate() does not change the direction the entity is facing. This method
    // allows the entity to freely spin around without affecting its orientation
    // and its right, up, and forward vectors. For example, if this entity is
    // a planet, then rotate() is used to spin the planet on its y axis. If this
    // entity is an asteroid, then rotate() is used to tumble the asteroid as
    // it moves in space.

    m_eulerRotate.x += pitchDegrees;
    m_eulerRotate.y += headingDegrees;
    m_eulerRotate.z += rollDegrees;

    if (m_eulerRotate.x > 360.0f)
        m_eulerRotate.x -= 360.0f;

    if (m_eulerRotate.x < -360.0f)
        m_eulerRotate.x += 360.0f;

    if (m_eulerRotate.y > 360.0f)
        m_eulerRotate.y -= 360.0f;

    if (m_eulerRotate.y < -360.0f)
        m_eulerRotate.y += 360.0f;

    if (m_eulerRotate.z > 360.0f)
        m_eulerRotate.z -= 360.0f;

    if (m_eulerRotate.z < -360.0f)
        m_eulerRotate.z += 360.0f;
}

const D3DXVECTOR3 &Entity3D::getForwardVector() const
{
    return m_forward;
}

const D3DXVECTOR3 &Entity3D::getPosition() const
{
    return m_position;
}

const D3DXVECTOR3 &Entity3D::getRightVector() const
{
    return m_right;
}

const D3DXVECTOR3 &Entity3D::getUpVector() const
{
    return m_up;
}

const D3DXVECTOR3 &Entity3D::getVelocity() const
{
    return m_velocity;
}

const D3DXMATRIX &Entity3D::getWorldMatrix() const
{
    return m_worldMatrix;
}

void Entity3D::setPosition(float x, float y, float z)
{
    m_position.x = x;
    m_position.y = y;
    m_position.z = z;
}

void Entity3D::setVelocity(float x, float y, float z)
{
    m_velocity.x = x;
    m_velocity.y = y;
    m_velocity.z = z;
}

void Entity3D::setWorldMatrix(const D3DXMATRIX &worldMatrix)
{
    m_worldMatrix = worldMatrix;
    D3DXQuaternionRotationMatrix(&m_orientation, &worldMatrix);
    
    m_position.x = worldMatrix(3,0);
    m_position.y = worldMatrix(3,1);
    m_position.z = worldMatrix(3,2);

    extractAxes();
}

void Entity3D::update(float elapsedTimeSec)
{
    D3DXVECTOR3 velocityElapsed, eulerOrientElapsed, eulerRotateElapsed;
    D3DXVECTOR3 oldPos, heading;
    D3DXQUATERNION tempQuat;
    D3DXMATRIX tempMtx;

    velocityElapsed = m_velocity * elapsedTimeSec;
    eulerOrientElapsed = m_eulerOrient * elapsedTimeSec;
    eulerRotateElapsed = m_eulerRotate * elapsedTimeSec;

    // Update the entity's position.
    
    extractAxes();

    oldPos = m_position;

    m_position += m_right * velocityElapsed.x;
    m_position += m_up * velocityElapsed.y;
    m_position += m_forward * velocityElapsed.z;

    heading = m_position - oldPos;
    D3DXVec3Normalize(&heading, &heading);

    // Update the entity's orientation.
    
    D3DXMatrixRotationQuaternion(&tempMtx, &m_orientation);
    tempQuat = eulerToQuaternion(tempMtx, eulerOrientElapsed.y,
                eulerOrientElapsed.x, eulerOrientElapsed.z);
    
    // When moving backwards invert rotations to match direction of travel.
    
    if (D3DXVec3Dot(&heading, &m_forward) < 0.0f)
        D3DXQuaternionInverse(&tempQuat, &tempQuat);

    m_orientation *= tempQuat;
    D3DXQuaternionNormalize(&m_orientation, &m_orientation);
    
    // Update the entity's free rotation.
    
    D3DXMatrixRotationQuaternion(&tempMtx, &m_rotation);
    tempQuat = eulerToQuaternion(tempMtx, eulerRotateElapsed.y,
                eulerRotateElapsed.x, eulerRotateElapsed.z);
    
    m_rotation *= tempQuat;
    D3DXQuaternionNormalize(&m_rotation, &m_rotation);
    
    // Update the entity's world matrix.

    tempQuat = m_rotation * m_orientation;
    D3DXQuaternionNormalize(&tempQuat, &tempQuat);
    
    D3DXMatrixRotationQuaternion(&m_worldMatrix, &tempQuat);
    m_worldMatrix(3,0) = m_position.x;
    m_worldMatrix(3,1) = m_position.y;
    m_worldMatrix(3,2) = m_position.z;

    // Clear the entity's cached euler rotations and velocity for this frame.

    m_velocity.x = m_velocity.y = m_velocity.z = 0.0f;
    m_eulerOrient.x = m_eulerOrient.y = m_eulerOrient.z = 0.0f;
    m_eulerRotate.x = m_eulerRotate.y = m_eulerOrient.z = 0.0f;
}

D3DXQUATERNION Entity3D::eulerToQuaternion(const D3DXMATRIX &m, float headingDegrees,
                                           float pitchDegrees, float rollDegrees) const
{
    // Construct a quaternion from an euler transformation. We do this rather
    // than use D3DXQuaternionRotationYawPitchRoll() to support constraining
    // heading changes to the world Y axis.

    D3DXQUATERNION result;
    D3DXQUATERNION rotation;
    D3DXVECTOR3 localXAxis(m(0,0), m(0,1), m(0,2));
    D3DXVECTOR3 localYAxis(m(1,0), m(1,1), m(1,2));
    D3DXVECTOR3 localZAxis(m(2,0), m(2,1), m(2,2));

    D3DXQuaternionIdentity(&result);

    if (headingDegrees != 0.0f)
    {
        if (m_constrainedToWorldYAxis)
        {
            D3DXVECTOR3 worldYAxis(0.0f, 1.0f, 0.0f);
            D3DXQuaternionRotationAxis(&rotation, &worldYAxis, D3DXToRadian(headingDegrees));
        }
        else
        {
            D3DXQuaternionRotationAxis(&rotation, &localYAxis, D3DXToRadian(headingDegrees));
        }

        result *= rotation;
    }

    if (pitchDegrees != 0.0f)
    {
        D3DXQuaternionRotationAxis(&rotation, &localXAxis, D3DXToRadian(pitchDegrees));
        result *= rotation;
    }

    if (rollDegrees != 0.0f)
    {
        D3DXQuaternionRotationAxis(&rotation, &localZAxis, D3DXToRadian(rollDegrees));
        result *= rotation;
    }

    return result;
}

void Entity3D::extractAxes()
{
    D3DXMATRIX m;  
    D3DXMatrixRotationQuaternion(&m, &m_orientation);

    m_right.x = m(0,0);
    m_right.y = m(0,1);
    m_right.z = m(0,2);
    D3DXVec3Normalize(&m_right, &m_right);
    
    m_up.x = m(1,0);
    m_up.y = m(1,1);
    m_up.z = m(1,2);
    D3DXVec3Normalize(&m_up, &m_up);
    
    m_forward.x = m(2,0);
    m_forward.y = m(2,1);
    m_forward.z = m(2,2);
    D3DXVec3Normalize(&m_forward, &m_forward);
}