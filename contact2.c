#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <process.h>
#include <stdlib.h>
#include <dos.h>

void login()
{
    int a = 0, i = 0;
    char uname[10], c = '-';
    char pword[10], code[10];
    char user[10] = "user";
    char pass[10] = "pass";

    do
    {
        printf("\n \xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
        printf("LOGIN FORM\n");
        printf("\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2");
        printf("\n\n\t\tENTER USERNAME:-");
        scanf("%s", &uname);
        printf("\n\n\t\tENTER PASSWORD:-");
        while (i < 10)
        {
            pword[i] = getch();
            c = pword[i];
            if (c == 13) break;
            else printf("*");
            i++;
        }
        pword[i] = '\0';
        // char code = pword;
        i = 0;
        // scanf("%s",&pword);

        if (strcmp(uname, "user") == 0 && strcmp(pword, "pass") == 0)
    {
    printf(" \n\n\n    WELCOME TO CONTACT MANAGEMENT SYSTEM !!!! LOGIN IS SUCCESSFUL");
    printf("\n\n\n\t\t\tPress any key to continue...");
    getch(); // holds the screen
    break;
}
else
{
    printf("\n    SORRY !!!! LOGIN IS UNSUCCESSFUL");
    a++;

    getch(); // holds the screen
}
}
while (a <= 2);
if (a > 2)
{
    printf("\nSorry you have entered the wrong username and password for four times!!!!");
    getch();
}
system("cls");
}

struct contact
{
    long ph;
    char name[20], add[20], email[30];
} list;

char query[20], name[20];

FILE *fp, *ft;

int i, n, ch, l, found;

int main(){
    login();
    system("cls");
    printf("\n\t \xB2\xB2\xB2\xB2\xB2\xB2 Contact Management System \xB2\xB2\xB2\xB2\xB2\xB2");

MENU\n\t\t\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\x B2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\n\t\t<1> Add new Contact\n\t\t<2> List all Contacts\n\t\t<3> Search for contact\n\t\t<4> Edit a Contact\n\t\t<5> Delete a Contact\n\t\t<0> Exit\n\t\t\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\xB2\ xB2\xB2\xB2\xB2\xB2\xB2\xB2\n\t\t");

for(i=0;i<50;i++)

// printf("\n\t\tCurrent date and time: %s", ctime(&t));

printf("\n\t\tYour Choice:");

}