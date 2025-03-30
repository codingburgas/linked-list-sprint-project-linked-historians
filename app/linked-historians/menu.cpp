#include "menu.h"
#include "event.h"

void displayOptions()
{
	std::cout << "1. Add an event\n";
	std::cout << "2. List all events\n";
	std::cout << "3. Search a text in an event\n";
	std::cout << "9. Exit\n";
}

void displayMenu(int& id, Authentication& auth)
{
	system("cls");
	char choice = ' ';

	EVENT* head = nullptr;
	std::string s;
	fetchEvents(&head, id, auth.getDb());

	while (choice != '9')
	{
		displayOptions();
		std::cout << "Enter your choice: ";
		choice = _getch(); 
		std::cout << choice << "\n"; 

		switch (choice)
		{
		case '1':
			addEvent(&head, id, auth.getDb());
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