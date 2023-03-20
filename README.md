## 15-464 Mini Project 2

I created a scene in Maya using nCloth simulation for option 5, and implemented a simple mass-spring cloth in AMC Viewer for option 1.

**Presentation:** https://docs.google.com/presentation/d/1imzpYMvvvcWPpJXoWOedwAWtd8bAh5vtxJbJkNqGJdg/edit?usp=sharing 

### Option 5: 
I learned how to use Maya’s bullet plugin and nCloth/nConstraint to simulate laundry sheets. 
- I simulated a single sheet as a subdivided quad plane, acting as an nCloth. 
- On the plane's edges, I selected 1-2 vertices on the boundary and added transform nConstraints to them to "pin" them in place. 
- To simulate wind blowing on the sheets, I adjusted the wind direction, air density, wind speed, and amount of wind noise in the nCloth's nucleus. Having a slightly upwards facing wind direction and a sizable (but not too much) amount of wind noise were crucial to making the cloth billow in variable and aesthetically pleasing ways. 
- I also adjusted the cloth's dynamic parameters itself (ex. its stretch, compression, and bend resistance values) to make it behave like different materials in the wind/gravity fields. Lower resistance values tended to allow the cloth to deform more and to flow/bend more easily and more flexibly in the simulation. 
- Adjusting the cloth mass and damping also affected how fast and sporadic the cloth moved in the simulation. 

The scene I created (of a girl sleeping in the middle of doing laundry on a balcony) can be viewed by loading `maya-sim-scene/laundry-sim.mb` into Maya and playing the keyframes from the bottom right corner. 

The animation settings I had the scene on when I simulated it played frames much faster than 24 fps, so I ended up speeding up the final scene I rendered by a fair amount. 

The final scene I rendered (and sped up) can be viewed at `maya-sim-scene/scene.mp4`. 

Some other things I would have liked to try was hiearchical cloth simulation, i.e. trying to simulate a hanging laundry string as a cloth, with the laundry cloths themselves having movement that depended on the laundry string's vertex positions. I also would have liked to integrate more rigid body simulation (ex. maybe with clothespins hanging on the laundry string, or wind chimes) into the scene, but wasn't sure how to apply soft body motion (nCloths on planes) to rigid bodies (3D meshes).

#### Assets used in scene: 
- girl: https://www.turbosquid.com/3d-models/girl-skurt-3d-model-1256673 
- stool: https://www.cgtrader.com/free-3d-models/interior/other/metal-art-stool 
- table: https://www.cgtrader.com/items/703385/download-page 
- detergent bottles: https://www.cgtrader.com/items/200836/download-page 
- railings: https://www.cgtrader.com/items/2645536/download-page 
- laundry basket: https://www.cgtrader.com/items/1949587/download-page 
- plastic laundry basket: https://www.cgtrader.com/items/721570/download-page 
- iron: https://www.cgtrader.com/items/3393594/download-page 
- environment map: https://polyhaven.com/a/table_mountain_2_puresky 

### Option 1: 
I implemented a simple mass-spring with forward euler integration in AMC Viewer, with stretch, shear, and bend springs on a gridded cloth. 

I added an extra `m_Fixed` field to the `Particle` type, to fix certain particles on the cloth so I could test the spring behaviors (ex. without having the entire cloth just fall due to gravity). For fixed particles, I computed their spring forces, but did not apply the forces to the particles or change their preset velocities and positions. For other particles, I computed their spring forces and forces applied on them due to gravity, using these to update their positions and velocities at each timestep using forward Euler integration.

I mainly tested out my simulation on 10x10 cloths. 

Some things I tried were varying the spring coefficients (i.e. k_s values) of the cloths. I found that lower k_s values lead to stretchier cloth, while higher k_s values lead to stiffer cloth.  

Extremely high k_s values lead to unstable simulations. This may also be due to the forward euler integration I used, however, since that is much more volatile than other integration schemes. 

I also tried varying the k_s values of the different springs (ex. increasing the shear and bend spring coefficients to be higher than the stretch spring coefficients).This yielded somewhat reasonable results, where the cloth was able to fold in on itself less, even without implementing self-collision. However, in general having higher k_s values seemed to yield strange behaviors-- if not simulations that went out of control, then simulations that reached equilibrium at strange places (where certain cloth particles would continue to oscillate for long periods of time). 

Finally, I wasn't able to get higher resolution cloths (ex. 30x30) to work with the simulation setup I had. I suspect this is due to the instability that forward euler integration tends to introduce. 

To run the cloth simulation in AMC Viewer, `cd` into `mass-spring/AMCViewer-simulation` and run `jam`. Then cd into `Dist` and run `./browser`. 

The files I modified from the starter code were:
- `Cloth.cpp`
- `SpringForce.cpp`
- `Particle.hpp`
- `Particle.cpp`

Some other things I would have been interested in trying were different cloth configurations, different integration schemes, collision with other objects, adding wind fields, and self-collision.
