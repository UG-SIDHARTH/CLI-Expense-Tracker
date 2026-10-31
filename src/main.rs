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

fn filter_expenses_by_amount(expenses: &[Expense], threshold: f64) -> Vec<Expense> {
    expenses
        .iter()
        .filter(|e| e.amount > threshold)
        .cloned()
        .collect()
}

fn calculate_total(expenses: &[Expense]) -> f64 {
    expenses.iter().map(|e| e.amount).sum()
}

fn save_expenses_to_csv(expenses: &[Expense], filename: &str) -> std::io::Result<()> {
    let mut wtr = csv::Writer::from_path(filename)?;

    for expense in expenses {
        wtr.write_record(&[&expense.description, &expense.amount.to_string()])?;
    }

    wtr.flush()?;
    Ok(())
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
                let threshold = prompt("\nEnter the amount threshold: ").parse().unwrap_or(0.0);
                let filtered_expenses = filter_expenses_by_amount(&expenses, threshold);
                list_expenses(&filtered_expenses);
            }
            "5" => {
                // TODO: Issue #3 - Add basic file I/O to save the expense list to a .csv file before the program exits.
                if let Err(e) = save_expenses_to_csv(&expenses, "expenses.csv") {
                    eprintln!("Error saving expenses to CSV: {}", e);
                } else {
                    println!("\nExpenses saved to `expenses.csv`.");
                }
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
