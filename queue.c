#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"

QueueNode* createQueueNode(int seatNumber, const char* passengerName, bool isVIP) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->seatNumber = seatNumber;
    newNode->isVIP = isVIP;
    strncpy(newNode->passengerName, passengerName, 49);
    newNode->passengerName[49] = '\0';
    newNode->next = NULL;
    return newNode;
}

void initQueue(BookingQueue* queue) {
    queue->front = NULL;
    queue->rear = NULL;
}

void enqueueBooking(BookingQueue* queue, int seatNumber, const char* passengerName, bool isVIP) {
    QueueNode* newNode = createQueueNode(seatNumber, passengerName, isVIP);
    if (queue->front == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        if (isVIP) {
            // Find the last VIP node to insert after it, or insert at front if no VIPs
            if (!queue->front->isVIP) {
                newNode->next = queue->front;
                queue->front = newNode;
            } else {
                QueueNode* current = queue->front;
                while (current->next != NULL && current->next->isVIP) {
                    current = current->next;
                }
                newNode->next = current->next;
                current->next = newNode;
                if (newNode->next == NULL) {
                    queue->rear = newNode;
                }
            }
        } else {
            queue->rear->next = newNode;
            queue->rear = newNode;
        }
    }
    printf("Added %s (%s) to the waiting list for seat %d.\n", passengerName, isVIP ? "VIP" : "Standard", seatNumber);
}

QueueNode* dequeueBooking(BookingQueue* queue) {
    if (queue->front == NULL) {
        return NULL;
    }
    QueueNode* temp = queue->front;
    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    return temp;
}

void processBookingQueue(SeatList* seatList, BookingQueue* queue) {
    QueueNode* temp;
    BookingQueue tempQueue;
    initQueue(&tempQueue);
    
    // Process everything in the current queue
    while ((temp = dequeueBooking(queue)) != NULL) {
        if (bookSeat(seatList, temp->seatNumber, temp->passengerName)) {
            printf("Waitlist: Booked %s (%s) to seat %d.\n", temp->passengerName, temp->isVIP ? "VIP" : "Standard", temp->seatNumber);
            free(temp);
        } else {
            // Still cannot book, add to temporary queue
            enqueueBooking(&tempQueue, temp->seatNumber, temp->passengerName, temp->isVIP);
            free(temp);
        }
    }
    
    // Put remaining waitlisted users back into the original queue
    while ((temp = dequeueBooking(&tempQueue)) != NULL) {
        enqueueBooking(queue, temp->seatNumber, temp->passengerName, temp->isVIP);
        free(temp);
    }
}

void displayWaitlist(BookingQueue* queue) {
    if (queue->front == NULL) {
        printf("\n--- Waitlist is currently empty. ---\n");
        return;
    }
    printf("\n--- Current Waitlist ---\n");
    QueueNode* current = queue->front;
    int position = 1;
    while (current != NULL) {
        printf("%d. %s [Seat: %d] (%s)\n", position++, current->passengerName, current->seatNumber, current->isVIP ? "VIP" : "Standard");
        current = current->next;
    }
    printf("------------------------\n");
}

void freeQueue(BookingQueue* queue) {
    QueueNode* current = queue->front;
    while (current != NULL) {
        QueueNode* next = current->next;
        free(current);
        current = next;
    }
    queue->front = queue->rear = NULL;
}
