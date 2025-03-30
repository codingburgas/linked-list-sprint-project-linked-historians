#include "event.h"
#include "searchAlgorithms.h"

void addEvent(EVENT** head)
{
	static int count = 0;
	if (count == 0)
	{
		std::cin.ignore();
		count++;
	}
	std::string title, date, info;

	std::cout << "Enter Title: ";
	std::getline(std::cin, title);
	std::cout << "Enter Date: ";
	std::getline(std::cin, date);
	std::cout << "Enter Info: ";
	std::getline(std::cin, info);

	EVENT* newEvent = new EVENT{ title, date, info, nullptr };

	if (*head == nullptr)
	{
		*head = newEvent;
	}
	else
	{
		EVENT* temp = *head;
		while (temp->next != nullptr)
		{
			temp = temp->next;
		}
		temp->next = newEvent;
	}
}
	
	void displayEvents(EVENT* head)
	{
		
			EVENT* list = head;
			std::cout << "Available events: \n";
			if (list == nullptr) {
				std::cout << "No historical events recorded.\n";
				return;
			}

			while (list != nullptr)
			{
				std::cout << "- " << list->title << "\n";
				list = list->next;
			}
			std::cout << "\nEnter the name of the event to view details: ";
			std::string searchTitle;
			std::getline(std::cin, searchTitle);
			list = head;

			while (list != nullptr)
			{
				if (list->title == searchTitle)
				{
					std::cout << "Info: " << list->info << "\n";
				}
				list = list->next;
				std::cout << "\n";
			}
	}

	void searchInEvent(EVENT* head, const std::string& searchKeyword)
	{
		EVENT* list = head;
		bool keywordFound = false;


		std::cout << "Available events: \n";
		if (list == nullptr) {
			std::cout << "No historical events recorded.\n";
			return;
		}

		while (list != nullptr)
		{
			std::cout << "- " << list->title << "\n";
			list = list->next;
		}

		
		list = head;
		std::cout << "\nEnter the name of the event to view details: ";
		std::string eventTitle;
		std::getline(std::cin, eventTitle);

		while (list != nullptr)
		{
			if (list->title == eventTitle)
			{
             	std::cout << "\nEnter a keyword to search in the info: ";
				std::string keyword;
				std::getline(std::cin, keyword);

				
				if (containsSubstring(list->info, keyword)) {
					std::cout << "Keyword found in the event info!\n";
					std::cout << "Info: " << list->info << "\n";
					keywordFound = true;
					std::cout << "\n";
				}

				break;
			}
			list = list->next;

			if (!keywordFound) {
				std::cout << "Keyword not found in the event's info.\n";
			}
		}
	}