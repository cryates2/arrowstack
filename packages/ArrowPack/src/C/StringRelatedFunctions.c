#include "StringRelatedFunctions.h"

bool EMSCRIPTEN_KEEPALIVE containsCharacter(char *string, char character) // Checks if string contains a certain character
{
    for (int i = 0; i < strlen(string); i++)
    {
        if (string[i] == character)
            return true;
    }
    return false;
}
char *GetFileExtension(const char *path) // Returns the file extension for the given path without the dot char
{
    unsigned int pathLen = strlen(path);

    unsigned int lastFullStop = 0;
    for (unsigned int i = 0; i < pathLen; i++)
    {
        if (path[i] == '.')
        {
            lastFullStop = i;
        }
    }
    if (lastFullStop == 0)
    {
        printf("Error: could not file character \".\" in path %s", path);
        exit(1);
    }

    lastFullStop++;                            // Stops fullstop character being included
    const int length = pathLen - lastFullStop; // gets length of file ext
    char *extension = malloc(length + 1);
    for (int i = 0; i < length; i++)
    {
        extension[i] = path[lastFullStop + i]; // string doesn't include fullstop
    }
    extension[length] = '\0';
    return extension;
}

char EMSCRIPTEN_KEEPALIVE *getSubstring(char *Text, int StartIndex, int EndIndex) // Returns substring between start and end indexes
{
    const int substringLength = EndIndex - StartIndex + 1;    // Gets the length of the substring
    char *substring = malloc(sizeof(char) * substringLength); // Allocates memory for substring
    for (int i = 0; i < substringLength; i++)
    {
        substring[i] = Text[StartIndex + i];
    }
    substring[substringLength] = '\0'; // terminate string with null terminator
    return substring;
}

char **SplitStringByChar(char *str, const char delimiter)
{
    unsigned int NumOfTokens = 0;
    char **Result = malloc(sizeof(char *));
    char *token = strtok(str, &delimiter);

    while (token != NULL)
    {
        NumOfTokens++;
        Result = (char **)malloc(sizeof(char *) * NumOfTokens + 1);
        Result[NumOfTokens - 1] = (char *)malloc(sizeof(char) * strlen(token));

        strcpy(Result[NumOfTokens - 1], token);
        token = strtok(NULL, &delimiter);
    }
    return Result;
}

char EMSCRIPTEN_KEEPALIVE *TurnToFullRelativePath(char *path, char *BasePath)
{ // Turns a relative path into absolute path
    /*if (!containsCharacter(path, ':')) {
        if (PATH_SEPARATOR) {}
    }
    if (!containsCharacter(path,':') || PATH_SEPARATOR == '/') {
        if (path[0] == '/' || path[0] == '\\') {
            *path += 1;
            strcat(entryPath, path);
            return entryPath;
        }
    }*/

    char *tempHolder; // Buffer to hold the absolute path

    if (path[0] == '/' || path[0] == '\\')
    {
        tempHolder = malloc(sizeof(char *) * (strlen(path) + strlen(Settings.entry)) + 1);
        strcpy(tempHolder, Settings.entry);
        strcat(tempHolder, path);
        return tempHolder;
    }
    else
    {

        int MatchesNum = GetNumOfRegexMatches(path, "\\.\\./");

        if (MatchesNum > 0)
        { // Handles paths containing ../
            printf("shoudnt run here\n");
            if (BasePath[0] == '\0')
            { // BasePath is only needed for paths with ../
                printf("Error no base path specified");
                exit(1);
                char *NeedVariableForNoError = malloc(sizeof(char));
                return NeedVariableForNoError;
            }
            char *PathCopy;
            strcpy(PathCopy, BasePath); // Create a copy of the path variable so it doesn't get overwritten by strtok()

            char **SplitString = SplitStringByChar(PathCopy, '/');

            char *FinalString = malloc(sizeof(char) * (strlen(path) + strlen(BasePath) + strlen(Settings.entry) + 1)); // Probably very inefficient
            int ArrayIndex = 0;
            for (int i = 0; i < (sizeof(SplitString) / sizeof(char *)) - MatchesNum; i++) // loops through array except for elements that need to be removed
            {
                for (int j = 0; j < sizeof(*SplitString[i]); j++) // Need to implement better way to do this
                {
                    if (!SplitString[i][j])
                    {
                        break;
                    }
                    if (SplitString[i][j] == '\0')
                    {
                        break;
                    }
                    else
                    {
                        FinalString[ArrayIndex] = SplitString[i][j];
                    }
                    ArrayIndex++;
                }
                FinalString[ArrayIndex] = '/';
                ArrayIndex++;
            }
            // FinalString[ArrayIndex] = '\0';
            char *TempEntry;
            strcpy(TempEntry, Settings.entry);
            strcat(TempEntry, FinalString);
            strcat(TempEntry, path);
            return TempEntry;
        }
        else
        {
            if (strstr(path, Settings.entry) != NULL) // path is already full path (might accidentally include paths with entry name in folder path)path o
            {
                return path;
            }
            char *TempPath; // Very messy code
            strcpy(TempPath, BasePath);
            strncat(TempPath, "/", 1);
            strcat(TempPath, path);
            return TempPath;
        }
    }
    return path; // Stops compiler from throwing exception on higher optimization levels
}
