#include "Cloth.hpp"
#include "SpringForce.hpp"
#include <cstdlib>

static std::vector<Particle*> pVector;
static std::vector<Force*> fVector;

static std::vector<std::vector<Particle*> > allParticles;
static std::vector<Force*> stretchForces;
static std::vector<Force*> shearForces;
static std::vector<Force*> bendForces;
static std::vector<Force*> allForces;
static std::vector<Particle*> fixedParticles;

bool stretch_on = true;
bool shear_on = true;
bool bend_on = true;

#define RAND (((rand()%2000)/1000.f)-1.f)
#define DAMP 0.98f

Cloth::Cloth(){

	dt = 0.1;

	// spring correctness test
	double dist = 2.0;
	Vector3f center = make_vector(-20.0f, 5.0f, 0.0f);
	Vector3f offset = make_vector(2.0f, 0.0f, 0.0f);
	pVector.push_back(new Particle(center + offset));
	pVector.push_back(new Particle(center + offset + offset));
	pVector.push_back(new Particle(center + offset + offset + offset));
	fVector.push_back(new SpringForce(pVector[0], pVector[1], dist, 4.0, 0.5));
	fVector.push_back(new SpringForce(pVector[1], pVector[2], dist, 4.0, 0.5));

	// grid
	Vector3f sideLength = make_vector(10.0f, 0.0f, 0.0f);
	Vector3f topleft = make_vector(-2.0f, 5.0f, 0.0f);
	int divisions = 10;
	Vector3f xStep = sideLength / divisions;
	Vector3f zStep = make_vector(0.0f, 0.0f, -12.0f) / divisions;
	for (int i = 0; i < divisions; i++) {
		std::vector<Particle*> row;
		for (int j = 0; j < divisions; j++) {
			Particle* p = new Particle(topleft + xStep * i + zStep * j);
			p->m_Mass = 1.0f;
			row.push_back(p);
		}
		allParticles.push_back(row);
	}

	int stretchDist = (length(sideLength) / divisions) ;
	double stretchKs = 6.0; 
	double stretchKd = 2.0; 
	double shearKs = 20.0;
	double shearKd = stretchKd;
	double bendKs = 10.0;
	double bendKd = stretchKd;
	for (int i = 0; i < divisions; i++) {
		for (int j = 0; j < divisions; j++) {
			// stretch spring
			if (stretch_on) {
				if (j < divisions - 1) {
					Force* stretch = new SpringForce(allParticles[i][j], 
						allParticles[i][j+1], 
						stretchDist, stretchKs, stretchKd);
					allForces.push_back(stretch);
					stretchForces.push_back(stretch);
				}
				if (i < divisions - 1) {
					Force* stretch = new SpringForce(allParticles[i][j], 
						allParticles[i+1][j], 
						stretchDist, stretchKs, stretchKd);
					allForces.push_back(stretch);
					stretchForces.push_back(stretch);
				}
			}

			// shear spring
			if (shear_on) {
				if (i < divisions - 1 && j < divisions - 1) {
					Force* shear = new SpringForce(allParticles[i][j], 
						allParticles[i+1][j+1], 
						stretchDist * sqrt(2), shearKs, shearKd);
					allForces.push_back(shear);
					shearForces.push_back(shear);
				}
				if (i < divisions - 1 && j > 0) {
					Force* shear = new SpringForce(allParticles[i][j], 
						allParticles[i+1][j-1], 
						stretchDist * sqrt(2), shearKs, shearKd);
					allForces.push_back(shear);
					shearForces.push_back(shear);
				}
			} 

			// bend spring 
			if (bend_on) {
				if (i < divisions - 2) {
					Force* bend = new SpringForce(allParticles[i][j], 
						allParticles[i+2][j], 
						stretchDist * 2, bendKs, bendKd);
					allForces.push_back(bend);
					bendForces.push_back(bend);
				}
				if (j < divisions - 2) {
					Force* bend = new SpringForce(allParticles[i][j], 
						allParticles[i][j+2], 
						stretchDist * 2, bendKs, bendKd);
					allForces.push_back(bend);
					bendForces.push_back(bend);
				}
			}
		}
	}

	// constrain center vertices
	for (int i = 5; i < 7; i++) {
		for (int j = 5; j < 7; j++) {
			allParticles[i][j]->m_Fixed = true;
		}
	}

	// constrain four corners 
	// allParticles[0][0]->m_Fixed = true;
	// allParticles[0][divisions-1]->m_Fixed = true;
	// allParticles[divisions-1][0]->m_Fixed = true;
	// allParticles[divisions-1][divisions-1]->m_Fixed = true;

}

Cloth::~Cloth(){
	pVector.clear();
	fVector.clear();

	allParticles.clear();
	allForces.clear();

}

void Cloth::reset(){
	int size = pVector.size();
	for(int ii=0; ii<size; ii++){
		pVector[ii]->reset();
	}

	for (int i=0; i < allParticles.size();i++) {
		for (int j=0; j < allParticles[i].size(); j++) {
			allParticles[i][j]->reset();
		}
	}

}

void Cloth::draw(){
	int size = pVector.size();
	for(int ii=0; ii<size; ii++){
		pVector[ii]->draw();
	}

	size = fVector.size();
	for(int ii=0; ii<size; ii++){
		fVector[ii]->draw();
	}

	for (int i = 0; i < stretchForces.size(); i++) {
		stretchForces[i]->draw();
	}
}

void Cloth::simulation_step(){

	///TODO:
	
	// first, you need to clear force accumulators for all the particles
	for (int i = 0; i < pVector.size(); i++)
	{
		pVector[i]->clearForce();
		if (i == 1) {
			pVector[i]->m_ForceAccumulator 
				+= make_vector(0.0f, -1.8f, 0.0f) * 
					pVector[i]->m_Mass;
		}
	}

	for (int i=0; i < allParticles.size();i++) {
		for (int j=0; j < allParticles[i].size(); j++) {
			allParticles[i][j]->clearForce();
			// don't apply gravity to fixed particles
			if (allParticles[i][j]->m_Fixed) continue;
			// apply gravity to non-fixed particles
			allParticles[i][j]->m_ForceAccumulator 
				+= make_vector(0.0f, -9.8f, 0.0f) * 
					allParticles[i][j]->m_Mass;
		}
	}

	///second, apply forces to them
	for (int i = 0; i < fVector.size(); i++)
	{
			fVector[i]->apply_force();
	}

	for (int i = 0; i < allForces.size(); i++)
	{
		allForces[i]->apply_force();
	}

	///if you want to implement hard constraints, the third step is to calculate constraint forces
	///for the basic cloth simulation, you can skip this.

	///Then, we can move forward
	///Change this to others if you want to implement RK2 or RK4 or other integration method
	euler_step();

	///Finally, if you want to implement collisions, you could solve them here
	///for the basic cloth simulation, you can skip this.
}

void Cloth::euler_step(){
	int ii, size = pVector.size();

	for(ii=0; ii<size; ii++){
		if (ii == 1) {
			pVector[ii]->m_Position += dt*pVector[ii]->m_Velocity;
			pVector[ii]->m_Velocity = DAMP*pVector[ii]->m_Velocity 
			+ pVector[ii]->m_ForceAccumulator/pVector[ii]->m_Mass
			*0.005;
		}
	}

	for (int i=0; i < allParticles.size();i++) {
		for (int j=0; j < allParticles[i].size(); j++) {
			// don't change the position or velocity of fixed particles
			if (allParticles[i][j]->m_Fixed) continue;
			allParticles[i][j]->m_Position += dt*allParticles[i][j]->m_Velocity;
			allParticles[i][j]->m_Velocity = DAMP*allParticles[i][j]->m_Velocity
				+ allParticles[i][j]->m_ForceAccumulator/allParticles[i][j]->m_Mass*0.005;
		}
	}
}

