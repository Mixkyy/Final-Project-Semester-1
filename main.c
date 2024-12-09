#include <stdio.h>
#include <stdlib.h>  

void clearScreen() {
    #ifdef _WIN32
        system("cls");  // For Windows
    #else
        system("clear");  
    #endif
}

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

        switch (ownerChoice) {
            case 1:
                printf("Viewing Reports...\n");
                break;
            case 2:
                printf("Accessing CRUD Operations...\n");
                break;
            case 3:
                printf("Restocking Items...\n");
                break;
            case 4:
                printf("Managing Discount Coupons...\n");
                break;
            case 5:
                printf("Returning to Main Menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (ownerChoice != 5);  // Return to Main Menu when the user selects 5
}

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

        switch (customerChoice) {
            case 1:
                printf("Viewing Available Items...\n");
                break;
            case 2:
                printf("Processing Purchase...\n");
                break;
            case 3:
                printf("Returning to Main Menu...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (customerChoice != 3); 
}

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

        switch (choice) {
            case 1:
                ownerMenu();  
                break;
            case 2:
                customerMenu(); 
                break;
            case 3:
                printf("Exiting the system\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

int main() {
    mainMenu();
    return 0;
}
