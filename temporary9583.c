#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 int main(int argc, char const *argv[]){char code[999999] = "print(\"hello world\")";for(int i = 0; i < strlen(code); i++){if(code[i] == '$'){code[i] = '\n';}}FILE * fptr;fptr = fopen("temporary9583.py", "w");fwrite(code, sizeof(char), strlen(code), fptr);fclose(fptr);system("python3 temporary9583.py");remove("temporary9583.py");return 0;}