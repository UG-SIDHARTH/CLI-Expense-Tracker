use std::io::{self, Write};

#[derive(Debug, Clone)]
pub struct Expense {
    pub description: String,
    pub amount: f64,
}

impl Expense {
    pub fn new(description: String, amount: f64) -> Self {
        Self {
            description,
            amount,
        }
    }
}

fn prompt(message: &str) -> String {
    print!("{}", message);
    io::stdout().flush().expect("Failed to flush stdout");

    let mut input = String::new();
    io::stdin()
        .read_line(&mut input)
        .expect("Failed to read line");

    input.trim().to_string()
}

fn add_expense(expenses: &mut Vec<Expense>) {
    println!("\n--- Add New Expense ---");

    let description = prompt("Enter expense description: ");
    if description.is_empty() {
        println!("Description cannot be empty. Aborting.");
        return;
    }

    let amount_input = prompt("Enter amount ($): ");
    match amount_input.parse::<f64>() {
        Ok(amount) if amount >= 0.0 => {
            expenses.push(Expense::new(description, amount));
            println!("Expense added successfully!");
        }
        _ => {
            println!("Invalid amount. Please enter a valid positive number.");
        }
    }
}

fn list_expenses(expenses: &[Expense]) {
    println!("\n--- All Expenses ---");
    if expenses.is_empty() {
        println!("No expenses recorded yet.");
        return;
    }

    for (index, expense) in expenses.iter().enumerate() {
        println!(
            "{}. {} - ${:.2}",
            index + 1,
            expense.description,
            expense.amount
        );
    }
}

fn calculate_total(expenses: &[Expense]) -> f64 {
    expenses.iter().map(|e| e.amount).sum()
}

fn main() {
    let mut expenses: Vec<Expense> = Vec::new();

    println!("==================================");
    println!("     CLI Expense Tracker");
    println!("==================================");

    loop {
        println!("\nSelect an option:");
        println!("1. Add Expense");
        println!("2. List Expenses");
        println!("3. View Total Sum (Issue #1)");
        println!("4. Filter Expenses by Amount (Issue #2)");
        println!("5. Exit & Save to CSV (Issue #3)");
        println!("6. Exit");

        let choice = prompt("\nEnter choice (1-6): ");

        match choice.as_str() {
            "1" => {
                add_expense(&mut expenses);
            }
            "2" => {
                list_expenses(&expenses);
            }
            "3" => {
                println!("\nTotal Expenses: ${:.2}", calculate_total(&expenses));
            }
            "4" => {
                // TODO: Issue #2 - Implement a filter to show only expenses over a certain amount.
                println!("\n[Issue #2 Not Implemented]: Filter and display expenses exceeding a specific amount threshold.");
            }
            "5" => {
                // TODO: Issue #3 - Add basic file I/O to save the expense list to a .csv file before the program exits.
                println!("\n[Issue #3 Not Implemented]: Save expenses to `expenses.csv` before exiting.");
                println!("Exiting program. Goodbye!");
                break;
            }
            "6" => {
                println!("Exiting program without saving. Goodbye!");
                break;
            }
            _ => {
                println!("Invalid option. Please enter a number between 1 and 6.");
            }
        }
    }
}
