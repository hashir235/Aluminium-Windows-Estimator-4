#include "FabricatorManager.h"
#include "FabricatorWindowTypes.h"
#include <iostream>
#include <limits>

using namespace std;

// ============================
// Add Window (Fabricator)
// ============================
void addWindowFabricator(vector<unique_ptr<FrameComponent>>& windows,
                         map<int, pair<string, function<unique_ptr<FrameComponent>()>>>& windowTypes) {
    cout << "\n--- Add New Fabricator Window ---\n";

    // Show all available types
    cout << "Select a window type:\n";
    for (const auto& [id, typePair] : windowTypes) {
        cout << id << ": " << typePair.first << "\n";
    }

    int choice;
    cout << "Enter choice: ";
    cin >> choice;

    if (!cin || windowTypes.find(choice) == windowTypes.end()) {
        cout << " Invalid choice!\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    // Create chosen window dynamically
    auto newWin = windowTypes.at(choice).second();

    // Input details
    if (newWin->inputWindowData()) {
        newWin->calculateSections();
        windows.push_back(std::move(newWin));
        cout << " Fabricator Window added successfully!\n";
    } else {
        cout << " Window data input failed. Try again.\n";
    }
}

// ============================
// Edit Window (Fabricator)
// ============================
void editWindowFabricator(vector<unique_ptr<FrameComponent>>& windows) {
    if (windows.empty()) {
        cout << "⚠ No windows to edit!\n";
        return;
    }

    cout << "\n--- Edit Fabricator Window ---\n";
    for (size_t i = 0; i < windows.size(); ++i) {
        cout << i + 1 << ": " << windows[i]->getLabel()
             << " (H=" << windows[i]->getHeight()
             << ", W=" << windows[i]->getWidth() << ")\n";
    }

    cout << "Select window index to edit: ";
    size_t idx;
    cin >> idx;

    if (idx == 0 || idx > windows.size()) {
        cout << " Invalid index!\n";
        return;
    }

    auto& win = windows[idx - 1];
    if (win->inputWindowData(true)) { // true = editing mode
        win->calculateSections();
        cout << " Window updated successfully!\n";
    } else {
        cout << "⚠ Edit cancelled.\n";
    }
}

// ============================
// Delete Window (Fabricator)
// ============================
void deleteWindowFabricator(vector<unique_ptr<FrameComponent>>& windows) {
    if (windows.empty()) {
        cout << "⚠ No windows to delete!\n";
        return;
    }

    cout << "\n--- Delete Fabricator Window ---\n";
    for (size_t i = 0; i < windows.size(); ++i) {
        cout << i + 1 << ": " << windows[i]->getLabel()
             << " (H=" << windows[i]->getHeight()
             << ", W=" << windows[i]->getWidth() << ")\n";
    }

    cout << "Select window index to delete: ";
    size_t idx;
    cin >> idx;

    if (idx == 0 || idx > windows.size()) {
        cout << " Invalid index!\n";
        return;
    }

    windows.erase(windows.begin() + (idx - 1));
    cout << " Window deleted!\n";
}
