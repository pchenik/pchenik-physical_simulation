#include "View.h"    // Очередная отработка элементарных графических примитивов
//                                  ©2018-08-22 Иерусалим.
const char
*_Mnt[]= {"январь","февраль","март","апрель","май","июнь","июль","август","сентябрь","октябрь","ноябрь","декабрь"},
         *_Day[]= {"понедельник","вторник","среда","четверг","пятница","суббота","воскресенье"};

static byte SeaColor[black+257][4]=// переопределение расцветки для палитры-256
{
    // white      silver        lightgray     gray          darkgray
    {255,255,255},{192,192,192},{159,159,159},{ 96, 96, 96},{ 63, 63, 63},
    // green   yellow      lime        olive        lightgreen
    { 0,128,0},{255,255,0},{ 0,255,0},{159,255, 63},{ 63,255,96 },
    // navy    blue       lightblue    cyan         aqua         lightcyan
    { 0,0,127},{ 0,0,255},{ 63,96,255},{ 0,159,159},{ 0,255,255},{ 96,212,212},
    // maroon     red           lightred      orange        pink
    {128,  0,  0},{255,  0,  0},{255, 96, 96},{255,128,  0},{255,192,203},
    // purple     magenta       fuchsia       lightmagenta  black = 25\{26}
    {128,  0,128},{191,  0,191},{255,  0,255},{255, 95,255},{ 0,0,0 }
};
void color( colors clr )
{
    glColor4ubv( (GLubyte*)(SeaColor[clr]) );
}
#define B(c) ((b<0?(c*(1+b)):(c+(255-c)*b))/255.0) //...с затенением\подсветкой
void color( colors clr,_Real b,_Real a ) // bright: -1 на черный; +1 до белого
{
    byte *C=SeaColor[clr];    // alfa:0-1
    glColor4d(B(C[0]),B(C[1]),B(C[2]),a );
}
#undef B

void point( const Real* a )
{
    glVertex3dv( a );
}

//   тонкая линия из точки (a) в точку (b)  ... в однородных координатах OpenGL
//
void point( const Real* a, colors clr )
{
    if( clr!=empty )color(clr);    //осторожно
    glVertex3dv(a);
}
void points( const Real* a, Real Size, colors color )
{
    glPointSize( Size );
    glBegin( GL_POINTS );
    point( a,color );
    glEnd();
    glPointSize( 1.0 );
}

void line( const Real* a, const Real* b )
{ glBegin( GL_LINES ),glVertex3dv( a ),glVertex3dv( b ),glEnd(); }

void line( const Real* a, const Real* b, colors clr )
{
    if( clr!=empty )
        color( clr );
    line( a,b );
}
void lines( const Real* a, const Real* b, colors clr )
{
    if( clr!=empty )color( clr );
    glBegin( GL_LINES );
    glVertex3dv( a );
    glVertex3dv( b );
    glVertex3d( a[0],-a[1],a[2] );
    glVertex3d( b[0],-b[1],b[2] );
    glEnd();
}
void circle( const Real *a, _Real r, bool fill )
{
    glBegin( fill?GL_POLYGON:GL_LINE_LOOP );
    for( Real a=0; a<_Pd; a+=_Ph/8 )
        glVertex3dv( (Vector){ r*sin( a ),r*cos( a ),0.0 } );
    glEnd();
}
// ... из точки (a) в точку (b) с объемной стрелкой в долях длины
//
void arrow( const Real *_a,const Real *_b,_Real l, colors clr )
{
    Point &a=*(Point*)_a,&b=*(Point*)_b;
    Vector d=a-b;
    d*=l/abs( d );
    Vector e= { d.z,d.x,d.y };
    e/=5.0;
    Vector f= { e.z,e.x,e.y };
    if( clr!=empty )color( clr );
    line( a,b+d );
    glBegin( GL_LINE_LOOP );
    point( b+d/2.0 ),point( b+d+e ),point( b ),point( b+d-e );
    point( b+d/2.0 ),point( b+d+f ),point( b ),point( b+d-f );
    point( b+d/2.0 );
    glEnd();
}

void arrow_point(const Point &a,const Point &b, _Real l, colors clr )
{
    //Point &a=*(Point*)_a,&b=*(Point*)_b;
    Vector d=a-b;
    d*=l/abs( d );
    Vector e= { d.z,d.x,d.y };
    e/=5.0;
    Vector f= { e.z,e.x,e.y };
    if( clr!=empty )
        color( clr );
    const Real *_a = (const Real *)&a;
    const Real *_b = (const Real *)&b;
    line( _a,b+d);
    glBegin( GL_LINE_LOOP );
    point( b+d/2.0 ),point( b+d+e ),point( _b ),point( b+d-e );
    point( b+d/2.0 ),point( b+d+f ),point( _b ),point( b+d-f );
    point( b+d/2.0 );
    glEnd();
}

void axis ( _Real L,_Real Y,_Real Z, const char *x,const char *y,const char *z,colors clr )
{
    const Real l=Real( L )/132;
    arrow( (Point){0,0,-Z},(Point){0,0,Z},l,clr ),
    arrow( (Point){0,-Y,0},(Point){0,Y,0},l, colors::empty ),
    arrow( (Point){-L,0,0},(Point){L,0,0},l, colors::empty );
    color( clr,-0.5 );
//    P.Text(_North,0,0,Z,z ),
//           P.Text(_North,0,Y+l,0,y ),
//           P.Text(_North_East,L+l,0,0,x);
}

//void axis ( Place &P,_Real L,_Real Y,_Real Z, const char *x,const char *y,const char *z,colors clr )
//{
//    const Real l=Real( L )/132;
//    arrow( (Point)
//    {
//        0,0,-Z
//    },(Point)
//    {
//        0,0,Z
//    },l,clr ),
//    arrow( (Point)
//    {
//        0,-Y,0
//    },(Point)
//    {
//        0,Y,0
//    },l ),
//    arrow( (Point)
//    {
//        -L,0,0
//        },(Point)
//    {
//        L,0,0
//    },l );
//    color( clr,-0.5 );
//    P.Text(_North,0,0,Z,z ),
//           P.Text(_North,0,Y+l,0,y ),
//           P.Text(_North_East,L+l,0,0,x);
//}
//View::View( const char* Title, int X,int Y, int W,int H, _Real Size )
//    : Window( Title,X,Y,W,H ),
//      eye( (Vector){ -130,-10,0 } ),look( (Vector){ 0,0,30 } ),
//      Distance( Size?-Size:-0.8*Width ),// расстояние от камеры до места съёмки
//      mx( 0 ),my( 0 )                 // указатель ставится в нулевое положение
//{ Activate(); View_initial();
//}
//Place& View::Draw(){ Activate();     // с исключением двойной перенастройки
//  glMatrixMode( GL_PROJECTION );     // размерах фрагмента экрана сброс текущих
//  glLoadIdentity();                  // матричных координат, настройка обзора и
//  gluPerspective( 16.2,Real( Width )/Height,-1,1 );     // экранных пропорций
//  gluLookAt( 0,0,Distance,look.x,look.y,look.z,0,1,0 ); // местоположение сцены
//  glMatrixMode( GL_MODELVIEW ); glLoadIdentity();       // в исходное положение
//  glRotated( eye.y-90,1,0,0 );                          // поставить на киль
//  glRotated( eye.z,   0,1,0 );                          // дифферент
//  glRotated( eye.x,   0,0,1 );                          // рыскание
//  return Place::Draw();
//}
////   Небольшой блок реагирования на мышиную возню в текущем окне
////
//Place& View::Mouse( int button, int x,int y )  // и её же указания к исполнению
// { bool ret=false; // Activate();
//   if( button==_MouseWheel )                  // на вращение колёсико / сенсора
//   { if( ret=(y!=0) ){ Distance += y*Distance/Height/3;
//                      // look.x += x*Distance/Width; mx-=x;
//                     } } else
//   if( ret=(x!=mx || y!=my) )   // местоположение курсора в локальных смещениях
//   { switch( button )             //  -- и не пустой вызов без свободной реакции
//     { case _MouseLeft:           //
//         if( ScanStatus()&CTRL )  //
//                eye.z+=Real( x-mx )/24,Distance-=( y-my )*Distance/Height; else
//                eye.y-=Real( y-my )/24,eye.x+=Real( x-mx )/16; break;
//       case _MouseRight: look.x-=( x-mx )*Distance/Width,       // смещение
//                         look.y-=( y-my )*Distance/Width; break;
////     case _MouseMiddle: y-=my;
//     } my=y,mx=x;
//   } if( ret )Draw();
//   return Place::Mouse( button,x,y ); //.Show().Save().Refresh()
// }
//bool View::KeyBoard( byte key )   // к спуску из внешних виртуальных транзакций
//{ static Real Di=0.0; if( !Di )Di=Distance;// запоминается из первого обращения
//         Real Ds=6*Distance/Width;
//  switch( key )
//  { case _Left: if( ScanStatus()&SHIFT )look.x+=Ds; else
//                if( ScanStatus()&CTRL  )eye.z--;    else eye.x--; break;
//    case _Right:if( ScanStatus()&SHIFT )look.x-=Ds; else
//                if( ScanStatus()&CTRL  )eye.z++;    else eye.x++; break;
//    case _Up:   if( ScanStatus()&CTRL  )Distance*=1.1; else
//                if( ScanStatus()&SHIFT )look.y+=Ds; else eye.y++; break;
//    case _Down: if( ScanStatus()&CTRL  )Distance/=1.1; else
//                if( ScanStatus()&SHIFT )look.y-=Ds; else eye.y--; break;
//    case _Home: Distance=Di,    // ... от точки взгляда до места обзора
//            eye=(Vector){ -120,-20,0 },look=(Vector){ -8,-4,30 }; break;
//   default: return Window::KeyBoard(key);// передача в цикл ожидания клавиатуры
//  } Draw(); return true;                 // либо - запрос от клавиатуры погашен
//}
//
// Настройка начальной раскраски и освещенности графического пространства/сцены
//              ... здесь необходима предварительная установка контекста OpenGL
void View_initial()
{
    glClearColor( 0.9,0.95,0.99,1 );      // светлый цвет экранного фона и затем
    glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT ); // полная расчистка окна
// glFrontFace( GL_CW );       // CCW видимы грани с обходом по часовой стрелке
// glCullFace ( GL_FRONT_AND_BACK ); // BACK-FRONT какие грани будут отбираться
// glEnable   ( GL_CULL_FACE );        // включение режима отбора треугольников
    glPointSize( 1.0 );
    glHint     ( GL_POINT_SMOOTH_HINT,GL_NICEST );
    glEnable   ( GL_POINT_SMOOTH );
    glLineWidth( 1.0 );
    glHint     ( GL_LINE_SMOOTH_HINT,GL_NICEST );
    glEnable   ( GL_LINE_SMOOTH );     // сглаживание линий
    glPolygonMode( GL_FRONT,GL_FILL );
    glPolygonMode( GL_BACK,GL_LINE ); //POINT );
    glShadeModel( GL_SMOOTH );       // FLAT закраска с использованием полутонов
    glHint     ( GL_POLYGON_SMOOTH_HINT,GL_NICEST );
    glEnable   ( GL_POLYGON_SMOOTH); // Really Nice Perspective Calculations
    glHint     ( GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST );
    glBlendFunc( GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA );

    glEnable   ( GL_DITHER );               // Предопределение графической среды
    glEnable   ( GL_ALPHA_TEST );
    glEnable   ( GL_BLEND );

    glFogi( GL_FOG_MODE,GL_EXP2 );
    glFogf( GL_FOG_DENSITY,0.0016 );
// glFogf( GL_FOG_START,Storm->Distance );
// glFogf( GL_FOG_END,-Storm->Distance );
    glHint     ( GL_FOG_HINT,GL_NICEST );
    glEnable   ( GL_FOG );
// glEnable   ( GL_STENCIL_TEST );
#if 1
    glLightModelfv( GL_LIGHT_MODEL_AMBIENT,(const float[])
    {
        0.8,1,0.9,0.75
    } );
    glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER,true );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE,true );

    glMaterialfv( GL_FRONT_AND_BACK,GL_AMBIENT, (const float[])
    {
        .2,.2,.2,1.
    } ); //=окружение
    glMaterialfv( GL_FRONT_AND_BACK,GL_DIFFUSE, (const float[])
    {
        .8,.8,.8,1.
    } ); //=рассеяние
    glMaterialfv( GL_FRONT_AND_BACK,GL_SPECULAR,(const float[])
    {
        .5,.5,.5,.5
    } ); // отражение
    glMaterialfv( GL_FRONT_AND_BACK,GL_EMISSION,(const float[])
    {
        .0,.0,.0,1.
    } ); //=излучение
    glMateriali( GL_FRONT_AND_BACK,GL_SHININESS,127 );// степень отсветки

    glLightfv( GL_LIGHT0,GL_AMBIENT, (const float[])
    {
        .1,.3,.2,.6
    } ); // окружение
    glLightfv( GL_LIGHT0,GL_DIFFUSE, (const float[])
    {
        .6,.8, 1, 1
    } ); // рассеяние
    glLightfv( GL_LIGHT0,GL_SPECULAR,(const float[])
    {
        .8,.9, 1, 1
    } ); // отражение
    glLightfv( GL_LIGHT0,GL_EMISSION,(const float[])
    {
        .6,.6,.6, 1
    } ); // излучение
    glLightfv( GL_LIGHT0,GL_POSITION,(const float[])
    {
        -2000,20,-100,1
        } );

    glLightfv( GL_LIGHT1,GL_AMBIENT, (const float[])
    {
        .1,.2,.3,.8
    } ); // окружение
    glLightfv( GL_LIGHT1,GL_DIFFUSE, (const float[])
    {
        .8, 1, 1, 1
    } ); // рассеяние
    glLightfv( GL_LIGHT1,GL_SPECULAR,(const float[])
    {
        .6,.8,.9, 1
    } ); // отражение
    glLightfv( GL_LIGHT1,GL_EMISSION,(const float[])
    {
        .6,.6,.6, 1
    } ); // излучение
    glLightfv( GL_LIGHT1,GL_POSITION,(const float[])
    {
        2000,100,10,1.0
    } );
#endif
    glEnable( GL_COLOR_MATERIAL );
    glEnable( GL_LIGHT0 );
    glEnable( GL_LIGHT1 );
    glEnable( GL_LIGHTING );
    glDepthRange( 1,0 );
    glDepthMask( GL_TRUE );
    glEnable( GL_DEPTH_TEST );        // растровая разборка отсечений по глубине
// glClearDepth( 2000.0 );           // Enables Clearing Of The Depth Buffer
    glDepthFunc( GL_LEQUAL );  // LESS GREATER ALWAYS взаимное накрытие объектов
    glEnable( GL_AUTO_NORMAL );
    glEnable( GL_NORMALIZE );
    glColorMaterial( GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE );
    for( int i=0; i<256; i++ )
        SeaColor[1+black+i][0]=byte(    pow(Real(i)/255,4)*180 ), // red   красный
                               SeaColor[1+black+i][1]=byte( 48+pow(Real(i)/255,2)*162 ), // green зеленый
                                       SeaColor[1+black+i][2]=byte( 96+pow(Real(i)/255,3)*94  ); // blue  синий
    for( int i=0; i<black+256; i++ )SeaColor[i][3]=255;         // alfa-сплошной
//
//FILE *F=fopen( "Vessel.mtl","wt" ); for( int i=0; i<black+257; i++ )
//fprintf( F,"newmtl c%d\nkd %0.3f %0.3f %0.3f\n",i, //<=black?-10:i-black-1,
// Real(SeaColor[i][0])/255,Real(SeaColor[i][1])/255,Real(SeaColor[i][2])/255 );
//fclose( F );
}
