# Test_SDL2_Box2D_Example

----------------------------------------------------------------------------------------------------
This is a simulation test using Box2D to provide aa simple interactive sandbox using SDL2 to render
and provide keyboard / mouse input.

It allows you to create static edges of any vertex length, various dynamic / static shapes, and 
humanish ragdolls. It provides a camera view onto the simulation and dynamic objects can be manipulated 
using the mouse.

I am aware that using a custom widget is not a particularly fast way to render something like this 
but it might serve as a good learning resource for both SDL2 and Box2D.

----------------------------------------------------------------------------------------------------
Controls:

Mouse wheel 				- zoom in / out
Left mouse button 			- pan camera
Right mouse button 			- click to place Box2D edge vertex (click a second time to finalise)
Ctrl + Right mouse button 	- hold ctrl and right click to place Box2D edge vertices (release ctrl and right click a final time to finalise)
Ctrl + Left mouse button	- click and hold objects to drag them around
1							- create a random equilateral shape with a random number of vertices at the mouse position
2							- create a random shape with a random number of vertices with edges with random lengths at the mouse position
3							- create a random circle with a random radius at the mouse position
4							- create a simple ragdoll at the mouse position
ctrl + [1, 2, 3]			- create a static (unmovable) shape for the key pressed at the mouse position
r							- reset the simulation
p                           - pause / unpause the simulation