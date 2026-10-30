import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class Main {

    static class Expense {
        String description;
        double amount;

        Expense(String desc, double val) {
            description = desc;
            amount = val;
        }
    }

    private static String escapeCsv(String value) {
        if (value == null) {
            return "";
        }
        if (value.contains(",") || value.contains("\"") || value.contains("\n")) {
            return "\"" + value.replace("\"", "\"\"") + "\"";
        }
        return value;
    }

    private static void exportToCsv(List<Expense> expenses, String filename) throws IOException {
        try (PrintWriter writer = new PrintWriter(new BufferedWriter(new FileWriter(filename)))) {
            writer.println("description,amount");
            for (Expense exp : expenses) {
                writer.printf("%s,%.2f%n", escapeCsv(exp.description), exp.amount);
            }
        }
    }

    public static void main(String[] args) {
        ArrayList<Expense> myExpensesList = new ArrayList<>();
        Scanner scan = new Scanner(System.in);

        boolean keepRunning = true;

        System.out.println("==================================");
        System.out.println("     CLI Expense Tracker v1.3");
        System.out.println("==================================");

        while (keepRunning) {
            System.out.println("\nSelect an option:");
            System.out.println("1. Add Expense");
            System.out.println("2. List Expenses");
            System.out.println("3. View Total Sum");
            System.out.println("4. Filter Expenses by Amount");
            System.out.println("5. Edit Expense");
            System.out.println("6. Delete Expense");
            System.out.println("7. Exit & Save to CSV");
            System.out.println("8. Exit");
            System.out.print("\nEnter choice (1-8): ");

            String userChoice = scan.nextLine().trim();

            if (userChoice.equals("1")) {
                System.out.print("Enter expense description: ");
                String desc = scan.nextLine().trim();
                if (desc.isEmpty()) {
                    System.out.println("Description cannot be empty. Aborting.");
                    continue;
                }

                System.out.print("Enter amount ($): ");
                String amtStr = scan.nextLine().trim();

                try {
                    double amt = Double.parseDouble(amtStr);
                    if (amt < 0) {
                        System.out.println("Amount cannot be negative. Try again.");
                        continue;
                    }
                    myExpensesList.add(new Expense(desc, amt));
                    System.out.println("Expense added successfully!");
                } catch (NumberFormatException ex) {
                    System.out.println("Bruh, that's not a valid number. Try again.");
                }

            } else if (userChoice.equals("2")) {
                System.out.println("\n--- All Expenses ---");

                if (myExpensesList.isEmpty()) {
                    System.out.println("No expenses recorded yet.");
                } else {
                    for (int i = 0; i < myExpensesList.size(); i++) {
                        Expense temp = myExpensesList.get(i);
                        System.out.printf("%d. %s - $%.2f%n", (i + 1), temp.description, temp.amount);
                    }
                }

            } else if (userChoice.equals("3")) {
                double sumTotal = 0.0;

                for (int i = 0; i < myExpensesList.size(); i++) {
                    sumTotal += myExpensesList.get(i).amount;
                }

                System.out.printf("\nTotal Expenses: $%.2f%n", sumTotal);

            } else if (userChoice.equals("4")) {
                System.out.print("Show expenses greater than: $");
                String thresholdStr = scan.nextLine().trim();

                try {
                    double limit = Double.parseDouble(thresholdStr);
                    System.out.printf("\nExpenses greater than $%.2f:%n", limit);

                    boolean foundAnything = false;
                    int count = 1;

                    for (int i = 0; i < myExpensesList.size(); i++) {
                        Expense e = myExpensesList.get(i);
                        if (e.amount > limit) {
                            System.out.printf("%d. %s - $%.2f%n", count++, e.description, e.amount);
                            foundAnything = true;
                        }
                    }

                    if (!foundAnything) {
                        System.out.println("Hmm, nothing found above that amount.");
                    }
                } catch (NumberFormatException e) {
                    System.out.println("Invalid number format entered.");
                }

            } else if (userChoice.equals("5")) {
                System.out.println("\n--- Edit Expense ---");
                if (myExpensesList.isEmpty()) {
                    System.out.println("No expenses to edit.");
                    continue;
                }

                for (int i = 0; i < myExpensesList.size(); i++) {
                    Expense temp = myExpensesList.get(i);
                    System.out.printf("%d. %s - $%.2f%n", (i + 1), temp.description, temp.amount);
                }

                System.out.print("Enter expense number to edit (1-" + myExpensesList.size() + "): ");
                String editNumStr = scan.nextLine().trim();

                try {
                    int index = Integer.parseInt(editNumStr) - 1;
                    if (index < 0 || index >= myExpensesList.size()) {
                        System.out.println("Invalid expense number.");
                        continue;
                    }

                    Expense target = myExpensesList.get(index);

                    System.out.print("Enter new description (press Enter to keep '" + target.description + "'): ");
                    String newDesc = scan.nextLine().trim();
                    if (!newDesc.isEmpty()) {
                        target.description = newDesc;
                    }

                    System.out.print("Enter new amount (press Enter to keep '$" + String.format("%.2f", target.amount) + "'): ");
                    String newAmtStr = scan.nextLine().trim();
                    if (!newAmtStr.isEmpty()) {
                        double newAmt = Double.parseDouble(newAmtStr);
                        if (newAmt < 0) {
                            System.out.println("Amount cannot be negative. Update cancelled.");
                            continue;
                        }
                        target.amount = newAmt;
                    }

                    System.out.println("Expense updated successfully!");
                } catch (NumberFormatException ex) {
                    System.out.println("Invalid number entered.");
                }

            } else if (userChoice.equals("6")) {
                System.out.println("\n--- Delete Expense ---");
                if (myExpensesList.isEmpty()) {
                    System.out.println("No expenses to delete.");
                    continue;
                }

                for (int i = 0; i < myExpensesList.size(); i++) {
                    Expense temp = myExpensesList.get(i);
                    System.out.printf("%d. %s - $%.2f%n", (i + 1), temp.description, temp.amount);
                }

                System.out.print("Enter expense number to delete (1-" + myExpensesList.size() + "): ");
                String delNumStr = scan.nextLine().trim();

                try {
                    int index = Integer.parseInt(delNumStr) - 1;
                    if (index < 0 || index >= myExpensesList.size()) {
                        System.out.println("Invalid expense number.");
                        continue;
                    }

                    Expense removed = myExpensesList.remove(index);
                    System.out.println("Deleted expense: " + removed.description + " ($" + String.format("%.2f", removed.amount) + ")");
                } catch (NumberFormatException ex) {
                    System.out.println("Invalid number entered.");
                }

            } else if (userChoice.equals("7")) {
                try {
                    exportToCsv(myExpensesList, "expenses.csv");
                    System.out.println("Saved " + myExpensesList.size() + " expense(s) to expenses.csv successfully!");
                    System.out.println("Exiting program. Bye!");
                    keepRunning = false;
                } catch (IOException ex) {
                    System.out.println("Error saving file: " + ex.getMessage());
                }

            } else if (userChoice.equals("8")) {
                System.out.println("Exiting program without saving. See ya!");
                keepRunning = false;

            } else {
                System.out.println("Wait, what? That's not a valid option. Try 1-8.");
            }
        }

        scan.close();
    }
}
