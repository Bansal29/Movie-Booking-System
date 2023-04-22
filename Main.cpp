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
    while (!movieFile.eof())
    {
        movies[i].id = i + 1;
        movieFile >> movies[i];
        i++;
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
