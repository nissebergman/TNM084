# set this variable to the director in which you saved the common files
commondir = ../../common/

all : lab3b

lab3b : lab3b.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab3b -I$(commondir) -Inoise noise/simplexnoise1234.c -I../common/Linux -DGL_GLEXT_PROTOTYPES lab3b.c $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm lab3b

