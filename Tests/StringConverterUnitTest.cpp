#ifdef _UNITTESTS_
#include "CppUnitTest.h"

#include <Core\String.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(StringConverterUnitTest)
	{
	public:

		TEST_METHOD(Utf8ToUtf16)
		{
			//Core::String test = "�";
			//std::wstring expect = L"�";
			//
			//Assert::AreEqual(con.toUtf16(test), expect);
			Assert::Fail(L"Failed due to compiler failing to compile above characters");

		}

		TEST_METHOD(Utf16ToUtf8)
		{
			//Core::String expect = "�";
			//std::wstring test= L"�";
			//Core::String result = con.toUtf8(test);
			//Assert::AreEqual(result, expect);
			Assert::Fail(L"Failed due to compiler failing to compile above characters");
		}
	};

}

#endif