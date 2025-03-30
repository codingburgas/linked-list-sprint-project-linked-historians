#include "menu.h"

void displayOptions()
{
	std::cout << "1. Add an event\n";
	std::cout << "2. List all events\n";
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
			break;
		case '2':
			break;
		case '9':
			break;
		default:
			break;
		}
	}
}