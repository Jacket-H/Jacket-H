#include "pch.h"
#include "CppUnitTest.h"
#include "../PaperCheck/math.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(TestEmptyFiles)
		{
			// 测试空文件输入
			paperA = "";
			paperB = "";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::AreEqual(0.0, result);
		}

		TEST_METHOD(TestIdenticalContent)
		{
			// 测试完全相同的内容
			paperA = "hello world this is a test";
			paperB = "hello world this is a test";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::IsTrue(result > 0.95);
		}

		TEST_METHOD(TestCompletelyDifferent)
		{
			// 测试完全不同的内容
			paperA = "abcdefghijklmnopqrstuvwxyz";
			paperB = "1234567890!@#$%^&*()_+-=";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::IsTrue(result < 0.3);
		}

		TEST_METHOD(TestChineseCharacters)
		{
			// 测试中文字符
			paperA = "这是一个测试中文的句子";
			paperB = "这是一个测试中文的句子";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::IsTrue(result > 0.95);
		}

		TEST_METHOD(TestMixedChineseEnglish)
		{
			// 测试中英文混合
			paperA = "hello世界test测试";
			paperB = "hello世界test测试";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::IsTrue(result > 0.95);
		}

		TEST_METHOD(TestShortText)
		{
			// 测试短文本（小于N-Gram长度）
			paperA = "hi";
			paperB = "hi";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::IsTrue(result >= 0.0 && result <= 1.0);
		}

		TEST_METHOD(TestWordFrequencyCalculation)
		{
			// 测试词频计算
			charsA = { "a", "b", "c", "a", "b", "a" };
			charsB = { "a", "b", "c", "d", "a", "b" };

			double result = WordFrequencyCalculation();

			// 验证词频统计正确
			Assert::AreEqual(3, mapA["a"]);
			Assert::AreEqual(2, mapA["b"]);
			Assert::AreEqual(1, mapA["c"]);
			Assert::AreEqual(2, mapB["a"]);
			Assert::AreEqual(2, mapB["b"]);
			Assert::AreEqual(1, mapB["c"]);
			Assert::AreEqual(1, mapB["d"]);

			// 余弦相似度应该在0.8左右
			Assert::IsTrue(result > 0.7 && result < 0.9);
		}

		TEST_METHOD(TestStringPreprocessing)
		{
			// 测试字符串预处理
			std::string input = "  Hello   World! 123  TEST  ";
			std::string result = StringPreprocessing(input);

			Assert::AreEqual(std::string("helloworld123test"), result);
		}

		TEST_METHOD(TestUTF8Split)
		{
			// 测试UTF-8字符分割
			std::string input = "hello世界test";
			auto result = SplitUTF8Chars(input);

			Assert::AreEqual(9, (int)result.size()); // 5英文 + 2中文 + 4英文
			Assert::AreEqual(std::string("h"), result[0]);
			Assert::AreEqual(std::string("e"), result[1]);
		}

		TEST_METHOD(TestHashRollingIdentical)
		{
			// 测试哈希滚动计算（相同内容）
			charsA = { "a", "b", "c", "d", "e", "f", "g" };
			charsB = { "a", "b", "c", "d", "e", "f", "g" };

			double result = HashRollingCalculation();
			Assert::AreEqual(1.0, result);
		}

	private:
		// 在每个测试结束后清理全局状态
		TEST_METHOD_CLEANUP(TestCleanup)
		{
			mapA.clear();
			mapB.clear();
			setA.clear();
			message.clear();
			paperA.clear();
			paperB.clear();
			charsA.clear();
			charsB.clear();
		}
	};
}
