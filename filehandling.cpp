#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void writeToFile(const string& filename) {
    ofstream outFile(filename); // Overwrites the file if exists
    if (!outFile) {
        cerr << "Error opening file for writing.\n";
        return;
    }

    string data;
    cout << "Enter text to write to the file (type 'exit' to stop):\n";
    while (true) {
        getline(cin, data);
        if (data == "exit") break;
        outFile << data << endl;
    }

    outFile.close();
    cout << "Data written to file successfully.\n";
}

void appendToFile(const string& filename) {
    ofstream outFile(filename, ios::app); // Open file in append mode
    if (!outFile) {
        cerr << "Error opening file for appending.\n";
        return;
    }

    string data;
    cout << "Enter text to append to the file (type 'exit' to stop):\n";
    while (true) {
        getline(cin, data);
        if (data == "exit") break;
        outFile << data << endl;
    }

    outFile.close();
    cout << "Data appended successfully.\n";
}

void readFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Error opening file for reading.\n";
        return;
    }

    string line;
    cout << "\n--- File Contents ---\n";
    while (getline(inFile, line)) {
        cout << line << endl;
    }

    inFile.close();
    cout << "--- End of File ---\n";
}

int main() {
    string filename = "data.txt";
    int choice;

    do {
        cout << "\n--- File Management Tool ---\n";
        cout << "1. Write to file (overwrite)\n";
        cout << "2. Append to file\n";
        cout << "3. Read from file\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // Clear newline from input 

        switch (choice) {
            case 1:
                writeToFile(filename);
                break;
            case 2:
                appendToFile(filename);
                break;
            case 3:
                readFromFile(filename);
                break;
            case 4:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 4);

    return 0;
}
