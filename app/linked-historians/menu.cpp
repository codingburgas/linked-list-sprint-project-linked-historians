#include "menu.h"
#include "event.h"

EVENT* head = nullptr;
std::string s;
void displayOptions()
{
	std::cout << "1. Add an event\n";
	std::cout << "2. List all events\n";
	std::cout << "3. Search a text in an event\n";
	std::cout << "9. Exit\n";
}

void displayMenu(int& id)
{
	system("cls");
	char choice = ' ';

	while (choice != '9')
	{
		displayOptions();
		std::cout << "Enter your choice: ";
		choice = _getch(); 
		std::cout << choice << "\n"; 

		switch (choice)
		{
		case '1':
			addEvent(&head);
			break;
		case '2':
			displayEvents(head);
			break;
		case '3':
			searchInEvent(head, s);
			break;
		case '9':
			break;
		default:
			break;
		}
	}
}