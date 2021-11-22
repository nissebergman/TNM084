# set this variable to the director in which you saved the common files
commondir = ../../common/

all : lab3a

lab3a : lab3a.c $(commondir)GL_utilities.c $(commondir)VectorUtils3.c $(commondir)LittleOBJLoader.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c
	gcc -Wall -o lab3a -I$(commondir) -Iglugg glugg.c -I../common/Linux -DGL_GLEXT_PROTOTYPES lab3a.c $(commondir)GL_utilities.c $(commondir)LittleOBJLoader.c $(commondir)VectorUtils3.c $(commondir)LoadTGA.c $(commondir)Linux/MicroGlut.c -lXt -lX11 -lGL -lm

clean :
	rm lab3a

#-Inoise noise/noise1234.c not needed for lab3a