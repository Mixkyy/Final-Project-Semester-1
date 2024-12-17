<<<<<<< HEAD

<<<<<<< HEAD
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
=======
    while (getchar() != '\n');
>>>>>>> 384deac9f17c31113b2b2f938c8e2df39dffc0d0
=======
stockCountLogging
>>>>>>> d78860d5750145e161c66db390ea934663dfa4ed
