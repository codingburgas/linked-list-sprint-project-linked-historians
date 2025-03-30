#include "event.h"

#include "pch.h"
void addEvent(EVENT** head)
{
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

			if (list == nullptr) {
				std::cout << "No historical events recorded.\n";
				return;
			}

			while (list != nullptr)
			{
				std::cout << "Title: " << list->title << std::endl;
				std::cout << "Date: " << list->date << std::endl;
				std::cout << "Info: " << list->info	 << std::endl;

				list = list->next;
			}
		
	}

	
