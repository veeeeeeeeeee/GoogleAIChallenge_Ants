#ifndef BOT_H_
#define BOT_H_

#include "State.h"
#include <map>
#include <cmath>

typedef std::pair<Location, Location> Job;
typedef std::pair<float, Job> JobDist;
typedef std::pair<float, Location> Node;
/*
    This struct represents your bot in the game of Ants
*/

const int rdir[4] = {2, 3, 0, 1};

struct Bot {
	State state;
	std::map<Location, Location> moves, mCopy;

	std::map<Location, Location> assigna;
	std::map<Location, Location> assignf;
	std::map<Location, Location> assigns;

	std::vector< std::vector<bool> > aa;
	std::vector< std::vector<bool> > aj;

	std::map<Job, int> assign;
	std::vector<Location> eHills;
	std::vector<Location> mList;

	std::vector< std::vector<int> > bfsmap;
	std::vector< std::vector<Square> > gridBuffer;

	struct cmp {
		bool operator()(Node const& lhs, Node const& rhs) const {
			return lhs.first > rhs.first;
		}
	};
	
	Bot();
	void playGame();    //plays a single game of Ants

	void makeMoves();   //makes moves for a single turn
	void endTurn();     //indicates to the engine that it has made its moves
	bool isMove(Location l, int d);
	void nextMove(Location src, Location dst);
	bool bfs(Location src, Location dst);
	bool rbfs(Location tgt, Location ant);

	void initialise();
	void escapeHill();
	void gather();
	void attack();
	void explore();
	void randomDeploy();

	void scout(Location src);
	void rgather();

	void diffusion(int r, int c);
	void trace();
	void traceSingle(Location ant, int goal);
};

#endif //BOT_H_
