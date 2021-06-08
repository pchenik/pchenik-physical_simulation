/**
      вот так
 **/
#ifndef VIEV_H_INCLUDED
#define VIEV_H_INCLUDED

// Очередная отработка элементарных графических примитив 2018-08-22
#include <GL/glu.h>
// базовые структуры тензорной математики
#include "../Type.h"
#include "../Math/Vector.h"

//#include "..\Window\Window.h"     // собственно графическая среда Window-Place

//
// Настройка начальной раскраски и освещенности графического пространства/сцены
//
void View_initial();              // ...предустановка графической среды OpenGL

typedef unsigned char byte;


  navy,blue,lightblue,cyan,aqua,lightcyan,maroon,red,lightred,orange,pink,
  purple,magenta,fuchsia,lightmagenta,black,empty=-1 //!=26\{27}
} colors;
     char *_Mnt[],*_Day[]; // словесные прописи имён месяцев года и дней недели

//
//  Небольшой комплекс процедур времени проведения вычислительного эксперимента
//
void color( colors );          // выбор цвета в палитре SeaColor
void color( colors clr,        // ...с относительной подсветкой/затенением
             _Real bright,     //   -1 => на черный; +1 => до белого
             _Real alfa=1.0 ); // прозрачность\смешение 1 => 0 выцветание blend
//
void point( const Real* a );
void line( const Real* a, const Real* b );

void points( const Real*, Real Size, colors color=empty );
//
void arrow_point(const Point &a,const Point &b, _Real l, colors clr );
void axis( _Real,_Real,_Real, const char *x,const char *y,const char *z,colors color=cyan );
//class View:          // блок визуализации возвращается сюда до лучших времен...
//public Window{       // собственно доступ к экрану, указателю и клавиатуре
//virtual Place& Mouse( int x,int y ){ return Place::Mouse( mx=x,my=y ); }// мимо
//virtual Place& Mouse( int b,int x,int y ); // курсор-указатель - мышь свободная
//protected:
//   Vector eye,look;  // направление взгляда и координаты местоположения сцены
//   Real Distance;    // расстояние от точки установки камеры до места обзора
//   int mx,my;        // растровые координаты курсора мышки в окне
//public:
//   View( const char* Title, int X=0,int Y=0, int W=0,int H=0,_Real Size=1 );
//virtual bool KeyBoard( byte Keyb );
//virtual Place& Draw();
//};

// __View_
#endif
