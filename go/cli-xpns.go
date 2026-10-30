package main

import "fmt"

type expense struct {
	Description string
	Amount      float64
}

func prompt(message string) string {
	fmt.Print(message)
	var input string
	fmt.Scanln(&input)
	return input
}

func addExpense(expenses *[]expense) {
	description := prompt("Enter expense description: ")
	amountStr := prompt("Enter expense amount: ")
	var amount float64
	fmt.Sscanf(amountStr, "%f", &amount)
	*expenses = append(*expenses, expense{Description: description, Amount: amount})

}

func listExpenses(expenses *[]expense) {
	fmt.Println("\n--- All Expenses ---")
	if len(*expenses) == 0 {
		fmt.Println("No expenses found.")
		return
	} else {
		for i, e := range *expenses {
			fmt.Printf("Expense %d: %s - $%.2f\n", i+1, e.Description, e.Amount)
		}
	}
}

func calculateTotal(expenses *[]expense) {
	var total float64
	for _, e := range *expenses {
		total += e.Amount
	}
	fmt.Printf("\nTotal Expenses: $%.2f\n", total)
}
func filterExpensesByAmount(expenses *[]expense, threshold float64) {
	fmt.Printf("\n--- Expenses Over $%.2f ---\n", threshold)
	var found bool = false
	for i, e := range *expenses {
		if e.Amount > threshold {
			fmt.Printf("Expense %d: %s - $%.2f\n", i+1, e.Description, e.Amount)
			found = true

		}
	}
	if !found {
		fmt.Println("No expenses found over the specified amount.")
	}
}

func main() {
	var expenses []expense
	fmt.Println("===============================")
	fmt.Println("CLI Expense Tracker")
	fmt.Println("===============================")

	var loop bool = true
	for loop {
		fmt.Println("\nSelect an option:")
		fmt.Println("1. Add Expense")
		fmt.Println("2. List Expenses")
		fmt.Println("3. View Total Sum (Issue #1)")
		fmt.Println("4. Filter Expenses by Amount (Issue #2)")
		fmt.Println("5. Exit & Save to CSV (Issue #3)")
		fmt.Println("6. Exit")
		var choice string = prompt("Enter your choice: ")
		switch choice {
		case "1":
			addExpense(&expenses)
		case "2":
			listExpenses(&expenses)
		case "3":
			calculateTotal(&expenses)
		case "4":
			thresholdStr := prompt("Enter the amount threshold: ")
			var threshold float64
			fmt.Sscanf(thresholdStr, "%f", &threshold)
			filterExpensesByAmount(&expenses, threshold)
		case "5":
			// TODO: Implement saving to CSV
			fmt.Println("to be implemented, trust me bro")
		case "6":
			loop = false
		default:
			fmt.Println("Invalid choice. Please try again.")
		}
	}
}
