/**
 Пакет векторно-тензорных математических преобразований в сеточном пространстве
       с локально определёнными корпускулами среди связных живых сил
         (с)2016, В.Храмушин, Санкт-Петербургский университет <GPL>
                  Иерусалим - Санкт-Петербург.
 **/
#ifndef __Matrix_Tensor_Base_
#define __Matrix_Tensor_Base_
#include "Vector.h"

struct Matrix; typedef const Matrix& _Matrix; // чистая матрица тензора
struct Tensor; typedef const Tensor& _Tensor; // тензор собственного базиса
struct Base;   typedef const Base&   _Base;   // связанный базис Евклидовой СК
//
//   Тензоры служат фиксации физических полей в локальных базисах с целью
//   — взаимного преобразования векторных координат и тензорных компонент
//   — изначально задается ортогональный ортонормированный базис, который
//     изнутри всегда видится единичной и ортогональной координатной системой
//   — три вектора образуют один столбец матрицы, их компоненты - по строкам
//
struct Matrix    // три базисных вектора с быстрым доступом к их области памяти
{ Vector x,y,z;            // тривекторы Евклида косоугольной системы координат
  Matrix& set( _Vector );              // —— самые простые производные операции
  Matrix& set( _Vector,_Vector );
  Matrix& operator=(_Real a ){ x=y.y=z.z=a; y.x=y.z=z.x=z.y=0; return *this; }
  Matrix& operator*=(_Real r ){ x*=r,y*=r,z*=r; return *this; }
  Matrix& operator*=(_Matrix );
  friend Matrix operator*(_Matrix A,_Real R ){return (Matrix){A.x*R,A.y*R,A.z*R};}
  friend Matrix operator*(_Matrix A,_Matrix B );
  friend Matrix operator+(_Matrix A,_Matrix B){return (Matrix){A.x+B.x,A.y+B.y,A.z+B.z};}
  friend Matrix operator-(_Matrix A,_Matrix B){return (Matrix){A.x-B.x,A.y-B.y,A.z-B.z};}
  //                                                                                                                                      //
  //   Переход из локального базиса к абсолютной системе координат
  //                                               (проекции с базисными осями)
  friend Vector operator * ( _Matrix,_Vector );    // переход из ЛСК в АСК
  friend Vector operator * ( _Vector v,_Matrix m ) // проекции на базисные оси
                { return (Vector){ v%m.x,v%m.y,v%m.z }; }

//operator Real(){ return (x*y)%z; }          //... определитель — объем триады
};
//
//    Смещение базисных осей для тензора инерции (Гюйгенс-Штейнер)
//    M - тензор инерции; С - координаты центра масс; R - новая точка отсчета
//
Matrix Steiner(_Matrix M,_Vector C,_Vector R=(Vector){0,0,0} ); // К центру C, затем к R
//
//  Пространственно-геометрические повороты задаются в радианах
//
Matrix Krylov( _Real,_Real=0,_Real=0 ); // крен дифферент рыскание
Matrix Euler( _Real,_Real=0,_Real=0 );  // вперед, вправо и вверх
                                        // прецессия, нутация, вращение
class Tensor:      // Арифметика линейной трансформации элементарной корпускулы
public Matrix      // — три базисных вектора, всегда ненулевые и некомпланарные
{ Vector xi,yi,zi; // — обратный тензор служит быстрому исполнению произведений
  Real d;          // — определитель - смешанное произведение базисных векторов
  //
  // пост-конструктор предваряет контроль состояния сплошной среды
  //                и допустимых операций над корпускулами
  //
 Tensor& det(){ d=(x*y)%z,xi=(y*z)/d,yi=(z*x)/d,zi=(x*y)/d; return *this; }
 Tensor& init(){ x.x=y.y=z.z=xi.x=yi.y=zi.z=d=1.0;          // Декартовый базис
     x.y=x.z=y.x=y.z=z.x=z.y=xi.y=xi.z=yi.x=yi.z=zi.x=zi.y=0.0; return *this; }
 Tensor& init(_Real R){ x.x=y.y=z.z=R; xi.x=yi.y=zi.z=1/R; d=R*R*R; // Евклидов
     x.y=x.z=y.x=y.z=z.x=z.y=xi.y=xi.z=yi.x=yi.z=zi.x=zi.y=0.0; return *this; }
public:
 //
 // Блок конструкторов C++ включает пересыльные процедуры прямого доступа set:
 //                     — сначала ортогональный и ортонормированный на единицу
 Tensor(){ init(); }
 Tensor(_Real m ){ init( m ); }           // ортогональный базис с масштабом: m
 Tensor(_Matrix M ): Matrix( M ){ det(); } // базис Эвклидова пространства
 Tensor(_Vector a ){ set( a ); det(); }    // конструирование из векторов с
 Tensor(_Vector a,_Vector b ){ set( a,b ); det(); }   // ортогональной осью
 Tensor(_Vector a,_Vector b,_Vector c ):Matrix( (Matrix){ a,b,c } ){ det(); }

 Tensor& operator=( _Matrix m ){ x=m.x,y=m.y,z=m.z; return det(); }
  //
  //   Блок основных пространственно-геометрических преобразований
  //
 Tensor& operator*=( _Real s )                  // - простое изменение масштаба
      { x*=s; y*=s; z*=s; d*=(s*s*s); xi/=s,yi/=s,zi/=s; return *this; }
 Tensor& operator*=(_Matrix m ) // подъем проекции к абсолютной системе отсчета
      {  *this = Matrix::operator*=( m ); return det(); }
 Tensor& operator/=(_Tensor m ); // возврат глобального отсчета в местный базис
  //
  //   Трансформация вектора для перехода в местную/дуальную систему координат
  //                                                (вектор на обратный тензор)
 friend Vector operator / ( _Vector,_Tensor );
  //
  //   Повороты вокруг базисных (локальных) координатных осей —— в радианах
  //
 Tensor& Identity(){ return init(); }     // нормированный ортогональный тензор
 Tensor& axiX(_Real a ){ x.x=1,x.y=x.z=y.x=z.x=0,y.y=z.z=cos(a),z.y=-(y.z=sin(a)); return det(); }
 Tensor& axiY(_Real a ){ y.y=1,y.x=y.z=x.y=z.y=0,z.z=x.x=cos(a),x.z=-(z.x=sin(a)); return det(); }
 Tensor& axiZ(_Real a ){ z.z=1,z.x=z.y=x.z=y.z=0,x.x=y.y=cos(a),y.x=-(x.y=sin(a)); return det(); }
 Tensor& rotX( _Real ); Tensor& rotY( _Real ); Tensor& rotZ( _Real ); // и угловой подворот
  //
  //   В качестве модуля/нормы  - детерминант/определитель
  //
 operator _Real(){ return d; }
};
struct Base: Tensor,Point                     // тензор формы с местоположением
{ Base():Tensor(),Point( (Point){0,0,0} ){}   // без излишних преобразований
  Base(_Real S ):Point((Point){0,0,0}),Tensor( S ){}// пространственный масштаб
  Base(_Point R,_Real S ): Point( R ),Tensor( S ){} //   -- смещение и масштаб
  Base(_Point R,_Vector a,_Vector b,_Vector c ): Point( R ),Tensor( a,b,c ){}

  Base& Identity(){ Point::operator=( 0.0 ); Tensor::Identity(); return *this; }
  Base& operator = ( _Point r  ){ Point::operator=( r );  return *this; }
  Base& operator = ( _Matrix m ){ Tensor::operator=( m ); return *this; }
  Base& operator +=( _Vector v ){ Point::operator+=( v ); return *this; }
  Base& operator -=( _Vector v ){ Point::operator-=( v ); return *this; }
  //
  //   Можно задавать переходы между абсолютной и локальной СК
  //
  Point out( _Vector a ){ return (Point)*this + (Matrix)*this * a; }
  Vector in( _Point A ){ return Vector( A-(Point)*this ) / (Tensor)*this; }
};
#endif // __Matrix_Tensor_Base_
