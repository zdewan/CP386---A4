#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <pthread.h> */
#include <sys/stat.h>
#include <time.h>

int *available;
int **allocated;
int **need;

int totalCustomers = 0;


//Read the input file
char *readFile() {
	FILE *sample = fopen("sample4_in.txt", "r");

	if (!sample) {
		printf("No File Found!\n");
		return ("ERROR!");
	}

	struct stat st;
	fstat(fileno(sample), &st);
	char* fileContent = (char*) malloc(((int) st.st_size + 1) * sizeof(char));
	fileContent[0] = '\0';

	while (!feof(sample)) {
		char request[100];
		if (fgets(request, 100, sample) != NULL)
			strncat(fileContent, request, strlen(request));
		totalCustomers++;
	}

	fclose(sample);
	return fileContent;
}

void checkNeed(int a, int b, int **allocated, int maximum[a][b], int **need) {
	for (int c = 0; c < b; c++) {
		for (int d = 0; d < a; d++)
			need[c][d] = maximum[c][d] - allocated[c][d];
	}
}
int main(int argc, char *argv[]) {
	int availableSize = argc - 1;
	char * fileInput = readFile();

//Dynamic allocation for: memory for available, allocated, need, and maximum - in preparation for the bankers algorithm.

//available
	available = (int*) malloc(availableSize * sizeof(int));
	for (int i = 0; i < availableSize; i++) {
		available[i] = atoi(argv[i + 1]);
	}

	//allocated
	allocated = (int **) malloc(totalCustomers * sizeof(int *));
	for (int i = 0; i < totalCustomers; i++) {
		allocated[i] = (int *) malloc(availableSize * sizeof(int));
	}
	// Initialize allocated array to zeros
	for (int i = 0; i < totalCustomers; i++) {
		for (int j = 0; j < availableSize; j++)
			allocated[i][j] = 0;
	}

	//need
	need = (int **) malloc(totalCustomers * sizeof(int *));
	for (int i = 0; i < totalCustomers; i++) {
		need[i] = (int *) malloc(availableSize * sizeof(int));
	}

	//Maximum
	int maximum[totalCustomers][availableSize];
	for (int m = 0; m < totalCustomers; m++) {
		for (int n = 0; n < availableSize; n++) {
			maximum[m][n] = 0;
		}
	}

	//Token the input given in the file to separate each value into some value maximum[i][j]
	char* lines[totalCustomers];
	char *command = NULL;
	int i = 0;
	command = strtok(fileInput, "\r\n");
	while (command != NULL) {
		lines[i] = malloc(sizeof(command) * sizeof(char));
		strcpy(lines[i], command);
		i++;
		command = strtok(NULL, "\r\n");
	}

	for (int k = 0; k < totalCustomers; k++) {
		char* tok = NULL;
		int l = 0;
		tok = strtok(lines[k], ",");
		while (tok != NULL) {
			maximum[k][l] = atoi(tok);
			l++;
			tok = strtok(NULL, ",");
		}
	}

//Now that all the values have been initialized, we can begin to run the main part of the program in which the user can run various commands.

//Start by printing number of customers, available resources, and maximum resources.
	printf("Number of Customers:%d", totalCustomers);

	printf("\nCurrently Available resources: ");
	for (int n = 0; n < availableSize; n++) {
		printf("%d ", available[n]);
	}

	printf("\nMaximum resources from file:\n");
	for (int n = 0; n < totalCustomers; n++) {
		for (int m = 0; m < availableSize; m++) {
			printf("%d ", maximum[n][m]);
		}
		printf("\n");
	}

//Start the ongoing while loop which takes user input.


	return 0;
}
