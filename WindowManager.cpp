#include "WindowManager.h"
#include "WindowTypes.h"
#include <iostream>
#include <limits>


using namespace std;

// ============================
// Add Window
// ============================
void addWindow(vector<unique_ptr<FrameComponent>>& windows,
               EstimateLengthManager& estimator,
               FinalSummaryManager& summaryManager) {
    cout << "\n--- Add New Window ---\n";

    // Window type selection menu
    cout << "Select a window type:\n";
    for (const auto& [id, typePair] : windowTypes) {
        cout << id << ": " << typePair.first << "\n";
    }

    int choice;
    cout << "Enter choice: ";
    cin >> choice;

    if (!cin || windowTypes.find(choice) == windowTypes.end()) {
        cout << "⚠️ Invalid choice!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    // Create the chosen window dynamically
    auto newWin = windowTypes.at(choice).second();

    // Take user input for dimensions etc.
    if (newWin->inputWindowData()) {
        newWin->calculateSections();
        newWin->sendToEstimator(estimator);
        summaryManager.addWindow(newWin->getLabel(), newWin->getAreaSqFt());


        // Push into main vector
        windows.push_back(std::move(newWin));
        cout << "✅ Window added successfully!\n";
    } else {
        cout << "⚠️ Window data input failed. Try again.\n";
    }
}

// ============================
// Edit Window
// ============================
void editWindow(std::vector<std::unique_ptr<FrameComponent>>& windows,
                EstimateLengthManager& estimator,
                FinalSummaryManager& summaryManager) 
{
    if (windows.empty()) {
        cout << "⚠️ No windows to edit!\n";
        return;
    }

    cout << "\n--- Edit Window ---\n";
    for (size_t i = 0; i < windows.size(); ++i) {
        cout << i + 1 << ": " << windows[i]->getLabel()
             << " (H=" << windows[i]->getHeight()
             << ", W=" << windows[i]->getWidth() << ")\n";
    }

    cout << "Select window index to edit: ";
    size_t idx;
    cin >> idx;

    if (idx == 0 || idx > windows.size()) {
        cout << "❌ Invalid index!\n";
        return;
    }

    auto& win = windows[idx - 1];

    if (win->inputWindowData(true)) {
        win->calculateSections();
        win->sendToEstimator(estimator);
        summaryManager.addWindow(win->getLabel(), win->getAreaSqFt());

        cout << "✅ Window updated!\n";
    }
}

// ============================
// Delete Window
// ============================
void deleteWindow(std::vector<std::unique_ptr<FrameComponent>>& windows,
                  EstimateLengthManager& estimator,
                  FinalSummaryManager& summaryManager) 
{
    if (windows.empty()) {
        cout << "⚠️ No windows to delete!\n";
        return;
    }

    cout << "\n--- Delete Window ---\n";
    for (size_t i = 0; i < windows.size(); ++i) {
        cout << i + 1 << ": " << windows[i]->getLabel()
             << " (H=" << windows[i]->getHeight()
             << ", W=" << windows[i]->getWidth() << ")\n";
    }

    cout << "Select window index to delete: ";
    size_t idx;
    cin >> idx;

    if (idx == 0 || idx > windows.size()) {
        cout << "❌ Invalid index!\n";
        return;
    }

    string label = windows[idx - 1]->getLabel();
    summaryManager.removeWindowSummary(label, windows[idx - 1]->getAreaSqFt()); // remove from summary manager
    estimator.removeWindow(label);                  // remove from estimator
    windows.erase(windows.begin() + (idx - 1));     // remove actual window object

    cout << "✅ Window deleted!\n";
}
