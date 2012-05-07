#ifndef PACMAN_H
#define PACMAN_H
#include "Object.h"
#include "Shader.h"

class Pacman : public Object
{
public:
	Pacman(ID3D10Device* p_device, Model* p__model, D3DXVECTOR3 p_position, D3DXVECTOR2 p_size);
	virtual ~Pacman();
	void	initialize(Shader* p_shader);
	virtual void	update(float p_deltaTime);
	void			render(D3DXMATRIX& p_view, D3DXMATRIX& p_projection);
	// void	orient(D3DXVECTOR3 p_orientation);
	void			setDirection(D3DXVECTOR3 p_direction); 
	void			turnLeft();
	void			turnRight();
	void			uTurn();
	D3DXVECTOR3		getDirection();
private:
	float m_forwardSpeed, m_headingSpeed, m_speedScale;
	D3DXVECTOR3	m_direction, m_velocity, m_eulerOrient;
	Shader* m_shader;
};
#endif