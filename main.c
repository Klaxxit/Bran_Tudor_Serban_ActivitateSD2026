#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { Trainee = 0, Senior = 1, Lead = 2 } Rank;

typedef struct {
    char name[50];
    Rank level;
} Employee;

typedef struct {
    char title[100];
    Rank minRank;
} Task;

typedef struct Department {
    char deptName[50];
    Employee* staff;
    int staffCount; 
    struct Department* next;
} Department;

Department* companyHead = NULL;
int deptCount = 0;

const char* rankToString(Rank r) {
    if (r == Trainee) return "Trainee";
    if (r == Senior)  return "Senior";
    if (r == Lead)    return "Lead";
    return "Unknown";
}

int isEmployeeQualified(const Employee* emp, const Task* task) {
    if (emp == NULL || task == NULL) return 0;
    return (emp->level >= task->minRank) ? 1 : 0;
}

const char* findEmployeeDepartment(const char* employeeName) {
    if (!employeeName) return NULL;
    Department* curr = companyHead;
    while (curr != NULL) {
        for (int i = 0; i < curr->staffCount; i++) {
            if (strcmp(curr->staff[i].name, employeeName) == 0) {
                return curr->deptName;
            }
        }
        curr = curr->next;
    }
    return NULL;
}

void saveData(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file for saving.\n");
        return;
    }
    fprintf(file, "%d\n", deptCount);
    Department* curr = companyHead;
    while (curr != NULL) {
        fprintf(file, "%s\n", curr->deptName);
        fprintf(file, "%d\n", curr->staffCount);
        for (int j = 0; j < curr->staffCount; j++) {
            fprintf(file, "%s\n", curr->staff[j].name);
            fprintf(file, "%d\n", curr->staff[j].level);
        }
        curr = curr->next;
    }
    fclose(file);
}

void loadData(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) return;

    char buffer[100];
    if (!fgets(buffer, sizeof(buffer), file)) {
        fclose(file);
        return;
    }
    deptCount = atoi(buffer);
    Department* tail = NULL;

    for (int i = 0; i < deptCount; i++) {
        Department* newDept = (Department*)malloc(sizeof(Department));
        newDept->next = NULL;

        fgets(newDept->deptName, 50, file);
        newDept->deptName[strcspn(newDept->deptName, "\n")] = 0;

        fgets(buffer, sizeof(buffer), file);
        newDept->staffCount = atoi(buffer);

        if (newDept->staffCount > 0) {
            newDept->staff = malloc(newDept->staffCount * sizeof(Employee));
            for (int j = 0; j < newDept->staffCount; j++) {
                fgets(newDept->staff[j].name, 50, file);
                newDept->staff[j].name[strcspn(newDept->staff[j].name, "\n")] = 0;

                fgets(buffer, sizeof(buffer), file);
                newDept->staff[j].level = (Rank)atoi(buffer);
            }
        } else {
            newDept->staff = NULL;
        }

        if (companyHead == NULL) {
            companyHead = newDept;
        } else {
            tail->next = newDept;
        }
        tail = newDept;
    }
    fclose(file);
}

int main() {
    loadData("company_data.txt");
    int choice;
    while (1) {
        printf("\n1. Add Employee\n2. View Organization\n3. Search Employee\n4. Exit\nSelection: ");
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

            Department* targetDept = NULL;
            Department* curr = companyHead;
            Department* tail = NULL;
            while (curr != NULL) {
                if (strcmp(curr->deptName, dName) == 0) {
                    targetDept = curr;
                    break;
                }
                tail = curr;
                curr = curr->next;
            }

            if (targetDept == NULL) {
                targetDept = (Department*)malloc(sizeof(Department));
                strcpy(targetDept->deptName, dName);
                targetDept->staff = NULL;
                targetDept->staffCount = 0;
                targetDept->next = NULL;
                
                if (companyHead == NULL) {
                    companyHead = targetDept;
                } else {
                    tail->next = targetDept;
                }
                deptCount++;
            }

            targetDept->staff = realloc(targetDept->staff, (targetDept->staffCount + 1) * sizeof(Employee));
            
            strcpy(targetDept->staff[targetDept->staffCount].name, eName);
            
            if(strcmp(eLevel, "Senior") == 0) targetDept->staff[targetDept->staffCount].level = Senior;
            else if(strcmp(eLevel, "Lead") == 0) targetDept->staff[targetDept->staffCount].level = Lead;
            else targetDept->staff[targetDept->staffCount].level = Trainee;

            targetDept->staffCount++;
            printf("Success!\n");

        } else if (choice == 2) {
            Department* curr = companyHead;
            while (curr != NULL) {
                printf("\nDept: %s\n", curr->deptName);
                for(int j = 0; j < curr->staffCount; j++) {
                    printf(" - %s [%s]\n", curr->staff[j].name, rankToString(curr->staff[j].level));
                }
                curr = curr->next;
            }
        } else if (choice == 3) {
            char searchName[50];
            printf("Enter Employee Name to Search: ");
            fgets(searchName, 50, stdin); searchName[strcspn(searchName, "\n")] = 0;
            const char* foundDept = findEmployeeDepartment(searchName);
            if (foundDept) {
                printf("Employee '%s' works in the '%s' department.\n", searchName, foundDept);
            } else {
                printf("Employee '%s' not found.\n", searchName);
            }
        } else if (choice == 4) {
            saveData("company_data.txt");
            Department* curr = companyHead;
            while (curr != NULL) {
                Department* temp = curr;
                free(curr->staff);
                curr = curr->next;
                free(temp);
            }
            break;
        }
    }
    return 0;
}