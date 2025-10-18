#pragma once
#include "../buyer/buyer.h"
#include "../item/item.h"
#include <string>
#include <vector>
#include <iostream>
class seller : public Buyer
{
private:
    int sellerId;
    std::string storeName;
    std::string storeAddress;
    std::string storePhone;
    std::string storeEmail;
    bool idDisplayed(int itemId) const
    {
        return itemId > 0;
    }
    vector<Item> items;

public:
    seller() = default;
    seller(Buyer buyer, int sellerId, const std::string &storeName, const std::string &storeAddress, const std::string &storePhone, const std::string &storeEmail);
    virtual ~seller() = default;
    bool addNewItem(int newId, const std::string &newName, int newQuantity, double newPrice);
    void updateItem(int itemId, const std::string &newName, int newQuantity, double newPrice);
    void makeItemVisibleToCustomer(int itemId);
    int getSellerId() const { return sellerId; }
    std::string getStoreName() const { return storeName; }
    std::string getStoreAddress() const { return storeAddress; }
    std::string getStorePhone() const { return storePhone; }
    std::string getStoreEmail() const { return storeEmail; }
    void setStoreName(const std::string &newStoreName) { storeName = newStoreName; }
    void setStoreAddress(const std::string &newStoreAddress) { storeAddress = newStoreAddress; }
    void setStorePhone(const std::string &newStorePhone) { storePhone = newStorePhone; }
    void setStoreEmail(const std::string &newStoreEmail) { storeEmail = newStoreEmail; }
    void displayInventory() const;
    int getItemsCount() const { return items.size(); }
    bool itemExists(int itemId) const;
    Item *getItemById(int itemId);
    bool replenishItem(int itemId, int quantity);
    bool discardItem(int itemId, int quantity);
    bool setItemPrice(int itemId, double newPrice);
    bool reduceItemQuantity(int itemId, int quantity);
    const vector<Item> &getItems() const { return items; }
    void displayVisibleItems() const;
};
