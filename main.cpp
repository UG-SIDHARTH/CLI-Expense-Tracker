#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>

struct Expense {
    std::string description;
    double amount;

    Expense(const std::string& desc, double amt)
        : description(desc), amount(amt) {}
};

static std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

static std::string readLine(std::istream& in = std::cin) {
    std::string line;
    if (std::getline(in, line)) {
        return trim(line);
    }
    return "";
}

static std::string escapeCsv(const std::string& value) {
    if (value.empty()) {
        return "";
    }
    if (value.find(',') != std::string::npos || 
        value.find('"') != std::string::npos || 
        value.find('\n') != std::string::npos) {
        std::string escaped = "\"";
        for (char ch : value) {
            if (ch == '"') {
                escaped += "\"\"";
            } else {
                escaped += ch;
            }
        }
        escaped += "\"";
        return escaped;
    }
    return value;
}

static void exportToCsv(const std::vector<Expense>& expenses, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    file << "description,amount\n";
    for (const auto& exp : expenses) {
        file << escapeCsv(exp.description) << "," 
             << std::fixed << std::setprecision(2) << exp.amount << "\n";
    }
}

int main() {
    std::vector<Expense> myExpensesList;
    bool keepRunning = true;

    std::cout << "==================================\n";
    std::cout << "     CLI Expense Tracker (C++)\n";
    std::cout << "==================================\n";

    while (keepRunning) {
        std::cout << "\nSelect an option:\n";
        std::cout << "1. Add Expense\n";
        std::cout << "2. List Expenses\n";
        std::cout << "3. View Total Sum\n";
        std::cout << "4. Filter Expenses by Amount\n";
        std::cout << "5. Edit Expense\n";
        std::cout << "6. Delete Expense\n";
        std::cout << "7. Exit & Save to CSV\n";
        std::cout << "8. Exit\n";
        std::cout << "\nEnter choice (1-8): ";

        std::string userChoice = readLine();

        if (userChoice == "1") {
            std::cout << "Enter expense description: ";
            std::string desc = readLine();
            if (desc.empty()) {
                std::cout << "Description cannot be empty. Aborting.\n";
                continue;
            }

            std::cout << "Enter amount ($): ";
            std::string amtStr = readLine();

            try {
                size_t idx = 0;
                double amt = std::stod(amtStr, &idx);
                if (idx != amtStr.length() || amt < 0.0) {
                    std::cout << "Invalid amount. Please enter a valid non-negative number.\n";
                    continue;
                }
                myExpensesList.emplace_back(desc, amt);
                std::cout << "Expense added successfully!\n";
            } catch (const std::exception&) {
                std::cout << "Bruh, that's not a valid number. Try again.\n";
            }

        } else if (userChoice == "2") {
            std::cout << "\n--- All Expenses ---\n";
            if (myExpensesList.empty()) {
                std::cout << "No expenses recorded yet.\n";
            } else {
                for (size_t i = 0; i < myExpensesList.size(); ++i) {
                    std::cout << (i + 1) << ". " << myExpensesList[i].description 
                              << " - $" << std::fixed << std::setprecision(2) 
                              << myExpensesList[i].amount << "\n";
                }
            }

        } else if (userChoice == "3") {
            double sumTotal = 0.0;
            for (const auto& exp : myExpensesList) {
                sumTotal += exp.amount;
            }
            std::cout << "\nTotal Expenses: $" << std::fixed << std::setprecision(2) 
                      << sumTotal << "\n";

        } else if (userChoice == "4") {
            std::cout << "Show expenses greater than: $";
            std::string thresholdStr = readLine();

            try {
                size_t idx = 0;
                double limit = std::stod(thresholdStr, &idx);
                if (idx != thresholdStr.length()) {
                    std::cout << "Invalid number format entered.\n";
                    continue;
                }

                std::cout << "\nExpenses greater than $" << std::fixed << std::setprecision(2) 
                          << limit << ":\n";

                bool foundAnything = false;
                size_t count = 1;
                for (const auto& exp : myExpensesList) {
                    if (exp.amount > limit) {
                        std::cout << count++ << ". " << exp.description 
                                  << " - $" << std::fixed << std::setprecision(2) 
                                  << exp.amount << "\n";
                        foundAnything = true;
                    }
                }

                if (!foundAnything) {
                    std::cout << "Hmm, nothing found above that amount.\n";
                }
            } catch (const std::exception&) {
                std::cout << "Invalid number format entered.\n";
            }

        } else if (userChoice == "5") {
            std::cout << "\n--- Edit Expense ---\n";
            if (myExpensesList.empty()) {
                std::cout << "No expenses to edit.\n";
                continue;
            }

            for (size_t i = 0; i < myExpensesList.size(); ++i) {
                std::cout << (i + 1) << ". " << myExpensesList[i].description 
                          << " - $" << std::fixed << std::setprecision(2) 
                          << myExpensesList[i].amount << "\n";
            }

            std::cout << "Enter expense number to edit (1-" << myExpensesList.size() << "): ";
            std::string editNumStr = readLine();

            try {
                size_t idx = 0;
                int choice = std::stoi(editNumStr, &idx);
                if (idx != editNumStr.length() || choice < 1 || choice > static_cast<int>(myExpensesList.size())) {
                    std::cout << "Invalid expense number.\n";
                    continue;
                }

                Expense& target = myExpensesList[choice - 1];

                std::cout << "Enter new description (press Enter to keep '" << target.description << "'): ";
                std::string newDesc = readLine();
                if (!newDesc.empty()) {
                    target.description = newDesc;
                }

                std::ostringstream ss;
                ss << std::fixed << std::setprecision(2) << target.amount;
                std::cout << "Enter new amount (press Enter to keep '$" << ss.str() << "'): ";
                std::string newAmtStr = readLine();
                if (!newAmtStr.empty()) {
                    size_t amtIdx = 0;
                    double newAmt = std::stod(newAmtStr, &amtIdx);
                    if (amtIdx != newAmtStr.length() || newAmt < 0.0) {
                        std::cout << "Amount cannot be negative or malformed. Update cancelled.\n";
                        continue;
                    }
                    target.amount = newAmt;
                }

                std::cout << "Expense updated successfully!\n";
            } catch (const std::exception&) {
                std::cout << "Invalid number entered.\n";
            }

        } else if (userChoice == "6") {
            std::cout << "\n--- Delete Expense ---\n";
            if (myExpensesList.empty()) {
                std::cout << "No expenses to delete.\n";
                continue;
            }

            for (size_t i = 0; i < myExpensesList.size(); ++i) {
                std::cout << (i + 1) << ". " << myExpensesList[i].description 
                          << " - $" << std::fixed << std::setprecision(2) 
                          << myExpensesList[i].amount << "\n";
            }

            std::cout << "Enter expense number to delete (1-" << myExpensesList.size() << "): ";
            std::string delNumStr = readLine();

            try {
                size_t idx = 0;
                int choice = std::stoi(delNumStr, &idx);
                if (idx != delNumStr.length() || choice < 1 || choice > static_cast<int>(myExpensesList.size())) {
                    std::cout << "Invalid expense number.\n";
                    continue;
                }

                Expense removed = myExpensesList[choice - 1];
                myExpensesList.erase(myExpensesList.begin() + (choice - 1));

                std::cout << "Deleted expense: " << removed.description << " ($" 
                          << std::fixed << std::setprecision(2) << removed.amount << ")\n";
            } catch (const std::exception&) {
                std::cout << "Invalid number entered.\n";
            }

        } else if (userChoice == "7") {
            try {
                exportToCsv(myExpensesList, "expenses.csv");
                std::cout << "Saved " << myExpensesList.size() 
                          << " expense(s) to expenses.csv successfully!\n";
                std::cout << "Exiting program. Bye!\n";
                keepRunning = false;
            } catch (const std::exception& ex) {
                std::cout << "Error saving file: " << ex.what() << "\n";
            }

        } else if (userChoice == "8") {
            std::cout << "Exiting program without saving. See ya!\n";
            keepRunning = false;

        } else {
            std::cout << "Wait, what? That's not a valid option. Try 1-8.\n";
        }
    }

    return 0;
}

