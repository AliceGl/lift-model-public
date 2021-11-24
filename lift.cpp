#include <iostream>
#include <vector>
#include <set>

struct Call {
	int moment, from, to; // time of call, call floor, destination floor
	Call(int moment = 0, int from = 0, int to = 0) : moment(moment), from(from), to(to) {}
};

std::istream& operator >> (std::istream& in, Call& cl) {
	in >> cl.moment >> cl.from >> cl.to;
	return in;
}

void addCall(Call& newCall, std::vector<std::vector<int>>& queueUp, std::vector<std::vector<int>>& queueDown) {
	if (newCall.to > newCall.from)
		queueUp[newCall.from].push_back(newCall.to); // if passenger going up
	else
		queueDown[newCall.from].push_back(newCall.to); // if passenger going down
	return;
}

void callsAppearing(int& currentCall, std::vector<Call>& callsList, int callsNumber, \
	int timer, std::vector<std::vector<int>>& queueUp, std::vector<std::vector<int>>& queueDown) {
	while (currentCall < callsNumber && timer == callsList[currentCall].moment) {
		addCall(callsList[currentCall], queueUp, queueDown); // if call appeared add call
		++currentCall;
	}
	return;
}

int chooseDirection(int currentFloor, int direction, std::set<int>& passengers, std::vector<std::vector<int>>& queueUp, \
	std::vector<std::vector<int>>& queueDown, int floorsNumber) {
	if (!passengers.empty())
		return (*passengers.begin() > currentFloor) ? 1 : -1; // moving to destination of passengers inside lift
	int cntUp = (int)queueUp[currentFloor].size(), cntDown = (int)queueDown[currentFloor].size();
	// number of passengers wants lift to go up and down
	for (int i = 1; i < currentFloor; ++i)
		cntDown += (int)queueUp[i].size() + (int)queueDown[i].size();
	for (int i = currentFloor + 1; i <= floorsNumber; ++i)
		cntUp += (int)queueUp[i].size() + (int)queueDown[i].size();
	if (!cntUp && !cntDown)
		return 0; // no people in queues
	if (!direction)
		return (cntDown > cntUp) ? -1 : 1; // moving to most people
	if (direction == 1 && cntUp)
		return 1;
	if (direction == -1 && cntDown)
		return -1;
	return -direction; // changing direction
}

void directionTest() {
	std::set<int> ex1 = { 3, 4 };
	std::set<int> ex2 = { 1 };
	std::set<int> ex3 = {};
	std::vector<std::vector<int>> q1 = { {}, {3}, {}, {4}, {} };
	std::vector<std::vector<int>> q2 = { {}, {}, {1}, {1}, {} };
	std::vector<std::vector<int>> q3 = { {}, {}, {}, {}, {} };

	if (!(chooseDirection(1, 1, ex3, q1, q2, 4) == 1 \
		&& chooseDirection(4, 1, ex3, q1, q2, 4) == -1 \
		&& chooseDirection(2, -1, ex2, q3, q2, 4) == -1 \
		&& chooseDirection(2, 1, ex1, q3, q3, 4) == 1 \
		&& chooseDirection(3, 1, ex3, q3, q3, 4) == 0 \
		&& chooseDirection(4, -1, ex3, q3, q2, 4) == -1)) {
		std::cout << "\nwrong chooseDirection\n";
		exit(0);
	}
	return;
}

int moveLift(int currentFloor, int direction) {
	return currentFloor + direction;
}

bool shouldGoIn(std::vector<int>& queue) {
	return !queue.empty();  // returns true if someone wants to go in
}

bool movePassengersOut(int currentFloor, std::set<int>& passengers) {
	bool returnValue = (passengers.find(currentFloor) != passengers.end());
	passengers.erase(currentFloor);
	return returnValue; // returns true if someone go out
}

void movePassengersIn(std::set<int>& passengers, std::vector<int>& queue) {
	for (int destination : queue)
		passengers.insert(destination);
	queue.clear();
	return;
}

void movePassengersTest() {
	std::set<int> p1 = { 2, 4 };
	std::set<int> p2 = { 1 };
	std::set<int> p3 = {};
	std::vector<int> q1 = { 3 }, q2 = { 1 }, q3 = {};

	movePassengersOut(2, p1);
	movePassengersIn(p1, q1);

	movePassengersOut(4, p3);
	movePassengersIn(p3, q2);

	movePassengersOut(2, p2);
	movePassengersIn(p2, q3);

	if (!(p1 == std::set<int>({ 3, 4 }) && q1.empty() \
		&& p3 == std::set<int>({ 1 }) && q2.empty() \
		&& p2 == std::set<int>({ 1 }) && q3.empty())) {
		std::cout << "\nwrong movePassengers\n";
		exit(0);
	}
}

bool isQueuesEmpty(int floorsNumber, std::vector<std::vector<int>>& queueUp, std::vector<std::vector<int>>& queueDown) {
	for (int i = 1; i <= floorsNumber; ++i) {
		if (!queueUp[i].empty() || !queueDown[i].empty())
			return false;
	}
	return true;
}

void readInput(int& floorsNumber, int& callsNumber, std::vector<Call>& callsList) {
	std::cin >> floorsNumber >> callsNumber;
	callsList.resize(callsNumber);
	for (int i = 0; i < callsNumber; ++i)
		std::cin >> callsList[i];
	return;
}

void printCurrentCondition(int timer, int currentFloor, int direction, bool isDoorOpened) {
	std::cout << timer << " --- " << currentFloor << ' ' << "v-^"[direction + 1] << ' ';
	if (isDoorOpened)
		std::cout << "[_]";
	else
		std::cout << "][";
	std::cout << '\n';
	return;
}

int main() {
#pragma warning (disable : 4996)
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	directionTest();
	movePassengersTest();

	int floorsNumber, callsNumber;
	std::vector<Call> callsList;
	readInput(floorsNumber, callsNumber, callsList);

	int currentFloor = 1, direction = 0;
	bool isWaiting = false; // should lift wait for passengers go in or out
	int currentCall = 0;
	std::set<int> passengers; // destination floors of passengers inside lift
	std::vector<std::vector<int>> queueUp(floorsNumber + 1), queueDown(floorsNumber + 1); // queue of passengers on each floor
	for (int timer = 0;; timer++) {
		callsAppearing(currentCall, callsList, callsNumber, timer, queueUp, queueDown);
		if (isWaiting) {
			isWaiting = false;
			movePassengersIn(passengers, (direction == 1) ? queueUp[currentFloor] : queueDown[currentFloor]);
			printCurrentCondition(timer, currentFloor, direction, true);
			continue;
		}
		if (currentCall == callsNumber && passengers.empty() && isQueuesEmpty(floorsNumber, queueUp, queueDown)) {
			printCurrentCondition(timer, currentFloor, 0, false);
			break; // all calls were served
		}
		currentFloor = moveLift(currentFloor, direction);
		bool didGoOut = movePassengersOut(currentFloor, passengers);
		direction = chooseDirection(currentFloor, direction, passengers, queueUp, queueDown, floorsNumber);
		// queue choosing according to direction
		if (didGoOut || shouldGoIn((direction == 1) ? queueUp[currentFloor] : queueDown[currentFloor]))
			isWaiting = true;
		printCurrentCondition(timer, currentFloor, direction, false);
	}

	return 0;
}