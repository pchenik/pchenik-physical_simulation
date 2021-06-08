/**
      вот так
 **/
#ifndef VIEV_H_INCLUDED
#define VIEV_H_INCLUDED

// Очередная отработка элементарных графических примитив 2018-08-22
#include <GL/glu.h>
// базовые структуры тензорной математики#include "../Math/Volume.h"
#include "../Type.h"
#include "../Math/Vector.h"

//#include "..\Window\Window.h"     // собственно графическая среда Window-Place

//
// Настройка начальной раскраски и освещенности графического пространства/сцены
//
void View_initial();              // ...предустановка графической среды OpenGL

typedef unsigned char byte;

typedef enum{ white,silver,lightgray,gray,darkgray,green,yellow,lime,olive,lightgreen,
  navy,blue,lightblue,cyan,aqua,lightcyan,maroon,red,lightred,orange,pink,
  purple,magenta,fuchsia,lightmagenta,black,empty=-1 //!=26\{27}
} colors;extern const
     char *_Mnt[],*_Day[]; // словесные прописи имён месяцев года и дней недели

//
//  Небольшой комплекс процедур времени проведения вычислительного эксперимента
//
void color( colors );          // выбор цвета в палитре SeaColor
void color( colors clr,        // ...с относительной подсветкой/затенением
             _Real bright,     //   -1 => на черный; +1 => до белого
             _Real alfa=1.0 ); // прозрачность\смешение 1 => 0 выцветание blend
////    тонкая линия из точки (a) в точку (b) ... в однородных координатах OpenGL////inline void point( const Real* a ) { glVertex3dv( a ); }
void point( const Real* a );//inline void line2( const Real* a, const Real* b )//          { glBegin( GL_LINES ),glVertex3dv( a ),glVertex3dv( b ),glEnd(); }
void line( const Real* a, const Real* b );
void point( const Real* a, colors clr );void line ( const Real* a, const Real* b, colors clr );
void points( const Real*, Real Size, colors color=empty );void lines( const Real*,const Real*, colors color=empty );void circle( const Real *center,_Real radius, bool=true ); // кружочек в {x-y}
////   ... из точки (a) в точку (b) с объемной стрелкой в долях длины//void arrow( const Real *_a,const Real *_b,_Real l,colors clr );
void arrow_point(const Point &a,const Point &b, _Real l, colors clr );
void axis( _Real,_Real,_Real, const char *x,const char *y,const char *z,colors color=cyan );//void axis( Place&,_Real,_Real,_Real, const char *x,const char *y,const char *z,colors color=cyan );                     //
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

