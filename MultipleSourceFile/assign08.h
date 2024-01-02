/*
* assign08.h
* Assignment 8 header
* auther: Xiaolong Wang (Wilson)
* date: Nov 30, 2023
*/

// Define data structure.
#ifndef DATA_STRUCT
#define DATA_STUCT

typedef enum { FIRST, BALANCE, PAYMENT } IndexKey;

typedef union {
	char	firstName[NAME_LENGTH];
	double  accountBalance;
	double	lastPaymentAmount;
} KeyType;

typedef struct {
	int		accountNumber;
	char	firstName[NAME_LENGTH];
	char	lastName[NAME_LENGTH];
	double	accountBalance;
	double	lastPaymentAmount;
} Client;

typedef struct {
	KeyType		key;
	long		filePos;
} IndexRecord;

typedef struct {
	IndexKey	idxKey;
	char		appName[sizeof(APP_NAME)];
	int			recCount;
} IndexHeader;

#endif // !DATA_STRUCT

// function prototypes
long fileSize(FILE* input);							// Get the file size.
void printIncorrectArgs(void);						// Print error.
void printSourceFileInexist(void);					// Print error.
void printIndexFileCanNotOpen(void);				// Print error.

// Get the file size.
long fileSize(FILE* input) {
	long orgPos;
	long startPos;
	long endPos;

	orgPos = ftell(input);			// Save original file position.

	rewind(input);
	startPos = ftell(input);		// Should always be 0.

	fseek(input, 0, SEEK_END);
	endPos = ftell(input);			// Get the end position.

	rewind(input);					// restore start position.
	return (endPos - startPos);
}

// If the number of arguments is not equal 3, print a warning.
void printIncorrectArgs(void) {
	printf("%s", "The number of arguments is not proper.\n");
}

// If the source file does not exist, print a warning.
void printSourceFileInexist(void) {
	printf("%s", "The source file does not exist\n");
}

// If can not open/create the index file, print a warning.
void printIndexFileCanNotOpen(void) {
	printf("%s", "Can not open / create index file.\n");
}
