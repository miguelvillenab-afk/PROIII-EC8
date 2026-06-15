#include <iostream>
#include "expenses.h"

int main() {

    ExpenseList expenses{
            {"2026-06-15", "food", "groceries", 120.50},
            {"2026-06-10", "utilities", "electricity", 45.00},
            {"2026-06-12", "transport", "uber", 15.00},
            {"2026-06-10", "food", "restaurant", 60.00}
    };

    std::cout << "=== Analizador de Gastos Personales ===\n\n";

    auto by_date_then_amount = [](const Expense& a, const Expense& b) {
        return std::tie(a.date, a.amount) < std::tie(b.date, b.amount);
    };
    sort_with(expenses, by_date_then_amount);

    AuditedExporter<SummaryExporter<JsonExporter>> exporter{
        SummaryExporter<JsonExporter>{
            JsonExporter{}
        }
    };

    std::string report = exporter.export_expenses(expenses);
    std::cout << report << "\n";

    return 0;
}
