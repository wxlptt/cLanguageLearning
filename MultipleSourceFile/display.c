/*
* The script can sort the file by first name, balance, and last payment.
* But this script only sort the file by balance.
* Display the information of the file.
* auther: Xiaolong Wang (Wilson)
* date: Nov 25, 2023
*/

#define _CRT_SECURE_NO_WARNINGS
#define NAME_LENGTH 15
#define APP_NAME "Assignment8"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "assign08.h"

// Function prototypes
void displayInNaturalOrder(FILE* fpData, FILE* fpIndexBalance);		// Print the file in natural order.
void displayInBalanceOrder(FILE* fpData, FILE* fpIndexBalance);		// Print the file in balance order.
void displayMenu(void);												// Display menu.

int main(int argc, char* argv[]) {

	// Check the number of arguments. It must be three, otherwise exit.
	if (argc != 3)
	{
		atexit(printIncorrectArgs);
		exit(EXIT_FAILURE);
	}

	// Create an array of function pointer.
	void(*pfunc[2]) (FILE * fpData, FILE * fpIndexBalance);

	FILE* fpData;
	FILE* fpIndexBalance;

	char choice = '0';
	
	// Open data file for reading, 
	// if it does not exist, print error message and exit the program.
	if ((fpData = fopen(argv[1], "rb")) == NULL)
	{
		atexit(printSourceFileInexist);
		exit(EXIT_FAILURE);
	}

	// Open/Create index file for writing, 
	// if it does not exist, print error message and exit the program.
	if ((fpIndexBalance = fopen("balance.idx", "rb")) == NULL)
	{
		atexit(printIndexFileCanNotOpen);
		exit(EXIT_FAILURE);
	}

	// Assign the function to each function pointer.
	pfunc[0] = displayInNaturalOrder;
	pfunc[1] = displayInBalanceOrder;

	while (choice != 3)
	{
		displayMenu();	
		scanf("%c", &choice);
		while (getchar() != '\n');

		// Based on the choice to display the data.
		switch (choice)
		{
		case '1':
			// Display the data in natural order, so it does not need to input index file.
			(*pfunc[0])(fpData, NULL);
			break;

		case '2':
			// Display the data in descending balance order.
			(*pfunc[1])(fpData, fpIndexBalance);
			break;

		case '3':
			printf("%s", "The program exits.");
			exit(EXIT_SUCCESS);
			break;

		default:
			printf("Invalid choice.\n");
			break;
		}
	}

	// Close files.
	fclose(fpData);
	fclose(fpIndexBalance);
}

// Display the file data in balance order.
void displayInBalanceOrder(FILE* fpData, FILE* fpIndexBalance) {

	size_t recCount;
	size_t readCount;
	IndexHeader indexHeader;
	IndexRecord indexTemp;

	Client temp = { 0, "", "", 0.0, 0.0 };

	// Calculate record count in data file.
	recCount = fileSize(fpData) / sizeof(Client);

	// Get the index header.
	readCount = fread(&indexHeader, sizeof(IndexHeader), 1, fpIndexBalance);

	// Check the index header name.
	if (strcmp(indexHeader.appName, APP_NAME) != 0)
	{
		printf("ERROR - Data and Index Files Created by Different Application Versions\n");
		exit(EXIT_FAILURE);
	}

	// Check the number of records in header.
	if (indexHeader.recCount != recCount)
	{
		printf("ERROR - Data and Index Files Have a Record Count Mismatch\n");
		exit(EXIT_FAILURE);
	}

	// Print the title.
	printf("Data file read in indexed order (Account Balance)\n\n");
	printf("%-20s%-20s%-20s%-20s%-20s\n",
		"Account Number", "First Name", "Last Name", "Account Balance", "Last Payment");
	printf("--------------------------------------------------------------------------------------------\n");

	// Read first index record.
	readCount = fread(&indexTemp, sizeof(IndexRecord), 1, fpIndexBalance);

	// Continue processing all index records.
	while (!feof(fpIndexBalance) && (readCount == 1))
	{
		// Seek record in data file based on file position.
		if (fseek(fpData, indexTemp.filePos, SEEK_SET) != 0) {
			printf("Seek Error\n");
			exit(EXIT_SUCCESS);
		}

		// Read data record and output.
		fread(&temp, sizeof(Client), 1, fpData);
		printf("%-20d%-20s%-20s%-20.2f%-20.2f\n",
			temp.accountNumber, temp.firstName, temp.lastName, temp.accountBalance, temp.lastPaymentAmount);

		// Read next index record.
		readCount = fread(&indexTemp, sizeof(IndexRecord), 1, fpIndexBalance);
	}
}

// Display the file data in natural order.
void displayInNaturalOrder(FILE* fpData, FILE* fpIndexBalance) {
	
	Client temp = { 0, "", "", 0.0, 0.0 };

	// Print the title.
	printf("Data file read in natural order:\n\n");
	printf("%-20s%-20s%-20s%-20s%-20s\n",
		"Account Number", "First Name", "Last Name", "Account Balance", "Last Payment");
	printf("--------------------------------------------------------------------------------------------\n");

	// Read first index record.
	fread(&temp, sizeof(Client), 1, fpData);

	// Continue processing all index records.
	while (!feof(fpData))
	{
		printf("%-20d%-20s%-20s%-20.2f%-20.2f\n",
			temp.accountNumber, temp.firstName, temp.lastName, temp.accountBalance, temp.lastPaymentAmount);

		// Read next index record.
		fread(&temp, sizeof(Client), 1, fpData);
	}
	
}

void displayMenu(void)
{
	printf("\nEnter your selection:\n"
		"   1) Display the file data in natural order.\n"
		"   2) Display the file data in balance order.\n"
		"   3) exit\n");
}
