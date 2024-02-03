using namespace std;

void displayMenu()
{
    cout << "************ MENU ************" << endl;
    cout << "1. Check Balance" << endl;
    cout << "2. Deposit" << endl;
    cout << "3. Withdraw" << endl;
    cout << "4. Transfer" << endl;
    cout << "5. Change PIN" << endl;
    cout << "6. Exit" << endl;
    cout << "*******************************" << endl;
}

// Function to validate PIN
bool validatePIN(int enteredPin, int pin)
{
    return enteredPin == pin;
}

// Function to display the account balance
void checkBalance(double balance)
{
    cout << "Your current balance is: $" << fixed << setprecision(2) << balance << endl;
}

// Function to deposit funds
void deposit(double &balance)
{
    double depositAmount;
    cout << "Enter the amount to deposit: $";
    cin >> depositAmount;

    if (depositAmount < 0)
    {
        cout << "Invalid amount. Deposit failed." << endl;
    }
    else
    {
        balance += depositAmount;
        cout << "Deposit successful. Current balance: $" << fixed << setprecision(2) << balance << endl;
    }
}

// Function to withdraw funds
void withdraw(double &balance)
{
    double withdrawAmount;
    cout << "Enter the amount to withdraw: $";
    cin >> withdrawAmount;

    if (withdrawAmount < 0)
    {
        cout << "Invalid amount. Withdrawal failed." << endl;
    }
    else if (withdrawAmount > balance)
    {
        cout << "Insufficient funds. Withdrawal failed." << endl;
    }
    else
    {
        balance -= withdrawAmount;
        cout << "Withdrawal successful. Current balance: $" << fixed << setprecision(2) << balance << endl;
    }
}

// Function to transfer funds to another account
void transfer(double &balance)
{
    double transferAmount;
    cout << "Enter the amount to transfer: $";
    cin >> transferAmount;

    if (transferAmount < 0)
    {
        cout << "Invalid amount. Transfer failed." << endl;
    }
    else if (transferAmount > balance)
    {
        cout << "Insufficient funds. Transfer failed." << endl;
    }
    else
    {
        string recipientAccount;
        cout << "Enter the recipient's account number: ";
        cin.ignore();
        getline(cin, recipientAccount);

        cout << "Transfer of $" << fixed << setprecision(2) << transferAmount << " to account " << recipientAccount << " successful." << endl;
        balance -= transferAmount;
        cout << "Current balance: $" << fixed << setprecision(2) << balance << endl;
    }
}

// Function to change the PIN
void changePIN(int &pin, bool &pinChanged)
{
    int currentPIN;
    cout << "Enter your current PIN: ";
    cin >> currentPIN;

    if (!validatePIN(currentPIN, pin))
    {
        cout << "Incorrect PIN. PIN change failed." << endl;
        return;
    }

    int newPIN;
    cout << "Enter the new PIN: ";
    cin >> newPIN;

    cout << "PIN changed successfully." << endl;
    pin = newPIN;
    pinChanged = true;
}