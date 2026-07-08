// CS216 Lab 2
// Muhammad Yusuf Rehman

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace std;

const string LAB_NAME = "CS216 Lab 2";
const string STUDENT_NAME = "Muhammad Yusuf Rehman";

const int MIN_STAT = 60;
const int MAX_STAT = 200;

const int COL_NME = 22;
const int COL_VAL = 14;
const int COL_RND = 6;
const int COL_DMG = 9;
const int COL_DEF_HP = 16;
const int COIN_SIDES = 2;

const string SPACING = "    "; 

enum MenuChoice
{
    BATTLE = 1,
    QUIT = 2
};

class Creature
{
private:
    string name = "unknown";
    int strength = MIN_STAT;
    int health = MIN_STAT;

public:
    Creature();
    Creature(string newName, int newStrength, int newHealth);

    void setCreature(string newName, int newStrength, int newHealth);
    void setHealth(int newHealth);

    string getName() const;
    int getStrength() const;
    int getHealth() const;
    int getDamage() const;
    string to_String() const;
};

void showHeading();
void clearFailedInput(const string& alertMsg);
void executeBattle();
string askForName(const string& promptText);
int generateStat();
int executeAttack(Creature& activeAttacker, Creature& activeDefender);
void declareVictor(const Creature& firstEnt, const Creature& secondEnt, int totalRounds);
void renderTable(const string& caption, const Creature& c1, const Creature& c2);
void renderCreatureColumns();
void renderBattleColumns();
void drawDivider(int length);

// Description: Primary driver loop that manages the menu and program state.
// Precondition: Program is successfully launched.
// Postcondition: Exits 0 when the user selects the quit option.
int main()
{
    int userInput = 0;
    srand(static_cast<unsigned int>(time(0)));
    
    showHeading();

    do
    {
        cout << "\n\nMenu\n1. Battle\n2. Quit\nEnter your choice: ";
        
        if (!(cin >> userInput))
        {
            userInput = 0; 
        }

        switch (userInput)
        {
            case BATTLE:
                executeBattle();
                break;
            case QUIT:
                cout << "\nGoodbye.\n";
                break;
            default:
                clearFailedInput("Invalid input detected. Please enter 1 or 2.");
                break;
        }
    } while (userInput != QUIT);

    return 0;
}

// Description: Initializes a creature with base threshold values.
// Precondition: None.
// Postcondition: Instantiates an object with generic naming and minimum stats.
Creature::Creature()
{
    setCreature("Creature", MIN_STAT, MIN_STAT);
}

// Description: Initializes a creature using specific provided arguments.
// Precondition: All three parameters contain valid data types.
// Postcondition: Instantiates an object with the passed configuration.
Creature::Creature(string newName, int newStrength, int newHealth)
{
    setCreature(newName, newStrength, newHealth);
}

// Description: Assigns values to the class variables.
// Precondition: Variables are supplied.
// Postcondition: Replaces existing member data, enforcing minimum stat rules.
void Creature::setCreature(string newName, int newStrength, int newHealth)
{
    name = newName;
    strength = (newStrength < 1) ? 1 : newStrength;
    health = (newHealth < 0) ? 0 : newHealth;
}

// Description: Modifies the current health pool.
// Precondition: A numeric integer is passed.
// Postcondition: Updates health while routing through the master mutator.
void Creature::setHealth(int newHealth)
{
    setCreature(name, strength, newHealth);
}

// Description: Retrieves the creature's identifier.
// Precondition: Object is populated.
// Postcondition: Identifier is returned.
string Creature::getName() const
{
    return name;
}

// Description: Retrieves the maximum damage potential.
// Precondition: Object is populated.
// Postcondition: Strength value is returned.
int Creature::getStrength() const
{
    return strength;
}

// Description: Retrieves the current life pool.
// Precondition: Object is populated.
// Postcondition: Health value is returned.
int Creature::getHealth() const
{
    return health;
}

// Description: Calculates randomized damage output based on the strength cap.
// Precondition: Strength is greater than zero.
// Postcondition: Returns a rolled integer representing an attack.
int Creature::getDamage() const
{
    return (rand() % strength) + 1;
}

// Description: Formats the member variables into a unified text line.
// Precondition: Column widths are established.
// Postcondition: Outputs a styled row for the table.
string Creature::to_String() const
{
    stringstream ss;
    
    ss << left << setw(COL_NME) << name
       << right << setw(COL_VAL) << strength
       << setw(COL_VAL) << health;

    return ss.str();
}

// Description: Outputs the basic lab and student identifiers.
// Precondition: Constants are assigned.
// Postcondition: Prints the first two lines of the application.
void showHeading()
{
    cout << LAB_NAME << '\n' << STUDENT_NAME << '\n';
}

// Description: Resets the input stream and discards bad characters.
// Precondition: A custom alert string is provided.
// Postcondition: Stream is restored and ready for new keyboard input.
void clearFailedInput(const string& alertMsg)
{
    cout << alertMsg << '\n';
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Description: Manages the setup, execution, and conclusion of a single fight.
// Precondition: The user invoked the battle option.
// Postcondition: The fight is simulated and fully printed to the console.
void executeBattle()
{
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string firstTag = askForName("\nEnter the first creature's name: ");
    string secondTag = askForName("Enter the second creature's name: ");
    
    Creature hero(firstTag, generateStat(), generateStat());
    Creature villain(secondTag, generateStat(), generateStat());
    
    int turnTracker = (rand() % COIN_SIDES == 0) ? 0 : 1;
    int roundNum = 0;

    renderTable("\nCreatures before the battle", hero, villain);

    cout << "\nFirst attacker: " << ((turnTracker == 0) ? hero.getName() : villain.getName()) << "\n\nBattle details\n";
    renderBattleColumns();

    while (hero.getHealth() > 0 && villain.getHealth() > 0)
    {
        roundNum++;
        
        Creature* activeAtt = (turnTracker % 2 == 0) ? &hero : &villain;
        Creature* activeDef = (turnTracker % 2 == 0) ? &villain : &hero;

        int dmgDealt = executeAttack(*activeAtt, *activeDef);

        cout << left << setw(COL_RND) << roundNum
             << setw(COL_NME) << activeAtt->getName()
             << right << setw(COL_DMG) << dmgDealt
             << SPACING 
             << left << setw(COL_NME) << activeDef->getName()
             << right << setw(COL_DEF_HP) << activeDef->getHealth() << '\n';

        turnTracker++;
    }

    declareVictor(hero, villain, roundNum);
    renderTable("\nCreatures after the battle", hero, villain);
}

// Description: Processes the winner logic and displays the final result string.
// Precondition: The fight loop has broken because a creature hit 0 health.
// Postcondition: The victor and total rounds are printed.
void declareVictor(const Creature& firstEnt, const Creature& secondEnt, int totalRounds)
{
    string victor = "n/a";
    string defeated = "n/a";

    if (firstEnt.getHealth() == 0)
    {
        victor = secondEnt.getName();
        defeated = firstEnt.getName();
    }
    else
    {
        victor = firstEnt.getName();
        defeated = secondEnt.getName();
    }

    cout << "\nWinner\n" << victor << " defeated " << defeated << " in " 
         << totalRounds << ((totalRounds == 1) ? " round.\n" : " rounds.\n");
}

// Description: Prompts the keyboard to obtain a valid character name.
// Precondition: A prompt text is supplied.
// Postcondition: Guarantees a non-empty string is acquired.
string askForName(const string& promptText)
{
    string captureName;
    cout << promptText;
    getline(cin, captureName);

    while (captureName == "")
    {
        cout << "Name cannot be blank. Try again: ";
        getline(cin, captureName);
    }
    return captureName;
}

// Description: Generates a randomized stat within the defined threshold bounds.
// Precondition: The random seed has been set.
// Postcondition: Returns a value between the minimum and maximum boundaries.
int generateStat()
{
    return (rand() % (MAX_STAT - MIN_STAT + 1)) + MIN_STAT;
}

// Description: Calculates hit logic and applies the math to the target.
// Precondition: Both entities exist.
// Postcondition: Subtracts points from the defender and returns the hit value.
int executeAttack(Creature& activeAttacker, Creature& activeDefender)
{
    int hitValue = activeAttacker.getDamage();
    activeDefender.setHealth(activeDefender.getHealth() - hitValue);
    return hitValue;
}

// Description: Outputs the pre- and post-battle overview tables.
// Precondition: Both creature objects have been instantiated.
// Postcondition: The data header and both rows are printed.
void renderTable(const string& caption, const Creature& c1, const Creature& c2)
{
    cout << caption << '\n';
    renderCreatureColumns();
    cout << c1.to_String() << '\n' << c2.to_String() << '\n';
}

// Description: Prints the column headers for the general status view.
// Precondition: Constants dictate column widths.
// Postcondition: Spaced header text is displayed.
void renderCreatureColumns()
{
    cout << left << setw(COL_NME) << "Creature"
         << right << setw(COL_VAL) << "Strength"
         << setw(COL_VAL) << "Health" << '\n';

    drawDivider(COL_NME + COL_VAL + COL_VAL);
}

// Description: Prints the column headers for the active fight log.
// Precondition: Constants dictate column widths.
// Postcondition: Spaced header text is displayed.
void renderBattleColumns()
{
    cout << left << setw(COL_RND) << "Round"
         << setw(COL_NME) << "Attacker"
         << right << setw(COL_DMG) << "Damage"
         << SPACING
         << left << setw(COL_NME) << "Defender"
         << right << setw(COL_DEF_HP) << "Defender Health" << '\n';

    drawDivider(COL_RND + COL_NME + COL_DMG + static_cast<int>(SPACING.length()) + COL_NME + COL_DEF_HP);
}

// Description: Replaces the manual for-loop with the modern iomanip method.
// Precondition: The total character width of the table is calculated.
// Postcondition: A horizontal rule is drawn using setfill.
void drawDivider(int length)
{
    cout << setfill('-') << setw(length) << "-" << setfill(' ') << '\n';
}

/*
Sample Test Run:
CS216 Lab 2
Muhammad Yusuf Rehman


Menu
1. Battle
2. Quit
Enter your choice: 1

Enter the first creature's name: yusuf
Enter the second creature's name: bob

Creatures before the battle
Creature                    Strength        Health
--------------------------------------------------
yusuf                            197            87
bob                              140            91

First attacker: yusuf

Battle details
Round Attacker                 Damage    Defender               Defender Health
-------------------------------------------------------------------------------
1     yusuf                       164    bob                                  0

Winner
yusuf defeated bob in 1 round.

Creatures after the battle
Creature                    Strength        Health
--------------------------------------------------
yusuf                            197            87
bob                              140             0


Menu
1. Battle
2. Quit
Enter your choice: 2

Goodbye.

Invalid input test run:
CS216 Lab 2
Muhammad Yusuf Rehman


Menu
1. Battle
2. Quit
Enter your choice: !
Invalid input detected. Please enter 1 or 2.


Menu
1. Battle
2. Quit
Enter your choice: ok i will try again
Invalid input detected. Please enter 1 or 2.


Menu
1. Battle
2. Quit
Enter your choice: 2

Goodbye.

blank name test run:
CS216 Lab 2
Muhammad Yusuf Rehman


Menu
1. Battle
2. Quit
Enter your choice: 1

Enter the first creature's name: 
Name cannot be blank. Try again: kardit
Enter the second creature's name: adim

Creatures before the battle
Creature                    Strength        Health
--------------------------------------------------
kardit                           178            95
adim                             116           135

First attacker: adim

Battle details
Round Attacker                 Damage    Defender               Defender Health
-------------------------------------------------------------------------------
1     adim                         86    kardit                               9
2     kardit                       19    adim                               116
3     adim                         56    kardit                               0

Winner
adim defeated kardit in 3 rounds.

Creatures after the battle
Creature                    Strength        Health
--------------------------------------------------
kardit                           178             0
adim                             116           116


Menu
1. Battle
2. Quit
Enter your choice: 2

Goodbye.
*/