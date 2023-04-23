#include <fstream>
#include <iostream>
#include <limits>
#include <string>

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
    friend istream& operator>>(istream& file, Movie& m);
};

istream& operator>>(istream& file, Movie& m)
{
    string temp;

    getline(file, m.name);

    getline(file, temp);
    m.duration = stoi(temp);

    getline(file, temp);
    m.rating = stof(temp);

    // skips blank line
    file.ignore(numeric_limits<streamsize>::max(), '\n');

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
    // virtual void book() = 0;
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
    static int id;
    string name, phone, email;

private:
    void SetId() { id++; }
    void SetName();
    void SetPhone();
    void SetEmail();

public:
    void SetDetails();
    string GetName() { return name; }
    string GetPhone() { return phone; }
    string GetEmail() { return email; }
};

void Customer::SetDetails()
{
    SetId();
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
    cout << "Enter your phone number : ";
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

int Customer::id = 0;

class Member : Customer
{
private:
    string accountNumber, password;

private:
    void SetAcctNum();
    void SetPassword();
    void SaveRecord();

public:
    void Register();
    void DisplayDetails();
    bool CheckMember();
};

void Member::Register()
{
    Customer::SetDetails();
    SetAcctNum();
    SetPassword();

    SaveRecord();
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

void Member::SaveRecord()
{
    ofstream memberFile("Members.txt", ios::app);

    if (memberFile.is_open())
    {
        memberFile << endl;
        memberFile << id << endl;
        memberFile << accountNumber << endl;
        memberFile << name << endl;
        memberFile << email << endl;
        memberFile << phone << endl;
        memberFile << password << endl;

        cout << "Record saved successfully" << endl;
    }
    else
    {
        cout << "Couldn't open file. Does it exist?" << endl;
    }

    memberFile.close();
}
//
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

int main()
{
    Theater theater;

    int ch = 0;

    while (ch != 3)
    {
        // system("clear");
        // system("CLS");
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
