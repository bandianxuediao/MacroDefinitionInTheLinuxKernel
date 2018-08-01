#include <stdio.h>
//#define MAX(a,b) a>b:a:b
//#define MAX(a,b) (a)>(b)?(a):(b)
//#define MAX(a,b) ((a)>(b)?(a):(b))
//#define MAX(a,b) ({\
//		int _x=(a);\
//		int _y=(b);\
//		_x>_y?_x:_y;})
//#define MAX(type,a,b) ({\
//                type _x=(a);\
//                type _y=(b);\
//                _x>_y?_x:_y;})
#define MAX(a,b) ({\
              typeof(a) _x=(a);\
              typeof(b) _y=(b);\
              (void) (&_x == &_y);      \
              _x>_y?_x:_y;})

int main()
{
//int a=3,b=5;
float a=2.1,b=1.6;
printf("a=%f , b=%f \r\n",a,b);
//printf("The Max Num is:%d\r\n",MAX(a,b));
//printf("The Max Num is:%d\r\n",MAX(a!=3,b!=3));
//printf("The Result is: %d\r\n",a+MAX(a++,b++));
//printf("a=%d , b=%d \r\n",a,b);
//printf("The Max Num is %f\r\n",MAX(float,a,b));
printf("Use typeof result is %f\r\n",MAX(a,5));
}
