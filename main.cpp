#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <ctime>
#include <limits>
#include <fstream>
#include <sstream>
#include "bank_customer/bank_customer.h"
#include "buyer/buyer.h"
#include "seller/seller.h"
#include "bank/bank.h"
#include "order/order.h"
#include "store/store.h"
enum PrimaryPrompt
{
    LOGIN,
    REGISTER,
    STORE_ANALYTICS,
    EXIT
};
enum BuyerMenu
{
    CHECK_ACCOUNT,
    UPGRADE_SELLER,
    BANK_FUNCTIONS,
    BROWSE_SHOP,
    VIEW_CART,
    CHECKOUT,
    MANAGE_ORDERS,
    VIEW_SPENDING,
    SELLER_MODE,
    LOGOUT_BUYER
};
enum BankMenu
{
    CHECK_BALANCE,
    BANK_DEPOSIT,
    BANK_WITHDRAW,
    VIEW_TRANSACTIONS,
    BACK_TO_BUYER
};
enum SellerInventoryMenu
{
    VIEW_INVENTORY,
    ADD_ITEM,
    UPDATE_ITEM,
    SET_PRICE,
    REPLENISH_STOCK,
    DISCARD_STOCK,
    TOGGLE_ITEM_VISIBILITY,
    BACK_TO_MAIN_MENU
};
using namespace std;
vector<BankCustomer> bankCustomers;
vector<Buyer> buyers;
vector<seller> sellers;
Bank mainBank("Main Bank");
Store mainStore("Online Marketplace");
int currentBuyerId = -1;
bool currentUserIsSeller = false;
void handleLogin();
void handleRegister();
void showBuyerMenu();
void handleSellerUpgrade();
void handleBankFunctions();
void handleSellerInventory();
void handleSellerMenu();
void handleBrowseShop();
void handleViewCart();
void handleCheckout();
void handleManageOrders();
void handleViewSpending();
void handleStoreAnalytics();
void handleSellerAnalytics();
bool findBuyerByName(const string &name, int &buyerIndex);
int findSellerIndexByBuyerId(int buyerId);
void clearInputBuffer();

// Serialization functions
void saveData();
void loadData();
void saveBankCustomers();
void loadBankCustomers();
void saveBuyers();
void loadBuyers();
void saveSellers();
void loadSellers();
int main()
{
    // Load data at program start
    loadData();

    PrimaryPrompt prompt = LOGIN;
    while (prompt != EXIT)
    {
        cout << "\n========================================" << endl;
        cout << "   ONLINE MARKETPLACE SYSTEM" << endl;
        cout << "========================================" << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Store Analytics (Admin)" << endl;
        cout << "4. Exit" << endl;
        cout << "Select an option: ";
        int choice;
        cin >> choice;
        prompt = static_cast<PrimaryPrompt>(choice - 1);
        switch (prompt)
        {
        case LOGIN:
            /* if Login is selected, based on authority then provide options:
            assume user is logged in as Buyer for now
            1. Check Account Status (will display if user is Buyer or Seller or both and linked banking account status)
            Will display Buyer, Seller and Banking Account details
            2. Upgrade Account to Seller
            Will prompt user to enter Seller details and create a Seller account linked to Buyer account
            Will reject if a user dont have a banking account linked
            3. Create Banking Account (if not already linked), will be replaced with banking functions
            Must provides: initial deposit amount, Address, Phone number, Email
            Banking functions will provides: Balance checking, Transaction History, Deposit, Withdraw
            4. Browse Store Functionality
            Will display all stores initially
            Need to select a store to browse each store inventory
            Will display all items in the store inventory
            After selecting an item, will display item details and option to add to cart
            After adding to cart, will notify user item is added to cart
            5. Order Functionality
            Will display all items in cart
            Will provide option to remove item from cart
            Will provide option to checkout
            After checkout invoice will be generated (will go to payment functionality)
            6. Payment Functionality
            Will display all listed invoices
            Pick an invoice to pay
            Will display invoice details and total amount
            Will provide option to pay invoice
            Payment is done through confirmation dialogue
            In confirmation dialogue, will display account balance as precursor
            User will need to manually enter invoice id to pay
            After paying balance will be redacted from buyer and added to the responding seller account
            After payment, order status will be changed to paid
            7. Logout (return to main menu)
            Display confirmation dialogue
            If confirmed, return to main menu
            If not, return to Buyer menu
            8. Delete Account (remove both Buyer and Seller account and relevant banking account)
            Display confirmation dialogue
            If confirmed, delete account and return to main menu
            If not, return to Buyer menu
            assume user is logged in as Seller for now
            9. Check Inventory
            10. Add Item to Inventory
            11. Remove Item from Inventory
            12. View Orders (will display all orders placed to this seller
            Only orders with paid status will be listed
            Order details will listing items, quantity, total amount, buyer details, order status (paid, cancelled, completed)
            extra functions
            9. Exit to main Menu
            10. Exit Program
            **/
            handleLogin();
            break;
        case REGISTER:
            /* if register is selected then went through registration process:
            1. Create a new Buyer Account
            Must provides: Name, Home Address, Phone number, Email
            2. Option to create a Seller Account (will be linked to Buyer account)
            Must provides: Store Name, Store Address, Store Phone number, Store Email
            After finished immediately logged in as Buyer/Seller
            */
            handleRegister();
            break;
        case STORE_ANALYTICS:
            handleStoreAnalytics();
            break;
        case EXIT:
            cout << "Thank you for using our system. Goodbye!" << endl;
            // Save data before exiting
            saveData();
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
    return 0;
}
void handleLogin()
{
    cout << "=== LOGIN ===" << endl;
    string name;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    int buyerIndex;
    if (findBuyerByName(name, buyerIndex))
    {
        currentBuyerId = buyerIndex;
        cout << "Login successful! Welcome back, " << name << endl;
        for (size_t i = 0; i < sellers.size(); i++)
        {
            if (sellers[i].getName() == name)
            {
                currentUserIsSeller = true;
                break;
            }
        }
        showBuyerMenu();
    }
    else
    {
        cout << "User not found. Please register first." << endl;
    }
}
void handleRegister()
{
    cout << "=== REGISTRATION ===" << endl;
    string name, address, phone, email;
    cout << "Enter your name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter your home address: ";
    getline(cin, address);
    cout << "Enter your phone number: ";
    getline(cin, phone);
    cout << "Enter your email: ";
    getline(cin, email);
    double initialDeposit;
    cout << "Enter initial deposit amount: $";
    cin >> initialDeposit;
    int newId = bankCustomers.size() + 1;
    BankCustomer newBankCustomer(newId, name, initialDeposit);
    bankCustomers.push_back(newBankCustomer);
    mainBank.addAccount(&bankCustomers.back());
    mainBank.recordTransaction(newId, DEPOSIT, initialDeposit, "Initial deposit");
    Buyer newBuyer(newId, name, address, phone, email, bankCustomers.back());
    buyers.push_back(newBuyer);
    cout << "\nRegistration successful!" << endl;
    cout << "Bank account created with balance: $" << initialDeposit << endl;
    char createSeller;
    cout << "Do you want to create a seller account? (y/n): ";
    cin >> createSeller;
    if (createSeller == 'y' || createSeller == 'Y')
    {
        cout << "Enter store name: ";
        cin.ignore();
        string storeName;
        getline(cin, storeName);
        cout << "Enter store address: ";
        string storeAddress;
        getline(cin, storeAddress);
        cout << "Enter store phone: ";
        string storePhone;
        getline(cin, storePhone);
        cout << "Enter store email: ";
        string storeEmail;
        getline(cin, storeEmail);
        seller newSeller(newBuyer, newId, storeName, storeAddress, storePhone, storeEmail);
        sellers.push_back(newSeller);
        currentUserIsSeller = true;
        cout << "Seller account created successfully!" << endl;
    }
    currentBuyerId = buyers.size() - 1;
    cout << "You are now logged in!" << endl;
    showBuyerMenu();
}
void showBuyerMenu()
{
    BuyerMenu buyerChoice = CHECK_ACCOUNT;
    while (buyerChoice != LOGOUT_BUYER)
    {
        cout << "\n========================================" << endl;
        cout << "   BUYER MENU" << endl;
        cout << "========================================" << endl;
        cout << "1. View Account Status" << endl;
        cout << "2. Upgrade to Seller" << endl;
        cout << "3. Banking Functions" << endl;
        cout << "4. Browse & Shop" << endl;
        cout << "5. View Shopping Cart" << endl;
        cout << "6. Checkout" << endl;
        cout << "7. Manage Orders" << endl;
        cout << "8. View Spending Analysis" << endl;
        if (currentUserIsSeller)
        {
            cout << "9. Switch to Seller Mode" << endl;
        }
        cout << "10. Logout" << endl;
        cout << "Select an option: ";
        int choice;
        cin >> choice;
        buyerChoice = static_cast<BuyerMenu>(choice - 1);
        switch (buyerChoice)
        {
        case CHECK_ACCOUNT:
            cout << "\n=== ACCOUNT STATUS ===" << endl;
            cout << "Name: " << buyers[currentBuyerId].getName() << endl;
            cout << "Address: " << buyers[currentBuyerId].getAddress() << endl;
            cout << "Phone: " << buyers[currentBuyerId].getPhone() << endl;
            cout << "Email: " << buyers[currentBuyerId].getEmail() << endl;
            cout << "Account Type: Buyer";
            if (currentUserIsSeller)
            {
                cout << " & Seller";
                for (size_t i = 0; i < sellers.size(); i++)
                {
                    if (sellers[i].getName() == buyers[currentBuyerId].getName())
                    {
                        cout << "\n--- Store Information ---" << endl;
                        cout << "Store Name: " << sellers[i].getStoreName() << endl;
                        cout << "Store Address: " << sellers[i].getStoreAddress() << endl;
                        cout << "Store Phone: " << sellers[i].getStorePhone() << endl;
                        cout << "Store Email: " << sellers[i].getStoreEmail() << endl;
                        break;
                    }
                }
            }
            cout << endl;
            buyers[currentBuyerId].getAccount().printInfo();
            break;
        case UPGRADE_SELLER:
            handleSellerUpgrade();
            break;
        case BANK_FUNCTIONS:
            handleBankFunctions();
            break;
        case BROWSE_SHOP:
            handleBrowseShop();
            break;
        case VIEW_CART:
            handleViewCart();
            break;
        case CHECKOUT:
            handleCheckout();
            break;
        case MANAGE_ORDERS:
            handleManageOrders();
            break;
        case VIEW_SPENDING:
            handleViewSpending();
            break;
        case SELLER_MODE:
            if (currentUserIsSeller)
            {
                handleSellerMenu();
            }
            else
            {
                if (buyers[currentBuyerId].getAccount().getBalance() <= 0)
                {
                    cout << "You need funds in your bank account to become a seller!" << endl;
                    break;
                }
                handleSellerUpgrade();
            }
            break;
        case LOGOUT_BUYER:
            cout << "Logging out..." << endl;
            currentBuyerId = -1;
            currentUserIsSeller = false;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}
void handleSellerUpgrade()
{
    if (currentUserIsSeller)
    {
        cout << "You already have a seller account!" << endl;
        return;
    }
    if (buyers[currentBuyerId].getAccount().getBalance() <= 0)
    {
        cout << "You need a bank account with funds to upgrade to seller!" << endl;
        return;
    }
    cout << "\n=== SELLER UPGRADE ===" << endl;
    cout << "Enter store name: ";
    cin.ignore();
    string storeName;
    getline(cin, storeName);
    cout << "Enter store address: ";
    string storeAddress;
    getline(cin, storeAddress);
    cout << "Enter store phone: ";
    string storePhone;
    getline(cin, storePhone);
    cout << "Enter store email: ";
    string storeEmail;
    getline(cin, storeEmail);
    seller newSeller(buyers[currentBuyerId], buyers[currentBuyerId].getId(), storeName, storeAddress, storePhone, storeEmail);
    sellers.push_back(newSeller);
    currentUserIsSeller = true;
    cout << "Seller account created successfully!" << endl;
    cout << "Store: " << storeName << endl;
}
void handleBankFunctions()
{
    BankMenu bankChoice = CHECK_BALANCE;
    while (bankChoice != BACK_TO_BUYER)
    {
        cout << "\n=== BANKING FUNCTIONS ===" << endl;
        cout << "1. Check Balance" << endl;
        cout << "2. Deposit (Top-up)" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. View Transaction History" << endl;
        cout << "5. Back to Buyer Menu" << endl;
        int choice;
        cin >> choice;
        bankChoice = static_cast<BankMenu>(choice - 1);
        switch (bankChoice)
        {
        case CHECK_BALANCE:
            cout << "\n=== BALANCE ===" << endl;
            buyers[currentBuyerId].getAccount().printInfo();
            break;
        case BANK_DEPOSIT:
        {
            double amount;
            cout << "Enter deposit amount: $";
            cin >> amount;
            if (amount > 0)
            {
                buyers[currentBuyerId].getAccount().addBalance(amount);
                mainBank.recordTransaction(buyers[currentBuyerId].getId(), DEPOSIT,
                                           amount, "Deposit to account");
                cout << "Deposit successful! New balance: $"
                     << buyers[currentBuyerId].getAccount().getBalance() << endl;
            }
            else
            {
                cout << "Invalid amount!" << endl;
            }
        }
        break;
        case BANK_WITHDRAW:
        {
            double amount;
            cout << "Enter withdrawal amount: $";
            cin >> amount;
            if (buyers[currentBuyerId].getAccount().withdrawBalance(amount))
            {
                mainBank.recordTransaction(buyers[currentBuyerId].getId(), WITHDRAW,
                                           amount, "Withdrawal from account");
                cout << "Withdrawal successful! New balance: $"
                     << buyers[currentBuyerId].getAccount().getBalance() << endl;
            }
        }
        break;
        case VIEW_TRANSACTIONS:
        {
            int days;
            cout << "View transactions for:\n1. Today\n2. Last 30 days\n3. All time\nChoice: ";
            int transChoice;
            cin >> transChoice;
            vector<Transaction> transactions;
            if (transChoice == 1)
            {
                days = 1;
                transactions = mainBank.getAccountTransactionsLastKDays(
                    buyers[currentBuyerId].getId(), days);
                cout << "\n=== Transactions Today ===" << endl;
            }
            else if (transChoice == 2)
            {
                days = 30;
                transactions = mainBank.getAccountTransactionsLastKDays(
                    buyers[currentBuyerId].getId(), days);
                cout << "\n=== Transactions Last 30 Days ===" << endl;
            }
            else
            {
                transactions = mainBank.getAccountTransactions(
                    buyers[currentBuyerId].getId());
                cout << "\n=== All Transactions ===" << endl;
            }
            if (transactions.empty())
            {
                cout << "No transactions found." << endl;
            }
            else
            {
                for (const auto &trans : transactions)
                {
                    trans.display();
                }
                cout << "Total transactions: " << transactions.size() << endl;
            }
        }
        break;
        case BACK_TO_BUYER:
            cout << "Returning to buyer menu..." << endl;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}
bool findBuyerByName(const string &name, int &buyerIndex)
{
    for (size_t i = 0; i < buyers.size(); i++)
    {
        if (buyers[i].getName() == name)
        {
            buyerIndex = i;
            return true;
        }
    }
    return false;
}
int findSellerIndexByBuyerId(int buyerId)
{
    for (size_t i = 0; i < sellers.size(); i++)
    {
        if (sellers[i].getId() == buyerId)
        {
            return i;
        }
    }
    return -1;
}
void handleSellerInventory()
{
    int sellerIndex = findSellerIndexByBuyerId(buyers[currentBuyerId].getId());
    if (sellerIndex == -1)
    {
        cout << "Error: Seller account not found!" << endl;
        return;
    }
    SellerInventoryMenu inventoryChoice = VIEW_INVENTORY;
    while (inventoryChoice != BACK_TO_MAIN_MENU)
    {
        cout << "\n=== INVENTORY MANAGEMENT ===" << endl;
        cout << "Store: " << sellers[sellerIndex].getStoreName() << endl;
        cout << "1. View All Items" << endl;
        cout << "2. Add New Item" << endl;
        cout << "3. Update Item" << endl;
        cout << "4. Set Item Price" << endl;
        cout << "5. Replenish Stock" << endl;
        cout << "6. Discard Stock" << endl;
        cout << "7. Toggle Visibility" << endl;
        cout << "8. Back" << endl;
        int choice;
        cin >> choice;
        inventoryChoice = static_cast<SellerInventoryMenu>(choice - 1);
        switch (inventoryChoice)
        {
        case VIEW_INVENTORY:
        {
            cout << "\n=== CURRENT INVENTORY ===" << endl;
            sellers[sellerIndex].displayInventory();
            break;
        }
        case ADD_ITEM:
        {
            cout << "\n=== ADD NEW ITEM ===" << endl;
            int itemId;
            string itemName;
            int quantity;
            double price;
            cout << "Enter Item ID: ";
            cin >> itemId;
            cin.ignore();
            cout << "Enter Item Name: ";
            getline(cin, itemName);
            cout << "Enter Quantity: ";
            cin >> quantity;
            cout << "Enter Price: $";
            cin >> price;
            if (itemId <= 0)
            {
                cout << "Error: Item ID must be positive!" << endl;
                break;
            }
            if (quantity < 0)
            {
                cout << "Error: Quantity cannot be negative!" << endl;
                break;
            }
            if (price < 0)
            {
                cout << "Error: Price cannot be negative!" << endl;
                break;
            }
            if (itemName.empty())
            {
                cout << "Error: Item name cannot be empty!" << endl;
                break;
            }
            if (sellers[sellerIndex].addNewItem(itemId, itemName, quantity, price))
            {
                cout << "Item added successfully!" << endl;
                cout << "Item ID: " << itemId << endl;
                cout << "Name: " << itemName << endl;
                cout << "Quantity: " << quantity << endl;
                cout << "Price: $" << price << endl;
                cout << "Note: Item is hidden by default. Use 'Toggle Item Visibility' to make it visible to customers." << endl;
            }
            break;
        }
        case UPDATE_ITEM:
        {
            cout << "\n=== UPDATE ITEM ===" << endl;
            int itemId;
            string newName;
            int newQuantity;
            double newPrice;
            cout << "Enter Item ID to update: ";
            cin >> itemId;
            if (!sellers[sellerIndex].itemExists(itemId))
            {
                cout << "Error: Item with ID " << itemId << " not found!" << endl;
                break;
            }
            cin.ignore();
            cout << "Enter new Item Name: ";
            getline(cin, newName);
            cout << "Enter new Quantity: ";
            cin >> newQuantity;
            cout << "Enter new Price: $";
            cin >> newPrice;
            if (newQuantity < 0)
            {
                cout << "Error: Quantity cannot be negative!" << endl;
                break;
            }
            if (newPrice < 0)
            {
                cout << "Error: Price cannot be negative!" << endl;
                break;
            }
            if (newName.empty())
            {
                cout << "Error: Item name cannot be empty!" << endl;
                break;
            }
            sellers[sellerIndex].updateItem(itemId, newName, newQuantity, newPrice);
            cout << "Item updated successfully!" << endl;
            cout << "Item ID: " << itemId << endl;
            cout << "New Name: " << newName << endl;
            cout << "New Quantity: " << newQuantity << endl;
            cout << "New Price: $" << newPrice << endl;
            break;
        }
        case SET_PRICE:
        {
            int itemId;
            double newPrice;
            cout << "\n=== SET ITEM PRICE ===" << endl;
            cout << "Enter Item ID: ";
            cin >> itemId;
            cout << "Enter new Price: $";
            cin >> newPrice;
            sellers[sellerIndex].setItemPrice(itemId, newPrice);
            break;
        }
        case REPLENISH_STOCK:
        {
            int itemId, quantity;
            cout << "\n=== REPLENISH STOCK ===" << endl;
            cout << "Enter Item ID: ";
            cin >> itemId;
            cout << "Enter quantity to add: ";
            cin >> quantity;
            sellers[sellerIndex].replenishItem(itemId, quantity);
            break;
        }
        case DISCARD_STOCK:
        {
            int itemId, quantity;
            cout << "\n=== DISCARD STOCK ===" << endl;
            cout << "Enter Item ID: ";
            cin >> itemId;
            cout << "Enter quantity to discard: ";
            cin >> quantity;
            sellers[sellerIndex].discardItem(itemId, quantity);
            break;
        }
        case TOGGLE_ITEM_VISIBILITY:
        {
            cout << "\n=== TOGGLE ITEM VISIBILITY ===" << endl;
            int itemId;
            cout << "Enter Item ID: ";
            cin >> itemId;
            if (!sellers[sellerIndex].itemExists(itemId))
            {
                cout << "Error: Item with ID " << itemId << " not found!" << endl;
                break;
            }
            sellers[sellerIndex].makeItemVisibleToCustomer(itemId);
            cout << "Visibility toggled!" << endl;
            break;
        }
        case BACK_TO_MAIN_MENU:
            cout << "Returning..." << endl;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}
void clearInputBuffer()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
void handleBrowseShop()
{
    if (sellers.empty())
    {
        cout << "\nNo sellers available yet." << endl;
        return;
    }
    cout << "\n=== BROWSE STORES ===" << endl;
    for (size_t i = 0; i < sellers.size(); i++)
    {
        cout << (i + 1) << ". " << sellers[i].getStoreName()
             << " (Seller ID: " << sellers[i].getId() << ")" << endl;
    }
    cout << "\nSelect a store (0 to cancel): ";
    int storeChoice;
    cin >> storeChoice;
    if (storeChoice < 1 || storeChoice > static_cast<int>(sellers.size()))
    {
        return;
    }
    int sellerIdx = storeChoice - 1;
    sellers[sellerIdx].displayVisibleItems();
    cout << "\nAdd item to cart? (y/n): ";
    char addChoice;
    cin >> addChoice;
    if (addChoice == 'y' || addChoice == 'Y')
    {
        int itemId, quantity;
        cout << "Enter Item ID: ";
        cin >> itemId;
        cout << "Enter Quantity: ";
        cin >> quantity;
        Item *item = sellers[sellerIdx].getItemById(itemId);
        if (item && item->getQuantity() >= quantity)
        {
            buyers[currentBuyerId].addToCart(itemId, item->getName(),
                                             sellers[sellerIdx].getId(),
                                             quantity, item->getPrice());
        }
        else if (item)
        {
            cout << "Not enough stock! Available: " << item->getQuantity() << endl;
        }
        else
        {
            cout << "Item not found!" << endl;
        }
    }
}
void handleViewCart()
{
    buyers[currentBuyerId].viewCart();
    if (buyers[currentBuyerId].getCart().empty())
    {
        return;
    }
    cout << "\nRemove item? (y/n): ";
    char removeChoice;
    cin >> removeChoice;
    if (removeChoice == 'y' || removeChoice == 'Y')
    {
        int index;
        cout << "Enter item number to remove: ";
        cin >> index;
        buyers[currentBuyerId].removeFromCart(index - 1);
    }
}
void handleCheckout()
{
    const vector<CartItem> &cart = buyers[currentBuyerId].getCart();
    if (cart.empty())
    {
        cout << "\nYour cart is empty!" << endl;
        return;
    }
    buyers[currentBuyerId].viewCart();
    double total = 0.0;
    for (const auto &item : cart)
    {
        total += item.quantity * item.price;
    }
    cout << "\nProceed with checkout? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm != 'y' && confirm != 'Y')
    {
        return;
    }
    if (buyers[currentBuyerId].getAccount().getBalance() < total)
    {
        cout << "\nInsufficient balance! Your balance: $"
             << buyers[currentBuyerId].getAccount().getBalance() << endl;
        return;
    }
    map<int, vector<CartItem>> itemsBySeller;
    for (const auto &item : cart)
    {
        itemsBySeller[item.sellerId].push_back(item);
    }
    for (const auto &sellerItems : itemsBySeller)
    {
        int sellerId = sellerItems.first;
        int orderIdx = mainStore.createOrder(buyers[currentBuyerId].getId(), sellerId);
        Order &order = mainStore.getOrder(orderIdx);
        for (const auto &item : sellerItems.second)
        {
            order.addItem(item.itemId, item.itemName, item.quantity, item.price);
            for (size_t i = 0; i < sellers.size(); i++)
            {
                if (sellers[i].getId() == sellerId)
                {
                    sellers[i].reduceItemQuantity(item.itemId, item.quantity);
                    break;
                }
            }
        }
        double orderTotal = order.getTotalAmount();
        buyers[currentBuyerId].getAccount().withdrawBalance(orderTotal);
        for (size_t i = 0; i < buyers.size(); i++)
        {
            if (buyers[i].getId() == sellerId)
            {
                buyers[i].getAccount().addBalance(orderTotal);
                break;
            }
        }
        mainBank.recordTransaction(buyers[currentBuyerId].getId(), PURCHASE,
                                   orderTotal, "Purchase - Order #" + to_string(order.getOrderId()));
        mainBank.recordTransaction(sellerId, PAYMENT_RECEIVED,
                                   orderTotal, "Sale - Order #" + to_string(order.getOrderId()));
        order.setStatus(PAID);
        buyers[currentBuyerId].addOrderToHistory(orderIdx);
        cout << "\nOrder #" << order.getOrderId() << " created and paid successfully!" << endl;
    }
    buyers[currentBuyerId].clearCart();
    cout << "\nCheckout complete! Total paid: $" << total << endl;
    cout << "New balance: $" << buyers[currentBuyerId].getAccount().getBalance() << endl;
}
void handleManageOrders()
{
    int choice = 0;
    while (choice != 6)
    {
        cout << "\n=== MANAGE ORDERS ===" << endl;
        cout << "1. View All Orders" << endl;
        cout << "2. View Pending Orders" << endl;
        cout << "3. View Paid Orders" << endl;
        cout << "4. View Completed Orders" << endl;
        cout << "5. View Cancelled Orders" << endl;
        cout << "6. Back" << endl;
        cout << "Select an option: ";
        cin >> choice;
        const vector<int> &orderHistory = buyers[currentBuyerId].getOrderHistory();
        const vector<Order> &allOrders = mainStore.getAllOrders();
        OrderStatus filter;
        string statusName;
        switch (choice)
        {
        case 1:
            filter = static_cast<OrderStatus>(-1);
            statusName = "All";
            break;
        case 2:
            filter = PENDING;
            statusName = "Pending";
            break;
        case 3:
            filter = PAID;
            statusName = "Paid";
            break;
        case 4:
            filter = COMPLETED;
            statusName = "Completed";
            break;
        case 5:
            filter = CANCELLED;
            statusName = "Cancelled";
            break;
        case 6:
            continue;
        default:
            cout << "Invalid option." << endl;
            continue;
        }
        cout << "\n=== Your Orders (" << statusName << ") ===" << endl;
        int count = 0;
        for (int orderIdx : orderHistory)
        {
            if (orderIdx < static_cast<int>(allOrders.size()))
            {
                const Order &order = allOrders[orderIdx];
                if (choice == 1 || order.getStatus() == filter)
                {
                    order.display();
                    count++;
                }
            }
        }
        if (count == 0)
            cout << "No orders found." << endl;
        else
            cout << "Total: " << count << " orders" << endl;
    }
}
void handleViewSpending()
{
    int days;
    cout << "\nEnter number of days to analyze: ";
    cin >> days;
    double spending = 0.0;
    const vector<int> &orderHistory = buyers[currentBuyerId].getOrderHistory();
    const vector<Order> &allOrders = mainStore.getAllOrders();
    for (int orderIdx : orderHistory)
    {
        if (orderIdx < static_cast<int>(allOrders.size()))
        {
            const Order &order = allOrders[orderIdx];
            if (order.isWithinDays(days) &&
                (order.getStatus() == PAID || order.getStatus() == COMPLETED))
            {
                spending += order.getTotalAmount();
            }
        }
    }
    cout << "\nTotal spending in last " << days << " days: $" << spending << endl;
    vector<Transaction> transactions = mainBank.getAccountTransactionsLastKDays(
        buyers[currentBuyerId].getId(), days);
    cout << "\n--- Cash Flow (Last " << days << " Days) ---" << endl;
    double totalCredit = 0, totalDebit = 0;
    for (const auto &trans : transactions)
    {
        if (trans.getType() == DEPOSIT || trans.getType() == PAYMENT_RECEIVED ||
            trans.getType() == REFUND)
        {
            totalCredit += trans.getAmount();
        }
        else
        {
            totalDebit += trans.getAmount();
        }
    }
    cout << "Total Credit: $" << totalCredit << endl;
    cout << "Total Debit: $" << totalDebit << endl;
    cout << "Net: $" << (totalCredit - totalDebit) << endl;
}
void handleSellerMenu()
{
    int sellerIdx = findSellerIndexByBuyerId(buyers[currentBuyerId].getId());
    if (sellerIdx == -1)
    {
        cout << "Error: Seller account not found!" << endl;
        return;
    }
    int choice = 0;
    while (choice != 6)
    {
        cout << "\n========================================" << endl;
        cout << "   SELLER MENU - " << sellers[sellerIdx].getStoreName() << endl;
        cout << "========================================" << endl;
        cout << "1. Manage Inventory" << endl;
        cout << "2. View Orders" << endl;
        cout << "3. Complete Order" << endl;
        cout << "4. Analytics & Reports" << endl;
        cout << "5. View Revenue" << endl;
        cout << "6. Back to Buyer Menu" << endl;
        cout << "Select an option: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
            handleSellerInventory();
            break;
        case 2:
        {
            cout << "\n=== Your Orders ===" << endl;
            const vector<Order> &allOrders = mainStore.getAllOrders();
            int count = 0;
            for (const auto &order : allOrders)
            {
                if (order.getSellerId() == sellers[sellerIdx].getId())
                {
                    order.display();
                    count++;
                }
            }
            if (count == 0)
                cout << "No orders yet." << endl;
            break;
        }
        case 3:
        {
            int orderId;
            cout << "\nEnter Order ID to complete: ";
            cin >> orderId;
            bool found = false;
            for (int i = 0; i < mainStore.getOrderCount(); i++)
            {
                Order &order = mainStore.getOrder(i);
                if (order.getOrderId() == orderId &&
                    order.getSellerId() == sellers[sellerIdx].getId())
                {
                    if (order.getStatus() == PAID)
                    {
                        order.setStatus(COMPLETED);
                        cout << "Order #" << orderId << " marked as completed!" << endl;
                    }
                    else
                    {
                        cout << "Order must be paid before completion!" << endl;
                    }
                    found = true;
                    break;
                }
            }
            if (!found)
                cout << "Order not found or doesn't belong to you!" << endl;
            break;
        }
        case 4:
            handleSellerAnalytics();
            break;
        case 5:
        {
            int days;
            cout << "\nEnter number of days to analyze: ";
            cin >> days;
            double revenue = 0.0;
            const vector<Order> &allOrders = mainStore.getAllOrders();
            for (const auto &order : allOrders)
            {
                if (order.getSellerId() == sellers[sellerIdx].getId() &&
                    order.isWithinDays(days) &&
                    (order.getStatus() == PAID || order.getStatus() == COMPLETED))
                {
                    revenue += order.getTotalAmount();
                }
            }
            cout << "\nTotal revenue in last " << days << " days: $" << revenue << endl;
            break;
        }
        case 6:
            cout << "Returning to buyer menu..." << endl;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}
void handleSellerAnalytics()
{
    int sellerIdx = findSellerIndexByBuyerId(buyers[currentBuyerId].getId());
    if (sellerIdx == -1)
        return;
    int choice = 0;
    while (choice != 3)
    {
        cout << "\n=== SELLER ANALYTICS ===" << endl;
        cout << "1. Top Popular Items This Month" << endl;
        cout << "2. Loyal Customers This Month" << endl;
        cout << "3. Back" << endl;
        cout << "Select an option: ";
        cin >> choice;
        const vector<Order> &allOrders = mainStore.getAllOrders();
        switch (choice)
        {
        case 1:
        {
            int k;
            cout << "Enter number of top items to display: ";
            cin >> k;
            cout << "\n=== Top " << k << " Most Popular Items This Month ===" << endl;
            map<int, pair<string, int>> itemSales;
            for (const auto &order : allOrders)
            {
                if (order.getSellerId() == sellers[sellerIdx].getId() &&
                    order.isWithinDays(30))
                {
                    for (const auto &item : order.getItems())
                    {
                        if (itemSales.find(item.itemId) == itemSales.end())
                        {
                            itemSales[item.itemId] = make_pair(item.itemName, 0);
                        }
                        itemSales[item.itemId].second += item.quantity;
                    }
                }
            }
            vector<pair<int, pair<string, int>>> itemVec(itemSales.begin(), itemSales.end());
            sort(itemVec.begin(), itemVec.end(),
                 [](const pair<int, pair<string, int>> &a, const pair<int, pair<string, int>> &b)
                 {
                     return a.second.second > b.second.second;
                 });
            int displayCount = min(k, static_cast<int>(itemVec.size()));
            for (int i = 0; i < displayCount; i++)
            {
                cout << (i + 1) << ". Item ID: " << itemVec[i].first
                     << " - " << itemVec[i].second.first
                     << " (Sold: " << itemVec[i].second.second << " units)" << endl;
            }
            if (itemVec.empty())
                cout << "No sales this month." << endl;
            break;
        }
        case 2:
        {
            int n;
            cout << "Enter number of top customers to display: ";
            cin >> n;
            cout << "\n=== Top " << n << " Loyal Customers This Month ===" << endl;
            map<int, int> customerPurchases;
            for (const auto &order : allOrders)
            {
                if (order.getSellerId() == sellers[sellerIdx].getId() &&
                    order.isWithinDays(30) &&
                    (order.getStatus() == PAID || order.getStatus() == COMPLETED))
                {
                    customerPurchases[order.getBuyerId()]++;
                }
            }
            vector<pair<int, int>> customerVec(customerPurchases.begin(), customerPurchases.end());
            sort(customerVec.begin(), customerVec.end(),
                 [](const pair<int, int> &a, const pair<int, int> &b)
                 {
                     return a.second > b.second;
                 });
            int displayCount = min(n, static_cast<int>(customerVec.size()));
            for (int i = 0; i < displayCount; i++)
            {
                cout << (i + 1) << ". Buyer ID: " << customerVec[i].first
                     << " - Purchases: " << customerVec[i].second << endl;
            }
            if (customerVec.empty())
                cout << "No customers this month." << endl;
            break;
        }
        case 3:
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}
void handleStoreAnalytics()
{
    int choice = 0;
    while (choice != 7)
    {
        cout << "\n=== STORE ANALYTICS (ADMIN) ===" << endl;
        cout << "1. Transactions in Last K Days" << endl;
        cout << "2. Paid but Incomplete Orders" << endl;
        cout << "3. Most Frequent Items" << endl;
        cout << "4. Most Active Buyers Today" << endl;
        cout << "5. Most Active Sellers Today" << endl;
        cout << "6. Bank Analytics" << endl;
        cout << "7. Back" << endl;
        cout << "Select an option: ";
        cin >> choice;
        switch (choice)
        {
        case 1:
        {
            int k;
            cout << "Enter number of days: ";
            cin >> k;
            mainStore.listTransactionsLastKDays(k);
            break;
        }
        case 2:
            mainStore.listPaidButIncomplete();
            break;
        case 3:
        {
            int m;
            cout << "Enter number of top items: ";
            cin >> m;
            mainStore.listTopMFrequentItems(m);
            break;
        }
        case 4:
        {
            int n;
            cout << "Enter number of top buyers: ";
            cin >> n;
            mainStore.listMostActiveBuyers(n);
            break;
        }
        case 5:
        {
            int n;
            cout << "Enter number of top sellers: ";
            cin >> n;
            mainStore.listMostActiveSellers(n);
            break;
        }
        case 6:
        {
            int bankChoice = 0;
            while (bankChoice != 5)
            {
                cout << "\n=== BANK ANALYTICS ===" << endl;
                cout << "1. Transactions Within a Week" << endl;
                cout << "2. All Bank Customers" << endl;
                cout << "3. Dormant Accounts" << endl;
                cout << "4. Top Users Today" << endl;
                cout << "5. Back" << endl;
                cout << "Select an option: ";
                cin >> bankChoice;
                switch (bankChoice)
                {
                case 1:
                    mainBank.listWeekTransactions();
                    break;
                case 2:
                    mainBank.listAllCustomers();
                    break;
                case 3:
                    mainBank.listDormantAccounts();
                    break;
                case 4:
                {
                    int n;
                    cout << "Enter number of top users: ";
                    cin >> n;
                    mainBank.listTopUsersToday(n);
                    break;
                }
                case 5:
                    break;
                default:
                    cout << "Invalid option." << endl;
                    break;
                }
            }
            break;
        }
        case 7:
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
    }
}

// Serialization Implementation
void saveData()
{
    cout << "Saving data..." << endl;
    saveBankCustomers();
    saveBuyers();
    saveSellers();
    cout << "Data saved successfully!" << endl;
}

void loadData()
{
    cout << "Loading data..." << endl;
    loadBankCustomers();
    loadBuyers();
    loadSellers();
    cout << "Data loaded successfully!" << endl;
}

void saveBankCustomers()
{
    ofstream file("bank_customers.dat");
    if (!file.is_open())
    {
        cerr << "Error: Could not open bank_customers.dat for writing" << endl;
        return;
    }

    file << bankCustomers.size() << endl;
    for (const auto &customer : bankCustomers)
    {
        file << customer.getId() << endl;
        file << customer.getName() << endl;
        file << customer.getBalance() << endl;
    }
    file.close();
}

void loadBankCustomers()
{
    ifstream file("bank_customers.dat");
    if (!file.is_open())
    {
        cout << "No existing bank customer data found. Starting fresh." << endl;
        return;
    }

    bankCustomers.clear();
    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++)
    {
        int id;
        string name;
        double balance;

        file >> id;
        file.ignore();
        getline(file, name);
        file >> balance;
        file.ignore();

        BankCustomer customer(id, name, balance);
        bankCustomers.push_back(customer);
    }
    file.close();
}

void saveBuyers()
{
    ofstream file("buyers.dat");
    if (!file.is_open())
    {
        cerr << "Error: Could not open buyers.dat for writing" << endl;
        return;
    }

    file << buyers.size() << endl;
    for (const auto &buyer : buyers)
    {
        file << buyer.getId() << endl;
        file << buyer.getName() << endl;
        file << buyer.getAddress() << endl;
        file << buyer.getPhone() << endl;
        file << buyer.getEmail() << endl;
    }
    file.close();
}

void loadBuyers()
{
    ifstream file("buyers.dat");
    if (!file.is_open())
    {
        cout << "No existing buyer data found. Starting fresh." << endl;
        return;
    }

    buyers.clear();
    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++)
    {
        int id;
        string name, address, phone, email;

        file >> id;
        file.ignore();
        getline(file, name);
        getline(file, address);
        getline(file, phone);
        getline(file, email);

        // Find corresponding bank customer
        bool found = false;
        for (auto &bankCustomer : bankCustomers)
        {
            if (bankCustomer.getId() == id)
            {
                Buyer buyer(id, name, address, phone, email, bankCustomer);
                buyers.push_back(buyer);
                found = true;
                break;
            }
        }

        if (!found)
        {
            cerr << "Warning: Bank customer not found for buyer ID " << id << endl;
        }
    }
    file.close();
}

void saveSellers()
{
    ofstream file("sellers.dat");
    if (!file.is_open())
    {
        cerr << "Error: Could not open sellers.dat for writing" << endl;
        return;
    }

    file << sellers.size() << endl;
    for (const auto &seller : sellers)
    {
        file << seller.getId() << endl;
        file << seller.getName() << endl;
        file << seller.getAddress() << endl;
        file << seller.getPhone() << endl;
        file << seller.getEmail() << endl;
        file << seller.getSellerId() << endl;
        file << seller.getStoreName() << endl;
        file << seller.getStoreAddress() << endl;
        file << seller.getStorePhone() << endl;
        file << seller.getStoreEmail() << endl;
    }
    file.close();
}

void loadSellers()
{
    ifstream file("sellers.dat");
    if (!file.is_open())
    {
        cout << "No existing seller data found. Starting fresh." << endl;
        return;
    }

    sellers.clear();
    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++)
    {
        int id, sellerId;
        string name, address, phone, email;
        string storeName, storeAddress, storePhone, storeEmail;

        file >> id;
        file.ignore();
        getline(file, name);
        getline(file, address);
        getline(file, phone);
        getline(file, email);
        file >> sellerId;
        file.ignore();
        getline(file, storeName);
        getline(file, storeAddress);
        getline(file, storePhone);
        getline(file, storeEmail);

        // Find corresponding buyer
        bool found = false;
        for (auto &buyer : buyers)
        {
            if (buyer.getId() == id)
            {
                seller newSeller(buyer, sellerId, storeName, storeAddress, storePhone, storeEmail);
                sellers.push_back(newSeller);
                found = true;
                break;
            }
        }

        if (!found)
        {
            cerr << "Warning: Buyer not found for seller ID " << id << endl;
        }
    }
    file.close();
}