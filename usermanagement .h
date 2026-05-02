#include<iostream>
#include<string>
#include<vector>
using namespace std;

class User {
protected:
    int userID;
    string uname;
    string email;
    string password;
    string role;
    bool loggedin;

    string hashPassword(string pass) {
        return to_string(hash<string>{}(pass));
    }

public:
    User(int id, string n, string e, string p, string r) {
        userID = id;
        uname = n;
        email = e;
        password = hashPassword(p);
        role = r;
        loggedin = false;
    }

    void login(string entered_uname, string entered_pass) {
        if (uname == entered_uname && password == hashPassword(entered_pass)) {
            loggedin = true;
            cout << "logged in successfully!" << endl;
            cout << "welcome back " << uname << endl;
        }
        else {
            cout << "invalid username or password, please try again" << endl;
        }
    }
    void logout() {
        if (loggedin == true) {
            loggedin = false;
            cout << "logged out successfully" << endl;
        }
        else {
            cout << "you are not logged in!" << endl;
        }
    }

    void viewProfile() {
        cout << "ID: " << userID << endl;
        cout << "Username: " << uname << endl;
        cout << "Email: " << email << endl;
        cout << "Role: " << role << endl;
    }

    void updateProfile(string new_uname, string new_email) {
        uname = new_uname;
        email = new_email;
        cout << "your profile has been updated!" << endl;
    }

    string getRole() { return role; }
    string getUname() { return uname; }
    int getID() { return userID; }
    bool isLoggedIn() { return loggedin; }

    virtual void accessPermissions() = 0;
};

// ==================== CUSTOMER ==================== //

class Customer : public User {
public:
    Customer(int id, string n, string e, string p)
        : User(id, n, e, p, "customer") {
    }

    void accessPermissions() override {
        cout << "Customer can: Browse, Add to cart, Checkout, Track orders" << endl;
    }

    void browseProducts() {}
    void addToCart() {}
    void checkout() {}
    void trackOrders() {}
    void viewHistory() {}
};

// ==================== VENDOR ==================== //

class Vendor : public User {
public:
    Vendor(int id, string n, string e, string p)
        : User(id, n, e, p, "vendor") {
    }

    void accessPermissions() override {
        cout << "Vendor can: Add inventory, Track sales, Receive notifications" << endl;
    }

    void addInventory() {}
    void updateInventory() {}
    void trackSales() {}
    void receiveNotifications() {}
};

// ==================== ADMIN ==================== //

class Admin : public User {
public:
    Admin(int id, string n, string e, string p)
        : User(id, n, e, p, "admin") {
    }

    void accessPermissions() override {
        cout << "Admin can: Manage users, Add/remove products, Generate reports, Manage promotions" << endl;
    }

    void addProduct() {}
    void removeProduct() {}
    void manageUsers() {}
    void generateReports() {}
    void managePromotions() {}
};
