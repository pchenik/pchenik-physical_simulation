///                                          Ver 3.1 /1991.01.19 - 2012.05.27/
///     Числовые векторы и двухмерные пространства адресных последовательностей
/*
        При распределении памяти резервируется блок из четырех слов, что
            обеспечивает выравнивание на границу 16 байт, затем, в последнее
            слово блока заносится общая длина линейного массива в байтах,
            а для матриц - количество строк, соответственно.
        Если при вызове Allocate длина массива указывается нулевой,
            то происходит полное освобождение ранее выделенной памяти.
        Если адрес нулевой - выделяется новый массив, иначе происходит
            перераспределения памяти, при котором начальный фрагмент
            сохраняется, добавленный - очищается
        Если при распределении двумерного массива в качестве длины строк
            указывается 0, то реальное распределение памяти и управление типом
            данных и длиной строк не производится, т.е. эти дополнительные
            операции остаются в управлении вызывающей программы.
*/
#include <stdio.h>
#include <stdlib.h>

size_t isAlloc( void* );                            // На выходе длина в байтах
void *Allocate( size_t, void* );                    // Линейный массив в байтах
void **Allocate( size_t,size_t,void* );             // и по строкам - двумерный

#define Word 4u                        // при выравнивании на границу 4 адресов
#define Align (sizeof(size_t)*Word)    //     = 32 байт, или - 4*64=256 бит

inline void* ALset( size_t *A, const size_t L )
{
    *A++ =0, *A++ =0, *A++ =0, *A++ =L;
    return A;  // сдвиг на 4 слова int
}                                               //           или 128 бит
size_t isAlloc( void* A )                           // На выходе длина в байтах
{
    if( A )return ( (size_t*)A )[-1];
    else return 0;  //   или количество
}                                                   //     строк матрицы
void *Allocate( size_t s, void *A )                 // Линейный массив в байтах
{
    size_t S=0;                                       //  ++ 4 слова выравнивания
    if( s || A )                                      // длина или реальный адрес
    {
        if( !s )
        {
            free( ((size_t*)A)-Word );
            A=NULL;
        }
        else   // освобождаемый массив                                      //
        {
            if( !A )A=malloc( s+Align );
            else   // Новый массив или перераспределение
            {
                S = ((size_t*)A)[-1];             //  изначально заданная длина массива
                if( S!=s )A=realloc( ((size_t*)A)-Word,s+Align );
                else return A;
            }
            if( !A )
            {
                printf( "\n\7 No memory %d",s );
                exit( 1 );
            }
            else
            {
                A=ALset( (size_t*)A,s );               // расчистка метки, запись длины
                while( S<s )((char*)A)[S++]=0;         // массива и расчистка хвоста
            }
        }
    }
    return A;
}
//     y -     количество строк в матрице (списке)
//      y=0 -  второй параметр не анализируется, а сама матрица удаляется
//     s -     длина строки в байтах
//      s=0 -  новые строки не выделяются, а старые не изменяются
//
void **Allocate( size_t y, size_t s, void *A )
{
    size_t Y=0;
    if( y || A )                              // Для двумерной матрицы или списка
    {
        if( A )                                 // -- если массив уже есть, то
        {
            Y=((size_t*)A)[-1];                   // -- его размер в префиксном слове
            while( Y>y )Allocate( 0,((void**)A)[--Y] );  // освобождение пустых строк
            if( !Y )
            {
                free( ((size_t*)A)-Word );    // и полное освобождение
                A=NULL;
            }
        }
        if( y )                                            // Приведение к простому
        {
            if( A!=NULL )((size_t*)A)[-1] *= sizeof( A );    // линейному массиву и
            ((size_t*)(A=Allocate( y*sizeof( A ),A )))[-1]=y; // перераспределение
        }                                                  // граничного вектора
        if( s!=0 )                                         // иначе - все как есть
            for( Y=0; Y<y; Y++ )((void**)A)[Y]=Allocate( s,((void**)A)[Y] );
    }
    return (void**)A;
}
