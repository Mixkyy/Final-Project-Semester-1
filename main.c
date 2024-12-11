#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define MAX_LINE_LENGTH 256

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

void ViewDiscountCoupon() {
    DiscountCoupon coupons[MAX_ROWS];
    int rowCount = 0;

    if (!readDiscountCoupons("Discount.csv", coupons, &rowCount)) {
        printf("Error reading the file.\n");
        return;
    }

    clearScreen();
    printf("=======================================\n");
    printf("        AVAILABLE DISCOUNT COUPONS\n");
    printf("=======================================\n");
    printf("Code            Discount [%%]      Condition\n");
    printf("-----------------------------------------------\n");

    for (int i = 0; i < rowCount; i++) {
        printf("%-15s%-18d%-20s\n",
               coupons[i].code,
               coupons[i].discount,
               coupons[i].condition);
    }

    printf("=======================================\n");
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

    printf("Enter Minimum Spend: ");
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
    printf("=======================================\n");
    printf("         Manage Discount Coupons\n");
    printf("=======================================\n");
    printf("1. View Discount Coupons\n");
    printf("2. Create A Coupon\n");
    printf("3. Delete A Coupon\n");
    printf("4. Return to Owner Features\n");
    printf("=======================================\n");
    printf("Enter your choice: ");
        scanf("%d", &CouponChoice);
        clearInputBuffer();
        switch (CouponChoice) {
            case 1:
                ViewDiscountCoupon();
                break;
            case 2:
                CreateCoupon();
                break;
            case 3:
                DeleteCoupon();
                break;
            case 4:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  
                return;
        }
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
    printf("===============================================================\n");
    printf("                         MENU\n");
    printf("===============================================================\n");
    printf("Code    Name                                             Price     Description\n");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < rowCount; i++) {
        printf("%-7s%-50s%-10.2f%-30s\n",
               menu[i].code,
               menu[i].name,
               menu[i].price,
               menu[i].description);
    }

    printf("===============================================================\n");
    printf("Press Enter to return to the menu...\n");
    getchar();
}



// Menu CRUD operations

void MenuCRUD() {
    int MenuCRUDchoice;
    clearScreen();
    printf("=======================================\n");
    printf("          Menu CRUD Operations\n");
    printf("=======================================\n");
    printf("1. Create A New Menu\n");
    printf("2. View Menu\n");
    printf("3. Edit An Existing Menu\n");
    printf("4. Delete Menu\n");
    printf("5. Return To Owner Features\n");
    printf("=======================================\n");
    printf("Enter your choice: ");
        scanf("%d", &MenuCRUDchoice);
        clearInputBuffer();
        switch (MenuCRUDchoice) {
            case 1:
                break;
            case 2:
                ViewMenu();
                break;
            case 3:
                break;
            case 4:
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
    printf("===============================================================\n");
    printf("                         STOCK\n");
    printf("===============================================================\n");
    printf("ID      Name                                        Quantity    Unit    Re-stock Date  Expiry Date\n");
    printf("---------------------------------------------------------------\n");

    for (int i = 0; i < rowCount; i++) {
        printf("%-7s%-50s%-12d%-8s%-15s%-15s\n",
               stock[i].id,
               stock[i].name,
               stock[i].quantity,
               stock[i].unit,
               stock[i].restock[0] == '\0' ? "N/A" : stock[i].restock,
               stock[i].expire[0] == '\0' ? "N/A" : stock[i].expire);
    }

    printf("===============================================================\n");
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


// Stock CRUD Operations

void StockCRUD() {
    int StockCRUDchoice;
    clearScreen();
    printf("=======================================\n");
    printf("          Stock CRUD Operations\n");
    printf("=======================================\n");
    printf("1. Create A New Stock\n");
    printf("2. View Stock\n");
    printf("3. Edit An Existing Stock\n");
    printf("4. Delete Stock\n");
    printf("5. Return To Owner Features\n");
    printf("=======================================\n");
    printf("Enter your choice: ");
        scanf("%d", &StockCRUDchoice);
        clearInputBuffer();
        switch (StockCRUDchoice) {
            case 1:
                AddNewStock();
                break;
            case 2:
                ViewStock();
                break;
            case 3:
                break;
            case 4:
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
    printf("=======================================\n");
    printf("            CRUD Operations\n");
    printf("=======================================\n");
    printf("1. Edit Menu\n");
    printf("2. Edit Stock\n");
    printf("3. Return to Owner Features\n");
    printf("=======================================\n");
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
    printf("=======================================\n");
    printf("             Current Stock             \n");
    printf("=======================================\n");

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

    printf("=======================================\n");
}

// Display Updated Combined Stock

void displayUpdatedCombinedStock(Item items[], int rowCount, char uniqueNames[][50], int uniqueCount) {
    printf("=======================================\n");
    printf("             Updated Stock             \n");
    printf("=======================================\n");

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

    printf("=======================================\n");
    printf("Press Enter to continue...");
    getchar(); // Wait for user input
    getchar(); // Handle newline left by previous input
    clearScreen();
    return;
}


void restockMenu(Item items[], int *rowCount, char uniqueNames[][50], int uniqueCount) {
    int choice, restockAmount;
    char restockDate[20], expireDate[20];

    displayCombinedStock(items, *rowCount, uniqueNames, uniqueCount);

    printf("Select an ingredient to restock (1-%d): ", uniqueCount);
    scanf("%d", &choice);

    if (choice < 1 || choice > uniqueCount) {
        printf("Invalid choice. Exiting restock process.\n");
        return;
    }

    choice--;

    printf("Enter restock amount for %s : ", uniqueNames[choice]);
    scanf("%d", &restockAmount);

    printf("Enter restock date (e.g., 1/1/2025): ");
    scanf("%s", restockDate);

    printf("Enter expiration date (e.g., 7/1/2025): ");
    scanf("%s", expireDate);

    int restocked = 0;

    for (int i = 0; i < *rowCount; i++) {
        if (strcmp(items[i].name, uniqueNames[choice]) == 0 && items[i].quantity == 0) {
            items[i].quantity += restockAmount;
            strcpy(items[i].restock, restockDate);
            strcpy(items[i].expire, expireDate);
            restocked = 1;
            break;
        }
    }

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

    if (!restocked) {
        if (*rowCount < MAX_ROWS) {
            Item newItem;
            sprintf(newItem.id, "NEW%d", *rowCount + 1);
            strcpy(newItem.name, uniqueNames[choice]);
            newItem.quantity = restockAmount;
            strcpy(newItem.unit, items[choice].unit);
            strcpy(newItem.restock, restockDate);
            strcpy(newItem.expire, expireDate);
            items[*rowCount] = newItem;
            (*rowCount)++;
        } else {
            printf("Error: Cannot add new row. Maximum capacity reached.\n");
        }
    }
    clearScreen();

    printf("Restocked %s. Updated inventory:\n", uniqueNames[choice]);
    displayUpdatedCombinedStock(items, *rowCount, uniqueNames, uniqueCount);
}


// Owner Menu

void ownerMenu() {
    int ownerChoice;
    do {
        clearScreen();
        printf("=======================================\n");
        printf("        OWNER FEATURES MENU\n");
        printf("=======================================\n");
        printf("1. View Reports\n");
        printf("2. Perform CRUD Operations\n");
        printf("3. Restock Items\n");
        printf("4. View Logs\n");
        printf("5. Manage Discount Coupons\n");
        printf("6. Back to Main Menu\n");
        printf("---------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &ownerChoice);
        clearInputBuffer();

        switch (ownerChoice) {
            case 1:
                break;
            case 2:
                CRUDoperationMenu();
                break;
            case 3:
                RestockFunction();
                break;
            case 4:
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

// Maximum cart size
#define MAX_CART_SIZE 50

// Global cart and cart size
CartItem cart[MAX_CART_SIZE];
int cartSize = 0;

// View purchased items
void purchase(){

}

void viewcart() {
    int choice;
    clearScreen();
    printf("=======================================\n");
    printf("                Cart\n");
    printf("=======================================\n");

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
    printf("2. Back\n");
    printf("Enter your choice : ");
    scanf("%d",&choice);
    if (choice == 1){
        purchase();
    }else if(choice == 2){
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

// Menu options and their prices
const char* menuItems[] = {
    "Roasted Pork Noodles",
    "Roasted Pork Noodles with Shrimp Wonton",
    "Roasted Chicken Noodles with Shrimp Wonton",
    "Shrimp Noodles with Egg",
    "Crab Noodles with Shrimp Wonton",
    "All In One Non-Seafood",
    "Crispy Pork Noodles with Pork Wonton",
    "Minced Pork Noodles with Pork Wonton"
};

const char* menudetails[] = {
    "Noodles / Vegetables / Roasted Pork",
    "Noodles / Vegetables / Roasted Pork / Shrimp Wonton",
    "Noodles / Vegetables / Roasted Chicken / Shrimp Wonton",
    "Noodles / Vegetables / Shrimp / Egg",
    "Noodles / Vegetables / Crab / Shrimp Wonton",
    "Noodles / Vegetables / Roasted Pork/ Roasted Chicken / Minced Pork/ Crispy Pork / Shrimp Wonton / Pork Wonton",
    "Noodles / Vegetables / Crispy Pork / Pork Wonton",
    "Noodles / Vegetables / Minced Pork / Pork Wanton"
};

const int menuPrices[] = {50, 70, 80, 90, 100, 150, 80, 70};

int searchItem(const char* query) {
    for (int i = 0; i < 8; i++) {
        if (strstr(menuItems[i], query) != NULL) {
            return i; // Return the index of the matching item
        }
    }
    return -1; // Return -1 if no match is found
}

void viewItemDetails(int choice){
    int a;
    int quantity;
            clearScreen();
            printf("=======================================\n");
            printf("    %s\n", menuItems[choice]);
            printf("=======================================\n");
            printf("%s\n",menudetails[choice-1]);
            printf("Cost: %d Baht\n", menuPrices[choice]);
            printf("1. Add to cart.\n");
            printf("2. Back to available items.\n");
            printf("Enter your choice : ");
            scanf("%d",&a);
            if(a == 1){
                printf("Enter the quantity: ");
                scanf("%d", &quantity);
            }else if(a == 2){
                return;
            }
            clearInputBuffer();
            addToCart(menuItems[choice - 1], quantity, menuPrices[choice]);
            printf("Added %d x %s to the cart.\n", quantity, menuItems[choice]);
            printf("Press Enter to continue...");
            getchar();
}

// Display available items
void viewAvailableItems() {
    int foodchoice, quantity;
    do {
        clearScreen();
        printf("=======================================\n");
        printf("            Available Items\n");
        printf("=======================================\n");
        for (int i = 0; i < 8; i++) {
            printf("%d. %s -- %d Baht\n", i + 1, menuItems[i], menuPrices[i]);
        }
        printf("=======================================\n");
        printf("9. Search items\n");
        printf("10. View Cart\n");
        printf("0. Exit\n");
        printf("Enter Your Choice: ");
        scanf("%d", &foodchoice);
        clearInputBuffer();

        if (foodchoice >= 1 && foodchoice <= 8) {
            // Item selected
            viewItemDetails(foodchoice - 1);
        } else if(foodchoice == 9){
            clearScreen();
            printf("Enter the item name or keyword to search: ");
            char query[50];
            fgets(query, sizeof(query), stdin);
            query[strcspn(query, "\n")] = 0; // Remove newline character

            int result = searchItem(query);
            if (result != -1) {
                printf("Item found: %s\n", menuItems[result]);
                printf("Press Enter to view details...");
                getchar();
                viewItemDetails(result);
            } else {
                printf("Item not found. Press Enter to return to the menu...");
                getchar();
            }
        } else if (foodchoice == 10) {
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
        printf("=======================================\n");
        printf("       CUSTOMER FEATURES MENU\n");
        printf("=======================================\n");
        printf("1. View Available Items\n");
        printf("2. Back to Main Menu\n");
        printf("---------------------------------------\n");
        printf("Enter your choice: ");
        scanf("%d", &customerChoice);
        clearInputBuffer();  

        switch (customerChoice) {
            case 1:
                {
                    int productChoice;
                    do {
                        clearScreen();
                        printf("=======================================\n");
                        printf("     View Available Items Menu\n");
                        printf("=======================================\n");
                        printf("1. View All Items\n");
                        printf("2. Back to Customer Menu\n");
                        printf("---------------------------------------\n");
                        printf("Enter your choice: ");
                        scanf("%d", &productChoice);
                        clearInputBuffer();  

                        switch (productChoice) {
                            case 1:
                                viewAvailableItems();
                                break;
                            case 2:
                                customerMenu();
                                break;
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

// Main Menu

void mainMenu() {
    int choice;
    do {
        clearScreen();
        printf("=======================================\n");
        printf("  INVENTORY MANAGEMENT SYSTEM\n");
        printf("=======================================\n");
        printf("1. Owner Features\n");
        printf("2. Customer Features\n");
        printf("3. Exit\n");
        printf("---------------------------------------\n");
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