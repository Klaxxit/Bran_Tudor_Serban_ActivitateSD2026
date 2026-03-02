#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { Trainee = 0, Senior = 1, Lead = 2 } Rank;

typedef struct {
    char name[50];
    Rank level;
} Employee;

typedef struct {
    char deptName[50];
    Employee* staff;
    int staffCount; 
} Department;

Department company[10];
int deptCount = 0;

const char* rankToString(Rank r) {
    if (r == Trainee) return "Trainee";
    if (r == Senior)  return "Senior";
    if (r == Lead)    return "Lead";
    return "Unknown";
}

int main() {
    int choice;
    while (1) {
        printf("\n1. Add Employee\n2. View Organization\n3. Exit\nSelection: ");
        scanf("%d", &choice);
        getchar();

        if (choice == 1) {
            char dName[50], eName[50], eLevel[20];
            printf("Enter Department: ");
            fgets(dName, 50, stdin); dName[strcspn(dName, "\n")] = 0;

            printf("Enter Name: ");
            fgets(eName, 50, stdin); eName[strcspn(eName, "\n")] = 0;

            printf("Enter Level (Trainee/Senior/Lead): ");
            fgets(eLevel, 20, stdin); eLevel[strcspn(eLevel, "\n")] = 0;

            int found = -1;
            for(int i = 0; i < deptCount; i++) {
                if(strcmp(company[i].deptName, dName) == 0) {
                    found = i;
                    break;
                }
            }

            if(found == -1) {
                strcpy(company[deptCount].deptName, dName);
                company[deptCount].staff = NULL;
                company[deptCount].staffCount = 0;
                found = deptCount;
                deptCount++;
            }

            company[found].staff = realloc(company[found].staff, (company[found].staffCount + 1) * sizeof(Employee));
            
            strcpy(company[found].staff[company[found].staffCount].name, eName);
            
            if(strcmp(eLevel, "Senior") == 0) company[found].staff[company[found].staffCount].level = Senior;
            else if(strcmp(eLevel, "Lead") == 0) company[found].staff[company[found].staffCount].level = Lead;
            else company[found].staff[company[found].staffCount].level = Trainee;

            company[found].staffCount++;
            printf("Success!\n");

        } else if (choice == 2) {
            for(int i = 0; i < deptCount; i++) {
                printf("\nDept: %s\n", company[i].deptName);
                for(int j = 0; j < company[i].staffCount; j++) {
                    printf(" - %s [%s]\n", company[i].staff[j].name, rankToString(company[i].staff[j].level));
                }
            }
        } else if (choice == 3) {
            for(int i = 0; i < deptCount; i++) {
                free(company[i].staff);
            }
            break;
        }
    }
    return 0;
}