# randomTerrain-opengl
Create a random terrain with faulting method using opengl  

1.设置  
该项目引用了glfw，glad和glm库，需要将额外的库添加到visual studio中  
这里采用的方法是添加额外的include文件夹  
在解决方案资源管理器中右键terrainGenerator打开属性  
在c/c++->常规->附加包含目录中添加\glfw-3.3.3.bin.WIN32\include文件夹的地址（注意要添加从C:/等系统盘开始的完整地址）  
在链接器->输入->附加依赖项中添加glfw3.lib;kernel32.lib;  


2.效果预览  
![高度渲染](https://github.com/BBzzzr/randomTerrain-opengl/blob/main/2.png)  
![贴图渲染](https://github.com/BBzzzr/randomTerrain-opengl/blob/main/1.png)  



3.附录  
该项目中shader.h文件引用自http://www.learnopengles.com/  
