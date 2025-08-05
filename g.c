#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CONTACTS 1000
#define MAX_GROUPS 10
#define NAME_LEN 50
#define EMAIL_LEN 50
#define PHONE_LEN 15
#define DATE_LEN 11
#define GROUP_NAME_LEN 30
#define ACTIVITY_LEN 100

typedef struct {
    char name[NAME_LEN];
    char email[EMAIL_LEN];
    char phone[PHONE_LEN];
    char birthday[DATE_LEN]; // Format: YYYY-MM-DD
    int group_id;
} Contact;

typedef struct {
    char name[GROUP_NAME_LEN];
} Group;

typedef struct {
    char activity[ACTIVITY_LEN];
    char timestamp[30];
} Activity;

Contact contacts[MAX_CONTACTS];
int contact_count = 0;
Group groups[MAX_GROUPS];
int group_count = 0;
Activity recent_activities[50];
int activity_count = 0;

// Hardcoded user for demo
const char* demo_email = "user@example.com";
const char* demo_phone = "1234567890";
const char* demo_password = "password";

// Helper functions
void add_activity(const char* msg) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char buf[30];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    snprintf(recent_activities[activity_count % 50].activity, ACTIVITY_LEN, "%s", msg);
    snprintf(recent_activities[activity_count % 50].timestamp, 30, "%s", buf);
    activity_count++;
}

void pause() {
    printf("\nPress Enter to continue...");
    getchar();
}

void clear() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// 1. Login
int login() {
    char input[EMAIL_LEN];
    char password[30];
    printf("Login (email or phone): ");
    fgets(input, EMAIL_LEN, stdin);
    input[strcspn(input, "\n")] = 0; // remove newline

    printf("Password: ");
    fgets(password, 30, stdin);
    password[strcspn(password, "\n")] = 0;

    if ((strcmp(input, demo_email) == 0 || strcmp(input, demo_phone) == 0) && strcmp(password, demo_password) == 0) {
        printf("Login successful!\n");
        add_activity("User logged in.");
        return 1;
    } else {
        printf("Invalid credentials!\n");
        add_activity("Failed login attempt.");
        return 0;
    }
}

// 2. Add new contact
void add_contact() {
    if (contact_count >= MAX_CONTACTS) {
        printf("Contacts full!\n");
        return;
    }
    Contact c;
    printf("Name: "); fgets(c.name, NAME_LEN, stdin); c.name[strcspn(c.name, "\n")] = 0;
    printf("Email: "); fgets(c.email, EMAIL_LEN, stdin); c.email[strcspn(c.email, "\n")] = 0;
    printf("Phone: "); fgets(c.phone, PHONE_LEN, stdin); c.phone[strcspn(c.phone, "\n")] = 0;
    printf("Birthday (YYYY-MM-DD): "); fgets(c.birthday, DATE_LEN, stdin); c.birthday[strcspn(c.birthday, "\n")] = 0;
    printf("Group ID (0 for None): "); scanf("%d", &c.group_id); getchar();

    contacts[contact_count++] = c;
    printf("Contact added!\n");
    add_activity("Added new contact.");
}

// 3. Delete contact
void delete_contact() {
    char search[NAME_LEN];
    printf("Enter name/email/phone to delete: ");
    fgets(search, NAME_LEN, stdin); search[strcspn(search, "\n")] = 0;
    for (int i = 0; i < contact_count; i++) {
        if (strstr(contacts[i].name, search) || strstr(contacts[i].email, search) || strstr(contacts[i].phone, search)) {
            printf("Deleting contact: %s\n", contacts[i].name);
            for (int j = i; j < contact_count - 1; j++)
                contacts[j] = contacts[j + 1];
            contact_count--;
            add_activity("Deleted a contact.");
            printf("Contact deleted.\n");
            return;
        }
    }
    printf("Contact not found.\n");
}

// 4. View all contacts
void view_contacts() {
    if (contact_count == 0) {
        printf("No contacts.\n");
        return;
    }
    printf("Contacts List:\n");
    for (int i = 0; i < contact_count; i++) {
        printf("%d. %s | %s | %s | Birthday: %s | Group: %d\n", i + 1, contacts[i].name, contacts[i].email, contacts[i].phone, contacts[i].birthday, contacts[i].group_id);
    }
    add_activity("Viewed all contacts.");
}

// 5. Search contact
void search_contact() {
    char search[NAME_LEN];
    printf("Enter name/email/phone to search: ");
    fgets(search, NAME_LEN, stdin); search[strcspn(search, "\n")] = 0;
    int found = 0;
    for (int i = 0; i < contact_count; i++) {
        if (strstr(contacts[i].name, search) || strstr(contacts[i].email, search) || strstr(contacts[i].phone, search)) {
            printf("%d. %s | %s | %s | Birthday: %s | Group: %d\n", i + 1, contacts[i].name, contacts[i].email, contacts[i].phone, contacts[i].birthday, contacts[i].group_id);
            found = 1;
        }
    }
    if (!found)
        printf("No contacts found.\n");
    add_activity("Searched for a contact.");
}

// 6. Edit contact
void edit_contact() {
    char search[NAME_LEN];
    printf("Enter name/email/phone to edit: ");
    fgets(search, NAME_LEN, stdin); search[strcspn(search, "\n")] = 0;
    for (int i = 0; i < contact_count; i++) {
        if (strstr(contacts[i].name, search) || strstr(contacts[i].email, search) || strstr(contacts[i].phone, search)) {
            printf("Editing %s\n", contacts[i].name);
            printf("New Name (or . to keep): "); char buf[NAME_LEN]; fgets(buf, NAME_LEN, stdin); buf[strcspn(buf, "\n")] = 0;
            if (strcmp(buf, ".") != 0 && strlen(buf) > 0) strcpy(contacts[i].name, buf);
            printf("New Email (or . to keep): "); fgets(buf, EMAIL_LEN, stdin); buf[strcspn(buf, "\n")] = 0;
            if (strcmp(buf, ".") != 0 && strlen(buf) > 0) strcpy(contacts[i].email, buf);
            printf("New Phone (or . to keep): "); fgets(buf, PHONE_LEN, stdin); buf[strcspn(buf, "\n")] = 0;
            if (strcmp(buf, ".") != 0 && strlen(buf) > 0) strcpy(contacts[i].phone, buf);
            printf("New Birthday (YYYY-MM-DD or . to keep): "); fgets(buf, DATE_LEN, stdin); buf[strcspn(buf, "\n")] = 0;
            if (strcmp(buf, ".") != 0 && strlen(buf) > 0) strcpy(contacts[i].birthday, buf);
            printf("New Group ID (or -1 to keep): "); int gid; scanf("%d", &gid); getchar();
            if (gid != -1) contacts[i].group_id = gid;
            printf("Contact updated.\n");
            add_activity("Edited a contact.");
            return;
        }
    }
    printf("Contact not found.\n");
}

// 7. Group contacts
void group_contacts() {
    printf("Groups:\n");
    for (int i = 0; i < group_count; i++)
        printf("%d. %s\n", i + 1, groups[i].name);
    printf("1. Add Group\n2. View Contacts in Group\nChoose: ");
    int ch; scanf("%d", &ch); getchar();
    if (ch == 1) {
        if (group_count >= MAX_GROUPS) {
            printf("Groups full!\n");
            return;
        }
        printf("Enter Group Name: ");
        fgets(groups[group_count].name, GROUP_NAME_LEN, stdin);
        groups[group_count].name[strcspn(groups[group_count].name, "\n")] = 0;
        printf("Group added with ID: %d\n", group_count);
        group_count++;
        add_activity("Added a new group.");
    } else if (ch == 2) {
        printf("Enter Group ID: "); int gid; scanf("%d", &gid); getchar();
        printf("Contacts in Group %d:\n", gid);
        for (int i = 0; i < contact_count; i++)
            if (contacts[i].group_id == gid)
                printf("%d. %s | %s | %s | Birthday: %s\n", i + 1, contacts[i].name, contacts[i].email, contacts[i].phone, contacts[i].birthday);
        add_activity("Viewed contacts in a group.");
    }
}

// 8. Birthday reminder
void birthday_reminder() {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char today[DATE_LEN];
    snprintf(today, DATE_LEN, "%04d-%02d-%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday);
    printf("Today's birthdays:\n");
    int found = 0;
    for (int i = 0; i < contact_count; i++) {
        // Compare only month and day for birthday reminder
        if (strlen(contacts[i].birthday) == 10 && 
            strncmp(contacts[i].birthday + 5, today + 5, 5) == 0) {
            printf("%s | %s | %s\n", contacts[i].name, contacts[i].email, contacts[i].phone);
            found = 1;
        }
    }
    if (!found) printf("No birthdays today.\n");
    add_activity("Checked birthday reminders.");
}

// 9. View recent activity
void view_recent_activity() {
    printf("Recent Activities:\n");
    int start = (activity_count > 10) ? activity_count - 10 : 0;
    for (int i = start; i < activity_count; i++) {
        printf("%s - %s\n", recent_activities[i % 50].timestamp, recent_activities[i % 50].activity);
    }
}

// Main menu
void menu() {
    int ch;
    do {
        printf("\nContact Management System\n");
        printf("1. Add New Contact\n");
        printf("2. Delete Contact\n");
        printf("3. View All Contacts\n");
        printf("4. Search Contact\n");
        printf("5. Edit Contact\n");
        printf("6. Group Contacts\n");
        printf("7. Birthday Reminder\n");
        printf("8. View Recent Activity\n");
        printf("0. Exit\n");
        printf("Choose: ");
        scanf("%d", &ch); getchar();
        clear();
        switch (ch) {
            case 1: add_contact(); break;
            case 2: delete_contact(); break;
            case 3: view_contacts(); break;
            case 4: search_contact(); break;
            case 5: edit_contact(); break;
            case 6: group_contacts(); break;
            case 7: birthday_reminder(); break;
            case 8: view_recent_activity(); break;
            case 0: printf("Exiting...\n"); add_activity("Exited system."); break;
            default: printf("Invalid choice.\n"); break;
        }
        pause();
        clear();
    } while (ch != 0);
}

int main() {
    clear();
    printf("Welcome to Contact Management System\n");
    while (!login()) {
        printf("Try again.\n");
    }
    menu();
    return 0;
}