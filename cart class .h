#include<iostream>
#include<string>
#include<vector>
using namespace std;

struct CartItem {
    int productID;
    string name;
    double price;
    int quantity;
    double discount;
};

class Cart {
private:
    int cartID;
    int userID;
    vector<CartItem> items;
    double totalPrice;

public:
    Cart(int cid, int uid) {
        cartID = cid;
        userID = uid;
        totalPrice = 0;
    }

    void displayCart() {
        if (items.empty()) {
            cout << "your cart is empty!" << endl;
            return;
        }
        totalPrice = 0;
        for (int i = 0; i < items.size(); i++) {
            double itemTotal = items[i].price * items[i].quantity;
            if (items[i].discount > 0)
                itemTotal -= itemTotal * items[i].discount / 100;
            totalPrice += itemTotal;
            cout << i + 1 << ". " << items[i].name
                << " | Price: " << items[i].price
                << " | Quantity: " << items[i].quantity
                << " | Discount: " << items[i].discount << "%" << endl;
        }
        cout << "Total price: " << totalPrice << endl;
    }

    void addItem(CartItem item) {
        items.push_back(item);
        cout << "item added successfully!" << endl;
    }

    void removeItem(int index) {
        index--;
        if (index >= 0 && index < items.size()) {
            cout << items[index].name << " removed from cart!" << endl;
            items.erase(items.begin() + index);
        }
        else {
            cout << "invalid number!" << endl;
        }
    }

    void editCart() {
        displayCart();
        if (items.empty()) return;

        int answer;
        do {
            cout << "\nif you want to edit your order:" << endl;
            cout << "1. delete any item" << endl;
            cout << "2. update quantity" << endl;
            cout << "3. clear your cart" << endl;
            cout << "0. done" << endl;
            cout << "enter the number you want: ";
            cin >> answer;

            int index;
            int newQty;

            switch (answer) {
            case 1:
                displayCart();
                cout << "Enter item number to delete: ";
                cin >> index;
                removeItem(index);
                break;

            case 2:
                displayCart();
                cout << "Enter item number to update quantity: ";
                cin >> index;
                index--;
                if (index >= 0 && index < items.size()) {
                    cout << "Enter new quantity: ";
                    cin >> newQty;
                    if (newQty <= 0) {
                        cout << "Quantity must be at least 1!" << endl;
                    }
                    else {
                        items[index].quantity = newQty;
                        cout << "quantity updated!" << endl;
                    }
                }
                else {
                    cout << "invalid number!" << endl;
                }
                break;

            case 3:
                items.clear();
                totalPrice = 0;
                cout << "cart cleared!" << endl;
                break;

            case 0:
                break;

            default:
                cout << "invalid choice!" << endl;
                break;
            }

            if (answer != 0) displayCart();

        } while (answer != 0);
    }
};

// ==================== ORDER ==================== //

class OrderItem {
public:
    int productID;
    string productName;
    int quantity;
    double price;
    double subtotal;

    OrderItem(int id, string n, int qty, double p) {
        productID = id;
        productName = n;
        quantity = qty;
        price = p;
        subtotal = price * quantity;
    }
};

class Order {
private:
    int orderID;
    int userID;
    vector<OrderItem> items;
    double totalAmount;
    string orderStatus;
    string paymentStatus;
    string orderDate;
    string deliveryDate;

public:
    Order(int oid, int uid, string odate) {
        orderID = oid;
        userID = uid;
        orderDate = odate;
        totalAmount = 0;
        paymentStatus = "unpaid";
        orderStatus = "pending";
        deliveryDate = "unknown";
    }

    void addItem(OrderItem item) {
        items.push_back(item);
        totalAmount += item.subtotal;
    }

    void placeOrder() {
        orderStatus = "pending";
        cout << "order placed successfully!" << endl;
    }

    void cancelOrder() {
        if (orderStatus == "pending") {
            orderStatus = "cancelled";
            cout << "order cancelled!" << endl;
        }
        else {
            cout << "cannot cancel this order!" << endl;
        }
    }

    void updateStatus(string newStatus) {
        orderStatus = newStatus;
        cout << "order status updated to: " << orderStatus << endl;
    }

    void trackOrder() {
        cout << "Order ID: " << orderID << endl;
        cout << "Status: " << orderStatus << endl;
        cout << "Order Date: " << orderDate << endl;
        cout << "Delivery Date: " << deliveryDate << endl;
    }

    void displayOrder() {
        for (int i = 0; i < items.size(); i++) {
            cout << i + 1 << ". " << items[i].productName
                << " | Qty: " << items[i].quantity
                << " | Subtotal: " << items[i].subtotal << endl;
        }
        cout << "Total: " << totalAmount << endl;
        cout << "Status: " << orderStatus << endl;
    }
};