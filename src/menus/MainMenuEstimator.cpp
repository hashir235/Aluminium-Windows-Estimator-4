#include "MainMenuEstimator.h"

void printWindowsSummary(const vector<unique_ptr<FrameComponent>>& windows) {
    cout << "\n--- Entered Windows Summary ---\n";
    for (size_t i = 0; i < windows.size(); ++i) {
        const auto& w = windows[i];

        // Try casting to SlideWindow to access SlideWindow-specific methods
        if (auto sw = dynamic_cast<S_Win_e*>(w.get())) {
            cout << i + 1 << ": " << sw->getLabel()
                 << "Size: " << sw->getSize() << "  |  "
                 << "Height: " << sw->getHeight() << " i/f |  "
                 << "Width: " << sw->getWidth() << " i/f |  "
                 << "Collar: " << sw->getCollarType() << "  |  "
                 << "Area: " << sw->getAreaSqFt() << " ft \n";
        }
        else if (auto swm = dynamic_cast<SM_Win_e*>(w.get())) {
            cout << i + 1 << ": " << swm->getLabel()
                 << "Size: " << swm->getSize() << "  |  "
                 << "Height: " << swm->getHeight() << " i/f |  "
                 << "Width: " << swm->getWidth() << " i/f |  "
                 << "Collar: " << swm->getCollarType() << "  |  "
                 << "Area: " << swm->getAreaSqFt() << "ft \n";
        }
        else if (auto sg = dynamic_cast<SG_Win_e*>(w.get())) {
            cout << i + 1 << ": " << sg->getLabel()
                 << "Size: " << sg->getSize() << "  |  "
                 << "Height: " << sg->getHeight() << " i/f |  "
                 << "Width: " << sg->getWidth() << " i/f |  "
                 << "Collar: " << sg->getCollarType() << "  |  "
                 << "Area: " << sg->getAreaSqFt() << "ft \n";
        }
        else if (auto sgm = dynamic_cast<SGM_Win_e*>(w.get())) {
            cout << i + 1 << ": " << sgm->getLabel()
                 << "Size: " << sgm->getSize() << "  |  "
                 << "Height: " << sgm->getHeight() << " i/f |  "
                 << "Width: " << sgm->getWidth() << " i/f |  "
                 << "Collar: " << sgm->getCollarType() << "  |  "
                 << "Area: " << sgm->getAreaSqFt() << "ft \n";
        }
        else if (auto sc = dynamic_cast<SC_Win_e*>(w.get())) {
            cout << i + 1 << ": " << sc->getLabel()
                 << "Size: " << sc->getSize() << "  |  "
                 << "Height: " << sc->getHeight() << " i/f |  "
                 << "RightWidth: " << sc->getRightWidth() << " i/f |  "
                 << "LeftWidth: " << sc->getLeftWidth() << " i/f |  "
                 << "Collar: " << sc->getCollarType() << "  |  "
                 << "Area: " << sc->getAreaSqFt() << "ft \n";
        }
        else if (auto scm = dynamic_cast<SCM_Win_e*>(w.get())) {
            cout << i + 1 << ": " << scm->getLabel()
                 << "Size: " << scm->getSize() << "  |  "
                 << "Height: " << scm->getHeight() << " i/f |  "
                 << "RightWidth: " << scm->getRightWidth() << " i/f |  "
                 << "LeftWidth: " << scm->getLeftWidth() << " i/f |  "
                 << "Collar: " << scm->getCollarType() << "  |  "
                 << "Area: " << scm->getAreaSqFt() << "ft \n";
        }
        else if (auto fc = dynamic_cast<FC_Win_e*>(w.get())) {
            cout << i + 1 << ": " << fc->getLabel()
                 << "Size: " << fc->getSize() << "  |  "
                 << "Height: " << fc->getHeight() << " i/f |  "
                 << "RightWidth: " << fc->getRightWidth() << " i/f |  "
                 << "LeftWidth: " << fc->getLeftWidth() << " i/f |  "
                 << "Collar: " << fc->getCollarType() << "  |  "
                 << "Area: " << fc->getAreaSqFt() << "ft \n";
        }
        else if (auto f = dynamic_cast<F_Win_e*>(w.get())) {
            cout << i + 1 << ": " << f->getLabel()
                 << "Size: " << f->getSize() << "  |  "
                 << "Height: " << f->getHeight() << " i/f |  "
                 << "Width: " << f->getWidth() << " i/f |  "
                 << "Collar: " << f->getCollarType() << "  |  "
                 << "Area: " << f->getAreaSqFt() << "ft \n";
        }
        else if (auto o = dynamic_cast<O_Win_e*>(w.get())) {
            cout << i + 1 << ": " << o->getLabel()
                 << "Size: " << o->getSize() << "  |  "
                 << "Height: " << o->getHeight() << " i/f |  "
                 << "Width: " << o->getWidth() << " i/f |  "
                 << "Collar: " << o->getCollarType() << "  |  "
                 << "Area: " << o->getAreaSqFt() << "ft \n";
        }
        else if (auto d = dynamic_cast<D_Win_e*>(w.get())) {
            cout << i + 1 << ": " << d->getLabel()
                 << "Size: " << d->getSize() << "  |  "
                 << "Height: " << d->getHeight() << " i/f |  "
                 << "Width: " << d->getWidth() << " i/f |  "
                 << "Collar: " << d->getCollarType() << "  |  "
                 << "Area: " << d->getAreaSqFt() << "ft \n";
        }
        else if (auto ar = dynamic_cast<A_Win_e*>(w.get())) {
            cout << i + 1 << ": " << ar->getLabel()
                 << "Size: " << ar->getSize() << "  |  "
                 << "Height: " << ar->getHeight() << " i/f |  "
                 << "Width: " << ar->getWidth() << " i/f |  "
                 << "Arch: " << ar->getArch() << " i/f |  "
                 << "Collar: " << ar->getCollarType() << "  |  "
                 << "Area: " << ar->getAreaSqFt() << "ft \n";
        }
        else {
            cout << i + 1 << ": " << w->getLabel() << " (Unknown window type)\n";
        }

    }
    
}


void addOrEditWindowsLoop(
    std::vector<std::unique_ptr<FrameComponent>>& windows,
    std::map<int, std::pair<std::string, std::function<std::unique_ptr<FrameComponent>()>>>& EstimatorWindowTypes,
    EstimateLengthManager& estimator,
    ManualRatesManager& ratesManager,          
    FinalSummaryManager& summaryManager,
    FinalCostCalculator& finalEstimator ,
    AutoRatesManager& autoRatesManager       
) {
    while (true) {
        // Show window types
        std::cout << "\n------ Window Types ------\n";
        for (const auto& [id, pair] : EstimatorWindowTypes) {
            std::cout << pair.first << "   [" << id << "]\n";
        }
        std::cout << "0. <- Back to Main Menu\n";

        int type;
        std::cout << "Select Type: ";
        if (!(std::cin >> type)) {
            std::cout << " Invalid input! Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (type == 0) {
            std::cout << " Returning to Main Menu...\n";
            break;
        }

        if (!EstimatorWindowTypes.count(type)) {
            std::cout << " Invalid Type! Please try again.\n";
            continue;
        }

        int num = 0;
        while (true) {
            num++;
            std::cout << "\n------------- Window/Door/Arch no[" << num << "]-------------\n\n";
            auto window = EstimatorWindowTypes[type].second();
            bool success = window->inputWindowData();
            if (!success) {
                std::cout << " Window entry cancelled.\n";
                break;  // Cancel adding this window, back to window types menu
            }

            window->calculateSections();
            window->sendToEstimator(estimator);
            summaryManager.addWindow(window->getLabel(), window->getAreaSqFt());

            windows.push_back(std::move(window));
            std::cout << " Window added successfully!\n";

            // Show current windows summary
            printWindowsSummary(windows);

            // Ask if user wants to add another of the same type
            std::cout << "Add another window of this type? (y/n): ";
            char cont;
            std::cin >> cont;
            if (cont != 'y' && cont != 'Y') break; // back to window types menu
        }
    }

    // --- After finishing adding windows, review/edit/delete loop ---
    while (true) {
        // Always show current summary
        printWindowsSummary(windows);

        std::cout << "\n What do you want to do with Windows?\n";
        std::cout << "   [1] Add New Window\n";
        std::cout << "   [2] Edit a Window\n";
        std::cout << "   [3] Delete a Window\n";
        std::cout << "   [4] Next to Estimate\n";
        std::cout << "   [0] <- Back\n";
        std::cout << "Enter choice: ";

        int option;
        if (!(std::cin >> option)) {
            std::cout << " Invalid input! Please enter a number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (option) {
            case 1: 
                addWindow(windows, estimator, summaryManager);                
                break;
            case 2: 
                 editWindow(windows, estimator, summaryManager); 
                break;
            case 3: 
                deleteWindow(windows, estimator, summaryManager); 
                break;
            case 4: 
                showFinalSummary( windows, estimator, ratesManager, summaryManager, finalEstimator, autoRatesManager );
                break;
            case 0: 
                std::cout << " Going back...\n"; 
                return;  // exit loop completely
            default: 
                std::cout << " Invalid choice.\n"; 
                break;
        }
    }
}


///////////////////////////////////// Show Final Summary /////////////////////////////////////

void showFinalSummary(
    vector<unique_ptr<FrameComponent>>& windows,
    EstimateLengthManager& estimator,
    ManualRatesManager& ratesManager,
    FinalSummaryManager& summaryManager,
    FinalCostCalculator& finalEstimator,
    AutoRatesManager& autoRatesManager 
) {
    if (windows.empty()) {
        cout << " No windows added yet!\n";
        return;
    }

    try {
        cout << "\n Performing Final Estimation...\n";

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
            autoRatesManager.enableAutoRates(false);

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

                // compute totals
                grandTotal = autoRatesManager.computeAndStoreTotals(sections);

                autoRatesManager.enableAutoRates(true);  // agar user ne auto mode select kiya

                // final table
                //autoRatesManager.printFinalSummaryTable();
            } else {
                cerr << " Failed to fetch rates online! Falling back to Manual mode.\n";
                modeChoice = 1;
                ratesManager.inputRatesFromUser();
                ratesManager.editRateByLineNumber();
                grandTotal = ratesManager.computeAndStoreTotals();
                ratesManager.printFinalSummaryTable();
                autoRatesManager.enableAutoRates(false);
            }
        }
        else {
            cout << " Invalid choice! Defaulting to Manual mode.\n";
            modeChoice = 1;
            ratesManager.inputRatesFromUser();
            ratesManager.editRateByLineNumber();
            grandTotal = ratesManager.computeAndStoreTotals();
            ratesManager.printFinalSummaryTable();
            autoRatesManager.enableAutoRates(false);

        }

        // 3. Aluminium total store
        summaryManager.setAluminiumTotal(grandTotal);

        // 4. Baaki rates input lo
        finalEstimator.inputRates();

        // 5. Loop for re-estimation
        while (true) {
            int choice = finalEstimator.displayFinalSummary();

            if (choice == 1) {
                cout << "\n Generating a NEW Bill...\n";

                if (modeChoice == 1) {
                    // Manual mode
                    ratesManager.inputRatesFromUser();
                    ratesManager.editRateByLineNumber();
                    grandTotal = ratesManager.computeAndStoreTotals();
                    ratesManager.printFinalSummaryTable();
                    autoRatesManager.enableAutoRates(false);

                } 
                else {
                    // Auto mode (try again online)
                    if (autoRatesManager.updateRateListFromServer(
                            "https://raw.githubusercontent.com/hashir235/Al_Ratelist/main/RateList.csv")) 
                    {
                        autoRatesManager.selectOptions();

                        auto sections = estimator.getSummaries();

                        autoRatesManager.reviewAndEditRates(sections);

                        grandTotal = autoRatesManager.computeAndStoreTotals(sections);

                        autoRatesManager.enableAutoRates(true);  // agar user ne auto mode select kiya
                        //autoRatesManager.printFinalSummaryTable();
                    } 
                    else {
                        cerr << " Online fetch failed! Switching to Manual.\n";
                        modeChoice = 1;
                        ratesManager.inputRatesFromUser();
                        ratesManager.editRateByLineNumber();
                        grandTotal = ratesManager.computeAndStoreTotals();
                        ratesManager.printFinalSummaryTable();
                        autoRatesManager.enableAutoRates(false);

                    }
                }

                summaryManager.setAluminiumTotal(grandTotal);
                finalEstimator.inputRates();
            }
            else if (choice == 2) {
                cout << "\n-- Re-entering Other Rates...\n";
                finalEstimator.inputRates();

            }else if (choice == 4) {
        cout << "\n Generating Material Table and PDF...\n";

         // -----------------------------------------
         // STEP 1: Create / Overwrite TXT file
         // -----------------------------------------
         if (modeChoice == 2) {

           if (autoRatesManager.isAutoRatesEnabled()) {
                auto sections = estimator.getSummaries();
                autoRatesManager.generateMaterialTxt(sections);   // creates data/Temp_material.txt
            }

        } else if (modeChoice == 1) {

            ratesManager.saveFinalSummaryToFile();                 // creates data/Temp_material.txt
        }
        // -----------------------------------------
        // STEP 2: Call Python script (Python will
        //         handle timestamp + PDF path)
        // -----------------------------------------
        std::string cmd =
            "python scripts/material_pdf_e.py data/Temp_material.txt";

        system(cmd.c_str());

        cout << "Material Table PDF generated successfully!\n";

    }else if(choice == 5){

        getCuttingSize(1);  // Creates data/CuttingSizeReport.txt

        int result = system(
            "python scripts/CuttingPDF_fixed.py data/CuttingSizeReport.txt"
        );

        if(result == 0) {
             cout << "PDF generated successfully.\n";
        } else {
            cout << "Failed to generate PDF. Check Python installation.\n";
        }

    }else{
                cout << " Final Estimation complete. Returning to Main Menu...\n";
                break;
        }

    }
    
    } catch (const std::exception& ex) {
        cerr << " Error during final summary: " << ex.what() << "\n";
    } catch (...) {
        cerr << " Unknown error occurred during final summary!\n";
    }
}


//////////////////////////// ⚙️ Section Settings Menu //////////////////////////

void settingsMenu() {
    int option;
    auto& manager = Section_Settings::getInstance();      // Old system (A)
    auto& managerB = general_settings::getInstance();    // New system (B)
    auto& managerC = Margins_Settings::getInstance();   // Newest system (C)
    AutoRatesManager autoRatesManager;  

    do {
        cout << "\n === Section Settings Menu ===\n"
             << "1. Show All Sections\n"
             << "2. Update Section Lengths\n"
             << "3. Rename a Section\n"
             << "4. Set maxExtraPieces\n"
             << "5. Set redZone1 & redZone2\n"
             << "6. Set Cutting Margin\n"
             << "0. 🔙 Back to Main Menu\n"
             << "Choose option: ";
        cin >> option;
        cin.ignore();

        switch (option) {
            case 1:
                manager.displayAllSettings();
                break;

            case 2: {
                string section;
                int count;

                cout << "Enter section name (e.g. DC30F) or 0 to go back: ";
                cin >> section;
                if (section == "0") break; // Back to settings menu

                cout << "How many lengths to enter? (0 to go back): ";
                cin >> count;
                if (count == 0) break; // Back

                if (count < 0) {
                    cout << " Count can't be negative!\n";
                    break;
                }

                vector<int> lengths(count);
                for (int i = 0; i < count; ++i) {
                    cout << "Enter length #" << (i + 1) << ": ";
                    cin >> lengths[i];
                }

                manager.setSectionLengths(section, lengths);
                manager.saveToFile("../data/Section_Settings.txt");
                cout << " Lengths updated and saved!\n";
                break;
            }

            case 3: {
                string oldName, newName;
                cout << "Enter current section name (0 to cancel): ";
                cin >> oldName;
                if (oldName == "0") {
                    cout << "↩️ Cancelled. Returning to settings menu...\n";
                    break;
                }

                cout << "Enter new section name (0 to cancel): ";
                cin >> newName;
                if (newName == "0") {
                    cout << "↩️ Cancelled. Returning to settings menu...\n";
                    break;
                }

                manager.renameSection(oldName, newName);
                manager.saveToFile("../data/Section_Settings.txt");
                cout << " Section renamed and saved!\n";
                break;
            }

            case 4: {
                int value;
                cout << "Enter new value for maxExtraPieces (0 to cancel): ";
                cin >> value;

                if (value == 0) {
                    cout << "↩ Cancelled. Returning to settings menu...\n";
                    break;
                }

                if (value < 0) {
                    cout << " Invalid: maxExtraPieces can't be negative!\n";
                } else {
                    managerB.setMaxExtraPieces(value);
                    managerB.saveSettings("../data/general_settings.txt");
                    cout << " maxExtraPieces updated and saved!\n";
                }
                break;
            }

            case 5: {
                double rz1, rz2;
                cout << "Enter redZone1 value (0 to cancel): ";
                cin >> rz1;
                if (rz1 == 0) {
                    cout << " Cancelled. Returning to settings menu...\n";
                    break;
                }

                cout << "Enter redZone2 value (0 to cancel): ";
                cin >> rz2;
                if (rz2 == 0) {
                    cout << " Cancelled. Returning to settings menu...\n";
                    break;
                }

                if (rz1 <= 0 || rz2 <= 0) {
                    cout << " Error: Values must be greater than 0.\n";
                } else {
                    managerB.setRedZone1(rz1);
                    managerB.setRedZone2(rz2);
                    managerB.saveSettings("../data/general_settings.txt");
                    cout << " redZone1 and redZone2 updated and saved!\n";
                }
                break;
            }

            case 6: {
    while (true) {
        std::cout << "\nCutting Margin Settings:\n";
        std::cout << "1. View Current Margins\n";
        std::cout << "2. Update a Margin\n";
        std::cout << "0. Back to Settings Menu\n";
        std::cout << "Enter your choice: ";

        int subChoice;
        std::cin >> subChoice;

        if (subChoice == 0) {
            std::cout << " Returning to Settings Menu...\n";
            break;
        }

        switch (subChoice) {
            case 1: { // view margins
                std::cout << "\n---------------------------------------------\n";
                std::cout << " No  | Section   | Cutting Margin\n";
                std::cout << "---------------------------------------------\n";

                const auto& allMargins = managerC.getAllMargins();
                int idx = 1;
                for (const auto& [section, margin] : allMargins) {
                    std::cout << " " << idx++ << "   | " << section << "   | " << margin << "\n";
                }
                break;
            }

            case 2: { // update margin
                const auto& allMargins = managerC.getAllMargins();
                if (allMargins.empty()) {
                    std::cout << " No margins available!\n";
                    break;
                }

                std::cout << "\n---------------------------------------------\n";
                std::cout << " No  | Section   | Cutting Margin\n";
                std::cout << "---------------------------------------------\n";

                int idx = 1;
                for (const auto& [section, margin] : allMargins) {
                    std::cout << " " << idx++ << "   | " << section << "   | " << margin << "\n";
                }

                std::cout << "\nEnter the number of section to update margin (0 to cancel): ";
                int choice;
                std::cin >> choice;

                if (choice == 0) {
                    std::cout << " Cancelled updating margin.\n";
                    break;
                }

                if (choice < 1 || choice > (int)allMargins.size()) {
                    std::cout << " Invalid choice! Please try again.\n";
                    break;
                }

                auto it = allMargins.begin();
                std::advance(it, choice - 1);
                std::string sectionName = it->first;

                std::cout << "Set the margin for section [" << sectionName << "]: ";
                double newMargin;
                std::cin >> newMargin;

                if (newMargin < 0) {
                    std::cout << " Margin can't be negative!\n";
                    break;
                }

                managerC.setMargin(sectionName, newMargin);
                managerC.saveToFile();
                std::cout << " Margin updated successfully!\n";
                break;
            }

            default:
                std::cout << " Invalid choice! Try again.\n";
            }
        }
        break;
    }

            case 0:
                cout << " Returning to Main Menu...\n";
                break;

            default:
                cout << " Invalid option. Try again.\n";
        }
    } while (option != 0);
}

