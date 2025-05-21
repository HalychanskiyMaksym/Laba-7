#include <iostream>         // Для вводу/виводу
#include <fstream>          // Для роботи з файлами
#include <vector>           // Для використання векторів
#include <string>           // Для роботи з рядками
#include <sstream>          // Для розбиття рядків
#include <ctime>            // Для роботи з часом
#include <algorithm>        // Для алгоритмів STL, таких як find_if
using namespace std;

// Функція обрізає пробіли з початку і кінця рядка
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos || end == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Функція для запису повідомлення у файл логів з міткою часу
void log(const string& msg) {
    ofstream logFile("log.txt", ios::app);
    time_t now = time(0);
    string timeStr = ctime(&now);
    timeStr.pop_back(); // Прибираємо символ переходу на новий рядок
    logFile << "[" << timeStr << "] " << msg << endl;
}

// Структура для представлення водія
struct Driver {
    int id;
    string name;
    string status;

    // Метод для створення об'єкта Driver з рядка файлу
    static Driver fromString(const string& line) {
        Driver d;
        stringstream ss(line);
        string part;
        getline(ss, part, ';'); d.id = stoi(trim(part));
        getline(ss, part, ';'); d.name = trim(part);
        getline(ss, part);     d.status = trim(part);
        return d;
    }

    // Перетворення об'єкта Driver на рядок для запису у файл
    string toString() const {
        return to_string(id) + ";" + name + ";" + status;
    }

    // Вивід інформації про водія у консоль
    void print() const {
        cout << "ID: " << id << ", Ім'я: " << name << ", Статус: " << status << endl;
    }
};

// Структура для представлення автомобіля
struct Vehicle {
    int id;
    string model;
    string status;

    static Vehicle fromString(const string& line) {
        Vehicle v;
        stringstream ss(line);
        string part;
        getline(ss, part, ';'); v.id = stoi(trim(part));
        getline(ss, v.model, ';');
        getline(ss, v.status);
        return v;
    }

    string toString() const {
        return to_string(id) + ";" + model + ";" + status;
    }

    void print() const {
        cout << "ID: " << id << ", Модель: " << model << ", Статус: " << status << endl;
    }
};

// Структура для представлення рейсу
struct Route {
    int id;
    string description;
    int driverId;
    int vehicleId;
    string status;

    static Route fromString(const string& line) {
        Route r;
        stringstream ss(line);
        string part;
        getline(ss, part, ';'); r.id = stoi(trim(part));
        getline(ss, r.description, ';');
        getline(ss, part, ';'); r.driverId = stoi(trim(part));
        getline(ss, part, ';'); r.vehicleId = stoi(trim(part));
        getline(ss, r.status);
        return r;
    }

    string toString() const {
        return to_string(id) + ";" + description + ";" + to_string(driverId) + ";" + to_string(vehicleId) + ";" + status;
    }

    void print() const {
        cout << "ID: " << id << ", Маршрут: " << description
             << ", Водій ID: " << driverId << ", Авто ID: " << vehicleId
             << ", Статус: " << status << endl;
    }
};

// Структура для представлення заявки на ремонт
struct RepairRequest {
    int id;
    int driverId;
    string description;
    string status;

    static RepairRequest fromString(const string& line) {
        RepairRequest r;
        stringstream ss(line);
        string part;
        getline(ss, part, ';'); r.id = stoi(trim(part));
        getline(ss, part, ';'); r.driverId = stoi(trim(part));
        getline(ss, r.description, ';');
        getline(ss, r.status);
        return r;
    }

    string toString() const {
        return to_string(id) + ";" + to_string(driverId) + ";" + description + ";" + status;
    }

    void print() const {
        cout << "ID: " << id << ", Водій ID: " << driverId << ", Проблема: " << description
             << ", Статус: " << status << endl;
    }
};

// Шаблонна функція для зчитування об'єктів з файлу
template<typename T>
vector<T> loadFromFile(const string& filename, T(*fromStr)(const string&)) {
    vector<T> items;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        if (!line.empty()) items.push_back(fromStr(line));
    }
    return items;
}

// Шаблонна функція для збереження об'єктів у файл
template<typename T>
void saveToFile(const string& filename, const vector<T>& items) {
    ofstream file(filename, ios::out | ios::trunc);
    for (const auto& item : items) {
        file << item.toString() << endl;
    }
}

// Основна функція програми
int main() {
    // Завантаження всіх даних з файлів
    vector<Driver> drivers = loadFromFile("drivers.txt", Driver::fromString);
    vector<Vehicle> vehicles = loadFromFile("vehicles.txt", Vehicle::fromString);
    vector<Route> routes = loadFromFile("routes.txt", Route::fromString);
    vector<RepairRequest> repairs = loadFromFile("repairs.txt", RepairRequest::fromString);

    int choice;
    do {
        // Меню програми
        cout << "\n--- МЕНЮ \"АВТОБАЗА\" ---\n";
        cout << "1. Переглянути водіїв\n2. Переглянути авто\n3. Переглянути рейси\n";
        cout << "4. Переглянути заявки на ремонт\n5. Додати рейс\n6. Додати заявку на ремонт\n";
        cout << "7. Переглянути лог\n8. Завершити рейс та оновити стан авто\n9. Змінити статус водія\n0. Вихід\nВаш вибір: ";
        cin >> choice;

        // Обробка вибору користувача
        if (choice == 1) for (auto& d : drivers) d.print();
        else if (choice == 2) for (auto& v : vehicles) v.print();
        else if (choice == 3) for (auto& r : routes) r.print();
        else if (choice == 4) for (auto& rq : repairs) rq.print();

        // Додавання нового рейсу
        else if (choice == 5) {
            Route r;
            r.id = routes.empty() ? 1 : routes.back().id + 1;
            cin.ignore();
            cout << "Опис маршруту: ";
            getline(cin, r.description);
            if (r.description.empty()) {
                cout << " Ви не ввели маршрут. \n";
                continue;
            }
            cout << "ID водія: "; cin >> r.driverId;
            cout << "ID авто: "; cin >> r.vehicleId;
            r.status = "Призначено";
            routes.push_back(r);
            saveToFile("routes.txt", routes);
            log(" Додано новий рейс: " + r.description);
            cout << " Рейс додано \n";
        }

        // Додавання нової заявки на ремонт
        else if (choice == 6) {
            RepairRequest rq;
            rq.id = repairs.empty() ? 1 : repairs.back().id + 1;
            cout << "ID водія: "; cin >> rq.driverId;
            cin.ignore();
            cout << "Опис проблеми: ";
            getline(cin, rq.description);
            rq.status = "Новий";
            repairs.push_back(rq);
            saveToFile("repairs.txt", repairs);
            log(" Заявка на ремонт від водія #" + to_string(rq.driverId));
            cout << " Заявку додано \n";
        }

        // Перегляд логів
        else if (choice == 7) {
            cout << "\n--- LOG ---\n";
            ifstream logf("log.txt");
            string line;
            while (getline(logf, line)) cout << line << endl;
        }

        // Завершення рейсу: оновлення статусу рейсу, авто та водія
        else if (choice == 8) {
            int routeId;
            cout << "Введіть ID рейсу, що завершено: ";
            cin >> routeId;

            auto routeIt = find_if(routes.begin(), routes.end(), [routeId](const Route& r) {
                return r.id == routeId;
            });

            if (routeIt == routes.end()) {
                cout << "Рейс не знайдено.\n";
                continue;
            }

            if (routeIt->status == "Виконано") {
                cout << "Рейс уже завершено.\n";
                continue;
            }

            routeIt->status = "Виконано";

            auto vehicleIt = find_if(vehicles.begin(), vehicles.end(), [routeIt](const Vehicle& v) {
                return v.id == routeIt->vehicleId;
            });

            if (vehicleIt == vehicles.end()) {
                cout << "Авто не знайдено.\n";
                continue;
            }

            cin.ignore();
            cout << "Введіть стан авто: ";
            string newStatus;
            getline(cin, newStatus);
            vehicleIt->status = newStatus;

            auto driverIt = find_if(drivers.begin(), drivers.end(), [routeIt](const Driver& d) {
                return d.id == routeIt->driverId;
            });
            if (driverIt != drivers.end()) {
                driverIt->status = "Вільний";
            }

            saveToFile("routes.txt", routes);
            saveToFile("vehicles.txt", vehicles);
            saveToFile("drivers.txt", drivers);

            log("Рейс #" + to_string(routeIt->id) + " завершено. Стан авто: " + newStatus);
            cout << "Рейс та стан авто оновлено. Статус водія встановлено як 'Вільний'.\n";
        }

        // Зміна статусу водія вручну
        else if (choice == 9) {
            int driverId;
            cout << "Введіть ID водія: ";
            cin >> driverId;

            auto it = find_if(drivers.begin(), drivers.end(), [driverId](const Driver& d) {
                return d.id == driverId;
            });

            if (it == drivers.end()) {
                cout << "Водія з таким ID не знайдено.\n";
                continue;
            }

            cin.ignore();
            cout << "Поточний статус: " << it->status << "\nНовий статус: ";
            string newStatus;
            getline(cin, newStatus);

            it->status = newStatus;
            saveToFile("drivers.txt", drivers);
            log("Оновлено статус водія #" + to_string(driverId) + " на: " + newStatus);
            cout << "Статус водія оновлено.\n";
        }

    } while (choice != 0);

    return 0;
}
