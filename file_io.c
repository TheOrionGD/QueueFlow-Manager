#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_io.h"

void saveData(SeatList* seatList, BookingQueue* queue, const char* seatFile, const char* queueFile) {
    // Save seats
    FILE* sf = fopen(seatFile, "w");
    if (sf != NULL) {
        // Save total seats first
        fprintf(sf, "%d\n", seatList->totalSeats);
        SeatNode* current = seatList->head;
        while (current != NULL) {
            if (current->isBooked) {
                fprintf(sf, "%d,%d,%s\n", current->seatNumber, 1, current->passengerName);
            } else {
                fprintf(sf, "%d,%d,\n", current->seatNumber, 0);
            }
            current = current->next;
        }
        fclose(sf);
        printf("Seat data saved successfully.\n");
    } else {
        printf("Error opening %s for writing.\n", seatFile);
    }

    // Save queue
    FILE* qf = fopen(queueFile, "w");
    if (qf != NULL) {
        QueueNode* current = queue->front;
        while (current != NULL) {
            fprintf(qf, "%d,%d,%s\n", current->seatNumber, current->isVIP ? 1 : 0, current->passengerName);
            current = current->next;
        }
        fclose(qf);
        printf("Waitlist data saved successfully.\n");
    } else {
        printf("Error opening %s for writing.\n", queueFile);
    }
}

void loadData(SeatList* seatList, BookingQueue* queue, const char* seatFile, const char* queueFile) {
    // Load seats
    FILE* sf = fopen(seatFile, "r");
    if (sf != NULL) {
        int totalSeats = 0;
        if (fscanf(sf, "%d\n", &totalSeats) == 1) {
            // Re-initialize list if needed
            if (seatList->head != NULL) {
                freeSeatList(seatList);
            }
            initializeSeatList(seatList, totalSeats);

            int seatNum, isBooked;
            char passengerName[50];
            char line[100];
            while (fgets(line, sizeof(line), sf)) {
                // Parse line: seatNumber,isBooked,passengerName
                char* token = strtok(line, ",");
                if (token != NULL) seatNum = atoi(token);
                
                token = strtok(NULL, ",");
                if (token != NULL) isBooked = atoi(token);
                
                token = strtok(NULL, "\n");
                if (token != NULL && isBooked == 1) {
                    strcpy(passengerName, token);
                    bookSeat(seatList, seatNum, passengerName);
                }
            }
        }
        fclose(sf);
        printf("Seat data loaded successfully.\n");
    } else {
        printf("No existing seat data found. Starting fresh.\n");
    }

    // Load queue
    FILE* qf = fopen(queueFile, "r");
    if (qf != NULL) {
        char line[100];
        while (fgets(line, sizeof(line), qf)) {
            int seatNum = 0, isVIP = 0;
            char passengerName[50] = "";
            char* token = strtok(line, ",");
            if (token != NULL) seatNum = atoi(token);
            
            token = strtok(NULL, ",");
            if (token != NULL) isVIP = atoi(token);
            
            token = strtok(NULL, "\n");
            if (token != NULL) {
                strcpy(passengerName, token);
                enqueueBooking(queue, seatNum, passengerName, isVIP == 1);
            }
        }
        fclose(qf);
        printf("Waitlist data loaded successfully.\n");
    }
}

void exportToCSV(SeatList* seatList, BookingQueue* queue, const char* filename) {
    FILE* f = fopen(filename, "w");
    if (f == NULL) {
        printf("Error opening %s for writing.\n", filename);
        return;
    }
    
    fprintf(f, "Type,SeatNumber,PassengerName,Status/VIP\n");
    
    // Export booked seats
    SeatNode* currentSeat = seatList->head;
    while (currentSeat != NULL) {
        if (currentSeat->isBooked) {
            fprintf(f, "Booking,%d,%s,N/A\n", currentSeat->seatNumber, currentSeat->passengerName);
        }
        currentSeat = currentSeat->next;
    }
    
    // Export waitlist
    QueueNode* currentQueue = queue->front;
    while (currentQueue != NULL) {
        fprintf(f, "Waitlist,%d,%s,%s\n", currentQueue->seatNumber, currentQueue->passengerName, currentQueue->isVIP ? "VIP" : "Standard");
        currentQueue = currentQueue->next;
    }
    
    fclose(f);
    printf("Successfully exported bookings and waitlist to %s\n", filename);
}
