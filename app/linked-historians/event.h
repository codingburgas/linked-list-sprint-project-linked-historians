#pragma once

#include "pch.h"

struct  EVENT {
	std::string title;
	std::string date;
	std::string info;
	EVENT* next;
};

void addEvent(EVENT** head, int& userId, sqlite3* db);

void displayEventInfo(EVENT* head);
void displaySortedEvents(EVENT* head);
void displayEvents(EVENT* head);

void searchInEvent(EVENT* head, const std::string& searchKeyword);
void fetchEvents(EVENT** head, int userId, sqlite3* db);