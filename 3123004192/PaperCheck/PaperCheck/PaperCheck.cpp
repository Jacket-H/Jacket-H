#include <iostream>
#include<fstream>
#include<unordered_map>
#include<unordered_set>
#include <map>
#include <string>

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
	for (int i = 0; i < data.size(); i++) 
	{
		unsigned char uc = static_cast<unsigned char>(data[i]);

		// 处理ASCII字符
		if (uc <= 127) 
		{
			if (isalnum(uc)) {
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

//相似度计算模块
//词（字）频计算
double WordFrequencyCalculation()
{

	return 1.0;
}
//哈希滚动计算
double HashRollingCalculation()
{
	return 1.0;
}
double SimilarityCalculation()
{
	return 1.0;
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
	filePathB = "orig_add.txt";
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
	message += paperA + "\n" + paperB;

	//输出
	Output(filePathC, message);

}

