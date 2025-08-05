#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME_LEN 50
#define PHONE_LEN 15
#define EMAIL_LEN 50
#define GROUP_LEN 20
#define DATE_LEN 11 // "YYYY-MM-DD" + null

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    char group[GROUP_LEN];
    char birthday[DATE_LEN]; // YYYY-MM-DD
} Contact;

void addContact();
void viewContacts();
void groupContacts();
void birthdayReminder();
int isBirthdayToday(const char *birthday);

int main() {
    int choice;
    do {
        printf("\n--- Contact Management System ---\n");
        printf("1. Add Contact\n");
        printf("2. View All Contacts\n");
        printf("3. Group Contacts\n");
        printf("4. Birthday Reminder\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch(choice) {
            case 1: addContact(); break;
            case 2: viewContacts(); break;
            case 3: groupContacts(); break;
            case 4: birthdayReminder(); break;
            case 5: printf("Exiting...\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while(choice != 5);

    return 0;
}

void addContact() {
    FILE *fp = fopen("contacts.txt", "a");
    if (fp == NULL) {
        printf("Error opening file!\n");
        return;
    }
    Contact c;

    printf("Enter Name: ");
    fgets(c.name, NAME_LEN, stdin); c.name[strcspn(c.name, "\n")] = 0;

    printf("Enter Phone: ");
    fgets(c.phone, PHONE_LEN, stdin); c.phone[strcspn(c.phone, "\n")] = 0;

    printf("Enter Email: ");
    fgets(c.email, EMAIL_LEN, stdin); c.email[strcspn(c.email, "\n")] = 0;

    printf("Enter Group (Family/Friends/Work/Other): ");
    fgets(c.group, GROUP_LEN, stdin); c.group[strcspn(c.group, "\n")] = 0;

    printf("Enter Birthday (YYYY-MM-DD): ");
    fgets(c.birthday, DATE_LEN, stdin); c.birthday[strcspn(c.birthday, "\n")] = 0;

    fprintf(fp, "%s|%s|%s|%s|%s\n", c.name, c.phone, c.email, c.group, c.birthday);
    fclose(fp);
    printf("Contact added successfully!\n");
}

void viewContacts() {
    FILE *fp = fopen("contacts.txt", "r");
    if (fp == NULL) {
        printf("No contacts found.\n");
        return;
    }
    Contact c;
    printf("\n--- All Contacts ---\n");
    while (fscanf(fp, "%49[^|]|%14[^|]|%49[^|]|%19[^|]|%10[^\n]\n", c.name, c.phone, c.email, c.group, c.birthday) == 5) {
        printf("Name: %s\nPhone: %s\nEmail: %s\nGroup: %s\nBirthday: %s\n-----------------\n", 
               c.name, c.phone, c.email, c.group, c.birthday);
    }
    fclose(fp);
}

void groupContacts() {
    char searchGroup[GROUP_LEN];
    printf("Enter group to view (Family/Friends/Work/Other): ");
    fgets(searchGroup, GROUP_LEN, stdin);
    searchGroup[strcspn(searchGroup, "\n")] = 0;

    FILE *fp = fopen("contacts.txt", "r");
    if (fp == NULL) {
        printf("No contacts found.\n");
        return;
    }
    Contact c;
    int found = 0;
    printf("\n--- Contacts in Group: %s ---\n", searchGroup);
    while (fscanf(fp, "%49[^|]|%14[^|]|%49[^|]|%19[^|]|%10[^\n]\n", c.name, c.phone, c.email, c.group, c.birthday) == 5) {
        if (strcasecmp(c.group, searchGroup) == 0) {
            printf("Name: %s\nPhone: %s\nEmail: %s\nBirthday: %s\n-----------------\n", 
                   c.name, c.phone, c.email, c.birthday);
            found = 1;
        }
    }
    if (!found)
        printf("No contacts found in this group.\n");
    fclose(fp);
}

void birthdayReminder() {
    FILE *fp = fopen("contacts.txt", "r");
    if (fp == NULL) {
        printf("No contacts found.\n");
        return;
    }
    Contact c;
    int found = 0;
    printf("\n--- Birthday Reminder ---\n");
    while (fscanf(fp, "%49[^|]|%14[^|]|%49[^|]|%19[^|]|%10[^\n]\n", c.name, c.phone, c.email, c.group, c.birthday) == 5) {
        if (isBirthdayToday(c.birthday)) {
            printf("It's %s's birthday today! (Group: %s, Phone: %s, Email: %s)\n", 
                    c.name, c.group, c.phone, c.email);
            found = 1;
        }
    }
    if (!found)
        printf("No birthdays today.\n");
    fclose(fp);
}

int isBirthdayToday(const char *birthday) {
    // birthday format: YYYY-MM-DD
    if (strlen(birthday) != 10) return 0;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int b_month, b_day;
    sscanf(birthday, "%*4d-%2d-%2d", &b_month, &b_day);
    return (tm.tm_mon + 1 == b_month && tm.tm_mday == b_day);
}