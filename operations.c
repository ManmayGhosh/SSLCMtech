#include <stdio.h>
#include <string.h>
#include "bookings.h"
#include "operations.h"

int listPassengers(char* busId) {
    if (busId == NULL || strlen(busId) == 0) {
        printf("BUS NOT FOUND\n");
        return 0;
    }
    Bus* current = busList;
    
    // Find bus
    while (current != NULL && strcmp(current->busId, busId) != 0) {
        current = current->next;
    }

    if (current == NULL) {
        printf("BUS NOT FOUND\n");
        return 0;
    }

    // Print all passengers for this busId
    for (int i = 0; i < current->passengerCount; i++) {
        printf("%s,%lld,%s,%s\n", 
               current->passengers[i].name,
               current->passengers[i].mobile,
               current->passengers[i].boardingPoint,
               current->passengers[i].droppingPoint);
    }
    return 1;
}

int countBookings(char* busId) {
    if (busId == NULL || strlen(busId) == 0) {
        printf("0\n");
        return 0;
    }
    int count = 0;
    Booking* current = waitingListHead;

    // Count passengers in waiting list for this bus
    while (current != NULL) {
        if (strcmp(current->busId, busId) == 0) {
            count++;
        }
        current = current->next;
    }

    printf("%d\n", count);
    return 1;
}

int sortBusList() {
    if (busList == NULL || busList->next == NULL) {
        return 1; // Nothing to sort, but not an error
    }

    // Convert linked list to array for sorting
    Bus* buses[500];
    int count = 0;
    Bus* current = busList;
    
    while (current != NULL && count < 500) { // Prevent buffer overflow
        buses[count++] = current;
        current = current->next;
    }

    if (current != NULL) {
        // Too many buses, buffer overflow would occur
        return 0;
    }

    // Simple bubble sort by busId
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (strcmp(buses[j]->busId, buses[j + 1]->busId) > 0) {
                Bus* temp = buses[j];
                buses[j] = buses[j + 1];
                buses[j + 1] = temp;
            }
        }
    }

    // Rebuild linked list
    if (count > 0) {
        busList = buses[0];
        for (int i = 0; i < count - 1; i++) {
            buses[i]->next = buses[i + 1];
        }
        buses[count - 1]->next = NULL;
    }
    return 1;
}

int printBusList() {
    if (!sortBusList()) {
        printf("Error: Too many buses to sort\n");
        return 0;
    }
    Bus* current = busList;
    
    while (current != NULL) {
        printf("%s,%d,%s,%s,%s\n",
               current->busId,
               current->capacity,
               current->driverName,
               current->boardingPoint,
               current->droppingPoint);
        current = current->next;
    }
    return 1;
}