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
    if (newName[0] != '\n') {
        strncpy(menu[foundIndex].name, newName, sizeof(menu[foundIndex].name) - 1);
    }

    char priceInput[20];
    printf("Enter new price (current: %.2f): ", menu[foundIndex].price);
    fgets(priceInput, sizeof(priceInput), stdin);

    // Remove the newline character if it exists
    priceInput[strcspn(priceInput, "\n")] = '\0';

    if (priceInput[0] != '\0') {
        menu[foundIndex].price = atof(priceInput);
    }

    char newDescription[100];
    printf("Enter new description (current: %s): ", menu[foundIndex].description);
    fgets(newDescription, sizeof(newDescription), stdin);
    if (newDescription[0] != '\n') {
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
        if (i == foundIndex) {
            fprintf(tempFile, "%s,%s,%.2f,%s\n", menu[i].code, menu[i].name, menu[i].price, menu[i].description);
        } else {
            fprintf(tempFile, "%s,%s,%.2f,%s\n", menu[i].code, menu[i].name, menu[i].price, menu[i].description);
        }
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

