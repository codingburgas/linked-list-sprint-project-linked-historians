#include "event.h"

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
				std::cout << "- " << list->title << std::endl;
				//std::cout << "Date: " << list->date << std::endl;
				//std::cout << "Info: " << list->info	 << std::endl;

				list = list->next;
				
			}
			std::cout << "\nEnter the title of the event to view details: ";
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

	
