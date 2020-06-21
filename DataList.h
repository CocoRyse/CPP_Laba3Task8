//
// Created by Виктор Решетов on 17.05.2020.
//

#include "string"
#include "CommunalPayment.h"
#include <deque>
#include <algorithm>
#include <fstream>
#include "Utils.h"

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


public:
    DataList() : deq(), helper() { }

    bool is_empty() {
        return deq.empty();
    }

    void add(CommunalPayment payment) {
        deq.push_back(payment);
    }

    void change(typename deque<T>::iterator x) {
        CommunalPayment payment;
        cin >> payment;
        *x = payment;
    }

    void delete_data(typename deque<T>::iterator x) {
        deq.erase(x);
    }

    void clear() {
        helper.clear();
        deq.clear();
    }

// ------------------------ Методы для работы с файлами, начало --------------------------------

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

    deque<CommunalPayment>::iterator begin() {
        return deq.begin();
    }
// ---------------------- Обёртки над стандартными алгоритмами, конец -------------------------------------


// ---------------------- Методы для вывода данных на экран --------------------------------------------------
    void print_data() {
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
