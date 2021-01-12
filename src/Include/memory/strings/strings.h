#ifndef _STRINGS_H
#define _STRINGS_H

/**
 * @file strings.h
 * @author Stefan Nuernberger
 * @date 09-13-2006
 * @brief String functions
 */

/**
* copies a string from dest to src
* @param dest address to write to
* @param src address to read from
* @return destination address
*/
CString strcpy(CString dest,CString src);

/**
 * copies n bytes from src to dest
 * @param dest address to write to
 * @param src address to read from
 * @param num number of bytes to copy
 * @return destination address
 */
String strncpy(String dest, CString src, UINT32 num);

/**
 * compares two string and returns matching
 * @param string1 a string
 * @param string2 another string
 * @return 0 if equal, non-0 else
 */
int strcmp(CString string1, CString string2);

/**
 * Comparese two strings, but at most length n
 */
int strncmp(const char *s1, const char *s2, UINT32 n);

BOOL StrEquals(CString s1, CString s2);

/**
 * returns the length of a string (first occurence of '\0')
 * @param string string to check
 * @return length of the string
 */
int strlen(CString string);

/**
 * returns the length of a string (first occurence of '\0')
 * But terminates if not found after maxLength
 * @param string string to check
 * @param maxLength maximum length of string. When not found till this position
 * search will abort
 * @return length of the string
 */
UINT32 strnlen(CString string, UINT32 maxLength);

/**
 * searches for search in string and returns a pointer pointing to
 * the start of search
 * @param string String to search in
 * @param search What to search
 * @return Pointer to start of 1st occurence of search in string
 */
String strchr(String string, char search);

/**
 * Converts a string number to a an integer
 */
SINT32 atoi(CString numberString);

/**
 * searches for search in string and returns a pointer pointing to
 * the start of search
 * @param string String to search in
 * @param length maximum length of string
 * @param search What to find
 * @return Pointer to start of 1st occurence of search in string
 */
String strnchr(String string, UINT32 length, char search);

/**
 * searches for search in string and returns postion of 1st
 * occurence if found, -1 otherwise
 * @param string Strint to search in
 * @param search What to find
 */
SINT32 strpos(String string, char search);

/**
 * Searches for search in string and returns position of 1st
 * occurence (if it's not > length) -1 otherwise
 * @param string Strint to search in
 * @param length maximum length of string
 * @param search What to find
 */
SINT32 strnpos(String string, UINT32 length, char search);

/**
 * Checks whether <code>lookFor</code> is the end of
 * <code>lookIn</code>
 */
BOOL StrEndsWith(CString lookIn, CString lookFor);

/**
 * Concatenates the string <code>concat</code> to
 * the end of string <code>destination</code>
 * @param destination String to concatenate to
 * @param concat String to concatenate
 * @return Returns the position in <code>destination</code>
 * where <code>concat</code> was added.
 */
String strcat(String destination, CString concat);


#endif
