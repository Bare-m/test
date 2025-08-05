#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CUSTOMERS 100
#define MAX_ORDERS 10

typedef struct {
    char itemName[50];
    float price;
    int paid;      // 1 = Paid, 0 = Not Paid
    int delivered; // 1 = Delivered, 0 = Not Delivered
    int inStock;
} Order;

typedef struct {
    char name[50];
    char phone[20];
    char address[100];
    Order orders[MAX_ORDERS];
    int orderCount;
} Customer;

Customer buyer[MAX_CUSTOMERS];
int buyerCount = 0;

void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Function Prototypes
void login();
void salerMenu();
void buyerMenu();
void addCustomer();
void viewCustomers();
void updateDeliveryStatus();
void searchContact();
void viewpayment();
void totalSales();
void deleteCustomer();
int findBuyerIndex(char phone[]);

// --- LOGIN ---
void login() {
    int choice;
    char password[20];

    while (1) {
        printf("\n-----------------------");
        printf("\n   >>> SYSTEM LOGIN <<<  \n");
        printf("-----------------------\n");
        printf("[1]. Saler login\n");
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
                printf("INCORRECT PASSWORD!!\n");
            }
        } else if (choice == 2) {
            buyerMenu();
        } else if (choice == 3) {
            printf("------->>Exiting<<--------\n");
            break;
        } else {
            printf("Invalid choice!\n");
        }
    }
}
// save file

void saveDatatofile() {
    FILE *fp = fopen("buyers.txt", "w");
    if (!fp) {
        printf("Error saving data!\n");
        return;
    }

    fprintf(fp, "%d\n", buyerCount); // Save total customers
    for (int i = 0; i < buyerCount; i++) {
        Customer *b = &buyer[i];
        fprintf(fp, "%s\n%s\n%s\n%d\n", b->name, b->phone, b->address, b->orderCount);

        for (int j = 0; j < b->orderCount; j++) {
            Order *o = &b->orders[j];
            fprintf(fp, "%s\n%f\n%d\n%d\n%d\n", o->itemName, o->price, o->paid, o->delivered, o->inStock);
        }
    }

    fclose(fp);
}

void loadDataFromFile() {
    FILE *fp = fopen("buyers.txt", "r");
    if (!fp) return; // First time use, no file yet

    fscanf(fp, "%d\n", &buyerCount);
    for (int i = 0; i < buyerCount; i++) {
        Customer *b = &buyer[i];
        fgets(b->name, sizeof(b->name), fp); strtok(b->name, "\n");
        fgets(b->phone, sizeof(b->phone), fp); strtok(b->phone, "\n");
        fgets(b->address, sizeof(b->address), fp); strtok(b->address, "\n");
        fscanf(fp, "%d\n", &b->orderCount);

        for (int j = 0; j < b->orderCount; j++) {
            Order *o = &b->orders[j];
            fgets(o->itemName, sizeof(o->itemName), fp); strtok(o->itemName, "\n");
            fscanf(fp, "%f\n%d\n%d\n%d\n",&o->price, &o->paid, &o->delivered, &o->inStock);
        }
    }

    fclose(fp);
}


// --- SALER FEATURES ---
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

    printf("Enter Order Info:\n");

    Order *order = &b->orders[b->orderCount];
    printf("Item type: ");
    fgets(order->itemName, sizeof(order->itemName), stdin);
    strtok(order->itemName, "\n");


    printf("total Price: ");
    scanf("%f", &order->price);
    flushInput();

    printf("Is it paid? (1 = Yes, 0 = No): ");
    scanf("%d", &order->paid);
    flushInput();

    order->delivered = 0;
    b->orderCount++;
    buyerCount++;

    printf("Buyer and order added successfully!\n");
    saveDatatofile();
}

void viewCustomers() {
    printf("\n--- Buyer List ---\n");
    for (int i = 0; i < buyerCount; i++) {
        printf("%d. %s (%s)\n", i + 1, buyer[i].name, buyer[i].phone);
    }
}

int findBuyerIndex(char phone[]) {
    for (int i = 0; i < buyerCount; i++) {
        if (strcmp(buyer[i].phone, phone) == 0)
            return i;
    }
    return -1;
}

void updateDeliveryStatus() {
    char phone[20];
    printf("Enter customer's phone number: ");
    fgets(phone, sizeof(phone), stdin);
    strtok(phone, "\n");

    int index = findBuyerIndex(phone);
    if (index == -1) {
        printf("Customer not found.\n");
        return;
    }

    Customer *b = &buyer[index];

    printf("\n--- Orders for %s ---\n", b->name);
    for (int i = 0; i < b->orderCount; i++) {
        printf("[%d] %s -$%2.f |  Delivery: %s | Payment: %s | Stock: %s\n", 
            i + 1, 
            b->orders[i].itemName,
            b->orders[i].price,
            b->orders[i].delivered ? "Delivered" : "Not Delivered",
            b->orders[i].paid ? "Paid" : "Unpaid",
            b->orders[i].inStock ? "In Stock" : "Out of Stock"
        );
    }

    int orderNum;
    printf("Enter order number to update: ");
    scanf("%d", &orderNum);
    flushInput();

    if (orderNum < 1 || orderNum > b->orderCount) {
        printf("Invalid order number.\n");
        return;
    }

    Order *o = &b->orders[orderNum - 1];

    int status;
    printf("Is the item delivered? (1 = Yes, 0 = No): ");
    scanf("%d", &status); flushInput();
    o->delivered = status;

    printf("\nIs the payment completed? (1 = Paid, 0 = Unpaid): ");
    scanf("%d", &status); flushInput();
    o->paid = status;

    printf("\nIs the item in stock? (1 = In Stock, 0 = Out of Stock): ");
    scanf("%d", &status); flushInput();
    o->inStock = status;

    printf("Order status updated successfully.\n");
    saveDatatofile();
}

void viewpayment() {
    int choice;
    char phone[20];

    printf("\n--- View Order & Payment ---\n");
    printf("Enter customer phone number: ");
    fgets(phone, sizeof(phone), stdin);
    strtok(phone, "\n");

    int index = findBuyerIndex(phone);
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
            Order *o = &b->orders[i];
            if (o->paid == 0) {
                printf("- %s ($%.2f) [UNPAID]\n", o->itemName, o->price);
                found = 1;
            }
        }
        if (!found) printf("No unpaid orders found.\n");
    } else if (choice == 2) {
        printf("\n--- Paid Orders ---\n");
        for (int i = 0; i < b->orderCount; i++) {
            Order *o = &b->orders[i];
            if (o->paid == 1) {
                printf("- %s($%.2f) [PAID]\n", o->itemName, o->price);
                found = 1;
            }
        }
        if (!found) printf("No paid orders found.\n");
    } else {
        printf("Invalid option.\n");
    }
}

void totalSales() {
    float total = 0;
    for (int i = 0; i < buyerCount; i++) {
        for (int j = 0; j < buyer[i].orderCount; j++) {
            if (buyer[i].orders[j].paid) {
                total += buyer[i].orders[j].price;
            }
        }
    }
    printf("Total sales from paid orders: $%.2f\n", total);
}

void deleteCustomer() {
    if (buyerCount == 0) {
        printf("No customers to delete.\n");
        return;
    }

    // Show customers
    printf("\n--- Customer List ---\n");
    for (int i = 0; i < buyerCount; i++) {
        printf("%d. %s (%s)\n", i + 1, buyer[i].name, buyer[i].phone);
    }

    int index;
    printf("Enter customer number to delete: ");
    scanf("%d", &index);
    flushInput();

    if (index < 1 || index > buyerCount) {
        printf("INVALID customer number.\n");
        return;
    }

    index -= 1;
    for (int i = index; i < buyerCount - 1; i++) {
        buyer[i] = buyer[i + 1];
    }
    buyerCount--;
    printf("Customer deleted from list.\n");
    saveDatatofile();
}

void salerMenu() {
    int choice;
    do {
        printf("\n----------------------");
        printf("\n>>> Seller Menu <<<\n");
        printf("----------------------\n");
        printf("[1] Add Customer & Order\n");
        printf("[2] View Customers\n");
        printf("[3] View Payment\n");
        printf("[4] View Total Sales\n");
        printf("[5] Delete Customer\n");
        printf("[6] Update Delivery Status\n");
        printf("[0] Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        flushInput();

        switch (choice) {
            case 1: addCustomer(); break;
            case 2: viewCustomers(); break;
            case 3: viewpayment(); break;
            case 4: totalSales(); break;
            case 5: deleteCustomer(); break;
            case 6: updateDeliveryStatus(); break;
            case 0: printf("Logging out...\n"); break;
            default: printf("Invalid choice!\n"); break;
        }
    } while (choice != 0);
}

// --- BUYER MENU ---
void buyerMenu() {
    char phone[20];
    printf("\nEnter your phone number: ");
    fgets(phone, sizeof(phone), stdin);
    strtok(phone, "\n");

    int index = findBuyerIndex(phone);
    if (index == -1) {
        printf("You're not registered yet.\n");
        return;
    }

    Customer *b = &buyer[index];
    int choice;
    do {
        printf("\n---------------------------");
        printf("\n>>> Buyer Menu <<<\n");
        printf("---------------------------\n");
        printf("[1] View Ongoing Orders\n");
        printf("[2] View Order History\n");
        printf("[0] Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        flushInput();

        if (choice == 1) {
            printf("\n--- Ongoing Orders ---\n");
            int found = 0;
            for (int j = 0; j < b->orderCount; j++) {
                Order *o = &b->orders[j];
                if (o->paid == 1 && o->delivered == 0) {
                    printf("- %s = $%.2f [Shipping]\n", o->itemName, o->price);
                    found = 1;
                }
            }
            if (!found) printf("No ongoing orders.\n");
        } else if (choice == 2) {
            printf("\n--- Order History ---\n");
            int found = 0;
            for (int j = 0; j < b->orderCount; j++) {
                Order *o = &b->orders[j];
                if (o->paid == 1 && o->delivered == 1) {
                    printf("- %s = $%.2f [Delivered]\n", o->itemName, o->price);
                    found = 1;
                }
            }
            if (!found) printf("No completed orders.\n");
        } else if (choice != 0) {
            printf("Invalid choice.\n");
        }
    } while (choice != 0);
}


// --- MAIN ---
int main() {
    loadDataFromFile();
    login();

    return 0;
}





