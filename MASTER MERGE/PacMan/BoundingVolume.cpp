#include "BoundingVolume.h"

//int BoundingVolume::getIndex()
//{
//	return m_index;
//}
//
//bool BoundingVolume::compair( int p_index )
//{
//	// Return true if it's the same volume, else return false.
//	if(p_index == m_index)
//	{
//		return true;
//	}
//
//	else
//	{
//		return false;
//	}
//}

int	BoundingVolume::getType()
{
	return m_type;
}

void BoundingVolume::updatePosition( D3DXMATRIX p_scale, D3DXMATRIX p_translate )
{
	m_prevPosition = m_position;
	D3DXVec3TransformCoord( &m_position, &m_position, &p_translate );
}

D3DXVECTOR3* BoundingVolume::getPosition()
{
	return &m_position;
}

D3DXVECTOR3* BoundingVolume::getPrevPosition()
{
	return &m_prevPosition;
}

void BoundingVolume::draw(D3DXMATRIX& p_world, D3DXMATRIX& p_view, D3DXMATRIX& p_proj){}