#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

 #ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// Function to clear the terminal

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Function to clear user's input

void clearInputBuffer() {
    while (getchar() != '\n');
}

// View Discount Coupons

#define MAX_ROWS 100
#define MAX_LINE_LENGTH 512
#define MAX_RECORDS 1000
#define FLAG_FILE "flag.csv"

typedef struct {
    char code[20];
    int discount;
    char condition[50];
    int minSpend;
} DiscountCoupon;

typedef struct {
    char id[20];
    char name[50];
    int quantity;
    char unit[20];
    char restock[20];  
    char expire[20];   
} Stock;

typedef struct {
    char id[10];
    char name[50];
    int quantity;
    char unit[10];
    char restock[20];
    char expire[20];
} Item;

typedef struct {
    char date[20];
    char productName[50];
    int quantity;
    float totalPrice;
} SaleRecord;

// Declare Functions for Restock

int readCSV(const char *filename, Item items[], int *rowCount);
void writeCSV(const char *filename, Item items[], int rowCount);
int getUniqueIngredients(Item items[], int rowCount, char uniqueNames[][50]);
void displayCombinedStock(Item items[], int rowCount, char uniqueNames[][50], int uniqueCount);
void restockMenu(Item items[], int *rowCount, char uniqueNames[][50], int uniqueCount);


// End of Declaring Functions for Restock
int readDiscountCoupons(const char *filename, DiscountCoupon coupons[], int *rowCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int isHeader = 1;
    *rowCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue;
        }

        sscanf(line, "%[^,],%d,%[^,],%d",
               coupons[*rowCount].code,
               &coupons[*rowCount].discount,
               coupons[*rowCount].condition,
               &coupons[*rowCount].minSpend);

        (*rowCount)++;
    }

    fclose(file);
    return 1;
}

// View Reports

SaleRecord sales[MAX_RECORDS];
int salesCount = 0;

// Function to load sales log from CSV file
void loadSalesLog(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open file %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (salesCount >= MAX_RECORDS) {
            printf("Warning: Maximum sales records reached.\n");
            break;
        }
        
        SaleRecord *record = &sales[salesCount];
        sscanf(line, "%[^,],%[^,],%d,%f", record->date, record->productName, &record->quantity, &record->totalPrice);
        salesCount++;
    }

    fclose(file);
}

// Function to filter sales by product name 
// Utility function to convert a string to lowercase
void toLowerCaseViewReport(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Updated function to filter sales by product name (case-insensitive)
void filterSalesByProductName(const char *productName) {
    clearScreen();
    printf("============================================================================================================\n");
    printf("                                        FILTER BY PRODUCT NAME                       \n");
    printf("============================================================================================================\n");
    printf("\nSales Report for Product: %s\n", productName);
    printf("%-20s %-50s %15s %15s\n", "Date", "Product Name", "Quantity", "Total Price");
    printf("------------------------------------------------------------------------------------------------------------\n");

    int found = 0;
    int totalQuantity = 0;  // To track total quantity
    float totalPrice = 0.0; // To track total price

    char lowerProductName[50];
    strncpy(lowerProductName, productName, 50);
    toLowerCaseViewReport(lowerProductName);  // Convert the search term to lowercase

    for (int i = 0; i < salesCount; i++) {
        char lowerSalesProductName[50];
        strncpy(lowerSalesProductName, sales[i].productName, 50);
        toLowerCaseViewReport(lowerSalesProductName);  // Convert product name in the CSV to lowercase

        if (strstr(lowerSalesProductName, lowerProductName)) {
            printf("%-20s %-50s %15d %15.2f\n",
                   sales[i].date, sales[i].productName, sales[i].quantity, sales[i].totalPrice);
            totalQuantity += sales[i].quantity;
            totalPrice += sales[i].totalPrice;
            found = 1;
        }
    }

    if (found) {
        printf("------------------------------------------------------------------------------------------------------------\n");
        printf("%-70s %16d %15.2f\n", "Total:", totalQuantity, totalPrice);
    } else {
        printf("No sales records found for the product name '%s'.\n", productName);
    }

    printf("\nPress Enter to return to the menu...");
    getchar();
    getchar();
}



void filterSalesByPriceRange() {
    clearScreen();  // To clear the screen and provide a fresh start for the UI
    printf("======================================================================================================\n");
    printf("                                        FILTER BY PRICE RANGE                        \n");
    printf("======================================================================================================\n");
    printf("------------------------------------------------------------------------------------------------------\n");

    float minPrice, maxPrice;

    // Asking for minimum price with a clearer UI and numbering
    printf("Please enter the price range for filtering sales:\n");
    printf("------------------------------------------------------------------------------------------------------\n");

    // Asking for minimum price
    printf("   [1] Minimum Price: ");
    while (scanf("%f", &minPrice) != 1) {
        printf("Invalid input. Please enter a valid number for minimum price: ");
        while(getchar() != '\n'); // Clear input buffer
    }

    // Asking for maximum price
    printf("   [2] Maximum Price: ");
    while (scanf("%f", &maxPrice) != 1) {
        printf("Invalid input. Please enter a valid number for maximum price: ");
        while(getchar() != '\n'); // Clear input buffer
    }

    // Validation for valid price range (minPrice should not be greater than maxPrice)
    while (minPrice > maxPrice) {
        printf("\nERROR: The minimum price cannot be greater than the maximum price.\n");
        printf("Please re-enter the prices.\n");

        // Re-enter prices
        printf("   [1] Minimum Price: ");
        while (scanf("%f", &minPrice) != 1) {
            printf("Invalid input. Please enter a valid number for minimum price: ");
            while(getchar() != '\n'); // Clear input buffer
        }

        printf("   [2] Maximum Price: ");
        while (scanf("%f", &maxPrice) != 1) {
            printf("Invalid input. Please enter a valid number for maximum price: ");
            while(getchar() != '\n'); // Clear input buffer
        }
    }

    // Confirmation message and price range details
    printf("\n=====================================================================================================\n");
    printf("   You have selected the following price range:\n");
    printf("   Minimum Price: %.2f  |  Maximum Price: %.2f\n", minPrice, maxPrice);
    printf("   Press Enter to confirm and proceed.\n");
    printf("=====================================================================================================\n");

    // Wait for the user to confirm
    getchar();  // To capture the newline character after scanf
    getchar();  // Wait for the user to press Enter before proceeding

    printf("======================================================================================================\n");
    printf("                                        FILTER BY PRICE RANGE                        \n");
    printf("======================================================================================================\n");
    // Displaying results in a table-like format
    printf("------------------------------------------------------------------------------------------------------\n");
    printf("Sales Report for Price Range: %.2f - %.2f\n", minPrice, maxPrice);
    printf("------------------------------------------------------------------------------------------------------\n");

    // Printing column headers
    printf("%-20s %-50s %-15s %-15s\n", "Date", "Product Name", "Quantity", "Total Price");
    printf("------------------------------------------------------------------------------------------------------\n");

    int found = 0;

    // Loop through sales and filter by price range
    for (int i = 0; i < salesCount; i++) {
        if (sales[i].totalPrice >= minPrice && sales[i].totalPrice <= maxPrice) {
            printf("%-20s %-50s %-15d %-15.2f\n",
                   sales[i].date, sales[i].productName, sales[i].quantity, sales[i].totalPrice);
            found = 1;
        }
    }

    // If no records found
    if (!found) {
        printf("No sales records found for the price range %.2f - %.2f.\n", minPrice, maxPrice);
    }

    printf("\nPress Enter to return to the menu...");
    getchar();  // This is the only getchar needed to capture the Enter key press
}






// Function to extract the date part from "YYYY-MM-DD HH:MM:SS"
void extractDate(const char *dateTime, char *datePart) {
    strncpy(datePart, dateTime, 10);
    datePart[10] = '\0'; // Ensure null-termination
}

// Function to extract the time part from "YYYY-MM-DD HH:MM:SS"
void extractTime(const char *dateTime, char *timePart) {
    strncpy(timePart, dateTime + 11, 8); // Extract from position 11
    timePart[8] = '\0'; // Ensure null-termination
}

// Sort dates function
void sortDates(char dates[][20], int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (strcmp(dates[i], dates[j]) > 0) {
                char temp[20];
                strcpy(temp, dates[i]);
                strcpy(dates[i], dates[j]);
                strcpy(dates[j], temp);
            }
        }
    }
}

// Updated calculateDailySummary function
void calculateDailySummary() {
    clearScreen();
    printf("========================================================================================================================\n");
    printf("                                DAILY SALES AND RESTOCK SUMMARY BY DATE & TIME                              \n");
    printf("========================================================================================================================\n");

    char uniqueDates[MAX_RECORDS][20];
    int uniqueDatesCount = 0;

    // Collect unique dates
    for (int i = 0; i < salesCount; i++) {
        char datePart[11];
        extractDate(sales[i].date, datePart);

        int exists = 0;
        for (int j = 0; j < uniqueDatesCount; j++) {
            if (strcmp(datePart, uniqueDates[j]) == 0) {
                exists = 1;
                break;
            }
        }

        if (!exists) {
            strcpy(uniqueDates[uniqueDatesCount], datePart);
            uniqueDatesCount++;
        }
    }

    // Sort dates in ascending order
    sortDates(uniqueDates, uniqueDatesCount);

    // Print report grouped by dates
    for (int i = 0; i < uniqueDatesCount; i++) {
        printf("\nDate: %s\n", uniqueDates[i]);
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        // Print header with fixed widths
        printf("%-10s | %-15s | %-42s | %-22s | %-22s\n", 
               "Time", "Event Type", "Product Name", "Quantity", "Total Price");
        printf("------------------------------------------------------------------------------------------------------------------------\n");

        int dayTotalQuantity = 0;
        float dayTotalSales = 0.0;

        for (int j = 0; j < salesCount; j++) {
            char datePart[11];
            char timePart[9];
            extractDate(sales[j].date, datePart);
            extractTime(sales[j].date, timePart);

            if (strcmp(datePart, uniqueDates[i]) == 0) {
                const char *eventType = (sales[j].totalPrice == 0) ? "Restock" : "Sale";
                const char *priceDisplay = (sales[j].totalPrice == 0) ? "N/A" : "";

                // Print data with the same widths as the header
                if (sales[j].totalPrice == 0) {
                    // For restocks, show "N/A" in the price column
                    printf("%-10s | %-15s | %-42s | %-22d | %-22s\n",
                           timePart,
                           eventType,
                           sales[j].productName,
                           sales[j].quantity,
                           priceDisplay);
                } else {
                    // For sales, print the total price
                    printf("%-10s | %-15s | %-42s | %-22d | %-22.2f\n",
                           timePart,
                           eventType,
                           sales[j].productName,
                           sales[j].quantity,
                           sales[j].totalPrice);
                }

                dayTotalQuantity += sales[j].quantity;
                dayTotalSales += sales[j].totalPrice;
            }
        }

        printf("========================================================================================================================\n");
        // Adjust total row to align with header columns
        printf("%-10s | %-15s | %-42s | %-22d | %-22.2f\n", 
               "Total:", "", "", dayTotalQuantity, dayTotalSales);
        printf("========================================================================================================================\n");
    }

    printf("\nPress Enter to return to the menu...");
    getchar();
    getchar();
}





// Function to find the most sold product in a day
void findMostSoldProductInDay(char *date) {
    clearScreen();
    printf("========================================================\n");
    printf("                  MOST SOLD PRODUCT IN A DAY            \n");
    printf("========================================================\n");
    printf("\nSales Report for Date: %s\n", date);

    int maxQuantity = 0;
    char mostSoldProduct[50] = "";
    int found = 0;

    // Iterate over all sales records
    for (int i = 0; i < salesCount; i++) {
        // Extract the date part (first 10 characters) from the full date-time string
        char saleDate[11];
        strncpy(saleDate, sales[i].date, 10); // Copy the first 10 characters (YYYY-MM-DD)
        saleDate[10] = '\0'; // Null-terminate the string

        // Check if the sale date matches the input date
        if (strcmp(saleDate, date) == 0) {
            found = 1;

            // Sum quantities of the same product on the same day
            int totalQuantity = sales[i].quantity;

            // Check if we have already seen this product for the given date
            for (int j = 0; j < i; j++) {
                char compareSaleDate[11];
                strncpy(compareSaleDate, sales[j].date, 10);
                compareSaleDate[10] = '\0';

                // If same product and same date, add quantities
                if (strcmp(saleDate, compareSaleDate) == 0 && strcmp(sales[i].productName, sales[j].productName) == 0) {
                    totalQuantity += sales[j].quantity;
                }
            }

            // Update max quantity and most sold product if needed
            if (totalQuantity > maxQuantity) {
                maxQuantity = totalQuantity;
                strcpy(mostSoldProduct, sales[i].productName);
            }
        }
    }

    if (found) {
        printf("Most Sold Product: %s\n", mostSoldProduct);
        printf("Quantity Sold: %d\n", maxQuantity);
    } else {
        printf("No sales records found for the date '%s'.\n", date);
    }

    printf("\nPress Enter to return to the menu...");
    getchar();
    getchar();
}






// Function to find the most sold product in a month
void findMostSoldProductInMonth(const char *specificMonth) {
    clearScreen();
    printf("========================================================\n");
    printf("                 MOST SOLD PRODUCT IN A MONTH           \n");
    printf("========================================================\n");
    printf("\nSales Report for Month: %s\n", specificMonth);

    char mostSoldProduct[50] = "";
    int maxQuantity = 0;
    int monthlyQuantity[MAX_RECORDS] = {0};
    char products[MAX_RECORDS][50];
    int productCount = 0;

    for (int i = 0; i < salesCount; i++) {
        if (strncmp(sales[i].date, specificMonth, 7) == 0) {
            int productIndex = -1;
            for (int j = 0; j < productCount; j++) {
                if (strcmp(products[j], sales[i].productName) == 0) {
                    productIndex = j;
                    break;
                }
            }

            if (productIndex == -1) {
                strcpy(products[productCount], sales[i].productName);
                productIndex = productCount;
                productCount++;
            }

            monthlyQuantity[productIndex] += sales[i].quantity;
            if (monthlyQuantity[productIndex] > maxQuantity) {
                maxQuantity = monthlyQuantity[productIndex];
                strcpy(mostSoldProduct, products[productIndex]);
            }
        }
    }

    if (maxQuantity > 0) {
        printf("Most Sold Product: %s\n", mostSoldProduct);
        printf("Quantity Sold: %d\n", maxQuantity);
    } else {
        printf("No sales records found for the month '%s'.\n", specificMonth);
    }

    printf("\nPress Enter to return to the menu...");
    getchar();
    getchar();
}

// Main function to display menu and interact with the user
// Function to ask for product name with a clean UI
void askProductName(char *productName) {
    clearScreen();
    printf("============================================================================================================\n");
    printf("                                      SALES REPORT - SEARCH BY PRODUCT NAME                                \n");
    printf("============================================================================================================\n");
    printf("\nEnter the product name you want to search for (case-insensitive):\n");
    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("Product Name: ");
    scanf(" %[^\n]", productName); // Input for product name
    printf("------------------------------------------------------------------------------------------------------------\n");
    printf("Searching for sales records matching '%s'...\n", productName);
    printf("\nPress Enter to continue...");
    getchar(); // Consume previous newline
    getchar(); // Wait for user confirmation before showing results
}

void displayMenu() { 
    int choice;
    char productName[50];
    float minPrice, maxPrice;
    char specificDate[20];
    char specificMonth[10];
    char question[100];

    do {
        clearScreen();
        printf("========================================================\n");
        printf("                   SALES REPORT SYSTEM                  \n");
        printf("========================================================\n");
        printf("1. Filter Sales by Product Name\n");
        printf("2. Filter Sales by Price Range\n");
        printf("3. Daily Sales Summary\n");
        printf("4. Most Sold Product in a Day\n");
        printf("5. Most Sold Product in a Month\n");
        printf("6. Exit\n");
        printf("========================================================\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Clear the input buffer after reading an integer to prevent double input issue
        getchar();

        switch (choice) {
            case 1:
                askProductName(productName); // Ask for product name
                filterSalesByProductName(productName);
                break;
            case 2:
                // Handle price range input here
                filterSalesByPriceRange(minPrice, maxPrice);
                break;
            case 3:
                calculateDailySummary();  // Summarize daily sales
                break;
            case 4:
                clearScreen();
                printf("Enter Specific Date (YYYY-MM-DD): ");
                // Use a single scanf for date input
                scanf(" %[^\n]", specificDate);  // Input the date
                findMostSoldProductInDay(specificDate);
                break;
            case 5:
                clearScreen();
                printf("Enter Specific Month (YYYY-MM): ");
                // Input the month
                scanf(" %[^\n]", specificMonth);
                findMostSoldProductInMonth(specificMonth);
                break;
            case 6:
                clearScreen();
                printf("\nExiting the program. Goodbye!\n");
                break;
            default:
                clearScreen();
                printf("\nInvalid choice! Please try again.\n");
                getchar();  // Wait for user to press Enter before continuing
        }
    } while (choice != 6);
}



//Logging

#define MAX_UNIT_LENGTH 20   // Maximum length for unit strings
#define MAX_ITEMS 1000
#define MAX_PRODUCTS 100
#define MAX_EVENTS 200
// Define the maximum sizes for the arrays
#define MAX_STOCK 100
#define MAX_SALES 100

// Define StockItem structure
typedef struct {
    char id[20];
    char name[50];
    int quantity;
    char unit[MAX_UNIT_LENGTH];
    char restockDate[20];
    char expireDate[20];
} StockItemLogging;

#define RESTOCK_EVENT 1
#define SALES_EVENT 2

// Define Stock structure
typedef struct {
    char id[20];
    char name[50];
    int quantity;
    char unit[20];
    char restock_date[20];
    char expire_date[20];
} StockLogging;

// Define SalesLog structure
typedef struct {
    char timestamp[20];
    char product_name[50];
    int quantity_sold;
    int total_price;
} SalesLogging;

// Define Event structure
typedef struct {
    char date[20];
    int type;  // e.g., RESTOCK_EVENT = 1, SALES_EVENT = 2
    char product_name[50];
    int quantity;
    int total_price;
} EventLogging;

StockItemLogging* stockItems = NULL;
int stockCountLogging = 0;

// void clearScreen() {
// #ifdef _WIN32
//     system("cls");
// #else
//     system("clear");
// #endif
// }

int dateToInt(const char* date) {
    int year, month, day;
    if (sscanf(date, "%d-%d-%d", &year, &month, &day) != 3) {
        fprintf(stderr, "Error: Invalid date format '%s'. Expected YYYY-MM-DD.\n", date);
        return -1;  // Return an invalid value for error
    }
    return year * 10000 + month * 100 + day;
}

// Load stock data from a CSV file
// Load stock data from CSV file
void loadStockLogging(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Failed to open %s\n", filename);
        return;
    }

    char line[MAX_LINE_LENGTH];
    stockCountLogging = 0;
    free(stockItems);
    stockItems = NULL;

    fgets(line, sizeof(line), file);  // Skip the header line

    // Now, we start reading actual data from the file
    while (fgets(line, sizeof(line), file)) {
        StockItemLogging temp;
        if (sscanf(line, "%[^,],%[^,],%d,%[^,],%[^,],%s", temp.id, temp.name, &temp.quantity,
                   temp.unit, temp.restockDate, temp.expireDate) < 6) {
            printf("Skipping invalid line: %s", line);
            continue;
        }

        stockItems = realloc(stockItems, sizeof(StockItemLogging) * (stockCountLogging + 1));
        stockItems[stockCountLogging++] = temp;
    }

    fclose(file);
}


// Save stock data to a CSV file
void saveStockLogging(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Failed to open %s for writing\n", filename);
        return;
    }

    fprintf(file, "id,name,quantity,unit,restock,expire\n");
    for (int i = 0; i < stockCountLogging; i++) {
        fprintf(file, "%s,%s,%d,%s,%s,%s\n",
                stockItems[i].id,
                stockItems[i].name,
                stockItems[i].quantity,
                stockItems[i].unit,
                stockItems[i].restockDate,
                stockItems[i].expireDate);
    }

    fclose(file);
}

// Comparison function for sorting by restock date (ascending)
int compareByRestockDate(const void* a, const void* b) {
    const StockItemLogging* itemA = (const StockItemLogging*)a;
    const StockItemLogging* itemB = (const StockItemLogging*)b;

    int dateA = dateToInt(itemA->restockDate);
    int dateB = dateToInt(itemB->restockDate);

    return dateA - dateB;
}

// Validate numeric input within a range
int getValidChoice(int min, int max) {
    int choice;
    while (1) {
        if (scanf("%d", &choice) == 1 && choice >= min && choice <= max) {
            return choice;
        }
        printf("Invalid input. Please enter a number between %d and %d: ", min, max);
        while (getchar() != '\n');  // Clear invalid input
    }
}

// Display product restocks in a tabular format
void showProductRestocks() {
    clearScreen();  // Ensure we clear the screen before displaying product restocks
    printf("===============================================================================\n");
    printf("                              PRODUCT RESTOCKS\n");
    printf("===============================================================================\n");

    loadStockLogging("Stock.csv");

    if (stockCountLogging == 0) {
        printf("No stock data available.\n");
        printf("===============================================================================\n");
        printf("Press Enter to return to the menu...");
        getchar();
        getchar();  // Wait for user input
        clearScreen();  // Clear the screen before returning
        return;
    }

    // Find the maximum length for each column
    int maxIdLen = 0, maxNameLen = 0, maxUnitLen = 0, maxRestockLen = 0, maxExpireLen = 0;
    for (int i = 0; i < stockCountLogging; i++) {
        maxIdLen = (strlen(stockItems[i].id) > maxIdLen) ? strlen(stockItems[i].id) : maxIdLen;
        maxNameLen = (strlen(stockItems[i].name) > maxNameLen) ? strlen(stockItems[i].name) : maxNameLen;
        maxUnitLen = (strlen(stockItems[i].unit) > maxUnitLen) ? strlen(stockItems[i].unit) : maxUnitLen;
        maxRestockLen = (strlen(stockItems[i].restockDate) > maxRestockLen) ? strlen(stockItems[i].restockDate) : maxRestockLen;
        maxExpireLen = (strlen(stockItems[i].expireDate) > maxExpireLen) ? strlen(stockItems[i].expireDate) : maxExpireLen;
    }

    // Print the header with dynamic column widths
    printf("%-15s %-*s %-10s %-*s %-*s %-*s\n", 
        "ID", maxNameLen + 2, "Name", "Quantity", maxUnitLen + 2, "Unit", maxRestockLen + 2, "Restock", maxExpireLen + 2, "Expire");
    printf("-------------------------------------------------------------------------------\n");

    qsort(stockItems, stockCountLogging, sizeof(StockItemLogging), compareByRestockDate);

    for (int i = 0; i < stockCountLogging; i++) {
        printf("%-15s %-*s %-10d %-*s %-*s %-*s\n",
               stockItems[i].id,
               maxNameLen + 2, stockItems[i].name,
               stockItems[i].quantity,
               maxUnitLen + 2, stockItems[i].unit,
               maxRestockLen + 2, stockItems[i].restockDate,
               maxExpireLen + 2, stockItems[i].expireDate);
    }

    printf("===============================================================================\n");
    printf("Press Enter to return to the menu...");
    getchar();
    getchar();  // Wait for user input
    clearScreen();  // Clear the screen before returning
}


typedef struct {
    char dateTime[20];
    char menuItem[100];
    int amount;
    double price;
} PurchaseLogging;

// Declare purchases array and count globally
PurchaseLogging purchases[MAX_ITEMS];
int purchaseCount = 0;


long long dateTimeToInt(const char* dateTime) {
    int year, month, day, hour, minute, second;
    if (sscanf(dateTime, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second) != 6) {
        fprintf(stderr, "Error: Invalid date/time format '%s'.\n", dateTime);
        return -1;  // Invalid date/time
    }
    return (long long)year * 10000000000LL + month * 100000000 + day * 1000000 +
           hour * 10000 + minute * 100 + second;
}

// Comparison function for sorting by date/time (newest first)
int compareByDateTime(const void* a, const void* b) {
    const PurchaseLogging* pa = (const PurchaseLogging*)a;
    const PurchaseLogging* pb = (const PurchaseLogging*)b;

    long long dateA = dateTimeToInt(pa->dateTime);
    long long dateB = dateTimeToInt(pb->dateTime);

    return (dateB > dateA) - (dateB < dateA);  // Descending order
}

void showCustomerPurchases() {
    clearScreen();  // Ensure we clear the screen before displaying customer purchases
    printf("===========================================================================================\n");
    printf("                                    CUSTOMER PURCHASES\n");
    printf("===========================================================================================\n");

    FILE *file = fopen("sales_log.csv", "r");
    if (!file) {
        printf("Failed to open sales_log.csv\n");
        printf("===========================================================================================\n");
        printf("Press Enter to return to the menu...");
        getchar();
        clearScreen();  // Clear the screen before returning
        return;
    }

    char line[MAX_LINE_LENGTH];
    purchaseCount = 0;

    // Skip the header line if it exists
    fgets(line, sizeof(line), file);

    // Read the file line by line
    while (fgets(line, sizeof(line), file) && purchaseCount < MAX_ITEMS) {
        char dateTime[20], menuItem[100];
        int amount;
        double price;

        // Parse the CSV line
        if (sscanf(line, "%[^,],%[^,],%d,%lf", dateTime, menuItem, &amount, &price) == 4) {
            strcpy(purchases[purchaseCount].dateTime, dateTime);
            strcpy(purchases[purchaseCount].menuItem, menuItem);
            purchases[purchaseCount].amount = amount;
            purchases[purchaseCount].price = price;
            purchaseCount++;
        } else {
            printf("Skipping invalid line: %s", line);
        }
    }

    fclose(file);

    // Sort the purchases by date/time
    qsort(purchases, purchaseCount, sizeof(PurchaseLogging), compareByDateTime);

    if (purchaseCount == 0) {
        printf("No customer purchases found.\n");
    } else {
        double totalPurchase = 0.0;
        int totalAmount = 0;

        // Print the header with column names
        printf("%-20s %-50s %-8s %-10s\n", "Date/Time", "Menu Item", "Amount", "Price");
        printf("-------------------------------------------------------------------------------------------\n");

        // Display the sorted data and calculate totals
        for (int i = 0; i < purchaseCount; i++) {
            printf("%-20s %-50s %-8d %-10.2f\n",
                   purchases[i].dateTime, purchases[i].menuItem, purchases[i].amount, purchases[i].price);
            totalPurchase += purchases[i].price;
            totalAmount += purchases[i].amount;
        }

        printf("-------------------------------------------------------------------------------------------\n");
        // Print totals under their respective columns
        printf("%-72s%-8d %-10.2f\n", "Total:", totalAmount, totalPurchase);
    }

    printf("===========================================================================================\n");
    printf("Press Enter to return to the menu...");
    getchar();
    getchar();  // Wait for user input
    clearScreen();  // Clear the screen before returning
}

int compare_dates(const void *a, const void *b) {
    EventLogging *eventA = (EventLogging *)a;
    EventLogging *eventB = (EventLogging *)b;
    return strcmp(eventA->date, eventB->date); // Lexicographical comparison of dates
}

// Trim function to remove leading/trailing whitespace
void trim(char* str) {
    int i = 0, j = strlen(str) - 1;

    while (isspace((unsigned char)str[i])) i++;  // Leading whitespace
    while (j > i && isspace((unsigned char)str[j])) j--;  // Trailing whitespace

    memmove(str, str + i, j - i + 1);
    str[j - i + 1] = '\0';
}

// Function to parse stock CSV file
void parse_stock_csv(const char *filename, StockLogging stock[], int *stock_count, EventLogging events[], int *event_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening stock CSV file.\n");
        exit(1);
    }
    
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\0' || line[0] == '\n') continue;  // Skip empty lines
        sscanf(line, "%[^,],%[^,],%d,%[^,],%[^,],%s",
            stock[*stock_count].id, stock[*stock_count].name, &stock[*stock_count].quantity,
            stock[*stock_count].unit, stock[*stock_count].restock_date, stock[*stock_count].expire_date);
        
        // Record a restock event
        strcpy(events[*event_count].date, stock[*stock_count].restock_date);
        events[*event_count].type = RESTOCK_EVENT;
        strcpy(events[*event_count].product_name, stock[*stock_count].name);
        events[*event_count].quantity = stock[*stock_count].quantity;
        (*event_count)++;
        
        (*stock_count)++;
    }
    fclose(file);
}

// Function to parse sales log CSV file
void parse_sales_log_csv(const char *filename, SalesLogging sales_log[], int *sales_count, EventLogging events[], int *event_count) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening sales log CSV file.\n");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '\0' || line[0] == '\n') continue;  // Skip empty lines
        sscanf(line, "%[^,],%[^,],%d,%d",
            sales_log[*sales_count].timestamp, sales_log[*sales_count].product_name,
            &sales_log[*sales_count].quantity_sold, &sales_log[*sales_count].total_price);
        
        // Record a sales event
        strcpy(events[*event_count].date, sales_log[*sales_count].timestamp);
        events[*event_count].type = SALES_EVENT;
        strcpy(events[*event_count].product_name, sales_log[*sales_count].product_name);
        events[*event_count].quantity = sales_log[*sales_count].quantity_sold;
        events[*event_count].total_price = sales_log[*sales_count].total_price;
        (*event_count)++;
        
        (*sales_count)++;
    }
    fclose(file);
}

// Function to generate and display the report
void generate_stock_sales_report(void) {
    StockLogging stock[MAX_PRODUCTS];
    SalesLogging sales_log[MAX_PRODUCTS];
    EventLogging events[MAX_EVENTS];
    int stock_count = 0, sales_count = 0, event_count = 0;

    // Parse the CSV files
    parse_stock_csv("Stock.csv", stock, &stock_count, events, &event_count);
    parse_sales_log_csv("sales_log.csv", sales_log, &sales_count, events, &event_count);

    // Sort events by date
    qsort(events, event_count, sizeof(EventLogging), compare_dates);
    
    // Display the report header
    printf("===========================================================================================\n");
    printf("                                    Stock and Sales Report      \n");
    printf("===========================================================================================\n");
    
    printf("===========================================================================================\n");
    printf("                                    Restock & Sales by Date         \n");
    printf("===========================================================================================\n");

    char current_date[20] = "";
    
    // Iterate through the events and print them grouped by date
    for (int i = 0; i < event_count; i++) {
        // Print the date header once per group
        if (strcmp(events[i].date, current_date) != 0) {
            strcpy(current_date, events[i].date);
            printf("\nDate: %s\n", current_date);
            printf("-------------------------------------------------------------------------------------------\n");
            printf("Event Type    | Product Name                                 | Quantity | Total Price\n");
            printf("-------------------------------------------------------------------------------------------\n");
        }

        // Print the event details with adjusted formatting
        if (events[i].type == RESTOCK_EVENT) {
            printf("Restock       | %-44s | %-8d | %-12s\n", events[i].product_name, events[i].quantity, "N/A");
        } else if (events[i].type == SALES_EVENT) {
            printf("Sale          | %-44s | %-8d | %-12d\n", events[i].product_name, events[i].quantity, events[i].total_price);
        }
    }
    
    printf("===========================================================================================\n");
    printf("                                        End of Report             \n");
    printf("===========================================================================================\n");
}

// Main menu
void mainLoggingMenu() {
    int choice;

    do {
        clearScreen();  // Optional: clear screen only if needed before showing the menu
        printf("===============================================================================\n");
        printf("                             Viewing Logs\n");
        printf("===============================================================================\n");
        printf("1. Product Restocks\n");
        printf("2. Customer Purchases\n");
        printf("3. Logging Report\n");  // Placeholder for Logging Report
        printf("4. Exit\n");  // Now, Exit is Case 4
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();  // to capture the newline character after scanf

        switch (choice) {
            case 1:
                clearScreen();  // Clear screen before showing Product Restocks
                showProductRestocks();
                break;
            case 2:
                clearScreen();  // Clear screen before showing Customer Purchases
                showCustomerPurchases();
                break;
            case 3:
                generate_stock_sales_report();  // Removed clearScreen() here
                printf("\nPress Enter to return to the menu...");
                getchar();  // Wait for user input before returning to the menu
                break;
            case 4:
                clearScreen();  // Clear screen before exiting
                printf("Exiting program...\n");
                break;
            default:
                clearScreen();
                printf("Invalid choice. Please enter again.\n");
                break;
        }
    } while (choice != 4);  // Exit the loop when user selects option 4
}

// Edit Coupon Function

void EditCoupon() {
    DiscountCoupon coupons[MAX_ROWS];
    int rowCount = 0;

    // Read existing coupons from file
    if (!readDiscountCoupons("Discount.csv", coupons, &rowCount)) {
        printf("Error reading the file.\n");
        return;
    }

    // Display available coupons
    clearScreen();
    printf("===================================================================================\n");
    printf("                            Edit Discount Coupons\n");
    printf("===================================================================================\n");
    printf("Available Coupons:\n");
    printf("Code            Discount [%%]      Condition                 Min Spend\n");
    printf("-----------------------------------------------------------------------------------\n");
    for (int i = 0; i < rowCount; i++) {
        printf("%-15s%-18d%-30s%d\n",
               coupons[i].code,
               coupons[i].discount,
               coupons[i].condition,
               coupons[i].minSpend);
    }

    // Prompt user to enter the coupon code to edit
    char couponCode[20];
    printf("\nEnter the coupon code to edit: ");
    fgets(couponCode, sizeof(couponCode), stdin);
    couponCode[strcspn(couponCode, "\n")] = '\0'; // Remove newline character

    // Find the coupon in the list
    int foundIndex = -1;
    for (int i = 0; i < rowCount; i++) {
        if (strcmp(coupons[i].code, couponCode) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Coupon not found.\n");
        printf("Press Enter to return to the menu...\n");
        getchar();
        return;
    }

    // Edit the selected coupon
    printf("\nEditing Coupon: %s\n", couponCode);
    printf("Enter new coupon code (current: %s): ", coupons[foundIndex].code);
    fgets(coupons[foundIndex].code, sizeof(coupons[foundIndex].code), stdin);
    coupons[foundIndex].code[strcspn(coupons[foundIndex].code, "\n")] = '\0';

    printf("Enter new discount percentage (current: %d): ", coupons[foundIndex].discount);
    scanf("%d", &coupons[foundIndex].discount);
    clearInputBuffer();

    printf("Enter new condition (current: %s): ", coupons[foundIndex].condition);
    fgets(coupons[foundIndex].condition, sizeof(coupons[foundIndex].condition), stdin);
    coupons[foundIndex].condition[strcspn(coupons[foundIndex].condition, "\n")] = '\0';

    printf("Enter new minimum spend (current: %d): ", coupons[foundIndex].minSpend);
    scanf("%d", &coupons[foundIndex].minSpend);
    clearInputBuffer();

    // Write the updated coupons back to the file
    FILE *file = fopen("Discount.csv", "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "Code,Discount,Condition,MinSpend\n");
    for (int i = 0; i < rowCount; i++) {
        fprintf(file, "%s,%d,%s,%d\n",
                coupons[i].code,
                coupons[i].discount,
                coupons[i].condition,
                coupons[i].minSpend);
    }

    fclose(file);
    printf("Coupon updated successfully!\n");
    printf("Press Enter to return to the menu...\n");
    getchar();
}


// View Discount Coupon Function

void ViewDiscountCoupon() {
    DiscountCoupon coupons[MAX_ROWS];
    int rowCount = 0;

    if (!readDiscountCoupons("Discount.csv", coupons, &rowCount)) {
        printf("Error reading the file.\n");
        return;
    }

    clearScreen();
    printf("===================================================================================\n");
    printf("                            AVAILABLE DISCOUNT COUPONS\n");
    printf("===================================================================================\n");
    printf("Code            Discount [%%]      Condition\n");
    printf("-----------------------------------------------------------------------------------\n");

    for (int i = 0; i < rowCount; i++) {
        printf("%-15s%-18d%-20s\n",
               coupons[i].code,
               coupons[i].discount,
               coupons[i].condition);
    }

    printf("===================================================================================\n");
    printf("Press Enter to return to the menu...\n");
    getchar();
}

// Create Discount Coupon

void CreateCoupon() {
    DiscountCoupon newCoupon;
    FILE *file = fopen("Discount.csv", "a");  // Open the file in append mode
    if (!file) {
        perror("Error opening file");
        return;
    }

    printf("Enter Coupon Code: ");
    fgets(newCoupon.code, sizeof(newCoupon.code), stdin);
    newCoupon.code[strcspn(newCoupon.code, "\n")] = '\0';  // Remove newline character

    printf("Enter Discount Percentage: ");
    scanf("%d", &newCoupon.discount);
    clearInputBuffer();  // Clear the input buffer after scanf

    printf("Enter Condition: ");
    fgets(newCoupon.condition, sizeof(newCoupon.condition), stdin);
    newCoupon.condition[strcspn(newCoupon.condition, "\n")] = '\0';  // Remove newline

    printf("Enter Minimum Spend (If there is no condition, enter 0): ");
    scanf("%d", &newCoupon.minSpend);
    clearInputBuffer();

    // Write new coupon to the file, with a newline after the entry
    fprintf(file, "%s,%d,%s,%d\n", newCoupon.code, newCoupon.discount, newCoupon.condition, newCoupon.minSpend);
    fclose(file);

    printf("Coupon created successfully!\n");
    printf("Press Enter to continue...");
    getchar();  // Wait for the user to press Enter
}

// Remove Discount Coupons

void DeleteCoupon() {
    DiscountCoupon coupons[MAX_ROWS];
    int rowCount = 0;

    if (!readDiscountCoupons("Discount.csv", coupons, &rowCount)) {
        printf("Error reading the file.\n");
        return;
    }

    char couponCode[20];
    int found = 0;

    printf("Enter the coupon code to delete: ");
    fgets(couponCode, sizeof(couponCode), stdin);
    couponCode[strcspn(couponCode, "\n")] = '\0';

    for (int i = 0; i < rowCount; i++) {
        if (strcmp(coupons[i].code, couponCode) == 0) {
            for (int j = i; j < rowCount - 1; j++) {
                coupons[j] = coupons[j + 1];
            }
            rowCount--;
            found = 1;
            break;
        }
    }

    if (found) {
        FILE *file = fopen("Discount.csv", "w");
        if (!file) {
            perror("Error opening file");
            return;
        }

        fprintf(file, "Code,Discount,Condition,MinSpend\n");

        for (int i = 0; i < rowCount; i++) {
            fprintf(file, "%s,%d,%s,%d\n",
                    coupons[i].code,
                    coupons[i].discount,
                    coupons[i].condition,
                    coupons[i].minSpend);
        }

        fclose(file);
        printf("Coupon deleted successfully!\n");
    } else {
        printf("Coupon not found.\n");
    }

    printf("Press Enter to continue...");
    getchar();
}



// Manage Coupon Menu

void ManageCouponsMenu() {
        int CouponChoice;
    clearScreen();
    printf("===================================================================================\n");
    printf("                            Manage Discount Coupons\n");
    printf("===================================================================================\n");
    printf("1. View Discount Coupons\n");
    printf("2. Create A Coupon\n");
    printf("3. Delete A Coupon\n");
    printf("4. Edit A Coupon\n");
    printf("5. Return to Owner Features\n");
    printf("===================================================================================\n");
    printf("Enter your choice: ");
        scanf("%d", &CouponChoice);
        clearInputBuffer();
        switch (CouponChoice) {
            case 1:
                ViewDiscountCoupon();
                ManageCouponsMenu();
                break;
            case 2:
                CreateCoupon();
                ManageCouponsMenu();
                break;
            case 3:
                DeleteCoupon();
                ManageCouponsMenu();
                break;
            case 4:
                EditCoupon();
                ManageCouponsMenu();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  
                return;
        }
}

// Create New Menu Item

void CreateNewMenuItem() {
    typedef struct {
        char name[100];
        double price;
        char description[100];
    } MenuItem;

    typedef struct {
        char name[50];
    } Ingredient;

    MenuItem newItem;
    Ingredient ingredients[MAX_ROWS];
    int ingredientCount = 0;
    double ingredientAmounts[MAX_ROWS] = {0};

    // Determine the next menu number
    int nextMenuNumber = 1; // Default to 1 if the file is empty

    FILE *menuFile = fopen("Menu.csv", "r");
    if (menuFile) {
        char line[MAX_LINE_LENGTH];
        int isHeader = 1;

        while (fgets(line, sizeof(line), menuFile)) {
            if (isHeader) {
                isHeader = 0;
                continue; // Skip the header row
            }

            nextMenuNumber++; // Count the number of existing rows
        }

        fclose(menuFile);
    }

    // Read ingredients from Stock.csv
    FILE *ingredientFile = fopen("Stock.csv", "r");
    if (!ingredientFile) {
        perror("Error opening Stock.csv");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int isHeader = 1;

    // Load ingredient names from the first column of Stock.csv
    while (fgets(line, sizeof(line), ingredientFile)) {
        if (isHeader) {
            isHeader = 0;
            continue; // Skip the header row
        }

        char *token = strtok(line, ",");
        if (token) {
            // Extract the ingredient name from the second column (after ID)
            token = strtok(NULL, ","); // Move to the second column which is the name
            if (token) {
                // Remove any newline character at the end of the name
                token[strcspn(token, "\n")] = '\0';

                // Check for duplicates
                int duplicate = 0;
                for (int i = 0; i < ingredientCount; i++) {
                    if (strcmp(ingredients[i].name, token) == 0) {
                        duplicate = 1;
                        break;
                    }
                }

                if (!duplicate) {
                    // Add the ingredient to the list
                    strncpy(ingredients[ingredientCount].name, token, sizeof(ingredients[ingredientCount].name) - 1);
                    ingredientCount++;
                }
            }
        }
    }

    fclose(ingredientFile);

    // UI to create a new menu item
    clearScreen();
    printf("=======================================\n");
    printf("          Create New Menu Item\n");
    printf("=======================================\n");

    printf("Menu item number: %d\n", nextMenuNumber);

    // Prompt user for new menu item details
    printf("Enter new menu item name: ");
    fgets(newItem.name, sizeof(newItem.name), stdin);
    newItem.name[strcspn(newItem.name, "\n")] = '\0';

    printf("Enter price for the new menu item: ");
    scanf("%lf", &newItem.price);
    clearInputBuffer();

    printf("Enter description for the new menu item: ");
    fgets(newItem.description, sizeof(newItem.description), stdin);
    newItem.description[strcspn(newItem.description, "\n")] = '\0';

    // Ask the user for the amount of each ingredient in Stock.csv
    printf("Enter amounts for each ingredient:\n");

    for (int i = 0; i < ingredientCount; i++) {
        printf("Amount of %s required for the new menu item: ", ingredients[i].name);
        scanf("%lf", &ingredientAmounts[i]);
        clearInputBuffer();
    }

    // Append the new item to the Menu.csv file
    menuFile = fopen("Menu.csv", "a");
    if (!menuFile) {
        perror("Error opening Menu.csv");
        return;
    }

    fprintf(menuFile, "%d,%s,%.2f,%s\n", 
            nextMenuNumber,
            newItem.name,
            newItem.price,
            newItem.description);
    fclose(menuFile);

    // Append the new item to Ingredient.csv in the required format
    FILE *ingredientFileWrite = fopen("Ingredient.csv", "a");
    if (!ingredientFileWrite) {
        perror("Error opening Ingredient.csv");
        return;
    }

    for (int i = 0; i < ingredientCount; i++) {
        // Write the menu item, ingredient, and amount
        fprintf(ingredientFileWrite, "%s,%s,%.2f\n", newItem.name, ingredients[i].name, ingredientAmounts[i]);
    }

    fclose(ingredientFileWrite);

    printf("\nNew menu item created successfully!\n");
    printf("Press Enter to return to the menu...\n");
    getchar();
}


// View Menu (Menu CRUD operations)

void ViewMenu() {
    typedef struct {
        char code[20];
        char name[100];
        double price;
        char description[100];
    } MenuItem;

    MenuItem menu[MAX_ROWS];
    int rowCount = 0;

    FILE *file = fopen("Menu.csv", "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int isHeader = 1;

    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue;
        }

        sscanf(line, "%[^,],%[^,],%lf,%[^\n]",
               menu[rowCount].code,
               menu[rowCount].name,
               &menu[rowCount].price,
               menu[rowCount].description);

        rowCount++;
    }

    fclose(file);

    clearScreen();
    printf("===================================================================================\n");
    printf("                                    MENU\n");
    printf("===================================================================================\n");
    printf("Code    Name                                             Price     Description\n");
    printf("-----------------------------------------------------------------------------------\n");

    for (int i = 0; i < rowCount; i++) {
        printf("%-7s%-50s%-10.2f%-30s\n",
               menu[i].code,
               menu[i].name,
               menu[i].price,
               menu[i].description);
    }

    printf("===================================================================================\n");
    printf("Press Enter to return to the menu...\n");
    getchar();
}

// Edit Menu Items

#include <ctype.h>

// Helper function to trim trailing whitespace
void trimWhitespace(char *str) {
    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }
}

void EditMenuItem() {
    typedef struct {
        char code[20];
        char name[100];
        double price;
        char description[100];
    } MenuItem;

    MenuItem menu[MAX_ROWS];
    int rowCount = 0;

    FILE *file = fopen("Menu.csv", "r+");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int isHeader = 1;

    // Read the menu items from the file into the menu array
    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue;
        }

        sscanf(line, "%[^,],%[^,],%lf,%[^\n]",
               menu[rowCount].code,
               menu[rowCount].name,
               &menu[rowCount].price,
               menu[rowCount].description);

        rowCount++;
    }

    clearScreen();
    printf("===================================================================================\n");
    printf("                                    MENU\n");
    printf("===================================================================================\n");
    printf("Code    Name                                             Price     Description\n");
    printf("-----------------------------------------------------------------------------------\n");

    // Print all the menu items
    for (int i = 0; i < rowCount; i++) {
        printf("%-7s%-50s%-10.2f%-30s\n",
               menu[i].code,
               menu[i].name,
               menu[i].price,
               menu[i].description);
    }

    printf("===================================================================================\n");

    // Ask the user for the menu item code to edit
    char editCode[20];
    printf("Enter the code of the menu item you want to edit: ");
    scanf("%s", editCode);
    clearInputBuffer();

    // Find the item to edit
    int foundIndex = -1;
    for (int i = 0; i < rowCount; i++) {
        if (strcmp(menu[i].code, editCode) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Menu item not found.\n");
        fclose(file);
        return;
    }

    // Print the current details of the selected menu item
    printf("\nCurrent details of the selected menu item:\n");
    printf("Code: %s\n", menu[foundIndex].code);
    printf("Name: %s\n", menu[foundIndex].name);
    printf("Price: %.2f\n", menu[foundIndex].price);
    printf("Description: %s\n", menu[foundIndex].description);

    // Prompt the user to edit the item
    printf("\nEnter new details (or press Enter to keep current values):\n");

    char newName[100];
    printf("Enter new name (current: %s): ", menu[foundIndex].name);
    fgets(newName, sizeof(newName), stdin);
    trimWhitespace(newName);
    if (newName[0] != '\0') {
        strncpy(menu[foundIndex].name, newName, sizeof(menu[foundIndex].name) - 1);
    }

    char priceInput[20];
    printf("Enter new price (current: %.2f): ", menu[foundIndex].price);
    fgets(priceInput, sizeof(priceInput), stdin);
    trimWhitespace(priceInput);
    if (priceInput[0] != '\0') {
        menu[foundIndex].price = atof(priceInput);
    }

    char newDescription[100];
    printf("Enter new description (current: %s): ", menu[foundIndex].description);
    fgets(newDescription, sizeof(newDescription), stdin);
    trimWhitespace(newDescription);
    if (newDescription[0] != '\0') {
        strncpy(menu[foundIndex].description, newDescription, sizeof(menu[foundIndex].description) - 1);
    }

    // Rewrite the menu file with updated details
    FILE *tempFile = fopen("Menu_temp.csv", "w");
    if (!tempFile) {
        perror("Error opening temporary file");
        fclose(file);
        return;
    }

    // Write the header to the temporary file
    fprintf(tempFile, "Code,Name,Price,Description\n");

    // Write the updated menu items to the temporary file
    for (int i = 0; i < rowCount; i++) {
        fprintf(tempFile, "%s,%s,%.2f,%s\n", 
                menu[i].code, 
                menu[i].name, 
                menu[i].price, 
                menu[i].description);
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated temporary file
    remove("Menu.csv");
    rename("Menu_temp.csv", "Menu.csv");

    printf("\nMenu item updated successfully!\n");
    printf("Press Enter to return to the menu...\n");
    getchar();
}

// Edit Ingredient

void EditIngredient() {
    typedef struct {
        char name[100];
        char ingredientName[100];
        int amountPerUnit;
    } Ingredient;

    Ingredient ingredients[MAX_ROWS];
    int rowCount = 0;

    // Open the Ingredient.csv file
    FILE *file = fopen("Ingredient.csv", "r+");
    if (!file) {
        perror("Error opening Ingredient.csv");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int isHeader = 1;

    // Read the ingredients from the file into the ingredients array
    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue;
        }

        sscanf(line, "%[^,],%[^,],%d",
               ingredients[rowCount].name,
               ingredients[rowCount].ingredientName,
               &ingredients[rowCount].amountPerUnit);

        rowCount++;
    }

    clearScreen();
    printf("===================================================================================\n");
    printf("                                 INGREDIENTS\n");
    printf("===================================================================================\n");
    printf("Menu Name                                   Ingredient Name                    Amount\n");
    printf("-----------------------------------------------------------------------------------\n");

    // Track which menu names have already been printed
    char printedMenus[MAX_ROWS][100] = {0};
    int printedCount = 0;

    for (int i = 0; i < rowCount; i++) {
        int alreadyPrinted = 0;

        // Check if the menu name has already been printed
        for (int j = 0; j < printedCount; j++) {
            if (strcmp(printedMenus[j], ingredients[i].name) == 0) {
                alreadyPrinted = 1;
                break;
            }
        }

        // Print the menu name only if it hasn't been printed before
        if (!alreadyPrinted) {
            printf("%-45s", ingredients[i].name);
            strncpy(printedMenus[printedCount++], ingredients[i].name, sizeof(printedMenus[0]) - 1);
        } else {
            printf("%-45s", ""); // Leave the menu name column blank for subsequent rows
        }

        // Print the ingredient details
        printf("%-35s%-10d\n",
               ingredients[i].ingredientName,
               ingredients[i].amountPerUnit);
    }

    printf("===================================================================================\n");

    // Ask the user for the menu name to edit its ingredients
    char menuName[100];
    printf("Enter the name of the menu you want to edit: ");
    fgets(menuName, sizeof(menuName), stdin);
    trimWhitespace(menuName);

    // Check if the menu name exists in the ingredient list
    int found = 0;
    for (int i = 0; i < rowCount; i++) {
        if (strcmp(ingredients[i].name, menuName) == 0) {
            found = 1;

            // Show the current details of the ingredient
            printf("\nIngredient: %s\n", ingredients[i].ingredientName);
            printf("Current amount per unit: %d\n", ingredients[i].amountPerUnit);

            // Prompt the user to enter a new amount
            printf("Enter new amount (or press Enter to keep current value): ");
            char amountInput[10];
            fgets(amountInput, sizeof(amountInput), stdin);
            trimWhitespace(amountInput);
            if (amountInput[0] != '\0') {
                ingredients[i].amountPerUnit = atoi(amountInput);
            }
        }
    }

    if (!found) {
        printf("No ingredients found for the specified menu.\n");
        fclose(file);
        return;
    }

    // Rewrite the Ingredient.csv file with updated details
    FILE *tempFile = fopen("Ingredient_temp.csv", "w");
    if (!tempFile) {
        perror("Error opening temporary file");
        fclose(file);
        return;
    }

    // Write the header to the temporary file
    fprintf(tempFile, "name,ingredientName,amountPerUnit\n");

    // Write the updated ingredients to the temporary file
    for (int i = 0; i < rowCount; i++) {
        fprintf(tempFile, "%s,%s,%d\n",
                ingredients[i].name,
                ingredients[i].ingredientName,
                ingredients[i].amountPerUnit);
    }

    fclose(file);
    fclose(tempFile);

    // Replace the original file with the updated temporary file
    remove("Ingredient.csv");
    rename("Ingredient_temp.csv", "Ingredient.csv");

    printf("\nIngredients updated successfully!\n");
    printf("Press Enter to return to the menu...\n");
    getchar();
}


// Delete Menu Items

void DeleteMenuItem() {
    typedef struct {
        char code[20];
        char name[100];
        double price;
        char description[100];
    } MenuItem;

    MenuItem menu[MAX_ROWS];
    int rowCount = 0;

    // Read from Menu.csv
    FILE *file = fopen("Menu.csv", "r");
    if (!file) {
        perror("Error opening Menu.csv");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int isHeader = 1;

    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue;
        }

        sscanf(line, "%[^,],%[^,],%lf,%[^\n]",
               menu[rowCount].code,
               menu[rowCount].name,
               &menu[rowCount].price,
               menu[rowCount].description);

        rowCount++;
    }
    fclose(file);

    // Display menu for deletion
    clearScreen();
    printf("===================================================================================\n");
    printf("                                       MENU\n");
    printf("===================================================================================\n");
    printf("Code    Name                                             Price     Description\n");
    printf("-----------------------------------------------------------------------------------\n");

    for (int i = 0; i < rowCount; i++) {
        printf("%-7s%-50s%-10.2f%-30s\n",
               menu[i].code,
               menu[i].name,
               menu[i].price,
               menu[i].description);
    }

    printf("===================================================================================\n");
    printf("Enter the code of the menu item to delete: ");

    char codeToDelete[20];
    fgets(codeToDelete, sizeof(codeToDelete), stdin);
    codeToDelete[strcspn(codeToDelete, "\n")] = '\0';

    FILE *tempFile = fopen("TempMenu.csv", "w");
    if (!tempFile) {
        perror("Error creating temporary file");
        return;
    }

    int found = 0;
    for (int i = 0; i < rowCount; i++) {
        if (strcmp(menu[i].code, codeToDelete) != 0) {
            fprintf(tempFile, "%s,%s,%.2f,%s\n",
                    menu[i].code,
                    menu[i].name,
                    menu[i].price,
                    menu[i].description);
        } else {
            found = 1;
        }
    }
    fclose(tempFile);

    // Update Menu.csv
    if (found) {
        remove("Menu.csv");
        rename("TempMenu.csv", "Menu.csv");
        printf("Menu item with code '%s' deleted successfully!\n", codeToDelete);

        // Delete related records from Ingredient.csv
        FILE *ingredientFile = fopen("Ingredient.csv", "r");
        if (!ingredientFile) {
            perror("Error opening Ingredient.csv");
            return;
        }

        FILE *tempIngredientFile = fopen("TempIngredient.csv", "w");
        if (!tempIngredientFile) {
            perror("Error creating temporary Ingredient file");
            fclose(ingredientFile);
            return;
        }

        while (fgets(line, sizeof(line), ingredientFile)) {
            char menuName[100], ingredientName[50];
            double amount;

            sscanf(line, "%[^,],%[^,],%lf", menuName, ingredientName, &amount);

            // Skip lines corresponding to the deleted menu item
            int skipLine = 0;
            for (int i = 0; i < rowCount; i++) {
                if (strcmp(menuName, menu[i].name) == 0 && strcmp(menu[i].code, codeToDelete) == 0) {
                    skipLine = 1;
                    break;
                }
            }

            if (!skipLine) {
                fprintf(tempIngredientFile, "%s,%s,%.2f\n", menuName, ingredientName, amount);
            }
        }

        fclose(ingredientFile);
        fclose(tempIngredientFile);

        remove("Ingredient.csv");
        rename("TempIngredient.csv", "Ingredient.csv");
    } else {
        remove("TempMenu.csv");
        printf("Menu item with code '%s' not found.\n", codeToDelete);
    }

    printf("Press Enter to return to the menu...\n");
    getchar();
}

// Edit Menu Item Menu

void EditMenuItemMenu() {
    int MenuMenuChoice;
    clearScreen();
    printf("===================================================================================\n");
    printf("                                Edit Menu Item\n");
    printf("===================================================================================\n");
    printf("1. Edit Name/Price/Description\n");
    printf("2. Edit Ingredients\n");
    printf("3. Return to Menu CRUD Operations\n");
    printf("===================================================================================\n");
    printf("Enter your choice: ");
        scanf("%d", &MenuMenuChoice);
        clearInputBuffer();
        switch (MenuMenuChoice) {
            case 1:
                EditMenuItem();
                EditMenuItemMenu();
                break;
            case 2:
                EditIngredient();
                EditMenuItemMenu();
                break;
            case 3:
                return;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  
                EditMenuItemMenu();
        }
}



// Menu CRUD operations

void MenuCRUD() {
    int MenuCRUDchoice;
    clearScreen();
    printf("===================================================================================\n");
    printf("                            Menu CRUD Operations\n");
    printf("===================================================================================\n");
    printf("1. Create A New Menu\n");
    printf("2. View Menu\n");
    printf("3. Edit An Existing Menu\n");
    printf("4. Delete Menu\n");
    printf("5. Return To Owner Features\n");
    printf("===================================================================================\n");
    printf("Enter your choice: ");
        scanf("%d", &MenuCRUDchoice);
        clearInputBuffer();
        switch (MenuCRUDchoice) {
            case 1:
                CreateNewMenuItem();
                MenuCRUD();
                break;
            case 2:
                ViewMenu();
                MenuCRUD();
                break;
            case 3:
                EditMenuItemMenu();
                MenuCRUD();
                break;
            case 4:
                DeleteMenuItem();
                MenuCRUD();
                break;
            case 5:
                return;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  
                MenuCRUD();
        }
}

// View Stock (Stock CRUD Operations)

void ViewStock() {

    Stock stock[MAX_ROWS];
    int rowCount = 0;

    FILE *file = fopen("Stock.csv", "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int isHeader = 1;

    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue;
        }

        sscanf(line, "%[^,],%[^,],%d,%[^,],%[^,],%[^\n]",
               stock[rowCount].id,
               stock[rowCount].name,
               &stock[rowCount].quantity,
               stock[rowCount].unit,
               stock[rowCount].restock,
               stock[rowCount].expire);

        rowCount++;
    }

    fclose(file);

    clearScreen();
    printf("===================================================================================\n");
    printf("                                    STOCK\n");
    printf("===================================================================================\n");
    printf("ID      Name                                        Quantity    Unit    Re-stock Date  Expiry Date\n");
    printf("-----------------------------------------------------------------------------------\n");

    for (int i = 0; i < rowCount; i++) {
        printf("%-7s%-50s%-12d%-8s%-15s%-15s\n",
               stock[i].id,
               stock[i].name,
               stock[i].quantity,
               stock[i].unit,
               stock[i].restock[0] == '\0' ? "N/A" : stock[i].restock,
               stock[i].expire[0] == '\0' ? "N/A" : stock[i].expire);
    }

    printf("===================================================================================\n");
    printf("Press Enter to return to the menu...\n");
    getchar();
}

// Add New Stock Function

void AddNewStock() {
    Stock newStock;
    FILE *file = fopen("Stock.csv", "a");
    if (!file) {
        perror("Error opening file");
        return;
    }

    printf("Enter Stock ID: ");
    fgets(newStock.id, sizeof(newStock.id), stdin);
    newStock.id[strcspn(newStock.id, "\n")] = '\0';

    printf("Enter Stock Name: ");
    fgets(newStock.name, sizeof(newStock.name), stdin);
    newStock.name[strcspn(newStock.name, "\n")] = '\0';

    printf("Enter Quantity: ");
    scanf("%d", &newStock.quantity);
    clearInputBuffer();

    printf("Enter Unit: ");
    fgets(newStock.unit, sizeof(newStock.unit), stdin);
    newStock.unit[strcspn(newStock.unit, "\n")] = '\0';

    printf("Enter Restock Date (e.g., 1/1/2025): ");
    fgets(newStock.restock, sizeof(newStock.restock), stdin);
    newStock.restock[strcspn(newStock.restock, "\n")] = '\0';

    printf("Enter Expiry Date (e.g., 1/1/2025): ");
    fgets(newStock.expire, sizeof(newStock.expire), stdin);
    newStock.expire[strcspn(newStock.expire, "\n")] = '\0';

    fprintf(file, "%s,%s,%d,%s,%s,%s\n", newStock.id, newStock.name, newStock.quantity, newStock.unit, newStock.restock, newStock.expire);
    fclose(file);

    printf("Stock added successfully!\n");
    printf("Press Enter to continue...");
    getchar();
}

// Deleting a stock function

void DeleteStockItem() {
    Stock stock[MAX_ROWS];
    int rowCount = 0;

    FILE *file = fopen("Stock.csv", "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int isHeader = 1;

    // Read existing stock items from the file
    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue; // Skip header
        }
        
        sscanf(line, "%[^,],%[^,],%d,%[^,],%[^,],%[^\n]",
               stock[rowCount].id,
               stock[rowCount].name,
               &stock[rowCount].quantity,
               stock[rowCount].unit,
               stock[rowCount].restock,
               stock[rowCount].expire);

        rowCount++;
    }

    fclose(file);

    // Display available stock items
    clearScreen();
    printf("===================================================================================\n");
    printf("                                    STOCK ITEMS\n");
    printf("===================================================================================\n");
    printf("ID    Name                            Quantity   Unit   Restock Date   Expire Date\n");
    printf("-----------------------------------------------------------------------------------\n");

    for (int i = 0; i < rowCount; i++) {
        printf("%-6s%-30s%-10d%-8s%-15s%-15s\n",
               stock[i].id,
               stock[i].name,
               stock[i].quantity,
               stock[i].unit,
               stock[i].restock,
               stock[i].expire);
    }

    printf("===================================================================================\n");
    printf("Enter the stock ID to delete: ");
    
    char idToDelete[10];
    fgets(idToDelete, sizeof(idToDelete), stdin);
    idToDelete[strcspn(idToDelete, "\n")] = '\0'; // Remove newline character

    FILE *tempFile = fopen("TempStock.csv", "w");
    if (!tempFile) {
        perror("Error creating temporary file");
        return;
    }

    int found = 0;
    for (int i = 0; i < rowCount; i++) {
        if (strcmp(stock[i].id, idToDelete) != 0) {
            fprintf(tempFile, "%s,%s,%d,%s,%s,%s\n",
                    stock[i].id,
                    stock[i].name,
                    stock[i].quantity,
                    stock[i].unit,
                    stock[i].restock,
                    stock[i].expire);
        } else {
            found = 1;
        }
    }

    fclose(tempFile);

    // If the stock item was found and removed, replace the original file
    if (found) {
        remove("Stock.csv");
        rename("TempStock.csv", "Stock.csv");
        printf("Stock item with ID '%s' deleted successfully!\n", idToDelete);
    } else {
        remove("TempStock.csv");
        printf("Stock item with ID '%s' not found.\n", idToDelete);
    }

    printf("Press Enter to return to the menu...\n");
    getchar();
}

// Editing a stock

void EditStockItem() {
    Stock stock[MAX_ROWS];
    int rowCount = 0;

    // Read existing stock items from file
    FILE *file = fopen("Stock.csv", "r");
    if (!file) {
        perror("Error opening file");
        return;
    }

    char line[MAX_LINE_LENGTH];
    int isHeader = 1;

    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue;
        }

        sscanf(line, "%[^,],%[^,],%d,%[^,],%[^,],%[^\n]",
               stock[rowCount].id,
               stock[rowCount].name,
               &stock[rowCount].quantity,
               stock[rowCount].unit,
               stock[rowCount].restock,
               stock[rowCount].expire);

        rowCount++;
    }

    fclose(file);

    // Display available stock items
    clearScreen();
    printf("===================================================================================\n");
    printf("                                Edit Stock Items\n");
    printf("===================================================================================\n");
    printf("Available Stock Items:\n");
    printf("ID     Name                               Quantity   Unit   Restock Date   Expiry Date\n");
    printf("-----------------------------------------------------------------------------------\n");

    for (int i = 0; i < rowCount; i++) {
        printf("%-6s%-35s%-10d%-8s%-15s%-15s\n",
               stock[i].id,
               stock[i].name,
               stock[i].quantity,
               stock[i].unit,
               stock[i].restock,
               stock[i].expire);
    }

    // Prompt user to enter the stock ID to edit
    char stockID[10];
    printf("\nEnter the stock item ID to edit: ");
    fgets(stockID, sizeof(stockID), stdin);
    stockID[strcspn(stockID, "\n")] = '\0'; // Remove newline character

    // Find the stock item in the list
    int foundIndex = -1;
    for (int i = 0; i < rowCount; i++) {
        if (strcmp(stock[i].id, stockID) == 0) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        printf("Stock item not found.\n");
        printf("Press Enter to return to the menu...\n");
        getchar();
        return;
    }

    // Editing the stock item
    printf("\nEditing Stock Item: %s\n", stockID);

    printf("Enter new name (current: %s): ", stock[foundIndex].name);
    fgets(stock[foundIndex].name, sizeof(stock[foundIndex].name), stdin);
    stock[foundIndex].name[strcspn(stock[foundIndex].name, "\n")] = '\0';

    printf("Enter new quantity (current: %d): ", stock[foundIndex].quantity);
    scanf("%d", &stock[foundIndex].quantity);
    clearInputBuffer();

    printf("Enter new unit (current: %s): ", stock[foundIndex].unit);
    fgets(stock[foundIndex].unit, sizeof(stock[foundIndex].unit), stdin);
    stock[foundIndex].unit[strcspn(stock[foundIndex].unit, "\n")] = '\0';

    printf("Enter new restock date (current: %s): ", stock[foundIndex].restock);
    fgets(stock[foundIndex].restock, sizeof(stock[foundIndex].restock), stdin);
    stock[foundIndex].restock[strcspn(stock[foundIndex].restock, "\n")] = '\0';

    printf("Enter new expiry date (current: %s): ", stock[foundIndex].expire);
    fgets(stock[foundIndex].expire, sizeof(stock[foundIndex].expire), stdin);
    stock[foundIndex].expire[strcspn(stock[foundIndex].expire, "\n")] = '\0';

    // Write the updated stock items back to the file
    FILE *tempFile = fopen("TempStock.csv", "w");
    if (!tempFile) {
        perror("Error opening temporary file");
        return;
    }

    // Write header
    fprintf(tempFile, "id,name,quantity,unit,restock,expire\n");
    for (int i = 0; i < rowCount; i++) {
        fprintf(tempFile, "%s,%s,%d,%s,%s,%s\n",
                stock[i].id,
                stock[i].name,
                stock[i].quantity,
                stock[i].unit,
                stock[i].restock,
                stock[i].expire);
    }

    fclose(tempFile);

    remove("Stock.csv");
    rename("TempStock.csv", "Stock.csv");

    printf("Stock item updated successfully!\n");
    printf("Press Enter to return to the menu...\n");
    getchar();
}


// Stock CRUD Operations

void StockCRUD() {
    int StockCRUDchoice;
    clearScreen();
    printf("===================================================================================\n");
    printf("                                Stock CRUD Operations\n");
    printf("===================================================================================\n");
    printf("1. Create A New Stock\n");
    printf("2. View Stock\n");
    printf("3. Edit An Existing Stock\n");
    printf("4. Delete Stock\n");
    printf("5. Return To Owner Features\n");
    printf("===================================================================================\n");
    printf("Enter your choice: ");
        scanf("%d", &StockCRUDchoice);
        clearInputBuffer();
        switch (StockCRUDchoice) {
            case 1:
                AddNewStock();
                StockCRUD();
                break;
            case 2:
                ViewStock();
                StockCRUD();
                break;
            case 3:
                EditStockItem();
                StockCRUD();
                break;
            case 4:
                DeleteStockItem();
                StockCRUD();
                break;
            case 5:
                return;
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  
                MenuCRUD();
        }
}

// CRUD Operation

void CRUDoperationMenu() {
    int CRUDchoice;
    clearScreen();
    printf("===================================================================================\n");
    printf("                                CRUD Operations\n");
    printf("===================================================================================\n");
    printf("1. Edit Menu\n");
    printf("2. Edit Stock\n");
    printf("3. Return to Owner Features\n");
    printf("===================================================================================\n");
    printf("Enter your choice: ");
        scanf("%d", &CRUDchoice);
        clearInputBuffer();
        switch (CRUDchoice) {
            case 1:
            MenuCRUD();
                break;
            case 2:
            StockCRUD();
                break;
            case 3:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  
                CRUDoperationMenu();
        }
}

// Restock Items Function in Owner Functions

int RestockFunction() {
    clearScreen();
    Item items[MAX_ROWS];
    int rowCount = 0;

    if (!readCSV("Stock.csv", items, &rowCount)) {
        printf("Error reading the file.\n");
        return EXIT_FAILURE;
    }

    char uniqueNames[MAX_ROWS][50];
    int uniqueCount = getUniqueIngredients(items, rowCount, uniqueNames);

    restockMenu(items, &rowCount, uniqueNames, uniqueCount);

    writeCSV("Stock.csv", items, rowCount);
}

int readCSV(const char *filename, Item items[], int *rowCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int isHeader = 1;
    *rowCount = 0;

    while (fgets(line, sizeof(line), file)) {
        if (isHeader) {
            isHeader = 0;
            continue;
        }

        sscanf(line, "%[^,],%[^,],%d,%[^,],%[^,],%s",
               items[*rowCount].id,
               items[*rowCount].name,
               &items[*rowCount].quantity,
               items[*rowCount].unit,
               items[*rowCount].restock,
               items[*rowCount].expire);

        (*rowCount)++;
    }

    fclose(file);
    return 1;
}

void writeCSV(const char *filename, Item items[], int rowCount) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "id,name,quantity,unit,restock,expire\n");

    for (int i = 0; i < rowCount; i++) {
        fprintf(file, "%s,%s,%d,%s,%s,%s\n",
                items[i].id,
                items[i].name,
                items[i].quantity,
                items[i].unit,
                items[i].restock,
                items[i].expire);
    }

    fclose(file);
}

int getUniqueIngredients(Item items[], int rowCount, char uniqueNames[][50]) {
    int uniqueCount = 0;

    for (int i = 0; i < rowCount; i++) {
        int isUnique = 1;

        for (int j = 0; j < uniqueCount; j++) {
            if (strcmp(items[i].name, uniqueNames[j]) == 0) {
                isUnique = 0;
                break;
            }
        }

        if (isUnique) {
            strcpy(uniqueNames[uniqueCount], items[i].name);
            uniqueCount++;
        }
    }

    return uniqueCount;
}

void displayCombinedStock(Item items[], int rowCount, char uniqueNames[][50], int uniqueCount) {
    printf("===================================================================================\n");
    printf("                                  Current Stock             \n");
    printf("===================================================================================\n");

    for (int i = 0; i < uniqueCount; i++) {
        int totalQuantity = 0;
        char unit[10] = "";

        for (int j = 0; j < rowCount; j++) {
            if (strcmp(items[j].name, uniqueNames[i]) == 0) {
                totalQuantity += items[j].quantity;

                if (strlen(items[j].unit) > 0) {
                    strcpy(unit, items[j].unit);
                }
            }
        }

         if (strlen(unit) == 0) {
            if (strcmp(uniqueNames[i], "Roasted Pork") == 0 || strcmp(uniqueNames[i], "Roasted Chicken") == 0) {
                strcpy(unit, "g");
            } else if (strcmp(uniqueNames[i], "Shrimp Wonton") == 0) {
                strcpy(unit, "piece");
            } else {
                strcpy(unit, "unit");  // Generic fallback unit
            }
        }

        printf("%d. %s: %d %s\n", i + 1, uniqueNames[i], totalQuantity, unit);
    }

    printf("===================================================================================\n");
}

// Display Updated Combined Stock
int parseDate(const char *date, int *year, int *month, int *day) {
    if (sscanf(date, "%4d-%2d-%2d", year, month, day) != 3) {
        return 0; // Invalid format
    }

    // Validate the date components
    if (*month < 1 || *month > 12 || *day < 1 || *day > 31) {
        return 0; // Invalid month or day
    }

    // Days in each month
    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((*year % 4 == 0 && *year % 100 != 0) || (*year % 400 == 0)) {
        daysInMonth[1] = 29; // Leap year adjustment for February
    }

    if (*day > daysInMonth[*month - 1]) {
        return 0; // Invalid day for the given month
    }

    return 1; // Valid date
}
int isValidDate(const char *date, const char *restockDate) {
    int year, month, day, rYear, rMonth, rDay;

    // Parse expiration date
    if (!parseDate(date, &year, &month, &day)) {
        return 0; // Invalid expiration date format
    }

    // Parse restock date
    if (!parseDate(restockDate, &rYear, &rMonth, &rDay)) {
        return 0; // Invalid restock date format (should not happen)
    }

    // Compare dates
    if (year < rYear || (year == rYear && month < rMonth) ||
        (year == rYear && month == rMonth && day <= rDay)) {
        return 0; // Expiration date is earlier or same as restock date
    }

    return 1; // Valid date
}
void displayUpdatedCombinedStock(Item items[], int rowCount, char uniqueNames[][50], int uniqueCount) {
    printf("===================================================================================\n");
    printf("                                    Updated Stock             \n");
    printf("===================================================================================\n");

    for (int i = 0; i < uniqueCount; i++) {
        int totalQuantity = 0;
        char unit[10] = "";

        for (int j = 0; j < rowCount; j++) {
            if (strcmp(items[j].name, uniqueNames[i]) == 0) {
                totalQuantity += items[j].quantity;

                if (strlen(items[j].unit) > 0) {
                    strcpy(unit, items[j].unit);
                }
            }
        }

        if (strlen(unit) == 0) {
            strcpy(unit, "unit");
        }

        printf("%d. %s: %d %s\n", i + 1, uniqueNames[i], totalQuantity, unit);
    }

    printf("===================================================================================\n");
    printf("Press Enter to continue...");
    getchar(); // Wait for user input
    getchar(); // Handle newline left by previous input
    clearScreen();
    return;
}


void restockMenu(Item items[], int *rowCount, char uniqueNames[][50], int uniqueCount) {
    int choice, restockAmount;
    char expireDate[20], restockDate[20], inputUnit[10];

    // Get the current date
    time_t now = time(NULL);
    strftime(restockDate, sizeof(restockDate), "%Y-%m-%d", localtime(&now));

    // Display the stock
    displayCombinedStock(items, *rowCount, uniqueNames, uniqueCount);

    // Prompt user to choose an ingredient
    printf("Select an ingredient to restock (1-%d): ", uniqueCount);
    scanf("%d", &choice);

    // Validate choice
    if (choice < 1 || choice > uniqueCount) {
        printf("Invalid choice. Exiting restock process.\n");
        return;
    }

    // Adjust choice for zero-based indexing
    choice--;

    // Prompt user for restock details
    printf("Enter restock amount for %s: ", uniqueNames[choice]);
    scanf("%d", &restockAmount);

    // Prompt user for unit and validate
    do {
        printf("Enter unit for %s (e.g., g, piece): ", uniqueNames[choice]);
        scanf("%s", inputUnit);

        int unitMatches = 0;
        for (int i = 0; i < *rowCount; i++) {
            if (strcmp(items[i].name, uniqueNames[choice]) == 0 && strcmp(items[i].unit, inputUnit) == 0) {
                unitMatches = 1;
                break;
            }
        }

        if (!unitMatches) {
            printf("Invalid unit. It must match the unit in the stock file.\n");
        } else {
            break;
        }
    } while (1);

    // Validate expiration date input
    do {
        printf("Enter expiration date (YYYY-MM-DD): ");
        scanf("%s", expireDate);

        if (!isValidDate(expireDate, restockDate)) {
            printf("Invalid expiration date. Ensure the format is YYYY-MM-DD and it is later than the restock date (%s).\n", restockDate);
        }
    } while (!isValidDate(expireDate, restockDate)); // Repeat until valid input

    // Handle restocking logic
    int restocked = 0;

    // First check if there's a row with quantity 0
    for (int i = 0; i < *rowCount; i++) {
        if (strcmp(items[i].name, uniqueNames[choice]) == 0 && items[i].quantity == 0) {
            items[i].quantity += restockAmount;
            strcpy(items[i].restock, restockDate); // Use system date for restock date
            strcpy(items[i].expire, expireDate);
            restocked = 1;
            break;
        }
    }

    // If no row with quantity 0, check for matching expiration date
    if (!restocked) {
        for (int i = 0; i < *rowCount; i++) {
            if (strcmp(items[i].name, uniqueNames[choice]) == 0 &&
                strcmp(items[i].expire, expireDate) == 0) {
                items[i].quantity += restockAmount;
                restocked = 1;
                break;
            }
        }
    }

    // If no matching row, add a new row
    if (!restocked) {
        if (*rowCount < MAX_ROWS) {
            Item newItem;
            sprintf(newItem.id, "NEW%d", *rowCount + 1);  // Generate a new unique ID
            strcpy(newItem.name, uniqueNames[choice]);
            newItem.quantity = restockAmount;
            strcpy(newItem.unit, inputUnit);
            strcpy(newItem.restock, restockDate); // Use system date for restock date
            strcpy(newItem.expire, expireDate);
            items[*rowCount] = newItem;
            (*rowCount)++;
        } else {
            printf("Error: Cannot add new row. Maximum capacity reached.\n");
        }
    }

    // Confirm update
    clearScreen();
    printf("Restocked %s on %s. Updated inventory:\n", uniqueNames[choice], restockDate);
    displayUpdatedCombinedStock(items, *rowCount, uniqueNames, uniqueCount);
}

// Restock Choice

void RestockChoice() {
        int RestockChoice;
    clearScreen();
    printf("===================================================================================\n");
    printf("                                Restock Menu\n");
    printf("===================================================================================\n");
    printf("1. Auto Restock Notifications\n");
    printf("2. Manual Restock Option\n");
    printf("3. Return To Owner Features\n");
    printf("===================================================================================\n");
    printf("Enter your choice: ");
        scanf("%d", &RestockChoice);
        clearInputBuffer();
        switch (RestockChoice) {
            case 1:
                clearScreen();
                printf("===================================================================================\n");
                printf("                           Auto Restock Notifications\n");
                printf("===================================================================================\n");
                
                printf("===================================================================================\n");
                break;
            case 2:
            RestockFunction();
                break;
            case 3:
                return;
            
               
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  
                return;
        }

}



// Owner Menu

void ownerMenu() {
    int ownerChoice;
    do {
        clearScreen();
        printf("===================================================================================\n");
        printf("                                OWNER FEATURES MENU\n");
        printf("===================================================================================\n");
        printf("1. View Reports\n");
        printf("2. Perform CRUD Operations\n");
        printf("3. Restock Items\n");
        printf("4. View Logs\n");
        printf("5. Manage Discount Coupons\n");
        printf("6. Back to Main Menu\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &ownerChoice);
        clearInputBuffer();

        switch (ownerChoice) {
            case 1:
                loadSalesLog("sales_log.csv");
                displayMenu();
                break;
            case 2:
                CRUDoperationMenu();
                break;
            case 3:
                RestockChoice();
                break;
            case 4:
                mainLoggingMenu();
                break;
            case 5:
                ManageCouponsMenu();
                break;
            case 6:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  
                break;
        }

    } while (ownerChoice != 5);
}

typedef struct {
    char name[50];
    int quantity;
    int cost;
} CartItem;

typedef struct {
    char code[10];           // Discount code
    int discountPercent;     // Discount percentage
    int minSpend;            // Minimum spend required
} Discount;

typedef struct {
    char name[50];
    char ingredientName[50];
    int amountPerUnit; // Amount of the ingredient needed per unit of menu
} MenuIngredient;

// Struct for Restock Rules
typedef struct {
    char name[50];
    int threshold;
    int restock_amount;
    int expire_days;
} RestockRule;

// Struct for Stock Items
typedef struct {
    char id[10];
    char name[50];
    int quantity;
    char unit[10];
    char restockDate[20];
    char expireDate[20];
} StockItem;

#define MAX_MENU_ITEMS 100
MenuIngredient menuIngredients[MAX_MENU_ITEMS];
int menuItemCount = 0;

#define STOCK_FILE "Stock.csv"
#define RESTOCK_FILE "RestockRules.csv"
#define MAX_DISCOUNTS 10

// Maximum cart size
#define MAX_CART_SIZE 50

// Global cart and cart size
CartItem cart[MAX_CART_SIZE];
int cartSize = 0;

Discount discounts[MAX_DISCOUNTS];
int discountCount = 10;

#define MAX_STOCKS 100
StockItem stocks[MAX_STOCKS];
int stockCount = 0;

int loadRestockRules(RestockRule rules[], int max_rules);
int loadStock(StockItem stock[], int max_stock);
void saveStock();
void autoRestock(StockItem stock[], int *stock_count, RestockRule rules[], int rule_count);
void addRestockedItem(StockItem stock[], int *stock_count, char *name, int restock_amount, int expire_days);

// Load Restock Rules from RestockRules.csv
int loadRestockRules(RestockRule rules[], int max_rules) {
    FILE *file = fopen(RESTOCK_FILE, "r");
    if (!file) {
        printf("Error: Cannot open %s\n", RESTOCK_FILE);
        return 0;
    }

    char line[100];
    int count = 0;

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && count < max_rules) {
        sscanf(line, "%49[^,],%d,%d,%d", 
               rules[count].name, 
               &rules[count].threshold, 
               &rules[count].restock_amount, 
               &rules[count].expire_days);
        count++;
    }

    fclose(file);
    return count;
}

// Load Stock from Stock.csv
int loadStock(StockItem stock[], int max_stock) {
    FILE *file = fopen(STOCK_FILE, "r");
    if (!file) {
        printf("Error: Cannot open %s\n", STOCK_FILE);
        return 0;
    }

    char line[150];
    int count = 0;

    // Skip header
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file) && count < max_stock) {
        sscanf(line, "%9[^,],%49[^,],%d,%9[^,],%19[^,],%19[^\n]", 
               stock[count].id, 
               stock[count].name, 
               &stock[count].quantity, 
               stock[count].unit, 
               stock[count].restockDate, 
               stock[count].expireDate);
        count++;
    }

    fclose(file);
    return count;
}

// Save Updated Stock to Stock.csv
// Save only new stocks to Stock.csv
// Save only new stocks (from auto restock) to Stock.csv
void saveStock(const char* Stock) {
    // Open the file in read+append mode
    FILE* file = fopen("Stock.csv", "a+"); // "a+" allows reading and appending
    if (file == NULL) {
        perror("Error saving stock file");
        return;
    }

    // Buffer to hold existing stock IDs in the file
    char existingIDs[MAX_STOCKS][10]; 
    int existingCount = 0;

    // First, read all existing stock IDs into existingIDs[]
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char id[10];
        if (sscanf(line, "%9[^,]", id) == 1) {
            strcpy(existingIDs[existingCount], id);
            existingCount++;
        }
    }

    // Append only new stock items (check if the ID already exists)
    for (int i = 0; i < stockCount; i++) {
        int isDuplicate = 0;

        // Check if the current stock ID already exists in the file
        for (int j = 0; j < existingCount; j++) {
            if (strcmp(stocks[i].id, existingIDs[j]) == 0) {
                isDuplicate = 1;
                break;
            }
        }

        // If not a duplicate, append it to the file
        if (!isDuplicate) {
            fprintf(file, "\n%s,%s,%d,%s,%s,%s",
                    stocks[i].id,
                    stocks[i].name,
                    stocks[i].quantity,
                    stocks[i].unit,
                    stocks[i].restockDate,
                    stocks[i].expireDate);
        }
    }

    fclose(file); // Close the file
}



// Add Restocked Item to Stock
void addRestockedItem(StockItem stock[], int *stock_count, char *name, int restock_amount, int expire_days) {
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char restock_date[20], expire_date[20];
    int countnewitem = 0;
    // Get restock date
    strftime(restock_date, sizeof(restock_date), "%Y-%m-%d", tm_info);

    // Calculate expire date
    tm_info->tm_mday += expire_days;
    mktime(tm_info);
    strftime(expire_date, sizeof(expire_date), "%Y-%m-%d", tm_info);

    // Add new stock entry
    StockItem new_item;
    sprintf(new_item.id, "AUTO%03d", *stock_count + 1);
    strcpy(new_item.name, name);
    new_item.quantity = restock_amount;
    strcpy(new_item.unit, "gram"); // Default unit
    strcpy(new_item.restockDate, restock_date);
    strcpy(new_item.expireDate, expire_date);


    stock[*stock_count] = new_item;
    (*stock_count)++;
}

// Perform Auto Restock
void autoRestock(StockItem stock[], int *stock_count, RestockRule rules[], int rule_count) {
    int total_quantity[50] = {0};
    char unique_names[50][50];
    int unique_count = 0;
    int countnewitem = 0;

    // Sum total quantities by name
    for (int i = 0; i < *stock_count; i++) {
        int found = 0;
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(stock[i].name, unique_names[j]) == 0) {
                total_quantity[j] += stock[i].quantity;
                found = 1;
                break;
            }
        }
        if (!found) {
            strcpy(unique_names[unique_count], stock[i].name);
            total_quantity[unique_count] = stock[i].quantity;
            unique_count++;
        }
    }

    // Compare with Restock Rules and restock if necessary
    for (int i = 0; i < rule_count; i++) {
        for (int j = 0; j < unique_count; j++) {
            if (strcmp(rules[i].name, unique_names[j]) == 0) {
                if (total_quantity[j] < rules[i].threshold) {
                    addRestockedItem(stock, stock_count, rules[i].name, rules[i].restock_amount, rules[i].expire_days);
                    
                }
            }
        }
    }
}

// Function to compare dates (returns -1 if date1 < date2, 1 if date1 > date2, 0 if equal)
int compareDates(const char* date1, const char* date2) {
    return strcmp(date1, date2);
}

// Function to load stock data from the CSV file
void loadStockFromCSV(const char* Stock) {
    FILE* file = fopen("Stock.csv", "r");
    if (file == NULL) {
        perror("Error opening stock file");
        exit(EXIT_FAILURE);
    }

    char line[512];
    fgets(line, sizeof(line), file); // Skip header

    while (fgets(line, sizeof(line), file)) {
        if (stockCount >= MAX_STOCKS) {
            printf("Stock capacity exceeded!\n");
            break;
        }

        char* token = strtok(line, ",");
        strncpy(stocks[stockCount].id, token, sizeof(stocks[stockCount].id) - 1);

        token = strtok(NULL, ",");
        strncpy(stocks[stockCount].name, token, sizeof(stocks[stockCount].name) - 1);

        token = strtok(NULL, ",");
        stocks[stockCount].quantity = atoi(token);

        token = strtok(NULL, ",");
        strncpy(stocks[stockCount].unit, token, sizeof(stocks[stockCount].unit) - 1);

        token = strtok(NULL, ",");
        strncpy(stocks[stockCount].restockDate, token, sizeof(stocks[stockCount].restockDate) - 1);

        token = strtok(NULL, ",");
        strncpy(stocks[stockCount].expireDate, token, sizeof(stocks[stockCount].expireDate) - 1);

        stockCount++;
    }

    fclose(file);
}

// Deduct stock based on ingredient requirements
void deductStock(const char* ingredient, int requiredQuantity) {
    for (int i = 0; i < stockCount; i++) {
        if (strcmp(stocks[i].name, ingredient) == 0) {
            for (int j = 0; j < stockCount; j++) {
                // Find the stock item with the nearest expiration date
                if (strcmp(stocks[j].name, ingredient) == 0 && stocks[j].quantity > 0) {
                    if (compareDates(stocks[j].expireDate, stocks[i].expireDate) < 0) {
                        StockItem temp = stocks[i];
                        stocks[i] = stocks[j];
                        stocks[j] = temp;
                    }
                }
            }

            // Deduct the stock
            if (stocks[i].quantity >= requiredQuantity) {
                stocks[i].quantity -= requiredQuantity;
                requiredQuantity = 0;
            } else {
                requiredQuantity -= stocks[i].quantity;
                stocks[i].quantity = 0;
            }

            if (requiredQuantity == 0) break;
        }
    }
    if (requiredQuantity > 0) {
        printf("Warning: Not enough %s in stock!\n", ingredient);
    }
}
// Save updated stock back to the file
void saveStockToCSV(const char* Stock) {
    // Open the file in write mode, which overwrites the existing content
    FILE* file = fopen("Stock.csv", "w");
    if (file == NULL) {
        perror("Error saving stock file");
        return;
    }

    // Write the header to the CSV file
    fprintf(file, "id,name,quantity,unit,restock,expire\n");

    // Write each stock item's details
    for (int i = 0; i < stockCount; i++) {
        fprintf(file, "%s,%s,%d,%s,%s,%s",
                stocks[i].id,
                stocks[i].name,
                stocks[i].quantity,
                stocks[i].unit,
                stocks[i].restockDate,
                stocks[i].expireDate);
        if(i>stockCount){
            fprintf(file,"\n");
        }
    }
    fclose(file); // Close the file to ensure data is written
}



void loadMenuRequirementsFromCSV(const char* Ingredient) {
    FILE* file = fopen("Ingredient.csv", "r");
    if (file == NULL) {
        perror("Error opening menu CSV file");
        return;
    }

    char line[512];
    menuItemCount = 0;

    // Skip the first line (header)
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        if (menuItemCount >= MAX_MENU_ITEMS) {
            printf("Menu capacity exceeded!\n");
            break;
        }

        // Parse the CSV line
        char* token = strtok(line, ",");
        if (token == NULL) continue;

        // Read menu name
        strncpy(menuIngredients[menuItemCount].name, token, sizeof(menuIngredients[menuItemCount].name) - 1);

        // Iterate over ingredients in the row
        while ((token = strtok(NULL, ",")) != NULL) {
            if (strcmp(token, "") != 0) {
                strncpy(menuIngredients[menuItemCount].ingredientName, token, sizeof(menuIngredients[menuItemCount].ingredientName) - 1);

                // Get the amount needed per unit
                token = strtok(NULL, ",");
                if (token != NULL) {
                    menuIngredients[menuItemCount].amountPerUnit = atoi(token);
                }
                menuItemCount++;
            }
        }
    }

    fclose(file);
}


void CutStocks() {
    for (int i = 0; i < cartSize; i++) {
        CartItem* item = &cart[i];

        // Find the matching menu item in the menuIngredients array
        for (int j = 0; j < menuItemCount; j++) {
            if (strcmp(item->name, menuIngredients[j].name) == 0) {
                // Deduct the required stock
                deductStock(menuIngredients[j].ingredientName, menuIngredients[j].amountPerUnit * item->quantity);
            }
        }
    }

    // Save the updated stock to file
    saveStockToCSV("Stock.csv");

    // Load restock rules and perform auto restock
    RestockRule restockRules[MAX_STOCKS];
    int restockRuleCount = loadRestockRules(restockRules, MAX_STOCKS);
    if (restockRuleCount > 0) {
        autoRestock(stocks, &stockCount, restockRules, restockRuleCount);
        saveStock("Stock.csv");  // Save stock after restocking
    }
}



void loadDiscountsFromCSV(const char* Discount) {
    FILE* file = fopen("Discount.csv", "r");
    if (file == NULL) {
        perror("Error opening discount file");
        exit(EXIT_FAILURE);
    }

    char line[512];
    int discountCount = 0;

    // Skip the first line (header)
    fgets(line, sizeof(line), file);

    while (fgets(line, sizeof(line), file)) {
        if (discountCount >= MAX_DISCOUNTS) {
            printf("Discounts capacity exceeded!\n");
            break;
        }

        char* token = strtok(line, ",");
        if (token == NULL) continue;

        // Read the discount code
        strncpy(discounts[discountCount].code, token, sizeof(discounts[discountCount].code) - 1);

        // Read discount percentage
        token = strtok(NULL, ",");
        if (token != NULL) {
            discounts[discountCount].discountPercent = atoi(token);
        }

        // Skip the condition column
        strtok(NULL, ",");

        // Read minimum spend
        token = strtok(NULL, ",");
        if (token != NULL) {
            discounts[discountCount].minSpend = atoi(token);
        }

        discountCount++;
    }

    fclose(file);
}

int applyDiscount(int totalCost) {
    int discountAmount = 0;
    int appliedDiscount = 0;

    // Automatically apply any applicable discount based on total cost
    for (int i = 0; i < discountCount; i++) {
        if (totalCost >= discounts[i].minSpend) {
            // Apply the discount
            discountAmount = (totalCost * discounts[i].discountPercent) / 100;
            appliedDiscount = 1;
            break; // Apply only the first applicable discount
        }
    }

    if (appliedDiscount) {
        printf("A discount of %d%% has been applied!\n", discounts[0].discountPercent);
    } else {
        printf("No discount applied.\n");
    }

    return discountAmount;
}

//logSaleToCSV
void logSaleToCSV(const char *filename) {
    FILE *file = fopen(filename, "a"); // เปิดไฟล์ในโหมด append
    if (file == NULL) {
        printf("Error: Unable to open sales log file.\n");
        return;
    }

    // รับวันที่และเวลาปัจจุบัน
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm_info);

    // เขียนข้อมูลสินค้าในตะกร้าลงไฟล์
    for (int i = 0; i < cartSize; i++) {
        fprintf(file, "%s,%s,%d,%d\n", date, cart[i].name, cart[i].quantity, cart[i].quantity * cart[i].cost);
    }

    fclose(file); // ปิดไฟล์
}


// View purchased items
// Function to manually apply a discount
void purchase() {
    if (cartSize == 0) {
        printf("Your cart is empty! Cannot proceed with the purchase.\n");
        return;
    }

    // Calculate total cost
    int totalCost = 0;
    for (int i = 0; i < cartSize; i++) {
        totalCost += cart[i].quantity * cart[i].cost;
    }

    clearScreen();  // Assuming a clearScreen() function exists
    printf("===================================================================================\n");
    printf("                                Purchase Summary\n");
    printf("===================================================================================\n");
    for (int i = 0; i < cartSize; i++) {
        printf("%d. %s x %d - %d Baht\n", i + 1, cart[i].name, cart[i].quantity, cart[i].quantity * cart[i].cost);
    }
    printf("-----------------------------------------------------------------------------------\n");
    printf("Total cost: %d Baht\n", totalCost);

    // Ask if the user has a discount
    char discountCode[10];
    printf("\nDo you have a discount code? (yes/no): ");
    char answer[4];
    scanf("%3s", answer);
    clearInputBuffer();

    int discountApplied = 0;
    if (strcmp(answer, "yes") == 0) {
        printf("Enter your discount code: ");
        scanf("%s", &discountCode);

        // Find matching discount code
        for (int i = 0; i < discountCount; i++) {
            if (strcmp(discounts[i].code, discountCode) == 0) {
                // Check if the user meets the minimum spend for this discount
                if (totalCost >= discounts[i].minSpend) {
                    int discountAmount = (totalCost * discounts[i].discountPercent) / 100;
                    totalCost -= discountAmount;
                    printf("Discount of %d%% applied! You saved %d Baht.\n", discounts[i].discountPercent, discountAmount);
                    discountApplied = 1;
                } else {
                    printf("You do not meet the minimum spend requirement of %d Baht for this discount.\n", discounts[i].minSpend);
                }
                break;
            }
        }

        if (!discountApplied) {
            printf("Invalid discount code or no discount applied.\n");
        }
    }

    // Confirm purchase
    printf("\nTotal cost after discount: %d Baht\n", totalCost);
    printf("Do you want to proceed with the purchase?\n");
    printf("1. Yes\n");
    printf("2. No\n");
    printf("Enter your choice: ");
    
    int choice;
    scanf("%d", &choice);
    clearInputBuffer();

    if (choice == 1) {
        printf("Processing your payment...\n");
         int payment = 0;
    while (1) {
        printf("Enter the amount of money you are paying (Baht): ");
        scanf("%d", &payment);
        clearInputBuffer();

        if (payment >= totalCost) {
            int change = payment - totalCost;
            printf("Payment received: %d Baht\n", payment);
            printf("Change to be returned: %d Baht\n", change);
            break;
        } else {
            printf("Insufficient payment. You need %d Baht more.\n", totalCost - payment);
        }
    }
        logSaleToCSV("sales_log.csv");

        printf("Transaction successful! Thank you for your purchase.\n");
        loadStockFromCSV("Stock");
        loadMenuRequirementsFromCSV("Ingredient");
        CutStocks();
        // Clear the cart after purchase
        cartSize = 0;
    } else {
        printf("Purchase cancelled. Returning to the menu...\n");
    }
    
    printf("Press Enter to continue...\n");
    getchar();  // Wait for user input before returning to the menu
}

void removeFromCart(int index) {
    if (index < 0 || index >= cartSize) {
        printf("Invalid index! Please try again.\n");
        return;
    }

    // เลื่อนสินค้าใน cart เพื่อแทนที่ตำแหน่งที่ลบ
    for (int i = index; i < cartSize - 1; i++) {
        cart[i] = cart[i + 1];
    }

    cartSize--; // ลดขนาดของ cart 

}



void viewcart() {
    int choice;
    clearScreen();
    printf("===================================================================================\n");
    printf("                                     Cart\n");
    printf("===================================================================================\n");

    if (cartSize == 0) {
        printf("Your cart is empty.\n");
    } else {
        int totalCost = 0;
        for (int i = 0; i < cartSize; i++) {
            printf("%d. %s x %d - %d Baht\n", i + 1, cart[i].name, cart[i].quantity, cart[i].quantity * cart[i].cost);
            totalCost += cart[i].quantity * cart[i].cost;
        }
        printf("---------------------------------------\n");
        printf("Total Cost: %d Baht\n", totalCost);
    }
    printf("1. Purchase\n");
    printf("2. Remove an item\n");
    printf("3. Back\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    clearInputBuffer();
    if (choice == 1){
        loadDiscountsFromCSV("Discount.csv");
        purchase();
    }else if (choice == 2) {
        if (cartSize == 0) {
            printf("Your cart is empty! No items to remove.\n");
            clearInputBuffer();
        } else {
            int index;
            printf("Enter the item number to remove: ");
            scanf("%d", &index);
            clearInputBuffer();
            removeFromCart(index - 1); // เรียกใช้ฟังก์ชัน removeFromCart (ใช้ index-1 เพื่อเริ่มจาก 0)
        }
        viewcart(); // แสดงตะกร้าอีกครั้งหลังจากลบสินค้า
    } else if (choice == 3) {
        return;
    }
    
}

// Add item to cart
void addToCart(const char* name, int quantity, int cost) {
    // Check if the item is already in the cart
    for (int i = 0; i < cartSize; i++) {
        if (strcmp(cart[i].name, name) == 0) {
            cart[i].quantity += quantity; // Update quantity
            return;
        }
    }

    // Add new item to the cart
    if (cartSize < MAX_CART_SIZE) {
        strcpy(cart[cartSize].name, name);
        cart[cartSize].quantity = quantity;
        cart[cartSize].cost = cost;
        cartSize++;
    } else {
        printf("Cart is full! Cannot add more items.\n");
    }
}

#define MAX_MENU_ITEMS 100
#define MAX_NAME_LEN 100
#define MAX_DETAIL_LEN 200
// Menu options and their prices
typedef struct {
    int Code;                     
    char Name[MAX_NAME_LEN];      
    int Price;  
    char Description[MAX_DETAIL_LEN]; 
} MenuItem;

MenuItem menu[MAX_MENU_ITEMS];
int menuSize = 0;

void loadMenuFromCSV(const char* Menu) {
    FILE* file = fopen("Menu.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char line[512];
    menuSize = 0;

    while (fgets(line, sizeof(line), file)) {
        if (menuSize >= MAX_MENU_ITEMS) {
            printf("Menu capacity exceeded!\n");
            break;
        }

        char* token = strtok(line, ",");
        if (token == NULL) continue;

        // อ่านโค้ดเมนู
        menu[menuSize].Code = atoi(token);

        // อ่านชื่อเมนู
        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(menu[menuSize].Name, token, MAX_NAME_LEN);
            menu[menuSize].Name[MAX_NAME_LEN - 1] = '\0';
        }

        // อ่านราคาเมนู
        token = strtok(NULL, ",");
        if (token != NULL) {
            menu[menuSize].Price = atoi(token);
        }

        // อ่านรายละเอียดเมนู
        token = strtok(NULL, ",");
        if (token != NULL) {
            strncpy(menu[menuSize].Description, token, MAX_DETAIL_LEN);
            menu[menuSize].Description[MAX_DETAIL_LEN - 1] = '\0';
        }

        menuSize++;
    }

    fclose(file);
    for (int i = 0; i < menuSize; i++) {
        printf("Menu item %d: %s, %d Baht\n", menu[i].Code, menu[i].Name, menu[i].Price);
    }
}

// Convert a string to lowercase
void toLowerCase(const char* src, char* dest) {
    while (*src) {
        *dest++ = tolower(*src++);
    }
    *dest = '\0';
}

int searchItem(const char* query, int* results) {
    char lowerQuery[50];
    toLowerCase(query, lowerQuery); // แปลง query เป็นพิมพ์เล็ก
    int count = 0;

    for (int i = 0; i < menuSize; i++) {
        char lowerMenuItem[MAX_NAME_LEN];
        toLowerCase(menu[i].Name, lowerMenuItem); // แปลงรายการใน menuItems เป็นพิมพ์เล็ก

        if (strstr(lowerMenuItem, lowerQuery) != NULL) {
            results[count++] = i; // เก็บดัชนีของเมนูที่ตรงกัน
        }
    }

    return count; // ส่งจำนวนผลลัพธ์ที่พบกลับ
}

void viewItemDetails(int choice){
    int a;
    int quantity;
            clearScreen();
            printf("===================================================================================\n");
            printf("    %s\n", menu[choice].Name);
            printf("===================================================================================\n");
            printf("%s\n", menu[choice].Description);
            printf("Cost: %d Baht\n", menu[choice].Price);
            printf("1. Add to cart.\n");
            printf("2. Back to available items.\n");
            printf("Enter your choice : ");
            scanf("%d",&a);
            if(a == 1){
                printf("Enter the quantity: ");
                scanf("%d", &quantity);
                clearInputBuffer();
            }else if(a == 2){
                return;
            }
            
            addToCart(menu[choice].Name, quantity, menu[choice].Price);
            printf("Added %d x %s to the cart.\n", quantity, menu[choice].Name);
            printf("Press Enter to continue...");
            getchar();
}

// Display available items
void viewAvailableItems() {
    int foodchoice, quantity;
    do {
        clearScreen();
        printf("===================================================================================\n");
        printf("                                Available Items\n");
        printf("===================================================================================\n");
        for (int i = 1; i < menuSize; i++) {
            // แสดงข้อมูลเมนู
            printf("[%d]. %s -- %d Baht\n", menu[i].Code, menu[i].Name, menu[i].Price);
        }
        printf("===================================================================================\n");
        printf("%d. Search items\n",menuSize+1);
        printf("%d. View Cart\n",menuSize+2);
        printf("0. Exit\n");
        printf("Enter Your Choice: ");
        scanf("%d", &foodchoice);
        clearInputBuffer();

        if (foodchoice >= 1 && foodchoice <= menuSize) {
            // Item selected
            viewItemDetails(foodchoice);
        } else if(foodchoice == menuSize+1){
            clearScreen();
            printf("Enter the item name or keyword to search: ");
            char query[50];
            fgets(query, sizeof(query), stdin);
            query[strcspn(query, "\n")] = 0; // Remove newline character

            int results[MAX_MENU_ITEMS];
            int matches = searchItem(query, results);

            if (matches == 1) {
                // Only one match found, view details directly
                printf("Item found: %s\n", menu[results[0]].Name);
                printf("Press Enter to view details...");
                getchar();
                viewItemDetails(results[0]);
            } else if (matches > 1) {
                // Multiple matches found
                printf("Multiple items found:\n");
                for (int i = 1; i < matches; i++) {
                    printf("%d. %s -- %d Baht\n", i + 1, menu[results[i]].Name, menu[results[i]].Price);
                }
                printf("Enter the number of the item you want to view: ");
                int selectedIndex;
                scanf("%d", &selectedIndex);
                clearInputBuffer();

                if (selectedIndex >= 1 && selectedIndex <= matches) {
                    viewItemDetails(results[selectedIndex - 1]);
                } else {
                    printf("Invalid choice. Press Enter to return to the menu...");
                    getchar();
                }
            } else {
                // No matches found
                printf("Item not found. Press Enter to return to the menu...");
                getchar();
            }
        } else if (foodchoice == menuSize+2) {
            // View cart
            viewcart();
        } else if (foodchoice != 0) {
            // Invalid choice
            printf("Invalid choice. Please try again.\n");
            printf("Press Enter to continue...");
            getchar();
        }
    } while (foodchoice != 0);
}

// Customer Menu

void customerMenu() {
    int customerChoice;
    do {
        clearScreen();
        printf("===================================================================================\n");
        printf("                            CUSTOMER FEATURES MENU\n");
        printf("===================================================================================\n");
        printf("1. View Available Items\n");
        printf("2. Back to Main Menu\n");
        printf("-----------------------------------------------------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &customerChoice);
        clearInputBuffer();  

        switch (customerChoice) {
            case 1:
                {
                    int productChoice;
                    do {
                        clearScreen();
                        printf("===================================================================================\n");
                        printf("                            View Available Items Menu\n");
                        printf("===================================================================================\n");
                        printf("1. View All Items\n");
                        printf("2. Back to Customer Menu\n");
                        printf("-----------------------------------------------------------------------------------\n");
                        printf("Enter your choice: ");
                        scanf("%d", &productChoice);
                        clearInputBuffer();  

                        switch (productChoice) {
                            case 1:
                                loadMenuFromCSV("Menu");
                                viewAvailableItems();
                                break;
                            case 2:
                                return;
                            default:
                                printf("Invalid choice. Please try again.\n");
                                printf("Press Enter to continue...");
                                getchar();  
                                break;
                        }

                    } while (productChoice != 2);
                        break;
                }
            case 2:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  // Wait for the user to press Enter
                break;
        }

    } while (customerChoice != 3);
}

int isAutoPurchasedToday() {
    FILE *file = fopen(FLAG_FILE, "a+"); // Open in append+read mode
    if (file == NULL) {
        printf("Error: Unable to open or create flag file.\n");
        return 1;
    }
    char today[20];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(today, sizeof(today), "%Y-%m-%d", tm_info); // Format today's date as "YYYY-MM-DD"
    // Write the header if the file is empty
    fseek(file, 0, SEEK_END); // Go to end of file
    if (ftell(file) == 0) {   // Check if file is empty
        fprintf(file, "date,flag\n");
    }
    // Append today's date
    fprintf(file, "%s,1\n", today);
    fclose(file);
    return 0;
}
// Function to update the flag file with today's date
void updateFlagFile() {
    FILE *file = fopen(FLAG_FILE, "a"); // Append mode
    if (file == NULL) {
        printf("Error: Unable to open flag file for writing.\n");
        return;
    }
    char today[20];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(today, sizeof(today), "%Y-%m-%d", tm_info); // Get today's date as string
    // Write today's date and flag to the file
    fprintf(file, "%s,1\n", today); // '1' indicates auto-purchase done
    fclose(file);
}
int timecheck() {
    FILE *file = fopen(FLAG_FILE, "a+"); // Open flag file in read+write mode
    if (file == NULL) {
        // If file doesn't exist, create it with header
        file = fopen(FLAG_FILE, "w");
        if (file == NULL) {
            printf("Error: Unable to create flag file.\n");
            return 0;
        }
        fprintf(file, "date,flag\n"); // Write header
        fclose(file);
        file = fopen(FLAG_FILE, "r+"); // Reopen in read+write mode
    }
    char today[20];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(today, sizeof(today), "%Y-%m-%d", tm_info); // Format today's date as "YYYY-MM-DD"
    char line[50];
    char date[20], flag[5];
    int alreadyPurchased = 0;
    // Check if today's date is already recorded
    while (fgets(line, sizeof(line), file)) {
        if (sscanf(line, "%19[^,],%4s", date, flag) == 2) {
            if (strcmp(date, today) == 0 && strcmp(flag, "1") == 0) {
                alreadyPurchased = 1; // Found today's date
                break;
            }
        }
    }
    // If not already purchased and today is Saturday, append today's date
    if (!alreadyPurchased && tm_info->tm_wday == 3) { // 6 = Saturday
        fseek(file, 0, SEEK_END); // Move to end of file
        fprintf(file, "%s,1\n", today);
    }
    fclose(file);
    return !alreadyPurchased; // Return 1 if auto-purchase performed, 0 otherwise
}
void autologSaleToCSV(const char *filename) {
    FILE *file = fopen(filename, "a"); // เปิดไฟล์ในโหมด append
    if (file == NULL) {
        printf("Error: Unable to open sales log file.\n");
        return;
    }
    // รับวันที่และเวลาปัจจุบัน
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char date[20];
    strftime(date, sizeof(date), "%Y-%m-%d %H:%M:%S", tm_info);
    const char *name = "All In One Non-Seafood";
    int quantity = 1, cost = 150;
    
    fprintf(file, "%s,%s,%d,%d\n", date, name, quantity, cost);
    
    fclose(file); // ปิดไฟล์
}
void autoCutStocks() {
    loadStockFromCSV("Stock.csv");
    loadMenuRequirementsFromCSV("Ingredient.csv");
    const char* selectedMenuName = "All In One Non-Seafood"; // Hardcoded menu name
    for (int j = 0; j < menuItemCount; j++) {
        if (strcmp(selectedMenuName, menuIngredients[j].name) == 0) {
            // Deduct stock for each ingredient in the selected menu
            deductStock(menuIngredients[j].ingredientName,
                        menuIngredients[j].amountPerUnit);
        }
    }
    saveStockToCSV("Stock.csv");
}
int autoPurchased(){
    int a=1;
    if (timecheck()) {
        autoCutStocks();  // Simulate cutting stock
        return a;
    } else {
        return 0;
    }
    
}

// Main Menu

void mainMenu() {
    int choice;
    do {
        clearScreen();
        printf("===================================================================================\n");
        printf("                        INVENTORY MANAGEMENT SYSTEM\n");
        printf("===================================================================================\n");
        printf("1. Owner Features\n");
        printf("2. Customer Features\n");
        printf("3. Exit\n");
        printf("-----------------------------------------------------------------------------------\n");
         if (autoPurchased()==1){
            
            printf("--Auto Purchased x1 All In One Non-Seafood--\n");
            printf("-----------------------------------------------------------------------------------\n");
            autologSaleToCSV("sales_log.csv");
        }
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();  

        switch (choice) {
            case 1:
                ownerMenu();
                break;
            case 2:
                customerMenu();
                break;
            case 3:
                printf("Exiting the system.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar(); 
                break;
        }

    } while (choice != 3);
}

int main() {
   mainMenu();
    return 0;
    }