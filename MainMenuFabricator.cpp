#include "MainMenuFabricator.h"
#include "FabricatorManager.h"
#include "FabricatorWindowTypes.h"


using namespace std;

// add these externs so we can use the globals from main.cpp
extern EstimateLengthManager estimator;
extern ManualRatesManager ratesManager;
extern AutoRatesManager autoRatesManager;

// Fabricator Windows Summary
void printFabricatorWindowsSummary(const vector<unique_ptr<FrameComponent>>& windows) {
    cout << "\n--- Entered Fabricator Windows Summary ---\n";
    for (size_t i = 0; i < windows.size(); ++i) {
        const auto& w = windows[i];
        cout << i + 1 << ": " << w->getLabel()
             << " | Hight: " << w->getHeight()
             << " | wigth: " << w->getWidth()
             << " | lock: " << w->getLockType() 
             << " | collar: " << w->getCollarType() 
             << endl;
    }
}

////////////////////////////  Add or Edit Windows for Fabrication ///////////////////////////
void addWindowForFabrication(
    vector<unique_ptr<FrameComponent>>& windows,
    map<int, pair<string, function<unique_ptr<FrameComponent>()>>>& FabricatorWindowTypes
) {

    // ✅ Purana estimation data clear karo
    estimator.clearAllData();
    ratesManager.reset();
    ratesManager.reset();

    while (true) {
        cout << "\n------ Fabricator Window Types ------\n";
        for (const auto& [id, pair] : FabricatorWindowTypes) {
            cout << pair.first << "   [" << id << "]\n";
        }
        cout << "0. <- Back to Main Menu\n";

        int type;
        cout << "Select Type: ";
        if (!(cin >> type)) {
            cout << "❌ Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (type == 0) {
            cout << "🔙 Returning to Main Menu...\n";
            break;
        }

        if (!FabricatorWindowTypes.count(type)) {
            cout << "❌ Invalid Type! Please try again.\n";
            continue;
        }

        int num = 0;
        while (true) {
            num++;
            cout << "\n------------- Fabricator Window no[" << num << "] -------------\n\n";
            auto window = FabricatorWindowTypes[type].second();
            bool success = window->inputWindowData();
            if (!success) {
                cout << "⚠ Window entry cancelled.\n";
                break;
            }

            window->calculateSections();
            window->sendToEstimator(estimator);   // ✅ send sections to estimator
            windows.push_back(std::move(window));
            cout << "✅ Window added successfully!\n";

            // Show current windows summary
            printFabricatorWindowsSummary(windows);

            cout << "Add another window of this type? (y/n): ";
            char cont;
            cin >> cont;
            if (cont != 'y' && cont != 'Y') break;
        }
    }

    // Review/Edit/Delete loop
    while (true) {
        printFabricatorWindowsSummary(windows);

        cout << "\n➡ What do you want to do with Fabricator Windows?\n";
        cout << "   [1] Add New Window\n";
        cout << "   [2] Edit a Window\n";
        cout << "   [3] Delete a Window\n";
        cout << "   [4] Get Material Needed\n";
        cout << "   [0] <- Back\n";
        cout << "Enter choice: ";

        int option;
        if (!(cin >> option)) {
            cout << "❌ Invalid input! Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (option) {
            case 1:
                // new add
                addWindowForFabrication(windows, FabricatorWindowTypes);
                break;
            case 2:
                // TODO: implement editWindowFabricator
                editWindowFabricator(windows);
                break;
            case 3:
                // TODO: implement deleteWindowFabricator
                 deleteWindowFabricator(windows);
                break;
            case 4:
                getMaterialNeeded(windows, estimator, ratesManager, autoRatesManager);
                break;
            case 0:
               cout << "🔙 Going back...\n";
               return;   // ✅ function se hi bahar nikal jao
            default:
                cout << "❌ Invalid choice.\n";
                break;
        }
    }
}


////////////////////////// ⚙ 2. Material requirement calculate karna
void getMaterialNeeded(
    vector<unique_ptr<FrameComponent>>& windows,
    EstimateLengthManager& estimator,
    ManualRatesManager& ratesManager,
    AutoRatesManager& autoRatesManager 
) {
    if (windows.empty()) {
        cout << "⚠️ No windows added yet!\n";
        return;
    }

    try {

        cout << "\n🔍 Performing Final Estimation...\n";
        // 1. Estimation chalao
        estimator.performEstimation();

        // 2. User ko choice do (Manual ya Auto)
        int modeChoice;
        cout << "\nChoose Rate Input Method:\n";
        cout << "1. Manual Input\n";
        cout << "2. Auto (CSV File)\n";
        cout << "Enter choice: ";
        cin >> modeChoice;

        double grandTotal = 0.0;

        if (modeChoice == 1) {
                // ----- Manual Mode -----
                ratesManager.inputRatesFromUser();
                ratesManager.editRateByLineNumber();
                grandTotal = ratesManager.computeAndStoreTotals();
                ratesManager.printFinalSummaryTable();
                
                cout << "\n✅ Grand Total Aluminium Cost = Rs. "
                << static_cast<int>(grandTotal) << "\n";
                
                while (true) {
                    // 🔹 Ask for discount
                    double discountPercent = 0.0;
                    cout << "Enter discount percentage (0 if none): ";
                    cin >> discountPercent;
                    
                    // 🔹 Apply discount
                    double finalAmount = grandTotal - (grandTotal * discountPercent / 100.0);
                    
                    // 🔹 Show final amount in curly braces
                    cout << "💰 Final Amount after discount = { Rs. "
                    << static_cast<int>(finalAmount) << " }\n";
                    
                    // 🔹 Ask user next step
                    int choice = -1;
                    cout << "\nNext Action:\n";
                    cout << "[1]. Try another discount\n";
                    cout << "[2]. Get Cutting Sizes Report\n";
                    cout << "[0]. <- Back to Home (Fabricator Menu)\n";
                    cout << "Enter choice: ";
                    cin >> choice;
                    
                    if (choice == 0) {
                        cout << "\n-- Going back to Fabricator Menu...\n";
                        break;  // loop se bahar nikal jao
                    } else if (choice == 1) {
                        continue; // loop repeat hoga, user new discount try karega
                        } else if (choice == 2) {
                        getCuttingSize();
                    } else {
                        cout << "⚠️ Invalid choice! Please enter 1 or 0.\n";
                    }
                    
}
            
        } 
        else if (modeChoice == 2) {
            
            // ----- Auto Mode -----
            if (autoRatesManager.updateRateListFromServer(
                    "https://raw.githubusercontent.com/hashir235/Al_Ratelist/main/RateList.csv")) 
            {
                
                autoRatesManager.selectOptions(); // user chooses thickness + color

                // sections ko pehle lao
                auto sections = estimator.getSummaries();

                // review & edit allow karo
                autoRatesManager.reviewAndEditRates(sections);

                grandTotal = autoRatesManager.computeAndStoreTotals(sections);
                
                cout << "\n✅ Grand Total Aluminium Cost = Rs. "
                << static_cast<int>(grandTotal) << "\n";
                
                while (true) {
                    // 🔹 Ask for discount
                    double discountPercent = 0.0;
                    cout << "Enter discount percentage (0 if none): ";
                    cin >> discountPercent;
                    
                    // 🔹 Apply discount
                    double finalAmount = grandTotal - (grandTotal * discountPercent / 100.0);
                    
                    // 🔹 Show final amount in curly braces
                    cout << "💰 Final Amount after discount = { Rs. "
                    << static_cast<int>(finalAmount) << " }\n";
                    
                    // 🔹 Ask user next step
                    int choice = -1;
                    cout << "\nNext Action:\n";
                    cout << "[1]. Try another discount\n";
                    cout << "[2]. Get Cutting Sizes Report\n";
                    cout << "[0]. <- Back to Home (Fabricator Menu)\n";
                    cout << "Enter choice: ";
                    cin >> choice;
                    
                    if (choice == 0) {
                        cout << "\n-- Going back to Fabricator Menu...\n";
                        break;  // loop se bahar nikal jao
                    } else if (choice == 1) {
                        continue; // loop repeat hoga, user new discount try karega
                        } else if (choice == 2) {
                        getCuttingSize();
                    } else {
                        cout << "⚠️ Invalid choice! Please enter 1 or 0.\n";
                    }
                }                    
                
            } else {
                cerr << "❌ Failed to fetch rates online! Falling back to Manual mode.\n";
                modeChoice = 1;
                ratesManager.inputRatesFromUser();
                ratesManager.editRateByLineNumber();
                grandTotal = ratesManager.computeAndStoreTotals();
                ratesManager.printFinalSummaryTable();
                
                cout << "\n✅ Grand Total Aluminium Cost = Rs. "
                << static_cast<int>(grandTotal) << "\n";
                
                while (true) {
                    // 🔹 Ask for discount
                    double discountPercent = 0.0;
                    cout << "Enter discount percentage (0 if none): ";
                    cin >> discountPercent;
                    
                    // 🔹 Apply discount
                    double finalAmount = grandTotal - (grandTotal * discountPercent / 100.0);
                    
                    // 🔹 Show final amount in curly braces
                    cout << "💰 Final Amount after discount = { Rs. "
                    << static_cast<int>(finalAmount) << " }\n";
                    
                    // 🔹 Ask user next step
                    int choice = -1;
                    cout << "\nNext Action:\n";
                    cout << "[1]. Try another discount\n";
                    cout << "[2]. Get Cutting Sizes Report\n";
                    cout << "[0]. Back to Home (Fabricator Menu)\n";
                    cout << "Enter choice: ";
                    cin >> choice;
                    
                    if (choice == 0) {
                        cout << "\n-- Going back to Fabricator Menu...\n";
                        break;  // loop se bahar nikal jao
                    } else if (choice == 1) {
                        continue; // loop repeat hoga, user new discount try karega
                        } else if (choice == 2) {
                        getCuttingSize();
                    } else {
                        cout << "⚠️ Invalid choice! Please enter 1 or 0.\n";
                    }
                }


}
        }
        else {
            cout << "⚠️ Invalid choice! Defaulting to Manual mode.\n";
            modeChoice = 1;
            ratesManager.inputRatesFromUser();
            ratesManager.editRateByLineNumber();
                grandTotal = ratesManager.computeAndStoreTotals();
                ratesManager.printFinalSummaryTable();
                
                cout << "\n✅ Grand Total Aluminium Cost = Rs. "
                << static_cast<int>(grandTotal) << "\n";
                
                while (true) {
                    // 🔹 Ask for discount
                    double discountPercent = 0.0;
                    cout << "Enter discount percentage (0 if none): ";
                    cin >> discountPercent;
                    
                    // 🔹 Apply discount
                    double finalAmount = grandTotal - (grandTotal * discountPercent / 100.0);
                    
                    // 🔹 Show final amount in curly braces
                    cout << "💰 Final Amount after discount = { Rs. "
                    << static_cast<int>(finalAmount) << " }\n";
                    
                    // 🔹 Ask user next step
                    int choice = -1;
                    cout << "\nNext Action:\n";
                    cout << "[1]. Try another discount\n";
                    cout << "[2]. Get Cutting Sizes Report\n";
                    cout << "[0]. Back to Home (Fabricator Menu)\n";
                    cout << "Enter choice: ";
                    cin >> choice;
                    
                    if (choice == 0) {
                        cout << "\n-- Going back to Fabricator Menu...\n";
                        break;  // loop se bahar nikal jao
                    } else if (choice == 1) {
                        continue; // loop repeat hoga, user new discount try karega
                    } else if (choice == 2) {
                        getCuttingSize();
                    } else {
                        cout << "⚠️ Invalid choice! Please enter 1 or 0.\n";
                    }
                }

       
        }

       

    } catch (const std::exception& ex) {
        cerr << "❌ Error during final summary: " << ex.what() << "\n";
    } catch (...) {
        cerr << "❌ Unknown error occurred during final summary!\n";
    }
}


void getCuttingSize() {
    cout << "\n[Fabricator] Cutting Size Report:\n";

    // 🔹 Step 1: Pehle check karo ke windows add hue hain?
    if (estimator.isEmpty()) {
        cout << "⚠️  Pehle windows add karo!\n";
        return;
    }

    // 🔹 Step 2: Check karo ke optimization ho chuka hai ya nahi
    if (!estimator.hasOptimizedData()) {
        cout << "⚠️  Pehle 'Length Optimization' (getMaterial) run karo!\n";
        return;
    }

    // 🔹 Step 3: Ab Cutting Size nikaalna safe hai
    const EstimateLengthResult& result = estimator.getGroupedSectionSummaries();

    CuttingSize cuttingPrinter;
    cuttingPrinter.printToFile(result);

    cout << "✅ Cutting sizes 'CuttingSizeReport.txt' me save ho gaye.\n";
}



/////////////////////////// ⚙ 5. Settings menu
void settingsFabricator() {
    cout << "\n[Fabricator Settings]\n";
    cout << "1. Update cutting margin\n";
    cout << "2. Back\n";

    int choice;
    cin >> choice;

    if (choice == 1) {
        double newMargin;
        cout << "Enter new cutting margin (cm): ";
        cin >> newMargin;
        // TODO: SettingsManagerC use karke save karo
        cout << "✅ Cutting margin updated.\n";
    }
}
