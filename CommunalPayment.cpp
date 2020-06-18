//
// Created by Виктор Решетов on 15.05.2020.
//

#include "CommunalPayment.h"
#include <utility>
#include <set>
#include <fstream>
#include "Utils.cpp"

CommunalPayment::CommunalPayment(Address new_address,
                                 string new_owner_surname,
                                 PaymentType new_type,
                                 Date new_Date, int new_sum, int new_penny,
                                 int new_days_past_due) :
        address(new_address),
        owner_surname(std::move(new_owner_surname)),
        type(new_type),
        date(new_Date), sum(new_sum), penny(new_penny),
        days_past_due(new_days_past_due)
{ } // я честно не знаю, как это можно нормально в такой форме форматировать

CommunalPayment::CommunalPayment() : address(),
                                     owner_surname(""),
                                     type(PaymentType::GasSupply),
                                     date({}),
                                     sum(0),
                                     penny(0),
                                     days_past_due(0)
{ }

std::istream& read_payment_from_file(std::istream &in, CommunalPayment &payment) {
    in >> payment.owner_surname;
    in >> payment.address;
    int code;
    in >> code;
    payment.type = PaymentType(code - 1);
    in >> payment.date;
    in >> payment.sum;
    in >> payment.penny;
    in >> payment.days_past_due;

    return in;
}

void on_input_error(bool error) {
    if (error)
        cout << "Ошибка! Повторите ввод!" << endl;
}

void read_payment_type_from_screen(std::istream &in, CommunalPayment &payment) {
    int code;
    bool error = true;

    while (error) {
        code = input_pos_int("1 — Водоснабжение\n"
                             "2 — Водоотведение\n"
                             "3 — Отопление\n"
                             "4 — Газоснабжение\n"
                             "5 — Электроснабжение\n"
                             "Тип платежа: ");

        error = code < 1 || code > 5;
        on_input_error(error);
    }

    payment.type = PaymentType(code - 1);
}

std::istream& read_payment_from_screen(std::istream &in, CommunalPayment &payment) {
    string tmp_input;
    cout << "Фамилия владельца: ";
    in >> payment.owner_surname;
    cout << payment.owner_surname << endl;

    in >> payment.address;

    read_payment_type_from_screen(in, payment);

    cout << "Введите дату платежа" << endl;
    in >> payment.date;

    payment.sum = input_pos_int("Сумма платежа", in);
    payment.penny = input_pos_int("Пенни", in);
    payment.days_past_due = input_pos_int("Количество дней просрочки платежа", in);

    return in;
}

std::istream &operator>>(std::istream &in, CommunalPayment &payment) {
    return (typeid(in) == typeid(std::ifstream) ? read_payment_from_file : read_payment_from_screen)(in, payment);
}

std::ostream& write_payment_to_file(std::ostream &out, const CommunalPayment &payment) {
    out << payment.owner_surname << endl;
    out << payment.address << endl;
    out << (int)payment.type << endl;
    out << payment.date << endl;
    out << payment.sum << endl;
    out << payment.penny << endl;
    out << payment.days_past_due << endl;

    return out;
}

std::ostream& write_payment_to_screen(std::ostream &out, const CommunalPayment &payment) {
    out << "Фамилия владельца: " << payment.owner_surname << endl;
    out << "Адрес: " << endl << payment.address << endl;

    out << "Тип платежа: ";
    switch (payment.type) {
        case PaymentType::WaterSupply:
            out << "Водоснабжение";
            break;
        case PaymentType::WaterDisposal:
            out << "Водоотведение";
            break;
        case PaymentType::HeatSupply:
            out << "Отопление";
            break;
        case PaymentType::GasSupply:
            out << "Газоснабжение";
            break;
        case PaymentType::PowerSupply:
            out << "Электроснабжение";
            break;
    }
    out << endl;

    out << "Дата: " << endl << payment.date << endl;
    out << "Сумма: " << payment.sum << endl;
    out << "Пенни: " << payment.penny << endl;
    out << "Количество дней просрочки платежа: " << payment.days_past_due << endl;

    return out;
}

std::istream& read_date_from_file(std::istream &in, Date &date) {
    in >> date.year;
    in >> date.month;
    in >> date.day;

    return in;
}

std::istream& read_year_from_screen(std::istream &in, Date &date) {
    bool error = true;
    while (error) {
        date.year = input_pos_int("Введите год", in);
        error = date.year < 1970 || date.year > 2020;
        on_input_error(error);
    }

    return in;
}

std::istream& read_month_from_screen(std::istream &in, Date &date) {
    bool error = true;

    while (error) {
        date.month = input_pos_int("Введите номер месяца", in);
        error = date.month < 1 || date.month > 12;
        on_input_error(error);
    }

    return in;
}

std::istream& read_day_from_screen(std::istream &in, Date &date) {
    bool error = true;

    while (error) {
        date.day = input_pos_int("Введите номер дня", in);
        switch (date.month) {
            case 1: case 3: case 5:
            case 7: case 8: case 10:
            case 12:
                error = date.day > 31;
                break;
            case 4: case 6: case 9:
            case 11:
                error = date.day > 30;
                break;
            case 2:
                int february_days_count =
                        date.year % 4 != 0 || (date.year % 100 == 0 && date.year % 400 != 0) ? 28 : 29;

                error = date.day > february_days_count;
                break;
        }
        error = error || date.day < 1;
        on_input_error(error);
    }

    return in;
}

std::istream& read_date_from_screen(std::istream &in, Date &date) {
    read_year_from_screen(in, date);
    read_month_from_screen(in, date);
    read_day_from_screen(in, date);

    return in;
}

std::ostream &operator<<(std::ostream &out, const CommunalPayment &payment) {
    return (typeid(out) == typeid(ofstream) ? write_payment_to_file : write_payment_to_screen)(out, payment);
}

bool CommunalPayment::operator==(const CommunalPayment& payment) const {
    return payment.owner_surname == owner_surname &&
           payment.address == address && payment.date == date &&
           (payment.penny > 0 || payment.days_past_due > 0 == penny > 0 || days_past_due > 0);
}

bool CommunalPayment::operator<(const CommunalPayment &payment) const {
    return owner_surname < payment.owner_surname;/* &&
           address       < payment.address       &&
           date          < payment.date          &&
           penny         < payment.penny         &&
           days_past_due < payment.days_past_due;*/
}

bool Date::operator==(Date another) const {
    return year  == another.year  &&
           month == another.month &&
           day   == another.day;
}

bool Address::operator==(Address t1) const {
    return house == t1.house && flat == t1.flat;
}

std::istream & operator>>(std::istream &in, Date &date) {
    return (typeid(in) == typeid(std::ifstream) ? read_date_from_file : read_date_from_screen)(in, date);
}

// ниже кмк достаточно маленькие перегрузки, чтобы их можно было воспринимать без введения дополнительных функций
std::ostream &operator<<(std::ostream &out, const Date &date) {
    if (typeid(out) == typeid(ofstream)) {
        out << date.year << endl;
        out << date.month << endl;
        out << date.day;
    } else {
        out << "Год: " << date.year << endl;
        out << "Месяц: " << date.month << endl;
        out << "День: " << date.day;
    }
    return out;
}

std::istream &operator>>(std::istream &in, Address &address) {
    if (typeid(in) == typeid(std::ifstream)) {
        in >> address.house;
        in >> address.flat;
    } else {
        address.house = input_pos_int("Номер дома", in);
        address.flat = input_pos_int("Номер квартиры", in);
    }
    return in;
}

std::ostream &operator<<(std::ostream &out, const Address &address) {
    if (typeid(out) == typeid(ofstream)) {
        out << address.house << endl;
        out << address.flat;
    } else {
        out << "Номер дома: " << address.house << endl;
        out << "Номер квартиры: " << address.flat;
    }
    return out;
}

bool Address::operator<(const Address &address) const {
    return house < address.house && flat < address.flat;
}

bool Date::operator<(const Date &date) const {
    return year < date.year && month < date.month && day < date.month;
}
