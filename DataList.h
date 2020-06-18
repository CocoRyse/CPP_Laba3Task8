//
// Created by Виктор Решетов on 17.05.2020.
//

#include "string"
#include "CommunalPayment.h"
#include <deque>
#include <algorithm>
#include <fstream>
#include "Utils.cpp"

using std::deque;
using std::istream_iterator;
using std::ostream_iterator;
using std::istream;
using std::ostream;
using std::ifstream;
using std::ofstream;

template<typename T>
class DataList {
    deque<T> deq;
    deque<T> helper;
    string storage_name = "CommunalPaymentStorage.txt";

    void add(istream_iterator<T> is) {
        deq.push_back(*is);
    }

    void change(typename deque<T>::iterator x) {
        istream_iterator<T> is(cin);
        *x = *is;
    }

    void delete_data(typename deque<T>::iterator x) {
        deq.erase(x);
    }

    void clear() {
        helper.clear();
        deq.clear();
    }

public:
    DataList() : deq(), helper() {
        load();
    }

    bool is_empty() {
        return deq.empty();
    }

// ------------------------ Методы-меню, начало ------------------------
    void add_data_menu() {
        istream_iterator<T> is;
        bool control = true;
        while (control) {
            control = promt_menu("Добавить данные?");
            if (control) {
                is = cin;
                add(is);
                cout << "Данные успешно добавлены." << endl;
            }
        }
    }

    void change_menu() {
        CommunalPayment payment;
        deque<CommunalPayment>::iterator iterator;

        cout << "Введите платёж, который хотите изменить" << endl;
        cin >> payment;

        iterator = find_if([&payment](const CommunalPayment &p) { return payment == p; });

        cout << "Было..." << endl;
        print_data(iterator);

        cout << "Стало..." << endl;
        change(iterator);

        cout << "Данные успешно изменены." << endl;
    }

    void clear_menu() {
        if (!promt_menu("Вы хотите очистить справочник?"))
            return;

        clear();
        cout << "Справочник успешно очищен" << endl;
    }

    void delete_menu() {
        CommunalPayment payment;
        deque<CommunalPayment>::iterator iterator;

        cout << "Введите платёж, который хотите удалить" << endl;
        cin >> payment;

        iterator = find_if([&payment](const CommunalPayment &p) { return payment == p; });
        delete_data(iterator);

        cout << "Данные о платеже успешно удалены" << endl;
    }

    void select_menu_text() {
        cout << "Какую выборку вы хотите составить?: " << endl;
        cout << "1. Выборка с одинаковой фамилией владельца" << endl;
        cout << "2. Выборка с одинаковым номером дома" << endl;
        cout << "3. Выборка с одинаковым номеров квартиры" << endl;
        cout << "4. Выборка с одинаковой датой" << endl;
        cout << "5. Выборка с одинаковым статусом долга" << endl;
        cout << "0. Отмена" << endl;
    }

    void select_menu() {
        char input;
        bool control = true;
        while (control) {
            select_menu_text();
            cin >> input;
            cin.clear();
            switch (input) {
                case '1': {
                    select_by_owner();
                    break;
                }
                case '2': {
                    select_by_house();
                    break;
                }
                case '3': {
                    select_by_flat();
                    break;
                }
                case '4': {
                    select_by_date();
                    break;
                }
                case '5': {
                    select_by_debt_state();
                    break;
                }
                case '0':
                    control = false;
                    break;
                default:
                    on_error();
                    break;
            }
        }
    }

    bool promt_menu(const string &text) {
        char input = '2';

        while (input != '0' && input != '1') {
            cout << text << endl;
            cout << "1 – да" << endl;
            cout << "0 – нет" << endl;

            cin >> input;
            cin.clear();

            if (input != '1' && input != '0')
                on_error();
        }

        return input == '1';
    }

    void select_by_owner() {
        string surname;

        cout << "Фамилия владельца: ";
        cin >> surname;
        cout << endl;
        cin.clear();

        copy_if([&surname](const CommunalPayment &p) { return p.owner_surname == surname; });
        print_some_data();
    }

    void select_by_flat() {
        int flat = input_pos_int("Введите номер квартиры");
        copy_if([&flat](const CommunalPayment &p) { return p.address.flat == flat; });
        print_some_data();
    }

    void select_by_house() {
        int house = input_pos_int("Введите номер дома");
        copy_if([&house](const CommunalPayment &p) { return p.address.house == house; });
        print_some_data();
    }

    void select_by_date() {
        Date date{};

        cout << "Дата платежа: ";
        cin >> date;

        copy_if([&date](const CommunalPayment &p) { return p.date == date; });
        print_some_data();
    }

    void select_by_debt_state() {
        bool is_owe = promt_menu("У платежей, которые вы хотите отобразить, имеются долги?");

        copy_if([&is_owe](const CommunalPayment &p) {
            return (p.penny > 0 || p.days_past_due > 0) == is_owe;
        });

        print_some_data();
    }

    void main_menu_text() {
        cout << "Выберите действие: " << endl;
        cout << "1. Добавить данные" << endl;

        if (is_empty()) {
            cout << "0. Выход" << endl;
            cout << "Внимание! Справочник пуст!" << endl;
            return;
        }

        cout << "2. Изменить данные о платеже" << endl;
        cout << "3. Удалить данные о платеже" << endl;
        cout << "4. Вывести список должников" << endl;
        cout << "5. Составить выборки" << endl;
        cout << "6. Очисить справочник" << endl;
        cout << "7. Вывести данные из справочника на экран" << endl;
        cout << "0. Выход" << endl;
    }

    void main_menu() {
        char input;
        bool control = true;
        while (control) {
            main_menu_text();
            cin >> input;
            cin.clear();
            switch (input) {
                case '1': {
                    add_data_menu();
                    break;
                }
                case '2': {
                    change_menu();
                    break;
                }
                case '3': {
                    delete_menu();
                    break;
                }
                case '4': {
                    print_list_of_debtors();
                    break;
                }
                case '5': {
                    select_menu();
                    break;
                }
                case '6': {
                    clear_menu();
                    break;
                }
                case '7': {
                    print_data();
                    break;
                }
                case '0':
                    control = false;
                    break;
                default:
                    on_error();
                    break;
            }
            save(); // после каждого действия сохраняю состояние
        }
    }
// ------------------------ Методы-меню, конец ------------------------


// ------------------------ Методы для работы с файлами, начало --------------------------------
    void load() {
        ifstream file(storage_name, std::ios_base::in);
        fread_full(file);
//        fread_full(ifstream(storage_name, std::ios_base::in));
    }

    void save() {
        ofstream file(storage_name, std::ios_base::out);
//        fwrite_full(ifstream(storage_name, std::ios_base::out));
        fwrite_full(file);
    }

    void fwrite_full(ofstream &_file) {
        copy(deq.begin(), deq.end(), ostream_iterator<T>(_file, "\n"));
    }

    void fread_full(ifstream &_file) {
        while (_file.peek() != EOF) {
            deq.push_back(*istream_iterator<T>(_file));
            _file.get();
            _file.get();
        }
    }
// ------------------- Методы для работы с файлами, конец ------------------------------------------


// ------------------- Обёртки над стандартными алгоритмами, начало -----------------------------------
    deque<CommunalPayment>::iterator find_if(const std::function<bool(CommunalPayment)> predicate) {
        return std::find_if(deq.begin(), deq.end(), predicate);
    }

    deque<CommunalPayment>::iterator lower_bound(CommunalPayment payment) {
        return std::lower_bound(deq.begin(), deq.end(), payment);
    }

    deque<CommunalPayment>::iterator upper_bound(CommunalPayment payment) {
        return std::upper_bound(deq.begin(), deq.end(), payment);
    }

    void sort() {
        std::sort(deq.begin(), deq.end());
    }

    void copy_if(const std::function<bool(CommunalPayment)> predicate) {
        helper.clear();
        std::copy_if(deq.begin(), deq.end(), back_inserter(helper), predicate);
    }

    deque<CommunalPayment>::iterator end() {
        return deq.end();
    }
// ---------------------- Обёртки над стандартными алгоритмами, конец -------------------------------------


// ---------------------- Методы для вывода данных на экран --------------------------------------------------
    void print_data() {
//        for (auto i = deq.begin(); i < deq.end(); i++)
//            print_data(i);
        copy(deq.begin(), deq.end(), ostream_iterator<T>(cout, "\n"));
    }

    void print_data(typename deque<T>::iterator x) {
        cout << *x << endl;
    }

    void print_some_data() {
        copy(helper.begin(), helper.end(), ostream_iterator<T>(cout, "\n"));
    }

    void print_list_of_debtors() {
        string debtors;

        helper.clear();
        copy_if([](const CommunalPayment &p) {
            return (p.penny > 0 || p.days_past_due > 0);
        });

        for (auto it = helper.begin(); it < helper.end(); it++) {
            CommunalPayment payment = *it;
            debtors += payment.owner_surname + '\n';
        }

        cout << "Должники:" << endl;
        cout << debtors;
    }
};
