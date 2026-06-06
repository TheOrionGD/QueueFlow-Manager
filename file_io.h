#ifndef FILE_IO_H
#define FILE_IO_H

#include "seat.h"
#include "queue.h"

// Function prototypes
void saveData(SeatList* seatList, BookingQueue* queue, const char* seatFile, const char* queueFile);
void loadData(SeatList* seatList, BookingQueue* queue, const char* seatFile, const char* queueFile);
void exportToCSV(SeatList* seatList, BookingQueue* queue, const char* filename);

#endif // FILE_IO_H
