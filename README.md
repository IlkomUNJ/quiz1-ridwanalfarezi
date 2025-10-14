## 👤 Identity

**Name:** Ridwan Alfarezi  
**NIM:** 1313624020  
**Course:** DPBO (Design & Object-Oriented Programming)

---

## 📋 Project Overview

A comprehensive **Online Marketplace System** built in C++ featuring buyer-seller interactions, shopping cart, order management, inventory control, and analytics. This project implements object-oriented programming concepts including inheritance, encapsulation, and composition.

### Key Features:

- 🛒 **Shopping System**: Browse stores, shopping cart, checkout
- 💼 **Seller Management**: Inventory control, order fulfillment, analytics
- 🏦 **Banking System**: Accounts, transactions, deposits, withdrawals
- 📊 **Analytics**: Sales tracking, customer insights, financial reports
- 👥 **User Management**: Registration, login, profile management

---

## 🚀 Quick Start

### Prerequisites

- C++11 or higher compiler (g++, MSVC, or clang++)
- Windows with MinGW, WSL, or Linux

### Compilation Options

#### **Method 1: WSL (Recommended - Easiest)**

```bash
wsl
cd /mnt/d/College/SMT\ 3/DPBO/quiz1-ridwanalfarezi
g++ main.cpp bank_customer.cpp -o marketplace -std=c++11
./marketplace
```

#### **Method 2: Direct g++ (May have linking issues on Windows)**

```bash
cd quiz1-ridwanalfarezi
g++ main.cpp bank_customer.cpp -o marketplace.exe -std=c++11
./marketplace.exe
```

#### **Method 3: Visual Studio (MSVC)**

```bash
cl /EHsc /std:c++11 main.cpp bank_customer.cpp /Fe:marketplace.exe
marketplace.exe
```

#### **Method 4: Meson Build System**

```bash
cd builddir
meson compile
./my_app.exe
```

---

## 🎮 How to Use

### 1. Main Menu

```
1. Register      → Create new buyer account
2. Login         → Enter the system
3. List Buyers   → See all registered users
4. Store Analytics → Admin view of marketplace data
5. Bank Analytics  → Admin view of banking data
6. Exit          → Quit program
```

### 2. Quick Test Scenario (5 Minutes)

#### Step 1: Register Two Users

```
Main Menu → 1 (Register)

User 1 - Alice (Buyer):
- Name: Alice
- Address: Jakarta Street 123
- Phone: 081234567890
- Email: alice@test.com
- Initial Deposit: 10000000 (10 million)

User 2 - Bob (Seller):
- Name: Bob
- Address: Bandung Street 456
- Phone: 081987654321
- Email: bob@test.com
- Initial Deposit: 5000000 (5 million)
```

#### Step 2: Create Seller Inventory

```
Main Menu → 2 (Login) → Enter ID: 2 (Bob)
Buyer Menu → 10 (Seller Mode)
Seller Menu → 1 (Manage Inventory)
Inventory Menu → 1 (Add Item)

Add items:
- Laptop Dell / 8000000 / 5
- Mouse Wireless / 150000 / 10
- Keyboard Mechanical / 500000 / 8
```

#### Step 3: Shop as Buyer

```
Logout → Login as Alice (ID: 1)
Buyer Menu → 5 (Browse & Shop)
Select Bob's store → Add to cart:
- Laptop: quantity 1
- Mouse: quantity 2
Exit (0)
```

#### Step 4: Checkout

```
Buyer Menu → 6 (View Cart) → Check items
Buyer Menu → 7 (Checkout) → Confirm (Y)
```

#### Step 5: Complete Order (As Seller)

```
Logout → Login as Bob (ID: 2)
Buyer Menu → 10 (Seller Mode)
Seller Menu → 2 (View Orders)
Enter order ID to complete → Confirm
```

#### Step 6: View Analytics

```
Seller Menu → 3 (View Analytics)
→ 1 (Popular Items) → Enter 5
→ 2 (Loyal Customers) → Enter 3

Logout → Main Menu
Main Menu → 4 (Store Analytics)
→ 4 (Most Active Buyers) → Enter 5
```

---

## 📖 Detailed Features

### 🛍️ Buyer Features

#### Shopping & Cart Management

- **Browse Stores**: View all sellers and their available items
- **Shopping Cart**: Add/remove items, view cart, calculate totals
- **Checkout**: Process cart with automatic payment and inventory updates
- **Order History**: Track all your orders with status filtering

#### Order Management

- View all orders (pending/completed/cancelled)
- Filter orders by status
- Track order details and total amounts

#### Financial Analysis

- **Spending Analysis**: View spending by time period (7/30/90 days)
- **Cash Flow**: Track money in (deposits, refunds) vs money out (purchases)
- **Transaction History**: View all transactions (today/30 days/all time)

#### Banking Operations

- Check account balance
- Deposit money (top-up)
- Withdraw funds
- View transaction history with filters

### 🏪 Seller Features

Sellers inherit all buyer features plus:

#### Inventory Management

- **Add Items**: Create new products with name, price, quantity
- **View Inventory**: See all items with stock levels
- **Set Price**: Update item prices
- **Replenish Stock**: Add quantity to existing items
- **Discard Stock**: Remove quantity (damaged goods, etc.)
- **Deactivate Items**: Remove items from sale

#### Order Processing

- View pending customer orders
- Complete orders (marks as fulfilled and ships)
- Track order details and customer information

#### Seller Analytics

- **Popular Items**: Top K most sold items with quantities
- **Loyal Customers**: Top N customers by purchase frequency
- Track sales performance

### 📊 Store Analytics (Admin View)

- **Recent Transactions**: View all marketplace activity in last K days
- **Paid But Incomplete**: Find pending orders that need fulfillment
- **Most Frequent Items**: Top M items sold across all sellers
- **Most Active Buyers**: Customers with most purchases
- **Most Active Sellers**: Sellers with most sales

### 🏦 Bank Analytics (Admin View)

- **Weekly Transactions**: All banking operations in last 7 days
- **Customer List**: View all registered bank accounts
- **Dormant Accounts**: Find inactive accounts (no activity > K days)
- **Top Active Users**: Most active accounts today