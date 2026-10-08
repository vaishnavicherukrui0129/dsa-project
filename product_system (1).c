/*
 * Product Search and Sorting System for E-Commerce
 * Prototype - DSA Mini Project (C)
 *
 * Features: Add, Display, Search (Linear / Binary), Sort (Bubble /
 * Selection / Merge), Sort each category separately, Update, Delete
 *
 * Compile:  gcc product_system.c -o product_system
 * Run:      ./product_system
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_PRODUCTS 100

struct Product {
    int id;
    char name[50];
    char category[30];
    float price;
    float rating;
    int stock;
};

struct Product products[MAX_PRODUCTS];
int count = 0;

/* ---------- Small input helpers (safe, beginner friendly) ---------- */

int readInt(const char *prompt) {
    char line[100];
    int value;
    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
        if (sscanf(line, "%d", &value) == 1) return value;
        printf("Invalid number. Try again.\n");
    }
}

float readFloat(const char *prompt) {
    char line[100];
    float value;
    while (1) {
        printf("%s", prompt);
        if (fgets(line, sizeof(line), stdin) == NULL) exit(0);
        if (sscanf(line, "%f", &value) == 1) return value;
        printf("Invalid number. Try again.\n");
    }
}

void readString(const char *prompt, char *dest, int size) {
    printf("%s", prompt);
    if (fgets(dest, size, stdin) == NULL) exit(0);
    dest[strcspn(dest, "\n")] = '\0';   /* remove the newline */
}

void toLowerStr(const char *src, char *dest) {
    int i = 0;
    for (; src[i] != '\0'; i++) dest[i] = (char)tolower((unsigned char)src[i]);
    dest[i] = '\0';
}

/* ---------- Display helpers ---------- */

void printHeader(void) {
    printf("\n%-6s %-20s %-14s %-10s %-7s %-6s\n",
           "ID", "Name", "Category", "Price", "Rating", "Stock");
    printf("-------------------------------------------------------------------\n");
}

void printProduct(struct Product p) {
    printf("%-6d %-20s %-14s %-10.2f %-7.1f %-6d\n",
           p.id, p.name, p.category, p.price, p.rating, p.stock);
}

void displayProducts(void) {
    if (count == 0) {
        printf("\nNo products available.\n");
        return;
    }
    printHeader();
    for (int i = 0; i < count; i++) printProduct(products[i]);
}

/* ---------- Searching ---------- */

/* Linear Search by ID - O(n) */
int linearSearch(int id) {
    for (int i = 0; i < count; i++) {
        if (products[i].id == id) return i;
    }
    return -1;
}

/* Binary Search by ID - O(log n). Array MUST be sorted by ID. */
int binarySearch(int id) {
    int low = 0, high = count - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (products[mid].id == id) return mid;
        if (products[mid].id < id) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

/* Linear search by name or category (partial, case-insensitive match) */
void searchByText(const char *text, int byName) {
    char key[50], field[50];
    int found = 0;
    toLowerStr(text, key);

    for (int i = 0; i < count; i++) {
        toLowerStr(byName ? products[i].name : products[i].category, field);
        if (strstr(field, key) != NULL) {
            if (!found) printHeader();
            printProduct(products[i]);
            found = 1;
        }
    }
    if (!found) printf("\nProduct not found.\n");
}

/* ---------- Sorting ---------- */

void swapProducts(struct Product *a, struct Product *b) {
    struct Product temp = *a;
    *a = *b;
    *b = temp;
}

/* Bubble Sort by price (ascending) - O(n^2), O(n) best with the flag */
void bubbleSortByPrice(void) {
    for (int i = 0; i < count - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < count - i - 1; j++) {
            if (products[j].price > products[j + 1].price) {
                swapProducts(&products[j], &products[j + 1]);
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

/* Bubble Sort by name (A-Z) */
void bubbleSortByName(void) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(products[j].name, products[j + 1].name) > 0)
                swapProducts(&products[j], &products[j + 1]);
        }
    }
}

/* Selection Sort by rating (highest first) - O(n^2) */
void selectionSortByRating(void) {
    for (int i = 0; i < count - 1; i++) {
        int maxIndex = i;
        for (int j = i + 1; j < count; j++) {
            if (products[j].rating > products[maxIndex].rating)
                maxIndex = j;
        }
        if (maxIndex != i) swapProducts(&products[i], &products[maxIndex]);
    }
}

/* Merge Sort by ID - O(n log n) */
void merge(int low, int mid, int high) {
    struct Product temp[MAX_PRODUCTS];
    int i = low, j = mid + 1, k = low;

    while (i <= mid && j <= high) {
        if (products[i].id <= products[j].id) temp[k++] = products[i++];
        else temp[k++] = products[j++];
    }
    while (i <= mid)  temp[k++] = products[i++];
    while (j <= high) temp[k++] = products[j++];

    for (i = low; i <= high; i++) products[i] = temp[i];
}

void mergeSort(int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        mergeSort(low, mid);
        mergeSort(mid + 1, high);
        merge(low, mid, high);
    }
}

/* ---------- Menu operations ---------- */

void addProduct(void) {
    if (count >= MAX_PRODUCTS) {
        printf("\nStorage full! Cannot add more products.\n");
        return;
    }
    struct Product p;
    p.id = readInt("Enter Product ID: ");
    if (linearSearch(p.id) != -1) {
        printf("A product with this ID already exists.\n");
        return;
    }
    readString("Enter Name: ", p.name, sizeof(p.name));
    readString("Enter Category: ", p.category, sizeof(p.category));
    p.price  = readFloat("Enter Price: ");
    p.rating = readFloat("Enter Rating (0-5): ");
    p.stock  = readInt("Enter Stock: ");

    if (p.price < 0 || p.rating < 0 || p.rating > 5 || p.stock < 0) {
        printf("Invalid values (price/stock >= 0, rating 0-5). Not added.\n");
        return;
    }
    products[count++] = p;
    printf("\nProduct added successfully.\n");
}

void searchMenu(void) {
    printf("\n--- Search Product ---\n");
    printf("1. By ID (Linear Search)\n");
    printf("2. By ID (Binary Search)\n");
    printf("3. By Name\n");
    printf("4. By Category\n");
    int choice = readInt("Enter choice: ");

    if (choice == 1 || choice == 2) {
        int id = readInt("Enter Product ID: ");
        int pos;
        if (choice == 1) {
            pos = linearSearch(id);
        } else {
            mergeSort(0, count - 1);      /* binary search needs sorted data */
            printf("(Products sorted by ID first for Binary Search)\n");
            pos = binarySearch(id);
        }
        if (pos == -1) {
            printf("\nProduct not found.\n");
        } else {
            printf("\nProduct Found\n");
            printf("ID       : %d\n", products[pos].id);
            printf("Name     : %s\n", products[pos].name);
            printf("Category : %s\n", products[pos].category);
            printf("Price    : %.2f\n", products[pos].price);
            printf("Rating   : %.1f\n", products[pos].rating);
            printf("Stock    : %d\n", products[pos].stock);
        }
    } else if (choice == 3 || choice == 4) {
        char text[50];
        readString(choice == 3 ? "Enter name: " : "Enter category: ",
                   text, sizeof(text));
        searchByText(text, choice == 3);
    } else {
        printf("Invalid choice.\n");
    }
}

void sortMenu(void) {
    if (count == 0) {
        printf("\nNo products to sort.\n");
        return;
    }
    printf("\n--- Sort Products ---\n");
    printf("1. By Price, low to high (Bubble Sort)\n");
    printf("2. By Rating, high to low (Selection Sort)\n");
    printf("3. By Product ID (Merge Sort)\n");
    printf("4. By Name, A to Z (Bubble Sort)\n");
    int choice = readInt("Enter choice: ");

    switch (choice) {
        case 1: bubbleSortByPrice();           break;
        case 2: selectionSortByRating();       break;
        case 3: mergeSort(0, count - 1);       break;
        case 4: bubbleSortByName();            break;
        default: printf("Invalid choice.\n");  return;
    }
    printf("\nSorted successfully.");
    displayProducts();
}

void updateProduct(void) {
    int id = readInt("Enter ID of product to update: ");
    int pos = linearSearch(id);
    if (pos == -1) {
        printf("\nProduct not found.\n");
        return;
    }
    float newPrice  = readFloat("Enter new Price: ");
    float newRating = readFloat("Enter new Rating (0-5): ");
    int   newStock  = readInt("Enter new Stock: ");

    if (newPrice < 0 || newRating < 0 || newRating > 5 || newStock < 0) {
        printf("Invalid values. Update cancelled.\n");
        return;
    }
    products[pos].price  = newPrice;
    products[pos].rating = newRating;
    products[pos].stock  = newStock;
    printf("\nProduct updated successfully.\n");
}

void deleteProduct(void) {
    int id = readInt("Enter ID of product to delete: ");
    int pos = linearSearch(id);
    if (pos == -1) {
        printf("\nProduct not found.\n");
        return;
    }
    for (int i = pos; i < count - 1; i++) products[i] = products[i + 1];
    count--;
    printf("\nProduct deleted successfully.\n");
}

/* ---------- Category-wise sorting (each category sorted separately) ---------- */

long compareCount = 0, moveCount = 0;
int sortField = 1;   /* 1 = price, 2 = rating, 3 = name, 4 = id */
int sortDir = 1;     /* 1 = low to high, -1 = high to low */

int compareText(const char *a, const char *b) {
    while (*a && *b) {
        int x = tolower((unsigned char)*a), y = tolower((unsigned char)*b);
        if (x != y) return x < y ? -1 : 1;
        a++; b++;
    }
    if (*a) return 1;
    if (*b) return -1;
    return 0;
}

/* Compares two products on the chosen field and order. Counts every call. */
int compareProducts(const struct Product *a, const struct Product *b) {
    int r;
    compareCount++;
    if (sortField == 1)      r = (a->price  > b->price)  - (a->price  < b->price);
    else if (sortField == 2) r = (a->rating > b->rating) - (a->rating < b->rating);
    else if (sortField == 3) r = compareText(a->name, b->name);
    else                     r = (a->id > b->id) - (a->id < b->id);
    return r * sortDir;
}

void bubbleSortArray(struct Product arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int swapped = 0;
        for (int j = 0; j < n - i - 1; j++) {
            if (compareProducts(&arr[j], &arr[j + 1]) > 0) {
                swapProducts(&arr[j], &arr[j + 1]);
                moveCount++;
                swapped = 1;
            }
        }
        if (!swapped) break;
    }
}

void selectionSortArray(struct Product arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int best = i;
        for (int j = i + 1; j < n; j++) {
            if (compareProducts(&arr[j], &arr[best]) < 0) best = j;
        }
        if (best != i) {
            swapProducts(&arr[i], &arr[best]);
            moveCount++;
        }
    }
}

void mergeArray(struct Product arr[], int low, int mid, int high) {
    struct Product temp[MAX_PRODUCTS];
    int i = low, j = mid + 1, k = low;
    while (i <= mid && j <= high) {
        if (compareProducts(&arr[i], &arr[j]) <= 0) temp[k++] = arr[i++];
        else temp[k++] = arr[j++];
    }
    while (i <= mid)  temp[k++] = arr[i++];
    while (j <= high) temp[k++] = arr[j++];
    for (i = low; i <= high; i++) { arr[i] = temp[i]; moveCount++; }
}

void mergeSortArray(struct Product arr[], int low, int high) {
    if (low < high) {
        int mid = low + (high - low) / 2;
        mergeSortArray(arr, low, mid);
        mergeSortArray(arr, mid + 1, high);
        mergeArray(arr, low, mid, high);
    }
}

/* Asks for a number between min and max and repeats until it is valid. */
int readChoice(const char *prompt, int min, int max) {
    while (1) {
        int v = readInt(prompt);
        if (v >= min && v <= max) return v;
        printf("Please enter a number from %d to %d.\n", min, max);
    }
}

/* Finds every different category (ignoring upper/lower case), then lets
   you pick a field, order and algorithm for each one separately. */
void sortEachCategory(void) {
    char cats[MAX_PRODUCTS][30];
    int numCats = 0;

    if (count == 0) {
        printf("\nNo products to sort.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        int known = 0;
        for (int c = 0; c < numCats; c++) {
            if (compareText(cats[c], products[i].category) == 0) { known = 1; break; }
        }
        if (!known) {
            strcpy(cats[numCats], products[i].category);
            numCats++;
        }
    }

    printf("\nFound %d categories. Each one is sorted on its own.\n", numCats);
    printf("(The main product list is not changed.)\n");

    for (int c = 0; c < numCats; c++) {
        struct Product group[MAX_PRODUCTS];
        int g = 0;
        const char *algoNames[] = {"", "Bubble Sort", "Selection Sort", "Merge Sort"};

        for (int i = 0; i < count; i++) {
            if (compareText(products[i].category, cats[c]) == 0) group[g++] = products[i];
        }

        printf("\n===== Category: %s (%d product%s) =====\n", cats[c], g, g == 1 ? "" : "s");

        if (g < 2) {
            printHeader();
            for (int i = 0; i < g; i++) printProduct(group[i]);
            printf("Only one product, nothing to sort.\n");
            continue;
        }

        printf("Sort by: 1. Price  2. Rating  3. Name  4. Product ID\n");
        sortField = readChoice("Choose field (1-4): ", 1, 4);
        printf("Order: 1. Low to high  2. High to low\n");
        sortDir = readChoice("Choose order (1-2): ", 1, 2) == 1 ? 1 : -1;
        printf("Algorithm: 1. Bubble Sort  2. Selection Sort  3. Merge Sort\n");
        int algo = readChoice("Choose algorithm (1-3): ", 1, 3);

        compareCount = 0;
        moveCount = 0;
        if (algo == 1)      bubbleSortArray(group, g);
        else if (algo == 2) selectionSortArray(group, g);
        else                mergeSortArray(group, 0, g - 1);

        printHeader();
        for (int i = 0; i < g; i++) printProduct(group[i]);
        printf("%s: %ld comparisons, %ld %s\n", algoNames[algo], compareCount, moveCount,
               algo == 3 ? "writes back to the array" : "swaps");
    }
}

/* Sample data from the project report */
void loadSampleData(void) {
    struct Product sample[] = {
        {101, "HP Laptop",       "Electronics", 55000, 4.5f, 20},
        {102, "Samsung Phone",   "Electronics", 25000, 4.6f, 15},
        {103, "Boat Headphones", "Electronics",  1500, 4.2f, 30},
        {104, "Nike Shoes",      "Fashion",      4000, 4.4f, 25},
        {105, "College Bag",     "Fashion",      1200, 4.0f, 40}
    };
    for (int i = 0; i < 5; i++) products[count++] = sample[i];
}

int main(void) {
    loadSampleData();

    while (1) {
        printf("\n========== E-COMMERCE PRODUCT SYSTEM ==========\n");
        printf("1. Add Product\n");
        printf("2. Display Products\n");
        printf("3. Search Product\n");
        printf("4. Sort Products\n");
        printf("5. Update Product\n");
        printf("6. Delete Product\n");
        printf("7. Sort Each Category Separately\n");
        printf("8. Exit\n");
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: addProduct();      break;
            case 2: displayProducts(); break;
            case 3: searchMenu();      break;
            case 4: sortMenu();        break;
            case 5: updateProduct();   break;
            case 6: deleteProduct();   break;
            case 7: sortEachCategory(); break;
            case 8: printf("\nThank you! Exiting program.\n"); return 0;
            default: printf("\nInvalid choice. Please try again.\n");
        }
    }
}
