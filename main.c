#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "seat.h"
#include "queue.h"
#include "file_io.h"

const char* SEAT_FILE = "bookings.dat";
const char* QUEUE_FILE = "waitlist.dat";

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int main() {
    SeatList seatList;
    seatList.head = NULL;
    seatList.totalSeats = 0;
    BookingQueue bookingQueue;
    initQueue(&bookingQueue);
    
    int choice, seatNumber;
    char passengerName[50];

    printf("Loading previous data...\n");
    loadData(&seatList, &bookingQueue, SEAT_FILE, QUEUE_FILE);

    if (seatList.head == NULL) {
        int totalSeats = 0;
        printf("\n--- Initial Setup ---\n");
        printf("Enter total number of seats for this system: ");
        if (scanf("%d", &totalSeats) != 1 || totalSeats <= 0) {
            printf("Invalid number of seats. Defaulting to 10.\n");
            totalSeats = 10;
        }
        clearInputBuffer();
        initializeSeatList(&seatList, totalSeats);
    }

    while (1) {
        printf("\n----- WAITING LIST MANAGEMENT SYSTEM -----\n");
        printf("1. Check seat availability\n");
        printf("2. Book a seat\n");
        printf("3. Cancel booking\n");
        printf("4. Edit booking\n");
        printf("5. Display seating chart\n");
        printf("6. Save & Exit\n");
        printf("7. Manage Seats\n");
        printf("8. Search & Reporting\n");
        printf("9. View Waitlist\n");
        printf("10. Auto-assign next available seat\n");
        printf("11. Export to CSV\n");
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            clearInputBuffer();
            printf("Invalid choice. Please enter a number.\n");
            continue;
        }
        clearInputBuffer();
        
        switch (choice) {
            case 1:
                printf("Enter seat number to check availability (1-%d): ", seatList.totalSeats);
                scanf("%d", &seatNumber);
                clearInputBuffer();
                if (isSeatAvailable(&seatList, seatNumber)) {
                    printf("Seat %d is available.\n", seatNumber);
                } else {
                    printf("Seat %d is already booked or invalid.\n", seatNumber);
                }
                break;
            case 2:
                printf("Enter seat number to book (1-%d): ", seatList.totalSeats);
                scanf("%d", &seatNumber);
                clearInputBuffer();
                if (seatNumber < 1 || seatNumber > seatList.totalSeats) {
                    printf("Invalid seat number.\n");
                } else {
                    printf("Enter passenger name: ");
                    fgets(passengerName, sizeof(passengerName), stdin);
                    passengerName[strcspn(passengerName, "\n")] = 0; // Remove trailing newline
                    
                    int isVIP = 0;
                    printf("Is this a VIP booking? (1 for Yes, 0 for No): ");
                    scanf("%d", &isVIP);
                    clearInputBuffer();
                    
                    if (!bookSeat(&seatList, seatNumber, passengerName)) {
                        enqueueBooking(&bookingQueue, seatNumber, passengerName, isVIP == 1);
                        processBookingQueue(&seatList, &bookingQueue);
                    } else {
                        printf("Booked %s to seat %d.\n", passengerName, seatNumber);
                    }
                }
                break;
            case 3:
                printf("Enter seat number to cancel booking: ");
                scanf("%d", &seatNumber);
                clearInputBuffer();
                if (seatNumber < 1 || seatNumber > seatList.totalSeats) {
                    printf("Invalid seat number.\n");
                } else {
                    cancelBooking(&seatList, seatNumber);
                    // Process waitlist automatically after cancellation
                    processBookingQueue(&seatList, &bookingQueue);
                }
                break;
            case 4:
                printf("Enter seat number to edit booking: ");
                scanf("%d", &seatNumber);
                clearInputBuffer();
                if (seatNumber < 1 || seatNumber > seatList.totalSeats) {
                    printf("Invalid seat number.\n");
                } else {
                    printf("Enter new passenger name: ");
                    fgets(passengerName, sizeof(passengerName), stdin);
                    passengerName[strcspn(passengerName, "\n")] = 0;
                    editBooking(&seatList, seatNumber, passengerName);
                }
                break;
            case 5:
                displaySeatingChart(&seatList);
                break;
            case 6:
                saveData(&seatList, &bookingQueue, SEAT_FILE, QUEUE_FILE);
                freeSeatList(&seatList);
                freeQueue(&bookingQueue);
                printf("Exiting system...\n");
                exit(0);
            case 7:
                printf("\n--- Manage Seats ---\n");
                printf("1. Add Seats\n2. Remove Seats\nEnter choice: ");
                int subchoice7;
                if (scanf("%d", &subchoice7) == 1) {
                    clearInputBuffer();
                    int numSeats;
                    if (subchoice7 == 1) {
                        printf("Enter number of seats to add: ");
                        if (scanf("%d", &numSeats) == 1) {
                            addSeats(&seatList, numSeats);
                        }
                    } else if (subchoice7 == 2) {
                        printf("Enter number of seats to remove: ");
                        if (scanf("%d", &numSeats) == 1) {
                            removeSeats(&seatList, numSeats);
                        }
                    } else {
                        printf("Invalid choice.\n");
                    }
                    clearInputBuffer();
                } else {
                    clearInputBuffer();
                    printf("Invalid input.\n");
                }
                break;
            case 8:
                printf("\n--- Search & Reporting ---\n");
                printf("1. Search Passenger\n2. View Statistics\nEnter choice: ");
                int subchoice8;
                if (scanf("%d", &subchoice8) == 1) {
                    clearInputBuffer();
                    if (subchoice8 == 1) {
                        char searchName[50];
                        printf("Enter passenger name to search: ");
                        fgets(searchName, sizeof(searchName), stdin);
                        searchName[strcspn(searchName, "\n")] = 0;
                        searchPassenger(&seatList, searchName);
                    } else if (subchoice8 == 2) {
                        reportBookingStats(&seatList);
                    } else {
                        printf("Invalid choice.\n");
                    }
                } else {
                    clearInputBuffer();
                    printf("Invalid input.\n");
                }
                break;
            case 9:
                displayWaitlist(&bookingQueue);
                break;
            case 10: {
                int nextSeat = getNextAvailableSeat(&seatList);
                if (nextSeat == -1) {
                    printf("No seats are currently available for auto-assignment.\n");
                } else {
                    printf("Auto-assigned seat %d.\n", nextSeat);
                    printf("Enter passenger name: ");
                    fgets(passengerName, sizeof(passengerName), stdin);
                    passengerName[strcspn(passengerName, "\n")] = 0; // Remove trailing newline
                    
                    int isVIP = 0;
                    printf("Is this a VIP booking? (1 for Yes, 0 for No): ");
                    scanf("%d", &isVIP);
                    clearInputBuffer();
                    
                    if (!bookSeat(&seatList, nextSeat, passengerName)) {
                        printf("Failed to auto-assign seat %d.\n", nextSeat);
                    } else {
                        printf("Successfully booked %s to seat %d.\n", passengerName, nextSeat);
                    }
                }
                break;
            }
            case 11: {
                char filename[100];
                printf("Enter filename for export (e.g. data.csv): ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;
                exportToCSV(&seatList, &bookingQueue, filename);
                break;
            }
            default:
                printf("Invalid choice\n");
        }
    }

    return 0;
}
