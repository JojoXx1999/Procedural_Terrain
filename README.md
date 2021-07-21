# Procedural_Terrain
Procedural project with post processing effects

Created in my third year of Abertay University

App1.cpp is the main file in this project, this file creates the game objects, contains the multiple
passes for post processesing effects and handles the ImGui interface for the user.

TerrainMeshExample.cpp contains most of the procedural generation effects for the terrain object and
creates/updates it's vertices.

Shadow_ps.hlsl is the main shader in this project. It allows the procedurally generated terrain to
cast shadows onto itself. This shader also controls the height and gradient based texturing

GeneratedName.cpp will create a random name for each scene, the types of names that can be generated 
depend on the type of terrain, for example: it's height.

More information can be found here: http://www.jodieduff.co.uk/procedural_terrain.html

