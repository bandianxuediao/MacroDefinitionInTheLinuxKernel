#Linux内核宏定义深度剖析(1)

##1、<定义一个宏，求两个数的最大值></br>
这个问题在嵌入式笔试题中出场率相当的高，能写出符合要求的宏很简单，但要写出完美的宏却还是有些难度的。</br>
### 符合要求的宏如下：</br>
　　`#define MAX(a,b) a>b?a:b`</br>
　　多数C语言初学者顺手就能写出这个宏（PS,倘若你暂时连这个都没想到的话还是过段时间再换工作吧。），一个三目运算符直接返回最大值。这个宏在一般情况下也没什么问题，完全可以使用。</br>
　　在linux环境下验证（我安装的Ubuntu）：</br>
　　在终端输入：`mkdir C`创建一个文件夹，用来存储我学习过程中创建的文件。</br>
　　`cd C`打开刚刚创建的`C文件夹`</br>
　　`touch ex1.c`创建一个空文件（直接使用`vi`可以省略这一步）</br>
　　`vi ex1.c`打开`ex1.c`文件</br>
　　我输入以下代码：</br>
　　`#include <stdio.h>`</br>
　　`#define MAX(a,b) a>b?a:b`</br>
　　`int main()`</br>
　　`{`</br>
　　`int a=3,b=5;`</br>
　　`printf("a=%d , b=%d ",a,b);`</br>
　　`printf("The Max Num is:%d",MAX(a,b));`</br>
　　`}`</br>
　　输入`：wq`保存退回终端窗口。</br>
　　`gcc ex1.c -o ex1.out`在终端窗口输入编译`ex1.c`，并生成可执行文件的命令。
　　`./ex1.out`输入命令执行我刚才编写的代码，执行结果如下图所示(相关文件请在本人GitHub中查看、下载)：
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-1.JPG)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-1.JPG)</br>
　　从执行结果可以看出来我刚才编写的这个宏定义
### 第一次变形
　　但是总有特殊的条件存在嘛，像这种`特殊`条件的存在，如果你在编程的时候没有考虑进去的话，这就是一个BUG。</br>
　　按照王老师的教程我修改printf如下：</br>
　　`printf("The Max Num is:%d\r\n",MAX(a!=3,b!=3));`</br>
　　自己分析下预期的输出结果应该是：1</br>
　　看下在Linux环境下的真是处理结果：</br>
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-2.JPG)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-2.JPG)</br>
　　分析一下代码，上面的MAX展开之后应该是这样：</br>
　　`MAX(a!=3>b!=3?a!=3:b!=3)`</br>
　　贴上来一张C/C++运算符优先级的表来供参考：
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-3.jpg)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-3.jpg)</br>
　　运算符的优先级直接打乱了预期的执行过程，工作初期经常因为优先级的问题而写了很多的BUG。后面学聪明了，不管优先级怎么样，直接按照自己的预期流程添加小括号`()`。那么按照这个思路是修改调用代码还是宏定义？正常的编程思想是需要修改宏定义的，一劳永逸。那么我修改宏定义如下：</br>
　　`#define MAX(a,b) (a)>(b)?(a):(b)`</br>
　　再次`gcc ex1.c -o ex1.out`编译`ex1.c`文件。</br>
　　`./ex1.out`运行刚编译完成的文件。</br>
　　在Linux环境下的执行结果如下图所示：</br>
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-4.jpg)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-4.jpg)</br>
　　从运行结果可以清楚地看到我得到了预期想要的结果，第一次变形成功。</br>
### 第二次变形</br>
　　再次根据王老师的教程修改代码：</br>
　　`printf("The Result is %d",a+MAX(a,b));`</br>
　　预期结果是输出 8 ，那看下Linux环境下真正的输出结果：</br>
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-5.jpg)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-5.jpg)</br>
　　为什么会有这个结果，其实分析一下代码就真相大白了：</br>
　　`a+a>b?a:b`->`3+3>5?3:5`</br>
　　那有了第一次的变形经验，很快就想到了再加一个小括号，也就是如果展开的表达式是这样子的就不会有问题了：`3+(3>5?3:5)`。</br>
　　于是我修改宏定义如下：</br>
　　`#define MAX(a,b) ((a)>(b)?(a):(b))`</br>
　　再次运行瞅一眼运行结果：</br>
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-6.jpg)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-6.jpg)</br>
　　确认没问题，结果是 8 。</br>
### 第三次变形</br>
　　再次修改代码如下：</br>
　　`printf("The Result is %d",a+MAX(a++,b++));`</br>
　　这个预期的输出结果是 9 ，运算之后 a=4 , b=6 。</br>
   实际运行结果如下：</br>
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-7.JPG)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-7.JPG)</br>
　　带入宏定义展开之后是：`a+((a++)>(b++)?(a++):(b++))`</br>
　　通过展开的式子可以很明显的看到计算之后 a=4 b=7 ,因为 b 进行了两次自加运算。有可能会有人就是想要这种结果，但是这么隐晦、不受控的式子真的不建议去使用。</br>
　　既然这样，那这个宏定义该怎么修改才能避免在调用MAX的时候参数自加两次呢？我可以在刚进入宏的时候就把参数的值取出来赋给另外的新变量，修改如下：</br>
`#define MAX(a,b) ({\`</br>
　　　　　　　　　　`int _x=(a);\`</br>
　　　　　　　　　　`int _y=(b);\`</br>
　　　　　　　　　　`_x>_y?_x:_y;})`</br>
　　看下运行结果：</br>
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-8.JPG)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-8.JPG)</br>
　　这个 9 的结果是 4+5=9 得来的，4是MAX运算之后的a的值；`MAX(a,b)`就是要返回两个参数里面最大的那个，对于我的式子来说就是要返回 a++ 和 b++ 里面的最大值，也就是 b++ 。所以结果是 4+5=9。</br>
　　如果对`４`有疑问的话可以参考。。。。（补）</br>

### 第四次变形</br>
　　上面第三次变形虽然完美解决了混乱的问题，但是它又带来了另外一个不容忽视的BUG。我定义了MAX宏里面的 _x 和 _y 是 int 型的，那如果需要比较的参数是其他不定类型呢？比如`MAX(2.1,1.6)`,这个时候总不能每一个不同的类型就单独定义一个MAX宏吧。将参数的类型当作一个参数传进来可能是此时此刻我能想到的最好的办法了。于是我修改MAX宏如下：</br>
`#define MAX(type,a,b) ({\`</br>
　　　　　　　　　　`type _x=(a);\`</br>
　　　　　　　　　　`type _y=(b);\`</br>
　　　　　　　　　　`_x>_y?_x:_y;})`</br>
　　这样的话我的式子可以写成`MAX(float,2.1,1.6);`</br>
　　Linux环境下的运行结果如下：</br>
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-9.JPG)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-9.JPG)</br>
　　OK，这个宏已经是非常完美了。但是在Linux环境GCC编译器下还能够更加完美，请看第五次变形。</br>
### 第五次变形</br>
　　ANSI C 定义了 sizeof 关键字，用来获取一个变量或数据类型在内存中所占的存储字节数。GNU C 扩展了一个关键字 typeof，用来获取一个变量或表达式的类型。这里使用关键字可能不太合适，因为毕竟 typeof 还没有被写入 C 标准，是 GCC 扩展的一个关键字。<br>
　　通过使用 typeof，我们可以获取一个变量或表达式的类型。所以 typeof 的参数有两种形式：表达式或类型。
　　OK，得到了上面的这个概念之后是不是突然间觉得第四次变形的宏定义还可以再次升级呢？当然可以，我来验证一下，最后一次变形了，直接贴出来ex1.c的整个截图，文件附在本篇结尾。
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-10.JPG)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-10.JPG)</br>
　　少不了的运行结果：</br>
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-11.JPG)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-11.JPG)</br>

　　如果自己能把这个宏写到这种程度就已经很厉害了，但是LinuxKernel贡献者考虑的更多，贴上来求最大最小值在LinuxKernel中的写法：</br>
`#define min(x, y) ({                \`</br>
　　`typeof(x) _min1 = (x);          \`</br>
　　`typeof(y) _min2 = (y);          \`</br>
　　`(void) (&_min1 == &_min2);      \`</br>
　　`_min1 < _min2 ? _min1 : _min2; })`</br>

`#define max(x, y) ({                \`</br>
　　`typeof(x) _max1 = (x);          \`</br>
　　`typeof(y) _max2 = (y);          \`</br>
　　`(void) (&_max1 == &_max2);      \`</br>
　　`_max1 > _max2 ? _max1 : _max2; })`</br>

　　仔细看上面的宏定义，比我写的多了一行：</br>
　　`(void) (&_max1 == &_max2);      \`</br>
　　根据王利涛老师的讲解，我理解如下：</br>
　　(&_max1 == &_max2)这个比较本身来说是没有任何意义的，并且整个宏也没有使用它的比较结果。没有使用比较结果是不是就没有用呢？再看下&_max1是变量_max1的地址，那两个变量的地址肯定是不相同的啊。然而，两个地址做对比的时候会涉及到各自的指针类型对比，当两个指针类型不相同的地址做对比时编译器会发出警告，提示指针类型不同，我自己测试结果如图：</br>
![七牛云备用链接](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-12.JPG)</br>
![GitHub备用链接](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-12.JPG)</br>
　　我输入的参数是MAX(2.1,5);根据上面的编译结果可以看到确实有了指针类型不同的警告。
　　第一章讲解完成之后是不是特别佩服LinuxKernel的贡献者，所以接下来在学习Linux的过程中我会不断地找到一些很棒的Linux的宏定义来和大家分享。</br>
　　更多好看好玩儿的东西请关注我的个人网站：[文禾每](www.wenhemei.com)</br>


　　
[本章文件ex1.c](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-ex1.c)</br>
[本章文件ex1.c](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/TestCode/LinuxKernel-ex1.c)</br>















</br>
</br></br></br></br></br></br></br></br></br>