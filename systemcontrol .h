#include<iostream>
#include<string>
#include<vector>
using namespace std;

class SystemController {
private:

    SystemController() {}
    static SystemController* instance;

public:
    vector<User*> users;
    vector<Product*> products;
    vector<Order*> orders;

    static SystemController* getInstance() {
        if (instance == nullptr) {
            instance = new SystemController();
        }
        return instance;
    }

    void addUser(User* u) {
        users.push_back(u);
        cout << "user added successfully!" << endl;
    }

    void addProduct(Product* p) {
        products.push_back(p);
        cout << "product added successfully!" << endl;
    }

    void addOrder(Order* o) {
        orders.push_back(o);
    }

    void displayAllProducts() {
        if (products.empty()) {
            cout << "no products available!" << endl;
            return;
        }
        for (int i = 0; i < products.size(); i++) {
            products[i]->displayInfo();
            products[i]->displayExtraInfo();
            cout << "---" << endl;
        }
    }

    void searchByCategory(string category) {
        for (int i = 0; i < products.size(); i++) {
            if (products[i]->getCategory() == category) {
                products[i]->displayInfo();
                cout << "---" << endl;
            }
        }
    }
};

SystemController* SystemController::instance = nullptr;