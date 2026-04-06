#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_SPOTS 10

// DSA: Structure for Min-Heap node
typedef struct {
    int free_time;  // Time when spot becomes free
    int spot_id;    // Spot number
} HeapNode;

// DSA: Smart Parking System structure
typedef struct {
    int spots[MAX_SPOTS];     // Array: -1 = free, else occupied until time
    HeapNode heap[MAX_SPOTS]; // Array-based Min-Heap
    int heap_size;            // Current heap size
    int total_spots;
} ParkingSystem;

// DSA: Initialize parking system
void initParking(ParkingSystem *ps, int n) {
    ps->total_spots = n;
    ps->heap_size = 0;
    for(int i = 0; i < n; i++) {
        ps->spots[i] = -1;  // All spots free initially
    }
}

// DSA: Swap two heap nodes
void swap(HeapNode *a, HeapNode *b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

// DSA: Heapify Up - O(log n)
void heapifyUp(ParkingSystem *ps, int idx) {
    while(idx > 0) {
        int parent = (idx - 1) / 2;
        if(ps->heap[parent].free_time > ps->heap[idx].free_time) {
            swap(&ps->heap[parent], &ps->heap[idx]);
            idx = parent;
        } else {
            break;
        }
    }
}

// DSA: Heapify Down - O(log n)
void heapifyDown(ParkingSystem *ps, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;
    
    if(left < ps->heap_size && ps->heap[left].free_time < ps->heap[smallest].free_time)
        smallest = left;
    if(right < ps->heap_size && ps->heap[right].free_time < ps->heap[smallest].free_time)
        smallest = right;
    
    if(smallest != idx) {
        swap(&ps->heap[idx], &ps->heap[smallest]);
        heapifyDown(ps, smallest);
    }
}

// DSA: Insert into heap - O(log n)
void heapInsert(ParkingSystem *ps, int free_time, int spot_id) {
    if(ps->heap_size >= MAX_SPOTS) return;
    
    ps->heap[ps->heap_size].free_time = free_time;
    ps->heap[ps->heap_size].spot_id = spot_id;
    heapifyUp(ps, ps->heap_size);
    ps->heap_size++;
}

// DSA: Extract minimum from heap - O(log n)
HeapNode extractMin(ParkingSystem *ps) {
    HeapNode min = ps->heap[0];
    ps->heap[0] = ps->heap[ps->heap_size - 1];
    ps->heap_size--;
    heapifyDown(ps, 0);
    return min;
}

// DSA: Peek minimum without removing
HeapNode peekMin(ParkingSystem *ps) {
    return ps->heap[0];
}

// DSA: Park a car - O(log n)
void parkCar(ParkingSystem *ps, int spot_id, int duration, int current_time) {
    int free_time = current_time + duration;
    ps->spots[spot_id] = free_time;
    heapInsert(ps, free_time, spot_id);
    printf("   Spot %d occupied until time %d\n", spot_id, free_time);
}

// DSA: Linear search for free spots - O(n)
void getFreeSpots(ParkingSystem *ps, int *free_spots, int *count) {
    *count = 0;
    for(int i = 0; i < ps->total_spots; i++) {
        if(ps->spots[i] == -1) {
            free_spots[*count] = i;
            (*count)++;
        }
    }
}

// DSA: Clean stale heap entries
void cleanHeap(ParkingSystem *ps) {
    while(ps->heap_size > 0 && ps->spots[ps->heap[0].spot_id] == -1) {
        extractMin(ps);
    }
}

// DSA: Get earliest free spot - O(log n)
int getEarliestFreeSpot(ParkingSystem *ps, int current_time, int *wait_time) {
    cleanHeap(ps);
    
    if(ps->heap_size == 0) {
        *wait_time = 0;
        return -1;
    }
    
    HeapNode earliest = peekMin(ps);
    *wait_time = (earliest.free_time > current_time) ? 
                  (earliest.free_time - current_time) : 0;
    return earliest.spot_id;
}

// DSA: Get waiting times for all spots - O(n)
void getAllWaitingTimes(ParkingSystem *ps, int current_time, int waiting_times[][2], int *count) {
    *count = 0;
    for(int i = 0; i < ps->total_spots; i++) {
        if(ps->spots[i] != -1) {
            int wait = ps->spots[i] - current_time;
            if(wait < 0) wait = 0;
            waiting_times[*count][0] = wait;  // wait time
            waiting_times[*count][1] = i;      // spot id
            (*count)++;
        }
    }
}

// DSA: Bubble sort for waiting times - O(n^2)
void sortByWaitTime(int arr[][2], int n) {
    for(int i = 0; i < n-1; i++) {
        for(int j = 0; j < n-i-1; j++) {
            if(arr[j][0] > arr[j+1][0]) {
                // Swap
                int temp_wait = arr[j][0];
                int temp_spot = arr[j][1];
                arr[j][0] = arr[j+1][0];
                arr[j][1] = arr[j+1][1];
                arr[j+1][0] = temp_wait;
                arr[j+1][1] = temp_spot;
            }
        }
    }
}

// DSA: Suggest best spot - O(n log n)
void suggestBestSpot(ParkingSystem *ps, int current_time) {
    int free_spots[MAX_SPOTS];
    int free_count;
    
    getFreeSpots(ps, free_spots, &free_count);
    
    // If free spots available
    if(free_count > 0) {
        printf("\n✅ FREE SPOTS AVAILABLE:\n");
        for(int i = 0; i < free_count; i++) {
            printf("   Spot %d is free NOW!\n", free_spots[i]);
        }
        printf("🎯 Best choice: Park at Spot %d immediately\n", free_spots[0]);
        return;
    }
    
    // No free spots - show all waiting times
    printf("\n🚫 ALL SPOTS FULL!\n");
    printf("📊 Waiting times for each spot:\n");
    
    int waiting_times[MAX_SPOTS][2];
    int count;
    getAllWaitingTimes(ps, current_time, waiting_times, &count);
    
    // Sort by wait time
    sortByWaitTime(waiting_times, count);
    
    // Display sorted waiting times
    for(int i = 0; i < count; i++) {
        printf("   Spot %d: %d min wait\n", waiting_times[i][1], waiting_times[i][0]);
    }
    
    // Best choice
    printf("\n🎯 BEST CHOICE: Spot %d (wait %d minutes)\n", 
           waiting_times[0][1], waiting_times[0][0]);
}

// Main function to demonstrate
int main() {
    ParkingSystem ps;
    initParking(&ps, 5);
    
    int current_time = 0;
    
    printf("=== SMART PARKING SYSTEM (DSA in C) ===\n\n");
    
    // Park some cars
    printf("Initial parking:\n");
    parkCar(&ps, 0, 30, current_time);  // Spot 0 free at 30
    parkCar(&ps, 1, 15, current_time);  // Spot 1 free at 15
    parkCar(&ps, 2, 45, current_time);  // Spot 2 free at 45
    parkCar(&ps, 3, 10, current_time);  // Spot 3 free at 10
    
    // New driver arrives at time 0
    printf("\n=== NEW DRIVER ARRIVES (Time = %d) ===\n", current_time);
    suggestBestSpot(&ps, current_time);
    
    // Demonstrate getting earliest free spot
    printf("\n=== EARLIEST FREE SPOT DETAILS ===\n");
    int wait_time;
    int earliest_spot = getEarliestFreeSpot(&ps, current_time, &wait_time);
    if(earliest_spot != -1) {
        printf("Spot %d will be free in %d minutes\n", earliest_spot, wait_time);
    }
    
    // Demonstrate spot release (car leaves early)
    printf("\n=== CAR LEAVES SPOT 1 EARLY ===\n");
    ps.spots[1] = -1;  // Manually free spot 1
    printf("Spot 1 is now FREE!\n");
    
    // Check updated status
    printf("\n=== AFTER SPOT 1 FREED ===\n");
    suggestBestSpot(&ps, current_time);
    
    return 0;
}