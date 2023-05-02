#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>

#define MOVIE_NUM 3
#define ROW_NUM 6
#define SEAT_NUM 10

using namespace std;

class Movie
{
private:
    int id;
    string name;
    int duration;
    float rating;

private:
    void SetDetails();
    friend class Theater;
    friend istream &operator>>(istream &file, Movie &m);
    friend ostream &operator<<(ostream &file, Movie &m);
};

void Movie::SetDetails()
{
    cout << "Movie Name: ";
    cin >> name;
    cout << "Duration: ";
    cin >> duration;
    cout << "Rating: ";
    cin >> rating;
}

istream &operator>>(istream &file, Movie &m)
{
    string temp;

    try
    {
        getline(file, m.name);

        getline(file, temp);
        m.duration = stoi(temp);

        getline(file, temp);
        m.rating = stof(temp);

        getline(file, temp);
    }
    catch (...)
    {
        cout << "Exception(in Movie)" << endl;
    }

    return file;
}

ostream &operator<<(ostream &file, Movie &m)
{
    file << m.name << endl;
    file << m.duration << endl;
    file << m.rating << endl;
    file << "--------" << endl;

    return file;
}

class Theater
{
private:
    string name;
    string location;
    int timeSlot[6] = {9, 12, 15, 18, 21, 0};
    Movie movies[MOVIE_NUM];

public:
    Theater();
    void AddMovies();
    void RemoveMovieFromFile();
    int DisplayMovies();
    void DisplayMovieDetails(int id);
    int DisplayTimeSlots();
};

Theater::Theater()
{
    ifstream movieFile("Movies.txt", ios::in);

    int i = 0;
    if (movieFile.is_open())
    {
        while (movieFile.good())
        {
            movies[i].id = i + 1;
            movieFile >> movies[i];
            i++;
        }
    }
    movieFile.close();
}

void Theater::AddMovies()
{
    ofstream movieFile("Movies.txt", ios::app);
    Movie m;
    m.SetDetails();
    movieFile << m;
}

void Theater::RemoveMovieFromFile()
{
    ifstream movieFile("Movies.txt", ios::in);
    ofstream temp("temp.txt", ios::out);

    Movie m;
    m.SetDetails();

    Movie curr;

    if (movieFile.is_open())
    {
        while (movieFile >> curr)
        {
            if (!(curr.name == m.name))
            {
                temp << curr;
            }
        }
    }
    movieFile.close();
    temp.close();

    remove("Movies.txt");
    rename("temp.txt", "Movies.txt");
}

int Theater::DisplayMovies()
{
    // Movie selectedMovie;
    int choice;

    cout << endl;
    cout << "Select a Movie" << endl;
    for (int i = 0; i < MOVIE_NUM; i++)
    {
        cout << "\t[" << i + 1 << "] " << movies[i].name << endl;
    }

    cout << "Movie number: ";
    cin >> choice;
    try
    {
        if (choice > 0 && choice <= MOVIE_NUM)
        {
            return choice;
        }
        throw choice;
    }
    catch (int choice)
    {
        cout << "Invalid choice. Enter correct movie number." << endl;
        return -1;
    }
}

void Theater::DisplayMovieDetails(int id)
{
    int index = id - 1;
    if (index >= 0)
    {
        cout << endl;
        cout << "Movie Name:\t" << movies[index].name << endl;
        cout << "Duration:\t" << movies[index].duration << " minutes" << endl;
        cout << "Ratings:\t" << movies[index].rating << endl;
    }
}

int Theater::DisplayTimeSlots()
{
    int choice;

    cout << endl;
    cout << "Select a time slot" << endl;
    for (int i = 0; i < 5; i++)
    {
        cout << "\t[" << i + 1 << "] " << timeSlot[i] << ":00 to "
             << timeSlot[i + 1] << ":00" << endl;
    }
    cout << "Slot number: ";
    cin >> choice;

    try
    {
        if (choice > 0 && choice <= 5)
        {
            return choice;
        }
        throw choice;
    }
    catch (int choice)
    {
        cout << "Invalid choice. Enter correct slot number" << endl;
        return -1;
    }
}

class Ticket
{
private:
    int timeSlot;
    int movieID;
    int seatNo, rowNo;
    string type;

private:
    int SetRow();
    void SetSeat();
    void SetType(int row);
    friend istream &operator>>(istream &file, Ticket &t);
    friend ostream &operator<<(ostream &file, Ticket &t);
    friend bool operator==(const Ticket &lhs, const Ticket &rhs);
    friend class Booking;

public:
    void SetDetails(int slot, int mno);
};

bool operator==(const Ticket &lhs, const Ticket &rhs)
{
    if (lhs.timeSlot == rhs.timeSlot && lhs.movieID == rhs.movieID &&
        lhs.rowNo == rhs.rowNo && lhs.seatNo == rhs.seatNo &&
        lhs.type == rhs.type)
    {
        return true;
    }
    return false;
}

void Ticket::SetDetails(int slot, int mno)
{
    timeSlot = slot;
    movieID = mno;
    int row = SetRow();
    SetSeat();
    SetType(row);
}

int Ticket::SetRow()
{
    cout << "Row No: ";
    cin >> rowNo;
    while (rowNo <= 0 || rowNo > ROW_NUM)
    {
        cout << "Enter the correct row no: ";
        cin >> rowNo;
    }
    return rowNo;
}

void Ticket::SetSeat()
{
    cout << "Seat No: ";
    cin >> seatNo;
    while (seatNo <= 0 || seatNo > SEAT_NUM)
    {
        cout << "Enter the correct seat no: ";
        cin >> seatNo;
    }
}

void Ticket::SetType(int rowNo)
{
    if (rowNo <= 2)
    {
        type = "Platinum";
    }
    else if (rowNo > 2 && rowNo <= 4)
    {
        type = "Gold";
    }
    else if (rowNo > 4 && rowNo <= 6)
    {
        type = "Silver";
    }
    cout << "Type: " << type << endl
         << endl;
}

istream &operator>>(istream &file, Ticket &t)
{
    string temp;
    try
    {
        getline(file, temp);
        t.timeSlot = stoi(temp);

        getline(file, temp);
        t.movieID = stoi(temp);

        getline(file, temp);
        t.rowNo = stoi(temp);

        getline(file, temp);
        t.seatNo = stoi(temp);
    }
    catch (exception e)
    {
        cout << "Exception(in Ticket)" << endl;
    }

    getline(file, t.type);
    getline(file, temp);

    return file;
}

ostream &operator<<(ostream &file, Ticket &t)
{
    file << t.timeSlot << endl;
    file << t.movieID << endl;
    file << t.rowNo << endl;
    file << t.seatNo << endl;
    file << t.type << endl;
    file << "--------" << endl;

    return file;
}

class Booking
{
private:
    int bookedSeats[5 + 1][MOVIE_NUM + 1][ROW_NUM + 1][SEAT_NUM + 1] = {0};

private:
    int CheckSeat(int timeSlot, int mId, int row, int seat);
    void ReserveSeat(Ticket t);
    void RemoveTicketFromFile(Ticket t);

public:
    Booking();
    void BookTicket(int slot, int mno);
    void CancelTicket(Ticket t);
    void DisplayAvailableSeats(int timeSlot, int mId);
};

Booking::Booking()
{
    ifstream booking("Booked.txt", ios::in);

    Ticket t;

    if (booking.is_open())
    {
        while (booking >> t)
        {
            bookedSeats[t.timeSlot][t.movieID][t.rowNo][t.seatNo] = 1;
        }
        booking.close();
    }
}

void Booking::BookTicket(int slot, int mno)
{
    Ticket t;
    t.SetDetails(slot, mno);
    ReserveSeat(t);
}

int Booking::CheckSeat(int timeSlot, int mId, int row, int seat)
{
    return bookedSeats[timeSlot][mId][row][seat];
}

void Booking::ReserveSeat(Ticket t)
{
    ofstream booking("Booked.txt", ios::app);

    if (CheckSeat(t.timeSlot, t.movieID, t.rowNo, t.seatNo) == 0)
    {
        booking << t;
        bookedSeats[t.timeSlot][t.movieID][t.rowNo][t.seatNo] = 1;
    }
    else
    {
        cout << "Seat already taken" << endl;
    }
}

void Booking::RemoveTicketFromFile(Ticket t)
{
    ifstream booking("Booked.txt", ios::in);
    ofstream temp("temp.txt", ios::out);

    Ticket curr;
    if (booking.is_open())
    {
        while (booking >> curr)
        {
            if (!(curr == t))
            {
                temp << curr;
            }
        }
    }
    booking.close();
    temp.close();

    remove("Booked.txt");
    rename("temp.txt", "Booked.txt");
    cout << "Ticket Cancelled Successfully" << endl;
}

void Booking::CancelTicket(Ticket t)
{
    if (CheckSeat(t.timeSlot, t.movieID, t.rowNo, t.seatNo) == 1)
    {
        bookedSeats[t.timeSlot][t.movieID][t.rowNo][t.seatNo] = 0;
        RemoveTicketFromFile(t);
    }
    else if (CheckSeat(t.timeSlot, t.movieID, t.rowNo, t.seatNo) == 0)
    {
        cout << "Ticket doesn't exist in our database. Did you enter the "
                "correct details?"
             << endl;
    }
}

void Booking::DisplayAvailableSeats(int timeSlot, int mId)
{
    // system("clear");
    int skip = 0;

    // for (int i = 1; i <= SEAT_NUM; i++)
    // {
    //     cout << "\t" << i;
    // }
    // cout << endl
    //      << endl;

    for (int i = 1; i <= ROW_NUM; i++)
    {
        if (i <= 2 && skip % 2 == 0)
        {
            cout << "[Platinum]" << endl;
            skip++;
        }
        else if (i > 2 && i <= 4 && skip % 2 == 1)
        {
            cout << "[Gold]" << endl;
            skip++;
        }
        else if (i > 4 && i <= 6 && skip % 2 == 0)
        {
            cout << "[Silver]" << endl;
            skip++;
        }
        char c = i + 64;
        cout << c << ": ";

        for (int j = 1; j <= SEAT_NUM; j++)
        {
            if (CheckSeat(timeSlot, mId, i, j))
            {
                cout << "\t[X]";
            }
            else
            {
                cout << "\t[" << j << "]";
            }
        }
        cout << endl
             << endl;
    }
    cout << "\t\t\t-----------------------------------" << endl;
    cout << "\t\t\t     All Eyes This Way Please!" << endl;
    cout << endl
         << endl;
    // cout << "O: Seat Available" << endl;
    cout << "X: Seat Taken" << endl
         << endl;
}

class Customer
{
protected:
    string name, phone, email;

private:
    void SetName();
    void SetPhone();
    void SetEmail();

public:
    void SetDetails();
};

void Customer::SetDetails()
{
    SetName();
    SetPhone();
    SetEmail();
}

void Customer::SetName()
{
    cout << "Enter your name: ";
    cin >> name;
}

void Customer::SetPhone()
{
    cout << "Enter your phone number: ";
    cin >> phone;

    while (phone.length() != 10)
    {
        cout << endl;
        cout << "Phone number should have 10 digits" << endl;
        cout << "Enter a valid phone number: ";
        cin >> phone;
    }
}

void Customer::SetEmail()
{
    int validEmail = 0;

    cout << "Enter your email address: ";
    cin >> email;

    while (!validEmail)
    {
        for (int i = 0; i < email.length(); ++i)
        {
            if (email.at(i) == '@')
            {
                validEmail = 1;
                return;
            }
        }

        cout << "Invalid email address" << endl;
        cout << "Enter a valid email address: ";
        cin >> email;
    }
}
class Member : Customer
{
private:
    string accountNumber, password;

private:
    void SetPassword();

public:
    void Register();
    friend istream &operator>>(istream &file, Member &m);
    friend ostream &operator<<(ostream &file, Member &m);
    friend class MemberDatabase;
};

void Member::Register()
{
    cout << endl;
    cout << "Registering for Membership:" << endl;
    Customer::SetDetails();
    accountNumber = to_string((rand() % 9999) + 10000);
    SetPassword();
}

void Member::SetPassword()
{
    string temp;
    int flag = 1;

    while (flag)
    {
        cout << "Enter your password: ";
        cin >> password;
        cout << "Reenter your password: ";
        cin >> temp;
        if (temp != password)
        {
            cout << "Password does not match" << endl;
            flag = 1;
        }
        else
        {
            flag = 0;
        }
    }
}

istream &operator>>(istream &file, Member &m)
{
    string temp;
    getline(file, m.accountNumber);
    getline(file, m.name);
    getline(file, m.email);
    getline(file, m.phone);
    getline(file, m.password);
    getline(file, temp);

    return file;
}

ostream &operator<<(ostream &file, Member &m)
{
    file << m.accountNumber << endl;
    file << m.name << endl;
    file << m.email << endl;
    file << m.phone << endl;
    file << m.password << endl;
    file << "----------" << endl;

    return file;
}

class MemberDatabase
{
private:
    vector<Member> members;

private:
    Member GetMember(string name);

public:
    MemberDatabase();
    void Display(Member m);
    void DisplayAllRecords();
    void SaveRecord(Member m);
    bool CheckMember(string name);
    void Login(string name);
    bool CheckPassword(Member m, string password);
};

MemberDatabase::MemberDatabase()
{
    Member member;
    ifstream memberFile("Members.txt", ios::in);

    if (memberFile.is_open())
    {
        while (memberFile >> member)
        {
            members.push_back(member);
        }
    }

    memberFile.close();
}

Member MemberDatabase::GetMember(string name)
{
    for (int i = 0; i < members.size(); i++)
    {
        if (name == members[i].name)
        {
            return members[i];
        }
    }
}

void MemberDatabase::SaveRecord(Member m)
{
    ofstream memberFile("Members.txt", ios::app);

    if (memberFile.is_open())
    {
        memberFile << m;
        members.push_back(m);
        cout << "Record saved successfully" << endl;
    }

    memberFile.close();
}

void MemberDatabase::DisplayAllRecords()
{
    cout << "======================================================\n"
         << "                   MEMBER INFORMATION                 \n"
         << "======================================================\n";

    for (int i = 0; i < members.size(); i++)
    {
        cout << "Account Number : " << members[i].accountNumber << endl;
        cout << "Name           : " << members[i].name << endl;
        cout << "Phone no.      : " << members[i].phone << endl;
        cout << "E-mail         : " << members[i].email << endl;
        cout << "------------------------------------------------------"
             << endl;
    }
}

void MemberDatabase::Display(Member m)
{
    cout << "======================================================\n"
         << "                   MEMBER INFORMATION                 \n"
         << "======================================================\n";

    cout << "Account Number : " << m.accountNumber << endl;
    cout << "Name           : " << m.name << endl;
    cout << "Phone no.      : " << m.phone << endl;
    cout << "E-mail         : " << m.email << endl;
    cout << "------------------------------------------------------" << endl;
}

bool MemberDatabase::CheckMember(string name)
{
    bool flag = false;
    for (int i = 0; i < members.size(); i++)
    {
        if (name == members[i].name)
        {
            flag = true;
            break;
        }
    }

    return flag;
}

bool MemberDatabase::CheckPassword(Member m, string password)
{
    if (m.password == password)
    {
        return true;
    }
    return false;
}

void MemberDatabase::Login(string name)
{
    string password;
    Member m = GetMember(name);

    if (CheckMember(m.name))
    {
        cout << "Password: ";
        cin >> password;
        if (CheckPassword(m, password))
        {
            cout << "Logged In successfully" << endl;
        }
        else
        {
            cout << "Wrong password" << endl;
        }
    }
    else
    {
        cout << "Member doesn't exist. Signup first." << endl;
    }
}

// helper functions

void Title()
{
    const int WIDTH1 = 90;
    const int WIDTH2 = 20;
    const string heading = "MOVIE TICKET RESERVATION SYSTEM";

    cout << endl
         << endl;

    for (int i = 0; i < WIDTH2; i++)
    {
        cout << ' ';
    }
    for (int i = 0; i < WIDTH1; i++)
    {
        cout << '*';
    }

    cout << endl
         << endl;
    for (int i = 0; i < 6; i++)
    {
        cout << "\t";
    }
    cout << heading;
    cout << endl
         << endl;

    for (int i = 0; i < WIDTH2; i++)
    {
        cout << ' ';
    }
    for (int i = 0; i < WIDTH1; i++)
    {
        cout << '*';
    }
    cout << endl;
}

int MemberMenu()
{
    int choice = 0;
    string name;
    MemberDatabase db;

    while (choice != 4)
    {
        cout << "[1] Login(for Members)" << endl;
        cout << "[2] Signup for Membership" << endl;
        cout << "[3] Continue as guest" << endl;
        cout << "Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            cout << "Name: ";
            cin >> name;
            db.Login(name);
            break;
        }

        case 2:
        {
            Member m;
            m.Register();
            db.SaveRecord(m);
            break;
        }

        case 3:
        {
            Customer c;
            c.SetDetails();
            break;
        }

        default:
            cout << "Invalid choice" << endl;
        }
    }

    return choice;
}

int main()
{
    srand(time(0)); // genrates random values
    Theater theater;
    Booking booking;
    int slot, mno, ch = 0;

    while (ch != 4)
    {
        // system("clear");
        // system("CLS");
        Title();
        // MemberMenu();

        cout << endl;
        cout << "[1] Book a Ticket" << endl;
        cout << "[2] Cancel Ticket" << endl;
        cout << "[3] Generate Bill" << endl;
        cout << "[4] Exit" << endl;
        cout << "Choice: ";
        cin >> ch;

        switch (ch)
        {
        case 1:
        {
            mno = theater.DisplayMovies();
            if (mno != -1)
            {
                slot = theater.DisplayTimeSlots();
                if (slot != -1)
                {
                    booking.DisplayAvailableSeats(slot, mno);
                    booking.BookTicket(slot, mno);
                }
            }
            break;
        }

        case 2:
        {
            system("cls");
            Ticket t;
            cout << "Enter the details of ticket to be cancelled" << endl;
            cout << "Timeslot: ";
            cin >> slot;
            cout << "Movie Number: ";
            cin >> mno;
            t.SetDetails(slot, mno);
            booking.CancelTicket(t);
            break;
        }

        case 3:
        {
            cout << "Generating bill" << endl;
            break;
        }

        case 4:
        {
            cout << "Exiting" << endl;
            break;
        }

        default:
            cout << "Invalid choice" << endl;
            break;
        }
    }

    return 0;
}