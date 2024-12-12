void CreateNewMenuItem() {
    clearScreen();
    char filename[] = "Menu.csv";
    FILE *file = fopen(filename, "a");  // Open file in append mode
    if (!file) {
        perror("Error opening file");
        return;
    }

    char code[10];
    char name[50];
    int price;
    char description[100];

    printf("=======================================\n");
    printf("          Add New Menu Item\n");
    printf("=======================================\n");

    // Input new menu item details
    printf("Enter Menu Code (numeric): ");
    fgets(code, sizeof(code), stdin);
    code[strcspn(code, "\n")] = '\0';  // Remove newline character

    printf("Enter Menu Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';  // Remove newline character

    printf("Enter Price: ");
    scanf("%d", &price);
    clearInputBuffer();  // Clear input buffer after scanf

    printf("Enter Description: ");
    fgets(description, sizeof(description), stdin);
    description[strcspn(description, "\n")] = '\0';  // Remove newline character

    // Append the new item to the file
    fprintf(file, "%s,%s,%d,%s\n", code, name, price, description);
    fclose(file);

    printf("New menu item added successfully!\n");
    printf("Press Enter to return to the menu...\n");
    getchar();
}
