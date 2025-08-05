#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CUSTOMERS 100
#define NAME_LEN 50
#define PHONE_LEN 20
#define ADDR_LEN 100
#define PASS "pass123"

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char address[ADDR_LEN];
    float total_order;
    float total_payment;
    int orders;
    int buyer_id; // for history
} Customer;

Customer customers[MAX_CUSTOMERS];
int customer_count = 0;

// Utility functions
void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int find_customer_by_phone(const char *phone) {
    for (int i = 0; i < customer_count; i++) {
        if (strstr(customers[i].phone, phone)) {
            return i;
        }
    }
    return -1;
}

int cmp_name(const void *a, const void *b) {
    return strcmp(((Customer*)a)->name, ((Customer*)b)->name);
}

// Seller functions
void add_customer() {
    if (customer_count >= MAX_CUSTOMERS) {
        printf("Customer list full!\n");
        return;
    }
    printf("Enter name: ");
    fgets(customers[customer_count].name, NAME_LEN, stdin);
    customers[customer_count].name[strcspn(customers[customer_count].name, "\n")] = 0;

    printf("Enter phone: ");
    fgets(customers[customer_count].phone, PHONE_LEN, stdin);
    customers[customer_count].phone[strcspn(customers[customer_count].phone, "\n")] = 0;

    printf("Enter address: ");
    fgets(customers[customer_count].address, ADDR_LEN, stdin);
    customers[customer_count].address[strcspn(customers[customer_count].address, "\n")] = 0;

    customers[customer_count].orders = 0;
    customers[customer_count].total_order = 0;
    customers[customer_count].total_payment = 0;
    customers[customer_count].buyer_id = customer_count + 1;
    customer_count++;
    printf("Customer added.\n");
}

void update_customer() {
    char phone[PHONE_LEN];
    printf("Enter phone number to update: ");
    fgets(phone, PHONE_LEN, stdin);
    phone[strcspn(phone, "\n")] = 0;

    int idx = find_customer_by_phone(phone);
    if (idx == -1) {
        printf("Customer not found.\n");
        return;
    }

    printf("Enter new name (leave blank to keep current): ");
    char buf[NAME_LEN];
    fgets(buf, NAME_LEN, stdin);
    buf[strcspn(buf, "\n")] = 0;
    if (strlen(buf) > 0) strcpy(customers[idx].name, buf);

    printf("Enter new address (leave blank to keep current): ");
    fgets(buf, ADDR_LEN, stdin);
    buf[strcspn(buf, "\n")] = 0;
    if (strlen(buf) > 0) strcpy(customers[idx].address, buf);

    printf("Customer updated.\n");
}

void view_customers_AZ() {
    if (customer_count == 0) {
        printf("No customers to display.\n");
        return;
    }
    Customer temp[MAX_CUSTOMERS];
    memcpy(temp, customers, sizeof(Customer) * customer_count);
    qsort(temp, customer_count, sizeof(Customer), cmp_name);
    printf("Customer List (A-Z):\n");
    for (int i = 0; i < customer_count; i++) {
        printf("%d. %s | %s | %s\n", i+1, temp[i].name, temp[i].phone, temp[i].address);
    }
}

void search_customer() {
    char phone[PHONE_LEN];
    printf("Enter at least 4 digits of phone: ");
    fgets(phone, PHONE_LEN, stdin);
    phone[strcspn(phone, "\n")] = 0;

    int found = 0;
    for (int i = 0; i < customer_count; i++) {
        if (strstr(customers[i].phone, phone)) {
            printf("%d. %s | %s | %s\n", i+1, customers[i].name, customers[i].phone, customers[i].address);
            found = 1;
        }
    }
    if (!found) printf("No customer found matching phone digits.\n");
}

void delete_customer() {
    char phone[PHONE_LEN];
    printf("Enter phone number to delete: ");
    fgets(phone, PHONE_LEN, stdin);
    phone[strcspn(phone, "\n")] = 0;

    int idx = find_customer_by_phone(phone);
    if (idx == -1) {
        printf("Customer not found.\n");
        return;
    }
    for (int i = idx; i < customer_count - 1; i++) {
        customers[i] = customers[i+1];
    }
    customer_count--;
    printf("Customer deleted.\n");
}

void view_order_payment() {
    int choice;
    printf("1. View Orders\n2. View Payments\nChoose: ");
    scanf("%d", &choice); clearInput();
    if (choice == 1) {
        for (int i = 0; i < customer_count; i++) {
            printf("%s has %d orders, total: %.2f\n",
                   customers[i].name, customers[i].orders, customers[i].total_order);
        }
    } else if (choice == 2) {
        for (int i = 0; i < customer_count; i++) {
            printf("%s paid: %.2f\n", customers[i].name, customers[i].total_payment);
        }
    } else {
        printf("Invalid option.\n");
    }
}

void show_total_sales() {
    float total = 0;
    for (int i = 0; i < customer_count; i++) {
        total += customers[i].total_order;
    }
    printf("Total sales: %.2f\n", total);
}

// Buyer functions
void buyer_view_stuff(int idx) {
    printf("Name: %s\nPhone: %s\nAddress: %s\n", customers[idx].name, customers[idx].phone, customers[idx].address);
    printf("Orders: %d\nTotal Order Amount: %.2f\nTotal Payment: %.2f\n", customers[idx].orders, customers[idx].total_order, customers[idx].total_payment);
}

void buyer_history(int idx) {
    printf("Order history for %s:\n", customers[idx].name);
    printf("Orders made: %d\nTotal spent: %.2f\n", customers[idx].orders, customers[idx].total_order);
}

void buyer_ongoing(int idx) {
    printf("Ongoing orders for %s: %d\n", customers[idx].name, customers[idx].orders);
}

int buyer_login() {
    char phone[PHONE_LEN];
    printf("Enter your phone number: ");
    fgets(phone, PHONE_LEN, stdin);
    phone[strcspn(phone, "\n")] = 0;

    int idx = find_customer_by_phone(phone);
    if (idx == -1) {
        printf("Buyer not found. Please contact seller to add you.\n");
        return -1;
    }
    return idx;
}

// Main menu
void seller_menu() {
    while (1) {
        printf("\n[Seller Menu]\n");
        printf("1. Add customer\n2. Update customer\n3. View customers (A-Z)\n4. Search customer\n5. Delete customer\n");
        printf("6. View order & payment\n7. Show total sales\n8. Log out\nChoose: ");
        int ch;
        scanf("%d", &ch); clearInput();
        switch (ch) {
            case 1: add_customer(); break;
            case 2: update_customer(); break;
            case 3: view_customers_AZ(); break;
            case 4: search_customer(); break;
            case 5: delete_customer(); break;
            case 6: view_order_payment(); break;
            case 7: show_total_sales(); break;
            case 8: return;
            default: printf("Invalid!\n");
        }
    }
}

void buyer_menu(int idx) {
    while (1) {
        printf("\n[Buyer Menu]\n");
        printf("1. View stuff\n2. History buying\n3. Ongoing\n4. Log out\nChoose: ");
        int ch;
        scanf("%d", &ch); clearInput();
        switch (ch) {
            case 1: buyer_view_stuff(idx); break;
            case 2: buyer_history(idx); break;
            case 3: buyer_ongoing(idx); break;
            case 4: return;
            default: printf("Invalid!\n");
        }
    }
}

int main() {
    int main_choice;
    while (1) {
        printf("\n[Main Menu]\n1. Seller (login required)\n2. Buyer\n3. Exit\nChoose: ");
        scanf("%d", &main_choice); clearInput();
        if (main_choice == 1) {
            char pass[20];
            printf("Enter seller password: ");
            fgets(pass, 20, stdin);
            pass[strcspn(pass, "\n")] = 0;
            if (strcmp(pass, PASS) == 0) {
                seller_menu();
            } else {
                printf("Wrong password!\n");
            }
        } else if (main_choice == 2) {
            int idx = buyer_login();
            if (idx != -1) {
                buyer_menu(idx);
            }
        } else if (main_choice == 3) {
            printf("Exiting system. Goodbye!\n");
            break;
        } else {
            printf("Invalid option!\n");
        }
    }
    return 0;
}