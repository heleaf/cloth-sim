#include "SpringForce.hpp"

SpringForce::SpringForce(Particle *p1, Particle *p2, double dist, double ks, double kd) : 
m_p1(p1), m_p2(p2), m_dist(dist), m_ks(ks), m_kd(kd){}

void SpringForce::draw()
{
	glBegin( GL_LINES );
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f( m_p1->m_Position.x, m_p1->m_Position.y, m_p1->m_Position.z);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f( m_p2->m_Position.x, m_p2->m_Position.y, m_p2->m_Position.z);
	glEnd();
}

void SpringForce::apply_force()
{
	auto p1 = m_p1->m_Position;
	auto p2 = m_p2->m_Position;
	auto v1 = m_p1->m_Velocity;
	auto v2 = m_p2->m_Velocity;
	auto ks = m_ks;
	auto kd = m_kd;
	auto l0 = m_dist;
	auto f1 = -(
		ks*(length(p1-p2)-l0) + 
		kd*(((v1-v2)*(p1-p2))/length(p1-p2))
	)*(p1-p2)/length(p1-p2);
	auto f2 = -f1;
	m_p1->m_ForceAccumulator += f1;
	m_p2->m_ForceAccumulator += f2;
}

