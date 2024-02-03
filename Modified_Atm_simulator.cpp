#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// Class representing a Bank Account
class BankAccount {
private:
    string accountNumber;
    int pin;
    double balance;
    vector<string> transactionHistory;

public:
    // Constructor
    BankAccount(string accNumber, int pin) {
        accountNumber = accNumber;
        this->pin = pin;
        balance = 0.0;
    }

    // Getter for account number
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
    void displayTransactionHistory() {
        cout << "Transaction History for Account Number: " << accountNumber << endl;
        for (const auto& transaction : transactionHistory) {
            cout << transaction << endl;
        }
        cout << endl;
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
        file << "Transaction History for Account Number: " << account.getAccountNumber() << endl;
        for (const auto& transaction : account.getTransactionHistory()) {
            file << transaction << endl;
        }
        file << endl;
        file.close();
    } else {
        cout << "Error: Unable to open the transaction history file." << endl;
    }
}

int main() {
    int enteredPIN;
    BankAccount account("", 0);

    cout << "ATM Simulator" << endl;

    // Check if the user has an existing account
    string accountNumber;
    cout << "Enter Account Number: ";
    cin >> accountNumber;

    ifstream file("accounts.txt");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (line == accountNumber) {
                int storedPIN;
                file >> storedPIN;
                account = BankAccount(accountNumber, storedPIN);
                break;
            }
        }
        file.close();
    }

    // If the account is not found, create a new account
    if (account.getAccountNumber() == "") {
        cout << "Account not found. Creating a new account..." << endl;
        account = createAccount();

        // Store the new account details in the file
        ofstream newAccountFile("accounts.txt", ios::app);
        if (newAccountFile.is_open()) {
            newAccountFile << account.getAccountNumber() << endl;
            newAccountFile << account.validatePIN(enteredPIN) << endl;
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
        cin >> enteredPIN;

        if (account.validatePIN(enteredPIN)) {
            cout << "PIN verification successful!" << endl;
            break;
        } else {
            attempts--;
            cout << "Invalid PIN. " << attempts << " attempts remaining." << endl;
        }

        if (attempts == 0) {
            cout << "PIN verification failed. Exiting..." << endl;
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
                cout << "Enter amount to deposit: $";
                cin >> amount;
                account.deposit(amount);
                cout << "Deposit successful!" << endl;
                break;
            case 3:
                cout << "Enter amount to withdraw: $";
                cin >> amount;
                if (account.withdraw(amount)) {
                    cout << "Withdrawal successful!" << endl;
                } else {
                    cout << "Insufficient funds. Withdrawal canceled." << endl;
                }
                break;
            case 4:
                cout << "Enter recipient's account number: ";
                cin >> accountNumber;
                cout << "Enter amount to transfer: $";
                cin >> amount;
                recipient = BankAccount(accountNumber, 0);
                if (account.transfer(recipient, amount)) {
                    cout << "Transfer successful!" << endl;
                } else {
                    cout << "Insufficient funds. Transfer canceled." << endl;
                }
                break;
            case 5:
                cout << "Enter new PIN: ";
                cin >> enteredPIN;
                account.changePIN(enteredPIN);
                cout << "PIN change successful!" << endl;
                break;
            case 6:
                account.displayTransactionHistory();
                break;
            case 7:
                storeTransactionHistory(account);
                cout << "Thank you for using the ATM. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 7);

    return 0;
}
