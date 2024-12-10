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
    printf("3. Return to Owner Features\n");
    printf("=======================================\n");
    printf("Enter your choice: ");
        scanf("%d", &CouponChoice);
        clearInputBuffer();
        switch (CouponChoice) {
            case 1:
                break;
            case 2:
                break;

            case 3:
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

// CRUD Operation Menu

void CRUDoperationMenu() {
    int CRUDchoice;
    clearScreen();
    printf("=======================================\n");
    printf("            CRUD Operations\n");
    printf("=======================================\n");
    printf("1. Create A New Product\n");
    printf("2. View Product\n");
    printf("3. Edit A Product\n");
    printf("4. Delete A Product\n");
    printf("5. Return to Owner Features\n");
    printf("=======================================\n");
    printf("Enter your choice: ");
        scanf("%d", &CRUDchoice);
        clearInputBuffer();
        switch (CRUDchoice) {
            case 1:
                break;
            case 2:
                break;
            case 3:
                break;
            case 4:
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  
                CRUDoperationMenu();
        }
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
        printf("4. Manage Discount Coupons\n");
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
            printf("Enter your choice :");
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

#define MAX_LINE_LENGTH 1024
#define MAX_ROWS 100
typedef struct {
    char id[10];
    char name[50];
    int quantity;
    char unit[10];
    char re_stock[20];
    char expire[20];
} Item;
// Function prototypes
int readProduct(const char *filename, Item items[], int *rowCount);
void writeProduct(const char *filename, Item items[], int rowCount);

int main() {
   mainMenu();
    return 0;
    Item items[MAX_ROWS];
    int rowCount = 0;

    // Read the CSV file
    if (!readProduct("Product.csv", items, &rowCount)) {
        printf("Error reading the file.\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < rowCount; i++) {
        if (strcmp(items[i].id, "N1") == 0) {
            items[i].quantity = 40;
        }
    }

    // Overwrite the original file with the updated content
    writeProduct("Product.csv", items, rowCount);

    printf("the data have update to Product.csv\n");
    return EXIT_SUCCESS;
}
int readProduct(const char *filename, Item items[], int *rowCount) {
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
            isHeader = 0; // Skip the header
            continue;
        }

        // Parse the row and populate the structure
        sscanf(line, "%[^,],%[^,],%d,%[^,],%[^,],%s",
               items[*rowCount].id,
               items[*rowCount].name,
               &items[*rowCount].quantity,
               items[*rowCount].unit,
               items[*rowCount].re_stock,
               items[*rowCount].expire);

        (*rowCount)++;
    }

    fclose(file);
    return 1;
}

// Function to overwrite the CSV file with updated data
void writeProduct(const char *filename, Item items[], int rowCount) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file");
        return;
    }

    // Write the header
    fprintf(file, "id,name,quantity,unit,re-stock,expire\n");

    // Write each row of data
    for (int i = 0; i < rowCount; i++) {
        fprintf(file, "%s,%s,%d,%s,%s,%s\n",
                items[i].id,
                items[i].name,
                items[i].quantity,
                items[i].unit,
                items[i].re_stock,
                items[i].expire);
    }

    fclose(file);
}

