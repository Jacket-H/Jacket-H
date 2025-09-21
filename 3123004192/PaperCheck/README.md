| 这个作业属于哪个课程 | <https://edu.cnblogs.com/campus/gdgy/Class12Grade23ComputerScience> |
| ---- | ---- |
| 这个作业要求在哪里 | <https://edu.cnblogs.com/campus/gdgy/Class12Grade23ComputerScience/homework/13468> |
| 这个作业的目标 | 进行一次开发，熟悉软件开发流程，掌握必备技巧 |

Github链接：<https://github.com/Jacket-H/Jacket-H>
作业Releases链接：<https://github.com/Jacket-H/Jacket-H/releases/tag/v1.0.0>
<br>
### 模块设计
主要可分为以下四个模块：
* 输入模块：负责从文件中读取论文的部分
* 预处理模块：去除论文的空格、标点、换行符，并将所有大写转换为小写后，进行分词
* 相似度计算：采用词频向量检查和哈希检查的混合方式，计算重复率
* 输出模块：将结果输出至文件
  
### 模块接口设计
- `main(int argc, char *argv[])`链接所有函数，并读取命令行参数
#### 输入模块
- `ReadFile(std::string filePath)`从文件中读取论文，输出string字符串
#### 预处理模块
- `StringPreprocessing(std::string data)`对string字符串进行处理，去除符号，将大写字母转换为小写
- `SplitUTF8Chars(std::string &text)`对处理后的字符串进行单字分割，确保**中文字符**能被正确分割而不是**变成乱码**，用于传递给核心算法模块进行运算
#### 相似度计算模块
- `SimilarityCalculation()`调用一下两个算法并对结果进行混合
  - `WordFrequencyCalculation()`词汇（单字）频率统计+余弦相似度计算
  - `HashRollingCalculation()`字符串哈希+Jaccard相似度计算
#### 输出模块
- `void Output(std::string filePath , std::string message)`将要输出的信息输出到文件中

### 算法的关键以及独到之处
单字频率统计采用余弦相似度计算，然后是滚动哈希记录字串，接着Jaccard进行相似度计算。
**单字频率**的方法能够有效处理打乱语序的抄袭操作，然而由于无法检测前后文顺序，存在**误判情况**，长论文对比极短论文时也容易出现**极大误差**。
**哈希记录字串+Jaccard相似度计算**能够直接发现复制粘贴类型的抄袭操作，在子串长度设置合理的情况下不容易误判，然而问题在于容易**被打乱语序的方式干扰**。

考虑到两种方法都具有缺陷，本次采用两种算法混合的方式。

### 计算模块接口部分的性能改进
以下是性能分析工具的分析结果
![image](https://img2024.cnblogs.com/blog/3699281/202509/3699281-20250922020014561-708782500.png)
![image](https://img2024.cnblogs.com/blog/3699281/202509/3699281-20250922020240154-1229011238.png)
非常容易能观察到，`WordFrequencyCalculation()`和`HashRollingCalculation()`这两个核心算法模块占据了95%的性能消耗

![image](https://img2024.cnblogs.com/blog/3699281/202509/3699281-20250922020642180-1072380995.png)
而进一步放大能够发现，在这两个核心算法中`std::Hash`和`SplitUTF8Chars(std::string &text)`分别占用了近35%和38%的性能消耗。
可以发现有两个优化方向：
1.将哈希优化为滚动哈希
2.减少`SplitUTF8Chars(std::string &text)`的重复调用

### 计算模块部分异常处理说明
##### 文件读取异常
若是文件无法读取，则将输入空文件，并输出警告于输出文件中
##### 输入空文件
原论文或抄袭版论文输入为空时，查重率将为零
<br>
#PSP2表格

| PSP2.1 | Personal Software Process Stages | 预估耗时（分钟） | 实际耗时（分钟） |
| ---- | ---- | ---- | ---- |
| Planning | 计划 | 20 | 7 |
| Estimate | 估计这个任务需要多少时间 | 520 | 607 |
| Development | 开发 | 430 | 515 |
| Analysis | 需求分析 (包括学习新技术) | 30 | 39 |
| Design Spec | 生成技术文档 | 60 | 48 |
| Design Review | 设计复审 | 20 | 12 |
| Coding Standard | 代码规范 (为目前的开发制定合适的规范) | 20 | 5 |
| Design | 具体设计 | 40 | 22 |
| Coding | 具体编码 | 120 | 250 |
| Code Review | 代码复审 | 20 | 9 |
| Test | 测试（自我测试，修改代码，提交修改） | 120 | 130 |
| Reporting | 报告 | 90 | 92 |
| Test Repor | 测试报告 | 40 | 67 |
| Size Measurement | 计算工作量 | 30 | 15 |
| Postmortem & Process Improvement Plan | 事后总结，并提出过程改进计划 | 20 | 10 |

