#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <ctime>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <memory>

// ─────────────────────────────────────────────
//  Enums
// ─────────────────────────────────────────────

enum class OrderStatus {
    Pending,
    Processing,
    Shipped,
    OutForDelivery,
    Delivered,
    Cancelled
};

enum class PaymentStatus {
    Unpaid,
    Paid,
    Refunded,
    Failed
};

// ─────────────────────────────────────────────
//  Helper: convert enum → readable string
// ─────────────────────────────────────────────

inline std::string orderStatusToString(OrderStatus s) {
    switch (s) {
        case OrderStatus::Pending:         return "Pending";
        case OrderStatus::Processing:      return "Processing";
        case OrderStatus::Shipped:         return "Shipped";
        case OrderStatus::OutForDelivery:  return "Out for Delivery";
        case OrderStatus::Delivered:       return "Delivered";
        case OrderStatus::Cancelled:       return "Cancelled";
        default:                           return "Unknown";
    }
}

inline std::string paymentStatusToString(PaymentStatus p) {
    switch (p) {
        case PaymentStatus::Unpaid:    return "Unpaid";
        case PaymentStatus::Paid:      return "Paid";
        case PaymentStatus::Refunded:  return "Refunded";
        case PaymentStatus::Failed:    return "Failed";
        default:                       return "Unknown";
    }
}

// ─────────────────────────────────────────────
//  Forward declaration
// ─────────────────────────────────────────────
class Product;

// ─────────────────────────────────────────────
//  OrderItem
// ─────────────────────────────────────────────

class OrderItem {
private:
    std::string productID;
    std::string productName;
    int         quantity;
    double      unitPrice;
    double      subtotal;

public:
    OrderItem(const std::string& pid,
              const std::string& pname,
              int qty,
              double price)
        : productID(pid), productName(pname),
          quantity(qty), unitPrice(price),
          subtotal(qty * price) {}

    // Getters
    std::string getProductID()   const { return productID; }
    std::string getProductName() const { return productName; }
    int         getQuantity()    const { return quantity; }
    double      getUnitPrice()   const { return unitPrice; }
    double      getSubtotal()    const { return subtotal; }

    void display() const {
        std::cout << "  - " << productName
                  << " (x" << quantity << ")"
                  << "  @ $" << std::fixed << std::setprecision(2) << unitPrice
                  << "  =>  $" << subtotal << "\n";
    }
};

// ─────────────────────────────────────────────
//  StatusEvent  (for the tracking timeline)
// ─────────────────────────────────────────────

struct StatusEvent {
    OrderStatus status;
    std::string timestamp;
    std::string note;

    StatusEvent(OrderStatus s, const std::string& ts, const std::string& n)
        : status(s), timestamp(ts), note(n) {}
};

// ─────────────────────────────────────────────
//  Order
// ─────────────────────────────────────────────

class Order {
private:
    std::string              orderID;
    std::string              userID;
    std::vector<OrderItem>   items;
    double                   totalAmount;
    OrderStatus              orderStatus;
    PaymentStatus            paymentStatus;
    std::string              orderDate;
    std::string              estimatedDelivery;
    std::string              deliveryAddress;
    std::vector<StatusEvent> statusHistory;   // tracking timeline

    // Returns current time as formatted string
    static std::string currentTimestamp() {
        std::time_t now = std::time(nullptr);
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return std::string(buf);
    }

    // Estimate delivery: orderDate + 5 days (simple simulation)
    static std::string estimateDelivery() {
        std::time_t now    = std::time(nullptr);
        std::time_t future = now + 5 * 24 * 3600;
        char buf[64];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d", std::localtime(&future));
        return std::string(buf);
    }

    double calculateTotal() const {
        double total = 0;
        for (const auto& item : items)
            total += item.getSubtotal();
        return total;
    }

public:
    // Constructor
    Order(const std::string& oid,
          const std::string& uid,
          const std::string& address)
        : orderID(oid), userID(uid),
          totalAmount(0),
          orderStatus(OrderStatus::Pending),
          paymentStatus(PaymentStatus::Unpaid),
          orderDate(currentTimestamp()),
          estimatedDelivery(estimateDelivery()),
          deliveryAddress(address)
    {
        // Log initial status
        statusHistory.emplace_back(OrderStatus::Pending,
                                   orderDate,
                                   "Order placed successfully.");
    }

    // ── Item management ──────────────────────

    void addItem(const OrderItem& item) {
        if (orderStatus != OrderStatus::Pending)
            throw std::logic_error("Cannot modify a non-pending order.");
        items.push_back(item);
        totalAmount = calculateTotal();
    }

    // ── Core actions ─────────────────────────

    void placeOrder() {
        if (items.empty())
            throw std::logic_error("Cannot place an empty order.");
        if (orderStatus != OrderStatus::Pending)
            throw std::logic_error("Order already placed.");

        updateStatus(OrderStatus::Processing, "Payment confirmed. Order is being prepared.");
    }

    void cancelOrder() {
        if (orderStatus == OrderStatus::Delivered)
            throw std::logic_error("Cannot cancel a delivered order.");
        if (orderStatus == OrderStatus::Cancelled)
            throw std::logic_error("Order is already cancelled.");

        updateStatus(OrderStatus::Cancelled, "Order cancelled by customer.");
        paymentStatus = PaymentStatus::Refunded;
    }

    // ── Status management ────────────────────

    void updateStatus(OrderStatus newStatus, const std::string& note = "") {
        orderStatus = newStatus;
        statusHistory.emplace_back(newStatus, currentTimestamp(), note);
        std::cout << "[Order " << orderID << "] Status updated to: "
                  << orderStatusToString(newStatus) << "\n";
    }

    void markPaymentPaid() {
        paymentStatus = PaymentStatus::Paid;
    }

    // ── Tracking ─────────────────────────────

    void trackOrder() const {
        std::cout << "\n╔══════════════════════════════════════════╗\n";
        std::cout << "  ORDER TRACKING — " << orderID << "\n";
        std::cout << "╚══════════════════════════════════════════╝\n";
        std::cout << "  Customer ID      : " << userID << "\n";
        std::cout << "  Order date       : " << orderDate << "\n";
        std::cout << "  Est. delivery    : " << estimatedDelivery << "\n";
        std::cout << "  Delivery address : " << deliveryAddress << "\n";
        std::cout << "  Payment          : " << paymentStatusToString(paymentStatus) << "\n";
        std::cout << "  Current status   : " << orderStatusToString(orderStatus) << "\n\n";

        std::cout << "  ── Timeline ────────────────────────────\n";
        for (size_t i = 0; i < statusHistory.size(); ++i) {
            const auto& e = statusHistory[i];
            // Mark current step with arrow
            std::string marker = (i == statusHistory.size() - 1) ? "  >>>" : "     ";
            std::cout << marker << " [" << e.timestamp << "]  "
                      << orderStatusToString(e.status) << "\n";
            if (!e.note.empty())
                std::cout << "          " << e.note << "\n";
        }
        std::cout << "\n";
    }

    // ── Display ──────────────────────────────

    void displaySummary() const {
        std::cout << "\n──────────────────────────────────────────\n";
        std::cout << "  Order ID  : " << orderID << "\n";
        std::cout << "  Status    : " << orderStatusToString(orderStatus) << "\n";
        std::cout << "  Items:\n";
        for (const auto& item : items)
            item.display();
        std::cout << "  Total     : $" << std::fixed << std::setprecision(2)
                  << totalAmount << "\n";
        std::cout << "──────────────────────────────────────────\n";
    }

    // ── Getters ──────────────────────────────

    std::string   getOrderID()       const { return orderID; }
    std::string   getUserID()        const { return userID; }
    OrderStatus   getOrderStatus()   const { return orderStatus; }
    PaymentStatus getPaymentStatus() const { return paymentStatus; }
    double        getTotalAmount()   const { return totalAmount; }
    std::string   getOrderDate()     const { return orderDate; }

    const std::vector<OrderItem>&   getItems()         const { return items; }
    const std::vector<StatusEvent>& getStatusHistory() const { return statusHistory; }
};


// ================================================================
//  CLASS: OrderHistory
//  Records and displays all past orders belonging to one user.
//
//  EDUCATIONAL NOTE: WHY A SEPARATE CLASS?
//  The Order class is responsible for ONE order's data and actions.
//  OrderHistory is responsible for COLLECTING many orders per user.
//  This follows the Single Responsibility Principle — each class
//  does one job. Mixing history into Order would bloat it.
//
//  EDUCATIONAL NOTE: STL containers used here
//  - vector<shared_ptr<Order>>: a dynamic list of smart pointers.
//    shared_ptr means OrderHistory doesn't OWN the orders — it just
//    holds a reference. The orders can still live elsewhere (e.g.
//    in a system-wide map) without being duplicated in memory.
//
//  EDUCATIONAL NOTE: ENCAPSULATION
//  The orders vector is private. The only way to add or read orders
//  is through the public methods below. This protects the internal
//  list from being modified accidentally from outside the class.
// ================================================================

class OrderHistory {
private:
    std::string                             userID;
    std::vector<std::shared_ptr<Order>>     orders;   // STL: vector of smart pointers

public:
    // Constructor: tied to one specific user
    explicit OrderHistory(const std::string& uid) : userID(uid) {}

    // ── addOrder() ───────────────────────────────────────────────
    // Called whenever a user successfully places a new order.
    // We store a shared_ptr so we share the same Order object —
    // no copying, no wasted memory.
    void addOrder(std::shared_ptr<Order> order) {
        if (order->getUserID() != userID)
            throw std::logic_error("Order does not belong to this user.");
        orders.push_back(order);
    }

    // ── viewHistory() ────────────────────────────────────────────
    // Prints a summary of every order the user has ever placed.
    // Uses a range-based for loop (modern C++) to iterate the vector.
    void viewHistory() const {
        std::cout << "\n╔══════════════════════════════════════════╗\n";
        std::cout << "  ORDER HISTORY — User: " << userID << "\n";
        std::cout << "╚══════════════════════════════════════════╝\n";

        if (orders.empty()) {
            std::cout << "  No orders found.\n\n";
            return;
        }

        // EDUCATIONAL NOTE: range-based for loop
        // 'const auto&' means: don't copy, don't modify — just read.
        // 'auto' lets the compiler figure out the type automatically.
        for (const auto& order : orders)
            order->displaySummary();

        std::cout << "  Total orders: " << orders.size() << "\n\n";
    }

    // ── getOrderByID() ───────────────────────────────────────────
    // Searches the history for a specific order by its ID.
    // Returns a shared_ptr (nullptr if not found).
    //
    // EDUCATIONAL NOTE: EXCEPTION HANDLING
    // If the order isn't found we throw std::runtime_error.
    // The caller (e.g. the menu) catches it and shows a message
    // instead of the program crashing.
    std::shared_ptr<Order> getOrderByID(const std::string& orderID) const {
        for (const auto& order : orders) {
            if (order->getOrderID() == orderID)
                return order;   // found — return the smart pointer
        }
        // Not found — throw an exception (spec requires exception handling)
        throw std::runtime_error("Order not found in history: " + orderID);
    }

    // ── trackOrderByID() ─────────────────────────────────────────
    // Convenience: find an order and immediately show its tracking.
    // Combines getOrderByID() + trackOrder() in one call.
    void trackOrderByID(const std::string& orderID) const {
        // getOrderByID throws if not found — no need to check manually
        getOrderByID(orderID)->trackOrder();
    }

    // ── filterByStatus() ─────────────────────────────────────────
    // Returns only orders matching a given status.
    // Useful for showing e.g. only "Delivered" or "Cancelled" orders.
    //
    // EDUCATIONAL NOTE: STL vector usage
    // We build a NEW vector containing only matching orders.
    // This doesn't modify the original 'orders' list.
    std::vector<std::shared_ptr<Order>> filterByStatus(OrderStatus status) const {
        std::vector<std::shared_ptr<Order>> result;
        for (const auto& order : orders) {
            if (order->getOrderStatus() == status)
                result.push_back(order);
        }
        return result;
    }

    // ── getTotalSpent() ──────────────────────────────────────────
    // Sums the total amount across ALL orders for this user.
    double getTotalSpent() const {
        double total = 0.0;
        for (const auto& order : orders)
            total += order->getTotalAmount();
        return total;
    }

    // ── Getters ──────────────────────────────────────────────────
    std::string getUserID()    const { return userID; }
    int         getOrderCount()const { return static_cast<int>(orders.size()); }
};
