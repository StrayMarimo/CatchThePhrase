#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

void ValidateArgs(char *file, int params_expected, int params_received);
void DieWithError(int lineIndex, char *errorMessage);
char* EncryptPhrase(char* phrase);
char* CapitalizePhrase(char* phrase);
void PrintLine(const char *format, ...);
void PrintSysMessage(int lineIndex, const char *format, ...);
void PrintFile(const char* filename);
#endif