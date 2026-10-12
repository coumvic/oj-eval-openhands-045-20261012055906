#include <cstdio>
#include <iostream>

using namespace std;

#define MAX_REQUESTS 500000

int main() {
    int n;
    scanf("%d", &n);
    
    int current_floor = 0;
    bool direction_up = true;
    int requests[MAX_REQUESTS];
    int request_count = 0;
    
    for (int i = 0; i < n; i++) {
        char operation[20];
        scanf("%s", operation);
        
        if (operation[0] == 'a' && operation[1] == 'c') {
            // action
            if (request_count == 0) {
                continue;
            }
            
            int target_floor = -1;
            
            if (direction_up) {
                // Find nearest floor above current
                int nearest_above = -1;
                for (int j = 0; j < request_count; j++) {
                    if (requests[j] > current_floor) {
                        if (nearest_above == -1 || requests[j] < nearest_above) {
                            nearest_above = requests[j];
                        }
                    }
                }
                
                if (nearest_above != -1) {
                    target_floor = nearest_above;
                } else {
                    // No floors above, reverse direction
                    direction_up = false;
                    // Find nearest floor below current
                    int nearest_below = -1;
                    for (int j = 0; j < request_count; j++) {
                        if (requests[j] < current_floor) {
                            if (nearest_below == -1 || requests[j] > nearest_below) {
                                nearest_below = requests[j];
                            }
                        }
                    }
                    target_floor = nearest_below;
                }
            } else {
                // Find nearest floor below current
                int nearest_below = -1;
                for (int j = 0; j < request_count; j++) {
                    if (requests[j] < current_floor) {
                        if (nearest_below == -1 || requests[j] > nearest_below) {
                            nearest_below = requests[j];
                        }
                    }
                }
                
                if (nearest_below != -1) {
                    target_floor = nearest_below;
                } else {
                    // No floors below, reverse direction
                    direction_up = true;
                    // Find nearest floor above current
                    int nearest_above = -1;
                    for (int j = 0; j < request_count; j++) {
                        if (requests[j] > current_floor) {
                            if (nearest_above == -1 || requests[j] < nearest_above) {
                                nearest_above = requests[j];
                            }
                        }
                    }
                    target_floor = nearest_above;
                }
            }
            
            if (target_floor != -1) {
                current_floor = target_floor;
                // Remove this request
                for (int j = 0; j < request_count; j++) {
                    if (requests[j] == target_floor) {
                        // Shift remaining elements
                        for (int k = j; k < request_count - 1; k++) {
                            requests[k] = requests[k + 1];
                        }
                        request_count--;
                        break;
                    }
                }
            }
        } else if (operation[0] == 'a' && operation[1] == 'd') {
            // add
            int floor;
            scanf("%d", &floor);
            if (request_count < MAX_REQUESTS) {
                requests[request_count++] = floor;
            }
        } else if (operation[0] == 'c') {
            // cancel
            int floor;
            scanf("%d", &floor);
            for (int j = 0; j < request_count; j++) {
                if (requests[j] == floor) {
                    // Shift remaining elements
                    for (int k = j; k < request_count - 1; k++) {
                        requests[k] = requests[k + 1];
                    }
                    request_count--;
                    break;
                }
            }
        } else if (operation[0] == 'l') {
            // locate
            printf("%d\n", current_floor);
        }
    }
    
    return 0;
}