#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// Function to check if a phone number contains all known digits
bool verifyPhoneNumber(const char *phoneNumber, const char *knownDigits) {
    int found = 0;
    int knownLength = strlen(knownDigits);
    
    for (int i = 0; i < knownLength; i++) {
        if (strchr(phoneNumber, knownDigits[i]) != NULL) {
            found++;
        }
    }
    
    return (found == knownLength);
}

int main() {
    // Sample contact database (in a real system, this would be from a file or database)
    const char *contacts[] = {
        "1234567890",
        "2345678901",
        "3456789012",
        "4567890123",
        "5678901234",
        "9876543210",
        "5551234567",
        NULL  // Marks end of array
    };
    
    char knownDigits[5]; // Buffer for known digits
    