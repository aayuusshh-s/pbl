#include <iostream>
#include <fstream>
#include <string>

#include "stays.h"

using namespace std;

class User {
public:
    string name;
    string email;
    string phone;
    string password;
    bool isProvider = false;

    User() {}

    User(string n, string e, string ph, string p, bool prov = false)
        : name(n),
          email(e),
          phone(ph),
          password(p),
          isProvider(prov) {}
};

class UserStore {
    static const int MAX = 100;

    User users[MAX];
    int count = 0;

public:

    UserStore() {
        load();
    }

    int find(const string &email) const {

        for (int i = 0; i < count; i++) {

            if (users[i].email == email)
                return i;
        }

        return -1;
    }

    bool add(const User &u) {

        if (count >= MAX || find(u.email) >= 0)
            return false;

        users[count++] = u;

        save();

        return true;
    }

    bool check(
        const string &email,
        const string &pass
    ) const {

        int i = find(email);

        return i >= 0 &&
               users[i].password == pass;
    }

    string nameOf(const string &email) const {

        int i = find(email);

        if (i >= 0)
            return users[i].name;

        return "";
    }

    bool isProvider(const string &email) const {

        int i = find(email);

        return i >= 0 &&
               users[i].isProvider;
    }

private:

    void save() const {

        ofstream file("users.txt");

        for (int i = 0; i < count; i++) {

            file << users[i].name << "|"
                 << users[i].email << "|"
                 << users[i].phone << "|"
                 << users[i].password << "|"
                 << users[i].isProvider << "\n";
        }

        file.close();
    }

    void load() {

        ifstream file("users.txt");

        if (!file)
            return;

        string name;
        string email;
        string phone;
        string password;
        string provider;

        while (getline(file, name, '|') &&
               getline(file, email, '|') &&
               getline(file, phone, '|') &&
               getline(file, password, '|') &&
               getline(file, provider)) {

            if (count < MAX) {

                users[count++] = User(
                    name,
                    email,
                    phone,
                    password,
                    provider == "1"
                );
            }
        }

        file.close();
    }
};


class StayFinder {

    UserStore store;
    StayStore stays;

    static string ask(const string &label) {

        string s;

        cout << label << ": ";
        getline(cin, s);

        return s;
    }

    static bool allDigits(const string &s) {

        if (s.empty())
            return false;

        for (char c : s) {

            if (c < '0' || c > '9')
                return false;
        }

        return true;
    }

    static bool validEmail(const string &e) {

        return e.find('@') != string::npos &&
               e.find('.') != string::npos;
    }

    static string validate(
        const User &u,
        const string &confirm
    ) {

        if (u.name.empty())
            return "Enter your name.";

        if (!validEmail(u.email))
            return "Invalid email.";

        if (u.phone.length() != 10 ||
            !allDigits(u.phone))
            return "Need a 10-digit mobile number.";

        if (u.password.length() < 8)
            return "Password needs 8+ characters.";

        if (u.password != confirm)
            return "Passwords do not match.";

        return "";
    }

    void dashboard(const string &email) {

        while (true) {

            cout << "\n--- Customer ---\n";

            cout << "Welcome "
                 << store.nameOf(email)
                 << "\n\n";

            cout << "1. Search Stay\n";
            cout << "2. View Apartment Details\n";
            cout << "3. Logout\n";

            string choice = ask("Choose");

            if (choice == "1") {

                searchStay();

            }
            else if (choice == "2") {

                stays.viewApartmentDetails();

            }
            else if (choice == "3") {

                break;

            }
            else {

                cout << "Invalid choice.\n";
            }
        }
    }

    void searchStay() {

        cout << "\n--- Search Stay ---\n";

        string city =
            ask("City / Area");

        string budget =
            ask("Max budget / night (Rs)");

        string guests =
            ask("Guests");

        string bhk =
            ask("BHK (1/2/3/4)");

        string in =
            ask("Check-in (yyyy-mm-dd)");

        string out =
            ask("Check-out (yyyy-mm-dd)");

        if (out <= in) {

            cout << "Error: check-out must be after check-in.\n";
            return;
        }

        if (!allDigits(budget) ||
            !allDigits(guests) ||
            !allDigits(bhk)) {

            cout << "Error: budget, guests and BHK must be numbers.\n";
            return;
        }

        cout << "\n========== RESULTS ==========\n";

        int n = stays.search(
            city,
            stoi(budget),
            stoi(guests),
            stoi(bhk)
        );

        if (n == 0) {

            cout << "No stays found.\n";
        }
        else {

            cout << n
                 << " stay(s) found.\n";
        }
    }

    void addStay(const string &owner) {

        cout << "\n--- Add Your Place ---\n";

        string name =
            ask("Place name");

        string type =
            ask("Type (Hotel/OYO/PG/B&B/Apartment)");

        string city =
            ask("City");

        string price =
            ask("Price per night (Rs)");

        string guests =
            ask("Max guests");

        string bhk =
            ask("BHK");

        if (name.empty() ||
            type.empty() ||
            city.empty()) {

            cout << "Error: name, type and city are required.\n";
            return;
        }

        if (!allDigits(price) ||
            !allDigits(guests) ||
            !allDigits(bhk)) {

            cout << "Error: price, guests and BHK must be numbers.\n";
            return;
        }

        Stay s(
            name,
            type,
            city,
            stoi(price),
            stoi(guests),
            stoi(bhk),
            owner
        );

        if (!stays.add(s)) {

            cout << "Error: could not add stay.\n";
        }
        else {

            cout << "Place added successfully.\n";
            cout << "Apartment details saved.\n";
        }
    }

    void providerMenu(const string &email) {

        while (true) {

            cout << "\n--- Provider ---\n";

            cout << "Welcome "
                 << store.nameOf(email)
                 << "\n\n";

            cout << "1. Add a place\n";
            cout << "2. My places\n";
            cout << "3. Logout\n";

            string choice =
                ask("Choose");

            if (choice == "1") {

                addStay(email);

            }
            else if (choice == "2") {

                cout << "\n--- My Places ---\n";

                if (stays.listByOwner(email) == 0)
                    cout << "No places yet.\n";

            }
            else if (choice == "3") {

                break;

            }
            else {

                cout << "Invalid choice.\n";
            }
        }
    }

    void login() {

        cout << "\n--- Login ---\n";

        string email =
            ask("Email");

        string pass =
            ask("Password");

        if (!store.check(email, pass)) {

            cout << "Error: wrong email or password.\n";
            return;
        }

        if (store.isProvider(email)) {

            providerMenu(email);

        }
        else {

            dashboard(email);
        }
    }

    void signup() {

        cout << "\n--- Create Account ---\n";

        string name =
            ask("Full Name");

        string email =
            ask("Email");

        string phone =
            ask("Mobile (10 digits)");

        string pass =
            ask("Password");

        string confirm =
            ask("Confirm Password");

        string role =
            ask("Account type (1 = Customer, 2 = Provider)");

        if (role != "1" &&
            role != "2") {

            cout << "Error: choose 1 or 2.\n";
            return;
        }

        User u(
            name,
            email,
            phone,
            pass,
            role == "2"
        );

        string err =
            validate(u, confirm);

        if (!err.empty()) {

            cout << "Error: "
                 << err
                 << "\n";

            return;
        }

        if (!store.add(u)) {

            cout << "Error: email already registered or list full.\n";
            return;
        }

        cout << "Account created.\n";
        cout << "Please login.\n";
    }

public:

    void run() {

        while (true) {

            cout << "\n========================\n";
            cout << "      StayFinder\n";
            cout << "========================\n";

            cout << "1. Login\n";
            cout << "2. Create New Account\n";
            cout << "3. Exit\n";

            string choice =
                ask("Choose");

            if (choice == "1") {

                login();

            }
            else if (choice == "2") {

                signup();

            }
            else if (choice == "3") {

                cout << "Thank you for using StayFinder.\n";
                break;

            }
            else {

                cout << "Invalid choice.\n";
            }
        }
    }
};


int main() {

    StayFinder app;

    app.run();

    return 0;
}