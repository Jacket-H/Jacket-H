#include <iostream>
#include<fstream>
#include <vector>
#include <string>

std::vector<std::string>paperA, paperB;
std::string message;

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

	file >> data;
	while (file >> word)
	{
		data += " "+word;
	}

	return data;
}

//预处理模块
std::vector<std::string> StringPreprocessing(std::string data)
{
	return std::vector<std::string>();
}

//相似度计算模块
double WordFrequencyCalculation()
{
	return 1.0;
}
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


	//测试是否能输出
	//message += "输出测试\n";
	//测试是否能输入
	message += dataA + "\n" + dataB;

	//输出
	Output(filePathC, message);

}

