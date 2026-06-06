#ifndef QUEUE_H
#define QUEUE_H

#include "seat.h"

// Define a structure for a queue node
typedef struct QueueNode {
    int seatNumber;
    bool isVIP;
    char passengerName[50];
    struct QueueNode* next;
} QueueNode;

// Define a structure for the booking queue
typedef struct {
    QueueNode* front;
    QueueNode* rear;
} BookingQueue;

// Function prototypes
void initQueue(BookingQueue* queue);
void enqueueBooking(BookingQueue* queue, int seatNumber, const char* passengerName, bool isVIP);
QueueNode* dequeueBooking(BookingQueue* queue);
void processBookingQueue(SeatList* seatList, BookingQueue* queue);
void displayWaitlist(BookingQueue* queue);
void freeQueue(BookingQueue* queue);

#endif // QUEUE_H
