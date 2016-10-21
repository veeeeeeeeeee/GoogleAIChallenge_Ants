#include "Bot.h"
#include <algorithm>

using namespace std;

//constructor
Bot::Bot() {
}

//plays a single game of Ants.
void Bot::playGame() {
	//reads the game parameters and sets up
	cin >> state;
	state.setup();
	endTurn();

	//continues making moves while the game is not over
	while(cin >> state) {
		state.updateVisionInformation();
		makeMoves();
		endTurn();
	}
}

/*----Library Begin----*/

bool Bot::isMove(Location l, int d) {
	Location nextL = state.getLocation(l, d);
	Square* nextS = &state.grid[nextL.row][nextL.col];

	if (!nextS->isWater && nextS->ant == -1) { // no enemy and our ants
		aa[l.row][l.col] = true;

		nextS->agents[0] = 0;
		nextS->agents[1] = 0;
		state.makeMove(l, d);
		return true;
	}
	else return false;
}

bool cmpJob(const pair<float, Job>& lhs, const pair<float, Job>& rhs) {
	return lhs.first < rhs.first;
}

int compareJob (const void * lhs, const void * rhs) {
	if (*(JobDist*)lhs <	*(JobDist*)rhs) return -1;
	if (*(JobDist*)lhs == *(JobDist*)rhs) return 0;
	if (*(JobDist*)lhs >	*(JobDist*)rhs) return 1;
}

void Bot::nextMove(Location src, Location dst) {
	// u, r, d, l
	// predict the direction
	vector<int> nextDir;

	int hc = state.cols/2; // half cols
	int hr = state.rows/2; // half rows

	if (src.row < dst.row) {
		if (dst.row - src.row <= hr)
			nextDir.push_back(2); // d
		else
			nextDir.push_back(0); // u
	}
	if (dst.row < src.row) {
		if (src.row - dst.row <= hr)
			nextDir.push_back(0); // u
		else
			nextDir.push_back(2); // d
	}

	if (src.col < dst.col) {
		if (dst.col - src.col <= hc)
			nextDir.push_back(1); // r
		else
			nextDir.push_back(3); // l
	}
	if (dst.col < src.col) {
		if (src.col - dst.col <= hc)
			nextDir.push_back(3); // l
		else
			nextDir.push_back(1); // r
	}

	for (int i=0; i<(int)nextDir.size(); i++) {
		int top = nextDir[i];
		if (isMove(src, top)) {
			aj[dst.row][dst.col] = true;

			return;
		}
	}

	for (int i=(int)nextDir.size()-1; i>=0; i--) {
		int top = rdir[nextDir[i]];
		if (isMove(src, top)) {
			aj[dst.row][dst.col] = true;
			return;
		}
	}
}

bool Bot::rbfs(Location target, Location ant) {
	priority_queue< Node, vector<Node>, cmp > q;
	bfsmap[target.row][target.col] = 1;
	vector<Location> toReset; toReset.push_back(target);
	int rd = 0;

	q.push(Node(state.distance(target, ant), target));
	bool loop = true;
	while (loop && !q.empty()) {
		Location c = q.top().second; q.pop();

		if (bfsmap[c.row][c.col] >= 50) {
			break;
		}

		for (int i=0; i<4; i++) {
			Location next = state.getLocation(c, i);
			if (next == ant) {
				rd = rdir[i];
				loop = false;
				break;
			}
			if (bfsmap[next.row][next.col] == -1) {
				bfsmap[next.row][next.col] = bfsmap[c.row][c.col] + 1;
				q.push(Node(state.distance(next, ant), next));
				toReset.push_back(next);
			}
		}
	}

	for (int i=0; i<(int)toReset.size(); i++) {
		Location tmp = toReset[i];
		bfsmap[tmp.row][tmp.col] = -1;
		Square t = state.grid[tmp.row][tmp.col];
		if (t.isWater)
			bfsmap[tmp.row][tmp.col] = -2;
		if (t.ant != -1 || t.hillPlayer == 0)
			bfsmap[tmp.row][tmp.col] = -2;
	}

	if (!loop) {
		if (isMove(ant, rd)) {
			aj[target.row][target.col] = true;
		}
		return true;
	}
	return false;
}

/**
 * scout function:
 * 	takes Location as parameter
 * 	start BFS from Location
 * 	if found an ant:
 * 		move the ant
 * 		mark the ant as occupied
 * 		mark the Location as occupied
 * end function
 */

void Bot::scout(Location src) {
	queue<Location> q;
	bfsmap[src.row][src.col] = 1;
	vector<Location> toReset; toReset.push_back(src);

	q.push(src);
	bool loop = true;

	while (loop && !q.empty()) {
		Location c = q.front(); q.pop();
		if (bfsmap[c.row][c.col] > 35) {
			break;
		}

		for (int i=0; i<4; i++) {
			Location n = state.getLocation(c, i);
			if (state.grid[n.row][n.col].ant == 0 && !aa[n.row][n.col]) {
				int rd = rdir[i];
				isMove(n, rd);
				aj[src.row][src.col] = true;
				break;
			}

			if (bfsmap[n.row][n.col] == -1) {
				bfsmap[n.row][n.col] = bfsmap[c.row][c.col] + 1;
				q.push(n);
				toReset.push_back(n);
			}
		}
	}

	for (int i=0; i<(int)toReset.size(); i++) {
		Location tmp = toReset[i];
		bfsmap[tmp.row][tmp.col] = -1;
		Square t = state.grid[tmp.row][tmp.col];
		if (t.isWater)
			bfsmap[tmp.row][tmp.col] = -2;
		if (t.ant != -1 || t.hillPlayer == 0)
			bfsmap[tmp.row][tmp.col] = -2;
	}
}

/*----Library End----*/

void Bot::initialise() {
	state.bug << "turn " << state.turn << ":" << endl;
	state.bug << state << endl;

	//-- update diffuse value --//
	gridBuffer = state.grid;

	for (int i=0; i<state.rows; i++) {
		for (int j=0; j<state.cols; j++) {
			Square* s = &state.grid[i][j];

			// lastSeen
			if (s->isVisible)
				s->lastSeen = 0;
			else s->lastSeen += 0.1f;

			// diffuse
			diffusion(i, j);
		}
	}
	state.grid = gridBuffer;

	bfsmap.clear();
	aa.clear(); aj.clear();

	// -1=moveable, -2=nah
	for (int i=0; i<state.rows; i++) {
		bfsmap.push_back(vector<int>());
		aa.push_back(vector<bool>());
		aj.push_back(vector<bool>());
		for (int j=0; j<state.cols; j++) {
			bfsmap[i].push_back(-1);
			aa[i].push_back(0);
			aj[i].push_back(0);
			Square tmp = state.grid[i][j];
			if (tmp.isWater)
				bfsmap[i][j] = -2;
			if (tmp.ant != -1 || tmp.hillPlayer == 0)
				bfsmap[i][j] = -2;
		}
	}
}

// simple food mission
void Bot::rgather() {
	for (int i=0; i<(int)state.food.size(); i++) {
		Location f = state.food[i];
		scout(f);
	}
}

void Bot::attack() {
	// treat eHills as similar as food
	vector< pair<float, Job> > atk;
	for (int i=0; i<(int)state.enemyHills.size(); i++) {
		Location h = state.enemyHills[i];
		bool knownHill = false;
		for (int j=0; j<(int)eHills.size(); j++) {
			if (eHills[j] == h) { // if h exists in eHills
				knownHill = true;
				break;
			}
		}
		if (!knownHill) { // otherwise, update it in eHills
			eHills.push_back(h);
		}
	}

	for (int i=(int)eHills.size()-1; i>=0; i--) {
		Location h = eHills[i];

		if (!state.grid[h.row][h.col].isHill) { // hill h in eHills is discovered having been destroyed
			// swap and pop_back()
			// works cause this loop only runs if eHills not empty
			Location tmp = eHills.back();
			eHills[(int)eHills.size()-1] = eHills[i];
			eHills[i] = tmp;

			eHills.pop_back();
		}
	}

	for (int i=0; i<(int)eHills.size(); i++) {
		scout(eHills[i]);
		scout(eHills[i]);
	}
}

void Bot::escapeHill() {
	for (int i=0; i<state.rows; i++) {
		for (int j=0; j<state.cols; j++) {
			Square s = state.grid[i][j];
			Location a(i, j);

			if (s.ant == 0 && s.hillPlayer == 0) {
				for (int i=0; i<4; i++) {
					if (isMove(a, i))
						break;
				}
			}

		}
	}
}

void Bot::randomDeploy() {
	for (int i=0; i<(int)state.myAnts.size(); i++) {
		Location a = state.myAnts[i];
		Location toMove = a;

		if (!aa[a.row][a.col]) {
			for (int j=0; j<(int)state.myHills.size(); j++) {
				Location h = state.myHills[j];

				toMove.row += ((a.row - h.row) + state.rows);
				toMove.col += ((a.col - h.col) + state.cols);

				toMove.row %= state.rows;
				toMove.col %= state.cols;
			}

			if (toMove == a) {
				for (int k=0; k<4; k++)
					if (isMove(a, k))
						break;
			}
			else if (!rbfs(toMove, a)) {
				nextMove(a, toMove);
			}
		}
	}
}

void Bot::makeMoves() {
	initialise();
	escapeHill();
	//rgather();
	//attack();
	//randomDeploy();
	trace();

	state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
}

//--- emergent behaviour ---//
void Bot::diffusion(int r, int c) {
	vector<int> toDiffuse;

	Square* s = &state.grid[r][c];
	Square* sb = &gridBuffer[r][c];

	// water
	if (s->isWater) {
		for (int i=0; i<2; i++) {
			s->agents[i] = 0;
			sb->agents[i] = 0;
		}

		return;
	}

	// food
	if (s->isFood || s->hillPlayer > 0) {
		s->agents[0] = 150.0f;
		sb->agents[0] = 150.0f;

		if (s->hillPlayer > 0) {
			s->agents[0] = 175.0f;
			sb->agents[0] = 175.0f;
		}
	}
	else {
		toDiffuse.push_back(0);
	}

	if (!s->isVisible) {
		s->agents[1] = 50.0f + s->lastSeen;
		sb->agents[1] = 50.0f + s->lastSeen;
	}
	else {
		toDiffuse.push_back(1);
	}

	//-- other types of diffuse --//

	Location current(r, c);
	for (int i=0; i<(int)toDiffuse.size(); i++) {
		float adj = 0;
		for (int j=0; j<4; j++) {
			Location next = state.getLocation(current, j);
			Square up = state.grid[next.row][next.col];

			adj += up.agents[i];
		}

		// query from the grid, update on the buffer, copy over later
		sb->agents[i] = adj * 0.25f;
	}
}

void Bot::trace() {
	for (int i=0; i<(int)state.myAnts.size(); i++) {
		Location a = state.myAnts[i];
		if (!aa[a.row][a.col]) {
			float foodScent = state.grid[a.row][a.col].agents[0];
			float sdScent = state.grid[a.row][a.col].agents[1];

			int goal;
			if (foodScent != 0)
				goal = 0;
			else goal = 1;

			traceSingle(a, goal);
		}
	}
}

void Bot::traceSingle(Location ant, int goal) {
	int d = 0;
	float scent = -1.0f;
	for (int i=0; i<4; i++) {
		Location n = state.getLocation(ant, i);
		Square s = state.grid[n.row][n.col];

		if (s.ant == -1 && scent < s.agents[goal]) {
			scent = s.agents[goal];
			d = i;
		}
	}

	isMove(ant, d);
}

//finishes the turn
void Bot::endTurn() {
	if(state.turn > 0)
		state.reset();
	state.turn++;

	cout << "go" << endl;
}
