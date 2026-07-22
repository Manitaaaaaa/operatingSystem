#include <stdio.h>
#include <string.h>

int main()
{
    int choice;
    char name[50];
    char text[100];
    char username[20];
    char password[20];
    char read;
    char write;
    char execute;
    char data[200];
    int i;
    char ch;
    FILE *fp;
    FILE *logFile;

    // Login
    printf("Login\n");

    printf("Username: ");
    scanf("%s", username);

    printf("Password: ");
    scanf("%s", password);

    if(strcmp(username, "student") == 0 && strcmp(password, "os2026") == 0)
    {
        printf("Login Successful\n\n");
    }
    else
    {
        printf("Invalid Username or Password\n");
        return 0;
    }

    // Menu
    printf("File System\n");
    printf("1. Create File\n");
    printf("2. Read File\n");
    printf("3. Write File\n");
    printf("4. Delete File\n");
    printf("5. File Permission\n");
    printf("6. Encrypt File\n");
    printf("7. Decrypt File\n");
    printf("8. Exit\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    // Create File
    if(choice == 1)
    {
        printf("Enter file name: ");
        scanf("%s", name);

        fp = fopen(name, "w");

        if(fp != NULL)
        {
            printf("File created.\n");

            logFile = fopen("audit.txt", "a");
            if(logFile != NULL)
            {
                fprintf(logFile, "File Created : %s\n", name);
                fclose(logFile);
            }

            fclose(fp);
        }
        else
        {
            printf("Error creating file.\n");
        }
    }

    // Read File
    else if(choice == 2)
    {
        printf("Enter file name: ");
        scanf("%s", name);

        fp = fopen(name, "r");

        if(fp == NULL)
        {
            printf("File not found.\n");
        }
        else
        {
            logFile = fopen("audit.txt", "a");
            if(logFile != NULL)
            {
                fprintf(logFile, "File Read : %s\n", name);
                fclose(logFile);
            }

            printf("\nFile Content:\n");

            while((ch = fgetc(fp)) != EOF)
            {
                printf("%c", ch);
            }

            fclose(fp);
        }
    }

    // Write File
    else if(choice == 3)
    {
        printf("Enter file name: ");
        scanf("%s", name);

        fp = fopen(name, "a");

        if(fp == NULL)
        {
            printf("Error opening file.\n");
        }
        else
        {
            printf("Enter text: ");
            scanf(" %[^\n]", text);

            fprintf(fp, "%s\n", text);

            printf("Data written.\n");

            logFile = fopen("audit.txt", "a");
            if(logFile != NULL)
            {
                fprintf(logFile, "File Modified : %s\n", name);
                fclose(logFile);
            }

            fclose(fp);
        }
    }

    // Delete File
    else if(choice == 4)
    {
        printf("Enter file name: ");
        scanf("%s", name);

        if(remove(name) == 0)
        {
            printf("File deleted.\n");

            logFile = fopen("audit.txt", "a");
            if(logFile != NULL)
            {
                fprintf(logFile, "File Deleted : %s\n", name);
                fclose(logFile);
            }
        }
        else
        {
            printf("File not found.\n");
        }
    }

    // File Permission
    else if(choice == 5)
    {
        printf("Enter file name: ");
        scanf("%s", name);

        printf("Read Permission (y/n): ");
        scanf(" %c", &read);

        printf("Write Permission (y/n): ");
        scanf(" %c", &write);

        printf("Execute Permission (y/n): ");
        scanf(" %c", &execute);

        printf("\nPermissions for %s\n", name);
        printf("Read    : %c\n", read);
        printf("Write   : %c\n", write);
        printf("Execute : %c\n", execute);
    }

    // Encrypt
    else if(choice == 6)
    {
        printf("Enter text: ");
        scanf(" %[^\n]", data);

        for(i = 0; data[i] != '\0'; i++)
        {
            data[i] = data[i] + 3;
        }

        printf("Encrypted Text: %s\n", data);
    }

    // Decrypt
    else if(choice == 7)
    {
        printf("Enter encrypted text: ");
        scanf(" %[^\n]", data);

        for(i = 0; data[i] != '\0'; i++)
        {
            data[i] = data[i] - 3;
        }

        printf("Decrypted Text: %s\n", data);
    }

    // Exit
    else if(choice == 8)
    {
        printf("Program Ended.\n");
    }

    // Invalid Choice
    else
    {
        printf("Invalid Choice.\n");
    }

    return 0;
}
