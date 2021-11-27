#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>

/*
 * Name:        Ryan Cole
 * Username:    rccole@mtu.edu
 * Description: This program is set up to train a simple language model from a text file.
 *              It first converts all letters in each word to uppercase and removes any non-letter characters (except for apostrophes).
 *              The program then assigns a hash value to each word. If the word is new, it is assigned to a bucket in a hash table.
 *              If the word is not unique, then that word's count is updated. After every word in the file is read into the program,
 *              the program prints output consisting of each word along with that word's unigram log base 10  probability.
 *              The size of the hash table the program uses is specified as by the user as a command-line argument.
 *              The program also supports a second optional command-line argument. If this argument is nonzero, the program prints
 *              debug output as it reads in each word.
 */

// Maximum length of words that we support
#define MAX_WORD_LEN 255

// Represents a single entry in a bucket's null-terminated linked list
// You should NOT modify this data type
typedef struct node {
    char* word;  	 // Null-terminated string for this word
    unsigned int count;  // How often we saw this word in the training file
    struct node* next;   // Pointer to next Node in this bucket
} Node;

// Function prototypes, this allows the function implementations to appear in any order in the file
// You will implement these functions:
unsigned int hashString(const char* str);
double calcUnigramLogProb(unsigned int count, unsigned long total);
void uppercaseAndStrip(char* dest, const char* src);

// Test function prototypes, you should NOT modify these
void testHashString();
void testCalcUnigramLogProb();
void testUppercaseAndStrip();
void testFillAllPrintable(char* str);

// Fill an array entry with all printable ASCII characters
void testFillAllPrintable(char* str) {
    // Create a maximum size string with all printable ASCII characters
    int index = 0;
    char ch = ' ';
    
    while (index < MAX_WORD_LEN) {
        str[index] = ch;
        ch++;
        if (ch > 126) {
            ch = ' ';
        }
        index++;
    }
}

// Test function for the hashString function
void testHashString() {
    char tests[][MAX_WORD_LEN + 1] = {"BAD", "DAB", "GOODFELLOWS", "WRITERSHIP", "a", "A", "abcdefghijklmnopqrstuvwxyz", "1234567890!@#$%^&*()", "", ""};
    testFillAllPrintable(tests[8]);
    
    int i = 0;
    while (strlen(tests[i]) > 0) {
        printf("\"%s\" -> %u\n", tests[i], hashString(tests[i]));
        i++;
    }
}

// Test function for the calcUnigramLogProb function
void testCalcUnigramLogProb() {
    const int SIZE = 10;
    unsigned int  counts[] = { 5 ,  50 ,   1,    1,   1234,   0 , 100, 1, 1        , UINT_MAX - 10000 };
    unsigned long totals[] = {10 , 100 , 100, 2000, 567890, 123 , 100, 1, ULONG_MAX, UINT_MAX         };
    
    for (int i = 0; i < SIZE; i++){
        printf("%5u %7lu -> %9.6f\n", counts[i], totals[i], calcUnigramLogProb(counts[i], totals[i]));
    }
}

// Test function for the stripLower function
void testUppercaseAndStrip() {
    char tests[][MAX_WORD_LEN + 1] = {"b", "B", "bad", "BAD", "BaD", "CAN'T", "well-done", "!L00K-", "1234567890", "abcdefghijklmnopqrstuvwxyz", "", ""};
    char dest[MAX_WORD_LEN + 1];
    testFillAllPrintable(tests[10]);
    
    int i = 0;
    while (strlen(tests[i]) > 0) {
        printf("\"%s\" (len %zu) -> ", tests[i], strlen(tests[i]));
        uppercaseAndStrip(dest, tests[i]);
        printf("\"%s\" (len %zu)\n", dest, strlen(dest));
        i++;
    }
}

/*
 * This function converts a null-terminated string to a hash value.
 *
 * Parameter:
 * str - the string to convert
 */
unsigned int hashString(const char* str) {
    unsigned int hashVal = 0;
    int strLen = strlen(str);

    // Calculate the hash value of the word as it iterates through each character in the string
    for (int i = 0; i < strLen; i++) {
        hashVal = (31 * hashVal) + ((int) *(str + i));
    }

    return hashVal;
}

/*
 * This function calculates the log base 10 unigram probability of each word by dividing by
 * dividing the count of the provided word by the total number of words.
 *
 * Parameters:
 * count - the number of times the word appears in the input text file
 * total - the total number of words in the input text file
 */
double calcUnigramLogProb(unsigned int count, unsigned long total) {
    double logProb = 0.0;
    logProb = log10( (double) count / total);
    return logProb;
}

/*
 * This function takes a string and converts all of its letters to uppercase.
 * It also removes any character that is not a letter or an apostrophe.
 *
 * Parameters:
 * dest - where the converted data is copies into
 * src -  a null-terminated string to convert
 */
void uppercaseAndStrip(char* dest, const char* src) {
    while (*src != '\0') {
        // Copy the character over if already uppercase or an apostrophe
        if ((*src >= 65 && *src <= 90) || *src == 39) {
            *dest = *src;
            dest++;
            src++;
          // Convert to uppercase and then copy the character over if it is lowercase   
        } else if (*src >= 97 && *src <= 122) {
            *dest = *src - 32;
            dest++;
            src++;
          // Move on to the next character if not a letter or an apostrophe
        } else {
            src++;
        }
    }

    *dest = '\0';
}

int main(int argc, char** argv) {
   // If no command line input we print out a help message and also run test functions
   if (argc <= 1) {
      printf("Usage: Unigram <hash table size> [debug]\n\n");
      testHashString();
      printf("\n");
      testCalcUnigramLogProb();
      printf("\n");
      testUppercaseAndStrip();
      return 0;
   }

   // Read in the size of the hash table, printing an error if the value is not a positive integer
   int hashSize = atoi(argv[1]);
   if (hashSize < 1) {
       printf("ERROR: Table size must be positive!\n");
       return 0;
   }

   // Turn on debug output if the second command-line argument is non-zero
   bool debug = false;
   if (argc > 2 && atoi(argv[2]) != 0) {
       debug = true;
   }

   // Create a hash table to hold nodes with the specified number of buckets
   Node* buckets[hashSize];
   for (int i = 0; i < hashSize; i++) {
       buckets[i] = NULL;
   }

   // Keep track of the total number of words
   unsigned long total = 0;

   // Variable to store each word in input
   char word[256];

   // Read in each string in the text file
   while (scanf("%255s", word) == 1) {
       // Allocate memory for the node and the word
       Node* addWord = malloc(sizeof(Node));
       addWord->word = malloc(strlen(word) + 1);
       uppercaseAndStrip(addWord->word, word);

       // Ignore the word if it is empty
       if (strcmp(addWord->word, "") == 0) {
           free(addWord->word);
           addWord->word = NULL;
           free(addWord);

           continue;
       }

       // If the word is nonempty, increment the total number of words
       total++;

       // Determine which bucket the word shoud be placed in
       unsigned long hashVal = hashString(addWord->word);
       unsigned int i = hashVal % hashSize;

       // Determine whether this is a newly encountered word. If not, update the count.
       bool newWord = true;
       Node* current = buckets[i];
       while (current != NULL) {
           if (strcmp(addWord->word, current->word) == 0) {
               newWord = false;
               break;
           }

           current = current->next;
       }

       // Insert the word if it is a newly encountered word.
       if (newWord) {
           addWord->next = buckets[i];
           buckets[i] = addWord;
           addWord->count = 1;
       } else {
           // Update the count if the word is not new
           current->count++;

           // Free the previously allocated memory
           free(addWord->word);
           addWord->word = NULL;
           free(addWord);
       }

       // Print debug output if specified
       if (debug && newWord) {
           printf("%s -> hash %ld, bucket %d, count %d\n", addWord->word, hashVal, i, addWord->count);
       } else if (debug) {
           printf("%s -> hash %ld, bucket %d, count %d\n", current->word, hashVal, i, current->count);
       }
   }

   // Iterate through each bucket in the hash table
   for (int i = 0; i < hashSize; i++) {
       Node* current = buckets[i];

       // Iterate through each node in the current bucket
       while (current != NULL) {
           Node* next = current->next;

           // Calculate the unigram probability and print basic output for each word
           double logProb = calcUnigramLogProb(current->count, total);
           printf("%.6f %s\n", logProb, current->word);

           // Free the memory allocated for this word and node
           free(current->word);
           current->word = NULL;
           free(current);

           current = next;
       }
   }
   
   return 0;
}
