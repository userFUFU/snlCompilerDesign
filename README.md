# snlCompilerDesign
 
小组成员：胡嘉仪，张蕾

### 总设计
在网页上输入源程序，存储为文件。  
处理程序以文件为输入，进行词法分析，形成Token序列。
    将词法错误存储到TokenError.txt中; Token序列输出到TokenResult.txt中。
以Token序列为输入，分别进行递归下降语法分析以及LL(1)语法分析，形成语法树。
    将语法错误存储于RDError.txt, LLError.txt; 语法树分别存储于RDResult.txt, LLResult.txt


### 词法分析
3.16 
+ 将直接输出转为存储在token结构体数组中，方便后续工作使用   (已完成)
+ 错误处理未完善 
+ lexicalDesign程序结构多处冗余，待改善

### 语法分析
3.19
+ 建立好语法分析相关头文件,考虑好构建语法树的数据结构 (大致完成)
+ LL(1)的Table (已完成)

3.23
+ LL(1) 分析法的predict部分 (产生式已写,与语法树构建相关的部分待做)


ExpIdK 即 VariK
TypeDef 即 TypeName

3.29
+ LL(1)语法分析已完成
+ print部分欠完成, 进不去递归 (已完成)

3.30
+ grammarTree 中全是NULL,由于指针问题 (已修改)
+ LL(1) debug 完成 ,RECORD 部分欠缺 (已完善)
+ LL(1)表达式左旋   (已解决)
+ 注 : case 39 多加了一个语法树上pop . 考虑 type var func  不完全有的情况

3.31
+ 递归下降 new 节点未填充信息(已完善)
+ 递归下降 debug (已完成)
+ 递归下降错误处理未全部完成 (基本完成，仍有一些细节上的错误未处理)

4.11
+ 已完成测试 （词法、语法）
+ 词法： end. 后若有内容，会识别成".(Record Member Symbol)" （已修改）
+ 递归下降 识别参数列表的时候很多错误 （已修改）

4.13
+ Gencode部分 (已完成)
+ 将所有输出分别写入文件 (已完成)
  