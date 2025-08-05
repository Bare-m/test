#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CUSTOMERS 100
#define MAX_ORDERS 10

// Structs
struct Order {
    char itemName[50];
    int quantity;
    float price;
    int paid;
    int delivered;
};

typedef struct {
    char name[50];
    char phone[20];
    char address[100];
    struct Order orders[MAX_ORDERS];
    int orderCount;
} Customer;

// Global variables
Customer buyer[MAX_CUSTOMERS];
int buyerCount = 0;

// Function prototypes
void flushInput();
void login();
void salerMenu();
void buyerMenu(char phone[]);

void addCustomer();
void viewCustomers();
void updateStatus();
int searchContact(char phone[]);
void viewpayment();
void totalSales();
void deleteCustomer();

// Main
int main() {
    login();
    return 0;
}

// Utility
void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Login function
void login() {
    int choice;
    char password[20];
    char phone[20];

    while (1) {
        printf("\n-----------------------");
        printf("\n   >>>SYSTEM LOGIN<<<  \n");
        printf("-----------------------\n");
        printf("[1]. Seller login\n");
        printf("[2]. Buyer login\n");
        printf("[3]. Exit\n");
        printf("Choose your option: ");
        scanf("%d", &choice);
        flushInput();

        if (choice == 1) {
            printf("Enter your password: ");
            scanf("%s", password);
            flushInput();
            if (strcmp(password, "admin123") == 0) {
                salerMenu();
            } else {
                printf("INCORRECT PASSWORD!\n");
            }
        } else if (choice == 2) {
            printf("Enter your phone number: ");
            fgets(phone, sizeof(phone), stdin);
            strtok(phone, "\n");
            buyerMenu(phone);
        } else if (choice == 3) {
            printf("------->>Exiting<<--------\n");
            break;
        } else {
            printf("Invalid choice!\n");
        }
    }
}

// Add Customer
void addCustomer() {
    if (buyerCount >= MAX_CUSTOMERS) {
        printf("Buyer limit reached.\n");
        return;
    }

    Customer *b = &buyer[buyerCount];

    printf("Name: ");
    fgets(b->name, sizeof(b->name), stdin);
    strtok(b->name, "\n");

    printf("Phone: ");
    fgets(b->phone, sizeof(b->phone), stdin);
    strtok(b->phone, "\n");

    printf("Address: ");
    fgets(b->address, sizeof(b->address), stdin);
    strtok(b->address, "\n");

    b->orderCount = 0;

    struct Order *order = &b->orders[b->orderCount];
    printf("Item name: ");
    fgets(order->itemName, sizeof(order->itemName), stdin);
    strtok(order->itemName, "\n");

    printf("Quantity: ");
    scanf("%d", &order->quantity);
    flushInput();

    printf("Price per item: ");
    scanf("%f", &order->price);
    flushInput();

    printf("Is it paid? (1 = Yes, 0 = No): ");
    scanf("%d", &order->paid);
    flushInput();

    printf("Is it delivered? (1 = Yes, 0 = No): ");
    scanf("%d", &order->delivered);
    flushInput();

    b->orderCount++;
    buyerCount++;

    printf("Buyer and order added successfully!\n");
}

// View customers
void viewCustomers() {
    printf("\n--- Buyer List ---\n");
    for (int i = 0; i < buyerCount; i++) {
        printf("%d. %s (%s)\n", i + 1, buyer[i].name, buyer[i].phone);
    }
}

// Search buyer by phone
int searchContact(char phone[]) {
    for (int i = 0; i < buyerCount; i++) {
        if (strcmp(buyer[i].phone, phone) == 0)
            return i;
    }
    return -1;
}

// View payment
void viewpayment() {
    int choice;
    char phone[20];

    printf("\n--- View Order & Payment ---\n");
    printf("Enter customer phone number: ");
    fgets(phone, sizeof(phone), stdin);
    strtok(phone, "\n");

    int index = searchContact(phone);
    if (index == -1) {
        printf("Customer not found.\n");
        return;
    }

    printf("\n[1] View Unpaid Orders\n[2] View Paid Orders\nChoose an option: ");
    scanf("%d", &choice);
    flushInput();

    Customer *b = &buyer[index];
    int found = 0;

    if (choice == 1) {
        printf("\n--- Unpaid Orders ---\n");
        for (int i = 0; i < b->orderCount; i++) {
            struct Order *o = &b->orders[i];
            if (o->paid == 0) {
                printf("- %s x%d ($%.2f) [UNPAID]\n", o->itemName, o->quantity, o->quantity * o->price);
                found = 1;
            }
        }
        if (!found) printf("No unpaid orders found.\n");
    } else if (choice == 2) {
        printf("\n--- Paid Orders ---\n");
        for (int i = 0; i < b->orderCount; i++) {
            struct Order *o = &b->orders[i];
            if (o->paid == 1) {
                printf("- %s x%d ($%.2f) [PAID]\n", o->itemName, o->quantity, o->quantity * o->price);
                found = 1;
            }
        }
        if (!found) printf("No paid orders found.\n");
    } else {
        printf("Invalid option.\n");
    }
}

// Total sales
void totalSales() {
    float total = 0;
    for (int i = 0; i < buyerCount; i++) {
        for (int j = 0; j < buyer[i].orderCount; j++) {
            if (buyer[i].orders[j].paid) {
                total += buyer[i].orders[j].quantity * buyer[i].orders[j].price;
            }
        }
    }
    printf("Total sales from paid orders: $%.2f\n", total);
}

// Delete customer
void deleteCustomer() {
    int index;
    printf("Enter contact number to delete: ");
    scanf("%d", &index);
    flushInput();

    if (index < 1 || index > buyerCount) {
        printf("INVALID contact number.\n");
        return;
    }

    index -= 1;
    for (int i = index; i < buyerCount - 1; i++) {
        buyer[i] = buyer[i + 1];
    }

    buyerCount--;
    printf("Customer deleted from list.\n");
}

// Seller menu
void salerMenu() {
    int choice;
    do {
        printf("\n----------------------");
        printf("\n>>> Seller Menu <<<\n");
        printf("-----------------------\n");
        printf("[1]. Add Customer & Order\n");
        printf("[2]. View Customers\n");
        printf("[3]. View Payment\n");
        printf("[4]. View Total Sales\n");
        printf("[5]. Delete Customer\n");
        printf("[0]. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        flushInput();

        switch (choice) {
            case 1: addCustomer(); break;
            case 2: viewCustomers(); break;
            case 3: viewpayment(); break;
            case 4: totalSales(); break;
            case 5: deleteCustomer(); break;
            case 0: printf("Logging out...\n"); break;
            default: printf("Invalid choice!\n"); break;
        }
    } while (choice != 0);
}

// Buyer menu
void buyerMenu(char phone[]) {
    int index = searchContact(phone);
    if (index == -1) {
        printf("You're not registered yet.\n");
        return;
    }

    Customer *b = &buyer[index];
    int choice;

    printf("Welcome, %s!\n", b->name);

    do {
        printf("\n---------------------------");
        printf("\n >>> Buyer Menu <<< ");
        printf("\n---------------------------\n");
        printf("[1]. View Ongoing Orders\n");
        printf("[2]. View Order History\n");
        printf("[0]. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        flushInput();

        if (choice == 1) {
            printf("\n--- Ongoing Orders ---\n");
            int found = 0;
            for (int j = 0; j < b->orderCount; j++) {
                struct Order *o = &b->orders[j];
                if (o->paid == 1 && o->delivered == 0) {
                    printf("- %s x%d = $%.2f [Shipping]\n", o->itemName, o->quantity, o->quantity * o->price);
                    found = 1;
                }
            }
            if (!found) printf("No ongoing orders.\n");
        } else if (choice == 2) {
            printf("\n--- Order History ---\n");
            int found = 0;
            for (int j = 0; j < b->orderCount; j++) {
                struct Order *o = &b->orders[j];
                if (o->paid == 1 && o->delivered == 1) {
                    printf("- %s x%d = $%.2f [Delivered]\n", o->itemName, o->quantity, o->quantity * o->price);
                    found = 1;
                }
            }
            if (!found) printf("No completed orders.\n");
        } else if (choice != 0) {
            printf("Invalid choice.\n");
        }

    } while (choice != 0);
}
