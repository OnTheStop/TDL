#include "SQL_response.h"

void SQL_response::ConnectionSetup()
{
	conn = PQconnectdb(connInfo);
	int choice = 0;
	int errorCounter = 0;

	while (PQstatus(conn) != CONNECTION_OK){
		std::cerr << "Ошибка подключения к базе данных: " << PQerrorMessage(conn) << std::endl;

		if (errorCounter == 2) {
			PQfinish(conn);
			throw std::runtime_error("Повторное подключение не выполнено, проверьте данные по которым вы подключаетесь!");
		}

		std::cout << "Хотите ли вы повторить попытку подключения?  1)Да. 2.Нет.\nВыбор: ";

		while (!(std::cin >> choice) || choice < 1 || choice > 2) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Некорректный ввод. Пожалуйста, выберите, что хотите сделать: ";
		}
		conn = PQconnectdb(connInfo);

		if (PQstatus(conn) == CONNECTION_OK) {
			std::cout << "Соединение успешно установлено!" << std::endl;
			break;
		}
	
		if (choice == 2) {
			PQfinish(conn);
			throw std::runtime_error("Соединение не выполнено, проверьте данные по которым вы подключаетесь!");
		}


		errorCounter++;
	}
}

bool SQL_response::isGetTo_doList(const std::string sql_data)
{
	ConnectionSetup();

	res = PQexec(conn, sql_data.c_str());
	
	if (PQresultStatus(res) != PGRES_TUPLES_OK)
			RequestError_Reconnection(PGRES_TUPLES_OK, sql_data);

	if (PQntuples(res) == 0) {
		PQfinish(conn);
		return 0;
	}
	else {
		PQfinish(conn);
		return 1;
	}
}

void SQL_response::GetTo_doListData(std::vector<ToDoList>& taskLists)
{
	ConnectionSetup();

	bool isSameID = false;
	bool test = false;

	std::string sql_data = "SELECT tasklists.id, tasklists.listName, tasks.taskName, tasks.description, tasks.deadline, tasks.isdone, tasks.id FROM tasks INNER JOIN tasklists ON tasks.tasklist_id = tasklists.ID;";

	res = PQexec(conn, sql_data.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK)
		RequestError_Reconnection(PGRES_TUPLES_OK, sql_data);

	int numRows = PQntuples(res);
	int numColumns = PQnfields(res);

	//std::lock_guard<std::mutex> lock(mtx);

	for (int i = 0; i < numRows; i++)
	{
		isSameID = false;

		//if (!dataProcessed) {
		if (i == 0) {

			ToDoList listTemp;
			listTemp.SetListID(std::stoi(PQgetvalue(res, i, 0)));
			listTemp.SetListName(PQgetvalue(res, i, 1));

			Tasks taskTemp;
			taskTemp.SetTaskName(PQgetvalue(res, i, 2));
			taskTemp.SetDescription(PQgetvalue(res, i, 3));
			taskTemp.SetDeadline(PQgetvalue(res, i, 4));
			taskTemp.SetIsDone(PQgetvalue(res, i, 5));
			taskTemp.SetTaskID(std::stoi(PQgetvalue(res, i, 6)));
			listTemp.SetTask_data(taskTemp);

			taskLists.push_back(listTemp);
		}
		if (i != 0) {
			for (auto& taskList : taskLists) {
				if (taskList.GetListID() == std::stoi(PQgetvalue(res, i, 0))) {
					//если такой объект есть, то заполняется вектор тасков
					Tasks temp;
					temp.SetTaskName(PQgetvalue(res, i, 2));
					temp.SetDescription(PQgetvalue(res, i, 3));
					temp.SetDeadline(PQgetvalue(res, i, 4));
					temp.SetIsDone(PQgetvalue(res, i, 5));
					temp.SetTaskID(std::stoi(PQgetvalue(res, i, 6)));
					taskList.SetTask_data(temp);

					isSameID = true;
				}
			}
			if (!isSameID) {
				//если такой объект отсутствует, то создается объект и после заполняются таски
				ToDoList listTemp;
				listTemp.SetListID(std::stoi(PQgetvalue(res, i, 0)));
				listTemp.SetListName(PQgetvalue(res, i, 1));

				Tasks taskTemp;
				taskTemp.SetTaskName(PQgetvalue(res, i, 2));
				taskTemp.SetDescription(PQgetvalue(res, i, 3));
				taskTemp.SetDeadline(PQgetvalue(res, i, 4));
				taskTemp.SetIsDone(PQgetvalue(res, i, 5));
				taskTemp.SetTaskID(std::stoi(PQgetvalue(res, i, 6)));
				listTemp.SetTask_data(taskTemp);

				taskLists.push_back(listTemp);
			}

		}
		/*threadCounter.fetch_add(1, std::memory_order_relaxed);*/

	}
	/*}
		dataProcessed = true;
		cv.notify_all();
	}
	else {
		cv.wait(lock, [this] { return dataProcessed == false; });
	}*/
	//????????????????????
}

void SQL_response::RequestError_Reconnection(ExecStatusType status, std::string sql_data)
{
	int choice = 0;
	int errorCounter = 0;

	while (PQresultStatus(res) != status) {
		std::cerr << "Ошибка выполнения запроса: " << PQresultErrorMessage(res);

		if (errorCounter == 2) {
			PQclear(res);
			PQfinish(conn);
			throw std::runtime_error("Повторный запрос не удалось выполнить, проверьте данные запроса!");
		}

		std::cout << "Хотите ли вы повторить попытку запроса?  1)Да. 2.Нет.\nВыбор: ";

		while (!(std::cin >> choice) || choice < 1 || choice > 2) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Некорректный ввод. Пожалуйста, выберите, что хотите сделать: ";
		}
		res = PQexec(conn, sql_data.c_str());

		if (PQresultStatus(res) != status)
			break;

		if (choice == 2) {
			PQclear(res);
			PQfinish(conn);
			throw std::runtime_error("Запрос не выполнен, проверьте данные запроса!");
		}


		errorCounter++;

	}
	std::cout << "Запрос успешно выполнен!" << std::endl;
}

void SQL_response::AddTaskList(ToDoList& list)
{
	ConnectionSetup();

	int choice = 0;
	int errorCounter = 0;
	std::string listName;

	std::cout << "Добавить дело: ";
	while (listName.empty())
		std::getline(std::cin >> std::ws, listName);

	std::string sql_data = "INSERT INTO tasklists (listname) VALUES ('" + listName + "') RETURNING id";
	res = PQexec(conn, sql_data.c_str());

	if (PQresultStatus(res) != PGRES_TUPLES_OK)
			RequestError_Reconnection(PGRES_TUPLES_OK, sql_data);

	list.SetListName(listName);
	list.SetListID(std::stoi(PQgetvalue(res, 0, 0)));

	PQclear(res);
	PQfinish(conn);
}

void SQL_response::AddTask(ToDoList& list)
{
	Tasks temp;

	int choice = 0;
	std::string taskName;
	std::string description;
	std::string deadline;
	std::string input;
	int year = 0, month = 0, day = 0;
	
	ConnectionSetup();

	while (true) {
		taskName.clear();
		description.clear();
		deadline.clear();
		input.clear();

		std::cout << "Введите название задачи: ";

		while (taskName.empty())
			std::getline(std::cin >> std::ws, taskName);

		std::cout << "Добавьте описание для вашей задачи: ";

		while (description.empty())
			std::getline(std::cin >> std::ws, description);


		std::cout << "Введите дату через пробел! В формате (год месяц день): ";

		while (true) {
			std::getline(std::cin, input);
			std::istringstream ss(input);

			if (ss >> year >> month >> day && month >= 1 && month <= 12 && day >= 1 && day <= 31) {
				std::chrono::year y(year);
				std::chrono::month m(month);
				std::chrono::day d(day);

				std::chrono::year_month_day ymd = y / m / d;
				deadline = std::to_string(year) + "-" + std::to_string(month) + "-" + std::to_string(day);
				break;
			}
			if (month == 0 || month < 1 || month > 12) {
				std::cerr << "Некоректный ввод даты! Месяц может быть в диапазоне от 1 до 12\nВвод: ";
			}
			if (day == 0 || day < 1 || day > 31) {
				std::cerr << "Некоректный ввод даты! День может быть в диапазоне от 1 до 31\nВвод: ";
			}
			else {
				std::cerr << "Некоректный ввод даты! Дата должна быть указана в формате: yyyy mm dd\nВвод: ";
			}
		}

		std::string sql_data = "INSERT INTO tasks (taskName, description, deadline, isdone, tasklist_id) VALUES ('" + taskName + "', '" + description + "', '" + deadline + "', 'false', " + std::to_string(list.GetListID()) + ") RETURNING id;";

		res = PQexec(conn, sql_data.c_str());

		if (PQresultStatus(res) != PGRES_TUPLES_OK)
				RequestError_Reconnection(PGRES_TUPLES_OK, sql_data);

		std::cout << "Хотите добавить еще одну задачу?  1)Да. 2)Нет.\nВыбор: ";

		while (!(std::cin >> choice) || choice < 1 || choice > 2) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Некорректный ввод. Пожалуйста, выберите, что хотите сделать: ";
		}

		if (choice == 2) {
			PQclear(res);
			PQfinish(conn);
			break;
		}
		else {
			temp.SetTaskID(std::stoi(PQgetvalue(res, 0, 0)));
			temp.SetTaskName(taskName);
			temp.SetDescription(description);
			temp.SetDeadline(deadline);
			temp.SetIsDone("false");
			list.SetTask_data(temp);
			PQclear(res);
		}
	}
}