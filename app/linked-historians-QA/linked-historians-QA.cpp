#include "CppUnitTest.h"
#include "utilities.h"
#include "authentication.cpp"

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

        TEST_METHOD(testSearchEventByKeyword)
        {
            using namespace utilities;

            EVENT* head = new EVENT{ "Test_752", 7, 4, 2025, "Searching test.", nullptr, "Battle" };
            EVENT* second = new EVENT{ "Test_751", 1, 1, 2020, "Dummy event.", nullptr, "Treaty" };
            head->next = second;

            std::string searchTitle = "Test_752";
            std::string keyword = "test";

            EVENT* current = head;
            bool foundTitle = false;
            bool keywordFound = false;

            while (current != nullptr) {
                if (current->title == searchTitle) {
                    foundTitle = true;
                    if (containsString(current->info, keyword)) {
                        keywordFound = true;
                    }
                    break;
                }
                current = current->next;
            }

            Assert::IsTrue(foundTitle);
            Assert::IsTrue(keywordFound);

            delete second;
            delete head;
        }
        TEST_METHOD(testSignUp)
        {
            sqlite3* db;
            int result = sqlite3_open("../data", &db);
            Assert::AreEqual(SQLITE_OK, result);
            Authentication auth(db);

            bool tableCreated = auth.createTable();
            Assert::IsTrue(tableCreated);
            bool cleanup = auth.executeQuery(std::string("DELETE FROM accounts WHERE username = 'Test_752';"));
            Assert::IsTrue(cleanup);

            std::string username = "Test_752";
            std::string password = "test";
            int userId = -1;

            bool success = auth.signUp(username, password, userId);
            Assert::IsTrue(success);
            Assert::IsTrue(userId > 0);

            sqlite3_close(db);
        }
        TEST_METHOD(testLogIn)
        {
            sqlite3* db;
            int result = sqlite3_open("../data", &db);
            Assert::AreEqual(SQLITE_OK, result);
            Authentication auth(db);

            bool tableCreated = auth.createTable();
            Assert::IsTrue(tableCreated);
            bool cleanup = auth.executeQuery(std::string("DELETE FROM accounts WHERE username = 'Test_752';"));
            Assert::IsTrue(cleanup);
            std::string username = "Test_752";
            std::string password = "test";
            int signUpId = -1;

            bool signUpSuccess = auth.signUp(username, password, signUpId);
            Assert::IsTrue(signUpSuccess);
            Assert::IsTrue(signUpId > 0);

            int loginId = -1;
            bool loginSuccess = auth.logIn(username, password, loginId);

            Assert::IsTrue(loginSuccess);
            Assert::AreEqual(signUpId, loginId);
            sqlite3_close(db);
        }

	};
}
