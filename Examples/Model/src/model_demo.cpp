#include "model.hpp"

#include <iostream>

#define VOTING_STATE 0

void enterVotingState(Model& model) {
	model.updateData("state", VOTING_STATE);
	model.addData("voting_state|hasplayervoted", 0);
	model.addData("voting_state|playersonquest", 0);
	model.addData("voting_state|votenum", 1);
	model.addData("voting_state|questnum", 1);
	model.addData("voting_state|player0|hasvoted", 0);
}

void exitVotingState(Model& model) {
	model.removeData("voting_state|hasplayervoted");
	model.removeData("voting_state|playersonquest");
	model.removeData("voting_state|votenum");
	model.removeData("voting_state|questnum");
	model.removeData("voting_state|player0|hasvoted");
}

int main() {
	std::cout << "HI!" << std::endl;
}