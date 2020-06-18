//
// Created by Виктор Решетов on 18.06.2020.
//

#include <iostream>
#include "string"

using std::string;
using std::cin;
using std::cout;
using std::endl;

void on_error() {
    cout << "Ошибка! Повторите ввод!" << endl;
}

int my_atoi(string str) { // не использовать сишные функции, правильно?
    int idx = 0;
    while (str[idx] == ' ')
        idx++;

    // так как в моих данных нигде отрицательные значения,
    // то в случае непереводимой строки возвращаю -1,
    // чтобы у меня был маркер для требования повторного ввода
    // в принципе по этому не использую strtol
    if (str[idx] != '+' && str[idx] != '-' && (str[idx] < '0' || str[idx] > '9'))
        return -1;

    int sgn = 1;

    if (str[idx] == '-')
        sgn = -1;

    if (str[idx] == '-' || str[idx] == '+')
        idx++;

    str = str.substr(idx);
    while (!str.empty() && (str[str.size() - 1] < '0' || str[str.size() - 1] > '9'))
        str.pop_back();

    int num = 0;
    idx = 0;

    while (idx < str.size() && !(str[idx] < '0' || str[idx] > '9')) {
        num = 10 * num + int(str[idx] - '0');
        if (num > INT_MAX) {
            num = -1;
            break;
        }
        idx++;
    }

    if (num == -1 && sgn == 1)
        return INT_MAX;

    if (num == -1 && sgn == -1)
        return INT_MIN;

    num *= sgn;

    return num < INT_MIN ? INT_MIN : (num > INT_MAX ? INT_MAX : num);
}

int input_pos_int(const string& text, std::istream &in = cin) {
    string input;
    int result = -1;

    while (result < 0) {
        if (!text.empty())
            cout << text << endl;

        in >> input;
        in.clear();
        result = my_atoi(input);

        if (result < 0)
            cout << "Ошибка! Повторите ввод!" << endl;
    }

    return result;
}
