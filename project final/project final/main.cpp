#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Forward declarations
class Product;
class Order;
class Customer;
class Seller;

// Product class
class Product {
public:
    string name;
    string description;
    double price;
    int stock;

    Product(const string& name, const string& description, double price, int stock)
        : name(name), description(description), price(price), stock(stock) {}

    void displayProductDetails() const {
        cout << "Product Name: " << name << endl;
        cout << "Description: " << description << endl;
        cout << "Price: $" << price << endl;
        cout << "Stock: " << stock << endl;
    }

    void updateStock(int amount) {
        stock += amount;
    }
};

// Order class
class Order {
public:
    Customer* customer;
    vector<Product*> products;
    double totalPrice;
    string status;

    Order(Customer* customer) : customer(customer), totalPrice(0), status("Pending") {}

    void calculateTotalPrice() {
        totalPrice = 0;
        for (const auto& product : products) {
            totalPrice += product->price;
        }
    }

    void updateStatus(const string& newStatus) {
        status = newStatus;
    }
};

// Base User class
class User {
protected:
    string name;
    string email;

public:
    User(const string& name, const string& email) : name(name), email(email) {}

    string getName() const { return name; }
    string getEmail() const { return email; }

    virtual void viewProfile() const {
        cout << "Name: " << name << endl;
        cout << "Email: " << email << endl;
    }

    virtual ~User() {}
};

// Seller class
class Seller : public User {
private:
    vector<Product*> products;

public:
    Seller(const string& name, const string& email) : User(name, email) {}

    void addProduct(Product* product) {
        products.push_back(product);
        cout << "Product " << product->name << " added successfully.\n";
    }

    void updateProduct(const string& productName, double newPrice, int newStock) {
        for (auto& product : products) {
            if (product->name == productName) {
                product->price = newPrice;
                product->stock = newStock;
                cout << "Product " << product->name << " updated successfully.\n";
                return;
            }
        }
        cout << "Product not found.\n";
    }

    void listProducts() const {
        if (products.empty()) {
            cout << "No products available.\n";
            return;
        }
        cout << "\nProducts Sold by " << name << ":\n";
        for (const auto& product : products) {
            product->displayProductDetails();
        }
    }
};

// Customer class
class Customer : public User {
private:
    vector<Product*> cart;
    vector<Order> orders;

public:
    Customer(const string& name, const string& email) : User(name, email) {}

    void browseProducts(const vector<Product*>& products) {
        cout << "\nAvailable Products:\n";
        for (const auto& product : products) {
            product->displayProductDetails();
        }
    }

    void addToCart(Product* product) {
        if (product->stock > 0) {
            cart.push_back(product);
            cout << product->name << " added to cart.\n";
        } else {
            cout << "Product out of stock.\n";
        }
    }

    void placeOrder() {
        if (cart.empty()) {
            cout << "Cart is empty.\n";
            return;
        }
        Order newOrder(this);
        newOrder.products = cart;
        newOrder.calculateTotalPrice();
        for (auto& product : cart) {
            product->updateStock(-1);
        }
        orders.push_back(newOrder);
        cart.clear();
        cout << "Order placed successfully. Total: $" << newOrder.totalPrice << endl;
    }

    void viewOrders() const {
        if (orders.empty()) {
            cout << "No orders placed yet.\n";
            return;
        }
        cout << "Order History:\n";
        for (const auto& order : orders) {
            cout << "- Total: $" << order.totalPrice << ", Status: " << order.status << endl;
        }
    }
};

// Admin class
class Admin : public User {
private:
    vector<Seller*> sellers;
    vector<Product*> products;

public:
    Admin(const string& name, const string& email) : User(name, email) {}

    void addSeller(Seller* seller) {
        sellers.push_back(seller);
        cout << "Seller " << seller->getName() << " added successfully.\n";
    }

    void listSellers() const {
        if (sellers.empty()) {
            cout << "No sellers available.\n";
            return;
        }
        cout << "\nSellers on the Platform:\n";
        for (const auto& seller : sellers) {
            cout << "Name: " << seller->getName() << ", Email: " << seller->getEmail() << endl;
        }
    }
};

// Menu Functions
void adminMenu(Admin& admin) {
    int choice;
    do {
        cout << "\nAdmin Menu:\n";
        cout << "1. Add Seller\n2. List Sellers\n3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: {
            string name, email;
            cout << "Enter seller name: ";
            cin >> name;
            cout << "Enter seller email: ";
            cin >> email;
            admin.addSeller(new Seller(name, email));
            break;
        }
        case 2:
            admin.listSellers();
            break;
        case 3:
            cout << "Exiting Admin Menu...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);
}

void customerMenu(Customer& customer, const vector<Product*>& products) {
    int choice;
    do {
        cout << "\nCustomer Menu:\n";
        cout << "1. Browse Products\n2. Add Product to Cart\n3. Place Order\n4. View Orders\n5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            customer.browseProducts(products);
            break;
        case 2: {
            string productName;
            cout << "Enter product name to add to cart: ";
            cin >> productName;
            bool found = false;
            for (auto& product : products) {
                if (product->name == productName) {
                    customer.addToCart(product);
                    found = true;
                    break;
                }
            }
            if (!found) {
                cout << "Product not found.\n";
            }
            break;
        }
        case 3:
            customer.placeOrder();
            break;
        case 4:
            customer.viewOrders();
            break;
        case 5:
            cout << "Exiting Customer Menu...\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 5);
}

int main() {
    Admin admin("Admin", "admin@example.com");
    vector<Product*> globalProducts = {
        new Product("Laptop", "High performance laptop", 1000.0, 10),
        new Product("Smartphone", "Latest smartphone", 800.0, 20)
    };

    Customer customer("John Doe", "john@example.com");

    int mainChoice;
    do {
        cout << "\nMain Menu:\n";
        cout << "1. Admin Menu\n2. Customer Menu\n3. Exit\n";
        cout << "Enter your choice: ";
        cin >> mainChoice;

        switch (mainChoice) {
        case 1:
            adminMenu(admin);
            break;
        case 2:
            customerMenu(customer, globalProducts);
            break;
        case 3:
            cout << "Exiting the system. Goodbye!\n";
            break;
        default:
            cout << "Invalid choice. Try again.\n";
        }
    } while (mainChoice != 3);

    // Clean up dynamically allocated memory
    for (auto& product : globalProducts) {
        delete product;
    }

    return 0;
}

