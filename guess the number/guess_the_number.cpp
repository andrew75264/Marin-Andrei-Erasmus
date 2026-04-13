#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    int attempt,attemptuser,k;
    srand(time(0));
    int randomnumber= rand() % 101;
    

/*     while (true){
        int stop = 0;
        int randomnumber=rand()%101;
        cout << randomnumber <<endl;
        cin >> stop;
        if (stop == 1){
            break;
        }
    } */
    


    attempt=10;
    attemptuser=10;
    k=0;

    cout<<"can you guess the number that i chose?"<<endl;
    cout<<"try and we'll see"<<endl;
    while(true)
    {cout<<"what's your guess= ";
     cin>>attempt;
    attemptuser--;
    k++;
    cout<<"you have "<<attemptuser<<" attempts left"<<endl;
    if(attemptuser==0)
    {cout<<"you lose, the number was "<<randomnumber<<endl;
        break
    ;}



    if(attempt==randomnumber)
    {cout<<"congratulations you guessed the number in "<<k<<" attempts"<<endl;
    break;
    }
    else if(attempt>randomnumber)
    {cout<<"the number is too high, try again"<<endl;
    }
    else if(attempt<randomnumber)
    {cout<<"the number is too low, try again"<<endl;
    }
    }

}

