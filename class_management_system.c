#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

struct Teacher {
    char id[20];
    char pass[20];
    char name[50];
};

void signUp();
void login();
void teacherMenu(char *teacherName);
void adminMenu();
void submitQuery(char *studentName);
void entertainmentMode();

struct Teacher teachers[3] = {
    {"tec1", "pass1", "Ms.Fatima Ali"},
    {"tec2", "pass2", "Sir.Mustafa"},
    {"tec3", "pass3", "Sir.Shayan"}
};

int main() {
    int choice;
    while (1) {
        printf("\n============================\n");
        printf("         MAIN MENU\n");
        printf("============================\n");
        printf("1] SignUP\n2] Login\n3] Entertainment MODE\n4] Exit\nSelection: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1: signUp(); break;
            case 2: login(); break;
            case 3: entertainmentMode(); break;
            case 4: exit(0);
        }
    }
    return 0;
}

void signUp() {
    char name[50], fName[50], contact[20], id[20], pass[20];
    printf("Enter Name: "); scanf("%s", name);
    printf("Enter Father Name: "); scanf("%s", fName);
    printf("Enter Contact: "); scanf("%s", contact);
    printf("Enter ID: "); scanf("%s", id);
    printf("Enter Password: "); scanf("%s", pass);

    pid_t pid = fork();
    if (pid == 0) {
        // 1. Write to the BEAUTIFUL table file for the viewer
        int exists = access("SignUp.txt", F_OK);
        FILE *fp = fopen("SignUp.txt", "a");
        if (exists == -1) {
            fprintf(fp, "%-15s | %-15s | %-12s | %-10s | %-10s\n", "NAME", "FATHER NAME", "CONTACT", "ID", "PASS");
            fprintf(fp, "--------------------------------------------------------------------------\n");
        }
        fprintf(fp, "%-15s | %-15s | %-12s | %-10s | %-10s\n", name, fName, contact, id, pass);
        fclose(fp);

        // 2. Write to a SIMPLE file for the program to read (No bars/lines)
        FILE *db = fopen("login_db.txt", "a");
        fprintf(db, "%s %s %s\n", id, pass, name);
        fclose(db);
        exit(0);
    } else {
        wait(NULL);
        printf("\nRegistration Successful!\n");
    }
}

void login() {
    int role;
    char id[20], pass[20], fId[20], fPass[20], fName[50];
    printf("\nLogin As:\n1] Teacher\n2] Student\n3] Admin\nSelection: ");
    scanf("%d", &role);

    if (role == 3) { adminMenu(); return; }

    printf("Enter ID: "); scanf("%s", id);
    printf("Enter Password: "); scanf("%s", pass);

    if (role == 1) {
        for(int i = 0; i < 3; i++) {
            if (strcmp(id, teachers[i].id) == 0 && strcmp(pass, teachers[i].pass) == 0) {
                printf("\n============================\n");
                printf("    WELCOME %s", teachers[i].name);
                printf("\n============================\n");
                teacherMenu(teachers[i].name);
                return;
            }
        }
        printf("Login Failed!\n");
    } else if (role == 2) {
        FILE *fp = fopen("login_db.txt", "r");
        if (!fp) { printf("No students found!\n"); return; }
        int found = 0;
        while (fscanf(fp, "%s %s %s", fId, fPass, fName) != EOF) {
            if (strcmp(id, fId) == 0 && strcmp(pass, fPass) == 0) {
                found = 1; break;
            }
        }
        fclose(fp);
        if (found) {
            printf("\n============================\n");
            printf("       WELCOME %s", fName);
            printf("\n============================\n");
            printf("1] View Announcement\n2] Submit Query\nSelection: ");
            int sub; scanf("%d", &sub);
            if (sub == 1) system("cat announcements.txt");
            else if (sub == 2) submitQuery(fName);
        } else printf("Login Failed!\n");
    }
}

void teacherMenu(char *teacherName) {
    int choice;
    printf("\n1] Make Announcement\n2] View Student Records\n3] View Queries\nSelection: ");
    scanf("%d", &choice);
    if (choice == 1) {
        char msg[200];
        printf("Announcement: "); scanf("%s", msg);
        int exists = access("announcements.txt", F_OK);
        FILE *fp = fopen("announcements.txt", "a");
        if (exists == -1) {
            fprintf(fp, "%-15s | %-30s\n", "TEACHER", "ANNOUNCEMENT");
            fprintf(fp, "------------------------------------------------\n");
        }
        fprintf(fp, "%-15s | %-30s\n", teacherName, msg);
        fclose(fp);
    } else if (choice == 2) {
        system("cat SignUp.txt");
    } else if (choice == 3) {
        system("cat queries.txt");
    }
}

void adminMenu() {
    int choice;
    printf("\n---------------------------\n      WELCOME Admin\n---------------------------\n");
    printf("1] View Students \n2] List Files\n3] Turn off PC\nSelection: ");
    scanf("%d", &choice);
    if (choice == 1) system("cat SignUp.txt");
    else if (choice == 2) system("ls -l");
    else if (choice == 3) system("sudo poweroff");
}

void submitQuery(char *studentName) {
    char q[200];
    printf("Query: "); scanf("%s", q);
    int exists = access("queries.txt", F_OK);
    FILE *fp = fopen("queries.txt", "a");
    if (exists == -1) {
        fprintf(fp, "%-15s | %-30s\n", "STUDENT", "QUERY");
        fprintf(fp, "------------------------------------------------\n");
    }
    fprintf(fp, "%-15s | %-30s\n", studentName, q);
    fclose(fp);
}

void entertainmentMode() {
    int choice;
    printf("\n1] CowSay\n2] xCowsay\n3] Big Text\n4] Sea Life\nSelection: ");
    scanf("%d", &choice);
    switch (choice) {
        case 1: system("cowsay Hello!"); break;
        case 2: system("xcowsay 'Hey yea'"); break;
        case 3: system("figlet DONE"); break;
        case 4: system("asciiquarium"); break;
    }
}
