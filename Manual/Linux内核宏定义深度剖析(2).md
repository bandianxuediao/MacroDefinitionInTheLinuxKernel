#Linux内核宏定义深度剖析(2)

##1、Linux 内核第一宏：container_of</br>
`#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)`
`#define  container_of(ptr, type, member) ({    \`
     `const typeof( ((type *)0)->member ) *__mptr = (ptr); \`
     `(type *)( (char *)__mptr - offsetof(type,member) );})`

![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-1.JPG)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-1.JPG)</br>


　　
[本章文件ex1.c](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-ex1.c)</br>
[本章文件ex1.c](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/TestCode/LinuxKernel-ex1.c)</br>















</br>
</br></br></br></br></br></br></br></br></br>