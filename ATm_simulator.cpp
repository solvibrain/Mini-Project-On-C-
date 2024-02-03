#include <iostream>
#include <iomanip>
using namespace std;
#include "features.h"

// Function to display the main menu


int main()
{
    // PIN verification
    int pin = 1234; // Set your desired PIN
    bool pinChanged = false;
    int enteredPin;
    int attempts = 0;

    while (attempts < 3)
    {
        cout << "Enter your PIN: ";
        cin >> enteredPin;

        if (validatePIN(enteredPin, pin))
        {
            cout << "PIN verification successful!" << endl;
            break;
        }
        else
        {
            cout << "Incorrect PIN. Please try again." << endl;
            attempts++;
        }
    }

    if (attempts >= 3)
    {
        cout << "Too many incorrect PIN attempts. Exiting." << endl;
        return 0;
    }

    // Main program loop
    int choice;
    double balance = 0;

    do
    {
        if (pinChanged)
        {
            cout << "Please re-validate your PIN." << endl;
            pinChanged = false;

            attempts = 0;
            while (attempts < 3)
            {
                cout << "Enter your PIN: ";
                cin >> enteredPin;

                if (validatePIN(enteredPin, pin))
                {
                    cout << "PIN verification successful!" << endl;
                    break;
                }
                else
                {
                    cout << "Incorrect PIN. Please try again." << endl;
                    attempts++;
                }
            }

            if (attempts >= 3)
            {
                cout << "Too many incorrect PIN attempts. Exiting." << endl;
                return 0;
            }
        }

        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: // Check Balance
            checkBalance(balance);
            break;

        case 2: // Deposit
            deposit(balance);
            break;

        case 3: // Withdraw
            withdraw(balance);
            break;

        case 4: // Transfer
            transfer(balance);
            break;

        case 5: // Change PIN
            changePIN(pin, pinChanged);
            break;

        case 6: // Exit
            cout << "Thank you for using the ATM. Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }

        cout << endl;

    } while (choice != 6);

    return 0;
}
