/**
      Трехмерные тензорные вычислительные объекты и нестационарные
      континуально-корпускулярные операции в адаптируемых и рекурсивно
      сгущаемых массивах нерегуляризованных узлов для сопряженных ячеек
      в интерполяционном окружении неразрывных пространств физических
      параметров состояния и механики глобального взаимовлияния с локальным
      взаимодействием поляризованных частиц-ячеек сплошной среды
      с естественными реологическими свойствами.
                                         (с)2016, В.Храмушин, ... Иерусалим
 **/
#ifndef __Volume_
#define __Volume_
//#include "../Type.h"
#include "Tensor.h"
//
//     Node - один узел в сеточной области с адресными ссылками {x,y,z}
//            на смежные узлы трехмерного тензорного базиса. Внутренний
//            массив *in[8] создает двоичное дробление на 8 вложенных
//            сеточных ячеек, доступных рекурсивно при с исходного уровня
//            C узлом Node связываются параметры состояния физического
//            пространства, содержащие скалярные-векторные-тензорные
//            вычислительные объекты, соотносимые либо с местоположением
//            центров объема/масс размечаемых пространственных ячеек,
//            либо непосредственно с координатами узлов в сеточной области.
//     Space - Исходное сеточное пространство верхнего уровня создается
//            одномоментно с фиксацией абсолютных координат через собственный
//            базис (Base) для всего пространства, что необходимо для
//            создания подвижной глобальной системы отсчета, связанной с
//            виртуальным расчетным объектом...
//
class  Flex;   typedef const Flex&   _Flex;   // пространственная кривая линия
class  Field;  typedef const Field&  _Field;  // поле волн Герстнера/Кельвина
struct Node;   typedef const Node&   _Node;   // узел со ссылками на смежные
struct Space;  typedef const Space&  _Space;  // пространство связанных ячеек
struct Volume; typedef const Volume& _Volume; // 3D расчетная область в целом
struct Array;  typedef const Array&  _Array;  // множество, строй, таблица
struct Edge;    // ребро, кромка
struct Verge;   // грань, кайма, обочина
struct Vertex;  // вершина, точка полигона
struct Affinor; // связный базис Евклидовой СК
struct Ball;   // естественно-физические определения одной свободной корпускулы
//
//      Сеточная область Space определяет нерегуляризованное интерполяционное
//      пространство динамически перестраиваемых узлов, содержащих скалярные,
//      векторные и/или тензорные физические объекты/величины, определяющие
//      густоту сеточного покрытия в зависимости от локальной кривизны
//      моделируемых параметров состояния сплошных/неразрывных сред и, по
//      возможности, с режимом прогноза трансформации гидродинамических потоков
//
struct Node       // узел с координатами X,Y,Z, смежными и внутренними связями
     : Point      // координаты центра масс для простого физического объекта...
{ Node *x,*y,*z;  // ссылки на смежные узлы настоящего/внешнего уровня рекурсии
//     *in;       // и восемь вложенных узловых ячеек, если таковые необходимы
};                //
//     По ходу вперед выполняется перенастройка сетки
//       с уточнением граничных условий и заданной густоты расчетных ячеек
//     На обратном пути выполняется ускоренный поиск для интерполяции или
//       приближенной аппроксимации моделируемых физических явлений и процессов
//
struct Space: Base        // функциональное связывание текущих координат узлов
{ const int nX,nY,nZ;     // размерность исходных узлов расчетной сетки в целом
  Node ***Ns;             //   трехмерный массив узлов базового уровня рекурсии
                          //   X6(вправо),Y4(вверх),Z5(ближе) + базис и масштаб
  Space( int x=24,int y=16,int z=20, Point=(Point){0,0,0}, Real=1.0 );
 ~Space();                //  выполняется освобождение всех рекурсивных уровней
  Space& operator=( _Point R ){ Point::operator=( R ); return *this; } // место
};
//     Корпускулярное покрытие моделируемой сплошной среды осуществляется
//       независимыми числовыми объектами, участвующими в построениях законов
//         движения и определяющих сложные и производные физические процессы
//           с возможностью параллельного исполнения весьма тяжелых расчетов
struct Volume
{  int nV;            // общее количество независимо сосуществующих корпускул
   Node **out;        // список адресов опорных базисов в пространстве Space
   Matrix *part;      // список частиц в последовательной Space-индексации
};                    //
struct Array          // Свободное движение независимых дипольных корпускул
{ int  nc,mc,         // текущее количество корпускул в общей расчетной своре
       kt,lt;         // индекс к началу и длина всех хвостов для траекторий
  Real  MV,           // исходная скорость, как дипольный момент всех корпускул
        dT;           // ~ 12 шагов по времени на проход вдоль одной корпускулы
  Ball *Dip;          // единый массив памяти для всех поляризованных корпускул
                      //
  Array& Create( int mPoints=0, int Fly_count=0, Real mv=1.0 );
  Array& Add( _Point R,_Vector M,_Vector V );  // вброс частицы под косым углом
  Array& Rid( int );                      // -- убрать точку по номеру в списке
  Array& Clear();
  Array():mc( 0 ){ Create(); }      // mc=0 - построение новой группы корпускул
 ~Array(){ Clear(); }
};
///
///    Блок некоторых особых прикладных объектов и операций с ними
///
//     функции скалярного и векторного воздействия для одиночного диполя
//
#if 0
inline Real one_2( _Real r ){ return r<1 ? sqrt(r)*(3.5-2.5*r) : 1.0/(r*r); }
inline Real one_3( _Real r ){ return r<1 ? sqrt(r)*(4.5-3.5*r) : 1.0/(r*r*r); }
#else
inline Real one_2( _Real r ){ return r<1 ? 2.0-r*r   : 1.0/(r*r);   } // 2^2
inline Real one_3( _Real r ){ return r<1 ? 2.0-r*r*r : 1.0/(r*r*r); } // 3^3
#endif
inline Vector dipole( _Vector d,  // дипольный радиус первого критического узла
                      _Vector r ) // радиус-вектор из удаленной точки к центру
{
Real s=norm( r );
      return ( 0.5*( r*( d*r ) ) - r*( d%r ) )*one_3( sqrt( s/norm( d ) ) )/s;
}
inline Vector dipole_flow( _Vector d, _Vector r )
{ Real s,f=one_3( sqrt( (s=norm( r ))/norm( d ) ) );
      return 4*( r*( 1.0-f )*(  d%r ) + r*( 1.0+0.5*f )*( d*r ) ) / s;
}
//      Массив свободных корпускул, обладающих дипольными свойствами и
//      взаимодействующих между собой по кратчайшим геодезическим прямым
//      (закон Био-Савара-Лапласа в пространстве гармонических функций)
//      -- ?? поляризация нейтрона нейтрализуется гироскопом спина ??
//
struct Ball    // естественно-физические определения одной свободной корпускулы
{ //int lt,kt; // длина и отсечка кольцевого буфера видимых траекторий
  Point   R;   // координаты подвижной корпускулы на текущий момент времени
  Vector *dR,  // обратный сдвиг индексов координат для отображения траекторий
          V,   // реальная скорость движения корпускулы с учетом инерции
          M;   // дипольные моменты с ориентацией по суммарным потокам
//Vector operator[]( int i ){ return dR[(i+kt)%lt]; }     // выборка траектории
//Ball& operator<<=( _Vector V ); // задвиг путевой разности и снижение индекса
  Ball& Fix( int kt, int lt, int ln ); // сброс индекса и установка новой длины
};
#endif
