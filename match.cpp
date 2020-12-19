#pragma warning(disable : 6262)
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
using namespace std;

const int MAX_WORD_LENGTH = 20;
const int MAX_DOC_LENGTH = 250;

int cleanupRules(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int nRules);
int determineScore(const char document[], const char wordin[][MAX_WORD_LENGTH + 1], const char wordout[][MAX_WORD_LENGTH + 1], int nRules);
void moveToTheEnd(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int nth, int nRules);
bool syntaxTest(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int nth);
int main()
{
	const int TEST1_NRULES = 3;
	char test1win[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {"family", "unhappy", "horse"};
	char test1wout[TEST1_NRULES][MAX_WORD_LENGTH + 1] = {"", "horse", "",};

	//To test that the function works
	assert(determineScore("Happy families are all alike; every unhappy family is unhappy in its own way.", test1win, test1wout, TEST1_NRULES) == 2);
	//To test that the function does not count a two-word rule if both wordin and wordout elements are found
	assert(determineScore("Happy horses are all alike; every unhappy horse is unhappy in its own way.", test1win, test1wout, TEST1_NRULES) == 1);
	//To test that the function only counts 1 if wordin from a one-word rule matches multiple time
	assert(determineScore("A horse!  A horse!  My kingdom for a horse!", test1win, test1wout, TEST1_NRULES) == 1);
	//To test that the function returns 0 when nRules is zero
	assert(determineScore("**** 2020 ****", test1win, test1wout, 0) == 0);
	//To test that the function returns 0 when the document is just an empty string
	assert(determineScore("", test1win, test1wout, 3) == 0);
	//To test that the function can sort out both non-letter characters and extra space
	assert(determineScore("       I       won  a'   'horse race      ", test1win, test1wout, 3) == 1);
	//To test that the function returns 0 when nRules is negative
	assert(determineScore("      I       won  a'   'horse race      ", test1win, test1wout, 0) == 0);
	
	const int TEST2_NRULES = 6;
	char test2wordin[TEST2_NRULES][MAX_WORD_LENGTH + 1] = { "Hello", "Jin", "Kenya", "Jordan" };
	char test2wordout[TEST2_NRULES][MAX_WORD_LENGTH + 1] = { "", "Jin", "Uganda", "Mike",};

	//To test that the function removes wordin and wordout if they are the same
	assert(cleanupRules(test2wordin, test2wordout, 4) == 3);
	
	const int TEST3_NRULES = 6;
	char test3wordin[TEST3_NRULES][MAX_WORD_LENGTH + 1] = { "Hello", "Jin", "Kenya", "Jordan", "Jordan", "Jordan"};
	char test3wordout[TEST3_NRULES][MAX_WORD_LENGTH + 1] = { "",	"",		"Hello", "Mike", "Mike", "Mike" };
	
	//To test that if more than 1 two-word rule has the same wordin and wordout, the function deletes them
	assert(cleanupRules(test3wordin, test3wordout, 6) == 4);
	
	const int TEST4_NRULES = 2;
	char test4wordin[TEST4_NRULES][MAX_WORD_LENGTH + 1] = { "H3ello", "Jin",};
	char test4wordout[TEST4_NRULES][MAX_WORD_LENGTH + 1] = { "",	"",	 };

	//To test that the function removes an element if it contains a non-letter character
	assert(cleanupRules(test4wordin, test4wordout, 2) == 1);

	const int TEST5_NRULES = 4;
	char test5wordin[TEST5_NRULES][MAX_WORD_LENGTH + 1] = { "Jin", "Jin", "Kenya", "Jin" };
	char test5wordout[TEST5_NRULES][MAX_WORD_LENGTH + 1] = { "Kim",	"", "Uganda", "Kyu"};

	//To test that if a one-word rule has the same win as other rules,
	//the function removes from the collection all such other rules, leaving just one instance of the one-word rule
	assert(cleanupRules(test5wordin, test5wordout, 4) == 2);

	const int TEST6_NRULES = 7;
	char test6wordin[TEST6_NRULES][MAX_WORD_LENGTH + 1] = { "Jin", "Jin", "Kenya", "Jin", "Kenya", "Mike", "Wo3ld" };
	char test6wordout[TEST6_NRULES][MAX_WORD_LENGTH + 1] = { "Kim",	"", "Uganda", "Jin", "Jordan", "Lamp", "cl" };

	//To test that the function works correctly under multiple errors in wordin and wordout arrays
	assert(cleanupRules(test6wordin, test6wordout, 7) == 4);
	
	const int TEST7_NRULES = 12;
	char test7wordin[TEST7_NRULES][MAX_WORD_LENGTH + 1] =
	{ "confusion", "FAMILY", "charm",    "hearty", "house" , "worn-out", "family", "charm", "ties", "", "charm", "FaMiLy" };
	char test7wordout[TEST7_NRULES][MAX_WORD_LENGTH + 1] =
	{ "",			"TIES", "confusion", "hearty", "intrigue", "younger", "first", "", "family", "frightened", "", "tIeS" };

	//To test that the function works correctly under multiple errors in wordin and wordout arrays
	assert(cleanupRules(test7wordin, test7wordout, 12) == 6);
	
	const int TEST8_NRULES = 7;
	char test8wordin[TEST8_NRULES][MAX_WORD_LENGTH + 1] = { "@ji@", "Jin", "Kenya", "Jin", "!@$@#$%", "Mike", "Wo3ld" };
	char test8wordout[TEST8_NRULES][MAX_WORD_LENGTH + 1] = { "Kim",	"-32", "Uganda", "Jin", "Jordan", "Lamp", "cl" };

	//To test that the function works correctly under multiple errors in wordin and wordout arrays
	assert(cleanupRules(test8wordin, test8wordout, 7) == 2);

	const int TEST9_NRULES = 7;
	char test9wordin[TEST9_NRULES][MAX_WORD_LENGTH + 1] = { "Avocado", "Jin", "Kenya", "Jin", "mango", "mike", "juliet" };
	char test9wordout[TEST9_NRULES][MAX_WORD_LENGTH + 1] = { "",	    "1g31b", "Uganda", "Jin", "Tango", "Lamp", "cl" };

	//To test that the function changes all uppercase to lowercase and therefore cannot accept "Avocado"
	assert(determineScore("Hey Mike and Juliet, do you prefer Avocado or Mango?", test9wordin, test9wordout, TEST9_NRULES) == 3);

	const int TEST10_NRULES = 7;
	char test10wordin[TEST10_NRULES][MAX_WORD_LENGTH + 1] = { "Jin", "Jin", "Kenya", "Jin", "Micheal", "Mike", "Jin" };
	char test10wordout[TEST10_NRULES][MAX_WORD_LENGTH + 1] = { "Kim",	"-32", "Uganda", "", "Jordan", "Lamp", "JEN" };

	//To test that the function works correctly under multiple errors in wordin and wordout arrays
	assert(cleanupRules(test10wordin, test10wordout, 7) == 4);

	
}

void moveToTheEnd(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int nth, int nRules)
{
	for (int o = nth; o < nRules - 1; o++)
	{
		strcpy(wordin[o], wordin[o + 1]);
		strcpy(wordout[o], wordout[o + 1]);
	}
	//since the last element is duplicated, change it to an empty string
	if (strlen(wordin[nRules - 1]) > 0)
		strcpy(wordin[nRules - 1], "\0");
	if (strlen(wordout[nRules - 1]) > 0)
		strcpy(wordout[nRules - 1], "\0");
}

bool syntaxTest(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int nth)
{
		// checks if both wordin and wordout are empty
		if (wordin[nth][0] == '\0' && wordout[nth][0] == '\0')
			return false;

		// checks if wordin and wordout are the same
		if (strcmp(wordin[nth], wordout[nth]) == 0)
			return false;

		// for two-word rule
		// checks if the wordin and wordout are letters
		if (strlen(wordin[nth]) > 0 && strlen(wordout[nth]) > 0)
		{
			for (int j = 0; j < strlen(wordin[nth]); j++)
			{
				if (!isalpha(wordin[nth][j]))
					return false;
			}
			for (int j = 0; j < strlen(wordout[nth]); j++)
			{
				if (!isalpha(wordout[nth][j]))
					return false;
			}
		}

		// for one-word rule
		// checks if the wordin is letter
		else if (strlen(wordin[nth]) > 0 && strlen(wordout[nth]) == 0)
		{
			for (int j = 0; j < strlen(wordin[nth]); j++)
			{
				if (!isalpha(wordin[nth][j]))
					return false;
			}
		}
		return true;
	}

int cleanupRules(char wordin[][MAX_WORD_LENGTH + 1], char wordout[][MAX_WORD_LENGTH + 1], int nRules)
{
	// treat negative nRules as zero
	if (nRules < 0)
		return 0;

	// if there is a syntax error, move the element of both wordin and wordout to the end and convert them to an empty string
	int numOfSyntaxError = 0;
	for (int k = 0; k < nRules - numOfSyntaxError; k++)
	{
		if (!syntaxTest(wordin, wordout, k))
		{
			moveToTheEnd(wordin, wordout, k, nRules);
			//start at position k - 1 
			k--;
			//By reducing the length by 1, it no longer counts the character that has been moved to the end
			numOfSyntaxError++;
		}
	}

	// convert all uppercase letter a lowercase letter
	for (int k = 0; k < nRules; k++)
	{
		for (int j = 0; j < strlen(wordin[k]); j++)
		{
			wordin[k][j] = tolower(wordin[k][j]);
		}
		for (int j = 0; j < strlen(wordout[k]); j++)
		{
			wordout[k][j] = tolower(wordout[k][j]);
		}
	}

	//if wordin from 1 word rule appears in other rules, change it to an empty string
	for (int k = 0; k < nRules; k++)
	{
		if (strlen(wordin[k]) > 0 && strlen(wordout[k]) == 0)
		{
			for (int j = 0; j < nRules; j++)
			{
				if (k != j && strcmp(wordin[k], wordin[j]) == 0)
				{
					if (strlen(wordin[j]) > 0)
						strcpy(wordin[j], "\0");
					if (strlen(wordout[j]) > 0)
						strcpy(wordout[j], "\0");
				}
			}
		}
	}

	// checks if more than one two-word rule have same wordin and wordout
	// if found, they will be moves to the end and changed to an empty string
	int numOfSameWordinandWordout = 0;
	for (int k = 0; k < nRules - numOfSameWordinandWordout; k++)
	{
		for (int j = k+1; j < nRules - 1; j++)
		{
			if (strcmp(wordin[k], wordin[j]) == 0 && strcmp(wordout[k], wordout[j]) == 0)
			{
				moveToTheEnd(wordin, wordout, j, nRules);
				//start at position k - 1 
				k--;
				//By reducing the length by 1, it no longer counts the character that has been moved to the end
				numOfSameWordinandWordout++;
			}
		}
	}
	
	// if wordin is empty, move to the end and switch to an empty string
	int numOfEmptyWordin = 0;
	for (int k = 0; k < nRules - numOfEmptyWordin; k++)
	{
		if (strlen(wordin[k]) == 0)
		{
			moveToTheEnd(wordin, wordout, k, nRules);
			//start at position k - 1 
			k--;
			//By reducing the length by 1, it no longer counts the character that has been moved to the end
			numOfEmptyWordin++;
		}
	}
	
	for (int k = 0; k < nRules; k++)
	{
		cout << wordin[k] << "     " << wordout[k] << endl;
	}

	int count = 0;
	for (int k = 0; k < nRules; k++)
	{
		if (strlen(wordin[k]) > 0)
			count++;
	}

	return count;
}
int determineScore(const char document[], const char wordin[][MAX_WORD_LENGTH + 1], const char wordout[][MAX_WORD_LENGTH + 1], int nRules)
{
	if (nRules < 0)
		return 0;
	
	char storageForDoc[MAX_DOC_LENGTH + 1];
	strcpy(storageForDoc, document);
	
	// checks if there is a character that is neither a letter or a space
	int unauthorizedLetter = 0;
	for (int k = 0; k < strlen(storageForDoc) - unauthorizedLetter; k++)
	{
		//if neither a letter or a space, switch it with the character at the next position
		//this moves the character to the end
		if (!isalpha(storageForDoc[k]) && !isspace(storageForDoc[k]))
		{
			for (int j = k; j < strlen(storageForDoc) - 1; j++)
			{
				storageForDoc[j] = storageForDoc[j + 1];
			}
			//start at position k - 1 
			k--;
			//By reducing the length by 1, it no longer counts the character that has been moved to the end
			unauthorizedLetter++;
		}
	}
	
	//the length is reduced to account for character that is neither a space nor a letter
	int sizeAfterTrim1 = strlen(storageForDoc) - unauthorizedLetter;

	//change an uppercase letter to a lowercase letter
	for (int k = 0; k < sizeAfterTrim1; k++)
		storageForDoc[k] = tolower(storageForDoc[k]);
		
	int numbOfSpace = 0;

	for (int k = 0; k < sizeAfterTrim1 - numbOfSpace; k++)
	{
		//if there is a space in the first position, move to the end
		if (k == 0 && isspace(storageForDoc[k]))
		{
			for (int j = k; j < strlen(storageForDoc) - 1; j++)
			{
				storageForDoc[j] = storageForDoc[j + 1];
			}
			k--;
			//By reducing the length by 1, it no longer counts the space that has been moved to the end
			numbOfSpace++;
		}

		//if there are more than one space character, then move all of them to the end
		else if (k > 0 && k < sizeAfterTrim1 - 1 && isspace(storageForDoc[k]) && isspace(storageForDoc[k+1]))
		{
			for (int j = k; j < strlen(storageForDoc) - 1; j++)
			{
				storageForDoc[j] = storageForDoc[j + 1];
			}
			k--;
			//By reducing the length by 1, it no longer counts the spaces that have been moved to the end
			numbOfSpace++;
		}
	}
	
	//the length is reduced to account for extra space
	int sizeAfterTrim2 = sizeAfterTrim1 - numbOfSpace;
	int numOfSpace = 0;

	//make the document into 2-D array
	char twoDdocument[MAX_DOC_LENGTH + 1][MAX_WORD_LENGTH+1];

	//counts the number of rows
	for (int k = 0; k < sizeAfterTrim2; k++)
	{
		if (isspace(storageForDoc[k]))
			numOfSpace++;
	}

	//the row should be number of spaces + 1
	numOfSpace += 1;

	int pos = 0, count = 0;
	for (int k = 0; k < numOfSpace; k++)
	{
		int numOfCharacters = 0;
		for (int j = 0; j < sizeAfterTrim2; j++)
		{
			if (j + pos < sizeAfterTrim2 && !isspace(storageForDoc[j + pos]))
			{
				//store the characters until a letter is found
				twoDdocument[k][j] = storageForDoc[j + pos];
				numOfCharacters++;
				count++;
			}
			else
				break;
		}

		//place a null character to mark the end
		//numOfCharacters is used to find a position where a null character should be placed
		twoDdocument[k][numOfCharacters] = '\0';

		//count is used to store a starting position after a space
		count++;
		pos = count;
	}
	
	int ActualMatching = 0;
	
	for (int k = 0; k < nRules; k++)
	{
		//the rows of wordin and wordout should be stored as 1d array
		//2d array cannot be used to compare with other 1d array
		char wordinComp[MAX_WORD_LENGTH + 1];
		char wordoutComp[MAX_WORD_LENGTH + 1];
		strcpy(wordinComp, wordin[k]);
		strcpy(wordoutComp, wordout[k]);

		int numbOfFirstRuleMatching = 0;
		int numbOfSecondRuleMatching = 0;
		int SecondRuleTest = 0;

		// for one-word rule
		if (strlen(wordinComp) > 0 && strlen(wordoutComp) == 0)
		{
			for (int j = 0; j < numOfSpace; j++)
			{
				//find if there is a matching word
				if (strcmp(wordinComp, twoDdocument[j]) == 0)
				{
					numbOfFirstRuleMatching++;
				}
			}
			
			//if there is a matching word, increase ActualMatching by 1
			if (numbOfFirstRuleMatching > 0)
				ActualMatching++;
		}

		// for two-wrod rule
		else
		{
			for (int j = 0; j < numOfSpace; j++)
			{
				//checks if wordin matches with a word in the document
				if (strcmp(wordinComp, twoDdocument[j]) == 0)
				{
					SecondRuleTest++;
					for (int z = 0; z < numOfSpace; z++)
					{
						//checks if wordout matches with any word in the document
						if (strcmp(wordoutComp, twoDdocument[z]) == 0)
						{
							//only when wordin matches while wordout does not match
							numbOfSecondRuleMatching++;
						}
					}
				}
			}
			// if wordin matches while wordout doesn't, increase the ActualMatching by 1
			if (numbOfSecondRuleMatching == 0 && SecondRuleTest > 0)
				ActualMatching++;
		}
	}
	
	return ActualMatching;
	
}