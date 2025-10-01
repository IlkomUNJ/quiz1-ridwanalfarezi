#include <iostream>
#include <vector>
#include <string>
#include "bank_customer.h"
#include "buyer.h"
#include "seller.h"
#include "bank.h"

enum PrimaryPrompt
{
    LOGIN,
    REGISTER,
    EXIT
};
enum BuyerMenu
{
    CHECK_ACCOUNT,
    UPGRADE_SELLER,
    BANK_FUNCTIONS,
    LOGOUT_BUYER
};
enum BankMenu
{
    CHECK_BALANCE,
    DEPOSIT,
    WITHDRAW,
    BACK_TO_BUYER
};

using namespace std;

vector<BankCustomer> bankCustomers;
vector<Buyer> buyers;
vector<seller> sellers;
Bank mainBank("Main Bank");
int currentBuyerId = -1;
bool currentUserIsSeller = false;

void handleLogin();
void handleRegister();
void showBuyerMenu();
void handleSellerUpgrade();
void handleBankFunctions();
bool findBuyerByName(const string &name, int &buyerIndex);

int main()
{
    // create a loop prompt
    PrimaryPrompt prompt = LOGIN;
    while (prompt != EXIT)
    {
        cout << "Select an option: " << endl;
        cout << "1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
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
        case EXIT:
            cout << "Exiting." << std::endl;
            break;
        default:
            cout << "Invalid option." << endl;
            break;
        }
        cout << endl;
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

        // Check if user is also a seller
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

    Buyer newBuyer(newId, name, address, phone, email, bankCustomers.back());
    buyers.push_back(newBuyer);

    cout << "Registration successful!" << endl;
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

        // Create seller account
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
        cout << "\n=== BUYER MENU ===" << endl;
        cout << "1. Check Account Status" << endl;
        cout << "2. Upgrade Account to Seller" << endl;
        cout << "3. Banking Functions" << endl;
        cout << "4. Logout" << endl;

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
                // Find and display seller information
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

    // Create seller account
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
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Back to Buyer Menu" << endl;

        int choice;
        cin >> choice;
        bankChoice = static_cast<BankMenu>(choice - 1);

        switch (bankChoice)
        {
        case CHECK_BALANCE:
            cout << "\n=== BALANCE ===" << endl;
            buyers[currentBuyerId].getAccount().printInfo();
            break;

        case DEPOSIT:
        {
            double amount;
            cout << "Enter deposit amount: $";
            cin >> amount;
            if (amount > 0)
            {
                buyers[currentBuyerId].getAccount().addBalance(amount);
                cout << "Deposit successful! New balance: $"
                     << buyers[currentBuyerId].getAccount().getBalance() << endl;
            }
            else
            {
                cout << "Invalid amount!" << endl;
            }
        }
        break;

        case WITHDRAW:
        {
            double amount;
            cout << "Enter withdrawal amount: $";
            cin >> amount;
            if (buyers[currentBuyerId].getAccount().withdrawBalance(amount))
            {
                cout << "Withdrawal successful! New balance: $"
                     << buyers[currentBuyerId].getAccount().getBalance() << endl;
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