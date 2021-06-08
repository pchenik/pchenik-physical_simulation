/**
      Векторные числовые объекты и арифметические операции
        (с)2016, В.Храмушин, ... Южно-Сахалинск
 **/
#ifndef __Vector_
#define __Vector_
#include <cmath>
#include "../Type.h"

struct Point;  typedef const Point& _Point;   // точка дальних мировых отсчетов
struct Vector; typedef const Vector& _Vector; // свободный разностный вектор
struct Matrix; typedef const Matrix& _Matrix; // чистая матрица тензора
struct Tensor; typedef const Tensor& _Tensor; // тензор собственного базиса
struct Base;   typedef const Base&   _Base;   // связанный базис Евклидовой СК
                    //
struct Vector       // Определение векторных операций в трехмерном пространстве
{ Real x,y,z;       //   ( совершенно открытый разностный и свободный объект )
  //
  //  операции с уже существующими векторами +- геометрическое сложение
  //            */ (Real) изменение длины и весьма опасное деление вектора
  //            *= площадь под нормалью двух векторов (векторное произведение)
  //
//Vector():x(0),y(0),z(0){}
//Vector& operator - (){ x=-x,y=-y,z=-z; return *this; }
  Vector& operator =  (_Real r ){ x=r; y=z=0.0; return *this; }
  Vector& operator *= (_Real r ){ x*=r,y*=r,z*=r; return *this; }
  Vector& operator /= (_Real r ){ x/=r,y/=r,z/=r; return *this; }
  Vector& operator =  (_Vector d ){ x=d.x,y=d.y,z=d.z; return *this; }
  Vector& operator += (_Vector d ){ x+=d.x,y+=d.y,z+=d.z; return *this; }
  Vector& operator -= (_Vector d ){ x-=d.x,y-=d.y,z-=d.z; return *this; }
  Vector& operator *= (_Vector d ) // произведение ортогонального вектора
          { return *this=(Vector){ y*d.z-z*d.y,z*d.x-x*d.z,x*d.y-y*d.x }; }

  Vector& operator *= (_Matrix );  // выход из местного базиса в глобальную СК
  Vector& operator /= (_Tensor );  // переход в локальный ортогональный базис
//??
  Vector& operator &= ( Vector d ){ return *this = d *= x*d.x+y*d.y+z*d.z; }
  Vector& operator ^= ( Vector n ){ return *this-=(n*=2*(x*n.x+y*n.y+z*n.z)); }
  //
  //      Три поворота вектора относительно глобальной системы отсчета
  //
  Vector& dirX(_Real a ){ Real c=cos(a),s=sin(a),w=y*c-z*s; z=z*c+y*s; y=w; return *this; }
  Vector& dirY(_Real a ){ Real c=cos(a),s=sin(a),w=z*c-x*s; x=x*c+z*s; z=w; return *this; }
  Vector& dirZ(_Real a ){ Real c=cos(a),s=sin(a),w=x*c-y*s; y=y*c+x*s; x=w; return *this; }
  //
  //      Две операции переформатирован и для открытия доступа
  //       ко всяким стандартным и различным пакетам программ
  //
//operator Real(){ return sqrt( x*x+y*y+z*z ); }  // ... пусть пока будет так
  operator Real*(){ return (Real*)this; }   // это для вектора OpenGL-3dv
//operator float*(){ static float b[3]; b[0]=x,b[1]=y,b[2]=z; return b; }
};
//     Арифметические операции с векторами в (не)дружественной среде
//
inline bool   operator ! (_Vector a ){ return !a.x && !a.y && !a.z; }
inline Vector operator - (_Vector a ){ return (Vector){ -a.x,-a.y,-a.z }; } // встречный вектор
inline Vector operator * (_Vector c,_Real w ){ return (Vector){ c.x*w,c.y*w,c.z*w }; }
inline Vector operator * (_Real w,_Vector c ){ return (Vector){ c.x*w,c.y*w,c.z*w }; }
inline Vector operator / (_Vector c,_Real w ){ return (Vector){ c.x/w,c.y/w,c.z/w }; }
inline Vector operator+(_Vector c,_Vector e ){ return (Vector){ c.x+e.x,c.y+e.y,c.z+e.z }; }
inline Vector operator-(_Vector c,_Vector e ){ return (Vector){ c.x-e.x,c.y-e.y,c.z-e.z }; }
inline bool operator ==(_Vector a,_Vector b ){ return a.x==b.x && a.y==b.y && a.z==b.z; }
inline bool operator !=(_Vector a,_Vector b ){ return a.x!=b.x || a.y!=b.y || a.z!=b.z; }
//
//   * - нормаль - ортогональный вектор, равный площади основания
//   % - проекция скалярного произведения векторов
//
inline Real operator % (_Vector a,_Vector b ){ return a.x*b.x+a.y*b.y+a.z*b.z; }
inline Vector operator & (_Vector a,_Vector b )      // скалярное, компонентное
        { return (Vector){ a.x*b.x,a.y*b.y,a.z*b.z }; }         // и векторное
inline Vector operator * (_Vector a,_Vector b )                 // произведение
        { return (Vector){ a.y*b.z-a.z*b.y,a.z*b.x-a.x*b.z,a.x*b.y-a.y*b.x }; }
//
//     в абсолютных координатах используется пораженная в правах точка Point
//
struct Point    // Координатная точка в масштабах абсолютного пространства
{ Real X,Y,Z;   // обозначаются прописными буквами абсолютно зависимых величин
         operator Real*(){ return (Real*)this; }
  Point& operator =  (_Real r ){ X=r; Y=Z=0.0; return *this; }       // очистка
  Point& operator /= (_Real r ){ X/=r,Y/=r,Z/=r; return *this; }     // масштаб
  Point& operator += (_Vector d ){ X+=d.x,Y+=d.y,Z+=d.z; return *this; }
  Point& operator -= (_Vector d ){ X-=d.x,Y-=d.y,Z-=d.z; return *this; }
};
//     Исходные арифметические действия с координатными точками и векторами
//
inline Point  operator * (_Point c,_Real w  ){ return (Point){ c.X*w,c.Y*w,c.Z*w }; }
inline Point  operator / (_Point c,_Real w  ){ return (Point){ c.X/w,c.Y/w,c.Z/w }; }
inline Point  operator + (_Point c,_Vector e){ return (Point){ c.X+e.x,c.Y+e.y,c.Z+e.z }; }
inline Point  operator - (_Point c,_Vector e){ return (Point){ c.X-e.x,c.Y-e.y,c.Z-e.z }; }
inline Vector operator - (_Point c,_Point e ){ return (Vector){c.X-e.X,c.Y-e.Y,c.Z-e.Z }; }
//
//     элементарные и раскрываемые операции-функции
//
inline Real   sqr( _Real a ){ return a*a; }
inline Real   abs( _Real a ){ return fabs( a ); }
inline Real   abs( _Vector a ){ return sqrt( a.x*a.x + a.y*a.y + a.z*a.z ); }
inline Real  norm( _Vector a ){ return a.x*a.x + a.y*a.y + a.z*a.z; }
inline Vector dir( _Vector a ){ return a/abs( a ); }
#endif
