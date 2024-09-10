#pragma once
#define NOMINMAX

#include <iostream>
#include <Windows.h>
#include <libpq-fe.h>
#include <string>
#include <chrono>

#include "ToDoList.h"

class SQL_response
{
private:
	const char* connInfo = nullptr;
	PGconn* conn = nullptr;
	PGresult* res = nullptr;

public:
	SQL_response() {}
	SQL_response(const char* connInfo) {
		this->connInfo = connInfo;
	}

	void ConnectionSetup();
	bool isGetTo_doList(const std::string sql_data);
	void GetTo_doListData(std::vector<ToDoList>& taskLists);
	void RequestError_Reconnection(ExecStatusType status, std::string sql_data);
	void AddTaskList(ToDoList& list);
	void AddTask(ToDoList &list);
};

