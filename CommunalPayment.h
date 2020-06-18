#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ofstream;

struct Address {
    int house;
    int flat;

    friend std::istream & operator>>(std::istream &in, Address &address);
    friend std::ostream & operator<<(std::ostream &out, const Address &address);
    bool operator == (Address t1) const;
    bool operator<(const Address &address) const;
};

struct Date {
    int year, month, day;

    friend std::istream & operator>>(std::istream &in, Date &date);
    friend std::ostream & operator<<(std::ostream &out, const Date &date);
    bool operator == (Date t1) const;
    bool operator<(const Date &date) const;
};

enum class PaymentType {
    WaterSupply,
    WaterDisposal,
    HeatSupply,
    GasSupply,
    PowerSupply
};

struct CommunalPayment {
    Address address;
    string owner_surname;
    PaymentType type;
    Date date;
    int sum;
    int penny;
    int days_past_due;

    CommunalPayment(Address new_address,
                    string new_owner_surname,
                    PaymentType new_type,
                    Date new_Date,
                    int new_sum,
                    int new_penny,
                    int new_days_past_due);

    CommunalPayment();

    friend std::istream &operator>>(std::istream &in, CommunalPayment &payment);

    friend std::ostream &operator<<(std::ostream &out, const CommunalPayment &payment);

    bool operator==(const CommunalPayment &payment) const;

    bool operator<(const CommunalPayment &payment) const;

};