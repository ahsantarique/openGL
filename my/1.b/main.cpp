#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<math.h>

#include <windows.h>
#include <glut.h>

#define pi (2*acos(0.0))

using namespace std;


double cameraHeight;
double cameraAngle;
int drawgrid;
int drawaxes;
double theta;
double radius;
double dist;
struct point
{
    double x,y,z;

    point(){

    }
    point(const point& another){
        x = another.x;
        y = another.y;
        z = another.z;
    }
    point(double a, double b, double c){
        x=a;
        y=b;
        z=c;
    }
    point operator +(const point &b){
        point another(x+b.x, y+b.y, z+b.z);
        return another;
    }
    point operator -(const point &b){
        point another(x-b.x, y-b.y, z-b.z);
        return another;
    }

    point operator *(int a){
        point another(x*a, y*a, z*a);
        return another;
    }

    point operator =(const point & another){
        x = another.x;
        y = another.y;
        z = another.z;
    }
};

point pos(200, 200, 0), u(0, 0, 1), r(-1/sqrt(2), 1/sqrt(2), 0), l(-1/sqrt(2), -1/sqrt(2), 0);


void drawAxes()
{
    if(drawaxes==1)
    {
        glColor3f(1.0, 0, 0);
        glBegin(GL_LINES);
        {
            glVertex3f( 300,0,0);
            glVertex3f(-300,0,0);
        }
        glEnd();

        glColor3f(0, 1.0, 0);
        glBegin(GL_LINES);
        {
            glVertex3f( 0,300,0);
            glVertex3f(0,-300,0);
        }
        glEnd();
        glColor3f(0, 0, 1.0);
        glBegin(GL_LINES);
        {
            glVertex3f(0,0, 300);
            glVertex3f(0,0,-300);
        }
        glEnd();

    }
}


void drawGrid()
{
    int i;
    if(drawgrid==1)
    {
        glColor3f(0.6, 0.6, 0.6);	//grey
        glBegin(GL_LINES);
        {
            for(i=-8; i<=8; i++)
            {

                if(i==0)
                    continue;	//SKIP the MAIN axes

                //lines parallel to Y-axis
                glVertex3f(i*10, -90, 0);
                glVertex3f(i*10,  90, 0);

                //lines parallel to X-axis
                glVertex3f(-90, i*10, 0);
                glVertex3f( 90, i*10, 0);
            }
        }
        glEnd();
    }
}

void drawSquare(double a)
{
    //glColor3f(1.0,0.0,0.0);
    glBegin(GL_QUADS);
    {
        glVertex3f( a, a,2);
        glVertex3f( a,-a,2);
        glVertex3f(-a,-a,2);
        glVertex3f(-a, a,2);
    }
    glEnd();
}


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0; i<segments; i++)
    {
        glBegin(GL_LINES);
        {
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCone(double radius,double height,int segments)
{
    int i;
    double shade;
    struct point points[100];
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw triangles using generated points
    for(i=0; i<segments; i++)
    {
        //create shading effect
        if(i<segments/2)shade=2*(double)i/(double)segments;
        else shade=2*(1.0-(double)i/(double)segments);
        glColor3f(shade,shade,shade);

        glBegin(GL_TRIANGLES);
        {
            glVertex3f(0,0,height);
            glVertex3f(points[i].x,points[i].y,0);
            glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}


void drawSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*2*pi);
            points[i][j].y=r*sin(((double)j/(double)slices)*2*pi);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        glColor3f((double)i/(double)stacks,0,0);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }
            glEnd();
        }
    }
}

void drawEighthSphere(double radius,int slices,int stacks)
{
    struct point points[100][100];
    int i,j;
    double h,r;
    //generate points
    for(i=0; i<=stacks; i++)
    {
        h=radius*sin(((double)i/(double)stacks)*(pi/2));
        r=radius*cos(((double)i/(double)stacks)*(pi/2));
        for(j=0; j<=slices; j++)
        {
            points[i][j].x=r*cos(((double)j/(double)slices)*pi/2);
            points[i][j].y=r*sin(((double)j/(double)slices)*pi/2);
            points[i][j].z=h;
        }
    }
    //draw quads using generated points
    for(i=0; i<stacks; i++)
    {
        //glColor3f((double)i/(double)stacks,0,0);
        glColor3f(1,0,0);
        for(j=0; j<slices; j++)
        {
            glBegin(GL_QUADS);
            {
                //upper hemisphere
                glVertex3f(points[i][j].x,points[i][j].y,points[i][j].z);
                glVertex3f(points[i][j+1].x,points[i][j+1].y,points[i][j+1].z);
                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,points[i+1][j+1].z);
                glVertex3f(points[i+1][j].x,points[i+1][j].y,points[i+1][j].z);
                //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
            }
            glEnd();
        }
    }
}

void drawQuarterCylinder(double radius, double height, int segments)
{
    int i;
    struct point points[100];
    glColor3f(0,1,0);
    //generate points
    for(i=0; i<=segments; i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*pi/2);
        points[i].y=radius*sin(((double)i/(double)segments)*pi/2);
    }
    //draw segments using generated points



    for(i=0; i<segments; i++)
    {
        glColor3f(0,(double)i/(double)segments,0);
        glBegin(GL_QUADS);
        {
            //upper hemisphere
            glVertex3f(points[i].x,points[i].y,-height/2);
            glVertex3f(points[i].x,points[i].y,height/2);
            glVertex3f(points[i+1].x,points[i+1].y,height/2);
            glVertex3f(points[i+1].x,points[i+1].y,-height/2);


            //lower hemisphere
//                glVertex3f(points[i][j].x,points[i][j].y,-points[i][j].z);
//                glVertex3f(points[i][j+1].x,points[i][j+1].y,-points[i][j+1].z);
//                glVertex3f(points[i+1][j+1].x,points[i+1][j+1].y,-points[i+1][j+1].z);
//                glVertex3f(points[i+1][j].x,points[i+1][j].y,-points[i+1][j].z);
        }
        glEnd();
    }

    }



void tRSphere(double angle){
    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(dist,dist,dist);
        radius = (80-dist);
        drawEighthSphere(radius,50,50);
    }
    glPopMatrix();
}


void tRCylinder(double angle){
    glPushMatrix();
    {
        glRotatef(angle,0,0,1);
        glTranslatef(dist,dist,0);
        drawQuarterCylinder(radius,2*(80-radius),50);
    }glPopMatrix();
}

void tRQuad(double angle){
    glPushMatrix();
    {
        glRotatef(angle,0,1,0);
        glTranslatef(0,0,-80);
        glColor3f(1,1,1);
        double x = 80-radius;
        glBegin(GL_QUADS);
        {
            glVertex3f(x, x, 0);
            glVertex3f(x, -x, 0);
            glVertex3f(-x,-x,0);
            glVertex3f(-x, x, 0);
        }
        glEnd();

    }
    glPopMatrix();
}
void drawSS()
{

    //glColor3f(1,0,0);
    glPushMatrix();
    {
        tRSphere(0);
        tRSphere(90);
        tRSphere(180);
        tRSphere(-90);
        glPushMatrix();
        {
            glRotatef(180,0,1,0);
            tRSphere(0);
            tRSphere(90);
            tRSphere(180);
            tRSphere(-90);
        }
        glPopMatrix();
    }
    glPopMatrix();          // eighth of a sphere

    glPushMatrix();
    {
        tRCylinder(0);
        tRCylinder(90);
        tRCylinder(180);
        tRCylinder(-90);
        glPushMatrix();
        {
            glRotatef(90,0,1,0);
            tRCylinder(0);
            tRCylinder(90);
            tRCylinder(180);
            tRCylinder(-90);
        }
        glPopMatrix();
        glPushMatrix();
        {
            glRotatef(90,1,0,0);
            tRCylinder(0);
            tRCylinder(90);
            tRCylinder(180);
            tRCylinder(-90);
        }
        glPopMatrix();
    }
    glPopMatrix();          // quarter of cylinder


     glPushMatrix();
     {
        tRQuad(0);//quad
        tRQuad(90);
        tRQuad(180);
        tRQuad(-90);
        glPushMatrix();
        {
            glRotatef(90,1,0,0);
            tRQuad(0);//quad
            tRQuad(90);
            tRQuad(180);
            tRQuad(-90);
        }
        glPopMatrix();
     }
     glPopMatrix();

//    drawSquare(20);
//
//    glRotatef(angle,0,0,1);
//    glTranslatef(110,0,0);
//    glRotatef(2*angle,0,0,1);
//    glColor3f(0,1,0);
//    drawSquare(15);
//
//
//    glRotatef(3*angle,0,0,1);
//    glTranslatef(40,0,0);
//    glRotatef(4*angle,0,0,1);
//    glColor3f(1,1,0);
//    drawSquare(5);
}


double temp;
point another;
void keyboardListener(unsigned char key, int x,int y)
{
    switch(key)
    {

    case '1':
        //drawgrid=1-drawgrid;
        //no change to u
        temp = l.x*cos(theta)+r.x*sin(theta);
        r.x = r.x*cos(theta)-l.x*sin(theta);
        l.x = temp;

        temp = l.y*cos(theta)+r.y*sin(theta);
        r.y = r.y*cos(theta)-l.y*sin(theta);
        l.y = temp;

        temp = l.z*cos(theta)+r.z*sin(theta);
        r.z = r.z*cos(theta)-l.z*sin(theta);
        l.z = temp;

        break;

    case '2':
        //no change to u

        temp = l.x*cos(-theta)+r.x*sin(-theta);
        r.x = r.x*cos(-theta)-l.x*sin(-theta);
        l.x = temp;
        temp = l.y*cos(-theta)+r.y*sin(-theta);
        r.y = r.y*cos(-theta)-l.y*sin(-theta);
        l.y = temp;
        temp = l.z*cos(-theta)+r.z*sin(-theta);
        r.z = r.z*cos(-theta)-l.z*sin(-theta);
        l.z = temp;
        break;

    case '3':
        //no change to r
        temp = l.x*cos(theta)+u.x*sin(theta);
        u.x =  u.x*cos(theta)-l.x*sin(theta);
        l.x = temp;
        temp = l.y*cos(theta)+u.y*sin(theta);
        u.y = u.y*cos(theta)-l.y*sin(theta);
        l.y = temp;
        temp = l.z*cos(theta)+u.z*sin(theta);
        u.z = u.z*cos(theta)-l.z*sin(theta);
        l.z = temp;
        break;

    case '4':
        //no change to r
        temp = l.x*cos(-theta)+u.x*sin(-theta);
        u.x =  u.x*cos(-theta)-l.x*sin(-theta);
        l.x = temp;
        temp = l.y*cos(-theta)+u.y*sin(-theta);
        u.y = u.y*cos(-theta)-l.y*sin(-theta);
        l.y = temp;
        temp = l.z*cos(-theta)+u.z*sin(-theta);
        u.z = u.z*cos(-theta)-l.z*sin(-theta);
        l.z = temp;
        break;

    case '5':
        //no change to l
        temp = r.x*cos(-theta)+u.x*sin(-theta);
        u.x =  u.x*cos(-theta)-r.x*sin(-theta);
        r.x = temp;
        temp = r.y*cos(-theta)+u.y*sin(-theta);
        u.y = u.y*cos(-theta)-r.y*sin(-theta);
        r.y = temp;
        temp = r.z*cos(-theta)+u.z*sin(-theta);
        u.z = u.z*cos(-theta)-r.z*sin(-theta);
        r.z = temp;
        break;

    case '6':
        //no change to r
        temp = r.x*cos(theta)+u.x*sin(theta);
        u.x =  u.x*cos(theta)-r.x*sin(theta);
        r.x = temp;
        temp = r.y*cos(theta)+u.y*sin(theta);
        u.y = u.y*cos(theta)-r.y*sin(theta);
        r.y = temp;
        temp = r.z*cos(theta)+u.z*sin(theta);
        u.z = u.z*cos(theta)-r.z*sin(theta);
        r.z = temp;
        break;

    default:
        break;
    }
}


void specialKeyListener(int key, int x,int y)
{
    switch(key)
    {
    case GLUT_KEY_DOWN:		//down arrow key
        //cameraHeight -= 3.0;
        pos = pos-l*5;
        break;
    case GLUT_KEY_UP:		// up arrow key
        //cameraHeight += 3.0;
        pos = pos+l*5;
        break;

    case GLUT_KEY_RIGHT:
        //cameraAngle += 0.03;
        pos = pos+r*5;
        break;
    case GLUT_KEY_LEFT:
        //cameraAngle -= 0.03;
        pos = pos-r*5;
        break;

    case GLUT_KEY_PAGE_UP:
        pos = pos+u*5;
        break;
    case GLUT_KEY_PAGE_DOWN:
        pos = pos-u*10;
        break;

    case GLUT_KEY_INSERT:
        break;

    case GLUT_KEY_HOME:
        if(dist>0) dist--;
        break;
    case GLUT_KEY_END:
        if(dist<79) dist++;
        break;

    default:
        break;
    }
}


void mouseListener(int button, int state, int x, int y) 	//x, y is the x-y of the screen (2D)
{
    switch(button)
    {
    case GLUT_LEFT_BUTTON:
        if(state == GLUT_DOWN) 		// 2 times?? in ONE click? -- solution is checking DOWN or UP
        {
            drawaxes=1-drawaxes;
        }
        break;

    case GLUT_RIGHT_BUTTON:
        //........
        break;

    case GLUT_MIDDLE_BUTTON:
        //........
        break;

    default:
        break;
    }
}



void display()
{

    //clear the display
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0,0,0,0);	//color black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /********************
    / set-up camera here
    ********************/
    //load the correct matrix -- MODEL-VIEW matrix
    glMatrixMode(GL_MODELVIEW);

    //initialize the matrix
    glLoadIdentity();

    //now give three info
    //1. where is the camera (viewer)?
    //2. where is the camera looking?
    //3. Which direction is the camera's UP direction?

    //gluLookAt(100,100,100,	0,0,0,	0,0,1);
    //gluLookAt(200*cos(cameraAngle), 200*sin(cameraAngle), cameraHeight,		0,0,0,		0,0,1);
    gluLookAt(pos.x, pos.y, pos.z,  pos.x + l.x, pos.y + l.y, pos.z + l.z,  u.x, u.y, u.z);

    //again select MODEL-VIEW
    glMatrixMode(GL_MODELVIEW);


    /****************************
    / Add your objects from here
    ****************************/
    //add objects

    drawAxes();
    drawGrid();

    //glColor3f(1,0,0);
    //drawSquare(10);

    drawSS();

    //drawCircle(30,24);

    //drawCone(20,50,24);

    //drawSphere(30,24,20);




    //ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
    glutSwapBuffers();
}


void animate()
{
    //codes for any changes in Models, Camera
    glutPostRedisplay();
}

void init()
{
    //codes for initialization
    drawgrid=0;
    drawaxes=1;
    cameraHeight=150.0;
    cameraAngle=1.0;
    theta=3*pi/180;
    radius = 0;
    dist = 50;
    //clear the screen
    glClearColor(0,0,0,0);

    /************************
    / set-up projection here
    ************************/
    //load the PROJECTION matrix
    glMatrixMode(GL_PROJECTION);

    //initialize the matrix
    glLoadIdentity();

    //give PERSPECTIVE parameters
    gluPerspective(80,	1,	1,	1000.0);
    //field of view in the Y (vertically)
    //aspect ratio that determines the field of view in the X direction (horizontally)
    //near distance
    //far distance
}

int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(0, 0);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

    glutCreateWindow("My OpenGL Program");

    init();

    glEnable(GL_DEPTH_TEST);	//enable Depth Testing

    glutDisplayFunc(display);	//display callback function
    glutIdleFunc(animate);		//what you want to do in the idle time (when no drawing is occuring)

    glutKeyboardFunc(keyboardListener);
    glutSpecialFunc(specialKeyListener);
    glutMouseFunc(mouseListener);

    glutMainLoop();		//The main loop of OpenGL

    return 0;
}
