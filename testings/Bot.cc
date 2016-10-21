#include "Bot.h"
#include <algorithm>

using namespace std;

//constructor
Bot::Bot() {

}

/*
bool Bot::rbfs(Location tgt, Location ant, bool sf) {
	// bfs from tgt to ant, move ant by the last step

	priority_queue< Node, vector<Node>, cmp > q;

	bfsmap[tgt.row][tgt.col] = 1;
	vector<Location> toReset;
	toReset.push_back(tgt);
	int rd = 0;

	bool loop = true;
	q.push(Node(state.distance(tgt, ant), tgt));

	while (loop && !q.empty()) {
		Location c = q.top().second; q.pop();
		for (int i=0; i<4; i++) {
			Location next = state.getLocation(c, i);
			if (bfsmap[next.row][next.col] == -1) {
				bfsmap[next.row][next.col] = bfsmap[c.row][c.col] + 1;
				toReset.push_back(next);
				q.push(Node(state.distance(next, ant), next));

				if (next == ant) {
					rd = rdir[i];
					loop = false;
					break;
				}
			}
		}
	}

	for (int i=0; i<(int)toReset.size(); i++) {
		Location t = toReset[i];
		bfsmap[t.row][t.col] = -1;
	}

	if (loop == false) {
		bool im = isMove(ant, rd);
		if (im) {
			aa[ant.row][ant.col] = true;
			if (sf == 0)
				as[tgt.row][tgt.col] = true;
			else af[tgt.row][tgt.col] = true;
			return true;
		}
		else return false;
	}
	return false;
}
*/

/*
void Bot::trace(vector<int>& path, vector< vector<int> >& m, Location src, Location dst) {
	Location c = dst;
	bool loop = true;
	while (loop) {
		for (int i=0; i<4; i++) {
			Location next = state.getLocation(c, i);
			if (m[next.row][next.col] == m[c.row][c.col] - 1) {
				c = next;
				if (i==0) path.push_back(2);
				if (i==1) path.push_back(3);
				if (i==2) path.push_back(0);
				if (i==3) path.push_back(1);
				break;
			}
		}
		if (m[c.row][c.col] == 1) // traced back to src
			loop = false;
	}
}

bool Bot::bfs(Location src, Location dst, bool sf) {
	priority_queue< Node, vector<Node>, cmp > q;

	bfsmap[src.row][src.col] = 1;
	vector<Location> toReset;
	toReset.push_back(src);

	bool loop = true;
	q.push(Node(state.distance(src, dst), src));

	while (loop && !q.empty()) {
		Location c = q.top().second; q.pop();
		for (int i=0; i<4; i++) {
			Location next = state.getLocation(c, i);
			if (bfsmap[next.row][next.col] == -1) { // legal to move in
				bfsmap[next.row][next.col] = bfsmap[c.row][c.col] + 1;
				toReset.push_back(next);
				q.push(Node(state.distance(next, dst), next));
			}
		}
		if (c == dst) {
			loop = false;
			//break;
		}
	}

	bool res;
	vector<int> p;
	if (loop == false) { // only trace if reached dst
		trace(p, bfsmap, src, dst);

		int top = p.back();
		if (isMove(src, top)) {
			//assigna[src] = dst;
			aa[src.row][src.col] = true;
			if (sf == 0)
				//assigns[dst] = src;
				as[dst.row][dst.col] = true;
			else //assignf[dst] = src;
				af[dst.row][dst.col] = true;
		}
		res = true;
	}
	else res = false; // if can't reach, use the normal move

	for (int i=0; i<(int)toReset.size(); i++) {
		Location t = toReset[i];
		bfsmap[t.row][t.col] = -1;
	}
	return res;
}
*/

/*
void Bot::explore() {
	// list all shadow spots
	// do similar to foods

	// if no food, crashed,
	// problem might be here
	vector<Location> shadow;
	vector< pair<float, Job> > explore;
	for (int i=rand()%(state.rows/6); i<state.rows; i+=(state.rows/6)) {
		for (int j=rand()%(state.cols/4); j<state.cols; j+=(state.cols/4)) {
			if (!state.grid[i][j].isVisible && !state.grid[i][j].isWater) {
				Location tmp(i, j);
				shadow.push_back(tmp);
			}
		}

		if ((int)shadow.size() > (int)state.myAnts.size())
			break;
	}
	for (int i=0; i<(int)state.myAnts.size(); i++) {
		Location ant = state.myAnts[i];
		//if (assigna.find(ant) == assigna.end()) // unemployed ant
		if (!aa[ant.row][ant.col])
		for (int j=0; j<(int)shadow.size(); j++) {
			Location s = shadow[j];
			Job tmp(ant, s);
			float dist = (float)state.distance(ant, s);
			pair<float, Job> add(dist, tmp);
			explore.push_back(add);
			break;
		}
	}

	int skip = (int)explore.size()/6;
	if (skip == 0 && !explore.empty())
		skip = 1;

	for (int i=0; i<(int)explore.size(); i+=skip) {
		Job tmp = explore[i].second;

		Location a = tmp.first;
		Location s = tmp.second;

		// if (assigna.find(a) == assigna.end()) {
		if (!aa[a.row][a.col]) {
		// bfs2(a, s, 0);
			nextMove(a, s, 0);
		}
	}
}
*/

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

bool Bot::isMove(Location l, int d) {
	map<Location, Location>::iterator mIt;
	Location nextL = state.getLocation(l, d);

	mIt = moves.find(nextL);
	if (state.grid[nextL.row][nextL.col].ant == -1 && mIt == moves.end() && !state.grid[nextL.row][nextL.col].isWater) { // no enemy and our ants
		moves[nextL] = l;
		mIt = moves.find(l);
		if (mIt != moves.end()) {
			moves.erase(mIt);
			mIt = moves.begin();
		}
        state.makeMove(l, d);
		return true;
	}
	else return false;
}

bool cmpJob(const pair<float, Job>& lhs, const pair<float, Job>& rhs) {
	return lhs.first < rhs.first;
}

int compareJob (const void * lhs, const void * rhs) {
  if (*(JobDist*)lhs <  *(JobDist*)rhs) return -1;
  if (*(JobDist*)lhs == *(JobDist*)rhs) return 0;
  if (*(JobDist*)lhs >  *(JobDist*)rhs) return 1;
}

void Bot::nextMove(Location src, Location dst, bool sf) {
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

	// assign foods for ants
	bool moved = false;
	for (int i=0; i<(int)nextDir.size(); i++) {
		int top = nextDir[i];
		if (isMove(src, top)) {
			//assigna[src] = dst;
			aa[src.row][src.col] = true;
			if (sf == 0)
				//assigns[dst] = src;
				as[dst.row][dst.col] = true;
			else //assignf[dst] = src;
				af[dst.row][dst.col] = true;

			moved = true;
			break;
		}
	}
	if (!moved) {
		for (int i=(int)nextDir.size()-1; i>=0; i--) {
			int top = rdir[nextDir[i]];
			if (isMove(src, top)) {
				//assigna[src] = dst;
				aa[src.row][src.col] = true;
				if (sf == 0)
					//assigns[dst] = src;
					as[dst.row][dst.col] = true;
				else //assignf[dst] = src;
					af[dst.row][dst.col] = true;

				moved = true;
				break;
			}
		}
	}
}

bool Bot::rbfs(Location target, Location ant, bool sf) {
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
	}

	if (!loop) {
		if (isMove(ant, rd)) {
			aa[ant.row][ant.col] = true;
			if (!sf)
				as[target.row][target.col] = true;
			else af[target.row][target.col] = true;
		}
		return true;
	}
	return false;
}

void Bot::initialise() {
    state.bug << "turn " << state.turn << ":" << endl;
    state.bug << state << endl;

	missionList();
	moves.clear();
	bfsmap.clear();
	aa.clear(); af.clear(); as.clear();

	// -1=moveable, -2=nah
	for (int i=0; i<state.rows; i++) {
		bfsmap.push_back(vector<int>());
		aa.push_back(vector<bool>());
		as.push_back(vector<bool>());
		af.push_back(vector<bool>());
		for (int j=0; j<state.cols; j++) {
			bfsmap[i].push_back(-1);
			aa[i].push_back(0);
			as[i].push_back(0);
			af[i].push_back(0);
			Square tmp = state.grid[i][j];
			if (tmp.isWater)
				bfsmap[i][j] = -2;
			if (tmp.isVisible)
			if (tmp.ant != -1)
				bfsmap[i][j] = -2;
		}
	}
	for (int i=0; i<(int)state.myHills.size(); i++) {
		Location tmp = state.myHills[i];
		bfsmap[tmp.row][tmp.col] = -2;
	}
	for (int i=0; i<(int)state.myAnts.size(); i++) {
		Location tmp = state.myAnts[i];
		bfsmap[tmp.row][tmp.col] = -2;
	}
}

void Bot::gather() {
	vector< pair<float, Job> > jobList;
	for (int i=0; i<(int)state.food.size(); i++) {
		Location f = state.food[i];
		for (int j=0; j<(int)state.myAnts.size(); j++) {
			Location a = state.myAnts[j];

			Job tmp = Job(a, f);
			float dist = (float)state.distance(a, f);
			pair<float, Job> add(dist, tmp);
			jobList.push_back(add);
		}
	}

	sort(jobList.begin(), jobList.end(), cmpJob);
	for (int i=0; i<(int)jobList.size()/5; i++) {
		Job tmp = jobList[i].second;

		Location a = tmp.first;
		Location f = tmp.second;

		//if (assignf.find(f) == assignf.end() && assigna.find(a) == assigna.end()) {
		if (!af[f.row][f.col] && !aa[a.row][a.col]) {
			if (!rbfs(f, a, 1)) {
				nextMove(a, f, 1);
			}
		}
	}
	for (int i=(int)jobList.size()/5; i<(int)jobList.size()/3; i++) {
		Job tmp = jobList[i].second;

		Location a = tmp.first;
		Location f = tmp.second;

		//if (assignf.find(f) == assignf.end() && assigna.find(a) == assigna.end()) {
		if (!af[f.row][f.col] && !aa[a.row][a.col]) {
			nextMove(a, f, 1);
		}
	}
}

void Bot::missionList() {
	mList = state.food;
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

	for (int i=0; i<(int)eHills.size(); i++) {
		Location h = eHills[i];

		if (!state.grid[h.row][h.col].isHill && state.grid[h.row][h.col].isVisible) { // hill h in eHills is discovered being destroyed
			// swap and pop_back()
			// works cause this loop only runs if eHills not empty
			Location tmp = eHills.back();
			eHills[(int)eHills.size()-1] = eHills[i];
			eHills[i] = tmp;

			eHills.pop_back();
		}
	}

	for (int i=0; i<(int)state.myAnts.size(); i++) {
		Location a = state.myAnts[i];
		//if (assigna.find(a) == assigna.end())
		if (!aa[a.row][a.col])
		for (int j=0; (int)j<eHills.size(); j++) {
			Location h = eHills[j];

			Job tmp = Job(a, h);
			float dist = (float)state.distance(a, h);
			pair<float, Job> add(dist, tmp);
			atk.push_back(add);
		}
	}

	if (!atk.empty())
		sort(atk.begin(), atk.end(), cmpJob);
/*
	int noJobs = (int)atk.size()/6;
	if (noJobs == 0 && !atk.empty())
		noJobs = 1;
		*/
	for (int i=0; i<(int)atk.size(); i++) {
		Job tmp = atk[i].second;

		Location a = tmp.first;
		Location h = tmp.second;

		//if (assignf.find(h) == assignf.end() && assigna.find(a) == assigna.end()) {
		if (!af[h.row][h.col] && !aa[a.row][a.col]) {
			if (!rbfs(h, a, 1))
				nextMove(a, h, 1);
		}
	}
}


void Bot::randomDeploy() {
	/*
	 * for each ant i, find the closest hill j
	 * randHill[j].push_back(i)
	 * afterwards:
	 * for each hill i
	 *  for each ant j near hill i
	 *  move ant j away from hill i
	 */

	vector< vector<pair<float, int> > > rMove;

	rMove.assign((int)state.myHills.size(), vector< pair<float, int> >());
	for (int i=0; i<(int)state.myAnts.size(); i++) {
		Location ant = state.myAnts[i];

		float minD = 9999;
		int hInd = 0;

		// finding closest hill to ant i
		if (!aa[ant.row][ant.col]) {
			for (int j=0; j<(int)state.myHills.size(); j++) {
				if (minD > state.distance(ant, state.myHills[j])) {
					minD = state.distance(ant, state.myHills[j]);
					hInd = j;
				}
			}
			rMove[hInd].push_back(pair<float, int>(minD, i)); // rMove[i] keeps all ants close to hill i
		}
	}
	int bfsjob = 0;
	for (int i=0; i<(int)rMove.size(); i++) {
		Location h = state.myHills[i];
		sort(rMove[i].begin(), rMove[i].end());
		for (int j=(int)rMove[i].size()-1; j>=0; j--) {
			Location a = state.myAnts[rMove[i][j].second];
			Location toMove;

			toMove.row = (a.row + (a.row - h.row) + state.rows) % state.rows;
			toMove.col = (a.col + (a.col - h.col) + state.cols) % state.cols;

			if (bfsjob < 250) {
				if (!rbfs(toMove, a, 0)) {
					bfsjob++;
					nextMove(a, toMove, 0);
				}
			}
			else {
				nextMove(a, toMove, 0);
			}
		}
	}
}

void Bot::makeMoves() {
	map<Location, Location>::iterator mIt = moves.begin();
	for (int i=0; i<(int)state.myHills.size(); i++) {
		mIt = moves.find(state.myHills[i]);
		if (mIt != moves.end())
			moves.erase(mIt);
	}

	initialise();
	gather();
	attack();
//	explore();
	randomDeploy();

	// move ants out of top of the hill
	/*
	 * moves.erase(hillLocation)
	 * each hill:
	 * if there's ant in hillLocation:
	 * run isMove and state.makeMove like normal move
	 */
	for (int i=0; i<(int)state.myHills.size(); i++) {
		Location tmp = state.myHills[i];
		int j=0;
		for (j=0; j<(int)state.myAnts.size(); j++) {
			if (tmp == state.myAnts[j]) {
				break;
			}
		}
		if (tmp == state.myAnts[j]) {
			mIt = moves.find(tmp);
			if (mIt == moves.end()) {
				int k = 0;
				do {
					k = rand() % 4;
				} while (isMove(tmp, k));
			}
		}
	}

    state.bug << "time taken: " << state.timer.getTime() << "ms" << endl << endl;
}

//finishes the turn
void Bot::endTurn() {
    if(state.turn > 0)
        state.reset();
    state.turn++;

    cout << "go" << endl;
}
