///
///   Первый пакет векторно-тензорных математических преобразований
///      (c)2013, В.Храмушин, Санкт-Петербургский университет
///
//    В геометрических построениях и физических операциях тензоры
//        служат для фиксации физических полей в локальных базисах,
//    и - для взаимного преобразования координат векторных отсчетов
//                           и пространственных физических объектов
#include "Tensor.h"
//
//    Опубличенные set.конструкторы для заполнения матриц существующих тензоров
//
Matrix& Matrix::set( _Vector a )       // задается ось Х и другие Y,Z - абы как
 { z=( x=a )*dir( y=abs( a )
            *dir( a*(Vector){0,0,1}+a*(Vector){0,1,0}+a*(Vector){1,0,0} ) );
   return *this;
 }
Matrix& Matrix::set( _Vector a,_Vector b )         // по двум векторам строится
 { z=(x=a)*(y=b); z/=sqrt( abs(z) ); return *this; // нормаль (к кубу) по корню
 }                                                 // опорной площади
//  Умножение\произведение тензорных объектов:
//   ортогональную матрицу поворота можно приставлять справа (домножать на нее)
//    !! надо бы ... продумать соответствующие позиции векторов строка/столбец,
//      чтобы получающиеся произведения проходили простыми векторными свертками
//
Matrix& Matrix::operator *= ( _Matrix r ){           // преобразование тензоров
  #if 0                                              //! ?как бы надо домножать
    x=(Vector){ x.x*r.x.x + x.y*r.y.x + x.z*r.z.x,   //! ?? только на обратный!
                x.x*r.x.y + x.y*r.y.y + x.z*r.z.y,   // использовано тензорное
                x.x*r.x.z + x.y*r.y.z + x.z*r.z.z }, // правило суммирования
    y=(Vector){ y.x*r.x.x + y.y*r.y.x + y.z*r.z.x,   // немых индексов
                y.x*r.x.y + y.y*r.y.y + y.z*r.z.y,   // : абсолютный базис с
                y.x*r.x.z + y.y*r.y.z + y.z*r.z.z }, //   нижними индексами
    z=(Vector){ z.x*r.x.x + z.y*r.y.x + z.z*r.z.x,   //   расположен слева,
                z.x*r.x.y + z.y*r.y.y + z.z*r.z.y,   // * локальный - справа
                z.x*r.x.z + z.y*r.y.z + z.z*r.z.z };
  #else
    x=(Vector){ x%r.x,x%r.y,x%r.z }, // !! - странно, что даже при такой записи
    y=(Vector){ y%r.x,y%r.y,y%r.z }, // матриц, все равно получается сложение
    z=(Vector){ z%r.x,z%r.y,z%r.z }; // произведений абы как - по столбцам *(-)
  #endif
  return *this;
}
Matrix operator * ( _Matrix a,_Matrix b )
{  return (Matrix){ (Vector){ a.x%b.x,a.x%b.y,a.x%b.z },
                    (Vector){ a.y%b.x,a.y%b.y,a.y%b.z },
                    (Vector){ a.z%b.x,a.z%b.y,a.z%b.z } };
}
Tensor& Tensor::operator /= ( _Tensor r ){           // преобразование тензоров
  #if 0                                              //! ?как бы надо домножать
    x=(Vector){ x.x*r.xi.x + x.y*r.yi.x + x.z*r.zi.x, //! - только на обратный!
                x.x*r.xi.y + x.y*r.yi.y + x.z*r.zi.y, // использовано тензорное
                x.x*r.xi.z + x.y*r.yi.z + x.z*r.zi.z }, // правило суммирования
    y=(Vector){ y.x*r.xi.x + y.y*r.yi.x + y.z*r.zi.x,   // немых индексов
                y.x*r.xi.y + y.y*r.yi.y + y.z*r.zi.y,   // : абсолютный базис с
                y.x*r.xi.z + y.y*r.yi.z + y.z*r.zi.z }, //   нижними индексами
    z=(Vector){ z.x*r.xi.x + z.y*r.yi.x + z.z*r.zi.x,   //   расположен слева,
                z.x*r.xi.y + z.y*r.yi.y + z.z*r.zi.y,   // * локальный - справа
                z.x*r.xi.z + z.y*r.yi.z + z.z*r.zi.z };
  #else
    x=(Vector){x%r.xi,x%r.yi,x%r.zi},
    y=(Vector){y%r.xi,y%r.yi,y%r.zi},
    z=(Vector){z%r.xi,z%r.yi,z%r.zi};
  #endif
  return det();
}
//   Переход из локального базиса к абсолютной системе координат
//
Vector operator * ( _Matrix m, _Vector v ){ return (Vector)
  #if 1
       { m.x.x*v.x + m.y.x*v.y + m.z.x*v.z,      // умножение вектора справа
         m.x.y*v.x + m.y.y*v.y + m.z.y*v.z,      // на собственный тензор слева
         m.x.z*v.x + m.y.z*v.y + m.z.z*v.z };    // - возврат из ЛСК в АСК
  #else
       { m.x % v, m.y % v, m.z % v };            // вектор локальный справа с
  #endif
     }                                           // опусканием на АСК из ЛСК
//
//   Трансформация вектора для перехода в локальный ортогональный базис
//               ( дуальную систему координат )
//
Vector operator / ( _Vector v, _Tensor m )       // подъем индексов в операции
     { return (Vector)                           // произведения с лок.базисом
  #if 0
       { v.x*m.xi.x + v.y*m.yi.x + v.z*m.zi.x,   // умножение левого вектора на
         v.x*m.xi.y + v.y*m.yi.y + v.z*m.zi.y,   // обратный тензор справа
         v.x*m.xi.z + v.y*m.yi.z + v.z*m.zi.z }; // - переход из АСК в ЛСК
  #else
       { v % m.xi, v % m.yi, v % m.zi };         // и снова - есть вопросы ???
  #endif
     }
Tensor& Tensor::rotX( _Real a ){ Real c=cos( a ),s=sin( a );
             return Tensor::operator *= ((Matrix){{1,0,0},{0,c,-s},{0,s,c}}); }
Tensor& Tensor::rotY( _Real a ){ Real c=cos( a ),s=sin( a );
             return Tensor::operator *= ((Matrix){{c,0,s},{0,1,0},{-s,0,c}}); }
Tensor& Tensor::rotZ( _Real a ){ Real c=cos( a ),s=sin( a );
             return Tensor::operator *= ((Matrix){{c,-s,0},{s,c,0},{0,0,1}}); }
//
//    Полные повороты движущегося объекта в пространстве относительно осей:
//      x(a)-крен; y(b)-дифферент/тангаж; z(c)-рыскание, что формально
//      приводится к последовательному исполнению: axiX(a).rotY(b).rotZ(c)
//                                                  ( Roll, Pitch, Yaw )
Matrix Krylov(_Real a,_Real b,_Real c ) // крен, дифферент, рыскание в радианах
{ return (Matrix){                      // c1*a2*b3
 (Vector){ cos( b )*cos( c ),
           cos( b )*sin( c ),
           -sin( b ) },

 (Vector){ sin( a )*sin( b )*cos( c )-cos( a )*sin( c ),
           sin( a )*sin( b )*sin( c )+cos( a )*cos( c ),
           sin( a )*cos( b ) },

 (Vector){ cos( a )*sin( b )*cos( c )+sin( a )*sin( c ),
           cos( a )*sin( b )*sin( c )-sin( a )*cos( c ),
           cos( a )*cos( b ) } };

// (Vector){ cos( b )*cos( c ),sin( a )*sin( b )*cos( c )-cos( a )*sin( c ),
//                             cos( a )*sin( b )*cos( c )+sin( a )*sin( c ) },
// (Vector){ cos( b )*sin( c ),sin( a )*sin( b )*sin( c )+cos( a )*cos( c ),
//                             cos( a )*sin( b )*sin( c )-sin( a )*cos( c ) },
// (Vector){ -sin( b ),sin( a )*cos( b ),cos( a )*cos( b ) } };
}
//    "Эйлеровы" углы из математической энциклопедии
//     ( последовательность в иных осях: а-прецессия, b-нутация и с-вращение )
//
Matrix Euler( _Real a,_Real b,_Real c )   // а-вперед; b-вправо; с-вверх
{ return (Matrix){                        // a-прецессия, b-нутация, с-вращение
   (Vector){ cos( a )*cos( c )-sin( a )*cos( b )*sin( c ),
            -cos( a )*sin( c )-sin( a )*cos( b )*cos( c ), sin( a )*sin( b ) },
   (Vector){ sin( a )*cos( c )+cos( a )*cos( b )*sin( c ),
            -sin( a )*sin( c )+cos( a )*cos( b )*cos( c ),-cos( a )*sin( b ) },
   (Vector){ sin( b )*sin( c ),sin( b )*cos( c ),cos( b ) } };
}
//    Смещение базисных осей для тензора инерции (Гюйгенс-Штейнер)
//    M - тензор инерции; С - координаты центра масс; R - новая точка отсчета
//
#define SH( A )(Matrix){ (Vector){ sqr( A.y )+sqr( A.z ),-A.x*A.y,-A.x*A.z }, \
                         (Vector){ -A.y*A.x,sqr( A.x )+sqr( A.z ),-A.y*A.z }, \
                         (Vector){ -A.z*A.x,-A.z*A.y,sqr( A.x )+sqr( A.y ) } }

Matrix Steiner( _Matrix M, _Vector C, _Vector R ) // Сначала к базовому центру
{ return M - SH( C ) + SH( R );                  // затем к новой точке отсчета
}
/*
Matrix& Matrix::Krylov(_Real a,_Real b,_Real c ) // крен - дифферент - рыскание
{ x.x=cos( b )*cos( c ),x.y=sin( a )*sin( b )*cos( c ) - cos( a )*sin( c ),
                        x.z=cos( a )*sin( b )*cos( c ) + sin( a )*sin( c );
  y.x=cos( b )*sin( c ),y.y=sin( a )*sin( b )*sin( c ) + cos( a )*cos( c ),
                        y.z=cos( a )*sin( b )*sin( c ) - sin( a )*cos( c );
  z.x=-sin( b ),z.y=sin( a )*cos( b ),z.z=cos( a )*cos( b );  return *this;
}
Matrix& Matrix::Euler(_Real a,_Real b,_Real c )  // а-вперед; b-вправо; с-вверх
{ x.x= cos( a )*cos( c )-sin( a )*cos( b )*sin( c ), // a-прецессия, b-нутация
  x.y=-cos( a )*sin( c )-sin( a )*cos( b )*cos( c ), // с-вращение
  x.z= sin( a )*sin( b ),y.x= sin( a )*cos( c )+cos( a )*cos( b )*sin( c ),
  y.y=-sin( a )*sin( c )+cos( a )*cos( b )*cos( c ),y.z=-cos( a )*sin( b ),
  z.z= sin( b )*sin( c ),z.y= sin( b )*cos( c ),z.z= cos( b ); return *this;
}*/
