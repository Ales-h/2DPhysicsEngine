
# 2DPhysicsEngine
This learning project serves as an introduction to implementing physics engines. It simulates simple mechanics in 2-Dimensions like motion, collision detection, collision resolution.




https://github.com/user-attachments/assets/52629cba-c126-4bed-a52d-eada9037d288




**Used Libraries**
* SDL3 & SDL_Image
* ImGui
* nloohman json

I used this project also as my high-school graduation(maturita) project, so i was required to wrote a 30-ish page long documentation in Czech, it covers mainly the theory part of the implementation (numerical methods, collision detection algorithms). You can find it here ![Documentation](assets/maturitni_prace.pdf).
## Features/Specifications
* RK4 method used for motion integration
* SAT for collision detection
* Impulse-based model for collision resolution (friction is also impulse-based but doesnt work that well)
* Scene saving/loading with json
* Scene editor of object
* User interaction (adding objects, making a force field)
* Settings for the simulation (gravitational acceleration, elasticity, friction, visual elements)
<br>

## Build 
### Linux/MacOS
After cloning the repo you can build this project with Cmake:
```sh
mkdir build 
cd build
cmake ..
make  
```
### Windows
On Windows you can use Visual Studio's option to create a VS solution from Cmake.
```sh
Visual Studio 2022 >> File >> Open >> Cmake...
```
Then choose the CMakeLists.txt in the cloned repo. If this doesn't work for you. You can follow the steps that are in Build Linux/MacOS and it should create VS solution but with worse compability with the IDE.


## More Demos of the engine:
https://github.com/user-attachments/assets/234050f1-9019-42af-9875-e12f7b60e134

https://github.com/user-attachments/assets/be27e2d6-ccb7-4771-abf1-bb63764ce1a5

https://github.com/user-attachments/assets/c17c28e7-de92-4c6b-917f-9a812d4c638f





