#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <limits>

// Constants for default values
const double DEFAULT_DA_RATE = 85.0;
const double DEFAULT_HRA_RATE = 15.0;
const double DEFAULT_MEDICAL_ALLOWANCE = 300.0;
const double DEFAULT_PROFESSIONAL_TAX = 200.0;
const double OVERTIME_RATE = 400.0;

// Global configuration
struct SystemConfig {
    double daRate;
    double hraRate;
    double medicalAllowance;
    double professionalTax;
};

SystemConfig config = {DEFAULT_DA_RATE, DEFAULT_HRA_RATE, DEFAULT_MEDICAL_ALLOWANCE, DEFAULT_PROFESSIONAL_TAX};

class Employee {
protected:
    int empNo;
    std::string empName;
    std::string empAddress;
    std::string empDepartment;
    std::string empDesignation;

public:
    virtual ~Employee() = default;

    virtual void getData() {
        std::cout << "Enter employee number: ";
        std::cin >> empNo;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Enter employee name: ";
        std::getline(std::cin, empName);

        std::cout << "Enter employee address: ";
        std::getline(std::cin, empAddress);

        std::cout << "Enter employee department: ";
        std::getline(std::cin, empDepartment);

        std::cout << "Enter employee designation: ";
        std::getline(std::cin, empDesignation);
    }

    virtual void displayData() const {
        std::cout << "\nEmployee Details:\n";
        std::cout << "Number: " << empNo << "\n";
        std::cout << "Name: " << empName << "\n";
        std::cout << "Address: " << empAddress << "\n";
        std::cout << "Department: " << empDepartment << "\n";
        std::cout << "Designation: " << empDesignation << "\n";
    }

    virtual void calculateSalary() = 0;
    virtual void generatePaySlip() const = 0;
    virtual void storeInFile() const = 0;

    int getEmpNo() const { return empNo; }
    std::string getEmpName() const { return empName; }

    friend std::ostream& operator<<(std::ostream& os, const Employee& emp);
};

std::ostream& operator<<(std::ostream& os, const Employee& emp) {
    emp.displayData();
    return os;
}

class PermanentEmployee : public Employee {
private:
    double basicSalary;
    double dearnessAllowance;
    double houseRentAllowance;
    double medicalAllowance;
    double providentFund;
    double professionalTax;
    double incomeTax;
    double grossSalary;
    double netSalary;

public:
    PermanentEmployee() {
        dearnessAllowance = 0;
        houseRentAllowance = 0;
        medicalAllowance = config.medicalAllowance;
        professionalTax = config.professionalTax;
    }

    void getData() override {
        Employee::getData();

        std::cout << "Enter basic salary: ";
        std::cin >> basicSalary;

        std::cout << "Enter income tax: ";
        std::cin >> incomeTax;
    }

    void calculateSalary() override {
        dearnessAllowance = basicSalary * (config.daRate / 100);
        houseRentAllowance = basicSalary * (config.hraRate / 100);
        providentFund = (basicSalary + dearnessAllowance) * 0.12;

        grossSalary = basicSalary + dearnessAllowance + houseRentAllowance + medicalAllowance;
        netSalary = grossSalary - (providentFund + professionalTax + incomeTax);
    }

    void displayData() const override {
        Employee::displayData();
        std::cout << "Type: Permanent\n";
        std::cout << "Basic Salary: " << basicSalary << "\n";
        std::cout << "Dearness Allowance: " << dearnessAllowance << "\n";
        std::cout << "House Rent Allowance: " << houseRentAllowance << "\n";
        std::cout << "Medical Allowance: " << medicalAllowance << "\n";
        std::cout << "Provident Fund: " << providentFund << "\n";
        std::cout << "Professional Tax: " << professionalTax << "\n";
        std::cout << "Income Tax: " << incomeTax << "\n";
        std::cout << "Gross Salary: " << grossSalary << "\n";
        std::cout << "Net Salary: " << netSalary << "\n";
    }

    void generatePaySlip() const override {
        std::string filename = "payslip_" + std::to_string(empNo) + ".txt";
        std::ofstream outFile(filename);

        if (outFile.is_open()) {
            outFile << "////////////////////////////////////Pay Slip////////////////////////////////////\n\n";
            outFile << "Employee Number: " << empNo << "\n";
            outFile << "Employee Name: " << empName << "\n";
            outFile << "Address: " << empAddress << "\n";
            outFile << "Designation: " << empDesignation << "\n";
            outFile << "Department: " << empDepartment << "\n";
            outFile << "Employee Type: Permanent\n\n";
            outFile << "Basic Salary: " << std::fixed << std::setprecision(2) << basicSalary << "\n";
            outFile << "Dearness Allowance: " << dearnessAllowance << "\n";
            outFile << "House Rent Allowance: " << houseRentAllowance << "\n";
            outFile << "Medical Allowance: " << medicalAllowance << "\n";
            outFile << "Provident Fund: " << providentFund << "\n";
            outFile << "Professional Tax: " << professionalTax << "\n";
            outFile << "Income Tax: " << incomeTax << "\n";
            outFile << "Gross Salary: " << grossSalary << "\n";
            outFile << "Net Salary: " << netSalary << "\n";

            outFile.close();
            std::cout << "Pay slip generated successfully: " << filename << "\n";
        } else {
            std::cerr << "Error creating pay slip file!\n";
        }
    }

    void storeInFile() const override {
        std::ofstream outFile("employees.dat", std::ios::app | std::ios::binary);

        if (outFile.is_open()) {
            outFile.write(reinterpret_cast<const char*>(this), sizeof(*this));
            outFile.close();
        } else {
            std::cerr << "Error opening file for writing!\n";
        }
    }
};

class ContractualEmployee : public Employee {
private:
    double grossSalary;
    double professionalTax;
    double incomeTax;
    double netSalary;

public:
    ContractualEmployee() {
        professionalTax = config.professionalTax;
    }

    void getData() override {
        Employee::getData();

        std::cout << "Enter gross salary: ";
        std::cin >> grossSalary;

        std::cout << "Enter income tax: ";
        std::cin >> incomeTax;
    }

    void calculateSalary() override {
        netSalary = grossSalary - (professionalTax + incomeTax);
    }

    void displayData() const override {
        Employee::displayData();
        std::cout << "Type: Contractual\n";
        std::cout << "Gross Salary: " << grossSalary << "\n";
        std::cout << "Professional Tax: " << professionalTax << "\n";
        std::cout << "Income Tax: " << incomeTax << "\n";
        std::cout << "Net Salary: " << netSalary << "\n";
    }

    void generatePaySlip() const override {
        std::string filename = "payslip_" + std::to_string(empNo) + ".txt";
        std::ofstream outFile(filename);

        if (outFile.is_open()) {
            outFile << "////////////////////////////////////Pay Slip////////////////////////////////////\n\n";
            outFile << "Employee Number: " << empNo << "\n";
            outFile << "Employee Name: " << empName << "\n";
            outFile << "Address: " << empAddress << "\n";
            outFile << "Designation: " << empDesignation << "\n";
            outFile << "Department: " << empDepartment << "\n";
            outFile << "Employee Type: Contractual\n\n";
            outFile << "Gross Salary: " << std::fixed << std::setprecision(2) << grossSalary << "\n";
            outFile << "Professional Tax: " << professionalTax << "\n";
            outFile << "Income Tax: " << incomeTax << "\n";
            outFile << "Net Salary: " << netSalary << "\n";

            outFile.close();
            std::cout << "Pay slip generated successfully: " << filename << "\n";
        } else {
            std::cerr << "Error creating pay slip file!\n";
        }
    }

    void storeInFile() const override {
        std::ofstream outFile("employees.dat", std::ios::app | std::ios::binary);

        if (outFile.is_open()) {
            outFile.write(reinterpret_cast<const char*>(this), sizeof(*this));
            outFile.close();
        } else {
            std::cerr << "Error opening file for writing!\n";
        }
    }
};

struct OvertimeRecord {
    int empNo;
    std::string empName;
    double overtimeHours;
    double overtimeDues;
};

class OvertimeManager {
private:
    std::vector<OvertimeRecord> records;

public:
    void addOvertime() {
        OvertimeRecord record;

        std::cout << "Enter employee number: ";
        std::cin >> record.empNo;

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter employee name: ";
        std::getline(std::cin, record.empName);

        std::cout << "Enter overtime hours: ";
        std::cin >> record.overtimeHours;

        record.overtimeDues = record.overtimeHours * OVERTIME_RATE;
        records.push_back(record);

        std::cout << "Overtime record added successfully.\n";
    }

    void displayAllOvertime() const {
        if (records.empty()) {
            std::cout << "No overtime records found.\n";
            return;
        }

        std::cout << "\nOvertime Dues Report:\n";
        std::cout << std::setw(10) << "Emp No." << std::setw(20) << "Employee Name"
                  << std::setw(15) << "Overtime Hours" << std::setw(15) << "Overtime Dues\n";
        std::cout << std::string(60, '-') << "\n";

        for (const auto& record : records) {
            std::cout << std::setw(10) << record.empNo
                      << std::setw(20) << record.empName
                      << std::setw(15) << record.overtimeHours
                      << std::setw(15) << record.overtimeDues << "\n";
        }
    }

    void saveOvertimeToFile() const {
        std::ofstream outFile("overtime_records.dat", std::ios::binary);

        if (outFile.is_open()) {
            for (const auto& record : records) {
                outFile.write(reinterpret_cast<const char*>(&record), sizeof(record));
            }
            outFile.close();
            std::cout << "Overtime records saved to file.\n";
        } else {
            std::cerr << "Error saving overtime records to file.\n";
        }
    }
};

class PaySlipSystem {
private:
    std::vector<Employee*> employees;
    OvertimeManager overtimeManager;

    void displayMainMenu() {
        std::cout << "\n===== Pay Slip Generation System =====\n";
        std::cout << "1. Add Permanent Employee\n";
        std::cout << "2. Add Contractual Employee\n";
        std::cout << "3. Generate Pay Slip\n";
        std::cout << "4. Display Employee Details\n";
        std::cout << "5. Configure System Settings\n";
        std::cout << "6. Manage Overtime\n";
        std::cout << "7. Display All Overtime Records\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter your choice: ";
    }

    void addEmployee(int type) {
        Employee* emp = nullptr;

        if (type == 1) {
            emp = new PermanentEmployee();
        } else if (type == 2) {
            emp = new ContractualEmployee();
        } else {
            std::cout << "Invalid employee type!\n";
            return;
        }

        emp->getData();
        emp->calculateSalary();
        employees.push_back(emp);
        emp->storeInFile();

        std::cout << "Employee added successfully!\n";
    }

    void generatePaySlip() {
        int empNo;
        std::cout << "Enter employee number: ";
        std::cin >> empNo;

        for (const auto& emp : employees) {
            if (emp->getEmpNo() == empNo) {
                emp->generatePaySlip();
                return;
            }
        }

        std::cout << "Employee not found!\n";
    }

    void displayEmployeeDetails() {
        int empNo;
        std::cout << "Enter employee number: ";
        std::cin >> empNo;

        for (const auto& emp : employees) {
            if (emp->getEmpNo() == empNo) {
                emp->displayData();
                return;
            }
        }

        std::cout << "Employee not found!\n";
    }

    void configureSystem() {
        int choice;
        std::cout << "\n===== System Configuration =====\n";
        std::cout << "1. Set Dearness Allowance Rate\n";
        std::cout << "2. Set HRA Rate\n";
        std::cout << "3. Set Medical Allowance\n";
        std::cout << "4. Set Professional Tax\n";
        std::cout << "5. Back to Main Menu\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "Enter new DA rate (%): ";
                std::cin >> config.daRate;
                break;
            case 2:
                std::cout << "Enter new HRA rate (%): ";
                std::cin >> config.hraRate;
                break;
            case 3:
                std::cout << "Enter new Medical Allowance: ";
                std::cin >> config.medicalAllowance;
                break;
            case 4:
                std::cout << "Enter new Professional Tax: ";
                std::cin >> config.professionalTax;
                break;
            case 5:
                return;
            default:
                std::cout << "Invalid choice!\n";
        }

        std::cout << "Configuration updated successfully.\n";
    }

    void manageOvertime() {
        overtimeManager.addOvertime();
        overtimeManager.saveOvertimeToFile();
    }

public:
    void run() {
        int choice;

        do {
            displayMainMenu();
            std::cin >> choice;

            switch (choice) {
                case 1:
                    addEmployee(1);
                    break;
                case 2:
                    addEmployee(2);
                    break;
                case 3:
                    generatePaySlip();
                    break;
                case 4:
                    displayEmployeeDetails();
                    break;
                case 5:
                    configureSystem();
                    break;
                case 6:
                    manageOvertime();
                    break;
                case 7:
                    overtimeManager.displayAllOvertime();
                    break;
                case 8:
                    std::cout << "Exiting system...\n";
                    break;
                default:
                    std::cout << "Invalid choice! Please try again.\n";
            }
        } while (choice != 8);

        // Clean up
        for (auto& emp : employees) {
            delete emp;
        }
        employees.clear();
    }
};

int main() {
    PaySlipSystem system;
    system.run();
    return 0;
}
