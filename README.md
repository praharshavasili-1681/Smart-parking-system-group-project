Smart Parking System
Group memeber names:
1)V.Praharsha
2)L.Iswarya
# SMART PARKING SYSTEM - Complete Documentation

##  PROBLEM STATEMENT:

imagine you manage a parking lot with 5 parking sports.Drivers keep coming and going throughout the day
.when a new driver arrives they want to know:

1. **Is there any free spot right now?**
2. **If all spots are full, which spot will become free the soonest?**
3. **How long do they need to wait for each spot?**

### Real-World Scenario
- **Spot 0** → Car parked for 30 minutes (free at time 30)
- **Spot 1** → Car parked for 15 minutes (free at time 15)
- **Spot 2** → Car parked for 45 minutes (free at time 45)
- **Spot 3** → Empty
- **Spot 4** → Empty

**New driver arrives at time 0** → System should tell them: "Spot 3 and 4 are free! Park now!"

**If all spots are full** → System should tell: "Spot 1 will be free in 5 minutes (shortest wait)"

### Why This is Challenging?
- Need to **quickly find** which spot becomes free soonest
- Need to **track** when each spot becomes free
- Need to **remove** spots from tracking when cars leave early
- Need to **suggest** the best spot to new drivers

---

##  DATA STRUCTURES USED:

### 1. **Array** (Simple List)
```c
int spots[MAX_SPOTS];  // Tracks each parking spot
```
- **Purpose**: Store status of each parking spot
- **Values**: `-1` = free, otherwise = time when spot becomes free
- **Example**: `spots[0] = 30` means spot 0 is occupied until time 30

### 2. **Min-Heap** (Priority Queue)
```c
typedef struct {
    int free_time;  // When spot becomes free
    int spot_id;    // Which spot number
} HeapNode;

HeapNode heap[MAX_SPOTS];  // The heap array
```
- **Purpose**: Always keep the spot that becomes free soonest at the top
- **Property**: Parent node always has SMALLER free_time than children
- **Visual Example**:
```
        (15, spot1)      ← Smallest time at top
        /         \
   (30, spot0)   (45, spot2)
```

### 3. **Structure** (Custom Data Type)
```c
typedef struct {
    int spots[MAX_SPOTS];
    HeapNode heap[MAX_SPOTS];
    int heap_size;
    int total_spots;
} ParkingSystem;
```
- **Purpose**: Group all parking data together
- **Benefits**: Easy to pass around, clean code organization

---

## ⚙️ ALGORITHM EXPLANATION

### Algorithm 1: **Park a Car** (Insert into Heap)
```
Step 1: Calculate free_time = current_time + duration
Step 2: Mark spot as occupied in array: spots[spot_id] = free_time
Step 3: Add to heap at the end
Step 4: Bubble UP to maintain heap property
```

**Time Complexity**: O(log n) - Fast even with many spots

---

### Algorithm 2: **Find Best Spot** (Main Algorithm)
```
IF any free spots exist:
    List all free spots
    Recommend first free spot
    
ELSE (all spots full):
    Step 1: Get waiting time for each occupied spot
    Step 2: Sort spots by waiting time (smallest first)
    Step 3: Recommend spot with smallest waiting time
```

**Example Walkthrough** (All spots full):
```
Occupied spots:
- Spot 0: free in 30 min
- Spot 1: free in 15 min  
- Spot 2: free in 45 min

Sort by wait time:
1. Spot 1 (15 min) ← BEST CHOICE
2. Spot 0 (30 min)
3. Spot 2 (45 min)

Output: "Park at Spot 1, wait only 15 minutes"
```

---

### Algorithm 3: **Heap Cleanup** (Remove Stale Entries)
```
Problem: When a car leaves early, array shows spot is free
        but heap still has old entry

Solution:
WHILE heap is not empty AND spot at top is actually free:
    Remove top from heap
    Fix heap property (heapify down)
```

**Visual Example**:
```
Before cleanup:
Heap: [(30,spot0), (15,spot1)]  Array: spot0 = -1 (free!)
                                      spot1 = 25 (occupied)

After cleanup:
Heap: [(25,spot1)]  ← Removed stale spot0 entry
```

---

### Algorithm 4: **Heap Insertion** (Bubble Up)
```
Add new node at the end:
Heap: [15, 30, 45, 10]  ← Added 10 at end

Compare with parent (index 1 = 30):
10 < 30 → SWAP
Heap: [15, 10, 45, 30]

Compare with parent (index 0 = 15):
10 < 15 → SWAP  
Heap: [10, 15, 45, 30]  ← Heap property restored!
```

---

### Algorithm 5: **Heap Deletion** (Extract Min)
```
Remove smallest (top):
Original: [10, 15, 45, 30]

Move last element to top:
[30, 15, 45]

Compare with children:
30 > 15 → SWAP with smaller child
[15, 30, 45]

Heap property restored! Smallest (15) is at top
```

---

##  COMPILATION INSTRUCTIONS:

### For Windows (using GCC)
```bash
# Step 1: Save the code as parking.c
# Step 2: Open Command Prompt
# Step 3: Navigate to file location
cd C:\Users\YourName\Desktop

# Step 4: Compile the program
gcc parking.c -o parking.exe

# Step 5: Run the program
parking.exe
```

### For Linux/Mac
```bash
# Step 1: Save the code as parking.c
# Step 2: Open Terminal
# Step 3: Navigate to file location
cd /home/username/Desktop

# Step 4: Compile the program
gcc parking.c -o parking

# Step 5: Run the program
./parking
```

### Alternative: Using Online Compiler
1. Go to **onlinegdb.com** or **replit.com**
2. Create new C program
3. Copy-paste the code
4. Click "Run"

### Troubleshooting Compilation
| Error Message | Solution |
|--------------|----------|
| `gcc: command not found` | Install GCC compiler |
| `parking.c: No such file` | Check file name and location |
| `syntax error` | Check for missing semicolons `;` |

---

## SAMPLE OUTPUT:

### Run 1: With Free Spots Available
```
=== SMART PARKING SYSTEM (DSA in C) ===

Initial parking:
   Spot 0 occupied until time 30
   Spot 1 occupied until time 15
   Spot 2 occupied until time 45
   Spot 3 occupied until time 10

=== NEW DRIVER ARRIVES (Time = 0) ===

✅ FREE SPOTS AVAILABLE:
   Spot 4 is free NOW!
🎯 Best choice: Park at Spot 4 immediately

=== EARLIEST FREE SPOT DETAILS ===
Spot 3 will be free in 10 minutes

=== CAR LEAVES SPOT 1 EARLY ===
Spot 1 is now FREE!

=== AFTER SPOT 1 FREED ===

✅ FREE SPOTS AVAILABLE:
   Spot 1 is free NOW!
   Spot 4 is free NOW!
🎯 Best choice: Park at Spot 1 immediately
```

### Run 2: All Spots Full
```
=== SMART PARKING SYSTEM ===

Initial parking (all spots full):
   Spot 0 occupied until time 30
   Spot 1 occupied until time 15
   Spot 2 occupied until time 45
   Spot 3 occupied until time 20
   Spot 4 occupied until time 25

=== NEW DRIVER ARRIVES (Time = 0) ===

🚫 ALL SPOTS FULL!
📊 Waiting times for each spot:
   Spot 1: 15 min wait
   Spot 3: 20 min wait
   Spot 4: 25 min wait
   Spot 0: 30 min wait
   Spot 2: 45 min wait

🎯 BEST CHOICE: Spot 1 (wait 15 minutes)
```

### Run 3: Time Progression
```
=== TIME ADVANCED to 20 minutes ===
Spot 1 is now FREE!
Spot 3 is now FREE!

=== NEW DRIVER ARRIVES ===

✅ FREE SPOTS AVAILABLE:
   Spot 1 is free NOW!
   Spot 3 is free NOW!
🎯 Best choice: Park at Spot 1 immediately
```

.
