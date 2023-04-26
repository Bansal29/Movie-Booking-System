#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#define MOVIE_NUM 3

using namespace std;

class Movie
{
private:
    int id;
    string name;
    int duration;
    float rating;

    friend class Theater;

    friend istream& operator>>(istream& file, Movie& m)
    {
        string temp;

        getline(file, m.name);

        getline(file, temp);
        m.duration = stoi(temp);

        getline(file, temp);
        m.rating = stof(temp);

        return file;
    }
};

class Theater
{
private:
    string name;
    string location;
    int timeSlot[6] = {9, 12, 15, 18, 21, 0};
    Movie movies[MOVIE_NUM];

public:
    Theater();
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
        while (!movieFile.eof())
        {
            movies[i].id = i + 1;
            movieFile >> movies[i];
            i++;
        }
    }
    movieFile.close();
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
            DisplayTimeSlots();
            DisplayMovieDetails(choice);
            return choice;
        }
        else
        {
            throw choice;
        }
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
        cout << "\t[" << i + 1 << "] " << timeSlot[i] << ":00 to " << timeSlot[i + 1] << ":00" << endl;
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
protected:
    int price;
};

// class Platinum : public Ticket
// {
// public:
//     Platinum() { price = 1000; }
//     void book();
// };
//
// class Gold : public Ticket
// {
// public:
//     Gold() { price = 500; }
//     void book();
// };
//
// class Silver : public Ticket
// {
// public:
//     Silver() { price = 250; }
//     void book();
// };

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
    void SetAcctNum();
    void SetPassword();

public:
    void Register();

    friend class MemberDatabase;
    friend istream& operator>>(istream& file, Member& m);
    friend ostream& operator<<(ostream& file, Member& m);
};

void Member::Register()
{
    Customer::SetDetails();
    SetAcctNum();
    SetPassword();
}

void Member::SetAcctNum()
{
    srand(time(0));
    accountNumber = to_string((rand() % 9999) + 10000);
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

istream& operator>>(istream& file, Member& m)
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

ostream& operator<<(ostream& file, Member& m)
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

public:
    void ReadRecords();
    void SaveRecord(Member m);
    void Display();
    bool CheckMember(string name);
};

void MemberDatabase::ReadRecords()
{
    Member member;
    ifstream memberFile("Members.txt", ios::in);

    if (memberFile.is_open())
    {
        while (!memberFile.eof())
        {
            memberFile >> member;
            members.push_back(member);
        }
    }
    else
    {
        cout << "Couldn't open the file" << endl;
    }

    memberFile.close();
}

void MemberDatabase::SaveRecord(Member m)
{
    ofstream memberFile("Members.txt", ios::app);

    if (memberFile.is_open())
    {
        memberFile << m;
        cout << "Record saved successfully" << endl;
    }
    else
    {
        cout << "Couldn't open file. Does it exist?" << endl;
    }

    memberFile.close();
}

void MemberDatabase::Display()
{
    cout << "======================================================\n"
         << "                   MEMBER INFORMATION                 \n"
         << "======================================================\n";

    for (int i = 0; i < members.size() - 1; i++)
    {
        cout << "\nAccount Number : " << members[i].accountNumber << "\nName           : " << members[i].name
             << "\nPhone no.      : " << members[i].phone << "\nE-mail         : " << members[i].email
             << "\n------------------------------------------------------" << endl;
    }
}

bool MemberDatabase::CheckMember(string name)
{
    bool flag = false;
    for (int i = 0; i < members.size() - 1; i++)
    {
        if (name == members[i].name)
        {
            flag = true;
            break;
        }
    }

    return flag;
}

// void Member::CheckMember()
// {
//     Member members[100];
//     int i = 0;
//
//     ifstream memberFile("Members.txt", ios::in);
//
//     if (memberFile.is_open())
//     {
//         while (!memberFile.eof())
//         {
//             memberFile >> members[i];
//             i++;
//         }
//     }
// }

// helper functions

void Title()
{
    const int WIDTH1 = 90;
    const int WIDTH2 = 20;
    const string heading = "MOVIE TICKET RESERVATION SYSTEM";

    cout << endl << endl;

    for (int i = 0; i < WIDTH2; i++)
    {
        cout << ' ';
    }
    for (int i = 0; i < WIDTH1; i++)
    {
        cout << '*';
    }

    cout << endl << endl;
    for (int i = 0; i < 6; i++)
    {
        cout << "\t";
    }
    cout << heading;
    cout << endl << endl;

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

int MainMenu()
{
    int choice;

    cout << "[1] Login(for Members)" << endl;
    cout << "[2] Signup for Membership" << endl;
    cout << "[3] Continue as guest" << endl;
    cout << "Choice: ";
    cin >> choice;

    return choice;
}

int main()
{
    Theater theater;

    int ch = 0;

    while (ch != 3)
    {
        // system("clear");
        // system("CLS");
        Title();

        switch (MainMenu())
        {
        case 1:
        {
        }
        }

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
            theater.DisplayMovies();
            break;
        }

        case 2:
        {
            cout << "Cancelling the ticket" << endl;
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
