#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>

#include <iostream>
#include <iomanip>  //stuff like set precision
#include <fstream>   //file read write
#include <string>
#include <sstream>  //read and write to string


bool xtimesdiv, ytimesdiv, ztimesdiv;

/*
 * In  /usr/include/GL/  */

#include <GL/gl.h>
#include <GL/glu.h>
//#include <GL/glx.h> //need for bitmap fonts?

const char* WindowTitle="Plot the Globe";
bool picking=false;


#include <SDL/SDL.h>
#include "C:\myIncludez\XFILESwin32\D3Dvec.h"
#include "C:\myIncludez\XFILESwin32\Triangle.h"
#include "C:\myIncludez\XFILESwin32\throwcatch.h"

double speed=50.0, angspeed=0.05;


typedef unsigned char byte;
typedef unsigned short int USHORT;
typedef unsigned int UINT;
typedef unsigned int DWORD;  //yep, a DWORD is the same as a UINT...
                             //we use it for stuff that has DWORD
			     //or glEnum types.

//const USHORT WIDTH=800, HEIGHT=600;  
const USHORT WIDTH=1200, HEIGHT=600;  //use this for small screen
const USHORT SCREEN_DEPTH=16;

extern const double twopi,pi;  //initialised externally

/*********************************************************/
/*           for periodic boundary condition special case */
bool startoffx=true;
bool startoffy=true;
bool launch=true;
/*************************************************************/
bool periodic_xz=false;  //periodiv in PRESENTATION and in OpenGL coords
bool periodicbcs=false;  //actual periodic boundary conditions in xy
bool collisiontest=false;
bool foggy=false;
bool drawlines=true;
bool drawback=true;  // draw background when controls are up
bool PickTri=false;  //if false, we pick nodes and not triangles
bool Gouraud=false;  //smooth Gouraud shading
bool Picking=false;  //don't pick anyithing if false
/********************MODES***********************************/
bool twodee=false;  // camera moves in xz only

                      //One, but ONE ONLY  of these three
		      //MUST be true
bool freedom=false;      // if true viewer is free to move anywhere
bool tethered=true;   //  if true viewer is attached to fixed point by a boom
bool staked=false;    //   if true viewer is not free to move, but can look about

bool wander=false;    //one of these must be set true if free is true
bool flight=false;

bool aeroplane=false;   // if true we have aeroplane roll - pitch control

                        
bool boom=true;      //  needed for tethered only 
                      //  if boom true  make exwhyzed false
bool exwhyzed=false;   //  if tethered make boom false if exwhyzed is true
                      //  if wander must always be true
		      //  if flight can be true or false
		      //  make false if aeroplane true
		      //
bool hidemousecursor=true;      

/*************************************************************/
int iside, ihalfside;   // iside is the number of segments per side
             //not the number of nodes!
double xlength, xside, scalefac;
int ix, iz;
int ixold, izold; 
int diffx, diffz;
int diffoldx, diffoldz;
int yshiftedge, xshiftedge;
double **xshift, **yshift;
int **index1, **index2;

void SetShiftX(int);
void SetShiftY(int);

/**********************************************************************/
D3Dvec *NodeV;
Triangle* Triangles;
D3Dvec* edges1;
D3Dvec* edges2;
D3Dvec* Normals;
D3Dvec* NormalsG;
#include "C:\myIncludez\XFILESwin32\List.h"
#include "C:\myIncludez\XFILESwin32\Node1.h"
typedef CMyNode1<int>  ListNode;
typedef CMyNode1<char>  ListNode_char;
typedef CMyLinkedList<ListNode> intList;
typedef CMyLinkedList<ListNode_char> charList;
int const MD=4;  //MAXIMUM DEPTH OF TREE (depth ranges from 0 to MD
    intList** CollideTri; 
    int nlists;
    intList*  NodeList;  
    charList*  WhichList;  
//intList hangs is attached to a tree branch, iNodeList is attatched to a node.



#include "C:\myIncludez\XFILESwin32\TriOctNode.h"
#include "C:\myIncludez\XFILESwin32\TriOctTree.h"
typedef CMyOctNode <D3Dvec> ONode; //An ONode shall hold a vector
                                   //describing the compare point
				   // and a list of integers
				   // (Triangle array indexes)
typedef CMyTriOctTree<ONode> OTree;   //OctTree consists of nodes
        OTree* CollideTree;
/**********************************************************************/
#include "C:\myIncludez\XFILESwin32\Dmatrix.h"
//#include "C:\myIncludez\XFILESwin32\Solver.h"
/**********************************************************************/
//void Solver(int &dim, double &maxerr, Dmatrix& Matrix, Dvector& rhs, Dvector& SolveVec);
int myGaussElim(int& dimen , double** Matr , double* VexRhs, double* VexLhs );
/**********************************************************************/

D3Dvec RealCamPos;  //periodic boundary conditions
                    //CamPos thinks it  wanders in infinite space
		    //But RealCamPos knows about the pbc.
double X1, X2, Y1, Y2, Z1, Z2;
#include "C:\myIncludez\XFILESwin32\Camera.h"
#include "C:\myIncludez\XFILESwin32\gl2ps.h"

extern int VideoFlags;
extern SDL_Surface * MainWindow;

extern double speed;
extern const double convert;


// Prototypes for functions
//void EventLoop(CCam  & Camera1);
void EventLoop();
void RenderScene(CCam  & Camera1);
void PickObjects();
void ProcessHitsLP(int, UINT *);
void ProcessHitsLC(int, UINT *);
void ProcessHitsLB(int, UINT *);
void ProcessHitsMC(int, UINT *);
void ProcessHitsDT(int, UINT *);
void ProcessHitsPC1(int, UINT *);
void ProcessHitsLeg(int, UINT *);
void ProcessTriangleHits(int, UINT *);
void ProcessNodeHits(int, UINT *);
void SetUp();
void ToggleWindow(void);
void MakeWindow(const char *Name, 
		const USHORT WIDTH,
	        const USHORT HEIGHT,
		 int VideoFlags);
void SizeOpenGLScreen(const USHORT, const USHORT);
void Init();
void InitialiseGL(int, int);
void HandleKeyPress(SDL_keysym * );
void HandleKeyRelease(SDL_keysym *);
void CheckMove(CCam &);
int Screenshot(SDL_Surface *screen, char *filename);

void CreateTexture(unsigned int textureArray[], char *strFilename, int textureID);

void OrthoMode(double , double , double, double);
void PerspectiveMode();
void DrawLightPos(bool);
void DrawLightCol(bool);
void DrawMatCol(bool);
void DrawLogicBox(bool);
void DrawTextMove(bool);
void DrawCurveCol(bool);
void DrawLegend(bool);

extern UINT base;
//bitmap font functions
void buildFont(void); 
void QuitFont(int );    
void KillFont();
void glPrint( char *);

void SetShiftX(int);
void SetShiftY(int);

void DrawScene(bool);


int arg_count;
char **arg_list;

#include "C:\myIncludez\XFILESwin32\Init.h"
int nplots;   
const int nplotsmax=40;
int *ndata, *ntype, *ncol, *nstyle, *npoint;
// ntype=1 for line 0 for point, npoint is the line thickness or pointsize

double **x;  //the data
double **y;

double xmin,xmax,xrange;
double ymin,ymax,yrange;

double xexp,yexp;
double xfact,yfact;
double xrange_scale, yrange_scale;
bool xmin_zero=false, ymin_zero=false, xmax_zero=false, ymax_zero=false;
bool xboth=false, yboth=false;

double xminexp,yminexp;
double xminfact,yminfact;
double xmin_scale,ymin_scale;

bool  xaddsubtract=false, yaddsubtract=false;

int ixnumb, iynumb;  //number of major tick marks and axis numbers
int ixnumb2,iynumb2;  //number of minor tickmarks

double xminshift,xstep,xstep_scale,x_start,xsubtract;  //different to xstart in SetUp.h
double yminshift,ystep,ystep_scale,y_start,ysubtract;
double x_start2, y_start2,xstep2,ystep2; //minor tickmarks

double xnumbers[13], ynumbers[13];
double xnumbers2[130], ynumbers2[130]; //for minor ticks algorithm
string xstrings[13], ystrings[13];

//keep 2d bounding box the same
//Camera is at  (0,0,3000) OpenGL coords
double xlow=-1200.0,ylow=-800.,xhigh=800.0,yhigh=800,zed=0.0;

int smallticksx,smallticksy;

string xtext,xunits,xaxistext,ytext,yunits,yaxistext;
int ixtext,ixunits,iytext,iyunits;

double shift1;  // shift all x axis numbers left
double shift2;  // shift all y axis numbers down
double shift3;  // shift upper x numbers  down
double shift4;  // shift lower x numbers  down
double shift5;  // shift left y numbers  left
double shift6;  // shift right y numbers  left
double shift7;  // shift x axis text left
double shift8;  // shift x axis text down
double shift9; // shift y axis text left
double shift10; // shift y axis text down

bool LegendExists;
string *LegendStrings;

/*    This is now shifted to Converter
vector <double> **WorldLat; vector <double> **WorldLong; //pointer to vector
vector <int> linelong; */
double **WorldLat; double **WorldLong;
int *linelong;
int nlines;


#include "C:\myIncludez\XFILESwin32\numbers_xy.h"


