/******************************************************************************

                            Online C Compiler.
                Code, Compile, Run and Debug C program online.
Write your code in this editor and press "Run" button to compile and execute it.

*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BIN_FILE "students.dat"
#define TXT_FILE "students.txt"

typedef struct {
    int roll;
    char name[50];
    char dept[30];
    int year;
    float marks;
    char grade;
} Student;

Student *records = NULL;
int count = 0, capacity = 0;

// Helper using static storage class to track operations and compute grades
char getGrade(float marks) {
    static int calcCount = 0;
    calcCount++;
    if (marks >= 90) return 'A';
    if (marks >= 75) return 'B';
    if (marks >= 60) return 'C';
    if (marks >= 40) return 'D';
    return 'F';
}

// Save to both Binary (fwrite) and Text (fprintf) files
void saveFiles() {
    FILE *fb = fopen(BIN_FILE, "wb");
    FILE *ft = fopen(TXT_FILE, "w");
    if (!fb || !ft) {
        printf("Error opening files for saving!\n");
        if (fb) fclose(fb);
        if (ft) fclose(ft);
        return;
    }
    if (count > 0) fwrite(records, sizeof(Student), count, fb);
    for (int i = 0; i < count; i++) {
        fprintf(ft, "%d|%s|%s|%d|%.2f|%c\n", records[i].roll, records[i].name,
                records[i].dept, records[i].year, records[i].marks, records[i].grade);
    }
    fclose(fb);
    fclose(ft);
}

// Load records from Binary file using malloc/realloc and EOF handling
void loadBinary() {
    FILE *fb = fopen(BIN_FILE, "rb");
    if (!fb) return; // First run, file doesn't exist yet

    capacity = 2;
    records = (Student *)malloc(capacity * sizeof(Student));
    if (!records) exit(1);

    Student temp;
    while (fread(&temp, sizeof(Student), 1, fb) == 1) { // Reads until EOF
        if (count == capacity) {
            capacity *= 2;
            Student *ptr = (Student *)realloc(records, capacity * sizeof(Student));
            if (!ptr) { free(records); fclose(fb); exit(1); }
            records = ptr;
        }
        records[count++] = temp;
    }
    fclose(fb);
}

// Demonstrate reading text file using fscanf and EOF
void readTextBackup() {
    FILE *ft = fopen(TXT_FILE, "r");
    if (!ft) { printf("No text file found.\n"); return; }
    Student s;
    printf("\n--- Text File Export View ---\n");
    while (fscanf(ft, "%d|%49[^|]|%29[^|]|%d|%f|%c\n", &s.roll, s.name, s.dept, &s.year, &s.marks, &s.grade) != EOF) {
        printf("Roll: %d | Name: %s | Dept: %s | Yr: %d | Marks: %.1f | Grade: %c\n",
               s.roll, s.name, s.dept, s.year, s.marks, s.grade);
    }
    fclose(ft);
}

void addStudent() {
    if (capacity == 0) {
        capacity = 2;
        records = (Student *)calloc(capacity, sizeof(Student)); // Zero-initialized allocation
        if (!records) return;
    } else if (count == capacity) {
        capacity *= 2;
        Student *ptr = (Student *)realloc(records, capacity * sizeof(Student));
        if (!ptr) { printf("Memory reallocation failed!\n"); return; }
        records = ptr;
    }

    Student s;
    printf("Roll No: "); scanf("%d", &s.roll);
    printf("Name: "); scanf(" %[^\n]", s.name);
    printf("Department: "); scanf(" %[^\n]", s.dept);
    printf("Year: "); scanf("%d", &s.year);
    printf("Marks: "); scanf("%f", &s.marks);
    s.grade = getGrade(s.marks);

    records[count++] = s;
    saveFiles();
    printf("Record added and saved!\n");
}

void displayAll() {
    if (count == 0) { printf("No records found.\n"); return; }
    printf("\nRoll\tName\t\tDept\tYear\tMarks\tGrade\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%-12s\t%s\t%d\t%.1f\t%c\n", records[i].roll, records[i].name,
               records[i].dept, records[i].year, records[i].marks, records[i].grade);
    }
}

void searchStudent() {
    int r;
    printf("Enter Roll No to search: "); scanf("%d", &r);
    for (int i = 0; i < count; i++) {
        if (records[i].roll == r) {
            printf("Found -> %s | Dept: %s | Year: %d | Marks: %.1f | Grade: %c\n",
                   records[i].name, records[i].dept, records[i].year, records[i].marks, records[i].grade);
            return;
        }
    }
    printf("Student not found.\n");
}

void updateStudent() {
    int r;
    printf("Enter Roll No to update: "); scanf("%d", &r);
    for (int i = 0; i < count; i++) {
        if (records[i].roll == r) {
            printf("New Dept: "); scanf(" %[^\n]", records[i].dept);
            printf("New Marks: "); scanf("%f", &records[i].marks);
            records[i].grade = getGrade(records[i].marks);
            saveFiles();
            printf("Record updated!\n");
            return;
        }
    }
    printf("Student not found.\n");
}

void deleteStudent() {
    int r;
    printf("Enter Roll No to delete: "); scanf("%d", &r);
    for (int i = 0; i < count; i++) {
        if (records[i].roll == r) {
            for (int j = i; j < count - 1; j++) records[j] = records[j + 1];
            count--;
            saveFiles();
            printf("Record deleted!\n");
            return;
        }
    }
    printf("Student not found.\n");
}

void filterByMarks() {
    float minMarks;
    printf("Enter minimum marks threshold: "); scanf("%f", &minMarks);
    printf("\nStudents scoring above %.1f:\n", minMarks);
    for (int i = 0; i < count; i++) {
        if (records[i].marks > minMarks)
            printf("%d - %s (%.1f)\n", records[i].roll, records[i].name, records[i].marks);
    }
}

int main() {
    loadBinary();
    int choice;
    while (1) {
        printf("\n1.Add  2.Display  3.Search  4.Update  5.Delete  6.Count  7.Filter  8.ReadText  9.Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Total Students: %d\n", count); break;
            case 7: filterByMarks(); break;
            case 8: readTextBackup(); break;
            case 9:
                free(records); // Release dynamic memory before exit
                printf("Memory freed. Exiting...\n");
                return 0;
            default: printf("Invalid choice.\n");
        }
    }
    free(records);
    return 0;
}