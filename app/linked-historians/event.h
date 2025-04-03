#pragma once

#include "pch.h"
#include "authentication.h"
#include "input.h";

struct  EVENT {
	std::string title;
	int dateDay;
	int dateMonth;
	int dateYear;
	std::string info;
	EVENT* next;
};
void addEventToFront(EVENT** head, EVENT* newEvent);
void addEvent(EVENT** head, int& userId, sqlite3* db);

void displayEventInfo(EVENT* head);
void displaySortedEvents(EVENT* head);
void displayEvents(EVENT* head, int& userId, sqlite3* db);

void deleteEvent(EVENT** head, std::string& title, int userId, sqlite3* db);
void editEvent(EVENT* head, int userId, sqlite3* db);