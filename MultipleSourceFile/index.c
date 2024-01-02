/*
* Assignment 8
* Create index file.
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
int compare(const void *left, const void *right);	// Used by qsort.

IndexKey indexKey;

int main(int argc, char* argv[]) {

	// Check the number of arguments. It must be three, otherwise exit.
	if (argc != 3) 
	{
		atexit(printIncorrectArgs);
		exit(EXIT_FAILURE);
	}

	FILE* fpData;
	FILE* fpIndexBalance;

	size_t readCount;
	size_t writeCount;
	int dataRecordCount;
	int indexRecordCount;
	long filePos;

	IndexHeader indexHeader;
	IndexRecord* indexRecords;

	Client temp = { 0, "", "", 0.0, 0.0 };

	// Sort by balance.
	indexKey = BALANCE;

	// Open data file for reading, 
	// if it does not exist, print error message and exit the program.
	if ( (fpData = fopen(argv[1], "rb")) == NULL )
	{
		atexit(printSourceFileInexist);
		exit(EXIT_FAILURE);
	}

	// Open/Create index file for writing, 
	// if it does not exist, print error message and exit the program.
	if ( (fpIndexBalance = fopen(argv[2], "wb")) == NULL )
	{
		atexit(printIndexFileCanNotOpen);
		exit(EXIT_FAILURE);
	}

	// Get the record count in the data file.
	dataRecordCount = fileSize(fpData) / sizeof(Client);

	// Allocate the memory.
	indexRecords = (IndexRecord*)calloc(dataRecordCount, sizeof(IndexRecord));

	// Populate index header.
	indexHeader.idxKey = indexKey;
	strcpy(indexHeader.appName, APP_NAME);
	indexHeader.recCount = dataRecordCount;

	rewind(fpData);

	filePos = ftell(fpData);
	indexRecordCount = 0;

	// Read first data record.
	readCount = fread(&temp, sizeof(Client), 1, fpData);
	// Read all data records.
	while (!feof(fpData) && (readCount == 1)) {
		// Sign the key and file position of the index record.
		indexRecords[indexRecordCount].key.accountBalance = temp.accountBalance;
		indexRecords[indexRecordCount].filePos = filePos;

		// Increament to next element.
		indexRecordCount++;

		// Store file position before next read advances it.
		filePos = ftell(fpData);

		// Read next record.
		readCount = fread(&temp, sizeof(Client), 1, fpData);
	}

	// Sort array of index records in memory,
	// that based on the compare function.
	qsort(indexRecords, indexRecordCount, sizeof(IndexRecord), compare);

	printf("Index Header Size: %zd\nIndex Record Size: %zd\n\n",
		sizeof(IndexHeader), sizeof(IndexRecord));

	printf("Creating Index File\n");

	// Write index header to file.
	writeCount = fwrite(&indexHeader, sizeof(IndexHeader), 1, fpIndexBalance);
	// Write index records to file.
	writeCount = fwrite(indexRecords, sizeof(IndexRecord), indexRecordCount, fpIndexBalance);

	// Check the index rusult.
	for (size_t i = 0; i < indexRecordCount; i++)
	{
		printf("%10d%20.2f\n\n", indexRecords[i].filePos, indexRecords[i].key.accountBalance);
	}

	printf("%zd Records Processed\n", writeCount);

	printf("\nFileSize: %d\n\n", fileSize(fpIndexBalance));

	// Release this memory.
	free(indexRecords);

	// Close files.
	fclose(fpData);
	fclose(fpIndexBalance);
}

// Function that qsort points to
int compare(const void *left, const void *right) {

	int result = 0;

	// Cast pointer parameters to IndexRecord.
	IndexRecord *pLeft = (IndexRecord *)left;
	IndexRecord *pRight = (IndexRecord *)right;

	// Compare the left and right account balance value.
	// Here is in decending order.
	if (pLeft->key.accountBalance > pRight->key.accountBalance)
	{
		return -1;
	}
	else 
	{
		return 1;
	}

	return result;
}
