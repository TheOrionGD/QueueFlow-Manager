#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seat.h"

SeatNode* createSeatNode(int seatNumber) {
    SeatNode* newNode = (SeatNode*)malloc(sizeof(SeatNode));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    newNode->seatNumber = seatNumber;
    newNode->isBooked = false;
    strcpy(newNode->passengerName, "");
    newNode->next = NULL;
    return newNode;
}

void initializeSeatList(SeatList* seatList, int totalSeats) {
    seatList->head = NULL;
    seatList->totalSeats = totalSeats;
    for (int i = 1; i <= totalSeats; i++) {
        SeatNode* newNode = createSeatNode(i);
        // Insert at the end to keep seat numbers sorted 1 to totalSeats
        if (seatList->head == NULL) {
            seatList->head = newNode;
        } else {
            SeatNode* current = seatList->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
}

bool isSeatAvailable(SeatList* seatList, int seatNumber) {
    SeatNode* current = seatList->head;
    while (current != NULL) {
        if (current->seatNumber == seatNumber) {
            return !current->isBooked;
        }
        current = current->next;
    }
    return false; // Seat not found
}

int getNextAvailableSeat(SeatList* seatList) {
    SeatNode* current = seatList->head;
    while (current != NULL) {
        if (!current->isBooked) {
            return current->seatNumber;
        }
        current = current->next;
    }
    return -1; // No seats available
}

bool bookSeat(SeatList* seatList, int seatNumber, const char* passengerName) {
    SeatNode* current = seatList->head;
    while (current != NULL) {
        if (current->seatNumber == seatNumber) {
            if (current->isBooked) {
                return false; // Seat already booked
            }
            current->isBooked = true;
            strncpy(current->passengerName, passengerName, 49);
            current->passengerName[49] = '\0';
            return true;
        }
        current = current->next;
    }
    return false; // Seat not found
}

void cancelBooking(SeatList* seatList, int seatNumber) {
    SeatNode* current = seatList->head;
    while (current != NULL) {
        if (current->seatNumber == seatNumber && current->isBooked) {
            current->isBooked = false;
            strcpy(current->passengerName, "");
            printf("Booking for seat %d has been cancelled.\n", seatNumber);
            return;
        }
        current = current->next;
    }
    printf("No booking found for seat %d.\n", seatNumber);
}

void editBooking(SeatList* seatList, int seatNumber, const char* passengerName) {
    SeatNode* current = seatList->head;
    while (current != NULL) {
        if (current->seatNumber == seatNumber && current->isBooked) {
            strncpy(current->passengerName, passengerName, 49);
            current->passengerName[49] = '\0';
            printf("Booking for seat %d has been updated to %s.\n", seatNumber, passengerName);
            return;
        }
        current = current->next;
    }
    printf("No booking found for seat %d.\n", seatNumber);
}

void displaySeatingChart(SeatList* seatList) {
    SeatNode* current = seatList->head;
    printf("\n--- Seating Chart ---\n");
    while (current != NULL) {
        printf("Seat %d: %s\n", current->seatNumber, current->isBooked ? current->passengerName : "Available");
        current = current->next;
    }
    printf("---------------------\n");
}

void freeSeatList(SeatList* seatList) {
    SeatNode* current = seatList->head;
    while (current != NULL) {
        SeatNode* next = current->next;
        free(current);
        current = next;
    }
    seatList->head = NULL;
}

void addSeats(SeatList* seatList, int numSeats) {
    if (numSeats <= 0) return;
    int currentTotal = seatList->totalSeats;
    for (int i = 1; i <= numSeats; i++) {
        SeatNode* newNode = createSeatNode(currentTotal + i);
        if (seatList->head == NULL) {
            seatList->head = newNode;
        } else {
            SeatNode* current = seatList->head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
        }
    }
    seatList->totalSeats += numSeats;
    printf("Successfully added %d seats. Total seats are now %d.\n", numSeats, seatList->totalSeats);
}

void removeSeats(SeatList* seatList, int numSeats) {
    if (numSeats <= 0 || seatList->totalSeats - numSeats < 1) {
        printf("Invalid number of seats to remove. Must leave at least 1 seat.\n");
        return;
    }
    
    // Check if the last `numSeats` are unbooked
    SeatNode* current = seatList->head;
    while (current != NULL) {
        if (current->seatNumber > seatList->totalSeats - numSeats && current->isBooked) {
            printf("Cannot remove seats: Seat %d is currently booked.\n", current->seatNumber);
            return;
        }
        current = current->next;
    }
    
    // Proceed to remove from the end
    int newTotal = seatList->totalSeats - numSeats;
    current = seatList->head;
    SeatNode* prev = NULL;
    while (current != NULL) {
        if (current->seatNumber > newTotal) {
            if (prev != NULL) prev->next = NULL;
            // Free the remaining nodes
            SeatNode* temp = current;
            while (temp != NULL) {
                SeatNode* next = temp->next;
                free(temp);
                temp = next;
            }
            break;
        }
        prev = current;
        current = current->next;
    }
    seatList->totalSeats = newTotal;
    printf("Successfully removed %d seats. Total seats are now %d.\n", numSeats, seatList->totalSeats);
}

void searchPassenger(SeatList* seatList, const char* name) {
    SeatNode* current = seatList->head;
    bool found = false;
    printf("\n--- Search Results for '%s' ---\n", name);
    while (current != NULL) {
        if (current->isBooked && strstr(current->passengerName, name) != NULL) {
            printf("Seat %d: %s\n", current->seatNumber, current->passengerName);
            found = true;
        }
        current = current->next;
    }
    if (!found) {
        printf("No passengers found matching that name.\n");
    }
    printf("-------------------------------\n");
}

void reportBookingStats(SeatList* seatList) {
    int bookedCount = 0;
    SeatNode* current = seatList->head;
    while (current != NULL) {
        if (current->isBooked) {
            bookedCount++;
        }
        current = current->next;
    }
    printf("\n--- Booking Statistics ---\n");
    printf("Total Seats: %d\n", seatList->totalSeats);
    printf("Booked Seats: %d\n", bookedCount);
    printf("Available Seats: %d\n", seatList->totalSeats - bookedCount);
    printf("Occupancy Rate: %.2f%%\n", seatList->totalSeats > 0 ? ((float)bookedCount / seatList->totalSeats) * 100.0f : 0.0f);
    printf("--------------------------\n");
}
