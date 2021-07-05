# randomTerrain-opengl
Create a random terrain with faulting method using opengl  

1.setting
This project use glfw, glad and glm library, you need to include these libraries into visual studio.

Right click terrainGenerator->Properties in SolutionExplorer
Add the address of "...glfw-3.3.3.bin.WIN32\include" into C++/C->general->Additinal Include Directories
Add "glfw3.lib;kernel32.lib;" to Linker->Input->Additional Dependencies


2.preview
![Rendering with texture](https://i.loli.net/2021/03/23/LBxw6vdMklAVTpU.png)   
![Rendering with altitude](https://i.loli.net/2021/03/23/JSrLub1C8UEdRQ3.png)  


3.appendix
The shader.h file in this project comes form http://www.learnopengles.com/  
(该项目中shader.h文件引用自http://www.learnopengles.com/)
