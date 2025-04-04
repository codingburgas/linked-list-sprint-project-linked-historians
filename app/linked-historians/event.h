#pragma once

#include "pch.h"
#include "authentication.h"
#include "input.h";


void addEventToFront(EVENT** head, EVENT* newEvent);
void addEvent(EVENT** head, int& userId, sqlite3* db);

void displayEventInfo(EVENT* head);
void displaySortedEvents(EVENT* head);
void displayEvents(EVENT* head, int& userId, sqlite3* db);

void deleteEvent(EVENT** head, std::string& title, int userId, sqlite3* db);
void editEvent(EVENT* head, int userId, sqlite3* db);