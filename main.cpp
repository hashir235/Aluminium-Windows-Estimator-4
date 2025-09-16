#include "MainMenuFunctions.h"

// 🔹 Global Managers
EstimateLengthManager estimator;
FinalSummaryManager summaryManager;
FinalCostCalculator finalEstimator;
ManualRatesManager ratesManager(estimator.getSummaries()); 
AutoRatesManager autoRatesManager;

int main() {
    SettingsManagerA::getInstance().loadFromFile("section_settings.txt");

    // Store all created windows here
    vector<unique_ptr<FrameComponent>> windows;

    while (true) {
        int mainChoice = -1;

        cout << "\n====== Main Menu ======\n"
             << "1. Clear All Entered Windows\n"
             << "2. Estimator\n"
             << "3. Fabricator\n"
             << "0. Exit\n"
             << "Select Option: ";

        if (!(cin >> mainChoice)) {
            cout << "❌ Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (mainChoice) {
            case 1: {
                std::cout << "\n⚠ WARNING: This will remove all entered windows data permanently!\n";
                std::cout << "   Are you sure you want to refresh? (y/n): ";
                std::string confirm;
                std::cin >> confirm;

                if (!confirm.empty() && (confirm == "y" || confirm == "Y")) {
                    windows.clear();
                    estimator.clearAllData();  // ✅ custom function

                    // Reset all window counters
                    S_Win::resetWindowCount();
                    SM_Win::resetWindowCount();
                    SG_Win::resetWindowCount();
                    SGM_Win::resetWindowCount();
                    SC_Win::resetWindowCount();
                    SCM_Win::resetWindowCount();
                    F_Win::resetWindowCount();
                    FC_Win::resetWindowCount();
                    O_Win::resetWindowCount();
                    D_Win::resetWindowCount();
                    A_Win::resetWindowCount();

                    std::cout << "✅ All window data cleared!\n";
                } else {
                    std::cout << "↩ Refresh cancelled.\n";
                }
                break;
            }

            case 2: { // Estimator submenu
                int estChoice = -1;
                while (true) {
                    cout << "\n====== Estimator Menu ======\n"
                         << "1. Add Window for Estimation\n"
                         << "2. Show Final Summary\n"
                         << "3. Settings\n"
                         << "0. Back to Main Menu\n"
                         << "Select Option: ";

                    if (!(cin >> estChoice)) {
                        cout << "❌ Invalid input! Please enter a number.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    if (estChoice == 0) break;

                    switch (estChoice) {
                        case 1:
                            addOrEditWindowsLoop(windows, windowTypes, estimator, ratesManager,
                                                summaryManager, finalEstimator, autoRatesManager);
                            break;
                        case 2:
                            showFinalSummary(windows, estimator, ratesManager, summaryManager,
                                            finalEstimator, autoRatesManager);
                            break;
                        case 3:
                            settingsMenu();
                            break;
                        default:
                            cout << "❌ Invalid option.\n";
                            break;
                    }
                }
                break;
            }

            case 3: { // Fabricator submenu
                int fabChoice = -1;
                while (true) {
                    cout << "\n====== Fabricator Menu ======\n"
                         << "1. Add Windows For Fabrication\n"
                         << "2. Get Material Needed\n"
                         << "3. Get Cutting Size\n"
                         << "4. Settings\n"
                         << "0. Back to Main Menu\n"
                         << "Select Option: ";

                    if (!(cin >> fabChoice)) {
                        cout << "❌ Invalid input! Please enter a number.\n";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    if (fabChoice == 0) break;

                    switch (fabChoice) {
                        case 1:
                            addWindowForFabrication();
                            break;
                        case 2:
                            getMaterialNeeded();   // ✅ tum isko implement karoge
                            break;
                        case 3:
                            getCuttingSize();
                            break;
                        case 4:
                            settingsMenu();
                            break;
                        default:
                            cout << "❌ Invalid option.\n";
                            break;
                    }
                }
                break;
            }

            case 0:
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "~   Exiting program. Thank you!   ~\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                std::cout << "\nPress Enter to exit...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                return 0;

            default:
                cout << "❌ Please enter a valid option (0-3).\n";
                break;
        }
    }
}
