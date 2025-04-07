#pragma once

#include "pch.h"
#include "authentication.h"
#include "input.h";


void addEventToFront(EVENT** head, EVENT* newEvent);
void addEvent(EVENT** head, int& userId, sqlite3* db);

void displayEventInfo(EVENT* head, const std::string& typeFilter, const std::string& nameFilter, const std::string& dateFilter);
void displaySortedEvents(EVENT*& head, const std::string& typeFilter, const std::string& nameFilter, const std::string& dateFilter);
void displayEvents(EVENT* head, int& userId, sqlite3* db);

void deleteEvent(EVENT** head, std::string& title, int userId, sqlite3* db);
void editEvent(EVENT* head, int userId, sqlite3* db);
void searchInEvent(EVENT* head, const std::string& searchKeyword);