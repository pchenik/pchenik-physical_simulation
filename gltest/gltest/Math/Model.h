/**                                                                        ¹²³ⁿ
 *      Подборка геометрических фигур с собственным пространственным базисом
 *      (c)2020, -- ‏יְרוּשָׁלַיִם
 */


#ifndef Model_H_INCLUDED
#define Model_H_INCLUDED

#include "Tensor.h"
//#include "..\Window\View.h"

struct Model:Base
{ Model& operator =( _Real Scale ){ Base::operator=(Scale);  return *this; }
  Model& operator =( _Point Rc   ){ X=Rc.X, Y=Rc.Y, Z=Rc.Z;  return *this; }
  Model& operator+=( _Vector dR  ){ X+=dR.x,Y+=dR.y,Z+=dR.z; return *this; }
  Model& cubic( bool paint_Model=false ); // n>=2 - параллели; m>=3 - меридианы
  Model& torus ( _Real r,_Real R, int=33,int=24, bool=false );   // минимум:
  Model& sphere(                  int=33,int=24, bool=false );   // n=2, m=3
  Model& dipole( _Real accel=0.0,                bool=false );  /// {-1<s<+oo}
};
// Model_H_INCLUDED
#endif
