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
    printf("1. Create A Coupon\n");
    printf("2. Delete A Coupon\n");
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
        printf("5. Back to Main Menu\n");
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
                ManageCouponsMenu();
                break;
            case 5:
                return;
            default:
                printf("Invalid choice. Please try again.\n");
                printf("Press Enter to continue...");
                getchar();  
                break;
        }

    } while (ownerChoice != 5);
}

// View Avaiable Items Menu

void viewAvailableItems() {
    clearScreen(); 
    printf("=======================================\n");
    printf("            Available Items\n");
    printf("=======================================\n");
    printf("1. Item 1\n");
    printf("2. Item 2\n");
    printf("3. Item 3\n");
    printf("4. Item 4\n");
    printf("=======================================\n");
    printf("Press Enter to go back...");
    getchar();  
    
}



void searchForItem() {
    char searchTerm[100];
    printf("Enter the name of the item to search: ");
    getchar(); 
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0; 

    if (strcmp(searchTerm, "Item 1") == 0) {
        printf("Item 1 found!\n");
    } else if (strcmp(searchTerm, "Item 2") == 0) {
        printf("Item 2 found!\n");
    } else if (strcmp(searchTerm, "Item 3") == 0) {
        printf("Item 3 found!\n");
    } else if (strcmp(searchTerm, "Item 4") == 0) {
        printf("Item 4 found!\n");
    } else {
        printf("Item not found.\n");
    }

    printf("Press Enter to continue...");
    getchar();  
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
        printf("2. Make a Purchase\n");
        printf("3. Back to Main Menu\n");
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
                        printf("2. Search for an Item\n");
                        printf("3. Back to Customer Menu\n");
                        printf("---------------------------------------\n");
                        printf("Enter your choice: ");
                        scanf("%d", &productChoice);
                        clearInputBuffer();  

                        switch (productChoice) {
                            case 1:
                                viewAvailableItems();
                                break;
                            case 2:
                                searchForItem();
                                break;
                            case 3:
                                customerMenu();
                                break;
                            default:
                                printf("Invalid choice. Please try again.\n");
                                printf("Press Enter to continue...");
                                getchar();  
                                break;
                        }

                    } while (productChoice != 3);
                    break;
                }
            case 2:
                break;
            case 3:
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
