#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

class Movie
{
public:
    int id;
    string name;
    int duration;
    float rating;
};

// istream& operator>>(istream& ins, Movie& m)
// {
//     getline(ins, m.name);
//     getline(ins, m.duration);
//     getline(ins, m.rating);
//
//     // skips blank line
//     ins.ignore(numeric_limits<streamsize>::max(), '\n');
//
//     return ins;
// }

class Theater
{
private:
    string name;
    string location;
    int timings[5] = {9, 12, 15, 18, 21};
    Movie movies[3];

public:
    Theater();
    int DisplayMovies();
    void DisplayMovieDetails(int id);
};

Theater::Theater()
{
    ifstream movieFile("Movies.txt", ios::app);

    int i = 0;
    while (!movieFile.eof())
    {
        movies[i].id = i + 1;
        movieFile >> movies[i].name;
        movieFile >> movies[i].duration;
        movieFile >> movies[i].rating;

        i++;
    }
    movieFile.close();
}

int Theater::DisplayMovies()
{
    Movie selectedMovie;
    int choice;

    cout << endl;
    cout << "Select a Movie" << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << "\t[" << i + 1 << "] " << movies[i].name << endl;
    }

    cout << "Movie number: ";
    cin >> choice;
    DisplayMovieDetails(choice);
    return choice;
}

void Theater::DisplayMovieDetails(int id)
{
    int index = id - 1;

    cout << endl;
    cout << "Movie Name:\t" << movies[index].name << endl;
    cout << "Duration:\t" << movies[index].duration << " minutes" << endl;
    cout << "Ratings:\t" << movies[index].rating << endl;
}

class Ticket
{
protected:
    int price;
    virtual void book() = 0;
};

class Platinum : public Ticket
{
public:
    Platinum() { price = 1000; }
    void book();
};

class Gold : public Ticket
{
public:
    Gold() { price = 500; }
    void book();
};

class Silver : public Ticket
{
public:
    Silver() { price = 250; }
    void book();
};

int main()
{
    Theater theater;

    int ch = 0;

    while (ch != 3)
    {
        cout << endl;
        cout << "[1] Book a Ticket" << endl;
        cout << "[2] Cancel Ticket" << endl;
        cout << "[3] Exit" << endl;
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
            break;
        }

        case 3:
        {
            cout << "Exiting..." << endl;
            break;
        }

        default:
            cout << "Invalid choice" << endl;
            break;
        }
    }

    return 0;
}
