#include "MainMenuFunctions.h"

// 🔹 Global Managers
EstimateLengthManager estimator;
FinalSummaryManager summaryManager;
FinalCostCalculator finalEstimator;
ManualRatesManager ratesManager(estimator.getSummaries()); 
AutoRatesManager autoRatesManager;

int main() {
  //  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    SettingsManagerA::getInstance().loadFromFile("section_settings.txt");
    
      // Store all created windows here
    vector<unique_ptr<FrameComponent>> windows;

      // Map of window type ID → (name, factory function)

    while (true) {
        int choice = -1;

        while (true) {
            cout << "\n====== Main Menu ======\n"
                 << "1. Clear All Entered Windows\n"
                 << "2. Add Window for Estimation\n"
                 << "3. Show Final Summary\n"
                 << "4. Add Window for Fabrication  // coming soon!\n"
                 << "5. Get Cutting Size            // coming soon!\n"
                 << "6. Settings\n"
                 << "0. Exit\n"
                 << "Select Option: ";

            if (cin >> choice) {
                if (choice >= 0 && choice <= 6) break;
                cout << "❌ Please enter a valid option (1-6).\n";
            } else {
                cout << "❌ Invalid input! Please enter a number.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        switch (choice) {

            case 1:{
               std::cout << "\n⚠ WARNING: This will remove all entered windows data permanently!\n";
               std::cout << "   Are you sure you want to refresh? (y/n): ";
               std::string confirm;
               std::cin >> confirm;

             if (!confirm.empty() && (confirm == "y" || confirm == "Y")) {

                windows.clear();
                estimator.clearAllData();  // ✅ custom function bana do

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

            case 2:
               addOrEditWindowsLoop(windows, windowTypes, estimator, ratesManager,
                                   summaryManager, finalEstimator, autoRatesManager);
                break;
            case 3:
              showFinalSummary( windows, estimator, ratesManager, summaryManager,
                               finalEstimator, autoRatesManager );
                break;
            case 4:
                addWindowForFabrication();
                break;
            case 5:
                getCuttingSize();
                break;
            case 6:
                settingsMenu();
                break;
            case 0:
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "~   Exiting program. Thank you!   ~\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

                std::cout << "\nPress Enter to exit...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.get();
                return 0;
        }
    }
}


