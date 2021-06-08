
#ifndef MFP_2021_H_INCLUDED
#define MFP_2021_H_INCLUDED

#include <iostream>
#include "../Math/Volume.h"
#include <openacc.h>
#include <cmath>

extern                // признаки режимов движения
bool Gravity_mode,    // =false свободное или гравитационное перемещение частиц
     linear_Model;    // =false линейно-упругая или ядерная модель корпускул
extern                //
Real ks,              // 0.4  коэффициент сопротивления для шара [0.2-0.4]
     wall;            // 1.32 стенки аквариума (высокого, пока)
Real     init_Scale;      // 1.0  начальный масштаб для каждой частицы

Real Init_Flight_Points( Array& );          // на выходе размер ячейки-частицы
void One_Inter_Step_of_Point( Array&,bool );
void One_Flight_Step_of_Point( Array&,int );

const double           //
g=9.8106,        // ускорение свободного падения [м/с2];
R_Earth=6367554.0,    // радиус Земли, сфероид Красовского [м]
ro=1.29,         // плотность воздуха 1.29 [кг/м3];
Rm=0.2,          // радиус шарообразного тела [м] - 0.1
M=0.5;           // масса тела

#endif // MFP_2009-01-15_H_INCLUDED
