#include "Phyzical Simulations.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>


long GetTickCount()
{
  struct timespec now;
  if (clock_gettime(CLOCK_MONOTONIC, &now))
    return 0;
  return now.tv_nsec / 1000000.0;
};

long ElapsedTime() { return GetTickCount()-StartTime; } //  от старта программы

static Course Configuration()
{
    return Course::_Esc;
}

void displayGlut()
{
    display();
}

bool display()
{
    auto start = std::chrono::system_clock::now();
    glClearColor( expMoving?0.6:0.8,0.9,0.9,0.9 );
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );

	//  прорисовка корпускул со следами траекторий, и стенок ящика [XY] ограничений
	//
    if( !Fly.nc )
    {
        if( expMoving )
        {
            Real angle = (ElapsedTime()/120)%360;
            glRotated( angle,angle/360,angle/180,angle/90 );  // проворот всей сцены
            angle *= M_PI/180.0;                              // градусы -> в радианы
            Mxl += (Vector){sin(angle),cos(angle),0 }/60;    // плоское перемещение
        }
        Model_Draw( MTyp );
    }
    else
    {
        for( int j=0; j<Fly.nc; j++ )
        {
            Ball &B=Fly.Dip[j];
            Mxl.set( B.M );
            Point R=B.R;
            Mxl=R;
            Model_Draw( MTyp );
            if( Fly.lt>0 )
            {
                glBegin( GL_LINE_STRIP );
                point( R,purple );
                for( int k=0; k<Fly.lt; k++ )
                    point(R+=B.dR[(k+Fly.kt)%Fly.lt]);
                glEnd();
            }
        }                             //  стенки XZ-ящика ограничений на случай
        if( Gravity_mode )              // запуска сил гравитационного притяжения
        {
            color( lightcyan,0.75,0.5 );
            glBegin( GL_QUAD_STRIP );
            glVertex3d( -wall,-wall,0 ),glVertex3d( -wall,-wall,wall/2 );
            glVertex3d( -wall, wall,0 ),glVertex3d( -wall, wall,wall/2 );
            glVertex3d(  wall, wall,0 ),glVertex3d(  wall, wall,wall/2 );
            glVertex3d(  wall,-wall,0 ),glVertex3d(  wall,-wall,wall/2 );
            glVertex3d( -wall,-wall,0 ),glVertex3d( -wall,-wall,wall/2 );
            glEnd();
        }
    }
    axis( Screen_Size/2,Screen_Size/2,Screen_Size/2,"X","Y","Z" );

    glutSwapBuffers();
    return true;
}

void timer_drawingGlut()
{
    Ball *balls = Fly.Dip;

    Vector *dR = new Vector[Fly.nc];
    Inter_And_Flight_Step(Fly,!linear_Model, dR);
    delete [] dR;
}

void TypeFigureSwitch(byte Typ, byte MTyp)
{
    // Переключение типа фигуры
    if( Typ!=MTyp )
    {
        if( Typ!=255 )
            MTyp=Model_Type( Typ );
        for( int i=0; i<Fly.nc; i++ )
            Fly.Dip[i].M=Particle_volume;
    }
}

void processSpecialKeysGlut(int key, int x, int y) {
    byte Typ=MTyp;
	switch(key) {
		case GLUT_KEY_F1:
				 break;
		case GLUT_KEY_F2:
				break;
		case GLUT_KEY_F3:
				break;
        case GLUT_KEY_INSERT:
        {
            Point R = { 5,4,3};
            Vector V = { -5,-4,-3};
            int mod = glutGetModifiers();
            if (mod == GLUT_ACTIVE_SHIFT)
            {
                R.X=-R.X;
                R.Y=-R.Y;
                R.Z=-R.Z;
                V=-V;
            }
            Fly.Add( R,(Vector){Particle_volume,0,0},V );
            break;
        }
        case GLUT_KEY_PAGE_UP:
            Particle_volume*=1.1;
            Typ=255;
            break;
        case GLUT_KEY_PAGE_DOWN:
            Particle_volume/=1.1;
            Typ=255;
            break;
    }

    TypeFigureSwitch(Typ, MTyp);
}


void keyboardGlut(unsigned char key,int x, int y)
{
    byte Typ=MTyp;
    switch (key)
    {
    case 'V':
        ks+=0.2;
        break;
    case 'v':
        ks-=0.2;
        break; // трение о воздух
    case _Enter:
        if( !Fly.nc )
            expMoving^=true;         // вольное движение
        else
            Gravity_mode^=true;
        break;
    case _Tab:
        linear_Model^=true;
        break;           // переключение типа частиц

    case 27:
        exit(0);
    case 'N':
    case 'n':
        Typ=NoneNodel;
        break;
    case 'K':
    case 'k':
        Typ=Cube;
        break;
    case 'T':
    case 't':
        Typ=Torus;
        break;
    case 'S':
    case 's':
        Typ=Sphere;
        break;
    case 'D':
    case 'd':
        Typ=Dipole;
        break;
    case ' ':
    {
        int mod = glutGetModifiers();
        if (mod == GLUT_ACTIVE_CTRL)
            ++Typ%=(Dipole+1);
        else   // закраска граней модели
            if(mod == GLUT_ACTIVE_SHIFT)
                Typ=!Typ?Dipole:Typ-1;
            else
                painting^=true;
        Draw(Width, Height);
        glutPostRedisplay();  // Redraw windows
        break;
    }
    default:
        if( key>='0' && key<='9' || key=='-' || key=='=' || key == 'q' || key == 'w')
        {
            if( Fly.nc )
            {
                Fly.Clear();
                (Point)Mxl=(Point)
                {
                    0,0,0
                };
            }
            else
            {
                if (key == 'w')
                    Fly.Create(42875, Fly_Count);
                else
                if (key == 'q')
                    Fly.Create(3375, Fly_Count);
                else
                    Fly.Create( key=='='?729                       //  остановка кубика
                            : ( key=='-'?125                       //  признак запуска
                                : ( key=='0'?27:key-'0')),Fly_Count ); //  на выходе размер
                Mxl=Particle_volume=Init_Flight_Points(Fly); //на старте частицы сжаты
                for( int i=0; i<Fly.nc; i++ )
                    Fly.Dip[i].M=Particle_volume;
            }
            Typ=255;  
        }
    }

    // Переключение типа фигуры
    TypeFigureSwitch(Typ, MTyp);
}


void timf(int value) // Timer function
{
  timer_drawingGlut();

  glutTimerFunc(1, timf, 0); // Setup next timer
  glutPostRedisplay();  // Redraw windows
}

void tim() // Timer function
{
  timer_drawingGlut();
  glutPostRedisplay();  // Redraw windows

}

void mouseButton(int button, int state, int x, int y) {
    static int mx = 0;
    static int my = 0;

    // Wheel reports as button 3(scroll up) and button 4(scroll down)
    if (button == 3) // It's a wheel event
    {
        // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
       if (state == GLUT_UP)
            return; // Disregard redundant GLUT_UP events
       else
       {
            // normal button event
            Distance += y*Distance/Height/3;
            // look.x += x*Distance/Width; mx-=x;
            Draw(Width,Height);
            glutPostRedisplay();
       }
    }
    else if (button == 4) // It's a wheel event
    {
        // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
       if (state == GLUT_UP)
            return; // Disregard redundant GLUT_UP events
       else
       {
            // normal button event
            Distance -= y*Distance/Height/3;
            Draw(Width,Height);
            glutPostRedisplay();
       }
    }
    else if( (x!=mx || y!=my) )    // местоположение курсора в локальных смещениях
    {
        switch( button )             //  -- и не пустой вызов без свободной реакции
        {
            case GLUT_LEFT_BUTTON:
                {
                    int mod = glutGetModifiers();
                    if (mod == GLUT_ACTIVE_CTRL)
                        eye.z+=Real( x-mx )/24,Distance-=( y-my )*Distance/Height;
                    else
                        eye.y-=Real( y-my )/24,eye.x+=Real( x-mx )/16;
                    Draw(Width,Height);
                    glutPostRedisplay();
                    break;
                }
            case GLUT_RIGHT_BUTTON:
                {
                    look.x-=( x-mx )*Distance/Width,       // смещение
                    look.y-=( y-my )*Distance/Width;
                    Draw(Width,Height);
                    glutPostRedisplay();
                    break;
                }
        }
        my=y,mx=x;
    }
}

void Draw(int Widthl, int Heightl)
{
  glMatrixMode( GL_PROJECTION );     // размерах фрагмента экрана сброс текущих
  glLoadIdentity();                  // матричных координат, настройка обзора и
  gluPerspective( 16.2,Real( Widthl )/Heightl,-1,1 );     // экранных пропорций
  gluLookAt( 0,0,Distance,look.x,look.y,look.z,0,1,0 ); // местоположение сцены
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity();       // в исходное положение
  glRotated( eye.y-90,1,0,0 );                          // поставить на киль
  glRotated( eye.z,   0,1,0 );                          // дифферент
  glRotated( eye.x,   0,0,1 );                          // рыскание
}

void init(void)
{
    Width = 500;
    Height = 500;
    eye = (Vector){ -130,-10,0 };
    look = (Vector){ 0,0,30 } ;
}

void reshape(int w, int h)
{
    Width = w;
    Height = h;
    glViewport(0,0,(GLsizei) w, (GLsizei) h);
    Draw(w, h);
}

void start()
{
	init();
	View_initial();
	// Создание списков для прорисовки
	CreateGlLists();
	Fly.Create( 0,Fly_Count );
	StartTime = GetTickCount();
	glutDisplayFunc(displayGlut);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboardGlut);
	glutSpecialFunc(processSpecialKeysGlut);
	glutMouseFunc(mouseButton);
	glutIdleFunc(tim);
}

static void Model_Draw( Model_Type typ ) // Простые равнообъёмные фигуры
{
    switch( typ )
    {
    case Cube  :
        Mxl*=pow( Particle_volume/(Real)Mxl,0.333 );
        Mxl.cubic(painting);
        break;
    case Sphere:
        Mxl*=pow( Particle_volume*0.75/_Pi/(Real)Mxl,0.333);
        // Использование списка для отображения шара быстрее в среднем в 2.4 раза
        glPushMatrix();
        glTranslated(Mxl.X, Mxl.Y, Mxl.Z);
        glCallList(theShere);
        glPopMatrix();
        // Использование нативного отображения шара циклами
        //Mxl.sphere( 15,24,painting );
        break;
    case Dipole:
        Mxl*=pow( Particle_volume*0.75/_Pi/(Real)Mxl,0.333);
        Mxl.dipole( Dipole_offset,painting );
        break;
    case Torus:
    {
        const Real K=0.01,r=1.0;       // коэффициент к диаметру дырки
        Mxl*=pow( Particle_volume*_Pi/_Pi/2/(1+K)/(Real)Mxl,0.333 )/2;
        Mxl.torus( r,r*( 1+K ),15,24,painting );
        break;
    }         // painting - признак закраски граней подвижной модели
    }
}

// Создание списков для прорисовки
void CreateGlLists()
{
    Model ModelShere;
    ModelShere.set((Vector){Particle_volume*0.15,0,0});
    theShere = glGenLists(1);
    glNewList(theShere, GL_COMPILE);
    ModelShere.sphere( 15,24,painting );
    glEndList();
}

