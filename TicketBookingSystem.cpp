#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <map>

using namespace std;

class User
{
protected:
    string userID;
    string name;
    string email;
    string password;

public:
    User() = default;
    User(string id, string n, string e, string p) : userID(id), name(n), email(e), password(p) {}

    virtual void displayMenu() = 0;

    string getEmail() const
    {
        return email;
    }

    string getPassword() const
    {
        return password;
    }

    string getName() const
    {
        return name;
    }
};

class Admin : public User
{
public:
    Admin(string id, string n, string e, string p) : User(id, n, e, p) {}

    void displayMenu() override
    {
        cout << endl;
        cout << "Admin Menu:" << endl;
        cout << "1. Manage Events (Add, Update, Delete)" << endl;
        cout << "2. View Bookings" << endl;
        cout << "3. Generate Reports" << endl;
        cout << "4. Logout" << endl;
    }
};

class Customer : public User
{
public:
    Customer(string id, string n, string e, string p) : User(id, n, e, p) {}

    void displayMenu() override
    {
        cout << endl;
        cout << "Customer Menu:" << endl;
        cout << "1. Search Events" << endl;
        cout << "2. Book Tickets" << endl;
        cout << "3. View Booking History" << endl;
        cout << "4. Logout" << endl;
    }
};

class Event
{
private:
    string eventID;
    string eventName;
    string date;
    string time;
    string venue;
    int availableSeats;

public:
    Event(string id, string name, string d, string t, string v, int seats) : eventID(id), eventName(name), date(d), time(t), venue(v), availableSeats(seats) {}

    string getEventID() const
    {
        return eventID;
    }

    string getEventName() const
    {
        return eventName;
    }

    int getAvailableSeats() const
    {
        return availableSeats;
    }

    void updateSeats(int bookedSeats)
    {
        if (availableSeats >= bookedSeats)
        {
            availableSeats -= bookedSeats;
        }
    }

    void displayDetails() const
    {
        cout << setw(10) << eventID << setw(20) << eventName << setw(15) << date << setw(10) << time << setw(20) << venue << setw(10) << availableSeats << endl;
    }
};

class Booking
{
private:
    string bookingID;
    string customerID;
    string eventID;
    int ticketsBooked;

public:
    Booking(string bID, string cID, string eID, int tickets) : bookingID(bID), customerID(cID), eventID(eID), ticketsBooked(tickets) {}

    string getBookingID() const
    {
        return bookingID;
    }

    string getCustomerID() const
    {
        return customerID;
    }

    string getEventID() const
    {
        return eventID;
    }

    int getTicketsBooked() const
    {
        return ticketsBooked;
    }

    void displayBooking() const
    {
        cout << setw(15) << bookingID << setw(15) << customerID << setw(15) << eventID << setw(10) << ticketsBooked << endl;
    }
};

class TicketBookingSystem
{
private:
    vector<Admin> admins;
    vector<Customer> customers;
    vector<Event> events;
    vector<Booking> bookings;

    User *login()
    {
        string email, password;
        cout << endl;
        cout << "Enter Email: ";
        cin >> email;
        cout << "Enter Password: ";
        cin >> password;

        for (auto &admin : admins)
        {
            if (admin.getEmail() == email && admin.getPassword() == password)
            {
                return &admin;
            }
        }

        for (auto &customer : customers)
        {
            if (customer.getEmail() == email && customer.getPassword() == password)
            {
                return &customer;
            }
        }

        cout << endl;
        cout << "Invalid credentials! Please try again." << endl;
        return nullptr;
    }

    void registerCustomer()
    {
        string id, name, email, password;
        cout << endl;
        cout << "Enter Name: ";
        cin >> name;
        cout << "Enter Email: ";
        cin >> email;

        for (const auto &customer : customers)
        {
            if (customer.getEmail() == email)
            {
                cout << endl;
                cout << "This email is already registered! Please log in." << endl;
                return;
            }
        }

        cout << "Enter Password: ";
        cin >> password;

        id = "C" + to_string(customers.size() + 1);
        customers.emplace_back(id, name, email, password);
        cout << endl;
        cout << "Registration Successful! Your Customer ID: " << id << endl;
    }

public:
    TicketBookingSystem()
    {
        admins.emplace_back("A1", "Admin", "admin@system.com", "admin123");
        events.emplace_back("E1", "Concert", "2024-12-25", "18:00", "Auditorium", 100);
        events.emplace_back("E2", "Theatre", "2024-12-30", "19:00", "Drama Hall", 50);
    }

    void start()
    {
        while (true)
        {
            cout << endl;
            cout << "Main Menu:" << endl;
            cout << "1. Login" << endl;
            cout << "2. Register (Customer)" << endl;
            cout << "3. Exit" << endl;
            cout << "Enter your choice: ";
            int choice;
            cin >> choice;

            switch (choice)
            {
            case 1:
            {
                User *user = login();
                if (user)
                {
                    handleUser(user);
                }
                break;
            }
            case 2:
                registerCustomer();
                break;
            case 3:
                cout << endl;
                cout << "Exiting... Thank you!" << endl;
                return;
            default:
                cout << endl;
                cout << "Invalid choice! Try again." << endl;
            }
        }
    }

    void handleUser(User *user)
    {
        while (true)
        {
            user->displayMenu();
            int choice;
            cin >> choice;

            Admin *admin = dynamic_cast<Admin *>(user);
            if (admin)
            {
                switch (choice)
                {
                case 1:
                    manageEvents();
                    break;
                case 2:
                    viewBookings();
                    break;
                case 3:
                    generateReport();
                    break;
                case 4:
                    return;
                default:
                    cout << endl;
                    cout << "Invalid choice! Try again." << endl;
                }
            }

            Customer *customer = dynamic_cast<Customer *>(user);
            if (customer)
            {
                switch (choice)
                {
                case 1:
                    searchEvents();
                    break;
                case 2:
                    bookTickets(customer);
                    break;
                case 3:
                    viewBookingHistory(customer);
                    break;
                case 4:
                    return;
                default:
                    cout << endl;
                    cout << "Invalid choice! Try again." << endl;
                }
            }
        }
    }

    void manageEvents()
    {
        while (true)
        {
            cout << endl;
            cout << "Manage Events:" << endl;
            cout << "1. Add Event" << endl;
            cout << "2. Update Event" << endl;
            cout << "3. Delete Event" << endl;
            cout << "4. Go Back" << endl;
            cout << "Enter your choice: ";
            int choice;
            cin >> choice;

            switch (choice)
            {
            case 1:
                addEvent();
                break;
            case 2:
                updateEvent();
                break;
            case 3:
                deleteEvent();
                break;
            case 4:
                return;
            default:
                cout << endl;
                cout << "Invalid choice! Try again." << endl;
            }
        }
    }

    void addEvent()
    {
        string id, name, date, time, venue;
        int seats;

        cout << endl;
        cout << "Enter Event ID: ";
        cin >> id;
        cout << "Enter Event Name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter Date (YYYY-MM-DD): ";
        cin >> date;
        cout << "Enter Time (HH:MM): ";
        cin >> time;
        cout << "Enter Venue: ";
        cin.ignore();
        getline(cin, venue);
        cout << "Enter Available Seats: ";
        cin >> seats;
        events.emplace_back(id, name, date, time, venue, seats);
        cout << endl;
        cout << "Event Added Successfully!" << endl;
    }

    void updateEvent()
    {
        string id;
        cout << endl;
        cout << "Enter Event ID to Update: ";
        cin >> id;

        for (auto &event : events)
        {
            if (event.getEventID() == id)
            {
                string name, date, time, venue;
                int seats;

                cout << endl;
                cout << "Enter New Event Name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter New Date (YYYY-MM-DD): ";
                cin >> date;
                cout << "Enter New Time (HH:MM): ";
                cin >> time;
                cout << "Enter New Venue: ";
                cin.ignore();
                getline(cin, venue);
                cout << "Enter New Available Seats: ";
                cin >> seats;
                event = Event(id, name, date, time, venue, seats);
                cout << endl;
                cout << "Event Updated Successfully!" << endl;
                return;
            }
        }
        cout << endl;
        cout << "Event Not Found!" << endl;
    }

    void deleteEvent()
    {
        string id;
        cout << endl;
        cout << "Enter Event ID to Delete: ";
        cin >> id;

        for (auto it = events.begin(); it != events.end(); ++it)
        {
            if (it->getEventID() == id)
            {
                events.erase(it);
                cout << endl;
                cout << "Event Deleted Successfully!" << endl;
                return;
            }
        }

        cout << endl;
        cout << "Event Not Found!" << endl;
    }

    void viewBookings()
    {
        cout << endl;
        cout << "All Bookings:" << endl;
        cout << setw(15) << "BookingID" << setw(15) << "CustomerID" << setw(15) << "EventID" << setw(10) << "Tickets" << endl;

        for (const auto &booking : bookings)
        {
            booking.displayBooking();
        }
    }

    void generateReport()
    {
        map<string, int> eventBookings;

        for (const auto &booking : bookings)
        {
            eventBookings[booking.getEventID()] += booking.getTicketsBooked();
        }

        cout << endl;
        cout << "Event Report:" << endl;
        cout << setw(10) << "EventID" << setw(10) << "Bookings" << endl;

        for (const auto &entry : eventBookings)
        {
            cout << setw(10) << entry.first << setw(10) << entry.second << endl;
        }
    }

    void searchEvents()
    {
        cout << endl;
        cout << "Available Events:" << endl;
        cout << setw(10) << "EventID" << setw(20) << "EventName" << setw(15) << "Date" << setw(10) << "Time" << setw(20) << "Venue" << setw(10) << "Seats" << endl;

        for (const auto &event : events)
        {
            event.displayDetails();
        }
    }

    void bookTickets(Customer *customer)
    {
        string eventID;
        int tickets;

        cout << endl;
        cout << "Enter Event ID to Book: ";
        cin >> eventID;
        cout << "Enter Number of Tickets: ";
        cin >> tickets;

        for (auto &event : events)
        {
            if (event.getEventID() == eventID)
            {
                if (event.getAvailableSeats() >= tickets)
                {
                    event.updateSeats(tickets);

                    string bookingID = "B" + to_string(bookings.size() + 1);
                    bookings.emplace_back(bookingID, customer->getName(), eventID, tickets);

                    cout << endl;
                    cout << "Booking Successful! Booking ID: " << bookingID << endl;
                }
                else
                {
                    cout << endl;
                    cout << "Not Enough Seats Available!" << endl;
                }
                return;
            }
        }
        cout << endl;
        cout << "Event Not Found!" << endl;
    }

    void viewBookingHistory(Customer *customer)
    {
        cout << endl;
        cout << "Your Bookings:" << endl;
        cout << setw(15) << "BookingID" << setw(15) << "CustomerID" << setw(15) << "EventID" << setw(10) << "Tickets" << endl;

        for (const auto &booking : bookings)
        {
            if (booking.getCustomerID() == customer->getName())
            {
                booking.displayBooking();
            }
        }
    }
};

int main()
{
    TicketBookingSystem system;
    system.start();
}