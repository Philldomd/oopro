#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include "stdafx.h"

class BoundingVolume
{
public:
	//int				getIndex();
	//bool			compair( int p_index );

	virtual void	updatePosition( D3DXMATRIX p_scale, D3DXMATRIX p_translation );
	D3DXVECTOR3*	getPosition();
	D3DXVECTOR3*	getPrevPosition();
	int				getType();
	virtual void	draw();
protected:
	D3DXVECTOR3		m_position;
	D3DXVECTOR3		m_prevPosition;
	//int				m_index;
	int				m_type;

	enum TYPE {
		AABBOX
	};
};

#endif