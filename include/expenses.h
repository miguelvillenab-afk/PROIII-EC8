//
// Created by miguel on 15/06/26.
//

#ifndef EC8_EXPENSES_H
#define EC8_EXPENSES_H
#pragma once
#include <string>
#include <vector>
#include <functional>
#include <concepts>
#include <ranges>
#include <algorithm>
#include <sstream>
using namespace std;

struct Expense {
    string date, category, detail;
    double amount{};
};

using ExpenseList = vector<Expense>;

template <typename T>
concept ExpenseExporter = requires(T exporter, const ExpenseList& expenses)
{
    {exporter.export_expenses(expenses)}->same_as<string>;
};

using ExportFunction = function<string(const ExpenseList&)>;

template <typename M, typename Cmp>
concept SortStrategy =
    ranges::random_access_range<M> && sortable<ranges::iterator_t<M>, Cmp>;

void sort_with(auto& vals, auto cmp)
    requires SortStrategy<decltype(vals), decltype(cmp)> {
    ranges::sort(vals,cmp);
}

struct CsvExporter {
    string export_expenses(const ExpenseList& expenses) const {
        ostringstream oss;
        oss << "date, category, detail" << endl;
        for (const auto& e : expenses) {
            oss << e.date << ", "
                << e.category << ", "
                << e.detail << ", "
                << e.amount << endl;
        }
        return oss.str();
    }
};

struct JsonExporter {
    string export_expenses(const ExpenseList& expenses) const {
        ostringstream oss;
        oss << "[\n";
        for (int i = 0; i < expenses.size(); ++i) {
            const auto& e = expenses[i];
            oss << "  {\n";
            oss << "    \"date\": \"" << e.date << "\",\n";
            oss << "    \"category\": \"" << e.category << "\",\n";
            oss << "    \"detail\": \"" << e.detail << "\",\n";
            oss << "    \"amount\": " << e.amount << "\n";
            oss << "  }" << (i < expenses.size() - 1 ? "," : "") << "\n";
        }
        oss << "]\n";
        return oss.str();
    }
};

struct TextExporter {
    string export_expenses(const ExpenseList& expenses) const {
        ostringstream oss;
        for (const auto& e : expenses) {
            oss << e.date << " - " << e.category << " - " << e.detail << " : " << e.amount << endl;
        }
        return oss.str();
    }
};

template <ExpenseExporter E, typename... Args>
ExportFunction make_exporter(Args&&... args) {
    return [exporter = E(forward<Args>(args)...)](const ExpenseList& expenses) {
        return exporter.export_expenses(expenses);
    };
}

template<ExpenseExporter I>
struct AuditedExporter {
    I inner;
    string export_expenses(const ExpenseList& expenses) const {
        string resultado = inner.export_expenses(expenses);
        resultado += "\nSe han exportado " + to_string(expenses.size()) + " gastos de forma segura\n";
        return resultado;
    }
};

template <ExpenseExporter I>
struct SummaryExporter {
    I inner;
    string export_expenses(const ExpenseList& expenses) const {
        string resultado = inner.export_expenses(expenses);
        double total_amount = 0.0;
        for (const auto& e : expenses) {
            total_amount += e.amount;
        }

        resultado += "\n===== Total: " + to_string(total_amount) + "=====\n";
        return resultado;
    }
};

#endif //EC8_EXPENSES_H
