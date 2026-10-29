import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
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

    public static void main(String[] args) {
        ArrayList<Expense> myExpensesList = new ArrayList<>();
        Scanner scan = new Scanner(System.in);

        boolean keepRunning = true;

        System.out.println("==================================");
        System.out.println("     CLI Expense Tracker v1.2");
        System.out.println("==================================");

        while (keepRunning) {
            System.out.println("\nSelect an option:");
            System.out.println("1. Add Expense");
            System.out.println("2. List Expenses");
            System.out.println("3. View Total Sum");
            System.out.println("4. Filter Expenses by Amount");
            System.out.println("5. Exit & Save to CSV");
            System.out.println("6. Exit");
            System.out.print("\nEnter choice (1-6): ");

            String userChoice = scan.nextLine();

            if (userChoice.equals("1")) {
                System.out.print("Enter expense description: ");
                String desc = scan.nextLine();

                System.out.print("Enter amount ($): ");
                String amtStr = scan.nextLine();

                try {
                    double amt = Double.parseDouble(amtStr);
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
                        System.out.println((i + 1) + ". " + temp.description + " - $" + temp.amount);
                    }
                }

            } else if (userChoice.equals("3")) {
                double sumTotal = 0.0;

                for(int i=0; i<myExpensesList.size(); i++) {
                    sumTotal += myExpensesList.get(i).amount;
                }

                System.out.println("\nTotal Expenses: $" + sumTotal);

            } else if (userChoice.equals("4")) {
                System.out.print("Show expenses greater than: $");
                String thresholdStr = scan.nextLine();

                try {
                    double limit = Double.parseDouble(thresholdStr);
                    System.out.println("\nExpenses greater than $" + limit + ":");

                    boolean foundAnything = false;

                    for (int i = 0; i < myExpensesList.size(); i++) {
                        Expense e = myExpensesList.get(i);
                        if (e.amount > limit) {
                            System.out.println((i + 1) + ". " + e.description + " - $" + e.amount);
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
                try {
                    FileWriter csvWriter = new FileWriter("expenses.csv");
                    csvWriter.write("description,amount\n");

                    for (int i = 0; i < myExpensesList.size(); i++) {
                        Expense currentExp = myExpensesList.get(i);
                        csvWriter.write(currentExp.description + "," + currentExp.amount + "\n");
                    }

                    csvWriter.close();
                    System.out.println("Saved stuff to expenses.csv successfully!");
                    System.out.println("Exiting program. Bye!");
                    keepRunning = false;

                } catch (IOException ex) {
                    System.out.println("Error saving file! Permissions issue?");
                }

            } else if (userChoice.equals("6")) {
                System.out.println("Exiting program without saving. See ya!");
                keepRunning = false;

            } else {
                System.out.println("Wait, what? That's not a valid option. Try 1-6.");
            }
        }

        scan.close();
    }
}
