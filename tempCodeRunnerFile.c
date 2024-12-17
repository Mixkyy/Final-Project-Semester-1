void DeleteMenuItem() {
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

    if (found) {
        remove("Menu.csv");
        rename("TempMenu.csv", "Menu.csv");
        printf("Menu item with code '%s' deleted successfully!\n", codeToDelete);
    } else {
        remove("TempMenu.csv");
        printf("Menu item with code '%s' not found.\n", codeToDelete);
    }

    printf("Press Enter to return to the menu...\n");
    getchar();
}