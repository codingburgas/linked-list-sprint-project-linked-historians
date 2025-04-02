#pragma once

#include "pch.h"
#include "authentication.h"

struct  EVENT {
	std::string title;
	std::string date;
	std::string info;
	EVENT* next;
};

void addEvent(EVENT** head, int& userId, sqlite3* db);

void displayEventInfo(EVENT* head);
void displaySortedEvents(EVENT* head);
void displayEvents(EVENT* head, int& userId, sqlite3* db);

void searchInEvent(EVENT* head, const std::string& searchKeyword);
void deleteEvent(EVENT** head, std::string& title, int userId, sqlite3* db);
void editEvent(EVENT* head, int userId, sqlite3* db);