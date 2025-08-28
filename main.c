#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bookings.h"
#include "operations.h"

// Function declarations for parsing (from bookings.c)
extern int parseAddBusLine(char *line, char *busId, int *capacity, char *driverName,char *boardingPoint, char *droppingPoint);
extern int parseBookingLine(char *line, char *name, long long *mobile,char *boardingPoint, char *droppingPoint);
extern int parseCancelLine(char *line, char *busId, long long *mobile);

void flushInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // discard till buffer line end
    }
}

int isValidBusIdMain(const char* busId) {
    if (busId == NULL || strlen(busId) == 0) 
        return 0;
    
    int len = strlen(busId);

    // Must be exactly 10 characters
    if (len != 10) 
        return 0;

    // Must contain only alphanumeric characters
    for (int i = 0; i < len; i++) {
        if (!isalnum((unsigned char)busId[i])) 
            return 0;
    }

    return 1;  // valid
}

int main()
{
    int n;

    // check initial input
    if (scanf("%d", &n) != 1 || n < 0)
    {
        printf("Error: Invalid number of buses\n");
        printf("Program Terminated\n");
        return 1;
    }

    // Read buses
    for (int i = 0; i < n; i++)
    {
        char line[300];

        // scanf return type 1 mtlb no error, return type -1/0 mtlb error
        if (scanf(" %299[^\n]", line) != 1)
        {
            printf("Error: Expected %d bus entries, but could not read entry %d\n", n, i + 1);
            printf("Program Terminated\n");
            return 1;
        }

        char busId[11], driverName[36], boardingPoint[50], droppingPoint[50];
        int capacity;

        // parse keeya gya ha taki input mai koi gdbd ho to handle ho jaey
        if (!parseAddBusLine(line, busId, &capacity, driverName, boardingPoint, droppingPoint))
        {
            printf("Error: Invalid bus data format in entry %d: %s\n", i + 1, line);
            printf("Program Terminated\n");
            return 1;
        }

        addBus(busId, capacity, driverName, boardingPoint, droppingPoint);
    }

    // Print sorted bus list
    printBusList();

    // Process operations
    char operation;
    while (scanf(" %c", &operation) == 1 && operation != 'e')
    {
        if (operation == 'a')
        {
            char line[300];
            if (scanf(" %299[^\n]", line) != 1)
            {
                printf("Error: Could not read bus data for add operation\n");
                printf("Program Terminated\n");
                return 1;
            }

            char busId[11], driverName[36], boardingPoint[50], droppingPoint[50];
            int capacity;

            if (!parseAddBusLine(line, busId, &capacity, driverName, boardingPoint, droppingPoint))
            {
                printf("Error: Invalid bus data format: %s\n", line);
                printf("Program Terminated\n");
                return 1;
            }

            addBus(busId, capacity, driverName, boardingPoint, droppingPoint);
        }
        else if (operation == 'b')
        {
            char line[300];
            if (scanf(" %299[^\n]", line) != 1)
            {
                printf("Error: Could not read booking data\n");
                printf("Program Terminated\n");
                return 1;
            }

            char name[36], boardingPoint[50], droppingPoint[50];
            long long mobile;

            if (!parseBookingLine(line, name, &mobile, boardingPoint, droppingPoint))
            {
                printf("Error: Invalid booking data format: %s\n", line);
                printf("Program Terminated\n");
                return 1;
            }

            bookTicket(name, mobile, boardingPoint, droppingPoint);
        }
        else if (operation == 'c')
        {
            char line[300];
            if (scanf(" %299[^\n]", line) != 1)
            {
                printf("Error: Could not read cancellation data\n");
                printf("Program Terminated\n");
                return 1;
            }

            char busId[11];
            long long mobile;

            if (!parseCancelLine(line, busId, &mobile))
            {
                printf("Error: Invalid cancellation data format: %s\n", line);
                printf("Program Terminated\n");
                return 1;
            }

            cancelBooking(busId, mobile);
        }
        else if (operation == 'l')
        {
            char busId[11];
            if (scanf(" %10s", busId) != 1)
            {
                printf("Error: Could not read bus ID for list operation\n");
                printf("Program Terminated\n");
                return 1;
            }
            flushInputBuffer();  // clears any leftover chars in the same line

            if (!isValidBusIdMain(busId))
            {
                printf("ERROR: Invalid Bus ID '%s'. Must be exactly 10 alphanumeric characters.\n", busId);
                printf("Program Terminated\n");
                return 1;
            }
            listPassengers(busId);
        }
        else if (operation == 'k')
        {
            char busId[11];
            if (scanf(" %10s", busId) != 1)
            {
                printf("Error: Could not read bus ID for count operation\n");
                printf("Program Terminated\n");
                return 1;
                printf("Program Terminated\n");
                return 1;
            }
            flushInputBuffer();  // clears any leftover chars and use keeya h extra char jo busId me aa jata h usko htane ke liye

            if (!isValidBusIdMain(busId))
            {
                printf("ERROR: Invalid Bus ID '%s'. Must be exactly 10 alphanumeric characters.\n", busId);
                printf("Program Terminated\n");
                return 1;
            }
            countBookings(busId);
        }
        else
        {
            printf("Error: Unknown operation '%c'\n", operation);
            return 1;
        }
    }

    printf("Program Terminated\n");
    return 0;
}