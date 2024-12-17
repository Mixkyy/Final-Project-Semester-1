
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
