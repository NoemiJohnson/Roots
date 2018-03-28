LIB = -lGL -lGLU -lglut
CC = g++ -Wall -Wno-deprecated-declarations
	
NJ_AngryBrick: NJ_AngryBrick.cpp 
	$(CC) -o NJ_AngryBrick NJ_AngryBrick.cpp $(LIB)
