#define NOMINMAX

#include <iostream>
#include <Windows.h>
#include <limits>
#include <string>
#include <thread>

#include "SQL_response.h"
#include "ToDoList.h"

int main() {
	HWND hwnd = GetConsoleWindow(); // �������� ���������� ���� �������
	if (hwnd != NULL) {
		ShowWindow(hwnd, SW_MAXIMIZE); // ��������������� ����
	}

	setlocale(LC_ALL, "rus");
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	SQL_response SQL_res("dbname=ToDoList user=postgres password=K75756125f hostaddr=::1 port=5432");
	std::vector<ToDoList> taskLists;
	int choice = 0;
	bool isDataThere = false;

	std::cout << "+------------+\n" <<
		"| ������ ��� |\n" <<
		"+------------+" << std::endl;

	try {
		isDataThere = SQL_res.isGetTo_doList("SELECT * FROM taskLists");//���������, ���� �� ��� ����� ������ ������ � ��
	}
	catch (const std::exception& ex) {

		if (std::string(ex.what()) == "��������� ����������� �� ���������, ��������� ������ �� ������� �� �������������!" || ex.what() == "���������� �� ���������, ��������� ������ �� ������� �� �������������!") {
			std::cerr << ex.what() << std::endl;
			return 1;
		}
		else if (ex.what() == "��������� ������ �� ������� ���������, ��������� ������ �������!" || ex.what() == "������ �� ��������, ��������� ������ �������!") {
			std::cerr << ex.what() << std::endl;
			return 2;
		}
	}

	if (isDataThere == 0) {
		std::cout << "�� ��� �� �������� �� ������ ����!\n������ �������� ����?\n1)��.\n2)���." << std::endl;
		std::cout << "��� �����: ";


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

		std::cout << "\n1)����������� ����." << std::endl;
		std::cout << "2)�������� ����." << std::endl;
		std::cout << "3)�������� ����." << std::endl;
		std::cout << "4)������� ����." << std::endl;
		std::cout << "5)�����." << std::endl;
		std::cout << "\n�������� ��� �������: ";

		/*	ChoiceSelection(choice, 1, 2);*/

		ChoiceSelection(choice, 1, 5);

		std::cout << std::endl;

		if (choice == 1) {

			for (int i = 0; i < taskLists.size(); i++) {
				std::cout << i + 1 << ")" << taskLists[i].GetListName() << std::endl;
			}

			std::cout << "�������� ����: ";
			int dynamicNum = taskLists.size();

			ChoiceSelection(choice, 1, dynamicNum);

			int currentList = choice - 1;
			taskLists[currentList].PrintTasks();

			std::cout << "\n1)�������� ������." << std::endl;
			std::cout << "2)�������� ������." << std::endl;
			std::cout << "3)������� ������." << std::endl;
			std::cout << "4)����������� ������." << std::endl;
			std::cout << "5)�����." << std::endl;
			std::cout << "\n�������� ��� �������: ";

			ChoiceSelection(choice, 1, 5);

			if (choice == 1)
				SQL_res.AddTask(taskLists[currentList]);
			else if (choice == 2) {
				std::cout << "\n�������� ������ ��� ����������." << std::endl;

				taskLists[currentList].TaskUpdate();
			}
		}

	}
	return 0;
}



