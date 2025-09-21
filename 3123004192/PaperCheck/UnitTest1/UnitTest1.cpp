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
			// ���Կ��ļ�����
			paperA = "";
			paperB = "";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::AreEqual(0.0, result);
		}

		TEST_METHOD(TestIdenticalContent)
		{
			// ������ȫ��ͬ������
			paperA = "hello world this is a test";
			paperB = "hello world this is a test";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::IsTrue(result > 0.95);
		}

		TEST_METHOD(TestCompletelyDifferent)
		{
			// ������ȫ��ͬ������
			paperA = "abcdefghijklmnopqrstuvwxyz";
			paperB = "1234567890!@#$%^&*()_+-=";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::IsTrue(result < 0.3);
		}

		TEST_METHOD(TestChineseCharacters)
		{
			// ���������ַ�
			paperA = "����һ���������ĵľ���";
			paperB = "����һ���������ĵľ���";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::IsTrue(result > 0.95);
		}

		TEST_METHOD(TestMixedChineseEnglish)
		{
			// ������Ӣ�Ļ��
			paperA = "hello����test����";
			paperB = "hello����test����";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::IsTrue(result > 0.95);
		}

		TEST_METHOD(TestShortText)
		{
			// ���Զ��ı���С��N-Gram���ȣ�
			paperA = "hi";
			paperB = "hi";
			charsA = SplitUTF8Chars(paperA);
			charsB = SplitUTF8Chars(paperB);

			double result = SimilarityCalculation();
			Assert::IsTrue(result >= 0.0 && result <= 1.0);
		}

		TEST_METHOD(TestWordFrequencyCalculation)
		{
			// ���Դ�Ƶ����
			charsA = { "a", "b", "c", "a", "b", "a" };
			charsB = { "a", "b", "c", "d", "a", "b" };

			double result = WordFrequencyCalculation();

			// ��֤��Ƶͳ����ȷ
			Assert::AreEqual(3, mapA["a"]);
			Assert::AreEqual(2, mapA["b"]);
			Assert::AreEqual(1, mapA["c"]);
			Assert::AreEqual(2, mapB["a"]);
			Assert::AreEqual(2, mapB["b"]);
			Assert::AreEqual(1, mapB["c"]);
			Assert::AreEqual(1, mapB["d"]);

			// �������ƶ�Ӧ����0.8����
			Assert::IsTrue(result > 0.7 && result < 0.9);
		}

		TEST_METHOD(TestStringPreprocessing)
		{
			// �����ַ���Ԥ����
			std::string input = "  Hello   World! 123  TEST  ";
			std::string result = StringPreprocessing(input);

			Assert::AreEqual(std::string("helloworld123test"), result);
		}

		TEST_METHOD(TestUTF8Split)
		{
			// ����UTF-8�ַ��ָ�
			std::string input = "hello����test";
			auto result = SplitUTF8Chars(input);

			Assert::AreEqual(9, (int)result.size()); // 5Ӣ�� + 2���� + 4Ӣ��
			Assert::AreEqual(std::string("h"), result[0]);
			Assert::AreEqual(std::string("e"), result[1]);
		}

		TEST_METHOD(TestHashRollingIdentical)
		{
			// ���Թ�ϣ�������㣨��ͬ���ݣ�
			charsA = { "a", "b", "c", "d", "e", "f", "g" };
			charsB = { "a", "b", "c", "d", "e", "f", "g" };

			double result = HashRollingCalculation();
			Assert::AreEqual(1.0, result);
		}

	private:
		// ��ÿ�����Խ���������ȫ��״̬
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
