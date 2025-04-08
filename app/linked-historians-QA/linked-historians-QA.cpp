#include "CppUnitTest.h"
#include "utilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace linkedhistoriansQA
{
	TEST_CLASS(linkedhistoriansQA)
	{
	public:
		
        TEST_METHOD(TestIsValidDateFormat)
        {
            using namespace utilities;

            Assert::IsTrue(isValidDateFormat("7/4/2025"));
            Assert::IsTrue(isValidDateFormat("01/01/2020"));
            Assert::IsFalse(isValidDateFormat("32/1/2023"));
            Assert::IsFalse(isValidDateFormat("12/13/2023"));
            Assert::IsFalse(isValidDateFormat("29/2/2023"));
            Assert::IsTrue(isValidDateFormat("29/2/2024"));
            Assert::IsFalse(isValidDateFormat("1-1-2023"));
            Assert::IsFalse(isValidDateFormat("")); 
        }
        TEST_METHOD(TestCreateEventDataIntegrity)
        {
            using namespace utilities;

            std::string title = "Test_752";
            std::string info = "testing of the functionality.";
            std::string type = "Battle";
            std::string dateStr = "7/4/2025";

            EVENT* newEvent = new EVENT{ title, 0, 0, 0, info, nullptr, type };
            formatDate(dateStr, newEvent);

            Assert::AreEqual(title, newEvent->title);
            Assert::AreEqual(info, newEvent->info);
            Assert::AreEqual(type, newEvent->type);
            Assert::AreEqual(7, newEvent->dateDay);
            Assert::AreEqual(4, newEvent->dateMonth);
            Assert::AreEqual(2025, newEvent->dateYear);

            delete newEvent;
        }

	};
}
