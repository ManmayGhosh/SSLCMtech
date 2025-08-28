#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bookings.h"

// Global variables
Bus* busList = NULL;
Booking* waitingListHead = NULL;
Booking* waitingListTail = NULL;

// Help function to trim whitespace
void trim(char* str) 
{
    if (str == NULL) return;
    
    // Trim leading whitespace
    int start = 0;
    while (str[start] && isspace(str[start])) {
        start++;
    }
    
    // Shift string left
    int i = 0;
    while (str[start + i]) {
        str[i] = str[start + i];
        i++;
    }
    str[i] = '\0';
    
    // Trim trailing whitespace
    int end = strlen(str) - 1;
    while (end >= 0 && isspace(str[end])) {
        str[end] = '\0';
        end--;
    }
}

// Help function to validate bus ID format
int isValidBusId(char* busId) {
    if (busId == NULL || strlen(busId) == 0) return 0;
    
    int len = strlen(busId);
    if (len != 10) return 0;
    
    // Check only alphanumeric characters
    for (int i = 0; i < len; i++) {
        if (!isalnum(busId[i])) return 0;
    }
    
    return 1;
}

// Helper function to validate mobile number
int isValidMobile(long long mobile) {
    // Check positive number and has 10 digits
    return mobile > 0 && mobile >= 1000000000LL && mobile <= 9999999999LL;
}

// Helper function to check for empty or null string
int isEmptyString(char* str) {
    return str == NULL || strlen(str) == 0;
}

int parseAddBusLine(char* line, char* busId, int* capacity, char* driverName, 
                    char* boardingPoint, char* droppingPoint) {
    if (line == NULL) return 0;
    
    char* tokens[5];
    int tokenCount = 0;
    
    // Create a copy of the line to avoid modifying original line
    char lineCopy[300];
    strcpy(lineCopy, line);
    
    // Split by comma
    char* token = strtok(lineCopy, ",");
    while (token != NULL && tokenCount < 5) {
        tokens[tokenCount] = token;
        tokenCount++;
        token = strtok(NULL, ",");
    }
    
    // Check if we have exactly 5 tokens
    if (tokenCount < 5) {
        printf("Error: Missing field\n");
        return 0;
    }
    else if (tokenCount > 5) {
        printf("Error: Too many fields\n");
        return 0;
    }
    
    // Trim and validate each field
    trim(tokens[0]);
    if (!isValidBusId(tokens[0])){
        printf("Error: Invalid Bus ID\n");
        return 0;
    }
    strcpy(busId, tokens[0]);
    
    trim(tokens[1]);
    *capacity = atoi(tokens[1]);
    if (*capacity <= 0 || *capacity > 10){
        printf("Error: Invalid Capacity\n");
        return 0;
    }

    trim(tokens[2]);
    if (isEmptyString(tokens[2])) return 0;
    strcpy(driverName, tokens[2]);
    
    trim(tokens[3]);
    if (isEmptyString(tokens[3])) return 0;
    strcpy(boardingPoint, tokens[3]);
    
    trim(tokens[4]);
    if (isEmptyString(tokens[4])) return 0;
    strcpy(droppingPoint, tokens[4]);
    
    return 1;
}

int parseBookingLine(char* line, char* name, long long* mobile, 
                     char* boardingPoint, char* droppingPoint) {
    if (line == NULL) return 0;
    
    char* tokens[4];
    int tokenCount = 0;
    
    char lineCopy[300];
    strcpy(lineCopy, line);
    
    char* token = strtok(lineCopy, ",");
    while (token != NULL && tokenCount < 4) {
        tokens[tokenCount] = token;
        tokenCount++;
        token = strtok(NULL, ",");
    }
    
    if (tokenCount < 4){
        printf("Error: Missing field\n");
        return 0;
    }
    else if (tokenCount > 4) {
        printf("Error: Too many fields\n");
        return 0;
    } 
    
    trim(tokens[0]);
    if (isEmptyString(tokens[0])) return 0;
    strcpy(name, tokens[0]);
    
    trim(tokens[1]);
    *mobile = atoll(tokens[1]);
    if (!isValidMobile(*mobile)){
        printf("Error: Invalid Mobile Number\n");
        return 0;
    }
    
    trim(tokens[2]);
    if (isEmptyString(tokens[2])) return 0;
    strcpy(boardingPoint, tokens[2]);
    
    trim(tokens[3]);
    if (isEmptyString(tokens[3])) return 0;
    strcpy(droppingPoint, tokens[3]);
    
    return 1;
}

int parseCancelLine(char* line, char* busId, long long* mobile) {
    if (line == NULL) return 0;
    
    char* tokens[2];
    int tokenCount = 0;
    
    char lineCopy[300];
    strcpy(lineCopy, line);
    
    char* token = strtok(lineCopy, ",");
    while (token != NULL && tokenCount < 2) {
        tokens[tokenCount] = token;
        tokenCount++;
        token = strtok(NULL, ",");
    }
    
    if (tokenCount < 2)
    {
        printf("Error: Missing fields\n");
        return 0;
    }
    else if (tokenCount > 2) {
        printf("Error: Too many fields\n");
        return 0;
    }
    
    trim(tokens[0]);
    if (!isValidBusId(tokens[0])) {
        printf("Error: Invalid Bus ID\n");
        return 0;
    }
    strcpy(busId, tokens[0]);
    
    trim(tokens[1]);
    *mobile = atoll(tokens[1]);
    if (!isValidMobile(*mobile)){
        printf("Error: Invalid Mobile Number\n");
        return 0;
    }
    
    return 1;
}

// Check if bus ID already exists
int busExists(char* busId) {
    Bus* current = busList;
    while (current != NULL) {
        if (strcmp(current->busId, busId) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

void addBus(char* busId, int capacity, char* driverName, char* boardingPoint, char* droppingPoint) {
    // Check if bus already exists
    if (busExists(busId)) {
        return; // Silently ignore duplicate bus IDs as per original behavior
    }
    
    Bus* newBus = (Bus*)malloc(sizeof(Bus));
    if (newBus == NULL) {
        printf("Error: Memory allocation failed\n");
        exit(1);
    }
    
    strcpy(newBus->busId, busId);
    strcpy(newBus->driverName, driverName);
    newBus->capacity = capacity;
    strcpy(newBus->boardingPoint, boardingPoint);
    strcpy(newBus->droppingPoint, droppingPoint);
    newBus->passengerCount = 0;
    newBus->isFull = false;
    newBus->next = NULL;

    // Insert at the beginning of the list
    if (busList == NULL) {
        busList = newBus;
    } else {
        newBus->next = busList;
        busList = newBus;
    }

    printf("%s Added\n", busId);
}

void bookTicket(char* name, long long mobile, char* boardingPoint, char* droppingPoint) {
    Bus* current = busList;
    Bus* foundBus = NULL;

    // Search for matching bus
    while (current != NULL) {
        if (strcmp(current->boardingPoint, boardingPoint) == 0 && 
            strcmp(current->droppingPoint, droppingPoint) == 0) {
            foundBus = current;
            break;
        }
        current = current->next;
    }

    if (foundBus == NULL) {
        printf("BUS NOT FOUND\n");
        return;
    }

    // Check if passenger already booked on this bus
    for (int i = 0; i < foundBus->passengerCount; i++) {
        if (foundBus->passengers[i].mobile == mobile) {
            printf("%s\n", foundBus->busId); // Already booked, just return bus ID
            return;
        }
    }

    // Check if bus has capacity
    if (foundBus->passengerCount < foundBus->capacity) {
        // Book the ticket
        strcpy(foundBus->passengers[foundBus->passengerCount].name, name);
        foundBus->passengers[foundBus->passengerCount].mobile = mobile;
        strcpy(foundBus->passengers[foundBus->passengerCount].boardingPoint, boardingPoint);
        strcpy(foundBus->passengers[foundBus->passengerCount].droppingPoint, droppingPoint);
        foundBus->passengerCount++;

        if (foundBus->passengerCount == foundBus->capacity) {
            foundBus->isFull = true;
        }

        printf("%s\n", foundBus->busId);
    } else {
        // Check if already in waiting list
        Booking* waitingCurrent = waitingListHead;
        while (waitingCurrent != NULL) {
            if (waitingCurrent->mobile == mobile && 
                strcmp(waitingCurrent->busId, foundBus->busId) == 0) {
                printf("Waiting %s\n", foundBus->busId);
                return;
            }
            waitingCurrent = waitingCurrent->next;
        }
        
        // Add to waiting list
        Booking* newBooking = (Booking*)malloc(sizeof(Booking));
        if (newBooking == NULL) {
            printf("Error: Memory allocation failed\n");
            exit(1);
        }
        
        strcpy(newBooking->name, name);
        newBooking->mobile = mobile;
        strcpy(newBooking->boardingPoint, boardingPoint);
        strcpy(newBooking->droppingPoint, droppingPoint);
        strcpy(newBooking->busId, foundBus->busId);
        newBooking->next = NULL;
        newBooking->prev = NULL;

        // Insert at tail of doubly linked list
        if (waitingListTail == NULL) {
            waitingListHead = waitingListTail = newBooking;
        } else {
            waitingListTail->next = newBooking;
            newBooking->prev = waitingListTail;
            waitingListTail = newBooking;
        }

        printf("Waiting %s\n", foundBus->busId);
    }
}

void cancelBooking(char* busId, long long mobile) {
    Bus* bus = busList;
    
    // Find the bus
    while (bus != NULL && strcmp(bus->busId, busId) != 0) {
        bus = bus->next;
    }

    if (bus == NULL) {
        printf("BOOKING NOT FOUND\n");
        return;
    }

    // Find and remove passenger
    int found = -1;
    for (int i = 0; i < bus->passengerCount; i++) {
        if (bus->passengers[i].mobile == mobile) {
            found = i;
            break;
        }
    }

    if (found == -1) {
        printf("BOOKING NOT FOUND\n");
        return;
    }

    // Remove passenger by shifting array
    for (int i = found; i < bus->passengerCount - 1; i++) {
        bus->passengers[i] = bus->passengers[i + 1];
    }
    bus->passengerCount--;
    
    // Set isFull to false if it was true
    if (bus->isFull) {
        bus->isFull = false;
    }

    // Check waiting list for this bus
    Booking* current = waitingListHead;
    while (current != NULL) {
        if (strcmp(current->busId, busId) == 0) {
            // Move passenger from waiting list to bus
            strcpy(bus->passengers[bus->passengerCount].name, current->name);
            bus->passengers[bus->passengerCount].mobile = current->mobile;
            strcpy(bus->passengers[bus->passengerCount].boardingPoint, current->boardingPoint);
            strcpy(bus->passengers[bus->passengerCount].droppingPoint, current->droppingPoint);
            bus->passengerCount++;

            if (bus->passengerCount == bus->capacity) {
                bus->isFull = true;
            }

            printf("%s Added\n", current->name);

            // Remove from waiting list
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                waitingListHead = current->next;
            }
            
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                waitingListTail = current->prev;
            }

            free(current);
            return;
        }
        current = current->next;
    }
}
