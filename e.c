#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100
#define NAME_LEN 50
#define PHONE_LEN 15
#define EMAIL_LEN 50

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
} Contact;

void addContact();
void displayContacts();
void searchContact();
void deleteContact();

int main() {
    int choice;
    do {
        printf("\n--- Contact Management System ---\n");
        printf("1. Add Contact\n");
        printf("2. Display Contacts\n");
        printf("3. Search Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch(choice) {
            case 1: addContact(); break;
            case 2: displayContacts(); break;
            case 3: searchContact(); break;
            case 4: deleteContact(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 5);

    return 0;
}

void addContact() {
    FILE *fp = fopen("contacts.dat", "ab");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    Contact c;
    printf("Enter Name: ");
    fgets(c.name, NAME_LEN, stdin);
    c.name[strcspn(c.name, "\n")] = 0; // remove newline

    printf("Enter Phone: ");
    fgets(c.phone, PHONE_LEN, stdin);
    c.phone[strcspn(c.phone, "\n")] = 0;

    printf("Enter Email: ");
    fgets(c.email, EMAIL_LEN, stdin);
    c.email[strcspn(c.email, "\n")] = 0;

    fwrite(&c, sizeof(Contact), 1, fp);
    fclose(fp);
    printf("Contact added successfully!\n");
}

void displayContacts() {
    FILE *fp = fopen("contacts.dat", "rb");
    if (fp == NULL) {
        printf("No contacts found.\n");
        return;
    }
    Contact c;
    printf("\n--- Contact List ---\n");
    while(fread(&c, sizeof(Contact), 1, fp)) {
        printf("Name : %s\nPhone: %s\nEmail: %s\n----------------\n", c.name, c.phone, c.email);
    }
    fclose(fp);
}

void searchContact() {
    char searchName[NAME_LEN];
    printf("Enter name to search: ");
    fgets(searchName, NAME_LEN, stdin);
    searchName[strcspn(searchName, "\n")] = 0;

    FILE *fp = fopen("contacts.dat", "rb");
    if (fp == NULL) {
        printf("No contacts found.\n");
        return;
    }
    Contact c;
    int found = 0;
    while(fread(&c, sizeof(Contact), 1, fp)) {
        if(strcasecmp(c.name, searchName) == 0) {
            printf("Contact found:\nName : %s\nPhone: %s\nEmail: %s\n", c.name, c.phone, c.email);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Contact not found.\n");
    fclose(fp);
}

void deleteContact() {
    char delName[NAME_LEN];
    printf("Enter name of contact to delete: ");
    fgets(delName, NAME_LEN, stdin);
    delName[strcspn(delName, "\n")] = 0;

    FILE *fp = fopen("contacts.dat", "rb");
    FILE *tmp = fopen("temp.dat", "wb");
    if (fp == NULL || tmp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    Contact c;
    int found = 0;
    while(fread(&c, sizeof(Contact), 1, fp)) {
        if(strcasecmp(c.name, delName) != 0) {
            fwrite(&c, sizeof(Contact), 1, tmp);
        } else {
            found = 1;
        }
    }
    fclose(fp);
    fclose(tmp);

    remove("contacts.dat");
    rename("temp.dat", "contacts.dat");

    if (found)
        printf("Contact deleted successfully.\n");
    else
        printf("Contact not found.\n");
}