/*Authors:
Yan Ho[RedID: 826654745]
Yan Ho Edoras: cssc1429
Jenny Nguyen[RedID: 826622622]
Jenny Nguyen Edoras: cssc1451

Course
- CS 480 Summer 2023 Section 01
- Programming Assignment #3 (a3)
*/

#ifndef MEMORY_H
#define MEMORY_H

#define MEMORY_SIZE_KB 256
#define UNIT_SIZE_KB 2
#define FIRST_FIT_STRATEGY 0
#define BEST_FIT_STRATEGY 1

// Structures moved to header file

struct MemoryNode {
    int process_id;
    int unitIndex;
    int num_units;
    MemoryNode *next;
};

struct MemAction {
    int type; // 0 for memory allocation, 1 for memory deallocation
    int process_id;
    int num_units;
    MemAction *next;
};

class MemoryManager {
    private:
        MemoryNode *mem;
        int allocation_strategy;
        int totalUnitsInMemory;
        int allocatedUnits; // New addition to track allocated units
    public:
        MemoryManager();
        ~MemoryManager();
        int allocateMem(int process_id, int num_units);
        int deallocateMem(int process_id);
        int fragmentCount();
        void freeMem();
        void printStats(double avgExtFragmentsEachReq, double avgNodesTransversed, double percentageDenied);
        void setAllocationStrategy(int strategy);
        int getTotalUnitsInMemory(); // New addition to expose totalUnitsInMemory
        int getAllocatedUnits(); // New addition to expose allocatedUnits
};

#endif
