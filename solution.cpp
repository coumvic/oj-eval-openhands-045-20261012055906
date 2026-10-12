#include <cstdio>

using namespace std;

const int MAX_N = 500000;

class ElevatorSystem {
private:
    int current_floor;
    bool direction_up;  // true for up, false for down
    int up_heap[MAX_N];    // min-heap for floors above current
    int down_heap[MAX_N];  // max-heap for floors below current
    int up_size, down_size;

    // Simple swap function
    void swap_int(int &a, int &b) {
        int temp = a;
        a = b;
        b = temp;
    }

    // Heap operations for up_heap (min-heap)
    void push_up(int x) {
        up_heap[up_size++] = x;
        int i = up_size - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (up_heap[parent] <= up_heap[i]) break;
            swap_int(up_heap[parent], up_heap[i]);
            i = parent;
        }
    }

    int pop_up() {
        if (up_size == 0) return -1;
        int result = up_heap[0];
        up_heap[0] = up_heap[--up_size];
        
        int i = 0;
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            
            if (left < up_size && up_heap[left] < up_heap[smallest]) {
                smallest = left;
            }
            if (right < up_size && up_heap[right] < up_heap[smallest]) {
                smallest = right;
            }
            
            if (smallest == i) break;
            swap_int(up_heap[i], up_heap[smallest]);
            i = smallest;
        }
        return result;
    }

    // Heap operations for down_heap (max-heap)
    void push_down(int x) {
        down_heap[down_size++] = x;
        int i = down_size - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (down_heap[parent] >= down_heap[i]) break;
            swap_int(down_heap[parent], down_heap[i]);
            i = parent;
        }
    }

    int pop_down() {
        if (down_size == 0) return -1;
        int result = down_heap[0];
        down_heap[0] = down_heap[--down_size];
        
        int i = 0;
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int largest = i;
            
            if (left < down_size && down_heap[left] > down_heap[largest]) {
                largest = left;
            }
            if (right < down_size && down_heap[right] > down_heap[largest]) {
                largest = right;
            }
            
            if (largest == i) break;
            swap_int(down_heap[i], down_heap[largest]);
            i = largest;
        }
        return result;
    }

    // Find and remove a specific value from heap
    bool remove_from_up_heap(int target) {
        for (int i = 0; i < up_size; i++) {
            if (up_heap[i] == target) {
                up_heap[i] = up_heap[--up_size];
                // Re-heapify from position i
                int current = i;
                while (true) {
                    int left = 2 * current + 1;
                    int right = 2 * current + 2;
                    int smallest = current;
                    
                    if (left < up_size && up_heap[left] < up_heap[smallest]) {
                        smallest = left;
                    }
                    if (right < up_size && up_heap[right] < up_heap[smallest]) {
                        smallest = right;
                    }
                    
                    if (smallest == current) break;
                    swap_int(up_heap[current], up_heap[smallest]);
                    current = smallest;
                }
                return true;
            }
        }
        return false;
    }

    bool remove_from_down_heap(int target) {
        for (int i = 0; i < down_size; i++) {
            if (down_heap[i] == target) {
                down_heap[i] = down_heap[--down_size];
                // Re-heapify from position i
                int current = i;
                while (true) {
                    int left = 2 * current + 1;
                    int right = 2 * current + 2;
                    int largest = current;
                    
                    if (left < down_size && down_heap[left] > down_heap[largest]) {
                        largest = left;
                    }
                    if (right < down_size && down_heap[right] > down_heap[largest]) {
                        largest = right;
                    }
                    
                    if (largest == current) break;
                    swap_int(down_heap[current], down_heap[largest]);
                    current = largest;
                }
                return true;
            }
        }
        return false;
    }

public:
    ElevatorSystem() : current_floor(0), direction_up(true), up_size(0), down_size(0) {}

    void add_request(int floor) {
        if (floor > current_floor) {
            push_up(floor);
        } else {
            push_down(floor);
        }
    }

    void cancel_request(int floor) {
        if (floor > current_floor) {
            remove_from_up_heap(floor);
        } else {
            remove_from_down_heap(floor);
        }
    }

    void execute_action() {
        if (up_size == 0 && down_size == 0) {
            return;  // No requests, stay still
        }

        int target_floor = -1;
        
        if (direction_up) {
            // Try to go up first
            if (up_size > 0) {
                target_floor = up_heap[0];
            } else {
                // No requests above, reverse direction
                direction_up = false;
                if (down_size > 0) {
                    target_floor = down_heap[0];
                }
            }
        } else {
            // Try to go down first
            if (down_size > 0) {
                target_floor = down_heap[0];
            } else {
                // No requests below, reverse direction
                direction_up = true;
                if (up_size > 0) {
                    target_floor = up_heap[0];
                }
            }
        }

        if (target_floor != -1) {
            // Remove from appropriate heap before updating current_floor
            if (target_floor > current_floor) {
                pop_up();
            } else {
                pop_down();
            }
            
            current_floor = target_floor;
            
            // Reorganize remaining requests based on new current floor
            int temp_up[MAX_N], temp_down[MAX_N];
            int temp_up_size = 0, temp_down_size = 0;
            
            // Move all remaining requests to temp arrays
            while (up_size > 0) {
                int floor = pop_up();
                if (floor > current_floor) {
                    temp_up[temp_up_size++] = floor;
                } else {
                    temp_down[temp_down_size++] = floor;
                }
            }
            
            while (down_size > 0) {
                int floor = pop_down();
                if (floor > current_floor) {
                    temp_up[temp_up_size++] = floor;
                } else {
                    temp_down[temp_down_size++] = floor;
                }
            }
            
            // Rebuild heaps
            for (int i = 0; i < temp_up_size; i++) {
                push_up(temp_up[i]);
            }
            for (int i = 0; i < temp_down_size; i++) {
                push_down(temp_down[i]);
            }
        }
    }

    int locate() const {
        return current_floor;
    }
};

int main() {
    int n;
    scanf("%d", &n);
    
    ElevatorSystem elevator;
    
    for (int i = 0; i < n; i++) {
        char operation[20];
        scanf("%s", operation);
        
        if (operation[0] == 'a' && operation[1] == 'c') {
            elevator.execute_action();
        } else if (operation[0] == 'a' && operation[1] == 'd') {
            int floor;
            scanf("%d", &floor);
            elevator.add_request(floor);
        } else if (operation[0] == 'c') {
            int floor;
            scanf("%d", &floor);
            elevator.cancel_request(floor);
        } else if (operation[0] == 'l') {
            printf("%d\n", elevator.locate());
        }
    }
    
    return 0;
}