#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "contacts.json"
#define BACKUP_FILENAME "backup.json"

typedef struct Contact {
    char name[100];
    char phone[15];
    char email[100];
    struct Contact* left;
    struct Contact* right;
} Contact;

Contact* root = NULL;

// Function declarations
int login();
int is_valid_email(const char* email);
int is_valid_phone(const char* phone);
Contact* insert_contact(Contact* root, Contact* new_contact);
void in_order(Contact* root);
Contact* search_contact(Contact* root, const char* keyword);
Contact* delete_contact(Contact* root, const char* name);
Contact* min_value_node(Contact* node);
void edit_contact(Contact* root);
void save_to_file(Contact* root, FILE* fp);
void load_from_file();
void backup_contacts();
void restore_contacts();
void free_contacts(Contact* root);

int main() {
    if (!login()) {
        printf("Login failed. Exiting...\n");
        return 1;
    }

    load_from_file();

    int choice;
    do {
        printf("\n--- Contact Management System (BST) ---\n");
        printf("1. Add Contact\n2. Display Contacts\n3. Search Contact\n4. Edit Contact\n5. Delete Contact\n6. Backup Contacts\n7. Restore Contacts\n8. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        char name[100];
        Contact* result = NULL;

        switch (choice) {
            case 1: {
                Contact* new_contact = (Contact*)malloc(sizeof(Contact));
                printf("Enter name: ");
                fgets(new_contact->name, sizeof(new_contact->name), stdin);
                new_contact->name[strcspn(new_contact->name, "\n")] = 0;

                do {
                    printf("Enter phone: ");
                    fgets(new_contact->phone, sizeof(new_contact->phone), stdin);
                    new_contact->phone[strcspn(new_contact->phone, "\n")] = 0;
                } while (!is_valid_phone(new_contact->phone));

                do {
                    printf("Enter email: ");
                    fgets(new_contact->email, sizeof(new_contact->email), stdin);
                    new_contact->email[strcspn(new_contact->email, "\n")] = 0;
                } while (!is_valid_email(new_contact->email));

                new_contact->left = new_contact->right = NULL;
                root = insert_contact(root, new_contact);
                printf("Contact added.\n");
                break;
            }
            case 2:
                in_order(root);
                break;
            case 3:
                printf("Enter name or phone to search: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                result = search_contact(root, name);
                if (result)
                    printf("Found: %s | %s | %s\n", result->name, result->phone, result->email);
                else
                    printf("Contact not found.\n");
                break;
            case 4:
                edit_contact(root);
                break;
            case 5:
                printf("Enter name to delete: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                root = delete_contact(root, name);
                break;
            case 6:
                backup_contacts();
                break;
            case 7:
                restore_contacts();
                break;
            case 8:
                {
                    FILE* fp = fopen(FILENAME, "w");
                    fprintf(fp, "[\n");
                    save_to_file(root, fp);
                    fseek(fp, -2, SEEK_END);
                    fprintf(fp, "\n]\n");
                    fclose(fp);
                    free_contacts(root);
                    break;
                }
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 8);

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

Contact* insert_contact(Contact* root, Contact* new_contact) {
    if (!root) return new_contact;
    if (strcmp(new_contact->name, root->name) < 0)
        root->left = insert_contact(root->left, new_contact);
    else
        root->right = insert_contact(root->right, new_contact);
    return root;
}

void in_order(Contact* root) {
    if (!root) return;
    in_order(root->left);
    printf("%s\t%s\t%s\n", root->name, root->phone, root->email);
    in_order(root->right);
}

Contact* search_contact(Contact* root, const char* keyword) {
    if (!root) return NULL;
    if (strstr(root->name, keyword) || strstr(root->phone, keyword))
        return root;
    if (strcmp(keyword, root->name) < 0)
        return search_contact(root->left, keyword);
    return search_contact(root->right, keyword);
}

void edit_contact(Contact* root) {
    char name[100];
    printf("Enter name to edit: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    Contact* result = search_contact(root, name);
    if (result) {
        printf("Enter new phone: ");
        fgets(result->phone, sizeof(result->phone), stdin);
        result->phone[strcspn(result->phone, "\n")] = 0;
        printf("Enter new email: ");
        fgets(result->email, sizeof(result->email), stdin);
        result->email[strcspn(result->email, "\n")] = 0;
        printf("Contact updated.\n");
    } else {
        printf("Contact not found.\n");
    }
}

Contact* delete_contact(Contact* root, const char* name) {
    if (!root) return NULL;
    if (strcmp(name, root->name) < 0)
        root->left = delete_contact(root->left, name);
    else if (strcmp(name, root->name) > 0)
        root->right = delete_contact(root->right, name);
    else {
        if (!root->left) {
            Contact* temp = root->right;
            free(root);
            return temp;
        } else if (!root->right) {
            Contact* temp = root->left;
            free(root);
            return temp;
        }
        Contact* temp = min_value_node(root->right);
        strcpy(root->name, temp->name);
        strcpy(root->phone, temp->phone);
        strcpy(root->email, temp->email);
        root->right = delete_contact(root->right, temp->name);
    }
    return root;
}

Contact* min_value_node(Contact* node) {
    Contact* current = node;
    while (current && current->left)
        current = current->left;
    return current;
}

void save_to_file(Contact* root, FILE* fp) {
    if (!root) return;
    fprintf(fp, "  {\"name\": \"%s\", \"phone\": \"%s\", \"email\": \"%s\"},\n", root->name, root->phone, root->email);
    save_to_file(root->left, fp);
    save_to_file(root->right, fp);
}

void load_from_file() {
    FILE* fp = fopen(FILENAME, "r");
    if (!fp) return;
    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strchr(line, '{')) {
            Contact* new_contact = (Contact*)malloc(sizeof(Contact));
            sscanf(line, " {\"name\": \"%[^"]\", \"phone\": \"%[^"]\", \"email\": \"%[^"]\"},",
                new_contact->name, new_contact->phone, new_contact->email);
            new_contact->left = new_contact->right = NULL;
            root = insert_contact(root, new_contact);
        }
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
    free_contacts(root);
    root = NULL;
    load_from_file();
    printf("Contacts restored from backup.\n");
}

void free_contacts(Contact* root) {
    if (!root) return;
    free_contacts(root->left);
    free_contacts(root->right);
    free(root);
}
