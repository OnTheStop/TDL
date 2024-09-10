#define NOMINMAX

#include <iostream>
#include <Windows.h>
#include <limits>
#include <string>
#include <thread>

#include "SQL_response.h"
#include "ToDoList.h"

int main() {
	HWND hwnd = GetConsoleWindow(); // Получить дескриптор окна консоли
	if (hwnd != NULL) {
		ShowWindow(hwnd, SW_MAXIMIZE); // Максимизировать окно
	}

	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	SQL_response SQL_res("dbname=ToDoList user=postgres password=K75756125f hostaddr=::1 port=5432");
	std::vector<ToDoList> taskLists;
	int choice = 0;
	bool isDataThere = false;

	std::cout << "+------------+\n" <<
		"| СПИСОК ДЕЛ |\n" <<
		"+------------+" << std::endl;

	try {
		isDataThere = SQL_res.isGetTo_doList("SELECT * FROM taskLists");//проверяем, есть ли уже какие нибудь данные в БД
	}
	catch (const std::exception& ex) {

		if (std::string(ex.what()) == "Повторное подключение не выполнено, проверьте данные по которым вы подключаетесь!" || ex.what() == "соединение не выполнено, проверьте данные по которым вы подключаетесь!") {
			std::cerr << ex.what() << std::endl;
			return 1;
		}
		else if (ex.what() == "Повторный запрос не удалось выполнить, проверьте данные запроса!" || ex.what() == "Запрос не выполнен, проверьте данные запроса!") {
			std::cerr << ex.what() << std::endl;
			return 2;
		}
	}

	if (isDataThere == 0) {
		std::cout << "Вы еще не добавили ни одного дела!\nХотите добавить дело?\n1)Да.\n2)Нет." << std::endl;
		std::cout << "Ваш выбор: ";


		ChoiceSelection(choice, 1, 2);

		if (choice == 1) {

			try {
				SQL_res.AddTaskList(taskLists[0]);
				SQL_res.AddTask(taskLists[0]);
				isDataThere = 1;
			}
			catch (const std::exception& ex) {
				std::cerr << ex.what() << std::endl;
				return 3;
			}
		}
		if (choice == 2)
			return 0;
	}

	if (isDataThere == 1) {

		SQL_res.GetTo_doListData(taskLists);

		std::cout << "\n1)Просмотреть дела." << std::endl;
		std::cout << "2)Добавить дело." << std::endl;
		std::cout << "3)Обновить дело." << std::endl;
		std::cout << "4)Удалить дело." << std::endl;
		std::cout << "5)Выйти." << std::endl;
		std::cout << "\nВыберите ваш вариант: ";

		/*	ChoiceSelection(choice, 1, 2);*/

		ChoiceSelection(choice, 1, 5);

		std::cout << std::endl;

		if (choice == 1) {

			for (int i = 0; i < taskLists.size(); i++) {
				std::cout << i + 1 << ")" << taskLists[i].GetListName() << std::endl;
			}

			std::cout << "Выберите дело: ";
			int dynamicNum = taskLists.size();

			ChoiceSelection(choice, 1, dynamicNum);

			int currentList = choice - 1;
			taskLists[currentList].PrintTasks();

			std::cout << "\n1)Добавить задачу." << std::endl;
			std::cout << "2)Обновить задачу." << std::endl;
			std::cout << "3)Удалить задачу." << std::endl;
			std::cout << "4)Просмотреть задачу." << std::endl;
			std::cout << "5)Назад." << std::endl;
			std::cout << "\nВыберите ваш вариант: ";

			ChoiceSelection(choice, 1, 5);

			if (choice == 1)
				SQL_res.AddTask(taskLists[currentList]);
			else if (choice == 2) {
				std::cout << "\nВыберите задачу для обновления." << std::endl;

				taskLists[currentList].TaskUpdate();
			}
		}

	}
	return 0;
}



