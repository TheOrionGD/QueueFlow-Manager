#ifndef SEAT_H
#define SEAT_H

#include <stdbool.h>

// Define a structure for a seat node in the linked list
typedef struct SeatNode {
    int seatNumber;
    bool isBooked;
    char passengerName[50];
    struct SeatNode* next;
} SeatNode;

// Define a structure for the seat linked list
typedef struct {
    SeatNode* head;
    int totalSeats;
} SeatList;

// Function prototypes
SeatNode* createSeatNode(int seatNumber);
void initializeSeatList(SeatList* seatList, int totalSeats);
bool isSeatAvailable(SeatList* seatList, int seatNumber);
bool bookSeat(SeatList* seatList, int seatNumber, const char* passengerName);
void cancelBooking(SeatList* seatList, int seatNumber);
void editBooking(SeatList* seatList, int seatNumber, const char* passengerName);
void displaySeatingChart(SeatList* seatList);
void freeSeatList(SeatList* seatList);
int getNextAvailableSeat(SeatList* seatList);
void addSeats(SeatList* seatList, int numSeats);
void removeSeats(SeatList* seatList, int numSeats);
void searchPassenger(SeatList* seatList, const char* name);
void reportBookingStats(SeatList* seatList);

#endif // SEAT_H
