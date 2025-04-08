#include "CppUnitTest.h"
#include "utilities.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace linkedhistoriansQA
{
	TEST_CLASS(linkedhistoriansQA)
	{
	public:
		
        TEST_METHOD(testIsValidDateFormat)
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
        TEST_METHOD(testCreateEventDataIntegrity)
        {
            using namespace utilities;

            std::string title = "Test_752";
            std::string info = "Testing of the functionality.";
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
        TEST_METHOD(testGetValidInput)
        {
            using namespace utilities;

            std::string input1 = "Hello";
            std::string input2 = " ";
            std::string input3 = "";

            Assert::IsFalse(isEmpty(input1));
            Assert::IsFalse(isEmpty(input2));
            Assert::IsTrue(isEmpty(input3));
        }
        TEST_METHOD(testEditEventData)
        {
            using namespace utilities;

            std::string title = "Test_752";
            std::string info = "Testing of the functionality.";
            std::string type = "Battle";
            std::string dateStr = "7/4/2025";

            EVENT* head = new EVENT{ title, 0, 0, 0, info, nullptr, type };
            formatDate(dateStr, head);

            head->title = "Test_751";
            head->info = "Editing an event test.";
            head->type = "Treaty";
            formatDate("1/1/2024", head);

            Assert::AreEqual(std::string("Test_751"), head->title);
            Assert::AreEqual(std::string("Editing an event test."), head->info);
            Assert::AreEqual(std::string("Treaty"), head->type);
            Assert::AreEqual(1, head->dateDay);
            Assert::AreEqual(1, head->dateMonth);
            Assert::AreEqual(2024, head->dateYear);

            delete head;
        }
        TEST_METHOD(testDeleteEventFromList)
        {
            using namespace utilities;

            EVENT* head = new EVENT{ "Test_752", 7, 4, 2025, "Deleting event test.", nullptr, "Battle" };
            EVENT* second = new EVENT{ "Test_751", 1, 1, 2020, "Dummy event.", nullptr, "Treaty" };
            head->next = second;

            std::string target = "Test_752";

            if (head != nullptr && head->title == target) {
                EVENT* toDelete = head;
                head = head->next;
                delete toDelete;
            }
            else {
                EVENT* prev = head;
                EVENT* curr = head->next;
                while (curr != nullptr && curr->title != target) {
                    prev = curr;
                    curr = curr->next;
                }
                if (curr != nullptr) {
                    prev->next = curr->next;
                    delete curr;
                }
            }
            EVENT* check = head;
            while (check != nullptr) {
                Assert::AreNotEqual(std::string("Test_752"), check->title);
                check = check->next;
            }
            delete head;
        }

	};
}
