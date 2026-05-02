//  main.cpp
//  ShoppingSystemPaymentMethod & promocode
//
//  Created by Mariam Abdel halim on 02/05/2026.
//

#include <iostream>
#include <vector>
#include <memory>
#include <string>
using namespace std ;
class Payment {
public:
    virtual void processPayment(double amount) const = 0 ; // abstract methode for processing
    virtual ~Payment() {} // virtual destructor
};
//credit card
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
// Fawry
class FawryPayment : public Payment{
public:
    FawryPayment() {}
    void processPayment(double amount) const override{
        cout<< " Fawry Payment Selected \n";
        cout<< " ur code is : 193456\n";
        cout<< " pay$$"<< amount << " using this code.\n";
    }
};
class Order {
public:
    double totalAmount;
    
    Order(double amount){
        totalAmount = amount;
    }
};
class PaymentProccessor {
public:
    void pay ( Payment* method, Order order){
        method->processPayment(order .totalAmount);
    }
};
class PromoCode {
public:
    static double applyDiscount( double amount , string code ){
        
        if ( code == "Mariamabdlhlm10") {
            double discount = amount * 0.10 ;
            return  amount - discount;
        }
        else if ( code == "Mariamabdlhlm20"){
            double discount = amount * 0.20 ;
            return amount - discount;
        }
        return amount ;
    }
};
int main() {
    double amount ;
    string code ;
    int choice ;
    
    cout << " enter total amount : ";
    cin >> amount;
    
    cout << " Enter promocode (or None ):";
    cin >> code;
    // applying promocode
    amount = PromoCode::applyDiscount(amount,code);
    
    cout<< " After discount: "<< amount << endl ;
    //crreate order
    Order order( amount);
    
    cout << " choose payment methode:";
    cout << "1. creditcard" ;
    cout << " 2. vodafonecash";
    cout << " 3. fawry";
    cin >> choice ;
    Payment* payment = nullptr ;
    PaymentProccessor processor ;
    if (choice == 1) {
        payment = new CreditCardPayment() ;
    }
    else if (choice == 2){
        string phone ;
        payment = new VodafoneCashPayment(phone);
    }
    else if (choice == 3 ){
        payment = new FawryPayment();
    }
    else{
        cout << " invalid choice" ;
        return 0 ;
    }
    processor.pay(payment, order);
    
    delete payment ;
    return 0 ;
}

