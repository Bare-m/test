/*
 * Contact Management System in C
 * Features: Add, Display, Search, Edit, Delete, Sort, File Save/Load,
 * Input Validation, Login, Backup/Restore, Dynamic Memory, Linked List, CSV Format
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "contacts.csv"
#define BACKUP_FILENAME "backup.csv"

typedef struct Contact {
    char name[100];
    char phone[15];
    char email[100];
    struct Contact* next;
} Contact;

Contact* head = NULL;

// Function declarations
int login();
int is_valid_email(const char* email);
int is_valid_phone(const char* phone);
void add_contact();
void display_contacts();
void search_contact();
void edit_contact();
void delete_contact();
void sort_contacts();
void save_to_file();
void load_from_file();
void backup_contacts();
void restore_contacts();
void free_contacts();

int main() {
    if (!login()) {
        printf("Login failed. Exiting...\n");
        return 1;
    }

    load_from_file();

    int choice;
    do {
        printf("\n--- Contact Management System ---\n");
        printf("1. Add Contact\n2. Display Contacts\n3. Search Contact\n4. Edit Contact\n5. Delete Contact\n6. Sort Contacts\n7. Backup Contacts\n8. Restore Contacts\n9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1: add_contact(); break;
            case 2: display_contacts(); break;
            case 3: search_contact(); break;
            case 4: edit_contact(); break;
            case 5: delete_contact(); break;
            case 6: sort_contacts(); break;
            case 7: backup_contacts(); break;
            case 8: restore_contacts(); break;
            case 9: save_to_file(); free_contacts(); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 9);

    return 0;
}

int login() {
    char user[50], pass[50];
    printf("Username: ");
    scanf("%s", user);
    printf("Password: ");
    scanf("%s", pass);
    return strcmp(user, "admin") == 0 && strcmp(pass, "1234") == 0;
}

int is_valid_email(const char* email) {
    return strchr(email, '@') && strchr(email, '.');
}

int is_valid_phone(const char* phone) {
    for (int i = 0; phone[i]; i++) {
        if (!isdigit(phone[i])) return 0;
    }
    return strlen(phone) >= 8 && strlen(phone) <= 14;
}

void add_contact() {
    Contact* new_node = (Contact*)malloc(sizeof(Contact));
    if (!new_node) return;

    printf("Enter name: ");
    fgets(new_node->name, sizeof(new_node->name), stdin);
    new_node->name[strcspn(new_node->name, "\n")] = 0;

    do {
        printf("Enter phone: ");
        fgets(new_node->phone, sizeof(new_node->phone), stdin);
        new_node->phone[strcspn(new_node->phone, "\n")] = 0;
    } while (!is_valid_phone(new_node->phone));

    do {
        printf("Enter email: ");
        fgets(new_node->email, sizeof(new_node->email), stdin);
        new_node->email[strcspn(new_node->email, "\n")] = 0;
    } while (!is_valid_email(new_node->email));

    new_node->next = head;
    head = new_node;
    printf("Contact added.\n");
}

void display_contacts() {
    Contact* temp = head;
    printf("\nName\t\tPhone\t\tEmail\n");
    while (temp) {
        printf("%s\t%s\t%s\n", temp->name, temp->phone, temp->email);
        temp = temp->next;
    }
}

void search_contact() {
    char keyword[100];
    printf("Enter name or phone to search: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    Contact* temp = head;
    while (temp) {
        if (strstr(temp->name, keyword) || strstr(temp->phone, keyword)) {
            printf("Found: %s | %s | %s\n", temp->name, temp->phone, temp->email);
        }
        temp = temp->next;
    }
}

void edit_contact() {
    char keyword[100];
    printf("Enter name to edit: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    Contact* temp = head;
    while (temp) {
        if (strcmp(temp->name, keyword) == 0) {
            printf("Enter new phone: ");
            fgets(temp->phone, sizeof(temp->phone), stdin);
            temp->phone[strcspn(temp->phone, "\n")] = 0;

            printf("Enter new email: ");
            fgets(temp->email, sizeof(temp->email), stdin);
            temp->email[strcspn(temp->email, "\n")] = 0;
            printf("Contact updated.\n");
            return;
        }
        temp = temp->next;
    }
    printf("Contact not found.\n");
}

void delete_contact() {
    char keyword[100];
    printf("Enter name to delete: ");
    fgets(keyword, sizeof(keyword), stdin);
    keyword[strcspn(keyword, "\n")] = 0;

    Contact *temp = head, *prev = NULL;
    while (temp) {
        if (strcmp(temp->name, keyword) == 0) {
            if (prev) prev->next = temp->next;
            else head = temp->next;
            free(temp);
            printf("Contact deleted.\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    }
    printf("Contact not found.\n");
}

void sort_contacts() {
    if (!head) return;
    for (Contact* i = head; i; i = i->next) {
        for (Contact* j = i->next; j; j = j->next) {
            if (strcmp(i->name, j->name) > 0) {
                Contact temp = *i;
                *i = *j;
                *j = temp;

                Contact* tmp = i->next;
                i->next = j->next;
                j->next = tmp;
            }
        }
    }
    printf("Contacts sorted alphabetically.\n");
}

void save_to_file() {
    FILE* fp = fopen(FILENAME, "w");
    if (!fp) return;
    Contact* temp = head;
    while (temp) {
        fprintf(fp, "%s,%s,%s\n", temp->name, temp->phone, temp->email);
        temp = temp->next;
    }
    fclose(fp);
}

void load_from_file() {
    FILE* fp = fopen(FILENAME, "r");
    if (!fp) return;

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        Contact* new_node = (Contact*)malloc(sizeof(Contact));
        sscanf(line, "%99[^,],%14[^,],%99[^"]", new_node->name, new_node->phone, new_node->email);
        new_node->next = head;
        head = new_node;
    }
    fclose(fp);
}

void backup_contacts() {
    FILE* src = fopen(FILENAME, "r");
    FILE* dest = fopen(BACKUP_FILENAME, "w");
    if (!src || !dest) return;

    char ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }
    fclose(src);
    fclose(dest);
    printf("Backup completed.\n");
}

void restore_contacts() {
    FILE* src = fopen(BACKUP_FILENAME, "r");
    FILE* dest = fopen(FILENAME, "w");
    if (!src || !dest) return;

    char ch;
    while ((ch = fgetc(src)) != EOF) {
        fputc(ch, dest);
    }
    fclose(src);
    fclose(dest);
    free_contacts();
    load_from_file();
    printf("Contacts restored from backup.\n");
}

void free_contacts() {
    Contact* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
