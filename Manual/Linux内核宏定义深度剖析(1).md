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
![七牛云](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-1.JPG)</br>
![GitHub](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-1.JPG)</br>
　　从执行结果可以看出来我刚才编写的这个宏定义
### 第一次变形
　　但是总有特殊的条件存在嘛，像这种`特殊`条件的存在，如果你在编程的时候没有考虑进去的话，这就是一个BUG。</br>
　　按照王老师的教程我修改printf如下：</br>
　　`printf("The Max Num is:%d\r\n",MAX(a!=3,b!=3));`</br>
　　自己分析下预期的输出结果应该是：1</br>
　　看下在Linux环境下的真是处理结果：</br>
![七牛云](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-2.JPG)</br>
![GitHub](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-2.JPG)</br>
　　分析一下代码，上面的MAX展开之后应该是这样：</br>
　　`MAX(a!=3>b!=3?a!=3:b!=3)`</br>
　　贴上来一张C/C++运算符优先级的表来供参考：
![七牛云](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-3.jpg)</br>
![GitHub](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-3.jpg)</br>
　　运算符的优先级直接打乱了预期的执行过程，工作初期经常因为优先级的问题而写了很多的BUG。后面学聪明了，不管优先级怎么样，直接按照自己的预期流程添加小括号`()`。那么按照这个思路是修改调用代码还是宏定义？正常的编程思想是需要修改宏定义的，一劳永逸。那么我修改宏定义如下：</br>
　　`#define MAX(a,b) (a)>(b)?(a):(b)`</br>
　　再次`gcc ex1.c -o ex1.out`编译`ex1.c`文件。</br>
　　`./ex1.out`运行刚编译完成的文件。</br>
　　在Linux环境下的执行结果如下图所示：</br>
![七牛云](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-4.jpg)</br>
![GitHub](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-4.jpg)</br>
　　从运行结果可以清楚地看到我得到了预期想要的结果，第一次变形成功。</br>
### 第二次变形</br>
　　再次根据王老师的教程修改代码：</br>
　　`printf("The Result is %d",a+MAX(a,b));`</br>
　　预期结果是输出 8 ，那看下Linux环境下真正的输出结果：</br>
![七牛云](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-5.jpg)</br>
![GitHub](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-5.jpg)</br>
　　为什么会有这个结果，其实分析一下代码就真相大白了：</br>
　　`a+a>b?a:b`->`3+3>5?3:5`</br>
　　那有了第一次的变形经验，很快就想到了再加一个小括号，也就是如果展开的表达式是这样子的就不会有问题了：`3+(3>5?3:5)`。</br>
　　于是我修改宏定义如下：</br>
　　`#define MAX(a,b) ((a)>(b)?(a):(b))`</br>
　　再次运行瞅一眼运行结果：</br>
![七牛云](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-6.jpg)</br>
![GitHub](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-6.jpg)</br>
　　确认没问题，结果是 8 。</br>
### 第三次变形</br>
　　再次修改代码如下：</br>
　　`printf("The Result is %d",a+MAX(a++,b++));`</br>
　　这个预期的输出结果是 9 ，运算之后 a=4 , b=6 。</br>
   实际运行结果如下：</br>
![七牛云](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-7.JPG)</br>
![GitHub](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-7.JPG)</br>
　　带入宏定义展开之后是：`a+((a++)>(b++)?(a++):(b++))`</br>
　　通过展开的式子可以很明显的看到计算之后 a=4 b=7 ,因为 a 和 b 都进行了两次自加运算。有可能会有人就是想要这种结果，但是这么隐晦、不受控的式子真的不建议去使用。</br>
　　既然这样，那这个宏定义该怎么修改才能避免在调用MAX的时候参数自加两次呢？我可以在刚进入宏的时候就把参数的值取出来赋给另外的新变量，修改如下：</br>
`#define MAX(a,b) ({\`</br>
　　　　　　　　　　`int _x=(a);\`</br>
　　　　　　　　　　`int _y=(b);\`</br>
　　　　　　　　　　`_x>_y?_x:_y;})`</br>
　　看下运行结果：</br>
![七牛云](http://pcnwqhy39.bkt.clouddn.com/LinuxKernel-1-8.JPG)</br>
![GitHub](https://github.com/bandianxuediao/MacroDefinitionInTheLinuxKernel/blob/master/Photo/LinuxKernel-1-8.JPG)</br>
　　这个 9 的结果是 4+5=9 得来的，4是MAX运算之后的a的值；`MAX(a,b)`就是要返回两个参数里面最大的那个，对于我的式子来说就是要返回 a++ 和 b++ 里面的最大值，也就是 b++ 。所以结果是 4+5=9。</br>
　　如果对`４`有疑问的话可以参考。。。。（补）</br>






















</br>
</br></br></br></br></br></br></br></br></br>