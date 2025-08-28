#ifndef BOOKINGS_H
#define BOOKINGS_H

#include <stdbool.h>

/*
Structures:
  - Passenger: Stores passenger details.
  - Bus: Stores bus details, including passengers and capacity.
  - Booking: Stores booking details for waiting list management.
*/
typedef struct Passenger
{
    char name[36];
    long long mobile;
    char boardingPoint[50];
    char droppingPoint[50];
} Passenger;

typedef struct Bus
{
    char busId[11];
    char driverName[36];
    int capacity;
    char boardingPoint[50];
    char droppingPoint[50];
    Passenger passengers[10]; // Max capacity is 10
    int passengerCount;
    bool isFull;
    struct Bus *next;
} Bus;

typedef struct Booking
{
    char name[36];
    long long mobile;
    char boardingPoint[50];
    char droppingPoint[50];
    char busId[11];
    struct Booking *next;
    struct Booking *prev;
} Booking;

/*
Global variables
- busList: Pointer to the head of the bus linked list.
- waitingListHead: Pointer to the head of the waiting list.
- waitingListTail: Pointer to the tail of the waiting list.
*/
extern Bus *busList;
extern Booking *waitingListHead;
extern Booking *waitingListTail;

/*
Function declarations
- addBus: Adds a new bus to the system.
- bookTicket: Books a ticket for a passenger.
- cancelBooking: Cancels a booking for a passenger.
*/
void addBus(char *busId, int capacity, char *driverName, char *boardingPoint, char *droppingPoint);
void bookTicket(char *name, long long mobile, char *boardingPoint, char *droppingPoint);
void cancelBooking(char *busId, long long mobile);

// Helper function declarations for validation and parsing input.
void trim(char *str);
int isValidBusId(char *busId);
int isValidMobile(long long mobile);
int isEmptyString(char *str);
int parseAddBusLine(char *line, char *busId, int *capacity, char *driverName,
                    char *boardingPoint, char *droppingPoint);
int parseBookingLine(char *line, char *name, long long *mobile,
                     char *boardingPoint, char *droppingPoint);
int parseCancelLine(char *line, char *busId, long long *mobile);
int busExists(char *busId);

#endif

