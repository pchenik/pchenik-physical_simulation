#ifndef __Type_h__
#define __Type_h__
#include <math.h>
#include <stdlib.h>
typedef double Real;           // Просто число ко всем арифметическим операциям
typedef const Real& _Real;     //   - константная ссылка - процедурный параметр
struct Event; typedef const Event& _Event; // Обобщение / синтез даты и времени
   //                                                               ©75 ‏יְרוּשָׁלַיִם
   //  a 6378245 м        Сфероид Красовского для морских карт России
   //  b 6356863.0188 SN\ Эллипсоид Красовского ©א
   //  c 6399698.9018 EW/ (b+c)/2 = 6367554.0094  e=298.3
   //  м 1855.35626248109543 м - сфероидальная миля
   //  g 9.8106 м/с²
const Real _Mile=1852.24637937659918,          // Морская (равнообъёмная) миля
   //  радиус эквивалентной сферы в отношении к равнообъёмному единичному кубу
  EqSphere=0.62035049089940001666800681204778, //     r = (3/4/π)^(1/3)
      // 1.24070098179880003333601362409556 - диаметр D = (6/π)^(1/3)
     _Pi=3.1415926535897932384626433832795,    //   π              M_PI
     _Pd=6.283185307179586476925286766559,     //   π x 2
     _Ph=1.570796326794896619231321691639,     //   π / 2          M_PI_2
     _iP=0.31830988618379067153776752674503,   //   1 / π          M_1_PI
     _Rd=57.295779513082320876798154814105,    // 180 / π
     _dR=0.01745329251994329576923690768489;   //   π / 180
//   _e  2.7182818284590452353602874713527                         M_E
enum Course
{  _North_West=3,_North=1,_North_East=9, _Home=3,   _Up =1,_PgUp ,
         _West=2,_Zenith=0,     _East=8, _Left=2,_Center=0,_Right=8,
   _South_West=6,_South=4,_South_East=12,_End =6, _Down =4,_PgDn=12,
   _Enter=13,_BkSp,_F1,_F2,_F3,_F4,_F5,_F6,_F7,_F8,_F9,_F10,_F11,_F12,
   _Esc=27,_Ins,_Del,_Tab = 9,_Blank=32                   // +5,+7,+10,+31 в запасе
};
enum{ _MouseMove,_MouseLeft,_MouseRight,_MouseMiddle,_MouseWheel };
enum{ RIGHT=1,LEFT,SHIFT, LCTRL,RCTRL=8,CTRL=12, L_ALT=16,R_ALT=32,ALT=48 };

void julday( long day,int& m,int& d,int& y );// Julian( day ) -> дата( m,d,y )
long julday( int m, int d, int y );         // возврат: дата( m,d,y ) -> Julian
long julday(); Real onetime();             // Текущие дата и время [дни и часы]
struct Event{ long D; Real T;             // Юлианская дата и часы от полуночи
  Event& Now();                          // здесь текущее компьютерное время
  Event(){ Now(); }                     //  ++ в простом/чистом конструкторе
  Event& Check();                      // согласование внешних сложений времени
  Event( _Event New ): D( New.D ),T( New.T ){ Check(); }
  Event( long d,_Real t=0.0 ): D( d ),T( t ){ Check(); }
  Event  operator + (_Real t ){ return Event( D,T+t ); }
  Event  operator - (_Real t ){ return Event( D,T-t ); }
  Event& operator +=(_Real t ){  T+=t; return Check(); }
  Event& operator -=(_Real t ){  T-=t; return Check(); }
  Event& UnPack( int &m,int &d,int &y ){ julday( D,m,d,y ); return *this; }
  long     Pack( int m, int d, int y  ){ return D = julday( m,d,y ); }
  friend Event operator+(_Event B,_Real t ){ Event E( B.D,B.T+t ); return E; }
  friend Real  operator-(_Event A,_Event B ){ return (A.T-B.T)+(A.D-B.D)*24; }
  friend bool operator> (_Event A,_Event B ){ return ( A-B )> 0.0; }
  friend bool operator>=(_Event A,_Event B ){ return ( A-B )>=0.0; }
  friend bool operator< (_Event A,_Event B ){ return ( A-B )< 0.0; }
  friend bool operator<=(_Event A,_Event B ){ return ( A-B )<=0.0; }
  friend bool operator==(_Event A,_Event B ){ return ( A-B )==0.0; }
  friend bool operator!=(_Event A,_Event B ){ return ( A-B )!=0.0; }
};class string{ char *s; int len;       // текстовая строчка неограниченной длины
  char* set( int );                   // контроль размерности удлинением на +2k
public: string(); ~string();          // начальная установка=2k очистка выходом
  operator char*(){ return s; }       // традиционный доступ по адресу строки и
  char& operator[]( int k ){ if( k+8>=len )set( k ); return s[k]; } // буквочки
};
//   Операции с угловыми градусами и разметкой шага шкал включены в julian.cpp
//
template<class real>real Angle( real );   // D° = ±180°
char* DtoA( Real,int=0,const char* ="°"); // (±)ⁿ123°45'67"00 как круговая мера
char* AtoD( char*, Real& );               // (°)÷(:) с возможной заменой на ':'
Real AxisStep( Real );                    // Step for Axis Estimation
//
//    Работа с текстовыми строчками в StrCpy.cpp
//
int strcut( char* Str );                 // Убираются концевые пробелы, символы
void Break( const char Msg[],... );      // с подсчетом оставшейся длины строки
char* fname( const char* FileName );     // Поиск собственно самого имени файла
char* sname( char* FullFileName );       //   то же, но с отсечением расширения
char* fext ( char*, const char* Ex=0 );  // Принудительная замена расширения
#if defined(_STDIO_H_)||defined(_INC_STDIO)// имён файлов - FileName Extensions
FILE *FileOpen                           // ++ кодировка длинных строк FileOpen
  ( char *fn, const char *tp, const char *ex, const char *ch, const char *tl );
char *getString( FILE *F );              // Чтение строки на статическом адресе
char *getString( FILE *F, int t );       // неограниченной длины + табуляторы
#endif                                   // результат - в подстрочках getString
int  Usize( const char *A, bool=false ); // длина на выходе из DOS/Win в UTF-8
int  Ulen( const char* UTF );            // количество позиций в строке UTF-8
char* Uget( const char *U );             // однократная выборка UTF-8 символа
char* Uset( const char *UTF, int k );    // установка на индекс -1 конец строки
char* Uset( int &k, const char *U );     //  ++ с подтверждением местоположения
char* Uset( char* U,int k,const char* S,bool ins=false ); // вставка S на U[k]
char* UtR( char &s, char *U );           // символ UTF-8 -> Russian-OEM(866)alt
char* UtW( char &s, char *U );           // символ UTF-8 -> Russian-Win-1251
char* UtC( unsigned &u, char *U );       // UTF-8 -> UniCode, на выходе адрес
const char* RtU( const char R );         // получение UTF-8 кода из символа OEM
const char* WtU( const char W );         // получение UTF-8 кода из Win-1251
const char* CtU( unsigned u );           // UniCode -> UTF-8    (int->string)
char* UlA( char* U, bool oem=false );    // Конвертация из Unicode на месте
char* UtA( const char* U, bool=false );  // Преобразование строк в буфере ввода
char* AtU( const char* A, bool=false );  // для OEM-866(true) и Win-1251(false)

//template<class T>inline const T abs(const T &A){ return A<0?-A:A; }
template<class T>inline const T &minmax( const T &a, const T &b, const T &c )
                                              { return a>=b?a:( c<=b?c:b ); }
#if !defined( min ) && !defined( max )
template<class T>inline const T &max(const T &a, const T &b){ return a>b?a:b; }
template<class T>inline const T &min(const T &a, const T &b){ return a<b?a:b; }
#endif
#ifndef __Allocate__
void  *Allocate( size_t Sz, void *A=NULL );         // Линейный массив в байтах
void **Allocate( size_t Ny,size_t Sz,void* =NULL ); // Количество и длина строк
size_t isAlloc( void* A ); // ?оперативный объём или количество строк в матрице
#endif
#endif
