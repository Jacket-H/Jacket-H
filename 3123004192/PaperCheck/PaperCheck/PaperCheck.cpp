#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <cmath>
#include <iomanip>

std::unordered_map<std::string, int>mapA, mapB;
std::unordered_set<std::string>setA;
std::string message, paperA, paperB;

//输入模块
std::string ReadFile(std::string filePath)
{
	std::string data, word;
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		message += "警告：" + filePath + "路径无法读取！\n";
		return data;
	}
	//这一步直接去除空格
	while (file >> word)
	{
		data +=word;
	}

	return data;
}

//预处理模块
//去除空格，顿号，换行等符号，并且将大写转换为小写，保留顺序
std::string StringPreprocessing(std::string data)
{
	std::string res;
	for (size_t i = 0; i < data.size(); i++) 
	{
		unsigned char uc = static_cast<unsigned char>(data[i]);

		// 处理ASCII字符
		if (uc <= 127) 
		{
			if (isalnum(uc)) 
			{
				res += tolower(uc);
			}
		}
		// 处理中文字符
		else if(uc >= 0xE0 && uc <= 0xEF && i + 2 < data.size()) 
		{
			unsigned char c2 = static_cast<unsigned char>(data[i + 1]);
			unsigned char c3 = static_cast<unsigned char>(data[i + 2]);

			// 检查是否是中文符号
			bool isPunctuation =
				(uc == 0xEF && (c2 == 0xBC || c2 == 0xBD)) ||  // ！，
				(uc == 0xE3 && c2 == 0x80) ||                   // 。、
				(uc == 0xE2 && c2 == 0x80) ||                   // 「」
				(uc == 0xEF && c2 == 0xBC && c3 == 0x9F) ||     // ？
				(uc == 0xEF && c2 == 0xBC && c3 == 0x9A) ||     // ：
				(uc == 0xEF && c2 == 0xBC && c3 == 0x9B);       // ；

			if (!isPunctuation) 
			{
				res += data[i];
				res += data[i + 1];
				res += data[i + 2];
			}
			i += 2;
		}
		else if (uc >= 0xC0 && uc <= 0xDF && i + 1 < data.size()) 
		{
			i += 1; 
		}
		else if (uc >= 0xF0 && uc <= 0xF7 && i + 3 < data.size()) 
		{
			i += 3; 
		}
	}
	return res;
}
//对文本进行分割
std::vector<std::string> SplitUTF8Chars(const std::string& text) 
{
	std::vector<std::string> result;
	for (size_t i = 0; i < text.size();) 
	{
		unsigned char c = static_cast<unsigned char>(text[i]);
		size_t charLen = 1;
		if ((c & 0x80) == 0) 
		{
			//英文/数字 (ASCII)
			charLen = 1;
		}
		else if ((c & 0xE0) == 0xC0) 
		{
			//2字节字符
			charLen = 2;
		}
		else if ((c & 0xF0) == 0xE0) 
		{
			//3字节字符
			charLen = 3;
		}
		else if ((c & 0xF8) == 0xF0) 
		{
			//4字节字符
			charLen = 4;
		}

		result.push_back(text.substr(i, charLen));
		i += charLen;
	}
	return result;
}

//相似度计算模块
//词（字）频计算
double WordFrequencyCalculation()
{
	//将字的出现次数计入map中
	for (auto& s : SplitUTF8Chars(paperA)) {
		mapA[s]++;
	}
	for (auto& s : SplitUTF8Chars(paperB)) {
		mapB[s]++;
	}

	if (mapA.empty() || mapB.empty()) return 0.0;

	//统计所有词汇（字）
	std::unordered_set<std::string>allWords;
	for (auto i : mapA) allWords.insert(i.first);
	for (auto i : mapB) allWords.insert(i.first);
	//创建频率向量
	std::vector<double> vec1, vec2;
	for (auto i : allWords)
	{
		vec1.push_back(mapA.count(i) ? mapA.at(i) : 0);
		vec2.push_back(mapB.count(i) ? mapB.at(i) : 0);
	}
	//余弦计算
	double dotProduct = 0.0, norm1 = 0.0, norm2 = 0.0;
	for (size_t i = 0; i < allWords.size(); i++)
	{
		dotProduct += vec1[i] * vec2[i];
		norm1 += vec1[i] * vec1[i];
		norm2 += vec2[i] * vec2[i];
	}
	norm1 = std::sqrt(norm1);
	norm2 = std::sqrt(norm2);
	if (!norm1 || !norm2) return 0.0;
	return dotProduct / (norm1*norm2);
}
//哈希滚动计算
double HashRollingCalculation()
{
	return 1.0;
}
//相似度统计
double SimilarityCalculation()
{
	double ans=0.0;
	ans += 1.0*WordFrequencyCalculation();
	ans += 0.0*HashRollingCalculation();
	return ans;
}

//输出模块
void Output(std::string filePath, std::string message)
{
	std::ofstream file(filePath);

	if (file.is_open())
	{
		file << message << std::endl;
	}
	else
	{
		std::cout << "无法打开文件！" << std::endl;
	}
	return;
}

int main(int argc, char *argv[])
{
	//文件路径
	std::string filePathA, filePathB, filePathC;
	filePathA = "orig.txt";
	filePathB = "orig_0.8_x.txt";
	filePathC = "output.txt";
	
	//从命令行参数中传入文件路径
	if (argc > 1)
	{
		if (argc != 4)
		{
			std::cout << "参数数量错误！" << std::endl;
			return 0;
		}
		filePathA = argv[1];
		filePathB = argv[2];
		filePathC = argv[3];
	}

	//读取文件
	std::string dataA, dataB;
	dataA = ReadFile(filePathA);
	dataB = ReadFile(filePathB);

	//预处理
	paperA = StringPreprocessing(dataA);
	paperB = StringPreprocessing(dataB);

	//测试是否能输出
	//message += "输出测试\n";
	//测试是否能输入
	//message += dataA + "\n" + dataB;
	//测试是否能预处理
	message += paperA + "\n" + paperB + "\n";
	//重复率输出
	std::ostringstream ansOstring;
	std::string ansString;
	ansOstring << std::fixed << std::setprecision(2) << SimilarityCalculation()*100.0;	//保留两位小数
	ansString = ansOstring.str();
	message += u8"原论文:" + filePathA + "\n";
	message += u8"检查论文" + filePathB + "\n";
	message += u8"重复率计算结果：" + ansString + "%";

	//输出
	Output(filePathC, message);

}

