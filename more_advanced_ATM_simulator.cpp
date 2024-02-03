#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#ifdef _WIN32
#include <conio.h>  // For Windows
#else
#include <termios.h>  // For Unix/Linux
#include <unistd.h>
#endif

using namespace std;

// Class representing a Bank Account
class BankAccount {
private:
    string accountNumber;
    int pin;
    double balance;
    vector<string> transactionHistory;

    // Function to get the current date and time
    


public:
    // Constructor
    BankAccount(string accNumber, int pin) {
        accountNumber = accNumber;
        this->pin = pin;
        balance = 0.0;
    }

    // Function to get the current date and time
    
    string getCurrentDateTime() const {
        time_t now = time(nullptr);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return buffer;
    }

    // Function to store the transaction history with date and time
    // void storeTransactionHistory() const {
    //     ofstream file("transaction_history.txt", ios::app);
    //     if (file.is_open()) {
    //         file << getCurrentDateTime() << " - Transaction for Account Number: " << accountNumber << endl;
    //         // Rest of the transaction details...
    //         file.close();
    //     } else {
    //         cout << "Error: Unable to store transaction history." << endl;
    //     }
    // }

    // Getter for account number
    // string getAccountNumber() {
    //     return accountNumber;
    // }

    string getAccountNumber() const {
        return accountNumber;
    }

    const vector<string>& getTransactionHistory() const {
    return transactionHistory;
}


    // PIN verification
    bool validatePIN(int enteredPIN) {
        return enteredPIN == pin;
    }

    // Balance checking
    double getBalance() {
        return balance;
    }

    // Deposit
    void deposit(double amount) {
        balance += amount;
        recordTransaction("Deposit", amount);
    }

    // Withdrawal
    bool withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            recordTransaction("Withdrawal", -amount);
            return true;
        }
        return false;
    }

    // Transfer
    bool transfer(BankAccount& recipient, double amount) {
        if (amount <= balance) {
            balance -= amount;
            recipient.deposit(amount);
            recordTransaction("Transfer to " + recipient.getAccountNumber(), -amount);
            return true;
        }
        return false;
    }

    // Change PIN
    void changePIN(int newPIN) {
        pin = newPIN;
        recordTransaction("PIN Change", 0.0);
    }

    // Record transaction history
    void recordTransaction(string transactionType, double amount) {
        string transaction = transactionType + " $" + to_string(amount);
        transactionHistory.push_back(transaction);
    }

    // Display transaction history
    // void displayTransactionHistory() {
    //     cout << "Transaction History for Account Number: " << accountNumber << endl;
    //     for (const auto& transaction : transactionHistory) {
    //         cout << transaction << endl;
    //     }
    //     cout << endl;
    // }

    void displayTransactionHistory() const {
        ifstream file("transaction_history.txt");
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                cout << line << endl;
            }
            file.close();
        } else {
            cout << "No transaction history found." << endl;
        }
    }
};

// Function to create a new Bank Account
BankAccount createAccount() {
    string accountNumber;
    int pin;

    cout << "Enter Account Number: ";
    cin >> accountNumber;

    cout << "Enter PIN: ";
    cin >> pin;

    BankAccount newAccount(accountNumber, pin);
    return newAccount;
}

// Function to store transaction history in a file
void storeTransactionHistory(const BankAccount& account) {
    ofstream file("transaction_history.txt", ios::app);
    if (file.is_open()) {
        file << account.getCurrentDateTime() << " - Transaction for Account Number: " << account.getAccountNumber()  << endl;
        // file << "Transaction History for Account Number: " << account.getAccountNumber() << endl;
        for (const auto& transaction : account.getTransactionHistory()) {
            file << transaction << endl;
        }
        file << endl;
        file.close();
    } else {
        cout << "Error: Unable to open the transaction history file." << endl;
    }
}


// Function to securely read the password
string secureInputPassword() {
    string password;
    char ch;
#ifdef _WIN32
    while ((ch = _getch()) != 13) {  // Enter key
        if (ch == '\b') {  // Backspace key
            if (!password.empty()) {
                cout << "\b \b";  // Move cursor back, overwrite character, move cursor back again
                password.pop_back();
            }
        } else {
            password.push_back(ch);
            cout << '*';
        }
    }
#else
    termios oldt;
    tcgetattr(STDIN_FILENO, &oldt);
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    getline(cin, password);
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif
    return password;
}

int main() {
    cout << "=== Welcome to the ATM Simulator ===" << endl;

    // Account login
    string accountNumber;
    int enteredPIN;

    cout << "Enter Account Number: ";
    cin >> accountNumber;

    cout << "Enter PIN: ";
    enteredPIN = stoi(secureInputPassword());  // Securely read and convert password

    // Check if the account exists
    bool accountExists = false;
    ifstream file("accounts.txt");
    if (file.is_open()) {
        string storedAccountNumber;
        int storedPIN;

        while (file >> storedAccountNumber >> storedPIN) {
            if (storedAccountNumber == accountNumber && storedPIN == enteredPIN) {
                accountExists = true;
                break;
            }
        }
        file.close();
    }

    // If the account is not found, create a new account
    BankAccount account(accountNumber, enteredPIN);
    if (!accountExists) {
        cout << "\nAccount not found. Creating a new account..." << endl;
        account = createAccount();

        // Store the new account details in the file
        ofstream newAccountFile("accounts.txt", ios::app);
        if (newAccountFile.is_open()) {
            newAccountFile << account.getAccountNumber() << " " << account.validatePIN(enteredPIN) << endl;
            newAccountFile.close();
        } else {
            cout << "Error: Unable to create a new account." << endl;
            return 0;
        }
    }

    // Validate PIN
    int attempts = 3;
    while (attempts > 0) {
        cout << "Enter PIN: ";
        enteredPIN = stoi(secureInputPassword());  // Securely read and convert password

        if (account.validatePIN(enteredPIN)) {
            cout << "\nPIN verification successful!" << endl;
            break;
        } else {
            attempts--;
            cout << "\nInvalid PIN. " << attempts << " attempts remaining." << endl;
        }

        if (attempts == 0) {
            cout << "\nPIN verification failed. Exiting..." << endl;
            return 0;
        }
    }

    // Menu
    int choice;
    double amount;
    BankAccount recipient("", 0);

    do {
        cout << "\n--- Main Menu ---" << endl;
        cout << "1. Check Balance" << endl;
        cout << "2. Deposit" << endl;
        cout << "3. Withdraw" << endl;
        cout << "4. Transfer" << endl;
        cout << "5. Change PIN" << endl;
        cout << "6. Transaction History" << endl;
        cout << "7. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Current Balance: $" << account.getBalance() << endl;
                break;
            case 2:
                cout << "Enter the amount to deposit: $";
                cin >> amount;
                account.deposit(amount);
                cout << "Deposit successful. New balance: $" << account.getBalance() << endl;
                break;
            case 3:
                cout << "Enter the amount to withdraw: $";
                cin >> amount;
                if (account.withdraw(amount))
                    cout << "Withdrawal successful. New balance: $" << account.getBalance() << endl;
                else
                    cout << "Insufficient balance. Withdrawal failed." << endl;
                break;
            case 4:
                cout << "Enter the recipient's account number: ";
                cin >> accountNumber;
                cout << "Enter the amount to transfer: $";
                cin >> amount;
                recipient = BankAccount(accountNumber, 0);
                if (account.transfer(recipient, amount))
                    cout << "Transfer successful. New balance: $" << account.getBalance() << endl;
                else
                    cout << "Insufficient balance. Transfer failed." << endl;
                break;
            case 5:
                cout << "Enter the new PIN: ";
                cin >> enteredPIN;
                account.changePIN(enteredPIN);
                cout << "PIN change successful!" << endl;
                break;
            case 6:
                account.displayTransactionHistory();
                break;
            case 7:
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        // Store transaction history after each operation
       storeTransactionHistory(account);

    } while (choice != 7);

    return 0;
}
