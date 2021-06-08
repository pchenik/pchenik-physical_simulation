

#ifndef PHYZICAL_SIMULATIONS_H_INCLUDED
#define PHYZICAL_SIMULATIONS_H_INCLUDED

#include <GL/glu.h>
#include <time.h>
#include "../Math/Model.h"
#include "../Math/Volume.h"
#include "../Window/View.h"
#include <iostream>
#include <openacc.h>
#include <GL/glut.h>

Vector eye,look;  // направление взгляда и координаты местоположения сцены
Real Distance = -32;    // расстояние от точки установки камеры до места обзора

static int Height;         // высота окна
static int Width;          // ширина окна

long GetTickCount();

static long StartTime;
long ElapsedTime();

void Draw(int Widthl, int Heightl);

//                    Блок анализа и настройки управляющих параметров программы
Real Init_Flight_Points( Array& );           // на выходе размер ячейки-частицы
//              Силовые реакции и приращение скорости при взаимодействии частиц
void One_Inter_Step_of_Point( Array&,bool );

void One_Inter_Step_of_PointBalls(int count, Real dT, Ball *balls, bool MFiz );
void Inter_And_Flight_Step(Array&, bool, Vector *);


//           Блок расчета кинематики частицы под действием внешних сил и трения
void One_Flight_Step_of_Point( Array&,int );
//
//                                  Пространственные размерности видимого поля
static const Real vX=16,vY=16,vZ=16,Screen_Size=max( max( vX,vY ),vZ );   // 4;
//               // пространство, отображаемое на экране ЭВМ
// ( "Моделирование корпускулярных взаимодействий",0,0,1600,1000,Screen_Size*2 );
//
//    Объединение геометрических фигур для динамического выбора и перенастройки
//


static enum Model_Type { NoneNodel,Cube,Torus,Sphere,Dipole } MTyp = Model_Type::Sphere;

static void Model_Draw( Model_Type typ );       // Простые равнообъёмные фигуры
static Array Fly;                 // собственно свора поляризованных корпускул
static Model Mxl;                 // собственный базис геометрического объекта
//
//    Определение основных рабочих параметров
//
//Real ks=0.02,
Real ks=0.4,                     // коэффициент сопротивления для шара[0.2-0.4]
     //wall=2; //1.32,             // стенки аквариума (бесконечно высокого,пока)
     wall=4;
bool Gravity_mode=false;         // свободное/гравитационное перемещение частиц
bool linear_Model=false;         // линейно-упругая или ядерная высокой энергии
// модель взаимодействия свободных частиц
static int Fly_Count=1440;        // количество шагов на видимой траектории
static bool painting=true,        // объемная или контурная прорисовка фигуры
            expMoving=false;      // вращение всего пространства сцены в OpenGL
static int  Draw_flow = 3;        // 0-нет; 1-вызванное; 2-полное; 3-вместе
static Real Dipole_offset=0.0,    // скоростная деформация диполя
            Particle_volume=1.0,  // контрольный объем моделируемой частицы
            fx=0.2,fy=0.2,fz=0;   // пространственный щаг для векторных отметок

// список для сферы
GLuint theShere;

static Course Configuration();

bool display();

bool timer_drawing();


int main_ps( int argc, char *argv[], char *envp[] );

void start();

// Создание списков для прорисовки
void CreateGlLists();

#endif // PHYZICAL_SIMULATIONS_H_INCLUDED
