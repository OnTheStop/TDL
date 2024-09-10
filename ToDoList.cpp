#include "ToDoList.h"

int ChoiceSelection(int& choice, int firstNum, int secondNum) {

	while (!(std::cin >> choice) || choice < firstNum || choice > secondNum) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (secondNum > 2)
			std::cout << "Некорректный ввод. Пожалуйста, введите целое число в диапазоне от 1 до " << secondNum << ": ";
		else
			std::cout << "Некорректный ввод. Пожалуйста, выберите, что хотите сделать: ";
	}

	return choice;
};

void Tasks::SetTaskName(const std::string taskName) { this->taskName = taskName; }

void Tasks::SetDescription(const std::string description) { this->description = description; }

void Tasks::SetDeadline(const std::string deadline) { this->deadline = deadline; }

void Tasks::SetIsDone(const std::string isDone) { 

	if (isDone.c_str()[0] == 't')
		this->isDone = true;

	if (isDone.c_str()[0] == 'f')
		this->isDone = false;
}

void Tasks::SetTaskID(const int taskID) { this->taskID = taskID; }

std::string Tasks::GetTaskName() { return taskName; }

std::string Tasks::GetDescription() { return description; }

std::string Tasks::GetDeadline() { return deadline; }

bool Tasks::GetDone() { return isDone; }

int Tasks::GetTaskID() { return taskID; }

void ToDoList::SetListName(const std::string listName) { this->listName = listName; }

void ToDoList::SetListID(const int listID) { this->listID = listID; }

void ToDoList::SetTask_data(const Tasks& task) { task_data.push_back(task); }

int ToDoList::GetListID() { return listID; }

std::string ToDoList::GetListName() { return listName; }

std::vector<Tasks> ToDoList::GetTask_data() { return task_data; }

void ToDoList::PrintTasks() {
	int width = 20;
	int maxCharacters = 40;

	std::cout << "\n" << std::setw(20) << listName << std::endl;
	

	for (int i = 0; i < task_data.size(); i++) {

		if (description.length() < maxCharacters)
		{
			if (task_data[i].GetDone())
				std::cout << "\n" << task_data[i].GetTaskName() << " | Статус: " << "выполнено" <<
				"\n" << std::setw(width) << task_data[i].GetDescription() << "\nСрок выполнения: " << task_data[i].GetDeadline();
			else {
				std::cout << "\n" << task_data[i].GetTaskName() << " | Статус: " << "не выполнено" <<
					"\n" << std::setw(width) << task_data[i].GetDescription() << "\nСрок выполнения: " << task_data[i].GetDeadline();
			}

			std::cout << std::endl;
		}
		else {
			std::string truncatedText = description.substr(0, maxCharacters);

			if (isDone)
				std::cout << "\n" << task_data[i].GetTaskName() << " | Статус: " << "выполнено" <<
				"\n" << std::setw(width) << task_data[i].GetDescription() << "\nСрок выполнения: " << task_data[i].GetDeadline();
			else {
				std::cout << "\n" << task_data[i].GetTaskName() << " | Статус: " << "не выполнено" <<
					"\n" << std::setw(width) << truncatedText << "..." << "\nСрок выполнения: " << task_data[i].GetDeadline();
			}

			std::cout << std::endl;
		}
	}
}

void ToDoList::TaskUpdate()
{
	int choice = 0;
	std::string taskName;

	for (int i = 0; i < task_data.size(); i++)
		std::cout << i + 1 << ")" << task_data[i].GetTaskName() << std::endl;

	std::cout << "\nВыберите задачу: ";
	ChoiceSelection(choice, 1, task_data.size());
	int currentTask = choice - 1;

	std::cout << "\n1)Обновить название задачи." << std::endl;
	std::cout << "2)Обновить описание задачи." << std::endl;
	std::cout << "3)Обновить срок выполнения задачи." << std::endl;
	std::cout << "4)Обновить статус задачи." << std::endl;
	std::cout << "5)Назад." << std::endl;
	std::cout << "\nВыберите ваш вариант: ";

	ChoiceSelection(choice, 1, 5);

	if (choice == 1) {
		std::cout << "Если передумаете, введите 'b'" << std::endl;
		std::cout << "Текущее название: " << task_data[currentTask].GetTaskName() << std::endl;
		std::cout << "Введите новое название: ";

		while (taskName.empty())
			std::getline(std::cin >> std::ws, taskName);

		
	}
}

