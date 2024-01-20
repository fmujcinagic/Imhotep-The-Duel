# Imhotep: The Duel
 
## Reference
This project was developed as part of the  Objektorientierte Programmierung 1 at TU Graz, under the guidance of Weinrauch Alexander.
## Project Description
Integrated real-time communication with the server using ```net::Client```, focusing on network protocol adherence and message parsing. Implemented AI algorithms to process game states and make strategic decisions, demonstrating skills in C++ programming and algorithmic thinking. Designed the client for robust performance, including handling end-game scenarios, which refined problem-solving and software development skills.

## Contributors
- Faris Mujcinagic [(@fmujcinagic)](https://github.com/fmujcinagic)
- Fedja Mulabegovic [(@fedjamulabegovic)](https://github.com/fedjamulabegovic)
- Milan Mejic

### First time setup
Run ```setup.sh``` linux/macOS or ```setup.bat``` on windows. 

### Building the project
It is recommended to create a build folder inside the root directory of this repository. Inside this ```build``` folder ```run cmake .```. to setup the build system. After this step is completed you can build your code with ```cmake --build .```. On newer cmake version you can use cmake ```--build . --parallel``` to speed up the compilation.
This will generate the executable imhotep_server inside the build folder. On Windows the executable will be located in a subdirectory named Debug inside the build folder.
