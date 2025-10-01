#pragma once
#include "buyer.h"
#include "item.h"
#include <string>
#include <vector>

class seller : public Buyer
{

private:
    // Add seller-specific private members here
    int sellerId;
    std::string storeName;
    std::string storeAddress;
    std::string storePhone;
    std::string storeEmail;
    bool idDisplayed(int itemId) const
    {
        // Example implementation, can be customized
        return itemId > 0; // Assuming valid IDs are positive integers
    }

    vector<Item> items; // Assuming seller has a collection of items

public:
    seller() = default;

    seller(Buyer buyer, int sellerId, const std::string &storeName, const std::string &storeAddress, const std::string &storePhone, const std::string &storeEmail)
        : Buyer(buyer.getId(), buyer.getName(), buyer.getAddress(), buyer.getPhone(), buyer.getEmail(), buyer.getAccount()),
          sellerId(sellerId), storeName(storeName), storeAddress(storeAddress), storePhone(storePhone), storeEmail(storeEmail)
    {
        Buyer::setId(buyer.getId());
    }

    virtual ~seller() = default;

    void addNewItem(int newId, const std::string &newName, int newQuantity, double newPrice)
    {
        Item newItem(newId, newName, newQuantity, newPrice);
        items.push_back(newItem);
    }

    void updateItem(int itemId, const std::string &newName, int newQuantity, double newPrice)
    {
        for (auto &item : items)
        {
            if (item.getId() == itemId)
            {
                item.alterItemById(itemId, newName, newQuantity, newPrice); // Assuming alterItemById is a method
            }
        }
    }

    void makeItemVisibleToCustomer(int itemId)
    {
        for (auto &item : items)
        {
            if (item.getId() == itemId)
            {
                item.setDisplay(true); // Assuming setDisplay is a method in Item class
                break;
            }
        }
    }

    // Getters for seller properties
    int getSellerId() const { return sellerId; }
    std::string getStoreName() const { return storeName; }
    std::string getStoreAddress() const { return storeAddress; }
    std::string getStorePhone() const { return storePhone; }
    std::string getStoreEmail() const { return storeEmail; }

    // Setters for seller properties
    void setStoreName(const std::string &newStoreName) { storeName = newStoreName; }
    void setStoreAddress(const std::string &newStoreAddress) { storeAddress = newStoreAddress; }
    void setStorePhone(const std::string &newStorePhone) { storePhone = newStorePhone; }
    void setStoreEmail(const std::string &newStoreEmail) { storeEmail = newStoreEmail; }

    // Add seller-specific members here
};