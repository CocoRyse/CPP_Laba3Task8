/*Результатом должна быть консольная программа с текстовым меню. Программа
должна содержать шаблонный класс для управления данными согласно заданию.
Для хранения данных необходимо выбрать оптимальный с точки зрения задания
контейнер.*/

/*
1. Предусмотреть операции добавления, изменения и удаления элемента
контейнера.
2. Реализовать ввод и вывод элементов контейнера с использованием потоковых
итераторов на экран и в файл.
3. Реализовать операции поиска по заданным критериям и выборки подмножества
элементов по заданным критериям. Реализовать поиск в двух вариантах: линейный
поиск и двоичный поиск на отсортированном контейнере.
4. Предусмотреть вывод подмножества выборки на экран и в файл.
*/

/*Структура записи о коммунальном платеже содержит поля: адрес квартиры,
фамилия владельца, вид платежа, дата платежа, сумма платежа, процент пени,
количество дней просрочки платежа. Поиск по номеру дома, квартиры, владельцу,
дате, наличии долга. Вывод суммы долга в результатах поиска.*/

/* ДОПОЛНЕНИЕ ДЛЯ ТРЕТЬЕЙ ЗАДАЧИ:
 * Реализовать справочник управляющей компании.
 *
 * Да, это абсолютное великолепное условие для задачи
 * Получается:
 * 1. Укрепляю работоспособность и обработку ошибок ввода (запрещаю падать программочке)
 * 2. Сохраняю состояние в служебном файле
 * 3. Переиспользую больше кода
 *
 * Ну и мне хотелось инкапсулировать максимум логики в самом даталисте,
 * чтобы он был типо как готова компонента (энивей вся логика из него выносится за пару минут,
 * и в main.cpp снова становится миллион функций, а DataList становится стройный хз как лучше
 *
 * Дополнительная логика типо возможности разделения функционала в зависимости от выбранной роли
 * тоже быстро докидывается, если это понадобится
*/



#include <iostream>
#include <set>
#include <vector>
#include "DataList.h"

using std::string;

#define storage_name "CommunalPaymentStorage.txt"

void load(DataList<CommunalPayment> &data_list) {
    ifstream file(storage_name, std::ios_base::in);
    data_list.fread_full(file);
}

void save(DataList<CommunalPayment> &data_list) {
    ofstream file(storage_name, std::ios_base::out);
    data_list.fwrite_full(file);
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

void Search(DataList<CommunalPayment> data_list)
{
    std::function<void(DataList<CommunalPayment>&, deque<CommunalPayment>::iterator)> if_not_found =
            [](DataList<CommunalPayment>& data_list, deque<CommunalPayment>::iterator iterator) {
                    cout << "Данные не найдены. ";
                    if (!promt_menu("Хотите добавить?"))
                        return;

                    data_list.add(*iterator);
                };

    CommunalPayment payment;
    deque<CommunalPayment>::iterator iterator;
    char input;

    cin >> payment;
    cout << "1. Использовать линейный поиск" << endl;
    cout << "2. Использовать бинарный поиск" << endl;
    cin >> input;
    cin.clear();

    if (input == '2') {
        data_list.sort();
        iterator = data_list.lower_bound(payment);
        if (iterator == data_list.end()) {
            if_not_found(data_list, iterator);
            return;
        }

        iterator = !(*iterator == payment) ? iterator - 1 : iterator;
    } else if (input == '1') {
        iterator = data_list.find_if([&payment](const CommunalPayment &p) { return payment == p; });
    } else {
        cout << "Неопознанная команда!" << endl;
    }

    if (iterator == data_list.end() || !(*iterator == payment)) {
        if_not_found(data_list, iterator);
        return;
    }

    data_list.print_data(iterator);
}

void select_by_owner(DataList<CommunalPayment> &data_list, string surname = "", bool full = false) {
    if (surname.empty()) {
        cout << "Фамилия владельца: ";
        cin >> surname;
        cout << endl;
        cin.clear();
    }

    std::function<bool(CommunalPayment)> by_full = [&surname](const CommunalPayment &p) {
        return p.owner_surname == surname;
    };

    std::function<bool(CommunalPayment)> by_part = [&surname](const CommunalPayment &p) {
        return p.owner_surname.find(surname) == 0;
    };

    data_list.copy_if(full ? by_full : by_part);
    data_list.print_some_data();
}

void select_debts_by_owner(DataList<CommunalPayment> &data_list, const string& surname) {
    data_list.copy_if([&surname](const CommunalPayment &p) {
        return p.owner_surname == surname && (p.penny > 0 || p.days_past_due > 0);
    });
    data_list.print_some_data();
}

void select_by_flat(DataList<CommunalPayment> &data_list) {
    int flat = input_pos_int("Введите номер квартиры");
    data_list.copy_if([&flat](const CommunalPayment &p) { return p.address.flat == flat; });
    data_list.print_some_data();
}

void select_by_house(DataList<CommunalPayment> &data_list) {
    int house = input_pos_int("Введите номер дома");
    data_list.copy_if([&house](const CommunalPayment &p) { return p.address.house == house; });
    data_list.print_some_data();
}

void select_by_date(DataList<CommunalPayment> &data_list) {
    Date date{};

    cout << "Дата платежа: ";
    cin >> date;

    data_list.copy_if([&date](const CommunalPayment &p) { return p.date == date; });
    data_list.print_some_data();
}

void select_by_debt_state(DataList<CommunalPayment> &data_list) {
    bool is_owe = promt_menu("У платежей, которые вы хотите отобразить, имеются долги?");

    data_list.copy_if([&is_owe](const CommunalPayment &p) {
        return (p.penny > 0 || p.days_past_due > 0) == is_owe;
    });

    data_list.print_some_data();
}

void print_list_of_debtors(DataList<CommunalPayment> &data_list) {
    auto debts = data_list.find_if(
            [](const CommunalPayment& payment) {
                return  payment.penny > 0 || payment.days_past_due > 0;
            });

    std::set<string> debtors;

    for (auto it = debts; it != data_list.end(); it++) {
        CommunalPayment payment = *it;
        debtors.insert(it->owner_surname);
    }

    for (const auto & debtor : debtors) {
        cout << debtor;
    }
}

void add_data_menu(DataList<CommunalPayment> &data_list) {
    CommunalPayment payment;
    bool control = true;
    while (control) {
        control = promt_menu("Добавить данные?");
        if (control) {
            cin >> payment;
            data_list.add(payment);
            cout << "Данные успешно добавлены." << endl;
        }
    }
}

void change_menu(DataList<CommunalPayment> &data_list) {
    CommunalPayment payment;
    deque<CommunalPayment>::iterator iterator;

    cout << "Введите платёж, который хотите изменить" << endl;
    cin >> payment;

    iterator = data_list.find_if([&payment](const CommunalPayment &p) { return payment == p; });

    cout << "Было..." << endl;
    data_list.print_data(iterator);

    cout << "Стало..." << endl;
    data_list.change(iterator);

    cout << "Данные успешно изменены." << endl;
}

void clear_menu(DataList<CommunalPayment> &data_list) {
    if (!promt_menu("Вы хотите очистить справочник?"))
        return;

    data_list.clear();
    cout << "Справочник успешно очищен" << endl;
}

void delete_menu(DataList<CommunalPayment> &data_list) {
    CommunalPayment payment;
    deque<CommunalPayment>::iterator iterator;

    cout << "Введите платёж, который хотите удалить" << endl;
    cin >> payment;

    iterator = data_list.find_if([&payment](const CommunalPayment &p) { return payment == p; });
    data_list.delete_data(iterator);

    cout << "Данные о платеже успешно удалены" << endl;
}

void select_menu_text() {
    cout << "Какую выборку вы хотите составить?: " << endl;
    cout << "1. Выборка по части фамилии владельца" << endl;
    cout << "2. Выборка с одинаковым номером дома" << endl;
    cout << "3. Выборка с одинаковым номеров квартиры" << endl;
    cout << "4. Выборка с одинаковой датой" << endl;
    cout << "5. Выборка с одинаковым статусом долга" << endl;
    cout << "0. Отмена" << endl;
}

void select_menu(DataList<CommunalPayment> &data_list) {
    char input;
    bool control = true;
    while (control) {
        select_menu_text();
        cin >> input;
        cin.clear();
        switch (input) {
            case '1': {
                select_by_owner(data_list);
                break;
            }
            case '2': {
                select_by_house(data_list);
                break;
            }
            case '3': {
                select_by_flat(data_list);
                break;
            }
            case '4': {
                select_by_date(data_list);
                break;
            }
            case '5': {
                select_by_debt_state(data_list);
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

void main_menu_text(DataList<CommunalPayment> &data_list) {
    cout << "Выберите действие: " << endl;
    cout << "1. Добавить данные" << endl;

    if (data_list.is_empty()) {
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

void main_menu(DataList<CommunalPayment> &data_list) {
    char input;
    bool control = true;
    while (control) {
        main_menu_text(data_list);
        cin >> input;
        cin.clear();
        switch (input) {
            case '1': {
                add_data_menu(data_list);
                break;
            }
            case '2': {
                change_menu(data_list);
                break;
            }
            case '3': {
                delete_menu(data_list);
                break;
            }
            case '4': {
                print_list_of_debtors(data_list);
                break;
            }
            case '5': {
                select_menu(data_list);
                break;
            }
            case '6': {
                clear_menu(data_list);
                break;
            }
            case '7': {
                data_list.print_data();
                break;
            }
            case '0':
                control = false;
                break;
            default:
                on_error();
                break;
        }
        save(data_list); // после каждого действия сохраняю состояние
    }
}

void user_menu_text() {
    cout << "1. Посмотреть список своих платежей" << endl;
    cout << "2. Посмотреть список просроченных платежей" << endl;
    cout << "0. Выйти" << endl;
}

std::vector<string> get_users_set(DataList<CommunalPayment> &data_list, const string& surname) {
    std::vector<string> users;
    for (auto &p : data_list) {
        if (p.owner_surname.find(surname) == 0) {
            users.push_back(p.owner_surname);
        }
    }

    return users;
}

bool select_user(DataList<CommunalPayment> &data_list, string& surname) {
    std::vector<string> users = get_users_set(data_list, surname);
    if (users.empty()) {
        cout << "Людей с такой фамилией нет среди клиентов"<< endl;
    }
    if (users.size() == 1) {
        cout << "Найдена только одна похожая фамилия: " << users[0] << endl;
        if (!promt_menu("Это вы?")) {
            return false;
        }
        surname = users[0];
        return true;
    }

    for (int i = 0; i < users.size(); i++) {
        cout << i + 1 << " " << users[i] << endl;
    }
    int input;
    while (true) {
        input = input_pos_int("Введите цифру, соответсвующую своей фамилии.\n"
                              "Если вашей фамилии нет в списке, введите 0");
        if (input < 0 || input > users.size()) {
            on_error();
            continue;
        }
        if (input == 0) {
            return false;
        }
        surname = users[input];
        return true;
    }
}

void user_menu(DataList<CommunalPayment> &data_list) {
    if (data_list.is_empty()) {
        cout << "К сожалению справочник пуст" << endl;
        return;
    }

    char input;
    bool control = true;

    string user_name;
    cout << "Введите свою фамилию" << endl;
    cin >> user_name;
    if (!select_user(data_list, user_name)){
        cout << "Справочник доступен только клиентам" << endl;
        return;
    }
    while (control) {
        user_menu_text();
        cin >> input;
        switch (input) {
            case '0':
                control = false;
                break;
            case '1':
                select_by_owner(data_list, user_name, true);
                break;
            case '2':
                select_debts_by_owner(data_list, user_name);
                break;
            default:
                on_error();
                break;
        }
    }
}

void start_menu_text() {
    cout << "Выберите режим работы" << endl;
    cout << "1. Сотрудник коммунальной компании" << endl;
    cout << "2. Получатель услуг" << endl;
    cout << "0. Выйти" << endl;
}

void start_menu(DataList<CommunalPayment> &data_list) {
    char input;
    bool control = true;
    while (control) {
        start_menu_text();
        cin >> input;
        cin.clear();
        switch (input) {
            case '0':
                control = false;
                break;
            case '1':
                main_menu(data_list);
                break;
            case '2':
                user_menu(data_list);
                break;
            default:
                on_error();
                break;
        }
    }
}

int main() {
    setlocale(LC_ALL, "Rus");
    DataList<CommunalPayment> data_list;
    load(data_list);
    start_menu(data_list);
    save(data_list);
    return 0;
}