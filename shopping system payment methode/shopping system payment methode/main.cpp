//
//  main.cpp
//  shopping system payment methode
//
//  Created by Mariam Abdel halim on 01/05/2026.
//

#include <iostream>
#include <vector>
#include <memory>
#include <string>
class Payment {
public:
    virtual void processPayment(double amount) const = 0 ; // abstract methode for processing
    virtual ~Payment() {} // virtual destructor
};
class CreditCardPayment : public Payment {
public:
    void processPayment( double amount) const override{
        std::cout <<"Processing Credit card payment of $"<< amount << std::endl;
    }
};
// vodafone cash
class VodafoneCashPayment : public Payment {
private:
    string phone;
    
public:
    VodafoneCashPayment(string p) : phone(p) {}
    void processPayment( double amount ) const override {
        cout << "Sending request to Vodafone Cash...\n";

        cout << "Phone: " << phone << endl;

        cout << "Amount: $" << amount << endl;
        cout << "Payment Successful!\n";
    }
};

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return EXIT_SUCCESS;
}
