# memory-management
/*Authors:
Yan Ho[RedID: 826654745]
Yan Ho Edoras: cssc1429
Jenny Nguyen[RedID: 826622622]
Jenny Nguyen Edoras: cssc1451

README.txt

Files:
- README.txt
- sim.cpp
- memory.h
- makefile

Code Summary:
Our program, named "sim", is a memory management simulator written in C++. It supports both first fit and best fit memory allocation strategies. The program creates 10000 random requests for memory allocation and deallocation, then simulates the processing of these requests according to the chosen memory allocation strategy. The program prints out the statistics such as average external fragmentation, average number of nodes transversed and the percentage of denied requests for each strategy.

Compile Instructions:
- The command to compile the program in terminal:
- g++ sim.cpp -o sim -std=c++11
- This command will create an executable named sim
- Other method would be to type make in terminal

Operating Instructions:
- After compiling, you can run the program by typing ./sim.
- The program will run the simulation for both first fit and best fit strategies, printing out the statistics for each.

Code Details:
- The MemoryManager class handles the allocation and deallocation of memory, and contains methods for changing the allocation strategy, freeing memory, and printing statistics.
- Memory is represented as a linked list of MemoryNode structures, with each node representing a block of memory allocated to a process.
- The allocateMem function tries to allocate memory for a process according to the current allocation strategy. If allocation is successful, it returns 0. If not, it returns 1.
- The deallocateMem function tries to deallocate memory from a process. If deallocation is successful, it returns 0. If the process does not exist, it returns 1.
- The fragmentCount function calculates the number of external fragments in the memory.
- The freeMem function deallocates all memory.
- The setAllocationStrategy function sets the current allocation strategy.
- The printStats function prints out statistics such as average external fragments, average nodes transversed, and the percentage of denied requests.

Known Issues:
- As of now, we believe there could be some inaccuracies in the calcuations of the program. 

Lessons Learned:
- We gained a deeper understanding of how memory management and allocation strategies work in a computer system.
- This assignment gave us hands-on experience with simulating memory allocation strategies and computing relevant statistics.
- We learned how to model memory as a linked list and how to implement the first fit and best fit strategies.
- We learned how to create a memory management simulator in C++ and handle the complexities of memory allocation and deallocation.
- We learned about the trade-offs between different memory allocation strategies, and how these can affect system performance.
