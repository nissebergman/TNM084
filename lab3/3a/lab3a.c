// Fractal tree generation

#include "../../common/Linux/MicroGlut.h"
#include "GL_utilities.h"
#include "VectorUtils3.h"
#include "LittleOBJLoader.h"
#include "LoadTGA.h"
#include "glugg.h"
#include <stdlib.h> 
#include <time.h>

// uses framework OpenGL
// uses framework Cocoa

void MakeCylinderAlt(int aSlices, float height, float topwidth, float bottomwidth)
{
	gluggMode(GLUGG_TRIANGLE_STRIP);
	vec3 top = SetVector(0,height,0);
	vec3 center = SetVector(0,0,0);
	vec3 bn = SetVector(0,-1,0); // Bottom normal
	vec3 tn = SetVector(0,1,0); // Top normal

	for (float a = 0.0; a < 2.0*M_PI+0.0001; a += 2.0*M_PI / aSlices)
	{
		float a1 = a;

		vec3 p1 = SetVector(topwidth * cos(a1), height, topwidth * sin(a1));
		vec3 p2 = SetVector(bottomwidth * cos(a1), 0, bottomwidth * sin(a1));
		vec3 pn = SetVector(cos(a1), 0, sin(a1));

// Done making points and normals. Now create polygons!
		gluggNormalv(pn);
	    gluggTexCoord(height, a1/M_PI);
	    gluggVertexv(p2);
	    gluggTexCoord(0, a1/M_PI);
	    gluggVertexv(p1);
	}

	// Then walk around the top and bottom with fans
	gluggMode(GLUGG_TRIANGLE_FAN);
	gluggNormalv(bn);
	gluggVertexv(center);
	// Walk around edge
	for (float a = 0.0; a <= 2.0*M_PI+0.001; a += 2.0*M_PI / aSlices)
	{
		vec3 p = SetVector(bottomwidth * cos(a), 0, bottomwidth * sin(a));
	    gluggVertexv(p);
	}
	// Walk around edge
	gluggMode(GLUGG_TRIANGLE_FAN); // Reset to new fan
	gluggNormalv(tn);
	gluggVertexv(top);
	for (float a = 2.0*M_PI; a >= -0.001; a -= 2.0*M_PI / aSlices)
	{
		vec3 p = SetVector(topwidth * cos(a), height, topwidth * sin(a));
	    gluggVertexv(p);
	}
}


mat4 projectionMatrix;

Model *floormodel;
GLuint grasstex, barktex;

// Reference to shader programs
GLuint phongShader, texShader;

// Floor quad
GLfloat vertices2[] = {	-20.5,0.0,-20.5,
						20.5,0.0,-20.5,
						20.5,0.0,20.5,
						-20.5,0.0,20.5};
GLfloat normals2[] = {	0,1.0,0,
						0,1.0,0,
						0,1.0,0,
						0,1.0,0};
GLfloat texcoord2[] = {	50.0f, 50.0f,
						0.0f, 50.0f,
						0.0f, 0.0f,
						50.0f, 0.0f};
GLuint indices2[] = {	0,3,2, 0,2,1};

// THIS IS WHERE YOUR WORK GOES!
/*
void MakeBranches() 
{
	srand(time(NULL));
	//int random = rand() % 0.7;
	//random = M_PI/4;
	float topwidth =  0.1;
	float bottomwidth = 0.15;
	float branchlength = 2.0f;

	//Branches
	gluggTranslate(0.0f, branchlength, 0.0f);
	gluggRotate(-random, 1.0f, 0.0f, -1.0f);
	MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
	gluggPushMatrix();

/*
	gluggRotate(random, 1.0f, 0.0f, 0.0f);
	MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
	gluggPushMatrix();

	gluggRotate(random, 1.0f, 0.0f, -1.0f);
	MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
	gluggPushMatrix();
	
}

void MakeLeaves() 
{
	float topwidth =  0.1;
	float bottomwidth = 0.15;
	float branchlength = 2.0f;

	for(int i = 0; i < 3; i++)
	{
		gluggPopMatrix();
		gluggTranslate(0.0f, branchlength, 0.0f);
		gluggRotate(-M_PI/4, .2f, 0.0f, -1.0f);
		MakeCylinderAlt(20, 1, topwidth, bottomwidth);

		gluggRotate(M_PI/4, .2f, 0.0f, 0.0f);
		MakeCylinderAlt(20, 1, topwidth, bottomwidth);

		gluggRotate(M_PI/4, .2f, 0.0f, -1.0f);
		MakeCylinderAlt(20, 1, topwidth, bottomwidth);
	}
}
*/

void recursiveTree(int depth, float topW, float botW, float len, float angle) {

	// Base Case
	if (depth == 0)
	{
		return;
	}
	else {
		//int random = rand() % 0.7;
		angle += 0.1;
		srand(time(NULL));
		angle = random;
		
		topW *= 0.6;
		botW *= 0.6;
		len *= 0.7;
		
		//Branches
		gluggPushMatrix();
		gluggTranslate(0.0f, len, 0.0f);
		gluggRotate(angle, 1.0f, 1.0f, -1.0f);
		MakeCylinderAlt(20, len, topW, botW);
		
		recursiveTree(depth-1, topW, botW, len, angle);
		gluggPopMatrix();

		//Branches
		gluggPushMatrix();
		gluggTranslate(0.0f, len, 0.0f);
		gluggRotate(2*angle, 1.0f, 1.0f, 1.0f);
		MakeCylinderAlt(20, len, topW, botW);
		
		recursiveTree(depth-1, topW, botW, len, angle);
		gluggPopMatrix();

		//Branches
		gluggPushMatrix();
		gluggTranslate(0.0f, len, 0.0f);
		gluggRotate(1.5*angle, -1.0f, 1.0f, -1.0f);
		MakeCylinderAlt(20, len, topW, botW);
		
		recursiveTree(depth-1, topW, botW, len, angle);
		gluggPopMatrix();
		
	}
}

GLuint MakeTree(int *count, GLuint program)
{
	gluggSetPositionName("inPosition");
	gluggSetNormalName("inNormal");
	gluggSetTexCoordName("inTexCoord");

	gluggBegin(GLUGG_TRIANGLES);

	// Between gluggBegin and gluggEnd, call MakeCylinderAlt plus glugg transformations
	// to create a tree.

	float topwidth =  0.1;
	float bottomwidth = 0.15;
	float branchlength = 2.0f;
	int height = 5;
	int currentHeight = 0;
	int counter = 0;
	float angle = 2*M_PI/3;

	
	//Stem
	MakeCylinderAlt(20, branchlength*0.75, topwidth, bottomwidth);
	gluggPushMatrix();

	recursiveTree(height, 0.1, 0.15, 2.0, angle);

/*
	MakeBranches();

	for(int i = 0; i <= height; i++)
	{
		MakeBranches();
		MakeLeaves();
		gluggPopMatrix();
		gluggPopMatrix();
		gluggPushMatrix();
	}
	*/
	
/*
	MakeBranches();
	MakeLeaves();
	gluggPopMatrix();
	gluggPopMatrix();
	gluggPushMatrix();

	MakeBranches();
	MakeLeaves();
	gluggPopMatrix();
	gluggPushMatrix();
*/

	

/*
	for(int i = 0; i <= 9; i++)
	{
		gluggTranslate(0.0f, branchlength, 0.0f);
		gluggRotate(-M_PI/4, 1.0f, 0.0f, -1.0f);
		MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);

		gluggRotate(M_PI/4, 1.0f, 0.0f, 0.0f);
		MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);

		gluggRotate(M_PI/4, 1.0f, 0.0f, -1.0f);
		MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
		gluggPopMatrix();
	}
/*
	for (int i = 0; i <= 8; i++)
	{
		height++;
		if (height == 3) 
		{
			gluggTranslate(0.0f, branchlength, 0.0f);
			gluggRotate(-M_PI/4, 0.0f, 0.0f, -1.0f);
			MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
			gluggRotate(M_PI/4, 0.0f, 0.0f, -1.0f);
			MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
			gluggRotate(M_PI/4, 0.0f, 0.0f, -1.0f);
			MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
			for (int i = 0; i < height; i++) {
				gluggPopMatrix();
				height--;
			}
		} else {
			gluggTranslate(0.0f, branchlength, 0.0f);
			gluggRotate(-M_PI/4, 0.0f, 0.0f, -1.0f);
			MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
			gluggPushMatrix();

			gluggRotate(M_PI/4, 0.0f, 0.0f, -1.0f);
			MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
			gluggPushMatrix();

			gluggRotate(M_PI/4, 0.0f, 0.0f, -1.0f);
			MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
			gluggPushMatrix();
		}
	}

/*
	gluggTranslate(0.0f, branchlength, 0.0f);
	gluggRotate(-M_PI/4, 0.0f, 0.0f, -1.0f);
	MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
	gluggPushMatrix();
	gluggRotate(M_PI/2, 0.0f, 0.0f, -1.0f);
	MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
	gluggPushMatrix();

	gluggTranslate(0.0f, branchlength, 0.0f);
	gluggRotate(-M_PI/4, 0.0f, 0.0f, -1.0f);
	MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
	gluggPushMatrix();
	gluggRotate(M_PI/2, 0.0f, 0.0f, -1.0f);
	MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
	gluggPushMatrix();
*/


	
	/*
	gluggTranslate(0.0f, branchlength, 0.0f);
	gluggRotate(2*M_PI/3, 0.0f, 0.0f, -1.0f);
	MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
	gluggPushMatrix();

	gluggTranslate(0.0f, branchlength, 0.0f);
	gluggRotate(2*M_PI/3, 0.0f, 0.0f, -1.0f);
	MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
	gluggPushMatrix();

	for(int i = 0; i < 3; i++)
	{
		gluggRotate(2*M_PI/3, 0.0f, 0.0f, -1.0f);
		MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
		gluggRotate(M_PI/3, 0.0f, 0.0f, -1.0f);
		MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
		gluggPopMatrix();
	}
	

/*
	for (int i = 0; i < height; i++)
	{
		topwidth *= 0.75f;
		bottomwidth *= 0.75f;
		branchlength *= 0.75f;

		gluggTranslate(0.0f, branchlength, 0.0f);
		for (int j = 0; j < 2; j++) {
			gluggRotate(j*M_PI/3, 0.0f, 0.0f, -1.0f);
			MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
			gluggPushMatrix();
		}
		gluggPopMatrix();
		gluggRotate(-2*M_PI/3, 0.0f, 0.0f, -1.0f);
		MakeCylinderAlt(20, branchlength, topwidth, bottomwidth);
		gluggPushMatrix();	
	}

*/

	return gluggEnd(count, program, 0);
}

GLuint tree;
int treecount;

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);

	// Set the clipping volume
	projectionMatrix = perspective(45,1.0f*w/h,1,1000);
	glUseProgram(phongShader);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUseProgram(texShader);
	glUniformMatrix4fv(glGetUniformLocation(texShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
}

void init(void)
{
	// GL inits
	glClearColor(0.2,0.2,0.5,0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	printError("GL inits");

	projectionMatrix = frustum(-0.1, 0.1, -0.1, 0.1, 0.2, 300.0);

	// Load and compile shader
	phongShader = loadShaders("phong.vert", "phong.frag");
	texShader = loadShaders("textured.vert", "textured.frag");
	printError("init shader");

	// Upload geometry to the GPU:
	floormodel = LoadDataToModel((vec3 *)vertices2, (vec3 *)normals2, (vec2 *)texcoord2, NULL,
			indices2, 4, 6);

// Important! The shader we upload to must be active!
	glUseProgram(phongShader);
	glUniformMatrix4fv(glGetUniformLocation(phongShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);
	glUseProgram(texShader);
	glUniformMatrix4fv(glGetUniformLocation(texShader, "projectionMatrix"), 1, GL_TRUE, projectionMatrix.m);

	glUniform1i(glGetUniformLocation(texShader, "tex"), 0); // Texture unit 0

	LoadTGATextureSimple("grass.tga", &grasstex);
	glBindTexture(GL_TEXTURE_2D, grasstex);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_S,	GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,	GL_TEXTURE_WRAP_T,	GL_REPEAT);

	LoadTGATextureSimple("bark2.tga", &barktex);

	tree = MakeTree(&treecount, texShader);

	printError("init arrays");
}

GLfloat a = 0.0;
vec3 campos = {0, 1.5, 10};
vec3 forward = {0, 0, -4};
vec3 up = {0, 1, 0};

void display(void)
{
	int i, j;

	printError("pre display");

	// clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 worldToView, m; // m1, m2, m, tr, scale;

	if (glutKeyIsDown('a'))
		forward = MultMat3Vec3(mat4tomat3(Ry(0.05)), forward);
	if (glutKeyIsDown('d'))
		forward = MultMat3Vec3(mat4tomat3(Ry(-0.05)), forward);
	if (glutKeyIsDown('w'))
		campos = VectorAdd(campos, ScalarMult(forward, 0.1));
	if (glutKeyIsDown('s'))
		campos = VectorSub(campos, ScalarMult(forward, 0.1));
	if (glutKeyIsDown('q'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		campos = VectorSub(campos, ScalarMult(side, 0.05));
	}
	if (glutKeyIsDown('e'))
	{
		vec3 side = CrossProduct(forward, SetVector(0,1,0));
		campos = VectorAdd(campos, ScalarMult(side, 0.05));
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

	a += 0.1;

	glBindTexture(GL_TEXTURE_2D, grasstex);
	// Floor
	glUseProgram(texShader);
	m = worldToView;
	glUniformMatrix4fv(glGetUniformLocation(texShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	DrawModel(floormodel, texShader, "inPosition", "inNormal", "inTexCoord");

	// Draw the tree, as defined on MakeTree
	glBindTexture(GL_TEXTURE_2D, barktex);
	glUseProgram(texShader);
    m = Mult(worldToView, T(0, 0, 0));
    glUniformMatrix4fv(glGetUniformLocation(texShader, "modelviewMatrix"), 1, GL_TRUE, m.m);
	glBindVertexArray(tree);	// Select VAO
	glDrawArrays(GL_TRIANGLES, 0, treecount);

	printError("display");

	glutSwapBuffers();
}

void keys(unsigned char key, int x, int y)
{
	switch (key)
	{
		case ' ':
			forward.y = 0;
			forward = ScalarMult(normalize(forward), 4.0);
			break;
	}
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitContextVersion(3, 2);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(840,560);
	glutCreateWindow ("Fractal tree lab");
	glutRepeatingTimer(20);
	glutDisplayFunc(display);
	glutKeyboardFunc(keys);
	glutReshapeFunc(reshape);
	init ();
	glutMainLoop();
}
