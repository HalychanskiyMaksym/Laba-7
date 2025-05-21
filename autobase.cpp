#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>
using namespace std;

// Функція обрізає пробіли на початку і в кінці рядка
string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == string::npos || end == string::npos) ? "" : s.substr(start, end - start + 1);
}

// Функція записує події (додавання, зміни) у файл логів
void log(const string& msg) {
    ofstream logFile("log.txt", ios::app);
    time_t now = time(0);
    string timeStr = ctime(&now);
    timeStr.pop_back(); // Прибираємо зайвий символ переходу на новий рядок
    logFile << "[" << timeStr << "] " << msg << endl;
}

// Структура "Водій"
struct Driver {
    int id;
    string name;
    string status;

    // Зчитує водія з рядка з файлу
    static Driver fromString(const string& line) {
        Driver d;
        stringstream ss(line);
        string part;
        getline(ss, part, ';'); d.id = stoi(trim(part));
        getline(ss, part, ';'); d.name = trim(part);
        getline(ss, part);     d.status = trim(part);
        return d;
    }

    // Перетворює водія в рядок для запису у файл
    string toString() const {
        return to_string(id) + ";" + name + ";" + status;
    }

    // Виводить інформацію про водія в консоль
    void print() const {
        cout << "ID: " << id << ", Ім'я: " << name << ", Статус: " << status << endl;
    }
};

// Структура "Авто"
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

// Структура "Рейс"
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

// Структура "Заявка на ремонт"
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

// Завантаження об'єктів з текстового файлу
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

// Збереження об'єктів у файл
template<typename T>
void saveToFile(const string& filename, const vector<T>& items) {
    ofstream file(filename, ios::out | ios::trunc);
    for (const auto& item : items) {
        file << item.toString() << endl;
    }
}

// Основна функція
int main() {
    // Завантаження даних з файлів у вектори
    vector<Driver> drivers = loadFromFile("drivers.txt", Driver::fromString);
    vector<Vehicle> vehicles = loadFromFile("vehicles.txt", Vehicle::fromString);
    vector<Route> routes = loadFromFile("routes.txt", Route::fromString);
    vector<RepairRequest> repairs = loadFromFile("repairs.txt", RepairRequest::fromString);

    int choice;
    do {
        // Виводимо меню
        cout << "\n--- МЕНЮ \"АВТОБАЗА\" ---\n";
        cout << "1. Переглянути водіїв\n2. Переглянути авто\n3. Переглянути рейси\n";
        cout << "4. Переглянути заявки на ремонт\n5. Додати рейс\n6. Додати заявку на ремонт\n";
        cout << "7. Переглянути лог\n0. Вихід\nВаш вибір: ";
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
                cout << " Ви не ввели маршрут. Спробуйте ще раз.\n";
                continue;
            }

            cout << "ID водія: "; cin >> r.driverId;
            cout << "ID авто: "; cin >> r.vehicleId;
            r.status = "Призначено";
            routes.push_back(r);
            saveToFile("routes.txt", routes);
            log(" Додано новий рейс: " + r.description);
            cout << " Рейс додано та збережено.\n";
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
            cout << " Заявку додано та збережено.\n";
        }

        // Перегляд логів
        else if (choice == 7) {
            cout << "\n--- LOG ---\n";
            ifstream logf("log.txt");
            string line;
            while (getline(logf, line)) cout << line << endl;
        }

    } while (choice != 0); // Повторювати, поки не вибрано вихід

    return 0;
}
