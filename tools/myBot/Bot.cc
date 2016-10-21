#include "Bot.h"
#include <cstdlib>
#include <ctime>
using namespace std;

//constructor
Bot::Bot()
{

};

//plays a single game of Ants.
void Bot::playGame()
{
    //reads the game parameters and sets up
	srand(time(NULL));
	cin >> state;
    state.setup();
    endTurn();

    //continues making moves while the game is not over
    while(cin >> state)
    {
        state.updateVisionInformation();
        makeMoves();
        endTurn();
    }
};

//makes the bots moves for the turn
void Bot::makeMoves()
{
    state.bug << "turn " << state.turn << ":" << endl;
    state.bug << state << endl;


    //picks out moves for each ant
    for(int ant=0; ant<(int)state.myAnts.size(); ant++)
	{
		bool validDir = false;
		bool allAround = false;
		bool decideDir = false;
		bool visited = false;
		int direction;
		int d = rand() % 4;
		Location n_loc;
		state.addPos(state.myAnts[ant]);

		for (int i = 0; i < 4; i++)
		{
			Location p_loc = state.getLocation(state.myAnts[ant], i);

			if (state.grid[p_loc.row][p_loc.col].ant != 0 && state.grid[p_loc.row][p_loc.col].isWater == false)
			{
				allAround = false;
				break;
			}
			else
			{
				allAround = true;
			}
		}
		
		if (allAround == false)
		{
			for (int i = 0; i < 4; i++)
			{
				Location p_loc = state.getLocation(state.myAnts[ant], i);

				if (!state.grid[p_loc.row][p_loc.col].isWater)
				{
					for (int j = 0; j < 4; j++)
					{
						n_loc = state.getLocation(p_loc, i);
						if (state.grid[n_loc.row][n_loc.col].isFood)
						{
							direction = i;
							decideDir = true;
							break;
						}
						else if (state.grid[n_loc.row][n_loc.col].hillPlayer != 0)
						{
								direction = i;
								decideDir = true;
								break;
						}
						p_loc = n_loc;
						
					}
				}

				if (decideDir == true)
				{
					break;
				}
			}
			
			if (decideDir == false)
			{
				while (validDir == false)
				{
					Location loc = state.getLocation(state.myAnts[ant], d);


					if (state.grid[loc.row][loc.col].ant != 0 && state.grid[loc.row][loc.col].isWater == false)
					{
						if (state.chkPos(loc) == true)
						{
							direction = d;
							break;
						}
					}
					else
					{
						d = rand() % 4;
					}
				}
			}

		state.makeMove(state.myAnts[ant], direction);
		}
	}

    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
};

/*void Bot::antMove(const Location & antLoc, int dir){
array< int, 2 > directions;
Location destLoc = state.getLocation(antLoc, dir);
int ndirs = state.getDirection(antLoc, destLoc, directions);
for (int i = 0; i < ndirs; ++i){
if (state.isFree(destLoc) && search.find(newLoc) == search.end())
{
state.makeMove(antLoc, directions[i]);
orders.insert(pair< Location, Location >(destLoc, antLoc));
}
}
}

void Bot::Search(){
int distance = 0;
const uint nFood = state.food.size(), nMyAnts = state.myAnts.size();
vector< Location >::iterator hillIt;
map< Location, Location >::iterator ordersIt;
bool movingOut = false;
orders.clear;

for (hillIt = state.myHills.begin(); hillIt < state.myHills.end(); ++hillIt){
orders.insert(pair< Location, Location >(*hillIt, Location()));
}

for (uint foodIdx = 0; foodIdx < nFood; ++foodIdx){
for (uint antIdx = 0; antIdx < nMyAnts; ++antIdx){
int fddir = state.getDirection(antLoc, destLoc, directions);
antMove(state.myAnts[antIdx], state.food[foodIdx], fddir);
}
}

set< Location >::iterator unseenIt = unseen.begin();
while (unseenIt != unseen.end())
{
if (state.grid[unseenIt->row][unseenIt->col].isVisible)
{
unseen.erase(unseenIt++);
}
else
{
++unseenIt;
}
}

for (uint antIdx = 0; antIdx < state.myAnts.size(); ++antIdx)
{
// Check that we are not already moving the ant
movingOut = false;
for (ordersIt = orders.begin(); ordersIt != orders.end(); ++ordersIt)
{
if (ordersIt->second == state.myAnts[antIdx])
{
movingOut = true;
break;
}

if (!movingOut)
{
int i;
for (i = 0, unseenIt = unseen.begin(); unseenIt != unseen.end(); ++unseenIt)
{
int rddir = state.getDirection(state.myAnts[antIdx], *unseenIt, directions);
antMove(state.myAnts[state.myAnts[antIdx], *unseenIt, rddir);
}
}
}
}

for (hillIt = state.myHills.begin(); hillIt != state.myHills.end(); ++hillIt)
{
vector< Location >::iterator it =
find(state.myAnts.begin(), state.myAnts.end(), *hillIt);
if (it != state.myAnts.end())
{
// Check that we are not already moving the ant
movingOut = false;
for (ordersIt = orders.begin(); ordersIt != orders.end(); ++ordersIt)
if (ordersIt->second == *hillIt)
{
movingOut = true;
break;
}

if (!movingOut)
for (int d = 0; d < TDIRECTIONS; ++d){
state.makeMove(*hillIt, d);
break;
}
}
}
}
*/

//finishes the turn
void Bot::endTurn()
{
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
};


