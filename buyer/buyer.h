#ifndef BUYER_H
#define BUYER_H
#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include "../bank_customer/bank_customer.h"
using namespace std;
struct CartItem
{
    int itemId;
    string itemName;
    int sellerId;
    int quantity;
    double price;
    CartItem(int id, const string &name, int seller, int qty, double p)
        : itemId(id), itemName(name), sellerId(seller), quantity(qty), price(p) {}
};
class Buyer
{
private:
    int id;
    string name;
    string address;
    string phone;
    string email;
    BankCustomer &account;
    vector<CartItem> shoppingCart;
    vector<int> orderHistory;

public:
    Buyer(int id, const string &name, const string &address, const string &phone, const string &email, BankCustomer &account0);
    int getId() const { return id; }
    string getName() const { return name; }
    string getAddress() const { return address; }
    string getPhone() const { return phone; }
    string getEmail() const { return email; }
    BankCustomer &getAccount() { return account; }
    const vector<int> &getOrderHistory() const { return orderHistory; }
    void setId(int newId) { id = newId; }
    void setName(const std::string &newName) { name = newName; }
    void setAddress(const std::string &newAddress) { address = newAddress; }
    void setPhone(const std::string &newPhone) { phone = newPhone; }
    void setEmail(const std::string &newEmail) { email = newEmail; }
    void addToCart(int itemId, const string &itemName, int sellerId, int quantity, double price);
    void removeFromCart(int index);
    void viewCart() const;
    void clearCart();
    const vector<CartItem> &getCart() const { return shoppingCart; }
    void addOrderToHistory(int orderIndex);
};
#endif
