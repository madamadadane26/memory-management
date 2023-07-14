

#include <iostream>
#include <ctime>
#include <vector>
#include "memory.h"  // Import from the header file

#define MEMORY_SIZE_KB 256
#define UNIT_SIZE_KB 2
#define FIRST_FIT_STRATEGY 0
#define BEST_FIT_STRATEGY 1
using namespace std;

MemoryManager::MemoryManager() {
    mem = nullptr;
    allocation_strategy = FIRST_FIT_STRATEGY;
    totalUnitsInMemory = MEMORY_SIZE_KB / UNIT_SIZE_KB;
    allocatedUnits = 0; // Initialize allocatedUnits to 0
}

MemoryManager::~MemoryManager() {
    freeMem();
}

void MemoryManager::setAllocationStrategy(int strategy) {
    // Check for valid input
    if (strategy != FIRST_FIT_STRATEGY && strategy != BEST_FIT_STRATEGY) {
        cout << "Invalid strategy. Please enter 0 for First Fit or 1 for Best Fit." << endl;
        return;
    }

    allocation_strategy = strategy;
}

int MemoryManager::allocateMem(int process_id, int num_units) {
    if (num_units > totalUnitsInMemory - allocatedUnits) { // Check if requested memory exceeds available memory
        return -1; // Indicate insufficient memory
    }

    if (mem == nullptr) {
        mem = new MemoryNode();
        mem->process_id = process_id;
        mem->num_units = num_units;
        mem->unitIndex = 0;
        mem->next = nullptr;
        allocatedUnits += num_units; // Update allocatedUnits
        return 0;
    }

    int unitIndex = 0;
    MemoryNode* prev = nullptr;
    MemoryNode* tmp = mem;
    int nodesTraversed = 0;

    if (allocation_strategy == FIRST_FIT_STRATEGY) {
        int foundSpace = 0;
        while (tmp != nullptr && unitIndex < totalUnitsInMemory) {
            nodesTraversed++;
            int startUnitIndex = tmp->unitIndex;
            int prevEndUnitIndex = prev == nullptr ? 0 : (prev->unitIndex + prev->num_units);

            if (startUnitIndex - prevEndUnitIndex >= num_units) {
                foundSpace = 1;
                break;
            }

            unitIndex = tmp->unitIndex + tmp->num_units;
            prev = tmp;
            tmp = tmp->next;
        }

        if (!foundSpace) {
            return -1; // Indicate insufficient memory
        }

        MemoryNode* node = new MemoryNode();
        node->process_id = process_id;
        node->num_units = num_units;

        if (prev == nullptr) {
            node->unitIndex = 0;
            node->next = mem;
            mem = node;
        } else {
            node->unitIndex = prev->unitIndex + prev->num_units;
            node->next = tmp;
            prev->next = node;
        }

        allocatedUnits += num_units; // Update allocatedUnits
        return nodesTraversed;
    } else {
        int foundSpace = 0;
        int minSpaceRequired = totalUnitsInMemory;
        MemoryNode* linkedNode = nullptr;

        while (tmp != nullptr && unitIndex < totalUnitsInMemory) {
            nodesTraversed++;
            int startUnitIndex = tmp->unitIndex;
            int prevEndUnitIndex = prev == nullptr ? 0 : (prev->unitIndex + prev->num_units);

            if (startUnitIndex - prevEndUnitIndex >= num_units && startUnitIndex - prevEndUnitIndex < minSpaceRequired) {
                linkedNode = prev;
                minSpaceRequired = startUnitIndex - prevEndUnitIndex;
            }

            unitIndex = tmp->unitIndex + tmp->num_units;
            prev = tmp;
            tmp = tmp->next;
        }

        if (linkedNode == nullptr) {
            if (totalUnitsInMemory - prev->unitIndex - prev->num_units < num_units) {
                return -1; // Indicate insufficient memory
            } else if (totalUnitsInMemory - prev->unitIndex - prev->num_units < minSpaceRequired) {
                linkedNode = prev;
                minSpaceRequired = totalUnitsInMemory - prev->unitIndex - prev->num_units;
            }
        }

        MemoryNode* node = new MemoryNode();
        node->process_id = process_id;
        node->num_units = num_units;

        if (prev == nullptr) {
            node->unitIndex = 0;
            node->next = mem;
            mem = node;
        } else {
            node->unitIndex = linkedNode->unitIndex + linkedNode->num_units;
            node->next = linkedNode->next;
            linkedNode->next = node;
        }

        allocatedUnits += num_units; // Update allocatedUnits
        return nodesTraversed;
    }
}

int MemoryManager::deallocateMem(int process_id) {
    MemoryNode* tmp = mem;
    MemoryNode* prev = nullptr;

    while (tmp != nullptr) {
        if (tmp->process_id == process_id) {
            if (prev == nullptr) {
                mem = tmp->next;
            } else {
                prev->next = tmp->next;
            }

            allocatedUnits -= tmp->num_units; // Update allocatedUnits
            delete tmp;
            return 1; // Indicate successful deallocation
        }

        prev = tmp;
        tmp = tmp->next;
    }

    return -1; // Indicate process_id not found
}

int MemoryManager::fragmentCount() {
    MemoryNode* tmp = mem;
    int fragmentCount = 0;
    int prevEndUnitIndex = 0;

    while (tmp != nullptr) {
        if (tmp->next != nullptr) {
            fragmentCount += tmp->next->unitIndex - (tmp->unitIndex + tmp->num_units);
        }

        prevEndUnitIndex = tmp->unitIndex + tmp->num_units;
        tmp = tmp->next;
    }

    fragmentCount += totalUnitsInMemory - prevEndUnitIndex;
    return fragmentCount;
}

void MemoryManager::freeMem() {
    MemoryNode* tmp = mem;

    while (tmp != nullptr) {
        MemoryNode* next = tmp->next;
        delete tmp;
        tmp = next;
    }

    mem = nullptr;
    allocatedUnits = 0; // Reset allocatedUnits
}

void MemoryManager::printStats(double avgExtFragmentsEachReq, double avgNodesTransversed, double percentageDenied) {
    if (allocation_strategy == FIRST_FIT_STRATEGY) {
        cout << "End of First Fit Allocation" << endl;
    } else {
        cout << "End of Best Fit Allocation" << endl;
    }
    cout << "Average External Fragments Each Request: " << avgExtFragmentsEachReq << endl;
    cout << "Average Nodes Transversed Each Allocation: " << avgNodesTransversed << endl;
    cout << "Percentage Allocation Requests Denied Overall: " << percentageDenied * 100 << "%" << endl;
}

int MemoryManager::getTotalUnitsInMemory() {
    return totalUnitsInMemory;
}

int MemoryManager::getAllocatedUnits() {
    return allocatedUnits;
}

int main() {
    srand(static_cast<unsigned int>(time(NULL)));

    vector<MemAction*> randomActions;

    // Create 10000 requests
    for (int i = 0; i < 10000; i++) {
        MemAction* newAction = new MemAction();
        newAction->type = (i < 5000) ? 0 : 1;
        newAction->process_id = i % 5000;
        newAction->num_units = (i < 5000) ? 3 + rand() % (10 - 3 + 1) : 0;
        newAction->next = nullptr;
        randomActions.push_back(newAction);
    }

    MemoryManager memManager;

    // Run simulation with both strategies and print stats
    for (int i = 0; i < 2; i++) {
        memManager.setAllocationStrategy(i);
        memManager.freeMem();
        
        double avgExtFragmentsEachReq = 0;
        double avgNodesTransversed = 0;
        double percentageDenied = 0;
        int allocationRequests = 0;

        for (size_t j = 0; j < randomActions.size(); j++) {
            MemAction* action = randomActions[j];

            if (action->type == 0) {
    allocationRequests++;
    int nt = memManager.allocateMem(action->process_id, action->num_units);

    if (nt == -1) {
        percentageDenied++;
    } else {
        avgNodesTransversed += nt;
        avgExtFragmentsEachReq += memManager.fragmentCount() - 1;
    }
    avgNodesTransversed++; // Increment for each allocation request (successful or denied)
            } else {
                memManager.deallocateMem(action->process_id);
            }
        }

        double avgFragments = avgExtFragmentsEachReq / allocationRequests;
        double avgTransversed = avgNodesTransversed / allocationRequests;
        double percentage = percentageDenied / allocationRequests;

        memManager.printStats(avgFragments, avgTransversed, percentage);
        
        
    }

    // Clean up memory
    for (MemAction* action : randomActions) {
        delete action;
    }

    return 0;
}
