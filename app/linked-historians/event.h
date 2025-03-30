#pragma once


#include "pch.h"

struct  EVENT {
	std::string title;
	std::string date;
	std::string info;
	EVENT* next;
};

void addEvent(EVENT** head);
void displayEvents(EVENT* head);
void searchInEvent(EVENT* head, const std::string& searchKeyword);