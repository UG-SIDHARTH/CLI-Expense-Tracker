#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DESC_LEN 128
#define INITIAL_CAPACITY 16

typedef struct {
    char description[MAX_DESC_LEN];
    double amount;
} Expense;

typedef struct {
    Expense *items;
    size_t count;
    size_t capacity;
} ExpenseList;

void init_list(ExpenseList *list) {
    list->capacity = INITIAL_CAPACITY;
    list->count = 0;
    list->items = (Expense *)malloc(list->capacity * sizeof(Expense));
    if (list->items == NULL) {
        fprintf(stderr, "Fatal error: memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }
}

void free_list(ExpenseList *list) {
    if (list->items != NULL) {
        free(list->items);
        list->items = NULL;
    }
    list->count = 0;
    list->capacity = 0;
}

void trim_newline(char *str) {
    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r' || str[len - 1] == ' ' || str[len - 1] == '\t')) {
        str[len - 1] = '\0';
        len--;
    }
}

void read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) != NULL) {
        trim_newline(buffer);
    } else {
        buffer[0] = '\0';
    }
}

void add_expense(ExpenseList *list) {
    char desc[MAX_DESC_LEN];
    char amt_str[64];

    printf("\n--- Add New Expense ---\n");
    printf("Enter expense description: ");
    read_line(desc, sizeof(desc));

    if (strlen(desc) == 0) {
        printf("Description cannot be empty. Aborting.\n");
        return;
    }

    printf("Enter amount ($): ");
    read_line(amt_str, sizeof(amt_str));

    char *endptr;
    double amt = strtod(amt_str, &endptr);
    if (endptr == amt_str || *endptr != '\0' || amt < 0.0) {
        printf("Invalid amount. Please enter a valid positive number.\n");
        return;
    }

    // Resize if capacity reached
    if (list->count >= list->capacity) {
        size_t new_cap = list->capacity * 2;
        Expense *temp = (Expense *)realloc(list->items, new_cap * sizeof(Expense));
        if (temp == NULL) {
            fprintf(stderr, "Failed to allocate memory for more expenses.\n");
            return;
        }
        list->items = temp;
        list->capacity = new_cap;
    }

    strncpy(list->items[list->count].description, desc, MAX_DESC_LEN - 1);
    list->items[list->count].description[MAX_DESC_LEN - 1] = '\0';
    list->items[list->count].amount = amt;
    list->count++;

    printf("Expense added successfully!\n");
}

void list_expenses(const ExpenseList *list) {
    printf("\n--- All Expenses ---\n");
    if (list->count == 0) {
        printf("No expenses recorded yet.\n");
        return;
    }

    for (size_t i = 0; i < list->count; i++) {
        printf("%zu. %s - $%.2f\n", i + 1, list->items[i].description, list->items[i].amount);
    }
}

double calculate_total(const ExpenseList *list) {
    double total = 0.0;
    for (size_t i = 0; i < list->count; i++) {
        total += list->items[i].amount;
    }
    return total;
}

void filter_expenses(const ExpenseList *list) {
    char threshold_str[64];
    printf("Show expenses greater than: $");
    read_line(threshold_str, sizeof(threshold_str));

    char *endptr;
    double limit = strtod(threshold_str, &endptr);
    if (endptr == threshold_str || *endptr != '\0') {
        printf("Invalid number format entered.\n");
        return;
    }

    printf("\nExpenses greater than $%.2f:\n", limit);
    bool found = false;
    size_t display_index = 1;

    for (size_t i = 0; i < list->count; i++) {
        if (list->items[i].amount > limit) {
            printf("%zu. %s - $%.2f\n", display_index++, list->items[i].description, list->items[i].amount);
            found = true;
        }
    }

    if (!found) {
        printf("No expenses found above that amount.\n");
    }
}

void edit_expense(ExpenseList *list) {
    printf("\n--- Edit Expense ---\n");
    if (list->count == 0) {
        printf("No expenses to edit.\n");
        return;
    }

    list_expenses(list);

    char input_str[64];
    printf("Enter expense number to edit (1-%zu): ", list->count);
    read_line(input_str, sizeof(input_str));

    char *endptr;
    long choice = strtol(input_str, &endptr, 10);
    if (endptr == input_str || *endptr != '\0' || choice < 1 || (size_t)choice > list->count) {
        printf("Invalid expense number.\n");
        return;
    }

    size_t index = (size_t)(choice - 1);
    Expense *exp = &list->items[index];

    char new_desc[MAX_DESC_LEN];
    printf("Enter new description (press Enter to keep '%s'): ", exp->description);
    read_line(new_desc, sizeof(new_desc));
    if (strlen(new_desc) > 0) {
        strncpy(exp->description, new_desc, MAX_DESC_LEN - 1);
        exp->description[MAX_DESC_LEN - 1] = '\0';
    }

    char new_amt_str[64];
    printf("Enter new amount (press Enter to keep '$%.2f'): ", exp->amount);
    read_line(new_amt_str, sizeof(new_amt_str));
    if (strlen(new_amt_str) > 0) {
        double new_amt = strtod(new_amt_str, &endptr);
        if (endptr == new_amt_str || *endptr != '\0' || new_amt < 0.0) {
            printf("Invalid amount entered. Update cancelled.\n");
            return;
        }
        exp->amount = new_amt;
    }

    printf("Expense updated successfully!\n");
}

void delete_expense(ExpenseList *list) {
    printf("\n--- Delete Expense ---\n");
    if (list->count == 0) {
        printf("No expenses to delete.\n");
        return;
    }

    list_expenses(list);

    char input_str[64];
    printf("Enter expense number to delete (1-%zu): ", list->count);
    read_line(input_str, sizeof(input_str));

    char *endptr;
    long choice = strtol(input_str, &endptr, 10);
    if (endptr == input_str || *endptr != '\0' || choice < 1 || (size_t)choice > list->count) {
        printf("Invalid expense number.\n");
        return;
    }

    size_t index = (size_t)(choice - 1);
    char deleted_desc[MAX_DESC_LEN];
    strncpy(deleted_desc, list->items[index].description, MAX_DESC_LEN);
    double deleted_amt = list->items[index].amount;

    // Shift remaining elements left
    for (size_t i = index; i < list->count - 1; i++) {
        list->items[i] = list->items[i + 1];
    }
    list->count--;

    printf("Deleted expense: %s ($%.2f)\n", deleted_desc, deleted_amt);
}

void write_escaped_csv(FILE *fp, const char *str) {
    bool has_special = false;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',' || str[i] == '"' || str[i] == '\n' || str[i] == '\r') {
            has_special = true;
            break;
        }
    }

    if (!has_special) {
        fputs(str, fp);
        return;
    }

    fputc('"', fp);
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == '"') {
            fputc('"', fp); // Double escape double-quote
        }
        fputc(str[i], fp);
    }
    fputc('"', fp);
}

bool save_to_csv(const ExpenseList *list, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        return false;
    }

    fprintf(fp, "description,amount\n");
    for (size_t i = 0; i < list->count; i++) {
        write_escaped_csv(fp, list->items[i].description);
        fprintf(fp, ",%.2f\n", list->items[i].amount);
    }

    fclose(fp);
    return true;
}

int main(void) {
    ExpenseList expenses;
    init_list(&expenses);

    printf("==================================\n");
    printf("     CLI Expense Tracker (C)\n");
    printf("==================================\n");

    bool running = true;
    char choice_str[32];

    while (running) {
        printf("\nSelect an option:\n");
        printf("1. Add Expense\n");
        printf("2. List Expenses\n");
        printf("3. View Total Sum\n");
        printf("4. Filter Expenses by Amount\n");
        printf("5. Edit Expense\n");
        printf("6. Delete Expense\n");
        printf("7. Exit & Save to CSV\n");
        printf("8. Exit\n");
        printf("\nEnter choice (1-8): ");

        read_line(choice_str, sizeof(choice_str));

        if (strcmp(choice_str, "1") == 0) {
            add_expense(&expenses);
        } else if (strcmp(choice_str, "2") == 0) {
            list_expenses(&expenses);
        } else if (strcmp(choice_str, "3") == 0) {
            printf("\nTotal Expenses: $%.2f\n", calculate_total(&expenses));
        } else if (strcmp(choice_str, "4") == 0) {
            filter_expenses(&expenses);
        } else if (strcmp(choice_str, "5") == 0) {
            edit_expense(&expenses);
        } else if (strcmp(choice_str, "6") == 0) {
            delete_expense(&expenses);
        } else if (strcmp(choice_str, "7") == 0) {
            if (save_to_csv(&expenses, "expenses.csv")) {
                printf("Saved %zu expense(s) to expenses.csv successfully!\n", expenses.count);
            } else {
                printf("Error saving to expenses.csv!\n");
            }
            printf("Exiting program. Goodbye!\n");
            running = false;
        } else if (strcmp(choice_str, "8") == 0) {
            printf("Exiting program without saving. Goodbye!\n");
            running = false;
        } else {
            printf("Invalid option. Please enter a number between 1 and 8.\n");
        }
    }

    free_list(&expenses);
    return 0;
}

