// Lab 3b
// Terrain generation

// Current contents:
// Terrain being generated on CPU (MakeTerrain). Simple terrain as example: Flat surface with a bump.
// Note the constants kTerrainSize and kPolySize for changing the number of vertices and polygon size!

// Things to do:
// Generate a terrain on CPU
// Generate a terrain on GPU, in the vertex shader.
// Generate textures for the surface (fragment shader).

// If you want to use Perlin noise, use the code from Lab 1.

#ifdef __APPLE__
	#include <OpenGL/gl3.h>
	#include "MicroGlut.h"
	// linking hint for Lightweight IDE
	//uses framework Cocoa
#endif
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include <math.h>
#include "simplexnoise1234.h"

mat4 projectionMatrix;
Model *floormodel;
GLuint grasstex;

// Reference to shader programs
GLuint phongShader, texShader;

#define kTerrainSize 32
#define kPolySize 1

// Terrain data. To be intialized in MakeTerrain or in the shader
vec3 vertices[kTerrainSize*kTerrainSize];
vec2 texCoords[kTerrainSize*kTerrainSize];
vec3 normals[kTerrainSize*kTerrainSize];
GLuint indices[(kTerrainSize-1)*(kTerrainSize-1)*3*2];

// These are considered unsafe, but with most C code, write with caution.
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

void MakeTerrain()
{
	// TO DO: This is where your terrain generation goes if on CPU.
	for (int x = 0; x < kTerrainSize; x++)
	for (int z = 0; z < kTerrainSize; z++)
	{
		int ix = z * kTerrainSize + x;

		#define bumpHeight 0.5
		#define bumpWidth 2.0

		// squared distance to center
		//float h = ( (x - kTerrainSize/2)/bumpWidth * (x - kTerrainSize/2)/bumpWidth +  (z - kTerrainSize/2)/bumpWidth * (z - kTerrainSize/2)/bumpWidth );
		//float y = MAX(0, 3-h) * bumpHeight;
		//float y = snoise2(x,z);

		// Properties
		const int OCTAVES = 4;
		float lacunarity = 3.0;
		float gain = .9;
		
		// Initial values
		float amplitude = .9;
		float frequency = 2.0;
		float y = 0.0;

		int temp_x = x;
		int temp_z = z;

		// Loop of octaves
		for (int i = 0; i < OCTAVES; i++) 
		{
			y += amplitude * snoise2(frequency*temp_x, frequency*temp_z);
			temp_x *= 2.;
			temp_z *= 2.;
			frequency *= lacunarity;
			amplitude *= gain;
		}
		
		vertices[ix] = SetVec3(x * kPolySize, y, z * kPolySize);
		texCoords[ix] = SetVec2(x, z);
		normals[ix] = SetVec3(0,1,0);
	}

	// Make indices
	// You don't need to change this.
	for (int x = 0; x < kTerrainSize-1; x++)
	for (int z = 0; z < kTerrainSize-1; z++)
	{
		// Quad count
		int q = (z*(kTerrainSize-1)) + (x);
		// Polyon count = q * 2
		// Indices
		indices[q*2*3] = x + z * kTerrainSize; // top left
		indices[q*2*3+1] = x+1 + z * kTerrainSize;
		indices[q*2*3+2] = x + (z+1) * kTerrainSize;
		indices[q*2*3+3] = x+1 + z * kTerrainSize;
		indices[q*2*3+4] = x+1 + (z+1) * kTerrainSize;
		indices[q*2*3+5] = x + (z+1) * kTerrainSize;
	}

	// Make normal vectors
	// TO DO: This is where you calculate normal vectors
	for (int x = 0; x < kTerrainSize; x++)
	for (int z = 0; z < kTerrainSize; z++)
	{
		/*
		   ^
		   1
		-1-0-1->
		  -1
		*/
	/*
		if (x == 0 || x == kTerrainSize-1) {
			continue;
		}
	*/

		int currentPosition = z * kTerrainSize + x;
		vec3 normalZ = SetVec3(0, 0, 0);
		vec3 normalX = SetVec3(0, 0, 0);
		
		if(currentPosition % kTerrainSize == 0)
		{
			//Vänstra kolumnen
			normalZ = VectorSub(vertices[currentPosition + 1], vertices[currentPosition]);
			printf("\nZ (v. kol): ");
			printVec3(normalZ);
		}


		if(currentPosition % kTerrainSize == (kTerrainSize - 1))
		{
			//Högra kolumnen
			normalZ = VectorSub(vertices[currentPosition], vertices[currentPosition - 1]);
			printf("\nZ (h.kol): ");
			printVec3(normalZ);
		}

		if(currentPosition < kTerrainSize)
		{
			//Övre raden
			normalX = VectorSub(vertices[currentPosition + kTerrainSize], vertices[currentPosition]);
			printf("\nX (upp): ");
			printVec3(normalX);
		} 

		if(currentPosition > (kTerrainSize*(kTerrainSize-1)))
		{
			//Undre raden
			normalX = VectorSub(vertices[currentPosition], vertices[currentPosition - kTerrainSize]);
			printf("\nX (ner): ");
			printVec3(normalX);
		}

		if (Norm(normalZ) < 0.1) 
		{
			normalZ = VectorSub(vertices[currentPosition + 1], vertices[currentPosition - 1]);
			printf("\nZ (vanlig): ");
			printVec3(normalZ);
		}

		if (Norm(normalX) < 0.1) 
		{
			normalX = VectorSub(vertices[currentPosition + kTerrainSize], vertices[currentPosition - kTerrainSize]);
			printf("\nX(vanlig): ");
			printVec3(normalX);
		}

		//normalX = Normalize(normalX);
		//normalZ = Normalize(normalZ);
		
		/*
		if (x == 1 && z == 1)
		{
			printf("X: ");
			printVec3(normalX);
			printf("Z: ");
			printVec3(normalZ);
			
		}
		*/

		normals[currentPosition] = CrossProduct(normalX, normalZ);
		printf("\nCross product: ");
		printVec3(normals[currentPosition]);
		printf("---------------------------------------------");
	}
}

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 300.0);

	// Load and compile shader
	texShader = loadShaders("textured.vert", "textured.frag");
	printError("init shader");

	// Upload geometry to the GPU:
	MakeTerrain();
	floormodel = LoadDataToModel(vertices, normals, texCoords, NULL,
			indices, kTerrainSize*kTerrainSize, (kTerrainSize-1)*(kTerrainSize-1)*2*3);

	printError("LoadDataToModel");

// Important! The shader we upload to must be active!
	glUseProgram(texShader);
	glUniformMatrix4fv(glGetUniformLocation(texShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUniform1i(glGetUniformLocation(texShader, "tex"), 0); // Texture unit 0

	LoadTGATextureSimple("grass.tga", &grasstex);
	glBindTexture(GL_TEXTURE_2D, grasstex);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,	GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,	GL_REPEAT);

	printError("init arrays");
}

vec3 campos = {kTerrainSize*kPolySize/4, 1.5, kTerrainSize*kPolySize/4};
vec3 forward = {8, 0, 8};
vec3 up = {0, 1, 0};

void display(void)
{
	int i, j;

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 worldToView, m;

	if (glutKeyIsDown('a'))
		forward = MultMat3Vec3(mat4tomat3(Ry(0.03)), forward);
	if (glutKeyIsDown('d'))
		forward = MultMat3Vec3(mat4tomat3(Ry(-0.03)), forward);
	if (glutKeyIsDown('w'))
		campos = VectorAdd(campos, ScalarMult(forward, 0.01));
	if (glutKeyIsDown('s'))
		campos = VectorSub(campos, ScalarMult(forward, 0.01));
	if (glutKeyIsDown('q'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		campos = VectorSub(campos, ScalarMult(side, 0.01));
	}
	if (glutKeyIsDown('e'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		campos = VectorAdd(campos, ScalarMult(side, 0.01));
	}

	// Move up/down
	if (glutKeyIsDown('z'))
		campos = VectorAdd(campos, ScalarMult(SetVector(0,1,0), 0.01));
	if (glutKeyIsDown('c'))
		campos = VectorSub(campos, ScalarMult(SetVector(0,1,0), 0.01));

	// NOTE: Looking up and down is done by making a side vector and rotation around arbitrary axis!
	if (glutKeyIsDown('+'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		mat4 m = ArbRotate(side, 0.05);
		forward = MultMat3Vec3(mat4tomat3(m), forward);
	}
	if (glutKeyIsDown('-'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		mat4 m = ArbRotate(side, -0.05);
		forward = MultMat3Vec3(mat4tomat3(m), forward);
	}

	worldToView = lookAtv(campos, VectorAdd(campos, forward), up);

	glBindTexture(GL_TEXTURE_2D, grasstex); // The texture is not used but provided as example
	// Floor
	glUseProgram(texShader);
	m = worldToView;
	glUniformMatrix4fv(glGetUniformLocation(texShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(floormodel, texShader, "inPosition", "inNormal", "inTexCoord");

	printError("display");

	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(840,560);
	glutCreateWindow ("Lab 3b");
	glutRepeatingTimer(20);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	init ();
	glutMainLoop();
}
