#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    //making sure python and c compiler is installed
    int checkPython = system("python3 --version > /dev/null 2>&1"); // checks if python is installed | succes-> 0 else-> != 0
    if(checkPython == 0){ //succes=good
        printf("python is currently installed\n");
    } else { // instruction
        printf("in order to use py_to_exe you need to have python installed,  in this terminal type:\n1: sudo apt\n2: sudo apt install python\n\t after this you will be able to use py_to_exe\n");
        return 1;
    }
    int checkGcc = system("gcc --version > /dev/null 2>&1"); // check gcc
    if(checkGcc == 0){
        printf("gcc is currently installed\n");
    } else {
        printf("in order to use py_to_exe you need to have atlest gcc build-essential installed,  in this terminal type:\n1: sudo apt\n2: sudo apt install build-essential\n\t after this you will be able to use py_to_exe\n");
        return 1;
    }
    system("clear");

    //logo
    printf("         _____   __    _\n");
    printf("        /  __ \\  \\ \\  | |          _       ______   __    __    ______   \n");
    printf("        | |__| |  \\ \\_| |    _____| \\     /  ___ \\  \\ \\  / /   /  ___ \\  \n");
    printf("        |  ___/    \\    /   |        \\   |  |___| |  \\ \\/ /   |  |___| | \n");
    printf("        | |         |  |    |______  /   |   _____|   |  |    |   _____| \n");
    printf("        | |        /  /           |_/    |  |_____   / /\\ \\   |  |_____  \n");
    printf("        |_|       |__|                    \\______/  /_/  \\_\\   \\______/  \n");
    printf("______________________________________________________________________\n");

    //store path of input
    char path[400];
    printf("Path of python file: ");
    if(fgets(path, sizeof(path), stdin) != NULL){//error handling if path not great format
        path[strcspn(path, "\n")] = '\0';
        printf("path stored successfully: %s\n\n", path);
    }
    else{
        perror("error occured during the input of the path\n");
        return 1;
    }

    //project name
    char name[40];
    printf("Name of the project(can't include spaces): ");
    if(fgets(name, sizeof(name), stdin) != NULL){
        printf("project name stored:%s\n", name);
        for(int i = 1; i < strlen(name); i++){ //removing \n
            if(name[i] == '\n'){
                name[i] = '\0';
            }
        }
    }
    else{
        printf("error occured while processing what you entered(name of the project will be output)\n");
        strncpy(name, "output", sizeof("output"));
    }


    //desktop app
    char boolDesktopApp[10];
    printf("create a desktop app?, type: y/n: ");
    fgets(boolDesktopApp, sizeof(boolDesktopApp), stdin);
    if(strlen(boolDesktopApp) > 0){ //fgetc returns FEO in case of a problem
        printf("input succesfully stored:%c\n", boolDesktopApp[0]);
    }
    else{
        printf("error occuered while reading your input(desktop app will not be created)\n");
        boolDesktopApp[0] = 'n';
    }

    //get the path of the user
    char *usersPath;
    usersPath = getenv("HOME");//stores the pat of the users directory  /home/ker/

    if(usersPath != NULL){ //error handling
        printf("user's repository found: %s\n", usersPath);
    }
    else{
        printf("user's home directory could not be found\n");
        return 1;
    }

    //opening output folder in the users folder
    //making the command string   "mkdir %spy_to_exe_output", usersPath
    char outputFolder[205]; ///home/ker/py_to_exe_output
    strncpy(outputFolder, usersPath, strlen(usersPath));
    strncat(outputFolder, "/py_to_exe_output/", sizeof("/py_to_exe_output/"));

    char mkdir[210] = "mkdir "; //mkdir /home/ker/py_to_exe_output
    strncat(mkdir, outputFolder, sizeof(outputFolder));

    system(mkdir); //create output folder


    //open input file
    FILE *fileptr;
    fileptr = fopen(path, "r");
    if(fileptr != NULL){//error handling if file not existent or cant be accessed etc.
        printf("input python file opened succesfully\n");
    }
    else{
        perror("error occured during opening the file\n");
        return 1;
    }


    //store python script from input
    char code[99999];//holds the entire input python code
    char buffer[3000]; // hold a line of string
    while(fgets(buffer, sizeof(buffer), fileptr)!= NULL){//append the python script character by character into code variable
        for(int i = 0; i < strlen(buffer); i ++){
            if(buffer[i] == '\n'){
                code[strlen(code)] = '\f';   //to write the whole py script into a char[] variable in c we need to replace every newline character with a unique char wich is going to be replaced back in the executer c after it will not matter wether there are \n's
            }
            else if(buffer[i] == '"'){//to store the script as a char[] the format ot the " nned to be \t
                code[strlen(code)] = '\\'; //code[i] = \\ -> wich will be stored as a single '\'
                code[strlen(code)] = '"'; // code[i+1] = " so codei:codei+1 is '\', "
                }     
            else{
                code[strlen(code)] = buffer[i];
            }
        }
    }
    if(strlen(code) > 0){
        printf("content of the python file stored succesfully\n");
    }
    else{
        printf("error occured while storing the content of the python file\n");
        return 1;
    }
    //close filepointer
    fclose(fileptr);



    //open temporary output c file in the output folder
    char temporaryCsource[210];
    strcpy(temporaryCsource, outputFolder);
    strncat(temporaryCsource, "temporary9583.c", sizeof("temporary9583.c"));

    fileptr = fopen(temporaryCsource,  "w");
    if(fileptr != NULL){//error handling
        printf("temporary c file created succesfully\n");
    }
    else{
        perror("error occured while creating temporary c file\n");
        return 1;
    }

    char embeddedC[] = "#include <stdio.h>\n#include <stdlib.h>\n#include <string.h>\n int main(int argc, char const *argv[]){int checkPython = system(\"python3 --version > /dev/null 2>&1\");if(checkPython != 0){printf(\"in order to execute this you need to have python installed, in a terminal:\\n- sudo apt\\n- sudo apt install python\\n\t after this you will be able to execute this\\n\");return 1;}char code[999999] = \"%s\";for(int i = 0; i < strlen(code); i++){if(code[i] == '\\f'){code[i] = '\\n';}}FILE * fptr;fptr = fopen(\"temporary9583.py\", \"w\");fwrite(code, sizeof(char), strlen(code), fptr);fclose(fptr);system(\"python3 temporary9583.py\");remove(\"temporary9583.py\");return 0;}";

    //writing a functional c scipt: contains the py script, able to make a py script with it, and can execute it, and delete it after 
    int charsWritten = fprintf(fileptr, embeddedC, code);

    if(charsWritten > 0){
        printf("content of python file succesfully embedded into a c file\n");
    }
    else{
        printf("error occured why embedding the content of python file\n");
        return 1;
    }
    
    //closing the temporary c file
    fclose(fileptr);

    //destination of the output executable  
    char exePath[245];
    strncpy(exePath, outputFolder, strlen(outputFolder));
    strncat(exePath, name, strlen(name)); 
    char dest[250] = " -o ";
    strncat(dest, exePath, strlen(exePath));

    //make an executable from the c file
    //creating the executing command
    char command[260] = "gcc ";
    strncat(command, temporaryCsource, sizeof(temporaryCsource));
    strncat(command, dest, sizeof(dest));
    system(command);

    //delete temporary c script
    remove(temporaryCsource);



    //create desktop app in the output folder if needed
    if(boolDesktopApp[0] == 'y'){

        char boolTerminal[10];         //choice of terminal 
        char terminalCommand[18]= "\nTerminal=";
        printf("\nDoes the desktop app needs a terminal with executing? y/n: ");
        fgets(boolTerminal, sizeof(boolTerminal), stdin);
        if(boolTerminal[0] == 'y'){
            strncat(terminalCommand, "true\0", sizeof("true\0"));//needed
        }else{
            strncat(terminalCommand, "false\0", sizeof("false\0")); //not
        }

        char pathofIcon[200]; //icon to exe if needed
        printf("\npath of image(for desktop app icon) leave blank if not needed: ");
        //fgets(pathofIcon, sizeof(pathofIcon), stdin);
        fgets(pathofIcon, sizeof(pathofIcon), stdin);
        if(strlen(pathofIcon) > 1){
            printf("path stored: %s\n", pathofIcon);
        }
        else{
            printf("no icon needed");
        }

        //make the path of the desktop
        char desktopPath[300];
        strncpy(desktopPath, usersPath, sizeof(usersPath));
        strncat(desktopPath, "/Desktop/", sizeof("/Desktop/"));


        //make path for the desktop app
        char appPath[310];
        strncpy(appPath, desktopPath, sizeof(desktopPath));
        strncat(appPath, name, sizeof(name));
        strncat(appPath, ".desktop", sizeof(".desktop"));

        /*  content of desktop app
        [Desktop Entry]
        Version=1.0
        Type=Application
        Comment=displays market prices
        Terminal=true
        Name=
        Exec=
        Icon=
        Categories=Utility;Development;
        */

        //creating the content of desktop app
        char contentOfDesktopApp[300] = "[Desktop Entry]\nVersion=1.0\nType=Application\nComment=py_to_exe\nTerminal=true\nName=";

        strncat(contentOfDesktopApp, name, strlen(name)); // specify projname

        strncat(contentOfDesktopApp, "\nExec=", sizeof("\nExec="));

        strncat(contentOfDesktopApp, exePath, strlen(exePath));  //specify the source of the executable

        if(strlen(pathofIcon) > 1){ //specify the path of icon if needed
            strncat(contentOfDesktopApp, "\nIcon=", sizeof("\nIcon="));
            strncat(contentOfDesktopApp, pathofIcon, strlen(pathofIcon));
        }

        strncat(contentOfDesktopApp, terminalCommand, sizeof(terminalCommand));
        
        strncat(contentOfDesktopApp, "\nCategories=Utility;Development;", sizeof("\nTerminal=true\nCategories=Utility;Development;"));

        //opening desktop app
        fileptr = fopen(appPath, "w");
        if(fileptr != NULL){
            printf("\ndesktop app succesfully created\n");
        }
        else{
            printf("error occured while creating desktop app\n");
            return 1;
        }

        //writing desktop app
        int charsWritten = fprintf(fileptr, "%s", contentOfDesktopApp);
        if(charsWritten > 0){
            printf("inner code of desktop app succesfully created\n");
        }
        else{
            printf("error occured while writing the code of the desktop app");
        }

        //closing the desktop app file
        fclose(fileptr);


        //path of the output
        printf("_____\npath of output:\n");
        printf("\texecutable's path: %s%s\n",outputFolder, name);
        printf("\tdesktop app: %s\n", appPath);
    }
    else{
        //path of the output
        printf("_____\npath of output:\n");
        printf("\texecutable's path: %s/%s\n",outputFolder, name);
    }
    char help[10];
    printf("\ntype h for further informations or press enter to exit: ");     //does not close the window immidiatelly
    fgets(help, sizeof(help), stdin);
    if(help[0] == 'h'){
        printf("__________\nmanual\n");
        printf("the converted executable is able to be executed on it's own\n -you can move that etc.");
        printf("However, the desktop app is a shortcut to the executable:\n -so if you delete the execuble or move it away it will not work unless you chage source of the exec that the app is executing.");
        printf(" -if you change the place of the image you selected as an icon, you will have to do the mentioned changes in the source oh the .desktop, but with the Icon variable.");
    }
    return 0;
}
