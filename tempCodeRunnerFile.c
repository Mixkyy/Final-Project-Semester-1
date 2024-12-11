int readCSV(const char *filename, Stock items[], int *rowCount);
void writeCSV(const char *filename, Stock items[], int rowCount);
int getUniqueIngredients(Stock items[], int rowCount, char uniqueNames[][50]);
void displayCombinedStock(Stock items[], int rowCount, char uniqueNames[][50], int uniqueCount);
void restockMenu(Stock items[], int *rowCount, char uniqueNames[][50], int uniqueCount);