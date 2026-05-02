#include<iostream>
#include<string>
#include<vector>
using namespace std;

class Product {
protected:
    int productID;
    string name;
    string category;
    double price;
    int quantityAvailable;
    string description;
    double discount;

public:
    Product(int id, string n, string cat, double p, int qty, string desc, double dis = 0) {
        productID = id;
        name = n;
        category = cat;
        price = p;
        quantityAvailable = qty;
        description = desc;
        discount = dis;
    }

    void displayInfo() {
        cout << "ID: " << productID << endl;
        cout << "Name: " << name << endl;
        cout << "Category: " << category << endl;
        cout << "Price: " << price << endl;
        cout << "Quantity: " << quantityAvailable << endl;
        cout << "Description: " << description << endl;
        cout << "Discount: " << discount << "%" << endl;
        cout << "Final Price: " << calculateFinalPrice() << endl;
    }

    double calculateFinalPrice() {
        if (discount == 0) return price;
        return price - (price * discount / 100);
    }

    void updateStock(int qty) {
        quantityAvailable += qty;
        if (quantityAvailable <= 0) {
            quantityAvailable = 0;
            cout << "this product is unavailable!" << endl;
        }
        else {
            cout << "stock updated, remaining: " << quantityAvailable << endl;
        }
    }

    int getID() { return productID; }
    string getName() { return name; }
    string getCategory() { return category; }
    double getPrice() { return price; }

    virtual void displayExtraInfo() = 0;
};

// ==================== ELECTRONICS ==================== //

class Electronics : public Product {
private:
    string brand;
    int warranty;

public:
    Electronics(int id, string n, double p, int qty, string desc, string b, int w, double dis = 0)
        : Product(id, n, "Electronics", p, qty, desc, dis) {
        brand = b;
        warranty = w;
    }

    void displayExtraInfo() override {
        cout << "Brand: " << brand << endl;
        cout << "Warranty: " << warranty << " years" << endl;
    }
};

// ==================== GROCERY ==================== //

class Grocery : public Product {
private:
    string expirationDate;

public:
    Grocery(int id, string n, double p, int qty, string desc, string expDate, double dis = 0)
        : Product(id, n, "Grocery", p, qty, desc, dis) {
        expirationDate = expDate;
    }

    void displayExtraInfo() override {
        cout << "Expiration Date: " << expirationDate << endl;
    }
};

// ==================== BOOKS ==================== //

class Books : public Product {
private:
    string author;
    string publisher;

public:
    Books(int id, string n, double p, int qty, string desc, string auth, string pub, double dis = 0)
        : Product(id, n, "Books", p, qty, desc, dis) {
        author = auth;
        publisher = pub;
    }

    void displayExtraInfo() override {
        cout << "Author: " << author << endl;
        cout << "Publisher: " << publisher << endl;
    }
};

// ==================== APPLIANCES ==================== //

class Appliances : public Product {
private:
    string brand;
    string powerConsumption;

public:
    Appliances(int id, string n, double p, int qty, string desc, string b, string power, double dis = 0)
        : Product(id, n, "Appliances", p, qty, desc, dis) {
        brand = b;
        powerConsumption = power;
    }

    void displayExtraInfo() override {
        cout << "Brand: " << brand << endl;
        cout << "Power Consumption: " << powerConsumption << endl;
    }
};

// ==================== FACTORY DESIGN PATTERN ==================== //

class ProductFactory {
public:
    static Product* createProduct(string type, int id, string n, double p, int qty, string desc = "", double dis = 0) {
        if (type == "Electronics") {
            string brand;
            int warranty;
            cout << "Enter brand: ";
            cin >> brand;
            cout << "Enter warranty (years): ";
            cin >> warranty;
            return new Electronics(id, n, p, qty, desc, brand, warranty, dis);
        }
        else if (type == "Grocery") {
            string expirationDate;
            cout << "Enter expiration date: ";
            cin >> expirationDate;
            return new Grocery(id, n, p, qty, desc, expirationDate, dis);
        }
        else if (type == "Books") {
            string author, publisher;
            cout << "Enter author name: ";
            cin >> author;
            cout << "Enter publisher name: ";
            cin >> publisher;
            return new Books(id, n, p, qty, desc, author, publisher, dis);
        }
        else if (type == "Appliances") {
            string brand, powerConsumption;
            cout << "Enter brand name: ";
            cin >> brand;
            cout << "Enter power consumption: ";
            cin >> powerConsumption;
            return new Appliances(id, n, p, qty, desc, brand, powerConsumption, dis);
        }
        else {
            cout << "product type not found!" << endl;
            return nullptr;
        }
    }
};