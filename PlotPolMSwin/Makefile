TARGET		=PlotPol

$(TARGET):	PlotPol.o   gl2ps.o 
	g++ -g -o $(TARGET) PlotPol.o gl2ps.o \
		-LC:/MinGW/lib  -lmingw32  -lSDLmain -lSDL \
                -lOGLFT -lfreetype -lgle -lpng -lz -lglu32 -lopengl32 -lseagods -lgfortran -mwindows
	
PlotPol.o:
	g++ -g  -c  PlotPol.cpp -IC:/MinGW/include/SDL  -D_GNU_SOURCE=1 -Dmain=SDL_main \
                -IC:/MinGW/include/oglft
                

gl2ps.o:
	gcc -g -c gl2ps.c 

clean:
	rm -f $(TARGET) *.o  core
